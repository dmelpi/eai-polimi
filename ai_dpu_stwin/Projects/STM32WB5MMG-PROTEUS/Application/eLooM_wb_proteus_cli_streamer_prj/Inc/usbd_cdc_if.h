/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : usbd_cdc_if.h
 * @version        : v3.0_Cube
 * @brief          : Header for usbd_cdc_if.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_IF_H__
#define __USBD_CDC_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc.h"

/* USER CODE BEGIN INCLUDE */
#include "FreeRTOS.h"
#include "queue.h"
#include "services/syserror.h"
/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief For Usb device.
  * @{
  */

/** @defgroup USBD_CDC_IF USBD_CDC_IF
  * @brief Usb VCP device module
  * @{
  */

/** @defgroup USBD_CDC_IF_Exported_Defines USBD_CDC_IF_Exported_Defines
  * @brief Defines.
  * @{
  */
/* USER CODE BEGIN EXPORTED_DEFINES */

/* USER CODE END EXPORTED_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Types USBD_CDC_IF_Exported_Types
  * @brief Types.
  * @{
  */

/* USER CODE BEGIN EXPORTED_TYPES */

/**
 * Create a type name for the application defined callback used for the fast data transfer operation.
 * @param p_ctx [IN] specifies an application specific data.
 * @param p_data_chunk [IN] specifies the data buffer.
 * @param chunk_size [IN] specifies the size of the data buffer.
 * @return 0 if the packet has been processed with success, another value to signal the end of the fast transfer operation.
 */
typedef unsigned short (*USB_CDC_FastTransferOpCallback_t)(void *p_ctx, uint8_t *p_data_chunk, uint32_t chunk_size);

/**
 * Specifies the contest for a fast transfer operation. The fast transfer is used when the normal packet processing,
 * done at application task level, is not fast enough, or when the host send packets too fast and there is not
 * enough processing time left at task level.
 */
typedef struct UsbCdcFastTransferOp
{
  /**
   * Application specific data. The USB CDC driver forward this data in the fast transfer callback.
   */
  void *p_ctx;

  /**
   * Application callback used for the fast transfer operation.
   */
  USB_CDC_FastTransferOpCallback_t fCallback;
} UsbCdcFastTransferOp_t;

/* USER CODE END EXPORTED_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Macros USBD_CDC_IF_Exported_Macros
  * @brief Aliases.
  * @{
  */

/* USER CODE BEGIN EXPORTED_MACRO */

/* USER CODE END EXPORTED_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

/** CDC Interface callback. */
extern USBD_CDC_ItfTypeDef USBD_Interface_fops_FS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_FunctionsPrototype USBD_CDC_IF_Exported_FunctionsPrototype
  * @brief Public functions declaration.
  * @{
  */

uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

/* USER CODE BEGIN EXPORTED_FUNCTIONS */

int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len);

/**
 * Set the queue used by the CDC driver to send the received packets for teh normal processing.
 * This queue is bypassed for a fast transfer operation because the processing is done directly in th ISR.
 * @param out_queue [IN] specifies a queue.
 */
void CDC_SetOutQueue(QueueHandle_t out_queue);

/**
 * Set the context for a fast transfer operation. When the context is not NULL then the fast transfer operation
 * start with the next received packet.
 *
 * @param fast_transfer_op_ctx [IN] specifies the context for a fast transfer operation.
 */
void CDC_SetFastTransferOp(UsbCdcFastTransferOp_t fast_transfer_op_ctx);

/* USER CODE END EXPORTED_FUNCTIONS */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CDC_IF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
