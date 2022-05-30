/**
 ******************************************************************************
 * @file    DataEventSrc.c
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

#include "events/DataEventSrc.h"
#include "events/DataEventSrc_vtbl.h"
#include "events/IDataEventListener.h"
#include "events/IDataEventListener_vtbl.h"


static const IEventSrc_vtbl sDataEventSrc_vtbl = {
    AEventSrv_vtblInit,
    AEventSrv_vtblAddEventListener,
    AEventSrv_vtblRemoveEventListener,
    AEventSrv_vtblGetMaxListenerCount,
    DataEventSrc_vtblSendEvent
};

/**
 * DataEventSrc internal state.
 */
struct _DataEventSrc {
  /**
   * Base class object.
   */
  AEventSrc super;
};


/* Public functions definition */
/*******************************/

IEventSrc *DataEventSrcAlloc(void) {
  IEventSrc *p_obj = (IEventSrc*) pvPortMalloc(sizeof(DataEventSrc_t));

  if (p_obj != NULL) {
    p_obj->vptr = &sDataEventSrc_vtbl;
  }

  return p_obj;
}


// IEventSoruce virtual functions definition.
// ******************************************

sys_error_code_t DataEventSrc_vtblSendEvent(const IEventSrc *_this, const IEvent *p_event, void *p_params) {
  assert_param(_this != NULL);
  DataEventSrc_t *p_obj = (DataEventSrc_t*)_this;
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  UNUSED(p_params);

  for (uint8_t i=0; i<AEVENT_SRC_CFG_MAX_LISTENERS; ++i) {
    if (p_obj->super.m_pxListeners[i] != NULL) {
      (void)IDataEventListenerOnNewDataReady(p_obj->super.m_pxListeners[i], (DataEvent_t*)p_event);
    }
  }

  return res;
}
