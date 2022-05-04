/**
 ******************************************************************************
 * @file    NeaiMessagesDef.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version V1.0.0
 * @date    15-September-2021
 *
 * @brief   NanoEdge AI commands ID
 *
 * This file declare the commands ID for the NeaiTask.
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
#ifndef INC_NEAIMESSAGESDEF_H_
#define INC_NEAIMESSAGESDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#define NAI_CMD_NEW_DATA_READY            (0x01U)
#define NAI_CMD_STOP_PROCESSING           (0x02U)
#define NAI_CMD_SET_MODE                  (0x03U)
#define NAI_CMD_CONNECT_TO_SENSOR         (0x04U)
#define NAI_CMD_DETACH_FROM_SENSOR        (0x05U)
#define NAI_CMD_ADD_DPU_LISTNER           (0x06U)
#define NAI_CMD_REMOVE_DPU_LISTNER        (0x07U)
#define NAI_CMD_SET_SENSITIVITY           (0x08U)
#define NAI_CMD_INIT                      (0x09U)


#define NAI_CMD_PARAM_DETECT              (0x10U)
#define NAI_CMD_PARAM_LEARN               (0x20U)

#ifdef __cplusplus
}
#endif

#endif /* INC_NEAIMESSAGESDEF_H_ */
