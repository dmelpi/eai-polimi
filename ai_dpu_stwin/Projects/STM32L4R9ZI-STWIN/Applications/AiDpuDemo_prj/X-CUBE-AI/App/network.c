/**
  ******************************************************************************
  * @file    network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Fri May 27 11:28:14 2022
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
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
#define AI_NETWORK_MODEL_SIGNATURE     "a67e1b7825edae5446509ca4379ef392"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Fri May 27 11:28:14 2022"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_N_BATCHES
#define AI_NETWORK_N_BATCHES         (1)




/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  float_input_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 128, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  variable_sum_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  variable_mul_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  label_output0_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 1, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  label_output1_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  Csvc_5_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  Cneg_5_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  Csvc_4_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  Cneg_4_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  Csvc_3_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  Cneg_3_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  Csvc_1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  Cneg_1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  svcsumc_2_left_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  svcsumc_2_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  Csvc_2_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  Cneg_2_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  svcsumc_3_left_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  svcsumc_3_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  Csvc_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  svcsumc_0_left_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  svcsumc_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  Cneg_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  svcsumc_1_left_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  svcsumc_1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#25 */
AI_ARRAY_OBJ_DECLARE(
  Csvcconc_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4, AI_STATIC)

/* Array#26 */
AI_ARRAY_OBJ_DECLARE(
  Cabs_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4, AI_STATIC)

/* Array#27 */
AI_ARRAY_OBJ_DECLARE(
  Cconc_abs1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4, AI_STATIC)

/* Array#28 */
AI_ARRAY_OBJ_DECLARE(
  Cconc_abs3_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4, AI_STATIC)

/* Array#29 */
AI_ARRAY_OBJ_DECLARE(
  Csvcfinal_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4, AI_STATIC)

/* Array#30 */
AI_ARRAY_OBJ_DECLARE(
  prediction_output_array, AI_ARRAY_FORMAT_BOOL,
  NULL, NULL, 6, AI_STATIC)

/* Array#31 */
AI_ARRAY_OBJ_DECLARE(
  iprediction_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6, AI_STATIC)

