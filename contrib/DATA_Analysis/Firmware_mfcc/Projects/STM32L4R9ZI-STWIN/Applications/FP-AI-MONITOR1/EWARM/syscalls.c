/**
 ******************************************************************************
 * @file    syscall.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$
 *
 * @brief   Minimal implementation of some system call
 *
 * This file implements the C Runtime support to redirect the printf
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

/* Includes */
//#include <sys/stat.h>
//#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
//#include <signal.h>
//#include <time.h>
//#include <sys/time.h>
//#include <sys/times.h>

/* integration with eLooM framework: */
/* 1. we map the low level put_char to the framework function used for the log. */
#if defined(DEBUG) || defined(SYS_DEBUG)
extern int SysDebugLowLevelPutchar(int x);
#define __io_putchar SysDebugLowLevelPutchar
#endif

/* 2. FP-AI-MONITOR1 use two channel, stdout and stderr, to implement the CLI and keep the eLooM log */
__weak int __io_write_in_console(const unsigned char *ptr, int len);

/* Variables */
__weak int __io_putchar(int ch);
__weak int __io_getchar(void);

size_t __write(int Handle, const unsigned char * Buf, size_t Bufsize);
size_t __read(int Handle, unsigned char *Buf, size_t Bufsize);

/** @brief IAR specific low level standard input
 * @param Handle IAR internal handle
 * @param Buf Buffer where to store characters read from stdin
 * @param Bufsize Number of characters to read
 * @retval Number of characters read
 */
size_t __read(int Handle, unsigned char *Buf, size_t Bufsize)
{
  if (Handle != 0){
    return -1;
  }
  return Bufsize;
}

/** @brief IAR specific low level standard output
 * @param Handle IAR internal handle
 * @param Buf Buffer containing characters to be written to stdout
 * @param Bufsize Number of characters to write
 * @retval Number of characters write
 */
size_t __write(int Handle, const unsigned char * Buf, size_t Bufsize)
{
#if defined(DEGUG) || defined(SYS_DEBUG)
  int DataIdx;
#endif

  /* Check for the command to flush all handles */
  if (Handle == -1)
  {
    return 0;
  }

  /* Only write to stdout (1) and stderr (2), otherwise return failure */
  if (Handle != 1 && Handle != 2){
    return -1;
  }
#if defined(DEGUG) || defined(SYS_DEBUG)
  if (Handle == 2)
  {
    /* remap the stderr on the console */
    return __io_write_in_console(Buf, Bufsize);
  }
  else
  {
    for (DataIdx = 0; DataIdx < Bufsize; DataIdx++)
    {
      __io_putchar( *Buf++ );
    }
  }
#else
  /* in RELEASE the log is disabled so we use stdout for the console. */
  if (Handle == 1)
  {
    /* remap the stdout on the console */
    return __io_write_in_console(Buf, Bufsize);
  }
#endif

  return Bufsize;

//  /* Check for the command to flush all handles */
//  if (Handle == -1)
//  {
//    return 0;
//  }
//
//  /* Only write to stdout and stderr, otherwise return failure */
//  if (Handle != 1 && Handle != 2){
//    return -1;
//  }
//
//  if (Bufsize == 0)
//  {
//    return 0;
//  }
//
//  // try to transmit data through the USB
//  uint16_t nTimeout = 3000; // try to send the data nTimout time.
//  for (; nTimeout; --nTimeout)
//  {
//    if (CDC_Transmit_FS((uint8_t *)Buf, Bufsize) == USBD_OK)
//    {
//      break;
//    }
//  }
//  if (nTimeout)
//  {
//    nTimeout = 3000;
//    /* Transmit zero-length packet to complete transfer */
//    for (; nTimeout; --nTimeout)
//    {
//      if (CDC_Transmit_FS((uint8_t *)Buf, 0) == USBD_OK)
//      {
//        break;
//      }
//    }
//  }
//
//  return Bufsize;
}
