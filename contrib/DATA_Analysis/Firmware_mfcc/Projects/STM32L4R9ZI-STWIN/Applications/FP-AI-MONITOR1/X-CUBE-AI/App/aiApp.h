/**
 ******************************************************************************
 * @file    aiApp.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$
 *
 * @brief   AI entry function definitions
 *
 * <DESCRIPTIOM>
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_AI_H
#define __APP_AI_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ai_platform.h"
#include "network.h"
#include "network_data.h"
#include <stdio.h>

int aiInit(void);
int aiDeInit(void);
int aiProcess(float *, float[2]);

void aiPrintNetworkInfoToFile(FILE *out);

#ifdef __cplusplus
}
#endif
#endif /* __APP_AI_H */
