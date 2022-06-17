/**
 ******************************************************************************
 * @file    AiUSC_DPU.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version 1.0.1
 * @date    10-Dec-2021
 *
 * @brief
 *
 * <DESCRIPTIOM>
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under ODE SOFTWARE LICENSE AGREEMENT
 * SLA0094, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0094
 *
 ******************************************************************************
 */

#include "AiUSC_DPU.h"
#include "AiUSC_DPU_vtbl.h"
#include "services/sysdebug.h"
#include <stdio.h>

/**
 * Specified the virtual table for the AiUSC_DPU_t class.
 */
static const IDPU_vtbl sAiUSC_DPU_vtbl = {
    AiUSC_DPU_vtblInit,
    ADPU_AttachToSensor_vtbl,
    ADPU_DetachFromSensor_vtbl,
    ADPU_AttachInputADPU_vtbl,
    ADPU_DetachFromADPU_vtbl,
    ADPU_DispatchEvents_vtbl,
    ADPU_RegisterNotifyCallbacks_vtbl,
    AiUSC_DPU_vtblProcess,
};


/* Inline functions definition */
/*******************************/


/* GCC requires one function forward declaration in only one .c source
 * in order to manage the inline.
 * See also http://stackoverflow.com/questions/26503235/c-inline-function-and-gcc
 */
#if defined (__GNUC__) || defined (__ICCARM__)
extern EAiUSC_Mode_t AiUSC_DPUGetProcessingMode(AiUSC_DPU_t *_this);
#endif


/* Private member function declaration */
/***************************************/

/**
 * Check if the DPU is initialized and ready to receive and process data.
 *
 * @param _this [IN] specifies a pointer to the object.
 * @return TRUE if the object is initialized, FALSE otherwise.
 */
static inline boolean_t AiUSC_DPUAreStreamsInitialized(AiUSC_DPU_t *_this);


/* Public API functions definition */
/***********************************/

IDPU *AiUSC_DPUAlloc() {
  IDPU *p_obj = (IDPU*) pvPortMalloc(sizeof(AiUSC_DPU_t));

  if (p_obj != NULL)
  {
    p_obj->vptr = &sAiUSC_DPU_vtbl;
  }

  return p_obj;
}

IDPU *AiUSC_DPUStaticAlloc(void *p_mem_block)
{
  IDPU *p_obj = (IDPU*)p_mem_block;
  if (p_obj != NULL)
  {
    p_obj->vptr = &sAiUSC_DPU_vtbl;
  }

  return p_obj;
}

sys_error_code_t AiUSC_DPUSetProcessingMode(AiUSC_DPU_t *_this, EAiUSC_Mode_t mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (mode == E_AI_USC_DETECTION)
  {
    _this->ai_processing_f = aiProcess ;
  }
  else
  {
    _this->ai_processing_f = NULL;
  }

  return res;
}

uint16_t AiUSC_DPUSetStreamsParam(AiUSC_DPU_t *_this, uint16_t signal_size, uint8_t axes, uint8_t cb_items)
{
  assert_param(_this != NULL);

  if (!AiUSC_DPUAreStreamsInitialized(_this))
  {
    /* DPU has been already initialized, so first reset it, if needed */
  }

  /* DPU converts input data in float */
  _this->super.dpuWorkingStream.packet.payload_type = AI_FMT;
  _this->super.dpuWorkingStream.packet.payload_fmt  = AI_SP_FMT_FLOAT32_RESET();

  /* the shape is 2D  */
  _this->super.dpuWorkingStream.packet.shape.n_shape                          = 2 ;
  _this->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH]  = signal_size;
  _this->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT] = axes;

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


sys_error_code_t AiUSC_DPUPrepareToProcessData(AiUSC_DPU_t *_this)
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

sys_error_code_t AiUSC_DPU_vtblInit(IDPU *_this) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AiUSC_DPU_t *p_obj =(AiUSC_DPU_t*)_this;

  res = ADPU_Init_vtbl(_this);
  if (!SYS_IS_ERROR_CODE(res)) {
    p_obj->ai_out[0] = 0;
    p_obj->ai_out[1] = 0;
    p_obj->stream_ready = FALSE;
    // take the ownership of the Sensor Event IF
    IEventListenerSetOwner((IEventListener *) ADPU_GetEventListenerIF(&p_obj->super), &p_obj->super);

    /* initialize AI library */
    if (aiInit(AI_USC_DPU_NAME)==0)
    {
	  /* set the initial mode to process */
	  p_obj->ai_processing_f = aiProcess;
    }
  }

  return res;
}

