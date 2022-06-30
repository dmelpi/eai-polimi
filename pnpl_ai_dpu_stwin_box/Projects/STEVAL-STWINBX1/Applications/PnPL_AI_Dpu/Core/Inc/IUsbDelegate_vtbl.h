/**
 ******************************************************************************
 * @file    IUsbDelegate_vtbl.h
 * @author  STMicroelectronics
 * @version 1.0.0
 * @date    May 13, 2022
 *
 * @brief   
 *
 * <DESCRIPTION>
 *
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
#ifndef CORE_INC_IUSBDELEGATE_VTBL_H_
#define CORE_INC_IUSBDELEGATE_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "services/systypes.h"
#include "services/syserror.h"
#include "services/systp.h"

/**
 * Create  type name for _IUsbDelegate_vtbl.
 */
typedef struct _IUsbDelegate_vtbl IUsbDelegate_vtbl;

/**
 * Specifies the virtual table for the  class.
 */
struct _IUsbDelegate_vtbl
{
  sys_error_code_t (*OnNewData)(IUsbDelegate *_this, const uint8_t *buffer, uint32_t length);
};

/**
 * IUsbDelegate interface internal state.
 * It declares only the virtual table used to implement the inheritance.
 */
struct _IUsbDelegate
{
  /**
   * Pointer to the virtual table for the class.
   */
  const IUsbDelegate_vtbl *vptr;
};

// Inline functions definition
// ***************************

static inline sys_error_code_t IUsbDelegateOnNewData(IUsbDelegate *_this, const uint8_t *buffer, uint32_t length)
{
  return _this->vptr->OnNewData(_this, buffer, length);
}

#ifdef __cplusplus
}
#endif

#endif /* CORE_INC_IUSBDELEGATE_VTBL_H_ */
