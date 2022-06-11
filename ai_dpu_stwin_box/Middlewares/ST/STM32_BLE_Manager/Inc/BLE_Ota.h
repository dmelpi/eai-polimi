/**
  ******************************************************************************
  * @file    BLE_Ota.h
  * @author  System Research & Applications Team - Agrate/Catania Lab.
  * @version 1.2.0
  * @date    28-Feb-2022
  * @brief   Ota characteristic APIs.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0094, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0094
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/  
#ifndef _BLE_OTA_H_
#define _BLE_OTA_H_

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef BLUE_WB

  /* Exported typedef --------------------------------------------------------- */
  typedef void(*CustomWriteRequestOta_t)(uint8_t *att_data, uint8_t data_length);

  /* Exported Variables ------------------------------------------------------- */
  extern CustomWriteRequestOta_t CustomWriteRequestOta;

  /* Exported functions ------------------------------------------------------- */

  /**
   * @brief  Init OTA characteristic
   * @param  None
   * @retval BleCharTypeDef* BleCharPointer: Data structure pointer for ota characteristic
   */
  extern BleCharTypeDef* BLE_InitOtaService(void);

#else /* BLUE_WB */
  #error "This Feature is only for WB BLE Chip"
#endif /* BLUE_WB */

#ifdef __cplusplus
}
#endif

#endif /* _BLE_GNSS_H_ */

/******************* (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