// static float test_in [32*46]={ 4.6561694336e+02,4.7585165405e+02,4.9248321533e+02,4.7125222778e+02,4.7223052979e+02,4.6614813232e+02,4.8008328247e+02,4.6628192139e+02,4.7422833252e+02,4.8335815430e+02,4.7881759644e+02,4.7954415894e+02,4.7710293579e+02,4.6899050903e+02,4.6756262207e+02,4.6969882202e+02,4.7315316772e+02,4.7889315796e+02,4.6731115723e+02,4.8187182617e+02,4.7861434937e+02,4.7827392578e+02,4.8225372314e+02,4.7256423950e+02,4.7532785034e+02,4.7380874634e+02,4.8666372681e+02,4.7568658447e+02,4.7477755737e+02,4.6548959351e+02,4.8420809937e+02,4.6449887085e+02,4.7980291748e+02,4.7069119263e+02,4.7787091064e+02,4.7460748291e+02,4.7630346680e+02,4.7885723877e+02,4.9679479980e+02,4.7371832275e+02,4.7799588013e+02,4.9813586426e+02,4.5961422729e+02,4.8189675903e+02,4.7910717773e+02,4.8487203979e+02,1.3808270264e+02,1.4215866089e+02,1.3061758423e+02,1.3337672424e+02,1.4553254700e+02,1.2594042206e+02,1.4042022705e+02,1.2697894287e+02,1.2617150879e+02,1.5358209229e+02,1.4575601196e+02,1.4589492798e+02,1.4352905273e+02,1.3506790161e+02,1.3732092285e+02,1.4040235901e+02,1.3382020569e+02,1.5214849854e+02,1.3418492126e+02,1.5186599731e+02,1.4258047485e+02,1.4811233521e+02,1.4193128967e+02,1.3363055420e+02,1.4087272644e+02,1.3438662720e+02,1.4530619812e+02,1.4692193604e+02,1.4417608643e+02,1.3839749146e+02,1.4970367432e+02,1.2062022400e+02,1.4206204224e+02,1.4706417847e+02,1.4147723389e+02,1.4820480347e+02,1.4161309814e+02,1.4492419434e+02,1.3446475220e+02,1.3702679443e+02,1.3989636230e+02,1.3497351074e+02,1.2565406036e+02,1.3792208862e+02,1.4612359619e+02,1.4715969849e+02,1.1333923340e+01,5.5256328583e+00,-2.9262685776e+00,2.2277030945e+00,5.3385334015e+00,-6.0250253677e+00,-1.4447746277e+00,8.5837215185e-01,-4.3681435585e+00,1.6013904572e+01,1.1785570145e+01,7.8790550232e+00,7.1463689804e+00,-6.9052209854e+00,4.4332919121e+00,-1.4408940077e-01,2.5804693699e+00,1.3110420227e+01,1.1773076057e+00,1.0763178825e+01,5.8609981537e+00,4.0255436897e+00,-1.1406706572e+00,-1.8187613487e+00,5.2658200264e+00,-3.8330888748e-01,2.9520094395e-03,1.2205106735e+01,6.8248634338e+00,4.0711832047e+00,8.5431079865e+00,-1.2023372650e+01,7.5362038612e-01,1.0632382393e+01,9.9734916687e+00,6.7131328583e+00,1.0326337814e+01,8.1165790558e+00,-1.1199623108e+01,-4.5919504166e+00,-1.8046391010e+00,-7.2482442856e+00,-3.1332695484e+00,-2.3474059105e+00,6.1841435432e+00,4.7398810387e+00,4.2884368896e+01,3.7365856171e+01,4.7019500732e+01,4.2855754852e+01,3.0350307465e+01,3.5946914673e+01,3.0780708313e+01,4.1869514465e+01,4.4660972595e+01,4.3567417145e+01,4.5088783264e+01,4.2356151581e+01,4.3136222839e+01,2.1158418655e+01,4.0831390381e+01,3.0015531540e+01,3.9111232758e+01,3.9953441620e+01,3.1445816040e+01,4.1543159485e+01,4.9486061096e+01,3.4647750854e+01,3.2822818756e+01,3.4979293823e+01,4.4743507385e+01,4.4904846191e+01,3.7809173584e+01,4.3304832458e+01,3.6623126984e+01,3.7652973175e+01,4.2055290222e+01,3.1437431335e+01,3.2798774719e+01,4.1000720978e+01,4.3892066956e+01,3.0242975235e+01,4.3408424377e+01,4.0253009796e+01,4.7797393799e+01,3.2747848511e+01,3.5740909576e+01,5.4648292542e+01,4.0298034668e+01,4.4522544861e+01,4.2352306366e+01,4.0626979828e+01,-1.0185688019e+01,-2.5861740112e+01,-2.3122426987e+01,-2.2131113052e+01,-3.0064853668e+01,-2.1175628662e+01,-3.3686683655e+01,-2.2751033783e+01,-2.5826129913e+01,-2.6931571960e+01,-2.1855175018e+01,-2.4212940216e+01,-2.1185441971e+01,-4.0467353821e+01,-2.1900474548e+01,-3.0911693573e+01,-2.5482433319e+01,-2.5616674423e+01,-2.1949806213e+01,-2.6443304062e+01,-1.6171781540e+01,-1.8470966339e+01,-2.7538997650e+01,-2.9953125000e+01,-2.2743146896e+01,-1.6611690521e+01,-2.5669000626e+01,-1.7257116318e+01,-2.4723588943e+01,-1.8380043030e+01,-2.4210176468e+01,-2.5726634979e+01,-2.9068374634e+01,-1.6517263412e+01,-2.3247695923e+01,-2.7662841797e+01,-2.1729640961e+01,-2.6639595032e+01,-1.7333259583e+01,-2.4251060486e+01,-3.0876955032e+01,-2.1541898727e+01,-1.4318704605e+01,-1.5970834732e+01,-1.6596946716e+01,-2.1460348129e+01,1.5418053627e+01,8.5301485062e+00,7.9654755592e+00,8.1369600296e+00,2.7401976585e+00,1.1662408829e+01,1.6979043484e+00,8.0928726196e+00,6.9644527435e+00,7.6366835833e-01,9.9426898956e+00,4.6200308800e+00,1.1002768517e+01,-5.8302617073e+00,4.7965779305e+00,-3.0917024612e-01,1.1201660156e+01,6.6879349947e-01,9.0843238831e+00,3.0114455223e+00,8.3915653229e+00,1.3922941208e+01,8.9638938904e+00,3.5655436516e+00,9.4704914093e+00,7.2323455811e+00,8.4844055176e+00,1.1214364052e+01,2.9005479813e+00,1.0925710678e+01,8.7467508316e+00,8.5653848648e+00,3.1211795807e+00,9.5489292145e+00,4.9977383614e+00,1.9045828581e+00,6.4831523895e+00,2.9958581924e+00,1.2008249283e+01,1.1498439789e+01,8.2796311378e-01,2.6055617332e+00,1.2543590546e+01,2.8077058792e+00,1.1117327690e+01,5.5023007393e+00,8.6408805847e+00,5.7291588783e+00,1.2327613831e+00,8.7100133896e+00,9.2277498245e+00,5.9114904404e+00,7.9291534424e+00,6.9583261013e-01,5.2356290817e+00,3.2822189331e+00,8.4185218811e+00,1.8337047100e+00,8.7072162628e+00,-8.8673591614e+00,-2.2048633099e+00,4.9149255753e+00,8.8770284653e+00,5.2746515274e+00,1.1001793146e+00,2.0799584389e+00,7.7471928596e+00,9.4184617996e+00,1.2835489273e+01,3.3586802483e+00,9.8426399231e+00,5.0208058357e+00,6.8955397606e+00,9.9610042572e+00,5.5667996407e-02,2.5197577477e+00,1.1728981018e+01,8.6197662354e+00,2.9362088442e-01,5.2935495377e+00,6.5652303696e+00,4.8740959167e+00,2.3986177444e+00,1.5697878599e+00,4.9831533432e+00,7.9866223335e+00,1.5219197273e+00,1.4267520905e+00,9.6376523972e+00,1.9454015493e+00,4.6095943451e+00,6.1847190857e+00,1.2126152992e+01,1.2063968182e+00,1.4149288177e+01,9.7572975159e+00,1.5201431274e+01,-1.9960296154e+00,8.4891338348e+00,-4.6571749449e-01,4.9572525024e+00,5.5648903847e+00,6.2524476051e+00,3.3723452687e-01,5.0999455452e+00,-9.4545116425e+00,3.2126927376e+00,1.0047979355e+01,6.4640688896e+00,9.0877208710e+00,4.7454619408e+00,6.8268136978e+00,2.5738708973e+00,8.3302459717e+00,1.3691219330e+01,5.2047783136e-01,2.0774714947e+00,5.7181587219e+00,3.5989170074e+00,1.2080578804e+01,4.1432380676e+00,1.3254022598e+00,1.5228971481e+01,7.6772480011e+00,7.3484802246e+00,5.2751159668e+00,9.2677202225e+00,7.6409540176e+00,2.4649908543e+00,-1.2364855409e-01,3.0818290710e+00,5.2476806641e+00,3.9398741722e+00,1.3733413696e+01,1.2548449516e+01,6.8632650375e+00,2.0175347328e+00,7.4458370209e+00,1.5034009933e+01,1.0919908524e+01,2.3123336792e+01,1.3995323181e+01,1.3389741898e+01,5.7210435867e+00,1.5107575417e+01,4.0939555168e+00,1.2793658257e+01,1.0386051178e+01,1.1064092636e+01,6.0498600006e+00,9.0285158157e+00,5.3753561974e+00,1.1963840485e+01,9.4576616287e+00,1.1447713852e+01,1.0541215897e+01,9.0166540146e+00,1.7248725891e+01,5.5027160645e+00,1.0023588181e+01,2.1119098663e+01,3.8629069328e+00,9.2356452942e+00,1.2682572365e+01,1.1866868973e+01,1.2264969826e+01,8.1730594635e+00,8.7875947952e+00,1.5888101578e+01,9.8070716858e+00,1.2134964943e+01,9.1185073853e+00,1.8681854248e+01,1.0525270462e+01,1.2297056198e+01,7.5696272850e+00,7.0825471878e+00,1.1658159256e+01,1.1169586182e+01,1.9432735443e+01,1.2246378899e+01,9.9459362030e+00,6.5138959885e+00,1.0514225006e+01,1.5731349945e+01,1.5859764099e+01,1.3964734077e+01,1.6617847443e+01,1.5363000870e+01,7.6248798370e+00,1.8358362198e+01,7.6823906898e+00,1.3973137856e+01,1.0621757507e+01,1.3742538452e+01,1.2280694962e+01,1.7346183777e+01,1.1661236763e+01,1.6533325195e+01,9.7864046097e+00,1.2253717422e+01,9.9566822052e+00,1.2142855644e+01,2.3063690186e+01,1.2999853134e+01,1.3252006531e+01,2.2813751221e+01,5.6202526093e+00,1.2904711723e+01,1.3971477509e+01,1.4683858871e+01,1.2077129364e+01,9.4769392014e+00,1.2421340942e+01,1.3951332092e+01,1.6642143250e+01,1.0080675125e+01,1.0248311996e+01,1.8581985474e+01,1.1029537201e+01,1.4292916298e+01,1.1232638359e+01,1.2562129974e+01,1.3993021011e+01,1.0766837120e+01,7.4864597321e+00,9.8034629822e+00,1.2105335236e+01,7.6603956223e+00,9.7332000732e+00,4.0327305794e+00,8.9996111393e-01,8.9515514374e+00,3.2294015884e+00,1.7090545893e+00,-4.4202985764e+00,4.7072863579e+00,2.0466482639e+00,4.0260906219e+00,2.6070687771e+00,-3.1396360397e+00,7.1796649694e-01,7.0098872185e+00,-1.7959833145e-01,5.5261588097e-01,-2.5236086845e+00,1.1668491364e+00,-5.7616367340e+00,1.6838581562e+00,9.5974082947e+00,1.6844606400e+00,4.7122783661e+00,5.1076335907e+00,-7.1180496216e+00,-6.6519570351e-01,-2.0703725815e+00,-2.9435708523e+00,-3.1102986336e+00,-2.9479575157e-01,2.3113527298e+00,1.8678128719e-01,1.1289594650e+01,9.2889860272e-02,-2.6176185608e+00,-1.5503193140e+00,-3.4112215042e-02,-5.2150440216e-01,-9.0639543533e-01,4.1195034981e+00,-3.9733450413e+00,-3.4591064453e+00,-3.1451652050e+00,1.2495622635e+00,2.8469290733e+00,-5.2014036179e+00,-6.4924411774e+00,1.2030893326e+01,5.8358759880e+00,1.6828094482e+01,4.9791264534e+00,1.3212139130e+01,3.1966373920e+00,7.7977313995e+00,8.5596199036e+00,1.2134983063e+01,1.3041934967e+01,2.3038604259e+00,9.2824974060e+00,9.9963741302e+00,7.7410430908e+00,5.3645858765e+00,5.4246325493e+00,8.9430828094e+00,2.3273301125e+00,8.4792375565e+00,1.1205215454e+01,8.8911485672e+00,9.9237251282e+00,1.0352977753e+01,3.6700072289e+00,7.5100107193e+00,5.1615619659e+00,5.1026859283e+00,3.7839028835e+00,6.8095493317e+00,1.0035242081e+01,1.0198812485e+01,1.0929824829e+01,8.1958255768e+00,7.8070287704e+00,2.6912629604e+00,1.0727937698e+01,7.8975515366e+00,8.7739000320e+00,6.0542087555e+00,-1.0455358028e-01,6.2952260971e+00,1.0556438446e+01,8.7020292282e+00,1.2037369728e+01,-3.6093574762e-01,6.7747986317e-01,-2.0703601837e+00,-7.6501607895e+00,-7.1945099831e+00,-1.0063799858e+01,-1.8464021385e-01,-1.1321667671e+01,-4.9743480682e+00,-6.6547341347e+00,-9.5792026520e+00,-6.3487811089e+00,-6.5260400772e+00,-2.8333237171e+00,-7.7090868950e+00,-4.8031907082e+00,-6.4564700127e+00,-6.9128699303e+00,-5.0487260818e+00,-6.8368196487e+00,-6.5274701118e+00,-6.0073347092e+00,-4.3426527977e+00,-9.7023859024e+00,-2.7042555809e+00,-3.3338756561e+00,-6.6531543732e+00,-5.3053302765e+00,-6.7450933456e+00,-7.3082871437e+00,-1.0209205627e+01,-2.8020896912e+00,-4.1925554276e+00,-4.0809559822e+00,-5.4527816772e+00,-8.9278945923e+00,-6.5794901848e+00,-2.7280306816e+00,-6.9825258255e+00,-3.3772504330e+00,2.7009320259e+00,-1.7061344147e+01,-6.7580814362e+00,-1.5545335770e+01,-6.6906781197e+00,-9.4221458435e+00,-1.2415559769e+01,-1.2178831100e+01,-1.9001923800e+00,-1.9504588842e+00,-6.7940340042e+00,1.3860564232e+00,7.1341457367e+00,-5.2602152824e+00,1.5952587128e+00,-1.1978293657e+00,-2.3793628216e+00,3.1767612696e-01,2.3389284611e+00,1.6393053532e+00,-6.0081105232e+00,3.9733629227e+00,1.8935866356e+00,-1.2232028246e+00,3.0052456856e+00,-1.6292672157e+00,6.7109435797e-01,-1.2440671921e+00,-5.6590777636e-01,-3.5315656662e+00,2.2327718735e+00,5.6108856201e+00,2.5277717113e+00,-1.4844870567e+00,-3.6493763924e+00,-9.7473788261e-01,-5.4637570381e+00,3.9441859722e+00,-2.4580788612e+00,-5.1753723621e-01,-1.7428631783e+00,5.1233392954e-01,1.1312080622e+00,-1.8138309717e+00,-2.7452272177e-01,1.3839066029e-02,8.5969638824e+00,-1.0149723053e+01,-2.4843823910e+00,-8.1502399445e+00,2.7198989391e+00,-5.2350473404e+00,-2.9920291901e+00,-7.6592330933e+00,-1.0229646683e+01,-3.7447397709e+00,-7.8950037956e+00,-1.6257896423e+00,6.7353510857e+00,-1.1166396141e+00,-3.7172708511e+00,-4.3846459389e+00,-6.0612088442e-01,2.1165895462e+00,1.9294714928e+00,5.7127428055e-01,-5.7853279114e+00,5.2096481323e+00,7.8494060040e-01,-2.7702527046e+00,-3.0476887226e+00,-4.1070365906e+00,-4.6645350456e+00,-5.5807981491e+00,-2.1194984913e+00,-2.9963197708e+00,-4.2125955224e-01,9.8984992504e-01,2.1427633762e+00,-2.5558152199e+00,-2.4743223190e+00,-2.5349004269e+00,-1.1773037910e+00,5.1212286949e+00,-4.3827133179e+00,-7.2098584175e+00,-4.7109489441e+00,-2.3400523663e+00,-2.7310357094e+00,-5.4517393112e+00,-3.8300027847e+00,-4.8079385757e+00,-3.1662507057e+00,-9.2919998169e+00,-6.9987087250e+00,3.8106026649e+00,4.0552949905e+00,2.0599267483e+00,3.5666832924e+00,-8.9504356384e+00,-1.2397850037e+01,-1.6708598137e+00,-3.9280767441e+00,-7.7018527985e+00,4.2065491676e+00,-1.1648944616e+00,-3.6471166611e+00,-2.9311244488e+00,7.2896748781e-01,3.1639847755e+00,5.5589771271e+00,-1.5264420211e-01,-1.8176441193e+00,2.7510609627e+00,2.7099866867e+00,-1.0405890942e+00,-1.0173578262e+01,-4.6323448420e-01,-7.1199264526e+00,-4.6472387314e+00,6.7905020714e-01,-5.5429215431e+00,-4.7262406349e-01,-5.3278150558e+00,-6.3966631889e-01,-1.4109287262e+00,1.5786585212e-01,-4.0064091682e+00,1.4587969780e+00,-1.3555402756e+00,-2.1532037258e+00,-6.4302310944e+00,-3.5339226723e+00,-8.2951564789e+00,-7.4928960800e+00,-5.5052547455e+00,-6.4925088882e+00,-4.9030456543e+00,7.0062832832e+00,-1.3032087088e+00,-6.1634130478e+00,-4.3878197670e+00,1.2048876286e-01,3.1351335049e+00,5.6092100143e+00,-4.6654405594e+00,-9.4907236099e-01,9.7384095192e-01,6.5735363960e-01,1.0774798393e+00,1.0027194977e+01,7.3715028763e+00,5.0182285309e+00,4.1502008438e+00,3.8406229019e+00,1.0176795959e+01,1.2027439117e+01,1.6879990101e+00,3.2148034573e+00,6.9013886452e+00,6.1370944977e+00,7.4059271812e+00,-3.8988943100e+00,6.2422914505e+00,7.6656091213e-01,2.6557633877e+00,3.2938992977e+00,8.8888359070e-01,4.3928933144e+00,4.0457592010e+00,6.9913110733e+00,2.3120937347e+00,2.8638134003e+00,4.8773355484e+00,5.7531485558e+00,4.1518888474e+00,7.8658533096e+00,-4.8331422806e+00,2.1068136692e+00,-1.1649701595e+00,-7.2509109974e-02,3.2819881439e+00,2.5037040710e+00,2.4790329933e+00,1.1128776550e+01,7.8003206253e+00,7.9081406593e+00,-1.0825049877e+00,4.6246328354e+00,1.8182512522e+00,1.1105608940e+01,1.9984843731e+00,-3.2610449791e+00,-4.4350786209e+00,-7.1893639565e+00,-1.8498178720e+00,4.3568639755e+00,7.4654567242e-01,5.3041801453e+00,-2.2815213203e+00,1.1370325089e+00,-2.7954044342e+00,8.3062505722e+00,-4.8348960876e+00,7.8401118517e-01,2.3600912094e+00,-1.8251379728e+00,1.4523334503e+00,-2.8259885311e+00,-9.6595865488e-01,-1.4266246557e+00,-2.0164527893e+00,-3.7169952393e+00,-1.6890524626e+00,-2.9533019066e+00,2.2582173347e-02,8.1763631105e-01,1.2577106953e+00,-2.3398537636e+00,2.2344870567e+00,-3.8548350334e+00,1.3784475327e+00,6.3032072783e-01,-1.0587780476e+00,-5.6290620565e-01,-2.4682855606e+00,-3.6383733749e+00,1.1451200247e+00,-2.5764124393e+00,-6.9797348976e-01,-3.0104029179e+00,1.1303372383e+00,-1.2163246870e+00,1.2826720476e+00,2.4322476387e+00,-3.3679780960e+00,1.0994331360e+01,2.1930503845e+00,2.3789119720e-01,-1.7948532104e+00,-7.0208442211e-01,-3.7739419937e-01,4.7358303070e+00,3.1182599068e-01,7.3544263840e+00,2.2167482376e+00,3.1265003681e+00,-6.6196856499e+00,7.7414569855e+00,-3.4498455524e+00,3.1237020493e+00,3.2925567627e+00,-2.1640968323e+00,8.7840193510e-01,-7.6209861040e-01,2.5870156288e+00,4.3845171928e+00,-2.9005784988e+00,-9.1734743118e-01,6.1458067894e+00,-2.1528873444e+00,2.2526879311e+00,1.8830348253e+00,6.5249335766e-01,1.9385790825e+00,8.1183457375e-01,-1.1335262060e+00,2.2819671631e+00,-3.4979887009e+00,9.0931396484e+00,9.7613388300e-01,-2.0726599693e+00,-1.0831604004e+00,-3.0991394520e+00,-1.2446932793e+00,-1.8227568865e+00,2.1887093782e-02,1.6363477707e+00,-3.7567112446e+00,1.0533100367e+00,6.1932644844e+00,1.7539784908e+00,1.1105903625e+01,5.7394561768e+00,3.5669529438e+00,-6.7524676323e+00,3.1749701500e+00,8.4129083157e-01,2.8803768158e+00,1.1887934208e+00,6.7705831528e+00,4.3322830200e+00,-2.0775685310e+00,1.7717018127e+00,4.2890214920e+00,-7.2718782425e+00,-1.9257943630e+00,-1.2214105129e+00,-2.7324485779e+00,-2.1321525574e+00,5.5165934563e-01,-2.2471866608e+00,4.0954005718e-01,-2.1105465889e+00,-1.8091288805e+00,3.9598312378e+00,1.5176119804e+00,1.7390805483e-01,-8.6543577909e-01,-7.1728515625e-01,1.7196052074e+00,-6.4536201954e-01,2.8048591614e+00,-1.6904921532e+00,-4.8825353384e-01,5.3767814636e+00,-1.2168287039e+00,-1.7355124950e+00,3.9153873920e-01,-6.8822031021e+00,-4.8250064850e+00,-5.7781319618e+00,-2.3121998310e+00,4.3238186836e-01,-3.8362197876e+00,-3.8313400745e-01,-9.4347476959e-02,-2.8457829952e+00,4.5010619164e+00,3.7921876907e+00,2.6632788181e+00,-6.4210729599e+00,-3.1868193150e+00,2.3913638592e+00,5.1161587238e-02,-4.7391372919e-01,4.6097173691e+00,7.3013858795e+00,-5.7047672272e+00,3.8463244438e+00,1.4998166561e+00,-7.4005403519e+00,-3.0910966396e+00,-3.8556089401e+00,-5.5345382690e+00,7.7394783497e-01,-1.3063720465e+00,-5.0476164818e+00,-2.3485066891e+00,2.6737842560e+00,-1.4222080708e+00,-1.2531909943e+00,1.2040805817e-01,-2.6626157761e+00,-2.8275346756e+00,-3.5936534405e-01,-2.1838569641e+00,-6.9358415604e+00,3.4092915058e-01,-5.5662379265e+00,-1.3335973024e+00,5.9854537249e-02,-4.6568460464e+00,-2.1336844563e-01,4.7558116913e+00,-3.8111438751e+00,-2.7172436714e+00,-6.2456068993e+00,-6.8041238785e+00,1.6725230217e+00,-4.6573624611e+00,-1.7744886875e+00,-4.1381320953e+00,-7.8313918114e+00,1.1825735569e+00,-6.3197398186e-01,2.0505442619e+00,-4.4037714005e+00,3.4096350670e+00,6.2688136101e+00,-1.4468382597e+00,1.6981141567e+00,-5.4944694042e-01,6.1584830284e+00,-8.3183383942e-01,4.8504128456e+00,-3.4848845005e-01,-4.1826729774e+00,2.5210785866e+00,1.2141571045e+00,6.0238587856e-01,6.3595619202e+00,-9.3136668205e-01,-5.1590490341e+00,7.2669780254e-01,9.6953086853e+00,7.5697174072e+00,3.8199238777e+00,-3.8840854168e+00,2.4718645215e-01,2.5872178078e+00,-1.1047279835e+00,5.4769802094e-01,-6.3913984299e+00,3.9337432384e-01,-4.0633296967e+00,-1.5962443352e+00,4.3327541351e+00,-1.9320983887e+00,5.6025447845e+00,5.6420063972e+00,1.3282570839e+00,1.7646474838e+00,-4.9677209854e+00,-5.7366533279e+00,2.2217028141e+00,6.8940782547e-01,2.2700130939e-01,-5.9464120865e-01,3.9848392010e+00,1.4732780457e+00,-5.4590392113e+00,1.1337550879e+00,-7.9395408630e+00,-6.0673367977e-01,8.7681192160e-01,-6.8979053497e+00,-4.3737878799e+00,-6.0809249878e+00,1.5057928562e+00,-1.7387278080e+00,2.4709939957e-02,-2.5745694637e+00,-8.2504577637e+00,-2.5643534660e+00,-2.2636995316e+00,-2.8791475296e+00,4.0702092648e-01,-2.3498404026e+00,-9.2979984283e+00,2.6473915577e+00,3.3783502579e+00,4.3141374588e+00,4.8944377899e+00,-5.3026027679e+00,-3.4961881638e+00,-3.1393017769e+00,-8.1328344345e+00,5.0749044418e+00,-7.5268435478e-01,-4.9851694107e+00,-4.3894777298e+00,-5.7706699371e+00,7.1186637878e-01,-3.4871146679e+00,6.1301147938e-01,-4.6745433807e+00,-6.1526519060e-01,1.4628336430e+00,-5.9733886719e+00,-4.6452569962e+00,-1.0771262646e+00,-4.7786855698e+00,-5.6317243576e+00,-6.2335753441e+00,-2.6501584053e+00,-3.8206905127e-01,-1.0025637627e+01,-1.4925509691e-02,-5.6621131897e+00,-3.8138673306e+00,-6.7767012119e-01,-1.2235267639e+01,-5.6818227768e+00,-6.2683701515e+00,2.1535544395e+00,-1.3004016876e+00,-3.7822403908e+00,-6.2532434464e+00,-7.4805359840e+00,-2.2833733559e+00,-3.5509333611e+00,-3.3706705570e+00,-3.6322505474e+00,8.9984357357e-01,-1.0733514786e+01,-4.6260643005e-01,-2.6322734356e+00,1.3498138189e+00,4.9390535355e+00,-6.0371685028e+00,-3.6553411484e+00,-4.6816883087e+00,-5.3527321815e+00,1.3266429901e+00,2.7304050922e+00,-5.8248524666e+00,-2.5671064854e+00,-3.9251220226e+00,-1.1441576481e+00,-4.2130851746e+00,-4.2541065216e+00,-5.7114505768e+00,5.9663999081e-01,-5.1273047924e-02,-5.5724096298e+00,1.3880348206e+00,-7.3529267311e-01,-1.9884622097e+00,-1.3198155403e+01,-6.3463516235e+00,-9.7641811371e+00,-3.1795372963e+00,-6.6681776047e+00,-3.6754622459e+00,-8.9778909683e+00,-1.9008378983e+00,-4.1516761780e+00,-1.2725543022e+01,-4.9138579369e+00,-8.0701990128e+00,-6.6467995644e+00,-1.7264482975e+00,-1.0591186523e+01,-1.2611398697e+01,-6.9776496887e+00,-6.4887161255e+00,-8.0712547302e+00,-3.1701357365e+00,-1.0514623642e+01,-3.4185771942e+00,-1.5409976959e+01,-7.3502492905e+00,-5.7571897507e+00,-2.8078765869e+00,-6.0029631853e-01,-1.0813791275e+01,-8.3233413696e+00,-5.6582169533e+00,-7.1296873093e+00,-4.9575910568e+00,-4.5150291920e-01,-6.9862232208e+00,-8.0989294052e+00,-1.0364939690e+01,-1.8564903736e+00,-4.5892868042e+00,-6.6298794746e+00,2.7441775799e-01,3.2007306814e-01,-3.9419138432e+00,-7.9533390999e+00,-4.0257992744e+00,-3.9260337353e+00,-8.2563912868e-01,-1.1655879974e+01,-4.7811613083e+00,-7.5994019508e+00,-4.8965072632e+00,-5.8900671005e+00,-2.9421830177e+00,-3.6267948151e+00,-7.2165775299e+00,-4.3254500628e-01,-7.2301816940e+00,5.6597971916e-01,-9.3400306702e+00,-1.4471264839e+01,1.5219426155e+00,-1.5809639931e+01,-1.2559375763e+01,-5.8289709091e+00,-1.9685008526e+00,-3.2096142769e+00,-4.9365329742e+00,-9.8079376221e+00,1.4604234695e+00,-1.1399031639e+01,-9.4855480194e+00,-1.8450713158e-01,7.8748190403e-01,-3.4812107086e+00,-7.7843012810e+00,-6.1953229904e+00,-4.4245405197e+00,-4.9117431641e+00,-9.9292874336e-02,-2.6770606041e+00,-1.9095342159e+00,-6.4813852310e+00,-1.0428983688e+01,-8.4654247761e-01,-1.4588142633e+00,-9.0092945099e+00,2.3600065708e+00,-2.1979236603e+00,-3.7819546461e-01,-4.2178597450e+00,2.3649287224e-01,-2.6565194130e+00,-1.8723552227e+00,-5.8761024475e+00,-6.6005439758e+00,-3.0353972912e+00,-4.2165517807e-01,-2.1006007195e+00,-3.1416616440e+00,1.4259457588e-01,-1.1637855530e+01,2.9104635715e+00,-8.0855846405e+00,-1.4627245069e-01,-6.3255500793e+00,-1.0203465462e+01,4.7283668518e+00,-6.4635376930e+00,-9.4744224548e+00,-2.6793880463e+00,2.2049040794e+00,1.2520408630e+00,-3.6111218929e+00,-5.6025652885e+00,6.7761654854e+00,-1.6909165382e+00,-2.9053645134e+00,4.6622314453e+00,5.0881547928e+00,-5.0001945496e+00,-3.2100214958e+00,-8.6964261532e-01,-3.1132715940e-01,-2.2196016312e+00,3.5112872124e+00,1.3282847404e+00,3.3777632713e+00,-1.7449420691e+00,-9.7192163467e+00,-2.4451200962e+00,-2.3810272217e+00,-8.2540941238e+00,3.7298452854e-01,-5.5443871021e-01,-7.5463640690e-01,2.0239155293e+00,3.6321096420e+00,2.1427907944e+00,-3.7346503735e+00,-5.9873170853e+00,-1.9936802387e+00,-5.6829872131e+00,6.2302904129e+00,1.1395294666e+00,-6.8815803528e+00,1.6507853270e+00,-6.5772237778e+00,9.6431905031e-01,-1.1699732780e+01,-4.9992889166e-01,-9.2751588821e+00,-2.6885743141e+00,7.1759257317e+00,1.4572460651e+00,-1.0788064003e+01,-3.2731933594e+00,-9.2671328783e-01,-2.6215529442e+00,-1.3343153000e+00,-2.1149168909e-01,2.1864266396e+00,3.0244846344e+00,-1.5204720497e+00,3.2159576416e+00,9.8057311773e-01,-4.3331871033e+00,-2.5122561455e+00,5.1312408447e+00,5.0994110107e+00,1.2575601339e+00,3.6802058220e+00,1.7695398331e+00,5.8488559723e+00,-2.5002026558e+00,-3.9660127163e+00,-5.0027971268e+00,-1.9362958670e+00,-5.8410263062e+00,9.7893786430e-01,1.8134459257e+00,6.5301537514e-01,4.7962680459e-01,4.5251250267e+00,1.8157329559e+00,-3.5771241188e+00,-3.4729127884e+00,5.0260200500e+00,-3.3541507721e+00,2.7540006638e+00,2.3927617073e+00,-7.6332831383e+00,4.4825134277e+00,-7.8741669655e-02,-1.6996526718e+00,-8.5024414062e+00,1.5988695621e+00,-9.6101341248e+00,-7.5785136223e-01,2.4060752392e+00,4.8226165771e-01,-4.3438310623e+00,-5.6683721542e+00,-1.2113438845e+00,-3.6807851791e+00,-1.2304303646e+00,-2.3548290730e+00,4.5846223831e+00,2.4537081718e+00,-5.6711330414e+00,1.1204375029e+00,-2.8291060925e+00,-2.5672259331e+00,2.8938376904e-01,2.8059489727e+00,6.3227233887e+00,1.0397281647e+00,4.8558368683e+00,3.2680940628e-01,6.3248038292e-01,1.0622751713e-01,4.0957880020e-01,-1.3650389910e+00,2.3482124805e+00,-2.9099612236e+00,1.4583487511e+00,-2.4072337151e-01,7.5668585300e-01,-1.1412459612e+00,6.0181751251e+00,1.3607318401e+00,-2.0085451603e+00,-1.5613406897e-01,7.6738719940e+00,5.0793542862e+00,2.9588377476e+00,2.1355977058e+00,-5.3300800323e+00,3.5542318821e+00,3.9785132408e+00,-1.9510080814e+00,2.9656660557e+00,5.7593398094e+00,-4.8957033157e+00,4.2489471436e+00,5.7869338989e-01,-1.9126449823e+00,3.0648422241e+00,3.6590814590e+00,2.8865170479e-01,-3.0269157887e+00,-3.4244828224e+00,-2.4996459484e-01,8.8347434998e+00,3.8145272732e+00,-2.1911396980e+00,1.4329774380e+00,-1.4564685822e+00,-2.0441670418e+00,3.4042668343e+00,-1.3045470715e+00,8.5113925934e+00,6.7995347977e+00,2.1732692719e+00,4.3701848984e+00,-3.3699197769e+00,5.0154495239e+00,1.8222936392e+00,5.8763265610e-01,1.8868304491e+00,-1.8511188030e+00,1.1704752445e+00,7.7951860428e-01,-3.6631393433e-01,9.7193312645e-01,4.5326032639e+00,8.1203782558e-01,2.7630548477e+00,9.7376698256e-01,6.5522661209e+00,4.7820453644e+00,3.8714184761e+00,3.1414940357e+00,-4.0223751068e+00,-4.0782856941e+00,6.2936925888e+00,-4.3896813393e+00,3.6575958729e+00,3.1736536026e+00,-7.9109430313e+00,2.7965064049e+00,-1.6393620968e+00,-7.1267986298e+00,3.9064295292e+00,4.8442950249e+00,-2.5814318657e+00,-2.8714685440e+00,-4.7656979561e+00,5.2696788311e-01,8.5532963276e-01,3.4139919281e+00,6.1172294617e-01,-2.8757041693e-01,-2.8581476212e+00,-1.0153235197e+00,-3.3231287003e+00,-4.9652457237e+00,1.5399661064e+00,6.7374606133e+00,-2.6607222557e+00,1.9582641125e+00,-3.4335196018e-01,1.9569530487e+00,-3.2079486847e+00,-2.2549390793e-01,9.2062890530e-01,4.2561807632e+00,-5.3882026672e-01,4.6156387329e+00,-2.8151602745e+00,-6.2726612091e+00,-2.6896524429e+00,-6.1340105534e-01,9.1598987579e-01,2.7780413628e+00,9.9030148983e-01,-5.0528879166e+00,-2.5809226036e+00,1.3284946680e+00,-2.6061415672e+00,-5.0751218796e+00,3.9387738705e+00,-6.8158493042e+00,-6.5382347107e+00,-2.6407690048e+00,-8.1353921890e+00,-1.2892920971e+00,-2.4271864891e+00,-6.3638305664e+00,-2.8587374687e+00,1.0038521290e+00,-2.8697524071e+00,-5.2355484962e+00,-5.3211765289e+00,-2.8343480825e-01,-7.5192356110e-01,4.0052647591e+00,-2.7655422688e-01,-2.3809466362e+00,-5.9352526665e+00,-3.3282990456e+00,-3.9842171669e+00,-7.5040006638e+00,-8.2767808437e-01,2.7569327354e+00,-5.7088308334e+00,-5.6007289886e-01,1.8547725677e+00,-6.8538308144e-01,-8.3577032089e+00,-1.0828082561e+00,-2.0379667282e+00,1.3569022417e+00,-7.0427167416e-01,4.3945846558e+00,-5.1372156143e+00,-9.8691501617e+00,-8.9415798187e+00,-2.5555887222e+00,-5.0672206879e+00,2.0166089535e+00,2.2143089771e+00,-5.5347824097e+00,-5.2509288788e+00,-2.3668582439e+00};

