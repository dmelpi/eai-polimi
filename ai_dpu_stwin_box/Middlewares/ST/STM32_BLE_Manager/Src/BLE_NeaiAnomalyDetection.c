/**
  ******************************************************************************
  * @file    BLE_NeaiAnomalyDetection.c
  * @author  System Research & Applications Team - Agrate/Catania Lab.
  * @version 1.x.0
  * @date    28-Feb-2022
  * @brief   Add Anomaly Detection info services using vendor specific profiles.
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

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "BLE_Manager.h"
#include "BLE_ManagerCommon.h"

/* Private define ------------------------------------------------------------*/
#define COPY_NEAI_ANOMALYDETECTION_CHAR_UUID(uuid_struct) COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x19,0x00,0x02,0x11,0xe1,0xac,0x36,0x00,0x02,0xa5,0xd5,0xc5,0x1b)

/* Exported variables --------------------------------------------------------*/
CustomReadNeaiAnomalyDetection_t CustomReadRequestAD=NULL;
CustomNotifyEventNeaiAnomalyDetection_t  CustomNotifyEventAD=NULL;

/* Private Types ----------------------------------------------------------- */

/* Private variables ---------------------------------------------------------*/

/* Data structure pointer for anomaly detection info service */
static BleCharTypeDef ADBleChar;
/* Size for Environmental BLE characteristic */
static uint8_t  ADCharSize;
/* How many AD libraries you want manage */
static uint8_t activeLibraries;

/* Private functions ---------------------------------------------------------*/
static void AttrMod_Request_AD(void *BleCharPointer,uint16_t attr_handle, uint16_t Offset, uint8_t data_length, uint8_t *att_data);
static void Read_Request_AD(void *BleCharPointer,uint16_t handle);


BleCharTypeDef* BLE_InitADService(uint8_t available_libraries)
{
  /* Data structure pointer for BLE service */
  BleCharTypeDef *BleCharPointer= NULL;
  ADCharSize=9;

  /* Init data structure pointer for anomaly detection info service */
  BleCharPointer = &ADBleChar;
  memset(BleCharPointer,0,sizeof(BleCharTypeDef));
  BleCharPointer->AttrMod_Request_CB= AttrMod_Request_AD;
  BleCharPointer->Read_Request_CB= Read_Request_AD;
  COPY_NEAI_ANOMALYDETECTION_CHAR_UUID((BleCharPointer->uuid));

  BleCharPointer->Char_UUID_Type= UUID_TYPE_128;
  BleCharPointer->Char_Value_Length= ADCharSize;
  BleCharPointer->Char_Properties= ((uint8_t)(CHAR_PROP_NOTIFY))|((uint8_t)(CHAR_PROP_READ));
  BleCharPointer->Security_Permissions= ATTR_PERMISSION_NONE;
  BleCharPointer->GATT_Evt_Mask= GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP;
  BleCharPointer->Enc_Key_Size= 16;
  BleCharPointer->Is_Variable= 0;

  activeLibraries = available_libraries;

  return BleCharPointer;
}


tBleStatus BLE_NeaiAnomalyDetectionUpdate(BLE_AD_phase_t phase, BLE_AD_state_t state, uint8_t completion_percentage, BLE_AD_output_t output, uint8_t similarity)
{

  tBleStatus ret;

  uint8_t buff[/*Execution Time*/ 4 + /*Phase*/ 1 + /* State*/ 1 + /* Phase Progress*/ 1 + /* Status*/ 1 + /* Similarity*/ 1 ];

  buff[0] = 0xFF;
  buff[1] = 0xFF;
  buff[2] = 0xFF;
  buff[3] = 0xFF;
  buff[4] = phase;
  buff[5] = state;
  buff[6] = completion_percentage;
  buff[7] = output;
  buff[8] = similarity;

  ret = ACI_GATT_UPDATE_CHAR_VALUE(&ADBleChar, 0, ADCharSize, buff);

  if (ret != (tBleStatus)BLE_STATUS_SUCCESS){
    if(BLE_StdErr_Service==BLE_SERV_ENABLE){
      BytesToWrite = (uint8_t)sprintf((char *)BufferToWrite, "Error Updating NEAI Anomaly Detection Char\n");
      Stderr_Update(BufferToWrite,BytesToWrite);
    } else {
      BLE_MANAGER_PRINTF("Error: Updating NEAI Anomaly Detection Char\r\n");
    }
  }
  return ret;
}


static void AttrMod_Request_AD(void *VoidCharPointer,uint16_t attr_handle, uint16_t Offset, uint8_t data_length, uint8_t *att_data)
{
  if(CustomNotifyEventEnv!=NULL) {
    if (att_data[0] == 01U) {
      CustomNotifyEventEnv(BLE_NOTIFY_SUB);
    } else if (att_data[0] == 0U){
      CustomNotifyEventEnv(BLE_NOTIFY_UNSUB);
    }
  }
#if (BLE_DEBUG_LEVEL>1)
  else {
    BLE_MANAGER_PRINTF("CustomNotifyEventAD function Not Defined\r\n");
  }

  if(BLE_StdTerm_Service==BLE_SERV_ENABLE) {
    BytesToWrite = (uint8_t)sprintf((char *)BufferToWrite,"--->NEAI_AD=%s\n", (att_data[0] == 01U) ? " ON" : " OFF");
    Term_Update(BufferToWrite,BytesToWrite);
  } else {
    BLE_MANAGER_PRINTF("--->NEAI_AD=%s", (att_data[0] == 01U) ? " ON\r\n" : " OFF\r\n");
  }
#endif
}


static void Read_Request_AD(void *VoidCharPointer,uint16_t handle)
{
  if(CustomReadRequestEnv != NULL) {
    BLE_AD_phase_t  phase;
    BLE_AD_state_t  state;
    uint8_t completion_percentage = 0;
    BLE_AD_output_t output;
    uint8_t similarity = 0;
    CustomReadRequestAD(&phase, &state, &completion_percentage, &output, &similarity);
    BLE_NeaiAnomalyDetectionUpdate(phase, state, completion_percentage, output, similarity);

  } else {
    BLE_MANAGER_PRINTF("\r\n\nRead request NEAI Anomaly Detection function not defined\r\n\n");
  }
}
