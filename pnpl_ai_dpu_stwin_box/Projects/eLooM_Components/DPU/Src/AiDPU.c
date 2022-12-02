/**
  ******************************************************************************
  * @file    AiDPU.c
  * @author  STMicroelectronics - AIS - MCD Team
  * @brief
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "AiDPU.h"
#include "AiDPU_vtbl.h"
#include "params.h"
#include "services/sysdebug.h"
#include "pre_processing_app.h"

// set the AIDPU_G_TP_MS_2 as 9.81 if acceleration data inside the DPU are needed in [m/s^2] otherwise set it to 1.0
//#define AIDPU_G_TO_MS_2 (9.81F)
#define AIDPU_G_TO_MS_2 (1.0F)

#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_AI, level, message)

/**
 * Specified the virtual table for the AiDPU_t class.
 */
static const IDPU_vtbl sAiDPU_vtbl = {
    AiDPU_vtblInit,
    ADPU_AttachToSensor_vtbl,
    ADPU_DetachFromSensor_vtbl,
    ADPU_AttachInputADPU_vtbl,
    ADPU_DetachFromADPU_vtbl,
    ADPU_DispatchEvents_vtbl,
    ADPU_RegisterNotifyCallbacks_vtbl,
    AiDPU_vtblProcess,
};


pre_processing_data_t pre_processing_data;


uint32_t tempo_1 = 0;
uint32_t tempo_2 = 0;
uint32_t delta_tempo = 0;
int counter_prova = 0;

/* Private member function declaration */
/***************************************/

/**
 * Check if the DPU is initialized and ready to receive and process data.
 *
 * @param _this [IN] specifies a pointer to the object.
 * @return TRUE if the object is initialized, FALSE otherwise.
 */
static inline boolean_t AiDPUAreStreamsInitialized(AiDPU_t *_this);


/* Public API functions definition */
/***********************************/

IDPU *AiDPUAlloc() {
  IDPU *p_obj = (IDPU*) SysAlloc(sizeof(AiDPU_t));

  if (p_obj != NULL)
  {
    p_obj->vptr = &sAiDPU_vtbl;
  }
  return p_obj;
}

IDPU *AiDPUStaticAlloc(void *p_mem_block)
{
  IDPU *p_obj = (IDPU*)p_mem_block;
  if (p_obj != NULL)
  {
    p_obj->vptr = &sAiDPU_vtbl;
  }

  return p_obj;
}

sys_error_code_t AiDPUSetSensitivity(AiDPU_t *_this, float sensi)
{
  assert_param(_this != NULL);

  _this->scale = sensi * AIDPU_G_TO_MS_2;

  return SYS_NO_ERROR_CODE;
}



uint16_t AiDPUSetStreamsParam(AiDPU_t *_this, uint16_t signal_size, uint8_t axes, uint8_t cb_items)
{
  assert_param(_this != NULL);

  if (!AiDPUAreStreamsInitialized(_this))
  {
    /* DPU has been already initialized, so first reset it, if needed */
  }

  /* DPU converts input data in float */
  _this->super.dpuWorkingStream.packet.payload_type = AI_FMT;
  _this->super.dpuWorkingStream.packet.payload_fmt  = AI_SP_FMT_FLOAT32_RESET();

  /* the shape is 1D the the microphone as 1 axis (X)*/
  _this->super.dpuWorkingStream.packet.shape.n_shape                          = 1 ;
  _this->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH]  = axes;
  _this->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT] = signal_size;

  /* Initialize the out stream */
  _this->super.dpuOutStream.packet.shape.n_shape                         = 1;
  _this->super.dpuOutStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] = 2;
  _this->super.dpuOutStream.packet.payload_type = AI_FMT;
  _this->super.dpuOutStream.packet.payload_fmt  = AI_SP_FMT_FLOAT32_RESET();
  _this->super.dpuOutStream.packet.payload_size = sizeof(_this->ai_out);
  _this->super.dpuOutStream.packet.payload      = (uint8_t*)&_this->ai_out[0];

  /* compute the size in byte of one cb item, */
  _this->super.n_bytes_for_item = axes * signal_size * sizeof(float);
  _this->super.cb_items         = cb_items;

  _this->stream_ready = TRUE;

  return (cb_items * _this->super.n_bytes_for_item);
}

