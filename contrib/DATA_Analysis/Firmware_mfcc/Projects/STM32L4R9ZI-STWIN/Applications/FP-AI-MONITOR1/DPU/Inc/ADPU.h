/**
 ******************************************************************************
 * @file    ADPU.h
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    6-Sep-2021
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
 
#ifndef INCLUDE_ADPU_H_
#define INCLUDE_ADPU_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "events/ISensorEventListener.h"
#include "events/ISensorEventListener_vtbl.h"
#include "events/ProcessEventSrc.h"
#include "events/ProcessEventSrc_vtbl.h"
#include "events/ProcessEvent.h"
#include "services/systp.h"


#include "IDPU.h"
#include "IDPUVtbl.h"
#include "CircularBuffer.h"
#include "ai_sp_dataformat.h"


#ifndef ADPU_CFG_MAX_SENSOR
#define ADPU_CFG_MAX_SENSOR           10
#endif

#define CB_MAX_ITEMS                  3


#define ADPU_NO_ERROR_CODE            0
#define ADPU_BASE_ERROR_CODE          ADPU_NO_ERROR_CODE   + 1
#define ADPU_ALREADY_ATTACHED         ADPU_BASE_ERROR_CODE + 2
#define ADPU_NOT_ATTACHED             ADPU_BASE_ERROR_CODE + 3
#define ADPU_NOT_IMPLEMENTED          ADPU_BASE_ERROR_CODE + 4
#define ADPU_NO_READY_ITEM_ERROR_CODE ADPU_BASE_ERROR_CODE + 5



  /** Describe the struct used to manage the CB**/
  typedef struct _CBHandle
  {
    CircularBuffer *pCircularBuffer;
    CBItem *pProducerDataBuff;
    CBItem *pConsumerDataBuff;
    uint16_t DataIdx;
  }CBHandle_t;

/** Describe the struct used to handle the different kind of sources **/
  typedef struct _SensorObs
  {
    CBHandle_t cb_handle;
    ISourceObservable  *sensorIF;
    uint8_t sensor_id;
  }SensorObs_t;

  typedef struct _ProcessObs
  {
    CBHandle_t cb_handle;
    IDPU  *adpu;
  }ProcessObs_t;



  /**
   * Create  type name for _AEventSrc.
   */
  typedef struct _ADPU ADPU;

  /**
   * AEventSrc internal state.
   */
  struct _ADPU {

    /** IDPU base interface**/
    IDPU super;

    /** Sensor Listener**/
    ISensorEventListener sensorListener;
    void *pOwner;

    /**Process Event Source**/
    IEventSrc * pProcessEventSrc;

    /** List of Sensors **/
    SensorObs_t  sensors[ADPU_CFG_MAX_SENSOR];
    uint16_t nSensor;
    uint8_t id_sensor_ready;

    /**Check if there is an ADPU attached **/
    uint8_t isADPUattached;

    /** Pointer to the output  **/
    IDPU *nextADPU;

    /** Input ADPU **/
    ProcessObs_t AttachedAdpu;

    /** Used at base class level to handle cb items **/
    uint8_t cb_items;
    uint32_t n_bytes_for_item;

    /** Callback to notify data are readied to be process**/
    DPU_ReadyToProcessCallback_t notifyCall;

    /** Describe the source type **/
    AI_SP_Stream_t sourceStream;

    /** Describe the how the DPU manages the date to process it **/
    AI_SP_Stream_t dpuWorkingStream;

    /** Describe the output type of the ADPU **/
    AI_SP_Stream_t dpuOutStream;

    /**
     * Specifies the parameter passed to the callback when new data are ready to be processed.
     */
    void *p_callback_param;

    boolean_t active;

  };


  /* Public API declaration */
  /**************************/

  IEventListener *ADPU_GetEventListenerIF(ADPU *_this);
  IEventSrc *ADPU_GetEventSrcIF(ADPU * _this);

  /**
   * Set the tag value to ADPU object
   * Note: This function is optional but useful to distinguish a specific ADPU among several ADPUs
   *
   * @param _this [IN] specifies a pointer to the object.
   * @param tag [IN] specifies a tag value to be set.
   * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
   * or NULL if out of memory error occurs.
   */
   sys_error_code_t ADPU_SetTag(ADPU *_this, uint32_t tag) ;

  /**
     * Get the tag value to the ADPU object
     *
     * @param _this [IN] specifies a pointer to the object.
     * @return the tag value set with
     * /code
     * ADPU_SetTag(ADPU *_this, uint32_t tag)
     * /endcode
     */
   uint32_t ADPU_GetTag(ADPU *_this) ;


   /**
      * Reset the ADPU object. All circular buffers are reinitialized, as well as the ADPU resources.
      *
      * @param _this [IN] specifies a pointer to the object.
      * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
      * or NULL if out of memory error occurs.
      */
   sys_error_code_t ADPU_Reset(ADPU *_this);


   sys_error_code_t ADPU_Suspend(ADPU *_this);

   bool ADPU_IsSuspended(ADPU *_this);

   sys_error_code_t ADPU_Resume(ADPU *_this);
#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_ADPU_H_ */
