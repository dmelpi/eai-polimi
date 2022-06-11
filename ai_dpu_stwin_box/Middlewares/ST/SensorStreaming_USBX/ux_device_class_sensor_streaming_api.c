/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** USBX Component                                                        */
/**                                                                       */
/**   Device Sensor Streaming Class                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define UX_SOURCE_CODE

/* Include necessary system files.  */

#include "ux_api.h"
#include "ux_device_stack.h"
#include "ux_device_class_sensor_streaming.h"

/**
 * @brief  ux_device_class_sensor_streaming_SetTransmissionEP
 *
 * This function maps streaming id and EP.
 * @param _this [IN] Pointer to sensor_streaming class.
 * @param id [IN] streaming id.
 *  @param ep [IN] ep.
 * @return Completion Status
 */
UINT ux_device_class_sensor_streaming_SetTransmissionEP(UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, uint8_t id, uint8_t ep)
{
  //assert_param(sensor_streaming); TODO add check null pointer

  if(id > N_CHANNELS_MAX)
  {
    return UX_ERROR;
  }
  if(ep > N_IN_ENDPOINTS-1)
  {
    return UX_ERROR;
  }

  sensor_streaming->hwcid->ep_map[id] = ep;
  return (UX_SUCCESS);
}

/**
 * @brief  ux_device_class_sensor_streaming_SetRxDataBuffer
 *
 * This function Sets Rx Buffer.
 * @param _this [IN] Pointer to sensor_streaming class.
 * @param  ptr: Rx Buffer
 * @return Completion Status
 */
UINT ux_device_class_sensor_streaming_SetRxDataBuffer(UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, uint8_t *ptr)
{
  sensor_streaming->hwcid->rx_buffer = ptr;
  return (UX_SUCCESS);
}

/**
 * @brief  ux_device_class_sensor_streaming_SetTxDataBuffer
 *         Set buffer data for a specific channel.
 *         A data "frame" of size bytes will be sent on the USB endpoints
 * @param _this [IN] Pointer to sensor_streaming class.
 * @param  ch_number: channel number
 * @param  ptr: pointer to data
 *              NOTE! Memory allocated must be at least equal to (size * 2) + 2;
 * @param  size: length of each packet in bytes
 * @retval status
 */
UINT ux_device_class_sensor_streaming_SetTxDataBuffer(UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, uint8_t id, uint8_t *ptr, uint16_t size)
{
  STREAMING_HandleTypeDef *hwcid = (STREAMING_HandleTypeDef*) sensor_streaming->hwcid;
  uint8_t **tx_buffer = hwcid->tx_buffer;
  uint32_t *tx_buff_idx = hwcid->tx_buff_idx;
  uint16_t *ch_data_size = hwcid->ch_data_size;

  tx_buffer[id] = ptr;
  /* Double buffer contains 2 * user data packet + 1st byte (id) for each half */
  ch_data_size[id] = (size * 2U) + 2U;
  /* write the id at the beginning of the buffer (first byte) */
  ptr[0] = id;
  /* move buffer index to position 1 */
  tx_buff_idx[id] = 1;

  return (UX_SUCCESS);
}

/**
 * @brief  ux_device_class_sensor_streaming_CleanTxDataBuffer
 *         discard remaining data in the buffer of the specified ch
 * @param _this [IN] Pointer to sensor_streaming class.
 * @param  ch_number: channel number
 * @retval status
 */
UINT ux_device_class_sensor_streaming_CleanTxDataBuffer(UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, uint8_t ch_number)
{
  STREAMING_HandleTypeDef *hwcid = (STREAMING_HandleTypeDef*) sensor_streaming->hwcid;
  hwcid->tx_buff_reset[ch_number] = 1;
  return (UX_SUCCESS);
}

/**
 * @brief  ux_device_class_sensor_streaming_FillTxDataBuffer
 *         Enqueue data on a specific channel
 * @param _this [IN] Pointer to sensor_streaming class.
 * @param  ch_number: channel number to be used
 * @param  buf: pointer to data to be sent
 * @param  size: length of data in bytes
 * @retval status
 */

