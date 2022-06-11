/**
 ******************************************************************************
 * @file    MfccDPU.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version 1.0.0
 * @date    May 28, 2021
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

#include "MfccDPU.h"
#include "MfccDPU_vtbl.h"
#include "MfccDPU_Tables.h"
#include "services/sysdebug.h"
#include <stdio.h>

#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_MFCC, level, message)

/**
 * Specified the virtual table for the MfccDPU_t class.
 */
static const IDPU_vtbl sMfccDPU_vtbl = {
    MfccDPU_vtblInit,
    ADPU_AttachToSensor_vtbl,
    ADPU_DetachFromSensor_vtbl,
    ADPU_AttachInputADPU_vtbl,
    ADPU_DetachFromADPU_vtbl,
    ADPU_DispatchEvents_vtbl,
    ADPU_RegisterNotifyCallbacks_vtbl,
    MfccDPU_vtblProcess,
};


/* Inline functions definition */
/*******************************/

/* Private member function declaration */
/***************************************/

/**
 * Check if the DPU is initialized and ready to receive and process data.
 *
 * @param _this [IN] specifies a pointer to the object.
 * @return TRUE if the object is initialized, FALSE otherwise.
 */
static inline boolean_t MfccDPUAreStreamsInitialized(MfccDPU_t *_this);

/* Public API functions definition */
/***********************************/

IDPU *MfccDPUAlloc() {
  IDPU *p_obj = (IDPU*) pvPortMalloc(sizeof(MfccDPU_t));

  if (p_obj != NULL)
  {
    p_obj->vptr = &sMfccDPU_vtbl;
  }

  return p_obj;
}

IDPU *MfccDPUStaticAlloc(void *p_mem_block)
{
  IDPU *p_obj = (IDPU*)p_mem_block;
  if (p_obj != NULL)
  {
    p_obj->vptr = &sMfccDPU_vtbl;
  }

  return p_obj;
}

uint32_t MfccDPUSetStreamsParam(MfccDPU_t *_this, uint16_t signal_size, uint8_t cb_items) {
  assert_param(_this != NULL);
  uint32_t working_buffer_size_byte = 0;

  if (!MfccDPUAreStreamsInitialized(_this))
  {
    /* DPU has been already initialized, so first reset it, if needed */
  }

  /* Initialize the input stream */
  /* DPU converts input data in float */
  _this->super.dpuWorkingStream.packet.payload_type = AI_FMT;
  _this->super.dpuWorkingStream.packet.payload_fmt = AI_SP_FMT_FLOAT32_RESET();

  /* the shape is 1D */
  _this->super.dpuWorkingStream.packet.shape.n_shape                          = 1 ;
  _this->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH]  = signal_size;

  /* compute the size in byte of one cb item, */
  _this->super.n_bytes_for_item =  signal_size * sizeof(float);

  /* Initialize the out stream */
  _this->super.dpuOutStream.packet.shape.n_shape                          = 2 ;
  _this->super.dpuOutStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH]  = MFCCDPU_NUM_MEL_COL;
  _this->super.dpuOutStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT] = MFCCDPU_NUM_MEL;

  _this->super.dpuOutStream.packet.payload_type = AI_FMT;
  _this->super.dpuOutStream.packet.payload_fmt  = AI_SP_FMT_FLOAT32_RESET();
  _this->super.dpuOutStream.packet.payload_size = sizeof(_this->mfcc_out);
  _this->super.dpuOutStream.packet.payload = (uint8_t*)&_this->mfcc_out[0];
  _this->super.cb_items = cb_items;

  _this->stream_ready = TRUE;

  working_buffer_size_byte = cb_items * _this->super.n_bytes_for_item;

  return working_buffer_size_byte;
}