sys_error_code_t AiUSC_DPU_vtblProcess(IDPU *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  ADPU * super = (ADPU*)_this;
  AiUSC_DPU_t *p_obj = (AiUSC_DPU_t*)_this;
  CBItem **p_consumer_buff = NULL;
  CircularBuffer *p_circular_buffer = NULL;

  //DPU has the priority
  if(!super->isADPUattached)
  {
    for(int i=0; i < ADPU_CFG_MAX_SENSOR; i++)
    {
      if(super->sensors[i].sensorIF != NULL)
      { //TODO sensors[i].pConsumerDataBuff could be not necessary
        uint32_t sensor_ready = CB_GetReadyItemFromTail(super->sensors[i].cb_handle.pCircularBuffer, &super->sensors[i].cb_handle.pConsumerDataBuff);
        if(sensor_ready == SYS_CB_NO_READY_ITEM_ERROR_CODE)
        {
          return SYS_NO_ERROR_CODE;
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
      return 0;
    }
    p_consumer_buff = &super->AttachedAdpu.cb_handle.pConsumerDataBuff;
    p_circular_buffer = super->AttachedAdpu.cb_handle.pCircularBuffer;
  }

  if ((*p_consumer_buff) != NULL)
  {

    float *p_in =  (float *)CB_GetItemData((*p_consumer_buff));
    /* call Ai library. */
	 p_obj->ai_processing_f(AI_USC_DPU_NAME, p_in, p_obj->ai_out);

	/* release the buffer as soon as possible */
	CB_ReleaseItem(p_circular_buffer, (*p_consumer_buff));
	(*p_consumer_buff) = NULL;

	ProcessEvent evt_acc;
	ProcessEventInit((IEvent*)&evt_acc, super->pProcessEventSrc, (ai_logging_packet_t*)&super->dpuOutStream, ADPU_GetTag(super));
	IDPU_DispatchEvents(_this, &evt_acc);
  }
  return xRes;
}

/* Private member function definition */
/**************************************/

static inline boolean_t AiUSC_DPUAreStreamsInitialized(AiUSC_DPU_t *_this)
{
  assert_param(_this != NULL);

  return _this->stream_ready;
}