UX_INTERRUPT_SAVE_AREA //used by UX_DISABLE and UX_ENABLE isr
UINT ux_device_class_sensor_streaming_FillTxDataBuffer(UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, uint8_t ch_number, uint8_t *buf, uint32_t size)
{
  STREAMING_HandleTypeDef *hwcid = (STREAMING_HandleTypeDef*) sensor_streaming->hwcid;
  uint8_t **tx_buffer = hwcid->tx_buffer;
  uint32_t *tx_buff_idx = hwcid->tx_buff_idx;
  uint16_t *ch_data_size = hwcid->ch_data_size;
  volatile uint8_t *tx_buff_reset = hwcid->tx_buff_reset;
  uint8_t  *tx_buff_ptr = (uint8_t*) tx_buffer[ch_number];
  volatile uint8_t *tx_buff_status;
  uint32_t i;

  UX_DISABLE
  tx_buff_status = hwcid->tx_buff_status;
  UX_RESTORE

  if(tx_buff_reset[ch_number] == 1U)
  {
//    if(ch_number >= (N_IN_ENDPOINTS - 1U))
//    {
      /* write ch_number at the beginning of the buffer (first byte) */
      tx_buff_ptr[0] = ch_number;
      /* move buffer index to position 1 */
      tx_buff_idx[ch_number] = 1;
//    }
//    else
//    {
//      tx_buff_idx[ch_number] = 0;
//    }
    UX_DISABLE
    tx_buff_status[ch_number] = 0;
    UX_RESTORE

    tx_buff_reset[ch_number] = 0;
  }

  uint32_t ch_tx_buff_idx = tx_buff_idx[ch_number];

  for(i = 0; i < size; i++)
  {
    tx_buff_ptr[ch_tx_buff_idx] = buf[i];
    ch_tx_buff_idx = (ch_tx_buff_idx + 1U) % (ch_data_size[ch_number]);

    if(ch_tx_buff_idx == ((uint32_t) ch_data_size[ch_number] / 2U))
    {
      UX_DISABLE
      tx_buff_status[ch_number] = 1;
      UX_RESTORE

//      if(ch_number >= (N_IN_ENDPOINTS - 1U))
//      {
        /* write ch_number at the beginning of the buffer (first byte) */
        tx_buff_ptr[ch_tx_buff_idx] = ch_number;
        /* move buffer index to position 1 */
        ch_tx_buff_idx = (ch_tx_buff_idx + 1U);
//      }
    }
    else if(ch_tx_buff_idx == 0U)
    {
      UX_DISABLE
      tx_buff_status[ch_number] = 2;
      UX_RESTORE

//      if(ch_number >= (N_IN_ENDPOINTS - 1U))
//      {
        /* write ch_number at the beginning of the buffer (first byte) */
        tx_buff_ptr[ch_tx_buff_idx] = ch_number;
        /* move buffer index to position 1 */
        ch_tx_buff_idx = (ch_tx_buff_idx + 1U);
//      }
    }
    else
    {
    }
  }
  tx_buff_idx[ch_number] = ch_tx_buff_idx;
  return (UX_SUCCESS);
}

/**
 * @brief  ux_device_class_sensor_streaming_StartStreaming
 *         Enable USB streaming.
 * @param _this [IN] Pointer to sensor_streaming class.
 * @retval status
 */
UINT ux_device_class_sensor_streaming_StartStreaming(UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming)
{
  STREAMING_HandleTypeDef *hwcid = (STREAMING_HandleTypeDef*) sensor_streaming->hwcid;

  uint8_t *status = &(hwcid->streaming_status);

  for(uint8_t i = 0; i < (N_IN_ENDPOINTS); i++)

  {
    sensor_streaming->ux_slave_class_sensor_streaming_bulkin[i].ep_param.last_packet_sent = 1;
    _ux_utility_thread_resume(&sensor_streaming->ux_slave_class_sensor_streaming_bulkin[i].thread);

  }
  *status = STREAMING_STATUS_STARTED;

  return (UX_SUCCESS);
}

/**
 * @brief  ux_device_class_sensor_streaming_StopStreaming
 *         Disable USB streaming.
 * @param _this [IN] Pointer to sensor_streaming class.
 * @retval status
 */
UINT ux_device_class_sensor_streaming_StopStreaming(UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming)
{
  STREAMING_HandleTypeDef *hwcid = (STREAMING_HandleTypeDef*) sensor_streaming->hwcid;

  uint8_t *status = &(hwcid->streaming_status);
  *status = STREAMING_STATUS_STOPPING;

  return (UX_SUCCESS);

}

/**
 * @brief  ux_device_class_sensor_streaming_stack_class_register
 *         Store class parameters
 * @param _this [IN] Pointer to sensor_streaming class.
 * @retval status
 */
UINT ux_device_class_sensor_streaming_stack_class_register(void *ss_class_parameter)
{
  return _ux_device_stack_class_register(_ux_system_slave_class_sensor_streaming_name, _ux_device_class_sensor_streaming_entry, 1, 0,
                                         (VOID*) ss_class_parameter);
}
