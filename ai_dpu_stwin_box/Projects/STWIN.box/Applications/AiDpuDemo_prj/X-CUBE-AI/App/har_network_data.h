
/**
  ******************************************************************************
  * @file    har_network_data.h
  * @author  AST Embedded Analytics Research Platform
  * @date    Wed Mar 23 19:38:45 2022
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef HAR_NETWORK_DATA_H
#define HAR_NETWORK_DATA_H
#pragma once

#include "har_network_config.h"
#include "ai_platform.h"

#define AI_HAR_NETWORK_DATA_CONFIG               (NULL)


#define AI_HAR_NETWORK_DATA_ACTIVATIONS_SIZES \
  { 2076, }
#define AI_HAR_NETWORK_DATA_ACTIVATIONS_SIZE     (2076)
#define AI_HAR_NETWORK_DATA_ACTIVATIONS_COUNT    (1)
#define AI_HAR_NETWORK_DATA_ACTIVATION_1_SIZE    (2076)



#define AI_HAR_NETWORK_DATA_WEIGHTS_SIZES \
  { 55592, }
#define AI_HAR_NETWORK_DATA_WEIGHTS_SIZE         (55592)
#define AI_HAR_NETWORK_DATA_WEIGHTS_COUNT        (1)
#define AI_HAR_NETWORK_DATA_WEIGHT_1_SIZE        (55592)



AI_DEPRECATED
#define AI_HAR_NETWORK_DATA_ACTIVATIONS(ptr_)  \
  ai_har_network_data_activations_buffer_get(AI_HANDLE_PTR(ptr_))

AI_DEPRECATED
#define AI_HAR_NETWORK_DATA_WEIGHTS(ptr_)  \
  ai_har_network_data_weights_buffer_get(AI_HANDLE_PTR(ptr_))


AI_API_DECLARE_BEGIN

/*!
 * @brief Get network activations buffer initialized struct.
 * @ingroup har_network_data
 * @param[in] ptr a pointer to the activations array storage area
 * @return an ai_buffer initialized struct
 */
AI_DEPRECATED
AI_API_ENTRY
ai_buffer ai_har_network_data_activations_buffer_get(const ai_handle ptr);

/*!
 * @brief Get network weights buffer initialized struct.
 * @ingroup har_network_data
 * @param[in] ptr a pointer to the weights array storage area
 * @return an ai_buffer initialized struct
 */
AI_DEPRECATED
AI_API_ENTRY
ai_buffer ai_har_network_data_weights_buffer_get(const ai_handle ptr);

/*!
 * @brief Get network weights array pointer as a handle ptr.
 * @ingroup har_network_data
 * @return a ai_handle pointer to the weights array
 */
AI_DEPRECATED
AI_API_ENTRY
ai_handle ai_har_network_data_weights_get(void);


/*!
 * @brief Get network params configuration data structure.
 * @ingroup har_network_data
 * @return true if a valid configuration is present, false otherwise
 */
AI_API_ENTRY
ai_bool ai_har_network_data_params_get(ai_network_params* params);


AI_API_DECLARE_END

#endif /* HAR_NETWORK_DATA_H */

