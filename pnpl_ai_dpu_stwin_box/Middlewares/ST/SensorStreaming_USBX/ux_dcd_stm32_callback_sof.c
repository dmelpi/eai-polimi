
/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** USBX Component                                                        */
/**                                                                       */
/**   STM32 Controller Driver                                             */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define UX_SOURCE_CODE
#define UX_DCD_STM32_SOURCE_CODE

/* Include necessary system files.  */

#include "ux_api.h"
#include "ux_dcd_stm32.h"
#include "ux_device_stack.h"
#include "ux_utility.h"

#include "ux_device_class_sensor_streaming.h"

static UINT _SetTxBuffer(UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, uint8_t *pbuff, uint16_t length, uint8_t epVectorIndex);
static UINT _TransmitPacket(UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, uint8_t epVectorIndex);

void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
  UX_SLAVE_DEVICE *device;
  UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming;

  /* Get the pointer to the device.  */
  device = &_ux_system_slave->ux_system_slave_device;

  /* As long as the device is in the CONFIGURED state.  */
  if(device->ux_slave_device_state == UX_DEVICE_CONFIGURED)
  {
    /* Get the sensor_streaming instance from this class container.  */
    sensor_streaming = (UX_SLAVE_CLASS_SENSOR_STREAMING*) device->ux_slave_device_first_interface->ux_slave_interface_class_instance;

    if(sensor_streaming != NULL)
    {
      uint8_t        i, ep;
      __IO uint8_t  *tx_buff_status = sensor_streaming->hwcid->tx_buff_status;
      uint8_t      **tx_buffer = sensor_streaming->hwcid->tx_buffer;
      uint16_t      *ch_data_size = sensor_streaming->hwcid->ch_data_size;
      uint8_t       *status = &sensor_streaming->hwcid->streaming_status;

      UX_THREAD_EP_BULKIN_PARAM *ep_bulk = NULL;

      if(*status == STREAMING_STATUS_STARTED)
      {
        for(i = 0; i < N_CHANNELS_MAX; i++)
        {
          ep = sensor_streaming->hwcid->ep_map[i]; //TODO check if ep is -1
          ep_bulk = &sensor_streaming->ux_slave_class_sensor_streaming_bulkin[ep].ep_param;
          if(ep_bulk->tx_state != 1U)
          {
            if(tx_buff_status[i] == 1U)
            {
              ep_bulk->last_packet_sent = 0;
              tx_buff_status[i] = 0;
              /*send*/
              (void) _SetTxBuffer(sensor_streaming, (uint8_t*) &(tx_buffer[i][0]), (ch_data_size[i] / 2U), ep);
              if(_TransmitPacket(sensor_streaming, ep) != UX_SUCCESS) //(i + 1U)|0x80U
              {
                //              return UX_ERROR;     /* USBD_FAIL */
              }
            }
            else if(tx_buff_status[i] == 2U)
            {
              ep_bulk->last_packet_sent = 0;
              tx_buff_status[i] = 0;
              /*send*/
              (void) _SetTxBuffer(sensor_streaming, (uint8_t*) &(tx_buffer[i][(ch_data_size[i] / 2U)]), (ch_data_size[i] / 2U), ep);
              if(_TransmitPacket(sensor_streaming, ep) != UX_SUCCESS)
              {
                //              return UX_ERROR;     /* USBD_FAIL */
              }
            }
            else
            {
            }
          }
        }

//        for(i = 0; i < (N_IN_ENDPOINTS - 1U); i++)
//        {
//          ep_bulk = &sensor_streaming->ux_slave_class_sensor_streaming_bulkin[i].ep_param;
//          if(ep_bulk->tx_state != 1U)
//          {
//            if(TxBuffStatus[i] == 1U)
//            {
//              ep_bulk->last_packet_sent = 0;
//              TxBuffStatus[i] = 0;
//              /*send*/
//              (void) _SetTxBuffer(sensor_streaming, (uint8_t*) &(TxBuffer[i][0]), (USB_DataSizePerEp[i] / 2U), i);
//              if(_TransmitPacket(sensor_streaming, i) != UX_SUCCESS) //(i + 1U)|0x80U
//              {
//                //              return UX_ERROR;     /* USBD_FAIL */
//              }
//            }
//            else if(TxBuffStatus[i] == 2U)
//            {
//              ep_bulk->last_packet_sent = 0;
//              TxBuffStatus[i] = 0;
//              /*send*/
//              (void) _SetTxBuffer(sensor_streaming, (uint8_t*) &(TxBuffer[i][(USB_DataSizePerEp[i] / 2U)]), (USB_DataSizePerEp[i] / 2U), i);
//              if(_TransmitPacket(sensor_streaming, i) != UX_SUCCESS)
//              {
//                //              return UX_ERROR;     /* USBD_FAIL */
//              }
//            }
//            else
//            {
//            }
//          }
//        }
//
//        ep_bulk = &sensor_streaming->ux_slave_class_sensor_streaming_bulkin[N_IN_ENDPOINTS - 1U].ep_param;
//        for(i = (N_IN_ENDPOINTS - 1U); i < N_CHANNELS_MAX; i++)
//        {
//          if(ep_bulk->tx_state != 1U)
//          {
//            if(TxBuffStatus[i] == 1U)
//            {
//              /*send*/
//              ep_bulk->last_packet_sent = 0;
//              (void) _SetTxBuffer(sensor_streaming, (uint8_t*) &(TxBuffer[i][0]), (USB_DataSizePerEp[i] / 2U), (N_IN_ENDPOINTS - 1));
//              if(_TransmitPacket(sensor_streaming, (N_IN_ENDPOINTS - 1)) == UX_SUCCESS)
//              {
//                TxBuffStatus[i] = 0;
//              }
//            }
//            else if(TxBuffStatus[i] == 2U)
//            {
//              /*send*/
//              ep_bulk->last_packet_sent = 0;
//              (void) _SetTxBuffer(sensor_streaming, (uint8_t*) &(TxBuffer[i][(USB_DataSizePerEp[i] / 2U)]), (USB_DataSizePerEp[i] / 2U), (N_IN_ENDPOINTS - 1));
//              if(_TransmitPacket(sensor_streaming, (N_IN_ENDPOINTS - 1)) == UX_SUCCESS)
//              {
//                TxBuffStatus[i] = 0;
//              }
//            }
//            else
//            {
//            }
//          }
//        }
      }
      else if(*status == STREAMING_STATUS_STOPPING)
      {
        uint32_t sum = 0;

        /* stop all endpoints, no need to check which one where active */
        for(i = 0; i < N_IN_ENDPOINTS; i++)
        {
          ep_bulk = &sensor_streaming->ux_slave_class_sensor_streaming_bulkin[i].ep_param;
          if(ep_bulk->last_packet_sent != 1U)
          {
            ep_bulk->last_packet_sent = 1;
            (void) _SetTxBuffer(sensor_streaming, (uint8_t*) &(tx_buffer[i][(ch_data_size[i] / 2U)]), (ch_data_size[i] / 2U), i);
            if(_TransmitPacket(sensor_streaming, i) == UX_SUCCESS)
            {
              tx_buff_status[i] = 0;
            }
          }
          sum += ep_bulk->last_packet_sent;
          if(sum == N_IN_ENDPOINTS)
          {
            *status = STREAMING_STATUS_IDLE;
          }
          _ux_utility_thread_suspend(&sensor_streaming->ux_slave_class_sensor_streaming_bulkin[i].thread);
        }
      }
    }
  }
}

