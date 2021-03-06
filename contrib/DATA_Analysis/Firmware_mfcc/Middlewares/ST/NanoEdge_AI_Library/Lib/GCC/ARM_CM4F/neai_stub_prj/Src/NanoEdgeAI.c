/**
 ******************************************************************************
 * @file           : lib.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Library body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "NanoEdgeAI.h"
#include <stdio.h>
#include <string.h>

#if defined(__GNUC__)
#define POST_PLACE_IN_SECTION __attribute__ ((section (".neai")))
#endif

#define NEAI_KNOWLEDGE_SIZE 4096

static uint8_t s_pnKnowledge[NEAI_KNOWLEDGE_SIZE] POST_PLACE_IN_SECTION = {0x12};
static uint8_t s_nDetectResult = 0;

static float s_fSensitivity = 0;

extern void HAL_Delay(uint32_t Delay);

/** Functions ----------------------------------------------------------------*/
	void NanoEdgeAI_initialize(void) {
	  memset(s_pnKnowledge, 0, sizeof(s_pnKnowledge));
	}

	uint8_t NanoEdgeAI_learn(float data_input[]) {
	  //to simulate the NanoEdge AI library.
	    HAL_Delay(15);
		return 1;
	}

	uint8_t NanoEdgeAI_detect(float data_input[]) {
	    HAL_Delay(15);
	    uint8_t nRes = s_nDetectResult;
	    if (++s_nDetectResult > 100) {
	      s_nDetectResult = 0;
	    }
		return nRes;
	}

	void NanoEdgeAI_set_sensitivity(float sensitivity) {
	  s_fSensitivity = sensitivity;
	}

	float NanoEdgeAI_get_sensitivity(void) {
		return s_fSensitivity;
	}

	void NanoEdgeAI_forecasting_set_limit(float min_value, float max_value) {

	}

	void NanoEdgeAI_forecasting_push(float value) {

	}

	void NanoEdgeAI_forecasting_get(float forecast[], uint32_t n_step) {

	}