/* Array#32 */
AI_ARRAY_OBJ_DECLARE(
  Vsvcv_5_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#33 */
AI_ARRAY_OBJ_DECLARE(
  Vnegv_5_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#34 */
AI_ARRAY_OBJ_DECLARE(
  Vnegv1_5_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#35 */
AI_ARRAY_OBJ_DECLARE(
  Vsvcv_4_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#36 */
AI_ARRAY_OBJ_DECLARE(
  Vnegv_4_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#37 */
AI_ARRAY_OBJ_DECLARE(
  Vnegv1_4_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#38 */
AI_ARRAY_OBJ_DECLARE(
  Vsvcv_3_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#39 */
AI_ARRAY_OBJ_DECLARE(
  Vnegv_3_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#40 */
AI_ARRAY_OBJ_DECLARE(
  Vnegv1_3_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#41 */
AI_ARRAY_OBJ_DECLARE(
  Vsvcv_1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#42 */
AI_ARRAY_OBJ_DECLARE(
  svcvote_2_left_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#43 */
AI_ARRAY_OBJ_DECLARE(
  svcvote_2_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#44 */
AI_ARRAY_OBJ_DECLARE(
  Vnegv_1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#45 */
AI_ARRAY_OBJ_DECLARE(
  Vnegv1_1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#46 */
AI_ARRAY_OBJ_DECLARE(
  Vsvcv_2_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#47 */
AI_ARRAY_OBJ_DECLARE(
  svcvote_3_left_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#48 */
AI_ARRAY_OBJ_DECLARE(
  svcvote_3_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#49 */
AI_ARRAY_OBJ_DECLARE(
  Vnegv_2_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#50 */
AI_ARRAY_OBJ_DECLARE(
  Vnegv1_2_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#51 */
AI_ARRAY_OBJ_DECLARE(
  Vsvcv_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#52 */
AI_ARRAY_OBJ_DECLARE(
  svcvote_1_left_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#53 */
AI_ARRAY_OBJ_DECLARE(
  svcvote_1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#54 */
AI_ARRAY_OBJ_DECLARE(
  Vnegv_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#55 */
AI_ARRAY_OBJ_DECLARE(
  Vnegv1_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#56 */
AI_ARRAY_OBJ_DECLARE(
  svcvote_0_left_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#57 */
AI_ARRAY_OBJ_DECLARE(
  svcvote_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#58 */
AI_ARRAY_OBJ_DECLARE(
  Vsvcconcv_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4, AI_STATIC)

/* Array#59 */
AI_ARRAY_OBJ_DECLARE(
  probabilities_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 4, AI_STATIC)

/* Array#60 */
AI_ARRAY_OBJ_DECLARE(
  variable_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#61 */
AI_ARRAY_OBJ_DECLARE(
  variable_offset_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#62 */
AI_ARRAY_OBJ_DECLARE(
  label_coefficients_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2970, AI_STATIC)

/* Array#63 */
AI_ARRAY_OBJ_DECLARE(
  label_support_vectors_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 126720, AI_STATIC)

/* Array#64 */
AI_ARRAY_OBJ_DECLARE(
  label_rho_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6, AI_STATIC)

/* Array#65 */
AI_ARRAY_OBJ_DECLARE(
  label_classlabels_ints_array, AI_ARRAY_FORMAT_U8,
  NULL, NULL, 4, AI_STATIC)

/* Array#66 */
AI_ARRAY_OBJ_DECLARE(
  label_vectors_per_class_array, AI_ARRAY_FORMAT_U16,
  NULL, NULL, 4, AI_STATIC)

/* Array#67 */
AI_ARRAY_OBJ_DECLARE(
  Csvc_5_indices_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#68 */
AI_ARRAY_OBJ_DECLARE(
  Csvc_4_indices_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#69 */
AI_ARRAY_OBJ_DECLARE(
  Csvc_3_indices_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#70 */
AI_ARRAY_OBJ_DECLARE(
  Csvc_1_indices_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#71 */
AI_ARRAY_OBJ_DECLARE(
  Csvc_2_indices_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#72 */
AI_ARRAY_OBJ_DECLARE(
  Csvc_0_indices_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#73 */
AI_ARRAY_OBJ_DECLARE(
  cst3_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#74 */
AI_ARRAY_OBJ_DECLARE(
  cst1_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#75 */
AI_ARRAY_OBJ_DECLARE(
  cst0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#76 */
AI_ARRAY_OBJ_DECLARE(
  Vsvcv_5_indices_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#77 */
AI_ARRAY_OBJ_DECLARE(
  Vsvcv_4_indices_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#78 */
AI_ARRAY_OBJ_DECLARE(
  Vsvcv_3_indices_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#79 */
AI_ARRAY_OBJ_DECLARE(
  Vsvcv_1_indices_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#80 */
AI_ARRAY_OBJ_DECLARE(
  Vsvcv_2_indices_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#81 */
AI_ARRAY_OBJ_DECLARE(
  Vsvcv_0_indices_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#82 */
AI_ARRAY_OBJ_DECLARE(
  label_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1035, AI_STATIC)

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  float_input_output, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &float_input_output_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  variable_sum_output, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &variable_sum_output_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  variable_mul_output, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &variable_mul_output_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  label_output0, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &label_output0_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  label_output1, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 6, 1, 1), AI_STRIDE_INIT(4, 4, 4, 24, 24),
  1, &label_output1_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  Csvc_5_output, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Csvc_5_output_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  Cneg_5_output, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Cneg_5_output_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  Csvc_4_output, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Csvc_4_output_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  Cneg_4_output, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Cneg_4_output_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  Csvc_3_output, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Csvc_3_output_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  Cneg_3_output, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Cneg_3_output_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  Csvc_1_output, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Csvc_1_output_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  Cneg_1_output, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Cneg_1_output_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  svcsumc_2_left_0_output, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcsumc_2_left_0_output_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  svcsumc_2_output, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcsumc_2_output_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  Csvc_2_output, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Csvc_2_output_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  Cneg_2_output, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Cneg_2_output_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  svcsumc_3_left_0_output, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcsumc_3_left_0_output_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  svcsumc_3_output, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcsumc_3_output_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  Csvc_0_output, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Csvc_0_output_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  svcsumc_0_left_0_output, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcsumc_0_left_0_output_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  svcsumc_0_output, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcsumc_0_output_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  Cneg_0_output, AI_STATIC,
  22, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Cneg_0_output_array, NULL)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  svcsumc_1_left_0_output, AI_STATIC,
  23, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcsumc_1_left_0_output_array, NULL)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  svcsumc_1_output, AI_STATIC,
  24, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcsumc_1_output_array, NULL)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  Csvcconc_output, AI_STATIC,
  25, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &Csvcconc_output_array, NULL)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  Cabs_output, AI_STATIC,
  26, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &Cabs_output_array, NULL)

/* Tensor #27 */
AI_TENSOR_OBJ_DECLARE(
  Cconc_abs1_output, AI_STATIC,
  27, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &Cconc_abs1_output_array, NULL)

/* Tensor #28 */
AI_TENSOR_OBJ_DECLARE(
  Cconc_abs3_output, AI_STATIC,
  28, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &Cconc_abs3_output_array, NULL)

/* Tensor #29 */
AI_TENSOR_OBJ_DECLARE(
  Csvcfinal_output, AI_STATIC,
  29, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &Csvcfinal_output_array, NULL)

/* Tensor #30 */
AI_TENSOR_OBJ_DECLARE(
  prediction_output, AI_STATIC,
  30, 0x0,
  AI_SHAPE_INIT(4, 1, 6, 1, 1), AI_STRIDE_INIT(4, 1, 1, 6, 6),
  1, &prediction_output_array, NULL)

/* Tensor #31 */
AI_TENSOR_OBJ_DECLARE(
  iprediction_output, AI_STATIC,
  31, 0x0,
  AI_SHAPE_INIT(4, 1, 6, 1, 1), AI_STRIDE_INIT(4, 4, 4, 24, 24),
  1, &iprediction_output_array, NULL)

/* Tensor #32 */
AI_TENSOR_OBJ_DECLARE(
  Vsvcv_5_output, AI_STATIC,
  32, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vsvcv_5_output_array, NULL)

/* Tensor #33 */
AI_TENSOR_OBJ_DECLARE(
  Vnegv_5_output, AI_STATIC,
  33, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vnegv_5_output_array, NULL)

/* Tensor #34 */
AI_TENSOR_OBJ_DECLARE(
  Vnegv1_5_output, AI_STATIC,
  34, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vnegv1_5_output_array, NULL)

/* Tensor #35 */
AI_TENSOR_OBJ_DECLARE(
  Vsvcv_4_output, AI_STATIC,
  35, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vsvcv_4_output_array, NULL)

/* Tensor #36 */
AI_TENSOR_OBJ_DECLARE(
  Vnegv_4_output, AI_STATIC,
  36, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vnegv_4_output_array, NULL)

/* Tensor #37 */
AI_TENSOR_OBJ_DECLARE(
  Vnegv1_4_output, AI_STATIC,
  37, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vnegv1_4_output_array, NULL)

/* Tensor #38 */
AI_TENSOR_OBJ_DECLARE(
  Vsvcv_3_output, AI_STATIC,
  38, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vsvcv_3_output_array, NULL)

/* Tensor #39 */
AI_TENSOR_OBJ_DECLARE(
  Vnegv_3_output, AI_STATIC,
  39, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vnegv_3_output_array, NULL)

/* Tensor #40 */
AI_TENSOR_OBJ_DECLARE(
  Vnegv1_3_output, AI_STATIC,
  40, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vnegv1_3_output_array, NULL)

/* Tensor #41 */
AI_TENSOR_OBJ_DECLARE(
  Vsvcv_1_output, AI_STATIC,
  41, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vsvcv_1_output_array, NULL)

/* Tensor #42 */
AI_TENSOR_OBJ_DECLARE(
  svcvote_2_left_0_output, AI_STATIC,
  42, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcvote_2_left_0_output_array, NULL)

/* Tensor #43 */
AI_TENSOR_OBJ_DECLARE(
  svcvote_2_output, AI_STATIC,
  43, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcvote_2_output_array, NULL)

/* Tensor #44 */
AI_TENSOR_OBJ_DECLARE(
  Vnegv_1_output, AI_STATIC,
  44, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vnegv_1_output_array, NULL)

/* Tensor #45 */
AI_TENSOR_OBJ_DECLARE(
  Vnegv1_1_output, AI_STATIC,
  45, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vnegv1_1_output_array, NULL)

/* Tensor #46 */
AI_TENSOR_OBJ_DECLARE(
  Vsvcv_2_output, AI_STATIC,
  46, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vsvcv_2_output_array, NULL)

/* Tensor #47 */
AI_TENSOR_OBJ_DECLARE(
  svcvote_3_left_0_output, AI_STATIC,
  47, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcvote_3_left_0_output_array, NULL)

/* Tensor #48 */
AI_TENSOR_OBJ_DECLARE(
  svcvote_3_output, AI_STATIC,
  48, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcvote_3_output_array, NULL)

/* Tensor #49 */
AI_TENSOR_OBJ_DECLARE(
  Vnegv_2_output, AI_STATIC,
  49, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vnegv_2_output_array, NULL)

/* Tensor #50 */
AI_TENSOR_OBJ_DECLARE(
  Vnegv1_2_output, AI_STATIC,
  50, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vnegv1_2_output_array, NULL)

/* Tensor #51 */
AI_TENSOR_OBJ_DECLARE(
  Vsvcv_0_output, AI_STATIC,
  51, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vsvcv_0_output_array, NULL)

/* Tensor #52 */
AI_TENSOR_OBJ_DECLARE(
  svcvote_1_left_0_output, AI_STATIC,
  52, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcvote_1_left_0_output_array, NULL)

/* Tensor #53 */
AI_TENSOR_OBJ_DECLARE(
  svcvote_1_output, AI_STATIC,
  53, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcvote_1_output_array, NULL)

/* Tensor #54 */
AI_TENSOR_OBJ_DECLARE(
  Vnegv_0_output, AI_STATIC,
  54, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vnegv_0_output_array, NULL)

/* Tensor #55 */
AI_TENSOR_OBJ_DECLARE(
  Vnegv1_0_output, AI_STATIC,
  55, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vnegv1_0_output_array, NULL)

/* Tensor #56 */
AI_TENSOR_OBJ_DECLARE(
  svcvote_0_left_0_output, AI_STATIC,
  56, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcvote_0_left_0_output_array, NULL)

/* Tensor #57 */
AI_TENSOR_OBJ_DECLARE(
  svcvote_0_output, AI_STATIC,
  57, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &svcvote_0_output_array, NULL)

/* Tensor #58 */
AI_TENSOR_OBJ_DECLARE(
  Vsvcconcv_output, AI_STATIC,
  58, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &Vsvcconcv_output_array, NULL)

/* Tensor #59 */
AI_TENSOR_OBJ_DECLARE(
  probabilities_output, AI_STATIC,
  59, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &probabilities_output_array, NULL)

/* Tensor #60 */
AI_TENSOR_OBJ_DECLARE(
  variable_scale, AI_STATIC,
  60, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &variable_scale_array, NULL)

/* Tensor #61 */
AI_TENSOR_OBJ_DECLARE(
  variable_offset, AI_STATIC,
  61, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &variable_offset_array, NULL)

/* Tensor #62 */
AI_TENSOR_OBJ_DECLARE(
  label_coefficients, AI_STATIC,
  62, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 2970), AI_STRIDE_INIT(4, 4, 4, 11880, 11880),
  1, &label_coefficients_array, NULL)

/* Tensor #63 */
AI_TENSOR_OBJ_DECLARE(
  label_support_vectors, AI_STATIC,
  63, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 126720), AI_STRIDE_INIT(4, 4, 4, 506880, 506880),
  1, &label_support_vectors_array, NULL)

/* Tensor #64 */
AI_TENSOR_OBJ_DECLARE(
  label_rho, AI_STATIC,
  64, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 6), AI_STRIDE_INIT(4, 4, 4, 24, 24),
  1, &label_rho_array, NULL)

/* Tensor #65 */
AI_TENSOR_OBJ_DECLARE(
  label_classlabels_ints, AI_STATIC,
  65, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 4), AI_STRIDE_INIT(4, 1, 1, 4, 4),
  1, &label_classlabels_ints_array, NULL)

/* Tensor #66 */
AI_TENSOR_OBJ_DECLARE(
  label_vectors_per_class, AI_STATIC,
  66, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 4), AI_STRIDE_INIT(4, 2, 2, 8, 8),
  1, &label_vectors_per_class_array, NULL)

/* Tensor #67 */
AI_TENSOR_OBJ_DECLARE(
  Csvc_5_indices, AI_STATIC,
  67, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Csvc_5_indices_array, NULL)

/* Tensor #68 */
AI_TENSOR_OBJ_DECLARE(
  Csvc_4_indices, AI_STATIC,
  68, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Csvc_4_indices_array, NULL)

/* Tensor #69 */
AI_TENSOR_OBJ_DECLARE(
  Csvc_3_indices, AI_STATIC,
  69, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Csvc_3_indices_array, NULL)

/* Tensor #70 */
AI_TENSOR_OBJ_DECLARE(
  Csvc_1_indices, AI_STATIC,
  70, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Csvc_1_indices_array, NULL)

/* Tensor #71 */
AI_TENSOR_OBJ_DECLARE(
  Csvc_2_indices, AI_STATIC,
  71, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Csvc_2_indices_array, NULL)

/* Tensor #72 */
AI_TENSOR_OBJ_DECLARE(
  Csvc_0_indices, AI_STATIC,
  72, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Csvc_0_indices_array, NULL)

/* Tensor #73 */
AI_TENSOR_OBJ_DECLARE(
  cst3, AI_STATIC,
  73, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &cst3_array, NULL)

/* Tensor #74 */
AI_TENSOR_OBJ_DECLARE(
  cst1, AI_STATIC,
  74, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &cst1_array, NULL)

/* Tensor #75 */
AI_TENSOR_OBJ_DECLARE(
  cst0, AI_STATIC,
  75, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &cst0_array, NULL)

/* Tensor #76 */
AI_TENSOR_OBJ_DECLARE(
  Vsvcv_5_indices, AI_STATIC,
  76, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vsvcv_5_indices_array, NULL)

/* Tensor #77 */
AI_TENSOR_OBJ_DECLARE(
  Vsvcv_4_indices, AI_STATIC,
  77, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vsvcv_4_indices_array, NULL)

/* Tensor #78 */
AI_TENSOR_OBJ_DECLARE(
  Vsvcv_3_indices, AI_STATIC,
  78, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vsvcv_3_indices_array, NULL)

/* Tensor #79 */
AI_TENSOR_OBJ_DECLARE(
  Vsvcv_1_indices, AI_STATIC,
  79, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vsvcv_1_indices_array, NULL)

/* Tensor #80 */
AI_TENSOR_OBJ_DECLARE(
  Vsvcv_2_indices, AI_STATIC,
  80, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vsvcv_2_indices_array, NULL)

/* Tensor #81 */
AI_TENSOR_OBJ_DECLARE(
  Vsvcv_0_indices, AI_STATIC,
  81, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &Vsvcv_0_indices_array, NULL)

/* Tensor #82 */
AI_TENSOR_OBJ_DECLARE(
  label_scratch0, AI_STATIC,
  82, 0x0,
  AI_SHAPE_INIT(4, 1, 1035, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4140, 4140),
  1, &label_scratch0_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  probabilities_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Vsvcconcv_output, &Csvcfinal_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &probabilities_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  probabilities_layer, 49,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &probabilities_chain,
  NULL, &probabilities_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vsvcconcv_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 4, &svcvote_0_output, &svcvote_1_output, &svcvote_2_output, &svcvote_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcconcv_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vsvcconcv_layer, 46,
  CONCAT_TYPE, 0x0, NULL,
  concat, forward_concat,
  &Vsvcconcv_chain,
  NULL, &probabilities_layer, AI_STATIC, 
  .axis = AI_SHAPE_CHANNEL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcvote_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &svcvote_0_left_0_output, &Vnegv1_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcvote_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcvote_0_layer, 44,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcvote_0_chain,
  NULL, &Vsvcconcv_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcvote_0_left_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Vnegv1_0_output, &Vnegv1_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcvote_0_left_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcvote_0_left_0_layer, 44,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcvote_0_left_0_chain,
  NULL, &svcvote_0_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vnegv1_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Vnegv_0_output, &cst1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vnegv1_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vnegv1_0_layer, 36,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &Vnegv1_0_chain,
  NULL, &svcvote_0_left_0_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vnegv_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vnegv_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vnegv_0_layer, 34,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &Vnegv_0_chain,
  NULL, &Vnegv1_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcvote_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &svcvote_1_left_0_output, &Vnegv1_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcvote_1_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcvote_1_layer, 42,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcvote_1_chain,
  NULL, &Vnegv_0_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcvote_1_left_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Vsvcv_0_output, &Vnegv1_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcvote_1_left_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcvote_1_left_0_layer, 42,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcvote_1_left_0_chain,
  NULL, &svcvote_1_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vsvcv_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &iprediction_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_0_indices),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vsvcv_0_layer, 18,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &Vsvcv_0_chain,
  NULL, &svcvote_1_left_0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vnegv1_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Vnegv_2_output, &cst1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vnegv1_2_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vnegv1_2_layer, 38,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &Vnegv1_2_chain,
  NULL, &Vsvcv_0_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vnegv_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vnegv_2_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vnegv_2_layer, 30,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &Vnegv_2_chain,
  NULL, &Vnegv1_2_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcvote_3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &svcvote_3_left_0_output, &Vsvcv_5_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcvote_3_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcvote_3_layer, 29,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcvote_3_chain,
  NULL, &Vnegv_2_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcvote_3_left_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Vsvcv_2_output, &Vsvcv_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcvote_3_left_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcvote_3_left_0_layer, 29,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcvote_3_left_0_chain,
  NULL, &svcvote_3_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vsvcv_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &iprediction_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_2_indices),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vsvcv_2_layer, 19,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &Vsvcv_2_chain,
  NULL, &svcvote_3_left_0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vnegv1_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Vnegv_1_output, &cst1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vnegv1_1_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vnegv1_1_layer, 35,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &Vnegv1_1_chain,
  NULL, &Vsvcv_2_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vnegv_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vnegv_1_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vnegv_1_layer, 27,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &Vnegv_1_chain,
  NULL, &Vnegv1_1_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcvote_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &svcvote_2_left_0_output, &Vnegv1_5_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcvote_2_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcvote_2_layer, 43,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcvote_2_chain,
  NULL, &Vnegv_1_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcvote_2_left_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Vsvcv_1_output, &Vsvcv_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcvote_2_left_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcvote_2_left_0_layer, 43,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcvote_2_left_0_chain,
  NULL, &svcvote_2_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vsvcv_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &iprediction_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_1_indices),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vsvcv_1_layer, 20,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &Vsvcv_1_chain,
  NULL, &svcvote_2_left_0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vnegv1_3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Vnegv_3_output, &cst1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vnegv1_3_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vnegv1_3_layer, 39,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &Vnegv1_3_chain,
  NULL, &Vsvcv_1_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vnegv_3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vnegv_3_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vnegv_3_layer, 31,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &Vnegv_3_chain,
  NULL, &Vnegv1_3_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vsvcv_3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &iprediction_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_3_indices),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vsvcv_3_layer, 21,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &Vsvcv_3_chain,
  NULL, &Vnegv_3_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vnegv1_4_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Vnegv_4_output, &cst1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vnegv1_4_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vnegv1_4_layer, 40,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &Vnegv1_4_chain,
  NULL, &Vsvcv_3_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vnegv_4_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vnegv_4_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vnegv_4_layer, 32,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &Vnegv_4_chain,
  NULL, &Vnegv1_4_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vsvcv_4_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &iprediction_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_4_indices),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vsvcv_4_layer, 22,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &Vsvcv_4_chain,
  NULL, &Vnegv_4_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vnegv1_5_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Vnegv_5_output, &cst1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vnegv1_5_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vnegv1_5_layer, 41,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &Vnegv1_5_chain,
  NULL, &Vsvcv_4_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vnegv_5_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_5_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vnegv_5_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vnegv_5_layer, 33,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &Vnegv_5_chain,
  NULL, &Vnegv1_5_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Vsvcv_5_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &iprediction_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_5_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Vsvcv_5_indices),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Vsvcv_5_layer, 24,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &Vsvcv_5_chain,
  NULL, &Vnegv_5_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  iprediction_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &prediction_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &iprediction_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  iprediction_layer, 17,
  CAST_TYPE, 0x0, NULL,
  cast, forward_cast,
  &iprediction_chain,
  NULL, &Vsvcv_5_layer, AI_STATIC, 
  .to_format = AI_ARRAY_FORMAT_FLOAT, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  prediction_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &label_output1, &cst0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &prediction_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  prediction_layer, 4,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &prediction_chain,
  NULL, &iprediction_layer, AI_STATIC, 
  .operation = ai_less, 
  .buffer_operation = ai_less_buffer, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Csvcfinal_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Csvcconc_output, &Cconc_abs3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvcfinal_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Csvcfinal_layer, 48,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &Csvcfinal_chain,
  NULL, &prediction_layer, AI_STATIC, 
  .operation = ai_div_f32, 
  .buffer_operation = ai_div_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Cconc_abs3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Cconc_abs1_output, &cst3),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Cconc_abs3_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Cconc_abs3_layer, 47,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &Cconc_abs3_chain,
  NULL, &Csvcfinal_layer, AI_STATIC, 
  .operation = ai_mul_f32, 
  .buffer_operation = ai_mul_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Cconc_abs1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Cabs_output, &cst1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Cconc_abs1_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Cconc_abs1_layer, 45,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &Cconc_abs1_chain,
  NULL, &Cconc_abs3_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Cabs_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvcconc_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Cabs_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Cabs_layer, 37,
  NL_TYPE, 0x0, NULL,
  nl, forward_abs,
  &Cabs_chain,
  NULL, &Cconc_abs1_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Csvcconc_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 4, &svcsumc_0_output, &svcsumc_1_output, &svcsumc_2_output, &svcsumc_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvcconc_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Csvcconc_layer, 28,
  CONCAT_TYPE, 0x0, NULL,
  concat, forward_concat,
  &Csvcconc_chain,
  NULL, &Cabs_layer, AI_STATIC, 
  .axis = AI_SHAPE_CHANNEL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcsumc_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &svcsumc_1_left_0_output, &Csvc_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcsumc_1_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcsumc_1_layer, 26,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcsumc_1_chain,
  NULL, &Csvcconc_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcsumc_1_left_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Cneg_0_output, &Csvc_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcsumc_1_left_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcsumc_1_left_0_layer, 26,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcsumc_1_left_0_chain,
  NULL, &svcsumc_1_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Cneg_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Cneg_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Cneg_0_layer, 10,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &Cneg_0_chain,
  NULL, &svcsumc_1_left_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcsumc_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &svcsumc_0_left_0_output, &Csvc_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcsumc_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcsumc_0_layer, 16,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcsumc_0_chain,
  NULL, &Cneg_0_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcsumc_0_left_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Csvc_0_output, &Csvc_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcsumc_0_left_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcsumc_0_left_0_layer, 16,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcsumc_0_left_0_chain,
  NULL, &svcsumc_0_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Csvc_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &label_output1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_0_indices),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Csvc_0_layer, 3,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &Csvc_0_chain,
  NULL, &svcsumc_0_left_0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcsumc_3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &svcsumc_3_left_0_output, &Cneg_5_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcsumc_3_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcsumc_3_layer, 25,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcsumc_3_chain,
  NULL, &Csvc_0_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcsumc_3_left_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Cneg_2_output, &Cneg_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcsumc_3_left_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcsumc_3_left_0_layer, 25,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcsumc_3_left_0_chain,
  NULL, &svcsumc_3_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Cneg_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Cneg_2_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Cneg_2_layer, 11,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &Cneg_2_chain,
  NULL, &svcsumc_3_left_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Csvc_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &label_output1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_2_indices),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Csvc_2_layer, 5,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &Csvc_2_chain,
  NULL, &Cneg_2_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcsumc_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &svcsumc_2_left_0_output, &Csvc_5_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcsumc_2_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcsumc_2_layer, 23,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcsumc_2_chain,
  NULL, &Csvc_2_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  svcsumc_2_left_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Cneg_1_output, &Cneg_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &svcsumc_2_left_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  svcsumc_2_left_0_layer, 23,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &svcsumc_2_left_0_chain,
  NULL, &svcsumc_2_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Cneg_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Cneg_1_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Cneg_1_layer, 12,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &Cneg_1_chain,
  NULL, &svcsumc_2_left_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Csvc_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &label_output1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_1_indices),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Csvc_1_layer, 6,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &Csvc_1_chain,
  NULL, &Cneg_1_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Cneg_3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Cneg_3_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Cneg_3_layer, 13,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &Cneg_3_chain,
  NULL, &Csvc_1_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Csvc_3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &label_output1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_3_indices),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Csvc_3_layer, 7,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &Csvc_3_chain,
  NULL, &Cneg_3_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Cneg_4_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Cneg_4_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Cneg_4_layer, 14,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &Cneg_4_chain,
  NULL, &Csvc_3_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Csvc_4_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &label_output1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_4_indices),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Csvc_4_layer, 8,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &Csvc_4_chain,
  NULL, &Cneg_4_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Cneg_5_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_5_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Cneg_5_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Cneg_5_layer, 15,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &Cneg_5_chain,
  NULL, &Csvc_4_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Csvc_5_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &label_output1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_5_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Csvc_5_indices),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Csvc_5_layer, 9,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &Csvc_5_chain,
  NULL, &Cneg_5_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  label_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &variable_mul_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &label_output0, &label_output1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 5, &label_coefficients, &label_support_vectors, &label_rho, &label_classlabels_ints, &label_vectors_per_class),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &label_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  label_layer, 1,
  SVC_TYPE, 0x0, NULL,
  svc, forward_svc,
  &label_chain,
  NULL, &Csvc_5_layer, AI_STATIC, 
  .kernel_type = AI_SVC_KERNEL_RBF, 
  .gamma = 0.0078125, 
  .coef0 = 0.0, 
  .degree = 3.0, 
  .proba_support = 0, 
  .has_classlabels_int = 1, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  variable_mul_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &variable_sum_output, &variable_scale),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &variable_mul_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  variable_mul_layer, 0,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &variable_mul_chain,
  NULL, &label_layer, AI_STATIC, 
  .operation = ai_mul_f32, 
  .buffer_operation = ai_mul_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  variable_sum_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &float_input_output, &variable_offset),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &variable_sum_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  variable_sum_layer, 0,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &variable_sum_chain,
  NULL, &variable_mul_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 519880, 1, 1),
    519880, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 4676, 1, 1),
    4676, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &float_input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &label_output0, &probabilities_output),
  &variable_sum_layer, 0, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 519880, 1, 1),
      519880, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 4676, 1, 1),
      4676, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &float_input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &label_output0, &probabilities_output),
  &variable_sum_layer, 0, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/


/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  ai_ptr activations_map[1] = AI_C_ARRAY_INIT;

  if (ai_platform_get_activations_map(activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    variable_sum_output_array.data = AI_PTR(activations_map[0] + 0);
    variable_sum_output_array.data_start = AI_PTR(activations_map[0] + 0);
    variable_mul_output_array.data = AI_PTR(activations_map[0] + 0);
    variable_mul_output_array.data_start = AI_PTR(activations_map[0] + 0);
    label_scratch0_array.data = AI_PTR(activations_map[0] + 512);
    label_scratch0_array.data_start = AI_PTR(activations_map[0] + 512);
    label_output1_array.data = AI_PTR(activations_map[0] + 4652);
    label_output1_array.data_start = AI_PTR(activations_map[0] + 4652);
    Csvc_5_output_array.data = AI_PTR(activations_map[0] + 0);
    Csvc_5_output_array.data_start = AI_PTR(activations_map[0] + 0);
    Cneg_5_output_array.data = AI_PTR(activations_map[0] + 4);
    Cneg_5_output_array.data_start = AI_PTR(activations_map[0] + 4);
    Csvc_4_output_array.data = AI_PTR(activations_map[0] + 8);
    Csvc_4_output_array.data_start = AI_PTR(activations_map[0] + 8);
    Cneg_4_output_array.data = AI_PTR(activations_map[0] + 12);
    Cneg_4_output_array.data_start = AI_PTR(activations_map[0] + 12);
    Csvc_3_output_array.data = AI_PTR(activations_map[0] + 16);
    Csvc_3_output_array.data_start = AI_PTR(activations_map[0] + 16);
    Cneg_3_output_array.data = AI_PTR(activations_map[0] + 20);
    Cneg_3_output_array.data_start = AI_PTR(activations_map[0] + 20);
    Csvc_1_output_array.data = AI_PTR(activations_map[0] + 24);
    Csvc_1_output_array.data_start = AI_PTR(activations_map[0] + 24);
    Cneg_1_output_array.data = AI_PTR(activations_map[0] + 28);
    Cneg_1_output_array.data_start = AI_PTR(activations_map[0] + 28);
    svcsumc_2_left_0_output_array.data = AI_PTR(activations_map[0] + 32);
    svcsumc_2_left_0_output_array.data_start = AI_PTR(activations_map[0] + 32);
    svcsumc_2_output_array.data = AI_PTR(activations_map[0] + 20);
    svcsumc_2_output_array.data_start = AI_PTR(activations_map[0] + 20);
    Csvc_2_output_array.data = AI_PTR(activations_map[0] + 0);
    Csvc_2_output_array.data_start = AI_PTR(activations_map[0] + 0);
    Cneg_2_output_array.data = AI_PTR(activations_map[0] + 28);
    Cneg_2_output_array.data_start = AI_PTR(activations_map[0] + 28);
    svcsumc_3_left_0_output_array.data = AI_PTR(activations_map[0] + 32);
    svcsumc_3_left_0_output_array.data_start = AI_PTR(activations_map[0] + 32);
    svcsumc_3_output_array.data = AI_PTR(activations_map[0] + 12);
    svcsumc_3_output_array.data_start = AI_PTR(activations_map[0] + 12);
    Csvc_0_output_array.data = AI_PTR(activations_map[0] + 4);
    Csvc_0_output_array.data_start = AI_PTR(activations_map[0] + 4);
    svcsumc_0_left_0_output_array.data = AI_PTR(activations_map[0] + 28);
    svcsumc_0_left_0_output_array.data_start = AI_PTR(activations_map[0] + 28);
    svcsumc_0_output_array.data = AI_PTR(activations_map[0] + 24);
    svcsumc_0_output_array.data_start = AI_PTR(activations_map[0] + 24);
    Cneg_0_output_array.data = AI_PTR(activations_map[0] + 0);
    Cneg_0_output_array.data_start = AI_PTR(activations_map[0] + 0);
    svcsumc_1_left_0_output_array.data = AI_PTR(activations_map[0] + 4);
    svcsumc_1_left_0_output_array.data_start = AI_PTR(activations_map[0] + 4);
    svcsumc_1_output_array.data = AI_PTR(activations_map[0] + 0);
    svcsumc_1_output_array.data_start = AI_PTR(activations_map[0] + 0);
    Csvcconc_output_array.data = AI_PTR(activations_map[0] + 28);
    Csvcconc_output_array.data_start = AI_PTR(activations_map[0] + 28);
    Cabs_output_array.data = AI_PTR(activations_map[0] + 0);
    Cabs_output_array.data_start = AI_PTR(activations_map[0] + 0);
    Cconc_abs1_output_array.data = AI_PTR(activations_map[0] + 44);
    Cconc_abs1_output_array.data_start = AI_PTR(activations_map[0] + 44);
    Cconc_abs3_output_array.data = AI_PTR(activations_map[0] + 0);
    Cconc_abs3_output_array.data_start = AI_PTR(activations_map[0] + 0);
    Csvcfinal_output_array.data = AI_PTR(activations_map[0] + 44);
    Csvcfinal_output_array.data_start = AI_PTR(activations_map[0] + 44);
    prediction_output_array.data = AI_PTR(activations_map[0] + 0);
    prediction_output_array.data_start = AI_PTR(activations_map[0] + 0);
    iprediction_output_array.data = AI_PTR(activations_map[0] + 8);
    iprediction_output_array.data_start = AI_PTR(activations_map[0] + 8);
    Vsvcv_5_output_array.data = AI_PTR(activations_map[0] + 0);
    Vsvcv_5_output_array.data_start = AI_PTR(activations_map[0] + 0);
    Vnegv_5_output_array.data = AI_PTR(activations_map[0] + 4);
    Vnegv_5_output_array.data_start = AI_PTR(activations_map[0] + 4);
    Vnegv1_5_output_array.data = AI_PTR(activations_map[0] + 32);
    Vnegv1_5_output_array.data_start = AI_PTR(activations_map[0] + 32);
    Vsvcv_4_output_array.data = AI_PTR(activations_map[0] + 4);
    Vsvcv_4_output_array.data_start = AI_PTR(activations_map[0] + 4);
    Vnegv_4_output_array.data = AI_PTR(activations_map[0] + 36);
    Vnegv_4_output_array.data_start = AI_PTR(activations_map[0] + 36);
    Vnegv1_4_output_array.data = AI_PTR(activations_map[0] + 40);
    Vnegv1_4_output_array.data_start = AI_PTR(activations_map[0] + 40);
    Vsvcv_3_output_array.data = AI_PTR(activations_map[0] + 36);
    Vsvcv_3_output_array.data_start = AI_PTR(activations_map[0] + 36);
    Vnegv_3_output_array.data = AI_PTR(activations_map[0] + 60);
    Vnegv_3_output_array.data_start = AI_PTR(activations_map[0] + 60);
    Vnegv1_3_output_array.data = AI_PTR(activations_map[0] + 64);
    Vnegv1_3_output_array.data_start = AI_PTR(activations_map[0] + 64);
    Vsvcv_1_output_array.data = AI_PTR(activations_map[0] + 60);
    Vsvcv_1_output_array.data_start = AI_PTR(activations_map[0] + 60);
    svcvote_2_left_0_output_array.data = AI_PTR(activations_map[0] + 68);
    svcvote_2_left_0_output_array.data_start = AI_PTR(activations_map[0] + 68);
    svcvote_2_output_array.data = AI_PTR(activations_map[0] + 36);
    svcvote_2_output_array.data_start = AI_PTR(activations_map[0] + 36);
    Vnegv_1_output_array.data = AI_PTR(activations_map[0] + 32);
    Vnegv_1_output_array.data_start = AI_PTR(activations_map[0] + 32);
    Vnegv1_1_output_array.data = AI_PTR(activations_map[0] + 60);
    Vnegv1_1_output_array.data_start = AI_PTR(activations_map[0] + 60);
    Vsvcv_2_output_array.data = AI_PTR(activations_map[0] + 32);
    Vsvcv_2_output_array.data_start = AI_PTR(activations_map[0] + 32);
    svcvote_3_left_0_output_array.data = AI_PTR(activations_map[0] + 68);
    svcvote_3_left_0_output_array.data_start = AI_PTR(activations_map[0] + 68);
    svcvote_3_output_array.data = AI_PTR(activations_map[0] + 4);
    svcvote_3_output_array.data_start = AI_PTR(activations_map[0] + 4);
    Vnegv_2_output_array.data = AI_PTR(activations_map[0] + 0);
    Vnegv_2_output_array.data_start = AI_PTR(activations_map[0] + 0);
    Vnegv1_2_output_array.data = AI_PTR(activations_map[0] + 32);
    Vnegv1_2_output_array.data_start = AI_PTR(activations_map[0] + 32);
    Vsvcv_0_output_array.data = AI_PTR(activations_map[0] + 0);
    Vsvcv_0_output_array.data_start = AI_PTR(activations_map[0] + 0);
    svcvote_1_left_0_output_array.data = AI_PTR(activations_map[0] + 8);
    svcvote_1_left_0_output_array.data_start = AI_PTR(activations_map[0] + 8);
    svcvote_1_output_array.data = AI_PTR(activations_map[0] + 12);
    svcvote_1_output_array.data_start = AI_PTR(activations_map[0] + 12);
    Vnegv_0_output_array.data = AI_PTR(activations_map[0] + 8);
    Vnegv_0_output_array.data_start = AI_PTR(activations_map[0] + 8);
    Vnegv1_0_output_array.data = AI_PTR(activations_map[0] + 0);
    Vnegv1_0_output_array.data_start = AI_PTR(activations_map[0] + 0);
    svcvote_0_left_0_output_array.data = AI_PTR(activations_map[0] + 8);
    svcvote_0_left_0_output_array.data_start = AI_PTR(activations_map[0] + 8);
    svcvote_0_output_array.data = AI_PTR(activations_map[0] + 0);
    svcvote_0_output_array.data_start = AI_PTR(activations_map[0] + 0);
    Vsvcconcv_output_array.data = AI_PTR(activations_map[0] + 16);
    Vsvcconcv_output_array.data_start = AI_PTR(activations_map[0] + 16);
    
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

  ai_ptr weights_map[1] = AI_C_ARRAY_INIT;

  if (ai_platform_get_weights_map(weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    variable_scale_array.format |= AI_FMT_FLAG_CONST;
    variable_scale_array.data = AI_PTR(weights_map[0] + 0);
    variable_scale_array.data_start = AI_PTR(weights_map[0] + 0);
    variable_offset_array.format |= AI_FMT_FLAG_CONST;
    variable_offset_array.data = AI_PTR(weights_map[0] + 512);
    variable_offset_array.data_start = AI_PTR(weights_map[0] + 512);
    label_coefficients_array.format |= AI_FMT_FLAG_CONST;
    label_coefficients_array.data = AI_PTR(weights_map[0] + 1024);
    label_coefficients_array.data_start = AI_PTR(weights_map[0] + 1024);
    label_support_vectors_array.format |= AI_FMT_FLAG_CONST;
    label_support_vectors_array.data = AI_PTR(weights_map[0] + 12904);
    label_support_vectors_array.data_start = AI_PTR(weights_map[0] + 12904);
    label_rho_array.format |= AI_FMT_FLAG_CONST;
    label_rho_array.data = AI_PTR(weights_map[0] + 519784);
    label_rho_array.data_start = AI_PTR(weights_map[0] + 519784);
    label_classlabels_ints_array.format |= AI_FMT_FLAG_CONST;
    label_classlabels_ints_array.data = AI_PTR(weights_map[0] + 519808);
    label_classlabels_ints_array.data_start = AI_PTR(weights_map[0] + 519808);
    label_vectors_per_class_array.format |= AI_FMT_FLAG_CONST;
    label_vectors_per_class_array.data = AI_PTR(weights_map[0] + 519812);
    label_vectors_per_class_array.data_start = AI_PTR(weights_map[0] + 519812);
    Csvc_5_indices_array.format |= AI_FMT_FLAG_CONST;
    Csvc_5_indices_array.data = AI_PTR(weights_map[0] + 519820);
    Csvc_5_indices_array.data_start = AI_PTR(weights_map[0] + 519820);
    Csvc_4_indices_array.format |= AI_FMT_FLAG_CONST;
    Csvc_4_indices_array.data = AI_PTR(weights_map[0] + 519824);
    Csvc_4_indices_array.data_start = AI_PTR(weights_map[0] + 519824);
    Csvc_3_indices_array.format |= AI_FMT_FLAG_CONST;
    Csvc_3_indices_array.data = AI_PTR(weights_map[0] + 519828);
    Csvc_3_indices_array.data_start = AI_PTR(weights_map[0] + 519828);
    Csvc_1_indices_array.format |= AI_FMT_FLAG_CONST;
    Csvc_1_indices_array.data = AI_PTR(weights_map[0] + 519832);
    Csvc_1_indices_array.data_start = AI_PTR(weights_map[0] + 519832);
    Csvc_2_indices_array.format |= AI_FMT_FLAG_CONST;
    Csvc_2_indices_array.data = AI_PTR(weights_map[0] + 519836);
    Csvc_2_indices_array.data_start = AI_PTR(weights_map[0] + 519836);
    Csvc_0_indices_array.format |= AI_FMT_FLAG_CONST;
    Csvc_0_indices_array.data = AI_PTR(weights_map[0] + 519840);
    Csvc_0_indices_array.data_start = AI_PTR(weights_map[0] + 519840);
    cst3_array.format |= AI_FMT_FLAG_CONST;
    cst3_array.data = AI_PTR(weights_map[0] + 519844);
    cst3_array.data_start = AI_PTR(weights_map[0] + 519844);
    cst1_array.format |= AI_FMT_FLAG_CONST;
    cst1_array.data = AI_PTR(weights_map[0] + 519848);
    cst1_array.data_start = AI_PTR(weights_map[0] + 519848);
    cst0_array.format |= AI_FMT_FLAG_CONST;
    cst0_array.data = AI_PTR(weights_map[0] + 519852);
    cst0_array.data_start = AI_PTR(weights_map[0] + 519852);
    Vsvcv_5_indices_array.format |= AI_FMT_FLAG_CONST;
    Vsvcv_5_indices_array.data = AI_PTR(weights_map[0] + 519856);
    Vsvcv_5_indices_array.data_start = AI_PTR(weights_map[0] + 519856);
    Vsvcv_4_indices_array.format |= AI_FMT_FLAG_CONST;
    Vsvcv_4_indices_array.data = AI_PTR(weights_map[0] + 519860);
    Vsvcv_4_indices_array.data_start = AI_PTR(weights_map[0] + 519860);
    Vsvcv_3_indices_array.format |= AI_FMT_FLAG_CONST;
    Vsvcv_3_indices_array.data = AI_PTR(weights_map[0] + 519864);
    Vsvcv_3_indices_array.data_start = AI_PTR(weights_map[0] + 519864);
    Vsvcv_1_indices_array.format |= AI_FMT_FLAG_CONST;
    Vsvcv_1_indices_array.data = AI_PTR(weights_map[0] + 519868);
    Vsvcv_1_indices_array.data_start = AI_PTR(weights_map[0] + 519868);
    Vsvcv_2_indices_array.format |= AI_FMT_FLAG_CONST;
    Vsvcv_2_indices_array.data = AI_PTR(weights_map[0] + 519872);
    Vsvcv_2_indices_array.data_start = AI_PTR(weights_map[0] + 519872);
    Vsvcv_0_indices_array.format |= AI_FMT_FLAG_CONST;
    Vsvcv_0_indices_array.data = AI_PTR(weights_map[0] + 519876);
    Vsvcv_0_indices_array.data_start = AI_PTR(weights_map[0] + 519876);
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
      
      .n_macc            = 1267548,
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
      
      .n_macc            = 1267548,
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

