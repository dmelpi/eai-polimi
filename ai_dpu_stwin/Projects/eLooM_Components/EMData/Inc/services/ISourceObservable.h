/**
 ******************************************************************************
 * @file    ISourceObservable.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version 3.0.0
 * @date    Jun 8, 2021
 *
 * @brief   Stream data source generic interface.
 *
 * This interface describe the API to observe the configuration of a
 * stream data source object (for example a sensor). This kind
 * of object has some properties like:
 * - Full Scale (FS)
 * - Output Data Rate (ODR)
 * - Sensitivity
 * - Object ID.
 *
 * An stream data source also expose an ::IEventSrc interface to allow an
 * observer object to monitor the data stream.
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */

#ifndef INCLUDE_ISOURCEOBSERVER_H_
#define INCLUDE_ISOURCEOBSERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "events/IEvent.h"


/**
 * Create  type name for ::_ISourceObservable.
 */
typedef struct _ISourceObservable ISourceObservable;


/* Public interface */
/********************/

/**
 * Get an ID value. This value should be unique between all the ::ISourceObservable object in the application.
 *
 * @param _this [IN] specifies a pointer to an object that implements the ::ISourceObservable interface.
 * @return the ID of the interface instance.
 */
static inline uint8_t ISourceGetId(ISourceObservable *_this);

/**
 * Get an ::IEventSrc interface to observe the data stream.
 *
 * @param _this [IN] specifies a pointer to an object that implements the ::ISourceObservable interface.
 * @return a pointer to an ::IEventSrc interface.
 */
static inline IEventSrc * ISourceGetEventSrcIF(ISourceObservable *_this);

/**
 * Get the Output Date Rate of the stream data source. The method get both the nominal and the measured
 * output data rate.
 *
 * @param _this  [IN] specifies a pointer to an object that implements the ::ISourceObservable interface.
 * @param p_measured [OUT] specifies the last measured data rate, or zero if the object has never produced data.
 * @param p_nominal [OUT] specifies the nominal data rate that is the ODR set during the object configuration.
 * @return SYS_NO_ERROR_CODE if success, an application specific error code otherwise.
 */
static inline sys_error_code_t ISourceGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal);

/**
 * Get the Full Scale (FS) of the stream data source.
 *
 * @param _this  [IN] specifies a pointer to an object that implements the ::ISourceObservable interface.
 * @return the full scale of the stream data source.
 */
static inline float ISourceGetFS(ISourceObservable *_this);

/**
 * Get the sensitivity of the of the stream data source.
 *
 * @param _this  [IN] specifies a pointer to an object that implements the ::ISourceObservable interface.
 * @return the sensitivity of the of the stream data source.
 */
static inline float ISourceGetSensitivity(ISourceObservable *_this);

/**
 * Get the type of the data produced by the stream data source. Valid value are:
 * - DATA_TYPE_UINT8
 * - DATA_TYPE_INT8
 * - DATA_TYPE_UINT16
 * - DATA_TYPE_INT16
 * - DATA_TYPE_UINT32
 * - DATA_TYPE_INT32
 * - DATA_TYPE_FLOAT
 * - DATA_TYPE_CUSTOM: specifies a complex data type like a struct etc.
 *
 * @param _this [IN] specifies a pointer to an object that implements the ::ISourceObservable interface.
 * @return the type of the data produced by the stream data source.
 */
static inline uint8_t ISourceGetDataType(ISourceObservable *_this);

/**
 * Get the number of dimensions of the data produced by the stream data source.
 * For example, for a three axis accelerometer the dimensions is three (x,y,z).
 * @param _this [IN] specifies a pointer to an object that implements the ::ISourceObservable interface.
 * @return the type of the data produced by the stream data source.
 */
static inline uint16_t ISourceGetDimensions(ISourceObservable *_this);


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_ISOURCEOBSERVER_H_ */
