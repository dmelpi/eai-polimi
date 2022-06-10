/**
  ******************************************************************************
  * @file           : ai_logging.c
  * @brief          : AILogging C implementation
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "ai_logging.h"



void ai_logging_clear_packet(ai_logging_packet_t *packet)
{
  packet->payload_type = AI_NONE;
  packet->payload = NULL;
  packet->payload_fmt = 0;
  packet->payload_size = 0;
  packet->shape.n_shape = 0;
  packet->timestamp = -1;
}


void ai_logging_create_shape_0d(ai_logging_shape *shape)
{
  shape->n_shape = 0;
  shape->shapes[AI_LOGGING_SHAPES_WIDTH] = 1;
  shape->shapes[AI_LOGGING_SHAPES_HEIGHT] = 1;
}
void ai_logging_create_shape_1d(ai_logging_shape *shape, uint16_t dim_x)
{
  shape->n_shape = 1;
  shape->shapes[AI_LOGGING_SHAPES_WIDTH] = dim_x;
  shape->shapes[AI_LOGGING_SHAPES_HEIGHT] = 1;

}
void ai_logging_create_shape_2d(ai_logging_shape *shape, uint16_t dim_x, uint16_t dim_y)
{
  shape->n_shape = 2;
  shape->shapes[AI_LOGGING_SHAPES_WIDTH] = dim_x;
  shape->shapes[AI_LOGGING_SHAPES_HEIGHT] = dim_y;
}
void ai_logging_create_shape_3d(ai_logging_shape *shape, uint16_t dim_x, uint16_t dim_y, uint16_t dim_z)
{
  shape->n_shape = 3;
  shape->shapes[AI_LOGGING_SHAPES_WIDTH] = dim_x;
  shape->shapes[AI_LOGGING_SHAPES_HEIGHT] = dim_y;
  shape->shapes[AI_LOGGING_SHAPES_DEPTH] = dim_z;
}



#ifdef __cplusplus
}
#endif
