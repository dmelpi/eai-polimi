/**
  ******************************************************************************
  * @file    BLE_NeaiAnomalyDetection.h
  * @author  System Research & Applications Team - Agrate/Catania Lab.
  * @version 1.0.0
  * @date    28-Apr-2022
  * @brief   NEAI(Cartesiam) Anomaly Detection info services APIs.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

  /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BLE_NEAI_ANOMALY_DETECTION_H_
#define _BLE_NEAI_ANOMALY_DETECTION_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported typedef ---------------------------------------------------------*/
typedef enum
{
  NEAI_AD_NORMAL			      = 0x00,
  NEAI_AD_ANOMALY		        = 0x01,
  NEAI_AD_UNUSED            = 0xFF
} BLE_AD_output_t;

typedef enum
{
  NEAI_AD_IDLE			        = 0x00,
  NEAI_AD_LEARNING		      = 0x01,
  NEAI_AD_DETECTION	        = 0x02
} BLE_AD_phase_t;

typedef enum
{
  NEAI_AD_OK			          = 0x00,
  NEAI_AD_NOINIT		        = 0x7B,
  NEAI_AD_BOARD_ERROR	      = 0x7C,
  NEAI_AD_BUFFER_ERROR	    = 0x7D,
  NEAI_AD_LESS_CALL  	      = 0x7E,
  NEAI_AD_UNKNOWN		        = 0x7F
} BLE_AD_state_t;

typedef void (*CustomReadNeaiAnomalyDetection_t)(BLE_AD_phase_t *phase, BLE_AD_state_t *state, uint8_t *completion_percentage, BLE_AD_output_t *output, uint8_t *similarity);
typedef void (*CustomNotifyEventNeaiAnomalyDetection_t)(BLE_NotifyEvent_t Event);

/* Exported Variables ------------------------------------------------------- */
extern CustomReadNeaiAnomalyDetection_t CustomReadRequestAD;
extern CustomNotifyEventNeaiAnomalyDetection_t CustomNotifyEventAD;

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Init NeaiAnomalyDetection info service
 * @param  uint8_t available_libraries:    numbers of libraries available in the application
 * @retval BleCharTypeDef* BleCharPointer: Data structure pointer for environmental info service
 */
extern BleCharTypeDef* BLE_InitADService(uint8_t available_libraries);

#ifndef BLE_MANAGER_SDKV2
/**
 * @brief  Setting NeaiAnomalyDetection   Advertise Data
 * @param  uint8_t *manuf_data:           Advertise Data
 * @retval None
 */
extern void BLE_SetNeaiAnomalyDetectionAdvertiseData(uint8_t *manuf_data);
#endif /* BLE_MANAGER_SDKV2 */

/**
 * @brief  Update NeaiAnomalyDetection characteristic value
 * @param  uint8_t phase:        0 when AI lib is stopped, 1 when learning occured, 2 when detection occured
 * @param  uint8_t state:        library result of current phase
 * @param  uint8_t status:       1 when an anomaly occurred, 0 otherwise
 * @retval tBleStatus:           Status
 */
extern tBleStatus BLE_NeaiAnomalyDetectionUpdate(BLE_AD_phase_t phase, BLE_AD_state_t state, uint8_t completion_percentage, uint8_t similarity, BLE_AD_output_t output);


#ifdef __cplusplus
}
#endif

#endif /* _BLE_NEAI_ANOMALY_DETECTION_H_ */
