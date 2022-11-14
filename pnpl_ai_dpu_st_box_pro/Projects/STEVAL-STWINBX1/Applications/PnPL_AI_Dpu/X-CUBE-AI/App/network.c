/**
  ******************************************************************************
  * @file    network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Tue Oct 11 17:42:42 2022
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "network.h"
#include "network_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_network
 
#undef AI_NETWORK_MODEL_SIGNATURE
#define AI_NETWORK_MODEL_SIGNATURE     "515d552da208ec781731ac99b2849a83"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Tue Oct 11 17:42:42 2022"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_N_BATCHES
#define AI_NETWORK_N_BATCHES         (1)

static ai_ptr g_network_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_network_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  serving_default_input0_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 1860, AI_STATIC)
/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1860, AI_STATIC)
/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_3_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3672, AI_STATIC)
/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  pool_6_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1824, AI_STATIC)
/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_8_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1824, AI_STATIC)
/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_11_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1776, AI_STATIC)
/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_13_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1776, AI_STATIC)
/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  pool_16_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 888, AI_STATIC)
/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  pool_18_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)
/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  dense_20_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)
/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  nl_21_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 1, AI_STATIC)
/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_1_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12, AI_STATIC)
/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_1_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12, AI_STATIC)
/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_3_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 216, AI_STATIC)
/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_3_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)
/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_8_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)
/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_8_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)
/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_11_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 432, AI_STATIC)
/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_11_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)
/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_13_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)
/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_13_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)
/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  dense_20_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)
/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  dense_20_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)
/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  serving_default_input0_output, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 12, 1, 155), AI_STRIDE_INIT(4, 4, 4, 48, 48),
  1, &serving_default_input0_output_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_1_output, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 12, 1, 155), AI_STRIDE_INIT(4, 4, 4, 48, 48),
  1, &eltwise_1_output_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_1_output0, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 12, 155, 1), AI_STRIDE_INIT(4, 4, 4, 48, 7440),
  1, &eltwise_1_output_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_3_output, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 153, 1), AI_STRIDE_INIT(4, 4, 4, 96, 14688),
  1, &conv2d_3_output_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_3_output0, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 153), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &conv2d_3_output_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  pool_6_output, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 76), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &pool_6_output_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_8_output, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 76), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &eltwise_8_output_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_8_output0, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 76, 1), AI_STRIDE_INIT(4, 4, 4, 96, 7296),
  1, &eltwise_8_output_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_11_output, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 74, 1), AI_STRIDE_INIT(4, 4, 4, 96, 7104),
  1, &conv2d_11_output_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_13_output, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 74, 1), AI_STRIDE_INIT(4, 4, 4, 96, 7104),
  1, &eltwise_13_output_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_13_output0, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 74), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &eltwise_13_output_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  pool_16_output, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 37), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &pool_16_output_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  pool_18_output, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &pool_18_output_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  dense_20_output, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &dense_20_output_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  nl_21_output, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &nl_21_output_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_1_scale, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 12, 1, 1), AI_STRIDE_INIT(4, 4, 4, 48, 48),
  1, &eltwise_1_scale_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_1_bias, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 12, 1, 1), AI_STRIDE_INIT(4, 4, 4, 48, 48),
  1, &eltwise_1_bias_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_3_weights, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 3, 3, 1, 24), AI_STRIDE_INIT(4, 4, 12, 36, 36),
  1, &conv2d_3_weights_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_3_bias, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &conv2d_3_bias_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_8_scale, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &eltwise_8_scale_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_8_bias, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &eltwise_8_bias_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_11_weights, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 6, 3, 1, 24), AI_STRIDE_INIT(4, 4, 24, 72, 72),
  1, &conv2d_11_weights_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_11_bias, AI_STATIC,
  22, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &conv2d_11_bias_array, NULL)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_13_scale, AI_STATIC,
  23, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &eltwise_13_scale_array, NULL)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_13_bias, AI_STATIC,
  24, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &eltwise_13_bias_array, NULL)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  dense_20_weights, AI_STATIC,
  25, 0x0,
  AI_SHAPE_INIT(4, 24, 1, 1, 1), AI_STRIDE_INIT(4, 4, 96, 96, 96),
  1, &dense_20_weights_array, NULL)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  dense_20_bias, AI_STATIC,
  26, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &dense_20_bias_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  nl_21_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_20_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_21_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  nl_21_layer, 21,
  NL_TYPE, 0x0, NULL,
  nl, forward_sigmoid,
  &nl_21_chain,
  NULL, &nl_21_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_20_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_18_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_20_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &dense_20_weights, &dense_20_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_20_layer, 20,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &dense_20_chain,
  NULL, &nl_21_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pool_18_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_16_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_18_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pool_18_layer, 18,
  POOL_TYPE, 0x0, NULL,
  pool, forward_ap,
  &pool_18_chain,
  NULL, &dense_20_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(1, 37), 
  .pool_stride = AI_SHAPE_2D_INIT(1, 37), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pool_16_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_13_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_16_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pool_16_layer, 16,
  POOL_TYPE, 0x0, NULL,
  pool, forward_mp,
  &pool_16_chain,
  NULL, &pool_18_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(1, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(1, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  eltwise_13_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_11_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_13_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &eltwise_13_scale, &eltwise_13_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  eltwise_13_layer, 13,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &eltwise_13_chain,
  NULL, &pool_16_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_11_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_8_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_11_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_11_weights, &conv2d_11_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv2d_11_layer, 11,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d,
  &conv2d_11_chain,
  NULL, &eltwise_13_layer, AI_STATIC, 
  .groups = 4, 
  .nl_params = NULL, 
  .nl_func = nl_func_relu_array_f32, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  eltwise_8_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_6_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_8_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &eltwise_8_scale, &eltwise_8_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  eltwise_8_layer, 8,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &eltwise_8_chain,
  NULL, &conv2d_11_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pool_6_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_3_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_6_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pool_6_layer, 6,
  POOL_TYPE, 0x0, NULL,
  pool, forward_mp,
  &pool_6_chain,
  NULL, &eltwise_8_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(1, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(1, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_1_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_3_weights, &conv2d_3_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv2d_3_layer, 3,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d,
  &conv2d_3_chain,
  NULL, &pool_6_layer, AI_STATIC, 
  .groups = 4, 
  .nl_params = NULL, 
  .nl_func = nl_func_relu_array_f32, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  eltwise_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &serving_default_input0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &eltwise_1_scale, &eltwise_1_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  eltwise_1_layer, 1,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &eltwise_1_chain,
  NULL, &conv2d_3_layer, AI_STATIC, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 3364, 1, 1),
    3364, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 22128, 1, 1),
    22128, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &serving_default_input0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &nl_21_output),
  &eltwise_1_layer, 0, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 3364, 1, 1),
      3364, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 22128, 1, 1),
      22128, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &serving_default_input0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &nl_21_output),
  &eltwise_1_layer, 0, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/


/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_network_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    serving_default_input0_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    serving_default_input0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    
    eltwise_1_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    eltwise_1_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    
    conv2d_3_output_array.data = AI_PTR(g_network_activations_map[0] + 7440);
    conv2d_3_output_array.data_start = AI_PTR(g_network_activations_map[0] + 7440);
    
    pool_6_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    pool_6_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    
    eltwise_8_output_array.data = AI_PTR(g_network_activations_map[0] + 7296);
    eltwise_8_output_array.data_start = AI_PTR(g_network_activations_map[0] + 7296);
    
    conv2d_11_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    conv2d_11_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    
    eltwise_13_output_array.data = AI_PTR(g_network_activations_map[0] + 7104);
    eltwise_13_output_array.data_start = AI_PTR(g_network_activations_map[0] + 7104);
    
    pool_16_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    pool_16_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    
    pool_18_output_array.data = AI_PTR(g_network_activations_map[0] + 3552);
    pool_18_output_array.data_start = AI_PTR(g_network_activations_map[0] + 3552);
    
    dense_20_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    dense_20_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    
    nl_21_output_array.data = AI_PTR(g_network_activations_map[0] + 4);
    nl_21_output_array.data_start = AI_PTR(g_network_activations_map[0] + 4);
    
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_network_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    eltwise_1_scale_array.format |= AI_FMT_FLAG_CONST;
    eltwise_1_scale_array.data = AI_PTR(g_network_weights_map[0] + 0);
    eltwise_1_scale_array.data_start = AI_PTR(g_network_weights_map[0] + 0);
    
    eltwise_1_bias_array.format |= AI_FMT_FLAG_CONST;
    eltwise_1_bias_array.data = AI_PTR(g_network_weights_map[0] + 48);
    eltwise_1_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 48);
    
    conv2d_3_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_3_weights_array.data = AI_PTR(g_network_weights_map[0] + 96);
    conv2d_3_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 96);
    
    conv2d_3_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_3_bias_array.data = AI_PTR(g_network_weights_map[0] + 960);
    conv2d_3_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 960);
    
    eltwise_8_scale_array.format |= AI_FMT_FLAG_CONST;
    eltwise_8_scale_array.data = AI_PTR(g_network_weights_map[0] + 1056);
    eltwise_8_scale_array.data_start = AI_PTR(g_network_weights_map[0] + 1056);
    
    eltwise_8_bias_array.format |= AI_FMT_FLAG_CONST;
    eltwise_8_bias_array.data = AI_PTR(g_network_weights_map[0] + 1152);
    eltwise_8_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 1152);
    
    conv2d_11_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_11_weights_array.data = AI_PTR(g_network_weights_map[0] + 1248);
    conv2d_11_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 1248);
    
    conv2d_11_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_11_bias_array.data = AI_PTR(g_network_weights_map[0] + 2976);
    conv2d_11_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 2976);
    
    eltwise_13_scale_array.format |= AI_FMT_FLAG_CONST;
    eltwise_13_scale_array.data = AI_PTR(g_network_weights_map[0] + 3072);
    eltwise_13_scale_array.data_start = AI_PTR(g_network_weights_map[0] + 3072);
    
    eltwise_13_bias_array.format |= AI_FMT_FLAG_CONST;
    eltwise_13_bias_array.data = AI_PTR(g_network_weights_map[0] + 3168);
    eltwise_13_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 3168);
    
    dense_20_weights_array.format |= AI_FMT_FLAG_CONST;
    dense_20_weights_array.data = AI_PTR(g_network_weights_map[0] + 3264);
    dense_20_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 3264);
    
    dense_20_bias_array.format |= AI_FMT_FLAG_CONST;
    dense_20_bias_array.data = AI_PTR(g_network_weights_map[0] + 3360);
    dense_20_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 3360);
    
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/


AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_network_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 282827,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_bool ai_network_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 282827,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}

AI_API_ENTRY
ai_error ai_network_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}

AI_API_ENTRY
ai_error ai_network_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    &AI_NET_OBJ_INSTANCE,
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}

AI_API_ENTRY
ai_error ai_network_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
    ai_error err;
    ai_network_params params;

    err = ai_network_create(network, AI_NETWORK_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE)
        return err;
    if (ai_network_data_params_get(&params) != true) {
        err = ai_network_get_error(*network);
        return err;
    }
#if defined(AI_NETWORK_DATA_ACTIVATIONS_COUNT)
    if (activations) {
        /* set the addresses of the activations buffers */
        for (int idx=0;idx<params.map_activations.size;idx++)
            AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
    }
#endif
#if defined(AI_NETWORK_DATA_WEIGHTS_COUNT)
    if (weights) {
        /* set the addresses of the weight buffers */
        for (int idx=0;idx<params.map_weights.size;idx++)
            AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
    }
#endif
    if (ai_network_init(*network, &params) != true) {
        err = ai_network_get_error(*network);
    }
    return err;
}

AI_API_ENTRY
ai_buffer* ai_network_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    ((ai_network *)network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}

AI_API_ENTRY
ai_buffer* ai_network_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    ((ai_network *)network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}

AI_API_ENTRY
ai_handle ai_network_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}

AI_API_ENTRY
ai_bool ai_network_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = ai_platform_network_init(network, params);
  if (!net_ctx) return false;

  ai_bool ok = true;
  ok &= network_configure_weights(net_ctx, params);
  ok &= network_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_network_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}

AI_API_ENTRY
ai_i32 ai_network_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_NETWORK_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

