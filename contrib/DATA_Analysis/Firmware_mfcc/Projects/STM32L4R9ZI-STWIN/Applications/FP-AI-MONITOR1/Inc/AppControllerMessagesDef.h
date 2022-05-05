/**
 ******************************************************************************
 * @file    AppControllerMessagesDef.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version V1.0.0
 * @date    15-September-2021
 *
 * @brief   AppController commands ID
 *
 * This file declare the commands ID for the AppControllerTask.
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

#ifndef INC_APPCONTROLLERMESSAGESDEF_H_
#define INC_APPCONTROLLERMESSAGESDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CTRL_CMD_NEW_CHAR            (0x01U)
#define CTRL_CMD_STRAT               (0x02U)
#define CTRL_CMD_DID_STOP            (0x03U)
#define CTRL_CMD_NEAI_PROC_RES       (0x04U)
#define CTRL_CMD_AI_PROC_RES         (0x05U)
#define CTRL_CMD_SWITCH_BANK         (0x06U)

#define CTRL_CMD_PARAM_NEAI_LEARN    (0x10U)
#define CTRL_CMD_PARAM_NEAI_DETECT   (0x20U)
#define CTRL_CMD_PARAM_AI            (0x30U)
#define CTRL_CMD_PARAM_FROM_AUTOMODE (0x40U)
#define CTRL_CMD_PARAM_FROM_CLI      (0x50U)

#ifdef __cplusplus
}
#endif

#endif /* INC_APPCONTROLLERMESSAGESDEF_H_ */
