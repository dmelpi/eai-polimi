/**
 ******************************************************************************
 * @file    SensorListener.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version v1.0.0
 * @date    Jan 31, 2022
 *
 * @brief   Simple object that listen the ::SensorEvent_t and print the value
 *          in the debug log.
 *
 *
 *********************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */

#ifndef INC_SENSORLISTENER_VTBL_H_
#define INC_SENSORLISTENER_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


  /* IListener virtual functions */
  sys_error_code_t SensorListener_vtblOnStatusChange(IListener *this); ///< @sa IListener_OnStatusChange

  /* IEventListener virtual functions */
  void *SensorListener_vtblGetOwner(IEventListener *_this); ///< @sa IEventListener_GetOwner
  void SensorListener_vtblSetOwner(IEventListener *_this, void *p_owner); ///< @sa IEventListener_SetOwner

  /* ISensorEventListener virtual functions */
  sys_error_code_t SensorListener_vtblOnNewDataReady(IEventListener *_this, const DataEvent_t *p_evt); ///< @sa IDataEventListener_OnNewDataReady


#ifdef __cplusplus
}
#endif

#endif /* INC_SENSORLISTENER_VTBL_H_ */
