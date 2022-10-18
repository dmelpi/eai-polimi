/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    app_usbx_device.c
 * @author  MCD Application Team
 * @brief   USBX Device applicative file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_usbx_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ux_system.h"
#include "ux_utility.h"
#include "ux_device_stack.h"
#include "ux_dcd_stm32.h"
#include "ux_device_descriptors.h"
#include "services/sysmem.h"
#include "ux_device_class_cdc_acm.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
PCD_HandleTypeDef HPCD_USB_OTG_FS;

/* CDC Class Calling Parameter structure */
UX_SLAVE_CLASS_CDC_ACM_PARAMETER cdc_acm_parameter;

/* Define constants.  */
#define USBX_APP_STACK_SIZE         1024
#define USBX_MEMORY_SIZE            (4 * 1024)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */
/**
 * @brief  Application USBX Device Initialization.
 * @param memory_ptr: memory pointer
 * @retval int
 */
UINT MX_USBX_Device_Init(VOID *memory_ptr)
{
  UINT ret = UX_SUCCESS;

  UCHAR *p_mem;
  /* Device framework FS length*/
  ULONG device_framework_fs_length;
  /* Device String framework length*/
  ULONG string_framework_length;
  /* Device language id framework length*/
  ULONG languge_id_framework_length;
  /* Device Framework Full Speed */
  UCHAR *p_device;
  /* String Framework*/
  UCHAR *p_string;
  /* Language_Id_Framework*/
  UCHAR *p_language_id;

  /* Allocate USBX_MEMORY_SIZE. */
  p_mem = (UCHAR*) SysAlloc(USBX_MEMORY_SIZE);

  /* Initialize USBX Memory */
  if(ux_system_initialize(p_mem, USBX_MEMORY_SIZE, UX_NULL, 0) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Get_Device_Framework_Full_Speed and get the length */
  p_device = USBD_Get_Device_Framework_Speed(USBD_FULL_SPEED, &device_framework_fs_length);

  /* Get_String_Framework and get the length */
  p_string = USBD_Get_String_Framework(&string_framework_length);

  /* Get_Language_Id_Framework and get the length */
  p_language_id = USBD_Get_Language_Id_Framework(&languge_id_framework_length);

  /* The code below is required for installing the device portion of USBX.
   In this application */
  ret = ux_device_stack_initialize(NULL, 0, p_device, device_framework_fs_length, p_string, string_framework_length, p_language_id, languge_id_framework_length,
                                   UX_NULL);

  /* Check the device stack class status */
  if(ret != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /*
   * Don't need to control VCP Parameters in this implementation
   */
  /* Initialize the CDC class parameters for the device. */
  cdc_acm_parameter.ux_slave_class_cdc_acm_instance_activate = UX_NULL;

  /* Deinitialize the CDC class parameters for the device. */
  cdc_acm_parameter.ux_slave_class_cdc_acm_instance_deactivate = UX_NULL;

  /* Manage the CDC class requests */
  cdc_acm_parameter.ux_slave_class_cdc_acm_parameter_change = UX_NULL;

  /* registers a slave class to the slave stack. The class is connected with
   interface 0 */
  ret = ux_device_stack_class_register(_ux_system_slave_class_cdc_acm_name,
  ux_device_class_cdc_acm_entry,
                                       1, 0, (VOID*) &cdc_acm_parameter);

  /* Check the device stack class status */
  if(ret != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  return ret;
}

/* USER CODE BEGIN 1 */

/**
 * @brief USB_OTG_FS Initialization Function
 * @param None
 * @retval None
 */
void MX_USB_OTG_FS_PCD_Init(void)
{
  HPCD_USB_OTG_FS.Instance = USB_OTG_FS;
  HPCD_USB_OTG_FS.Init.dev_endpoints = 6;
  HPCD_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  HPCD_USB_OTG_FS.Init.Sof_enable = DISABLE;
  HPCD_USB_OTG_FS.Init.low_power_enable = DISABLE;
  HPCD_USB_OTG_FS.Init.lpm_enable = DISABLE;
  HPCD_USB_OTG_FS.Init.battery_charging_enable = DISABLE;
  HPCD_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  HPCD_USB_OTG_FS.Init.vbus_sensing_enable = DISABLE;
  if(HAL_PCD_Init(&HPCD_USB_OTG_FS) != HAL_OK)
  {
    while(1);
  }
}

/**
 * @brief MX_USB_Device_Init
 *        Initialization of USB device.
 * Init USB device Library, add supported class and start the library
 * @retval None
 */
void MX_USB_Device_Init(void)
{
  MX_USB_OTG_FS_PCD_Init();

  HAL_PCDEx_SetRxFiFo(&HPCD_USB_OTG_FS, 0x100);
  HAL_PCDEx_SetTxFiFo(&HPCD_USB_OTG_FS, 0, 0x10);
  HAL_PCDEx_SetTxFiFo(&HPCD_USB_OTG_FS, 1, 0x20);
  HAL_PCDEx_SetTxFiFo(&HPCD_USB_OTG_FS, 2, 0x10);

  /* initialize the device controller driver*/
  _ux_dcd_stm32_initialize((ULONG) USB_OTG_FS, (ULONG) &HPCD_USB_OTG_FS);
}

/* USER CODE END 1 */
