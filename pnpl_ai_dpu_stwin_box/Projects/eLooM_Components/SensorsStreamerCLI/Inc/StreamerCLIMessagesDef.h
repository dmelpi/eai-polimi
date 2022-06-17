/**
 ******************************************************************************
 * @file    AppControllerMessagesDef.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$
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

#define SCLI_CMD_NEW_CHAR            (0x01U)
#define SCLI_CMD_STRAT               (0x02U)
#define SCLI_CMD_DID_STOP            (0x03U)
#define SCLI_CMD_NEW_DATA_READY      (0x04U)

#define SCLI_CMD_PARAM_STREAMER      (0x10U)
#define SCLI_CMD_PARAM_FROM_AUTOMODE (0x20U)
#define SCLI_CMD_PARAM_FROM_CLI      (0x30U)

#ifdef __cplusplus
}
#endif

#endif /* INC_APPCONTROLLERMESSAGESDEF_H_ */
