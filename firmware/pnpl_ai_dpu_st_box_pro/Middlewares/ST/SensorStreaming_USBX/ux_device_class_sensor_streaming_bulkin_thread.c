/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** USBX Component                                                        */
/**                                                                       */
/**   Device Sensor Streaming Class IN Thread Entry point                 */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define UX_SOURCE_CODE

/* Include necessary system files.  */

#include "ux_api.h"
#include "ux_device_class_sensor_streaming.h"

/**
 * @brief  _ux_device_class_sensor_streaming_bulkin_entry
 *
 * Entry function for each EP thread. Each EP performs the data
 * transmission through its own the thread.
 * @param _this [IN] Pointer to EP specific parameter set in initialization phase.
 */
VOID _ux_device_class_sensor_streaming_bulkin_entry(ULONG ss)
{
  UX_SLAVE_DEVICE *device;
  UINT status;
  ULONG actual_length = 0;
  UX_THREAD_EP_BULKIN_PARAM *param = (UX_THREAD_EP_BULKIN_PARAM*) ss;

  /* Get the pointer to the device.  */
  device = &_ux_system_slave->ux_system_slave_device;

  while(1)
  {
    /* As long as the device is in the CONFIGURED state.  */
    if(device->ux_slave_device_state == UX_DEVICE_CONFIGURED)
    {
      status = _ux_utility_semaphore_get(&param->semaphore, TX_WAIT_FOREVER);
      /* Check completion status and make sure the message is what we
       expected. */
      if((status != TX_SUCCESS))
      {
        while(1);
      }
      status = _ux_device_class_sensor_streaming_write(param->endpoint, param->tx_buffer, param->tx_size, &actual_length);
      param->tx_state = 0;
    }
  }
}