sys_error_code_t AiDPUPrepareToProcessData(AiDPU_t *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  ADPU_Reset((ADPU*)_this);
  _this->ai_out[0] = 0;
  _this->ai_out[1] = 0;

  return res;
}


/* IDPU virtual functions definition */
/*************************************/

sys_error_code_t AiDPU_vtblInit(IDPU *_this) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AiDPU_t *p_obj =(AiDPU_t*)_this;

  res = ADPU_Init_vtbl(_this);
  if (!SYS_IS_ERROR_CODE(res))
  {
    p_obj->ai_out[0] = 0;
    p_obj->ai_out[1] = 0;
    p_obj->stream_ready = FALSE;
    // take the ownership of the Sensor Event IF
    IEventListenerSetOwner((IEventListener *) ADPU_GetEventListenerIF(&p_obj->super), &p_obj->super);

    /* Free signal pre-processing functions */
    pre_processing_free(&pre_processing_data);

    /* Initialize signal pre-processing functions */
    pre_processing_init(&pre_processing_data);

    /* Initialize AI library */
    if (aiInit(NETWORK_NAME)==0)
    {
	  /* set the initial mode to process */
	  p_obj->ai_processing_f = aiProcess;
    }
  }

  return res;
}

sys_error_code_t AiDPU_vtblProcess(IDPU *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  ADPU *super = (ADPU*) _this;
  AiDPU_t *p_obj = (AiDPU_t*) _this;
  CBItem **p_consumer_buff = NULL;
  CircularBuffer *p_circular_buffer = NULL;

  //DPU has the priority
  if(!super->isADPUattached)
  {
    for(int i = 0; i < ADPU_CFG_MAX_SENSOR; i++)
    {
      if(super->sensors[i].sensorIF != NULL)
      { //TODO sensors[i].pConsumerDataBuff could be not necessary
        uint32_t sensor_ready = CB_GetReadyItemFromTail(super->sensors[i].cb_handle.pCircularBuffer, &super->sensors[i].cb_handle.pConsumerDataBuff);
        if(sensor_ready == SYS_CB_NO_READY_ITEM_ERROR_CODE)
        {
//          return SYS_NO_ERROR_CODE;
          return SYS_CB_NO_READY_ITEM_ERROR_CODE;
        }
        p_consumer_buff = &super->sensors[i].cb_handle.pConsumerDataBuff;
        p_circular_buffer = super->sensors[i].cb_handle.pCircularBuffer;
      }
    }
  }
  else
  {
    uint32_t process_ready = CB_GetReadyItemFromTail(super->AttachedAdpu.cb_handle.pCircularBuffer, &super->AttachedAdpu.cb_handle.pConsumerDataBuff);
    if(process_ready == SYS_CB_NO_READY_ITEM_ERROR_CODE)
    {
//      return SYS_NO_ERROR_CODE;
      return SYS_CB_NO_READY_ITEM_ERROR_CODE;
    }
    p_consumer_buff = &super->AttachedAdpu.cb_handle.pConsumerDataBuff;
    p_circular_buffer = super->AttachedAdpu.cb_handle.pCircularBuffer;
  }

  if((*p_consumer_buff) != NULL)
  {

		//tempo_1 = HAL_GetTick();

	 /* if(counter_prova==0)
	  {
		  tempo_1 = HAL_GetTick();
	  }

	if(counter_prova==100)
	  {
		tempo_2 = HAL_GetTick();
		delta_tempo = tempo_2-tempo_1;
	  }
	counter_prova= counter_prova+1;*/

	  /*-----August  experiment for data acquisition
	  assert_param(p_obj->scale != 0.0F);
	  float *p_in = (float*) CB_GetItemData((*p_consumer_buff));
	  float scale = p_obj->scale;

	  float32_t raw_data[INPUT_BUFFER_SIZE];
	  //monodimensional_data_t raw_data[INPUT_BUFFER_SIZE];


	  for(int i = 0; i < INPUT_BUFFER_SIZE; i++)
		  {
			raw_data[i] = *p_in++;
		  }

	  for(int i = 0; i < INPUT_BUFFER_SIZE; i++)  //AL MOMENTO CB size = 16
		  {
			  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE,("%f, \r\n",(float32_t)(raw_data[i])));
			  //p_in=p_in+7; //undersampling to keep up with pace of the acquisition??
			  // previously done as p_in=p_in+4*sizeof(p_in);
		  }


	  CB_ReleaseItem(p_circular_buffer, (*p_consumer_buff));
	  (*p_consumer_buff) = NULL; */

	  /*starting original code--> now modified to accommodate new pre-processing */
	  tridimensional_data_t raw_data[INPUT_BUFFER_SIZE];
	  //assert_param(p_obj->scale != 0.0F); ---> scale parameter is not used with the microphone
	  assert_param(AIDPU_NB_AXIS == p_obj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH]);
	  assert_param(INPUT_BUFFER_SIZE == p_obj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT]);

	  float *p_in = (float*) CB_GetItemData((*p_consumer_buff));
	  float scale = 1;  //Solved problem normalizing

	  /*Modified 24/10 memcopy & return asap
	  float cb_copy[INPUT_BUFFER_SIZE];
	  float *dst= cb_copy;
	  memcpy(dst, p_in, INPUT_BUFFER_SIZE*sizeof(float));
	  // release the buffer as soon as possible
	  CB_ReleaseItem(p_circular_buffer, (*p_consumer_buff));
	  (*p_consumer_buff) = NULL;*/


	  /*Modified on 20/09*/
	  //Since data are monodimensional this loop is not efficient, may basically use a memcpy
	  for(int i = 0; i < INPUT_BUFFER_SIZE; i++)
	  {
		  raw_data[i].x = *p_in++ *scale;
		  raw_data[i].y = 0;
		  raw_data[i].z = 0;
	  }

	 	/*dummy data creation: sine wave as acquired by the sensor
	//Ocio sono 512
	float32_t dummy_data[INPUT_BUFFER_SIZE]={0.0, 63.03224911666324, -5.439143093615382, 75.86400042071924, 6.0999601706873, -6.150047290729713, -49.070181684458554, 7.697022502855882, -98.52307038704039, 58.645241701127894, -19.774769335153064, 33.331293738607336, 42.742540360439094, 8.440094639523746, -23.1709319965405, 2.732505336810645, -34.47810196435401, -39.53812506272409, 62.515279033468225, -44.686847873329796, 72.89694863066798, -3.737510067984555, 3.3283767911982824, -40.402666621020245, 11.190821664196207, -83.82836048134322, 42.731466520330144, 4.869411192746919, 14.982762624068059, 74.77165890603236, -5.462044224128344, -0.5120006617252981, -27.066855569220074, -33.08289586627394, -80.41094047105572, 54.727011313032214, -54.26028495436294, 82.34480290824199, 31.871319594246714, 26.803760888391274, -0.13141672008310934, 4.229361991000726, -75.69601567929037, -13.473519281232434, -6.085700819820939, -40.7419249427093, 84.24457320449521, -11.59892023462071, 40.85984751936899, -3.6455741735967866, 2.541448221175033, -72.21216190698127, 44.71924194887726, -63.32030985353991, 41.403963848011614, 33.042167749375345, -2.51856441859589, 23.014304488603706, -9.02027215116923, -43.180700704285144, -31.59505722696185, 18.956351751065455, -57.11057860290912, 99.15685352006156, -8.973249557445804, 49.352933212256794, 4.897975171486697, -7.407765005086473, -75.74682853479663, 6.085873194167069, -63.495293815059206, 2.910332442870066, 62.51988634603512, -4.780289686447892, 75.93175257541385, 4.763554595283544, -7.380892690529006, -48.786044737198935, 6.422097914257261, -97.81998546115409, 60.15278284541407, -20.57227118222066, 35.07948038710895, 42.26267720641977, 7.8603764931660685, -23.321871539867473, 2.9074919936607273, -35.8938682973784, -37.662092305897254, 61.67004815983644, -44.60806557946769, 73.56314156272072, -4.934567721990888, 3.046523552227848, -39.94165918956244, 10.770163063128255, -83.3690576533918, 44.67256733301207, 3.6383945612313155, 16.494770582221843, 73.7924310332676, -6.68024486324518, -1.1325897271686456, -27.331484570091458, -34.25052619306464, -78.43527228691431, 55.157240567722226, -53.75745303821077, 84.2349535318224, 30.616669855071525, 26.54257511587728, -0.7977035999800304, 2.983999825152331, -76.5648186439132, -11.96767537036658, -7.285929128356509, -38.70529940414076, 84.61680879485245, -11.994118639375559, 41.311810490041, -3.9983457132915365, 1.3473641510713836, -71.50959136099043, 44.70602324091949, -64.08385715014455, 43.25869671251939, 31.587457351757973, -2.265509624104686, 22.851046864484655, -9.600445960045494, -43.5777500274274, -29.87247517093988, 18.117768622937604, -55.549774418864544, 99.7210544420038, -10.249025763278869, 49.63418290893502, 3.625603860962996, -8.685500910037154, -75.58105646898677, 6.720206907224025, -63.908930209240296, 5.818227543783771, 61.95834776660978, -4.109593759688991, 75.94931768331045, 3.4000631737371627, -8.589606901880353, -48.50062265797319, 5.150220062248066, -97.04795086489345, 61.63222879401788, -21.348138722614586, 36.83787860376381, 41.7405636890684, 7.281582678425321, -23.468055487847845, 3.043749388846651, -37.28808256386048, -35.77677358081895, 60.78593396306336, -44.48217672944095, 74.20992075221041, -6.131646982694013, 2.799725113541294, -39.47821028512245, 10.337301336705949, -82.86757557574214, 46.56393416039729, 2.3940138934851634, 18.00890541519656, 72.75906798199912, -7.882183987816992, -1.7304309753451168, -27.59727028403436, -35.373397435494425, -76.41974118009124, 55.55061150464159, -53.218936668424355, 86.07952533864926, 29.3198777609664, 26.283669277771544, -1.4868618382817855, 1.7277786082493762, -77.37743933952758, -10.465862710027901, -8.468792024151238, -36.62300895176777, 84.94420547835757, -12.376093434534218, 41.7571600321113, -4.386861370915232, 0.1562466996261591, -70.79003622799307, 44.648022819024966, -64.80467268965668, 45.10140799043949, 30.115368264061914, -1.9732470615280953, 22.680208783653292, -10.180154985443151, -43.93432292529571, -28.165215458928788, 17.25980120707114, -53.96381679478632, 100.2154644015914, -11.522591162752448, 49.91379789428229, 2.3338827261823987, -9.93174822159122, -75.36755487432404, 7.341879222624806, -64.27311971140028, 8.721250789056912, 61.347827995370174, -3.427343450340304, 75.9159817266359, 2.0110456180386516, -9.775309718327051, -48.21399997749925, 3.8831217937014264, -96.2073903694164, 63.08261479671966, -22.101685517625537, 38.60471780276599, 41.17569787240109, 6.704179957685447, -23.61040486642476, 3.141567471414188, -38.65936982634407, -33.883072731600336, 59.86428442791067, -44.30852146847456, 74.83645705682811, -7.327782286054818, 2.5876336121516497, -39.013696410037284, 9.892610216520023, -82.3248474972895, 48.404338616038174, 1.137659193346062, 19.524526523446923, 71.67235778734204, -9.066105908204904, -2.3056441585182306, -27.86383261584544, -36.450756412736794, -74.36632437717594, 55.90679399205599, -52.64518570652781, 87.8766926424106, 27.981932268455466, 26.0274102823431, -2.198854029495182, 0.46253589538368944, -78.1333045285731, -8.96871003635946, -9.633017144065926, -34.496533446863694, 85.22592815296173, -12.744537765952927, 42.19449876773529, -4.811230416803097, -1.0309093436312953, -70.05428429511065, 44.546125851638834, -65.48154541699955, 46.93117854563274, 28.62730206392588, -1.6417488028490475, 22.50083334006114, -10.758940210592513, -44.25109532201813, -26.474907424044282, 16.383261210824386, -52.353699322869545, 100.63994618470281, -12.792181574997457, 50.191628499417774, 1.023781737873863, -11.145137544724832, -75.10724429273631, 7.950633573104874, -64.58787492668247, 11.61697331765475, 60.68857425862446, -2.733833898370074, 75.83108497217985, 0.5981044993550704, -10.937146380388757, -47.92624585880169, 2.622522177293984, -95.29879924561932, 64.50298547555794, -22.83225771468539, 40.37819766318628, 40.56762476647026, 6.128636773404427, -23.749827831732446, 3.2013006078234625, -40.0063661353912, -31.981889245389706, 58.90647676004414, -44.08649973260492, 75.44191297030903, -8.522017542090502, 2.4098435585871094, -38.54948329362587, 9.4364802924392, -81.74182762319336, 50.19261830594091, -0.12925416891683028, 21.040990504383863, 70.53313944189651, -10.23028162193934, -2.8583881572777745, -28.130789498744846, -37.48191043571486, -72.27703110553963, 56.22548970198164, -52.03667794941148, 89.62467327137965, 26.603879304199083, 25.77416035312092, -2.9336036169071544, -0.809876654743025, -78.83189710567676, -7.476842908117153, -10.777365310775265, -32.327412049067014, 85.46116979563503, -13.099161246801426, 42.6224296695714, -5.271500882125187, -2.213104193881138, -69.30311122585651, 44.40126999902428, -66.11330336133949, 48.74708603536969, 27.12466272156809, -1.2710530265578157, 22.31195871334689, -11.33634474746491, -44.52878294887371, -24.803139777875657, 15.488990128301316, -50.72042072864911, 100.99443389277995, -14.056031030291372, 50.467507972189246, -0.3037097774920561, -12.32435142179479, -74.8010938769657, 8.546222213247145, -64.85325913931754, 14.502974737785493, 59.98088658041883, -2.029366909570827, 75.69402288171035, -0.8371167759278322, -12.074288738984173, -47.63741450667141, 1.370124177471368, -94.32274402165649, 65.89239572011054, -23.53923445019522, 42.156490456787694, 39.91593760176884, 5.555423117786529, -23.887218180236598, 3.2233669543179317, -41.32772027647099, -30.074118069269215, 57.91391532847659, -43.815572475853855, 76.0254434863987, -9.713406941941237, 2.2658928125036786, -38.086923688468424, 8.969319019622198, -81.11948967536722, 51.92767789398536, -1.405287174412452, 22.55765120027899, 69.34230168234639, -11.373011232550521, -3.3888607947130662, -28.39775739647466, -38.4662277544207, -70.15390014884764, 56.506432802584264, -51.39391828609409, 91.32173070763176, 25.18682080725087, 25.524276451651048, -3.690995065437722, -2.0875958687624263, -79.47275694056758, -5.990883600504123, -11.900631929394313, -30.117241615801774, 85.64915285600631, -13.439689814970249, 43.03955828283598, -5.767659099380047, -3.3893340497686495, -68.53727992375862, 44.21444373763369, -66.6988155021728, 50.548205192192704, 25.608854925252096, -0.861264062192804, 22.112619845422365, -11.911913890508126, -44.768139781136426, -23.151458605866637, 14.57785852430029, -49.064984076925526, 101.27893263020059, -15.312374215794318, 50.74125220930162, -1.6475843964315695, -13.468125922818473, -74.4501200407695, 9.128406604328388, -65.06938573752666, 17.376845931832676, 59.22511790263528, -1.3142506289503402, 75.50424694506951, -2.2929360534084857, -13.185936395810705, -47.34754559615968, 0.1276123601288326, -93.27986214681074, 67.2499115962711, -24.222028200124576, 43.93774341829979, 39.22027904678887, 4.985010387564696, -24.023453897533408, 3.208247734525214, -42.62209552033627, -28.160649441051227, 56.88802958706087, -43.495262814772396, 76.5861969986201, -10.90101573380625, 2.155263638762756, -37.627355181575766, 8.491550699701264, -80.4588254531981, 53.60849007297057, -2.688978919869067, 24.073859738796394, 68.10078172144298, -12.492626327432115, -3.8972985893374474, -28.6643517901459, -39.40313791898251, -67.99899736296199, 56.7493906196732, -50.717437835268115, 92.96617616487337, 23.731913688643857, 25.27810969091493, -4.470874091940199, -3.368749662946857, -80.05548165284182, -4.5114509873146265, -13.001648324376806, -27.86767501893987, 85.78913063431212, -13.765865571408451, 43.44449493865943, -6.2996293332849, -4.558592097611863, -67.75753993530137, 43.986684618610795, -67.23699359281406, 52.333608124474175, 24.08128242696246, -0.4125523366535848, 21.901850139990987, -12.485195160071047, -44.969956436029825, -21.521365427240198, 13.650765266919342, -47.38839599738982, 101.49351810151379, -16.559448929077025, 51.01265951635397, -3.0068178659069584, -14.575252155494134, -74.05538504107385, 9.696957804149562, -65.23641757867175, 20.236191845766683, 58.421674135921094, -0.588799225275185, 75.2612654351013, -3.76763457825679, -14.271317818655685, -47.05666471874809, -1.1033493667738856, -92.17086156175661, 68.57461126740948, -24.880085078608577, 45.720081154713995, 38.48034236641698, 4.417871223392165, -24.15939574775667, };
	for(int i = 0; i < INPUT_BUFFER_SIZE; i++)
	{
	  raw_data[i].x = dummy_data[i];
	  raw_data[i].y = 0;
	  raw_data[i].z = 0;
	}*/


	/*dummy data creation: ramp from 0 to 512
	for(int i = 0; i < INPUT_BUFFER_SIZE; i++)
	{
	  raw_data[i].x = i;
	  raw_data[i].y = 0;
	  raw_data[i].z = 0;
	}*/

	if(counter_prova%3 == 0)
	{
    /*dummy data creation: sin440 hz as acquired by the firmware*/
    float32_t preprocessing_output_array[INPUT_BUFFER_SIZE/2];  //ocio qua

    //old data_output_size
    //AI_NETWORK_IN_1_SIZE
    //override data_size_out to align with the mfccs
    //int DATA_OUTPUT_SIZE = 128;

    //call preprocessing function
    pre_processing_process(raw_data, INPUT_BUFFER_SIZE, preprocessing_output_array, AI_NETWORK_IN_1_SIZE, &pre_processing_data);


    /*cut input samples number:
    float32_t *src = preprocessing_output_array;
    float32_t preprocessing_output_array_cut[200];
    float32_t *dst_1= preprocessing_output_array_cut;

    memcpy(dst_1, src, 200*sizeof(float));  //ocio qua */

    // call Ai library.
    p_obj->ai_processing_f(NETWORK_NAME, (float*)preprocessing_output_array, p_obj->ai_out);

    //SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("[AiDPU_vtblProcess()] label_id: %d, accuracy: %f.\r\n", (int) (p_obj->ai_out[0]), (float) (p_obj->ai_out[1])));

	}

    // release the buffer as soon as possible
    CB_ReleaseItem(p_circular_buffer, (*p_consumer_buff));
	(*p_consumer_buff) = NULL;

    //tempo_2 = HAL_GetTick();
   	//delta_tempo = tempo_2-tempo_1;
   	counter_prova = counter_prova +1;
    //ProcessEvent evt_acc;
    //ProcessEventInit((IEvent*) &evt_acc, super->pProcessEventSrc, (ai_logging_packet_t*) &super->dpuOutStream, ADPU_GetTag(super));
    //IDPU_DispatchEvents(_this, &evt_acc);




	/* ORIGINAL CODE:
	  	tridimensional_data_t raw_data[INPUT_BUFFER_SIZE];

	      assert_param(p_obj->scale != 0.0F);
	      assert_param(AIDPU_NB_AXIS == p_obj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH]);
	      assert_param(INPUT_BUFFER_SIZE == p_obj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT]);

	      float *p_in = (float*) CB_GetItemData((*p_consumer_buff));
	      float scale = p_obj->scale;

	      for(int i = 0; i < INPUT_BUFFER_SIZE; i++)
	      {
	        raw_data[i].x = *p_in++ * scale;
	        raw_data[i].y = *p_in++ * scale;
	        raw_data[i].z = *p_in++ * scale;
	      }


	       float32_t preprocessing_output_array[AI_NETWORK_IN_1_SIZE];

	      // call preprocessing function
	       pre_processing_process(raw_data, INPUT_BUFFER_SIZE, preprocessing_output_array, AI_NETWORK_IN_1_SIZE, &pre_processing_data);
	      // call Ai library.
	      p_obj->ai_processing_f(NETWORK_NAME, (float*) preprocessing_output_array, p_obj->ai_out);
	      SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("[AiDPU_vtblProcess()] label_id: %d, accuracy: %f.\r\n", (int) (p_obj->ai_out[0]), (float) (p_obj->ai_out[1])));

	      // release the buffer as soon as possible
	   	CB_ReleaseItem(p_circular_buffer, (*p_consumer_buff));
	  	(*p_consumer_buff) = NULL;

	      ProcessEvent evt_acc;
	      ProcessEventInit((IEvent*) &evt_acc, super->pProcessEventSrc, (ai_logging_packet_t*) &super->dpuOutStream, ADPU_GetTag(super));
	      IDPU_DispatchEvents(_this, &evt_acc);
	  */

  }
  return xRes;
}

/* Private member function definition */
/**************************************/

static inline boolean_t AiDPUAreStreamsInitialized(AiDPU_t *_this)
{
  assert_param(_this != NULL);

  return _this->stream_ready;
}
