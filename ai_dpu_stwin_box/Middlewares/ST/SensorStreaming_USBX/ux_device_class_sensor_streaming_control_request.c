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
 * @brief  _ux_device_class_sensor_streaming_control_request
 *
 *	This function manages the based sent by the host on the control
 *   endpoints with a CLASS or VENDOR SPECIFIC type.
 *  @param _this [IN] Pointer to sensor_streaming class.
 */
UINT _ux_device_class_sensor_streaming_control_request(UX_SLAVE_CLASS_COMMAND *command)
{
  UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming;
  UX_SLAVE_CLASS *class;
  UINT status = UX_SUCCESS;

//  UX_SLAVE_TRANSFER                       *transfer_request;
//  UX_SLAVE_DEVICE                         *device;
//  ULONG                                   request;
//  ULONG                                   value;
//  ULONG                                   request_length;
//  ULONG                                   transmit_length;
//  uint8_t                                  request_type;
//  uint16_t                                  request_index;

  /* Get the class container.  */
  class = command->ux_slave_class_command_class_ptr;

  /* Get the class instance in the container.  */
  sensor_streaming = (UX_SLAVE_CLASS_SENSOR_STREAMING*) class->ux_slave_class_instance;

  status = _ux_utility_event_flags_set(&sensor_streaming->ux_slave_class_sensor_streaming_event_flags_group, UX_DEVICE_CLASS_SENSOR_STREAMING_WRITE_EVENT,
                                       UX_OR);

//    /* Send message to queue 0. */
//           status = tx_queue_send(&sensor_streaming ->queue_0, &thread_1_messages_sent, TX_NO_WAIT);
//
//           /* Check completion status. */
//           if (status != TX_SUCCESS)
//               while(1);

//  device =  &_ux_system_slave -> ux_system_slave_device;
//  if (device -> ux_slave_device_state == UX_DEVICE_CONFIGURED)
//  {
//
//
//
//    /* Send message to queue 0. */
//    //  ULONG thread_1_messages_sent = 1;
//    //
//    //  status = tx_queue_send(sensor_streaming -> queue_ctl,...... &thread_1_messages_sent, TX_WAIT_FOREVER);
//    //  /* Check completion status. */
//    //   if (status != TX_SUCCESS)
//    //     while(1);
//    //    /* Get the pointer to the device.  */
//    //    device =  &_ux_system_slave -> ux_system_slave_device;
//
//    /* Get the pointer to the transfer request associated with the control endpoint.  */
//    transfer_request =  &device -> ux_slave_device_control_endpoint.ux_slave_endpoint_transfer_request;
//
//    /* Extract all necessary fields of the request.  */
//
//    request_type =  *(transfer_request -> ux_slave_transfer_request_setup + UX_SETUP_REQUEST_TYPE);
//    request =  *(transfer_request -> ux_slave_transfer_request_setup + UX_SETUP_REQUEST);
//    value =  _ux_utility_short_get(transfer_request -> ux_slave_transfer_request_setup + UX_SETUP_VALUE);
//    request_length =   _ux_utility_short_get(transfer_request -> ux_slave_transfer_request_setup + UX_SETUP_LENGTH);
//    request_index =   _ux_utility_short_get(transfer_request -> ux_slave_transfer_request_setup + UX_SETUP_INDEX);
//
//
//    transmit_length = request_length ;
//
//    if (request_length != 0U)
//    {
//      if ((request_type & 0x80U) != 0U) /* GET */
//      {
//        /* Invoke the application.  */
//        sensor_streaming -> ux_slave_class_sensor_streaming_parameter->ux_slave_class_sensor_streaming_instance_control
//        (0,
//         request,
//         value,
//         request_index,
//         (uint8_t *)transfer_request->ux_slave_transfer_request_data_pointer,
//         request_length);
//
//        /* Set the phase of the transfer to data out.  */
//        transfer_request -> ux_slave_transfer_request_phase =  UX_TRANSFER_PHASE_DATA_OUT;
//
//        /* Perform the data transfer.  */
//        ULONG transmit_length = request_length; //TODO check difference between transmit and request (here could be the same value)
//        _ux_device_stack_transfer_request(transfer_request, transmit_length, request_length);
//
//      }
//      else /* SET */
//      {
//
//        transfer_request -> ux_slave_transfer_request_phase =  UX_TRANSFER_PHASE_DATA_IN;
//
//        /* Perform the data transfer.  */
//        _ux_device_stack_transfer_request(transfer_request, request_length, request_length);
//
//        /* Invoke the application.  */
//        sensor_streaming -> ux_slave_class_sensor_streaming_parameter->ux_slave_class_sensor_streaming_instance_control
//        (1,
//         request,
//         value,
//         request_index,
//         (uint8_t *)transfer_request->ux_slave_transfer_request_data_pointer,
//         request_length);
//      }
//    }
//  }

  /* It's handled.  */
  return (status);
}

