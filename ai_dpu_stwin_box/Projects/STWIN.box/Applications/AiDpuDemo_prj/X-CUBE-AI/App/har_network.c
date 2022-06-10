/**
  ******************************************************************************
  * @file    har_network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Wed Mar 23 19:38:45 2022
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


#include "har_network.h"
#include "har_network_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"




#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_har_network
 
#undef AI_HAR_NETWORK_MODEL_SIGNATURE
#define AI_HAR_NETWORK_MODEL_SIGNATURE     "c4b5d0b82be85da884e846d792a5f055"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Wed Mar 23 19:38:45 2022"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_HAR_NETWORK_N_BATCHES
#define AI_HAR_NETWORK_N_BATCHES         (1)




/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  X_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 72, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  variable_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  label_output0_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 1, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  label_output1_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 4, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  variable_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1728, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  label_coefficients_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1350, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  label_support_vectors_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 10800, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  label_rho_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  label_classlabels_ints_array, AI_ARRAY_FORMAT_U8,
  NULL, NULL, 4, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  label_vectors_per_class_array, AI_ARRAY_FORMAT_U8,
  NULL, NULL, 4, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  label_prob_a_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  label_prob_b_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  label_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 495, AI_STATIC)

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  X_output, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 1, 1), AI_STRIDE_INIT(4, 4, 4, 288, 288),
  1, &X_output_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  variable_output, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &variable_output_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  label_output0, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &label_output0_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  label_output1, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &label_output1_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  variable_weights, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 72, 24, 1, 1), AI_STRIDE_INIT(4, 4, 288, 6912, 6912),
  1, &variable_weights_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  label_coefficients, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1350), AI_STRIDE_INIT(4, 4, 4, 5400, 5400),
  1, &label_coefficients_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  label_support_vectors, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 10800), AI_STRIDE_INIT(4, 4, 4, 43200, 43200),
  1, &label_support_vectors_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  label_rho, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 6), AI_STRIDE_INIT(4, 4, 4, 24, 24),
  1, &label_rho_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  label_classlabels_ints, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 4), AI_STRIDE_INIT(4, 1, 1, 4, 4),
  1, &label_classlabels_ints_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  label_vectors_per_class, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 4), AI_STRIDE_INIT(4, 1, 1, 4, 4),
  1, &label_vectors_per_class_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  label_prob_a, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 6), AI_STRIDE_INIT(4, 4, 4, 24, 24),
  1, &label_prob_a_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  label_prob_b, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 6), AI_STRIDE_INIT(4, 4, 4, 24, 24),
  1, &label_prob_b_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  label_scratch0, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 495, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1980, 1980),
  1, &label_scratch0_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  label_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &variable_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &label_output0, &label_output1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 7, &label_coefficients, &label_support_vectors, &label_rho, &label_classlabels_ints, &label_vectors_per_class, &label_prob_a, &label_prob_b),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &label_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  label_layer, 1,
  SVC_TYPE, 0x0, NULL,
  svc, forward_svc,
  &label_chain,
  NULL, &label_layer, AI_STATIC, 
  .kernel_type = AI_SVC_KERNEL_RBF, 
  .gamma = 0.003969999961555004, 
  .coef0 = 0.0, 
  .degree = 3.0, 
  .proba_support = 1, 
  .has_classlabels_int = 1, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  variable_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &X_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &variable_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &variable_weights),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  variable_layer, 0,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &variable_chain,
  NULL, &label_layer, AI_STATIC, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 55592, 1, 1),
    55592, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 2076, 1, 1),
    2076, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_HAR_NETWORK_IN_NUM, &X_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_HAR_NETWORK_OUT_NUM, &label_output0, &label_output1),
  &variable_layer, 0, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 55592, 1, 1),
      55592, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 2076, 1, 1),
      2076, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_HAR_NETWORK_IN_NUM, &X_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_HAR_NETWORK_OUT_NUM, &label_output0, &label_output1),
  &variable_layer, 0, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/


/******************************************************************************/
AI_DECLARE_STATIC
ai_bool har_network_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  ai_ptr activations_map[1] = AI_C_ARRAY_INIT;

  if (ai_platform_get_activations_map(activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    variable_output_array.data = AI_PTR(activations_map[0] + 0);
    variable_output_array.data_start = AI_PTR(activations_map[0] + 0);
    label_scratch0_array.data = AI_PTR(activations_map[0] + 96);
    label_scratch0_array.data_start = AI_PTR(activations_map[0] + 96);
    
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool har_network_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  ai_ptr weights_map[1] = AI_C_ARRAY_INIT;

  if (ai_platform_get_weights_map(weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    variable_weights_array.format |= AI_FMT_FLAG_CONST;
    variable_weights_array.data = AI_PTR(weights_map[0] + 0);
    variable_weights_array.data_start = AI_PTR(weights_map[0] + 0);
    label_coefficients_array.format |= AI_FMT_FLAG_CONST;
    label_coefficients_array.data = AI_PTR(weights_map[0] + 6912);
    label_coefficients_array.data_start = AI_PTR(weights_map[0] + 6912);
    label_support_vectors_array.format |= AI_FMT_FLAG_CONST;
    label_support_vectors_array.data = AI_PTR(weights_map[0] + 12312);
    label_support_vectors_array.data_start = AI_PTR(weights_map[0] + 12312);
    label_rho_array.format |= AI_FMT_FLAG_CONST;
    label_rho_array.data = AI_PTR(weights_map[0] + 55512);
    label_rho_array.data_start = AI_PTR(weights_map[0] + 55512);
    label_classlabels_ints_array.format |= AI_FMT_FLAG_CONST;
    label_classlabels_ints_array.data = AI_PTR(weights_map[0] + 55536);
    label_classlabels_ints_array.data_start = AI_PTR(weights_map[0] + 55536);
    label_vectors_per_class_array.format |= AI_FMT_FLAG_CONST;
    label_vectors_per_class_array.data = AI_PTR(weights_map[0] + 55540);
    label_vectors_per_class_array.data_start = AI_PTR(weights_map[0] + 55540);
    label_prob_a_array.format |= AI_FMT_FLAG_CONST;
    label_prob_a_array.data = AI_PTR(weights_map[0] + 55544);
    label_prob_a_array.data_start = AI_PTR(weights_map[0] + 55544);
    label_prob_b_array.format |= AI_FMT_FLAG_CONST;
    label_prob_b_array.data = AI_PTR(weights_map[0] + 55568);
    label_prob_b_array.data_start = AI_PTR(weights_map[0] + 55568);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/

AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_har_network_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_HAR_NETWORK_MODEL_NAME,
      .model_signature   = AI_HAR_NETWORK_MODEL_SIGNATURE,
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
      
      .n_macc            = 109728,
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
ai_bool ai_har_network_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_HAR_NETWORK_MODEL_NAME,
      .model_signature   = AI_HAR_NETWORK_MODEL_SIGNATURE,
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
      
      .n_macc            = 109728,
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
ai_error ai_har_network_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}

AI_API_ENTRY
ai_error ai_har_network_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    &AI_NET_OBJ_INSTANCE,
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}

AI_API_ENTRY
ai_error ai_har_network_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
    ai_error err;
    ai_network_params params;

    err = ai_har_network_create(network, AI_HAR_NETWORK_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE)
        return err;
    if (ai_har_network_data_params_get(&params) != true) {
        err = ai_har_network_get_error(*network);
        return err;
    }
#if defined(AI_HAR_NETWORK_DATA_ACTIVATIONS_COUNT)
    if (activations) {
        /* set the addresses of the activations buffers */
        for (int idx=0;idx<params.map_activations.size;idx++)
            AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
    }
#endif
#if defined(AI_HAR_NETWORK_DATA_WEIGHTS_COUNT)
    if (weights) {
        /* set the addresses of the weight buffers */
        for (int idx=0;idx<params.map_weights.size;idx++)
            AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
    }
#endif
    if (ai_har_network_init(*network, &params) != true) {
        err = ai_har_network_get_error(*network);
    }
    return err;
}

AI_API_ENTRY
ai_buffer* ai_har_network_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    ((ai_network *)network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}

AI_API_ENTRY
ai_buffer* ai_har_network_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    ((ai_network *)network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}

AI_API_ENTRY
ai_handle ai_har_network_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}

AI_API_ENTRY
ai_bool ai_har_network_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = ai_platform_network_init(network, params);
  if (!net_ctx) return false;

  ai_bool ok = true;
  ok &= har_network_configure_weights(net_ctx, params);
  ok &= har_network_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_har_network_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}

AI_API_ENTRY
ai_i32 ai_har_network_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_HAR_NETWORK_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

