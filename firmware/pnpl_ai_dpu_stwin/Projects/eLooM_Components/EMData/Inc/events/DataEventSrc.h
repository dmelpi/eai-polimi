/**
 ******************************************************************************
 * @file    DataEventSrc.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version M.m.b
 * @date    May 13, 2022
 *
 * @brief
 *
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */
#ifndef EMDATA_INC_EVENTS_DATAEVENTSRC_H_
#define EMDATA_INC_EVENTS_DATAEVENTSRC_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "events/AEventSrc.h"
#include "events/AEventSrc_vtbl.h"
#include "FreeRTOS.h"


/**
 * Create  type name for ::_DataEventSrc.
 */
typedef struct _DataEventSrc DataEventSrc_t;


/* Public API declaration */
/**************************/

/**
 * Allocate an instance of DataEventSrc.
 *
 * @return a pointer to the generic object ::IEventSrc if success,
 * or NULL if out of memory error occurs.
 */
IEventSrc *DataEventSrcAlloc(void);

/**
 * Deallocate an instance of DataEventSrc.
 *
 * @params p_obj
 */
static inline
void DataEventSrcFree(IEventSrc *p_obj);


/* Inline functions definition */
/*******************************/

static inline
void DataEventSrcFree(IEventSrc *p_obj)
{
  /* kernel deallocator already check for a NULL pointer. */
  vPortFree(p_obj);
}


#ifdef __cplusplus
}
#endif

#endif /* EMDATA_INC_EVENTS_DATAEVENTSRC_H_ */