sys_error_code_t MfccDPUPrepareToProcessData(MfccDPU_t *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  ADPU_Reset((ADPU*)_this);
  _this->mfcc_out[0] = 0;
  _this->mfcc_out[1] = 0;

  return res;
}
//#define MFCC_GEN_LUT
#ifdef MFCC_GEN_LUT
static void genLUT(void)
{
#define NUM_MEL      32
#define NUM_MEL_COEF 3772
#define NUM_MFCC     32
#define FFT_LEN      4096
#define SMP_RATE     192000

  float32_t Win [FFT_LEN];
  uint32_t  start_indices[NUM_MEL];
  uint32_t  stop_indices [NUM_MEL];
  float32_t melFilterLut [NUM_MEL_COEF];
  float32_t dct[NUM_MEL*NUM_MFCC];
  MelFilterTypeDef           S_MelFilter;
  DCT_InstanceTypeDef        S_DCT;
  int i;

  /* Init window function */
  if (Window_Init(Win, FFT_LEN , WINDOW_HANN) != 0){
	while(1);
  }
  printf("Hanning window: %d \n\r",FFT_LEN);
  for (i=0;i<FFT_LEN;i++)	{
	printf("%.10e,",Win[i]);
	if(!((i+1)%8)) printf("\n");
  }

  S_MelFilter.pStartIndices = &start_indices[0];
  S_MelFilter.pStopIndices  = &stop_indices[0];
  S_MelFilter.pCoefficients = &melFilterLut[0];
  S_MelFilter.NumMels       = NUM_MEL;
  S_MelFilter.FFTLen        = FFT_LEN;
  S_MelFilter.SampRate      = SMP_RATE;
  S_MelFilter.FMin          = 0.0;
  S_MelFilter.FMax          = SMP_RATE / 2.0;
  S_MelFilter.Formula       = MEL_SLANEY;
  S_MelFilter.Normalize     = 1;
  S_MelFilter.Mel2F         = 1;

  MelFilterbank_Init(&S_MelFilter);
  if (S_MelFilter.CoefficientsLength != NUM_MEL_COEF){
	while(1); /* Adjust NUM_MEL_COEFS to match S_MelFilter.CoefficientsLength */
  }
  printf("Mel coefs : \n");
  for (i=0;i<NUM_MEL_COEF;i++)
  {
	  printf("%.10e,",melFilterLut[i]);
	  if(!((i+1)%8)) printf("\n");
  }
  printf("\nstart idx : \n");
  for (i=0;i<NUM_MEL;i++)
  {
	  printf("%4lu,",start_indices[i]);
	  if(!((i+1)%8)) printf("\n");
  }
  printf("stop  idx : \n");
  for (i=0;i<NUM_MEL;i++)
  {
	  printf("%4lu,",stop_indices[i]);
	  if(!((i+1)%8)) printf("\n");
  }
  printf("\n DCT table \n");

  S_DCT.NumFilters      = NUM_MFCC;
  S_DCT.NumInputs       = NUM_MEL;
  S_DCT.Type            = DCT_TYPE_II_ORTHO;
  S_DCT.RemoveDCTZero   = 0;
  S_DCT.pDCTCoefs       = dct;
  if (DCT_Init(&S_DCT) != 0)
  {
	while(1);
  }
  for (i=0;i<NUM_MEL * NUM_MFCC;i++)
  {
	  printf("%.10e,",dct[i]);
	  if(!((i+1)%8)) printf("\n");
  }
  printf("\n");
}
#endif
/**
 * @brief Initialize LogMel preprocessing
 * @param none
 * @retval none
 */

