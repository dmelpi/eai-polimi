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
#include "ux_device_class_sensor_streaming.h"
#include "ux_device_stack.h"

/**
 * @brief  _ux_device_class_sensor_streaming_read
 * This function reads from the sensor_streaming class.
 * @param _this [IN] Pointer to sensor_streaming command.
 * @param buffer [IN] Pointer to buffer to save received data
 * @param requested_length [IN] Length of bytes to read.
 * @param actual_length [IN] Pointer to save number of bytes read
 *
 * @return Completion Status
 */
UINT _ux_device_class_sensor_streaming_read(UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, UCHAR *buffer, ULONG requested_length, ULONG *actual_length)
{

  UX_SLAVE_ENDPOINT *endpoint;
  UX_SLAVE_DEVICE *device;
  UX_SLAVE_TRANSFER *transfer_request;
  UINT status;
  ULONG local_requested_length;

  /* If trace is enabled, insert this event into the trace buffer.  */
  UX_TRACE_IN_LINE_INSERT(UX_TRACE_DEVICE_CLASS_SENSOR_STREAMING_READ, sensor_streaming, buffer, requested_length, 0, UX_TRACE_DEVICE_CLASS_EVENTS, 0, 0)

  /* Get the pointer to the device.  */
  device = &_ux_system_slave->ux_system_slave_device;

  /* As long as the device is in the CONFIGURED state.  */
  if(device->ux_slave_device_state != UX_DEVICE_CONFIGURED)
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_CLASS, UX_CONFIGURATION_HANDLE_UNKNOWN);

    /* If trace is enabled, insert this event into the trace buffer.  */
    UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_CONFIGURATION_HANDLE_UNKNOWN, device, 0, 0, UX_TRACE_ERRORS, 0, 0)

    /* Cannot proceed with command, the interface is down.  */
    return (UX_CONFIGURATION_HANDLE_UNKNOWN);
  }

  /* Locate the OUT endpoint.  */
  endpoint = sensor_streaming->ux_slave_class_sensor_streaming_bulkout_endpoint;

  /* Check endpoint. If NULL, we have not yet received the proper SET_INTERFACE command.  */
  if(endpoint == UX_NULL)
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_CLASS, UX_ENDPOINT_HANDLE_UNKNOWN);

    return (UX_ENDPOINT_HANDLE_UNKNOWN);
  }

  /* All SENSOR_STREAMING reading  are on the endpoint OUT, from the host.  */
  transfer_request = &endpoint->ux_slave_endpoint_transfer_request;

  /* Reset the actual length.  */
  *actual_length = 0;

  /* Set return status to SUCCESS to make certain compilers happy.  */
  status = UX_SUCCESS;

  /* Check if we need more transactions.  */
  while(device->ux_slave_device_state == UX_DEVICE_CONFIGURED && requested_length != 0)
  {
    /* Check if we have enough in the local buffer.  */
    if(requested_length > UX_SLAVE_REQUEST_DATA_MAX_LENGTH)
    {
      /* We have too much to transfer.  */
      local_requested_length = UX_SLAVE_REQUEST_DATA_MAX_LENGTH;
    }
    else
    {
      /* We can proceed with the demanded length.  */
      local_requested_length = requested_length;
    }
    /* Send the request to the device controller.  */
    status = _ux_device_stack_transfer_request(transfer_request, local_requested_length, local_requested_length);

    /* Check the status */
    if(status == UX_SUCCESS)
    {
      /* We need to copy the buffer locally.  */
      _ux_utility_memory_copy(buffer, transfer_request->ux_slave_transfer_request_data_pointer, local_requested_length); /* Use case of memcpy is verified. */

      /* Next buffer address.  */
      buffer += transfer_request->ux_slave_transfer_request_actual_length;

      /* Set the length actually received. */
      *actual_length += transfer_request->ux_slave_transfer_request_actual_length;

      /* Decrement what left has to be done.  */
      requested_length -= transfer_request->ux_slave_transfer_request_actual_length;
    }
    else /* We got an error.  */
    {
      return (status);
    }
  }

  /* Check why we got here, either completion or device was extracted.  */
  if(device->ux_slave_device_state != UX_DEVICE_CONFIGURED)
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_CLASS, UX_TRANSFER_NO_ANSWER);

    /* If trace is enabled, insert this event into the trace buffer.  */
    UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_TRANSFER_NO_ANSWER, transfer_request, 0, 0, UX_TRACE_ERRORS, 0, 0)

    /* Device must have been extracted.  */
    return (UX_TRANSFER_NO_ANSWER);
  }
  else
  {
    /* Simply return the last transaction result.  */
    return (status);
  }
}

