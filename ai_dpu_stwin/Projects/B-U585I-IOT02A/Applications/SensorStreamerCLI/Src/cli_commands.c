/**
 ******************************************************************************
 * @file    cli_commands.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$
 *
 * @brief   This file defines the CLI commands.
 *
 * This file defines the CLI commands and the API to register the command
 * with the FreeRTOS+CLI.
 *
 *********************************************************************************
 * @attention
 *
 * <h2><center>COPYRIGHT &copy; 2021 STMicroelectronics</center></h2>
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */

#include "cli_commands.h"
#include "services/systp.h"
#include <stdio.h>
#include <string.h>

#ifndef CLI_CMD_CFG_BOARD_NAME
#define CLI_CMD_CFG_BOARD_NAME "\tGeneric board"
#endif


/* Private functions declaration */
/*********************************/

static BaseType_t CLIParseInfoCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t CLIParseUidCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t CLIParseResetCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);


/* Define the FreeRTOS CLI commands */
/************************************/

static const CLI_Command_Definition_t sCLIGenericCommands[] = {
    {
        "info", /* command string to type */
        "\r\ninfo:\r\n Show firmware details and version.\r\n", /* command online help string */
        CLIParseInfoCmd, /* function to run */
        0 /* no parameters are expected. */
    },
    {
        "uid", /* command string to type */
        "\r\nuid:\r\n Show STM32 UID.\r\n", /* command online help string */
        CLIParseUidCmd, /* function to run */
        0 /* no parameters are expected. */
    },
    {
        "reset", /* command string to type */
        "\r\nreset:\r\n MCU System reset.\r\n", /* command online help string */
        CLIParseResetCmd, /* function to run */
        0 /* no parameters are expected. */
    },
    {
        "null",
        "",
        CLIParseNullCmd,
        0
    }
};


/* Public functions definition */
/*******************************/

void RegisterGenericCLICommands(void)
{
  RegisterCLICommands(sCLIGenericCommands);
}

void  RegisterCLICommands(const CLI_Command_Definition_t *p_cli_cmd_list)
{
  const CLI_Command_Definition_t *p_cmd = &p_cli_cmd_list[0];
  while (p_cmd->pxCommandInterpreter != CLIParseNullCmd)
  {
    FreeRTOS_CLIRegisterCommand(p_cmd);
    p_cmd++;
  }
}

BaseType_t CLIParseNullCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  sprintf(p_write_buffer, "not implemented\r\n");
  return pdFALSE;
}


/* Private functions definition */
/********************************/

static BaseType_t CLIParseInfoCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  UNUSED(write_buffer_len);
  static BaseType_t res = 0;

  if(res == 0)
  {
    sprintf(p_write_buffer, "\r\nSTMicroelectronics %s:\r\n"
            "\tVersion %c.%c.%c\r\n"
            CLI_CMD_CFG_BOARD_NAME
            "\r\n",
            "Sensor Streamer", '1', '0', '0');
    res = 1; /* There is more to output */
  }
  else
  {
    sprintf(p_write_buffer, "\t(HAL %ld.%ld.%ld_%ld)\r\n\tCompiled %s %s"
#if defined (__IAR_SYSTEMS_ICC__)
            " (IAR)\r\n",
#elif defined (__CC_ARM)
            " (KEIL)\r\n",
#elif defined (__GNUC__)
            " (openstm32)\r\n",
#endif
            HAL_GetHalVersion() >> 24,
            (HAL_GetHalVersion() >> 16) & 0xFF,
            (HAL_GetHalVersion() >> 8) & 0xFF,
            HAL_GetHalVersion() & 0xFF,
            __DATE__, __TIME__);
    res = 0; /* done */
  }

  return res;
}

static BaseType_t CLIParseUidCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  /* Write back the STM32 UID */
  uint8_t *uid = (uint8_t *)UID_BASE;
  uint32_t mcu_dev_id = HAL_GetDEVID();
  sprintf(p_write_buffer, "%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X_%.3lX\n",
          uid[3], uid[2], uid[1], uid[0],
          uid[7], uid[6], uid[5], uid[4],
          uid[11], uid[10], uid[9], uid[8],
          mcu_dev_id);

  return 0;
}

static BaseType_t CLIParseResetCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  /* System Reset */
  HAL_NVIC_SystemReset();

  return 0;
}

