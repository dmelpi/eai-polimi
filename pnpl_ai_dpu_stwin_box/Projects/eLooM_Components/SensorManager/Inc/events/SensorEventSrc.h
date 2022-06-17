/**
  ******************************************************************************
  * @file    SensorEventSrc.h
  * @author  SRA - MCD
  * @version 1.0.0
  * @date    6-Sep-2021
  *
  * @brief   Extend the abstract ::AEventSrc class.
  *
  * ::IEventSrc class specialized for the ::SensorEvent.
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
#ifndef INCLUDE_EVENTS_SENSOREVENTSRC_H_
#define INCLUDE_EVENTS_SENSOREVENTSRC_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "events/AEventSrc.h"
#include "events/AEventSrc_vtbl.h"


/**
  * Create  type name for _SensorEventSrc.
  */
typedef struct _SensorEventSrc SensorEventSrc;


// Public API declaration
//***********************

/**
  * Allocate an instance of SensorEventSrc.
  *
  * @return a pointer to the generic object ::IEventSrc if success,
  * or NULL if out of memory error occurs.
  */
IEventSrc *SensorEventSrcAlloc(void);

/**
  * Deallocate an instance of SensorEventSrc.
  *
  */
static inline void SensorEventSrcFree(IEventSrc *pxObj);

// Inline functions definition
// ***************************

static inline
void SensorEventSrcFree(IEventSrc *pxObj)
{

  /* kernel deallocator already check for a NULL pointer. */
  //vPortFree(pxObj); /*TODO: ThreadX version*/
}


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_EVENTS_SENSOREVENTSRC_H_ */
