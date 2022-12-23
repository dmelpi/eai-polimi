/**
 ******************************************************************************
 * @file    app_messages_parser.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$
 *
 * @brief   Utility function for the application messages.
 *
 * DEfien the API for the application messages.
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

#include "app_messages_parser.h"

uint16_t AppMsgGetSize(uint8_t message_id)
{
  uint16_t msg_size = 0;
  switch (message_id)
  {
    case APP_MESSAGE_ID_UTIL:
      msg_size = sizeof (struct utilMessage_t);
      break;

    case APP_MESSAGE_ID_PRC:
      msg_size = sizeof(struct prcMessage_t);
      break;

    case APP_REPORT_ID_FORCE_STEP:
      msg_size = sizeof(struct internalReportFE_t);
      break;

    default:
      msg_size = 0;
  }

  return msg_size;
}
