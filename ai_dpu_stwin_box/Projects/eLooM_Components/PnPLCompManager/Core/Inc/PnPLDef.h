/**
 ******************************************************************************
 * @file    PnPLDef.h
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    27-Oct-2021
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
#ifndef INC_PNPLDEF_H_
#define INC_PNPLDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* PnP-Like commands codes*/
// CMD TYPE
#define PNPL_CMD_GET            (uint8_t)(0x10)
//#define PNPL_CMD_GET_ACQ_INFO   (uint8_t)(0x11)
#define PNPL_CMD_SET            (uint8_t)(0x20)
#define PNPL_CMD_UPDATE_DEVICE  (uint8_t)(0x21)
//#define PNPL_CMD_CONTROL        (uint8_t)(0x30)
#define PNPL_CMD_SYSTEM_CONFIG  (uint8_t)(0x40)
#define PNPL_CMD_SYSTEM_INFO    (uint8_t)(0x50)
#define PNPL_CMD_COMPUTE_STREAM_IDS (uint8_t)(0x60)
#define PNPL_CMD_ERROR          (uint8_t)(0x70)

#define COMP_KEY_MAX_LENGTH 		32
#define COM_MAX_PNPL_COMPONENTS 20 //TODO ???


#define PNPL_CMD_NO_ERROR_CODE      (0x0)
#define PNPL_CMD_ERROR_CODE         (0x1)

/**
 * Create type name for _PnPLCommand_t.
 */
typedef struct _PnPLCommand_t PnPLCommand_t;

/**
 *  _PnPLCommand_t internal structure.
 */
struct _PnPLCommand_t
{

  /**
   * Specifies the command type.
   */
  uint8_t comm_type;

  /**
   * Specifies the component name.
   */
  char comp_name[COMP_KEY_MAX_LENGTH];

};

#ifdef __cplusplus
  }
  #endif

#endif /* INC_PNPLDEF_H_ */