/**
 * @brief  _SetTxBuffer
 * enqueue a packet on the specific endpoint.
 * @param _this [IN] Pointer to sensor_streaming command.
 * @param pbuff [IN] pointer to buffer
 * @param length [IN] size of buffer
 * @param epVectorIndex [IN] endpoint index number
 * @return Completion Status
 */
static UINT _SetTxBuffer(UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, uint8_t *pbuff, uint16_t length, uint8_t epVectorIndex)
{
  UX_THREAD_EP_BULKIN_PARAM *ep_bulk = NULL;
  ep_bulk = &sensor_streaming->ux_slave_class_sensor_streaming_bulkin[epVectorIndex].ep_param;
  ep_bulk->tx_buffer = pbuff;
  ep_bulk->tx_size = length;

  return UX_SUCCESS; /* USBD_OK */
}

/**
 * @brief  _TransmitPacket
 * enqueue a packet on the specific endpoint.
 * @param _this [IN] Pointer to sensor_streaming command.
 * @param epVectorIndex [IN] endpoint index number
 *
 * @return Completion Status
 */
static UINT _TransmitPacket(UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, uint8_t epVectorIndex)
{
  UINT status;
  UX_THREAD_EP_BULKIN_PARAM *ep_bulk = NULL;
  ep_bulk = &sensor_streaming->ux_slave_class_sensor_streaming_bulkin[epVectorIndex].ep_param;

  if(ep_bulk->tx_state == 0U)
  {
    /* Tx Transfer in progress */
    ep_bulk->tx_state = 1;
//    status = tx_semaphore_put(&ep_bulk->semaphore_0);
    status = _ux_utility_semaphore_put(&ep_bulk->semaphore);
    /* Check completion status. */
    if(status != TX_SUCCESS)
    {
      while(1);
    }
    return UX_SUCCESS;
  }
  else
  {
    return UX_TRANSFER_NOT_READY;
  }
}