static void Preprocessing_Init(MfccDPU_t *_this)
{

#ifdef MFCC_GEN_LUT
  genLUT();
#endif
  /* Init RFFT */
  arm_rfft_fast_init_f32(&_this->S_Rfft, MFCCDPU_FFT_LEN);

  /* Init Spectrogram */
  _this->S_Spectr.pRfft                    = &_this->S_Rfft;
  _this->S_Spectr.Type                     = SPECTRUM_TYPE_POWER;
  _this->S_Spectr.pWindow                  = (float32_t *) hannWin_4096;
  _this->S_Spectr.SampRate                 = MFCCDPU_SAMPLE_RATE;
  _this->S_Spectr.FrameLen                 = MFCCDPU_FRAME_LEN;
  _this->S_Spectr.FFTLen                   = MFCCDPU_FFT_LEN;
  _this->S_Spectr.pScratch                 = _this->pSpectrScratchBuffer;

  /* Init mel filterbank */
  _this->S_MelFilter.pStartIndices         = (uint32_t *) melFiltersStartIndices_4096_32;
  _this->S_MelFilter.pStopIndices          = (uint32_t *) melFiltersStopIndices_4096_32;
  _this->S_MelFilter.pCoefficients         = (float32_t *) melFilterLut_4096_32;
  _this->S_MelFilter.NumMels               = MFCCDPU_NUM_MEL;
  _this->S_MelFilter.FFTLen                = MFCCDPU_FFT_LEN;
  _this->S_MelFilter.SampRate              = MFCCDPU_SAMPLE_RATE;
  _this->S_MelFilter.FMin                  = 0.0;
  _this->S_MelFilter.FMax                  = MFCCDPU_SAMPLE_RATE / 2.0;
  _this->S_MelFilter.Formula               = MEL_SLANEY;
  _this->S_MelFilter.Normalize             = 1;
  _this->S_MelFilter.Mel2F                 = 1;

  /* Init MelSpectrogram */
  _this->S_MelSpectr.SpectrogramConf       = &_this->S_Spectr;
  _this->S_MelSpectr.MelFilter             = &_this->S_MelFilter;


  /* Init LogMelSpectrogram */
  _this->S_LogMelSpectr.MelSpectrogramConf = &_this->S_MelSpectr;
  _this->S_LogMelSpectr.LogFormula         = LOGMELSPECTROGRAM_SCALE_DB;
  _this->S_LogMelSpectr.Ref                = 1.0;
  _this->S_LogMelSpectr.TopdB              = HUGE_VALF;

  /* Init DCT operation */
  _this->S_DCT.NumFilters                  = MFCCDPU_NUM_MFCC;
  _this->S_DCT.NumInputs                   = MFCCDPU_NUM_MEL;
  _this->S_DCT.Type                        = DCT_TYPE_II_ORTHO;
  _this->S_DCT.RemoveDCTZero               = 0;
  _this->S_DCT.pDCTCoefs                   = (float32_t *) dct2_32_32;

  /* Init MFCC */
  _this->S_Mfcc.LogMelConf                 = &_this->S_LogMelSpectr;
  _this->S_Mfcc.pDCT                       = &_this->S_DCT;
  _this->S_Mfcc.NumMfccCoefs               = MFCCDPU_NUM_MFCC;
  _this->S_Mfcc.pScratch                   = _this->pMfccScratchBuffer;
  _this->S_MelFilter.NumMels               = MFCCDPU_NUM_MEL;
}

/* IDPU virtual functions definition */
/*************************************/
sys_error_code_t MfccDPU_vtblInit(IDPU *_this) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  MfccDPU_t *p_obj =(MfccDPU_t*)_this;

  res = ADPU_Init_vtbl(_this);
  if (!SYS_IS_ERROR_CODE(res)) {
    p_obj->mfcc_out[0] = 0;
    p_obj->mfcc_out[1] = 0;
    p_obj->stream_ready = FALSE;
    // take the ownership of the Sensor Event IF
    IEventListenerSetOwner((IEventListener *) ADPU_GetEventListenerIF(&p_obj->super), &p_obj->super);

    /*
     * initialise AI preprocessing ( MFCC computation)
     */
    Preprocessing_Init(p_obj);
  }

  return res;
}


sys_error_code_t MfccDPU_vtblProcess(IDPU *_this)
{

  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  ADPU * super = (ADPU*)_this;
  MfccDPU_t *p_obj = (MfccDPU_t*)_this;
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
	  ai_logging_packet_t* p_packet  = (ai_logging_packet_t*)&super->dpuOutStream.packet;
	  float *p_out = (float *)(p_packet->payload);
	  p_out += p_obj->mfccColIdx*(MFCCDPU_NUM_MEL);
	  MfccColumn(&p_obj->S_Mfcc, p_in , p_out);   	  /* call Mfcc library. */
	  /* release the buffer as soon as possible */
	  CB_ReleaseItem(p_circular_buffer, (*p_consumer_buff));
	  (*p_consumer_buff) = NULL;

	  if (MFCCDPU_NUM_MEL_COL == ++p_obj->mfccColIdx)
	  {
	    p_out = (float *)(p_packet->payload);
		ProcessEvent evt_acc ;
		p_obj->mfccColIdx = 0;
		ProcessEventInit((IEvent*)&evt_acc, super->pProcessEventSrc, (ai_logging_packet_t*)&super->dpuOutStream, ADPU_GetTag(super));
		IDPU_DispatchEvents(_this, &evt_acc);
	  }
  }
  return xRes;

}

/* Private member function definition */
/**************************************/

static inline boolean_t MfccDPUAreStreamsInitialized(MfccDPU_t *_this)
{
  assert_param(_this != NULL);

  return _this->stream_ready;
}
