/**
 ******************************************************************************
 * @file    cli_commands.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$*
 * @brief   This file declare the CLI commands API.
 *
 * Used for FreeRTOS+CLI implementation.
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
 ******************************************************************************
 */
#ifndef SRC_CLI_COMMANDS_H_
#define SRC_CLI_COMMANDS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"


  /**
   * Register the generic CLI command with FreeRTOS+CLI framework:
   * - help
   * - info
   * - uid
   * - reset
   */
  void RegisterGenericCLICommands(void);

  /**
   * Register a set of CLI command with FreeRTOS+CLI framework.
   *
   * @param p_cli_cmd_list [IN] specifies a NULL terminated array of CLI commands.
   * The command interpreter of the last command must be CLIParseNullCmd().
   */
  void RegisterCLICommands(const CLI_Command_Definition_t *p_cli_cmd_list);

  /**
   * This CLI command interpreter is a marker to identify the last command in CLI command list.
   */
  BaseType_t CLIParseNullCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);

#ifdef __cplusplus
}
#endif

#endif /* SRC_CLI_COMMANDS_H_ */
