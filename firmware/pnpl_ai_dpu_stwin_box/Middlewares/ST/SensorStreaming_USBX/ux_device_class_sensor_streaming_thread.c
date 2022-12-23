
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

/* Remove compiling warning. */
VOID _ux_device_class_sensor_streaming_thread(ULONG sensor_streaming_class);

/**
 * @brief  _ux_device_class_sensor_streaming_thread
 * This function is the thread of the sensor_streaming class.
 * @param _this [IN] Address of sensor_streaming class container .
 * @return Completion Status
 */
VOID _ux_device_class_sensor_streaming_thread(ULONG sensor_streaming_class)
{

  UX_SLAVE_CLASS *class;
  UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming;
  UX_SLAVE_TRANSFER *transfer_request;
  UX_SLAVE_DEVICE *device;
  //  UX_SLAVE_INTERFACE          *interface;
  //  UX_SLAVE_ENDPOINT           *endpoint_in;
  //  UX_SLAVE_ENDPOINT           *endpoint_out;
//      UINT                        status;
  //  ULONG                       length;
  ULONG request;
  ULONG value;
  ULONG request_length;
  //  ULONG                                   transmit_length;
  uint8_t request_type;
  uint16_t request_index;
  ULONG actual_flags;

//  ULONG received_message;

  //  /* Cast properly the pima instance.  */
  //  UX_THREAD_EXTENSION_PTR_GET(class, UX_SLAVE_CLASS, sensor_streaming_class)
  //
  //  /* Get the sensor_streaming instance from this class container.  */
  //  sensor_streaming =  (UX_SLAVE_CLASS_SENSOR_STREAMING *) class -> ux_slave_class_instance;
  //
  //  /* Get the pointer to the device.  */
  //  device =  &_ux_system_slave -> ux_system_slave_device;

  //  ULONG received_message;
  //  UINT status;

  /* This thread runs forever but can be suspended or resumed.  */
  while(1)
  {
    //  ULONG received_message;

    /* Retrieve a message from the queue. */
    //    status = tx_queue_receive(sensor_streaming -> queue_ctl, &received_message, TX_WAIT_FOREVER);
    /* Check completion status and make sure the message is what we
     expected. */
    //    if ((status != TX_SUCCESS) )
    //     while(1);
    /* Cast properly the pima instance.  */
    UX_THREAD_EXTENSION_PTR_GET(class, UX_SLAVE_CLASS, sensor_streaming_class)

    /* Get the sensor_streaming instance from this class container.  */
    sensor_streaming = (UX_SLAVE_CLASS_SENSOR_STREAMING*) class->ux_slave_class_instance;

    _ux_utility_event_flags_get(&sensor_streaming->ux_slave_class_sensor_streaming_event_flags_group, UX_DEVICE_CLASS_SENSOR_STREAMING_WRITE_EVENT,
    UX_OR_CLEAR,
                                &actual_flags, UX_WAIT_FOREVER);

    /* Retrieve a message from the queue. */
//           status = tx_queue_receive(&sensor_streaming ->queue_0, &received_message, TX_WAIT_FOREVER);

//    if(actual_flags != 0)
//    {

    /* Get the pointer to the device.  */
    device = &_ux_system_slave->ux_system_slave_device;

    /* As long as the device is in the CONFIGURED state.  */
    if(device->ux_slave_device_state == UX_DEVICE_CONFIGURED)
    {

      /* Cast properly the dpump instance.  */
      UX_THREAD_EXTENSION_PTR_GET(class, UX_SLAVE_CLASS, sensor_streaming_class);

      /* Get the dpump instance from this class container.  */
      sensor_streaming = (UX_SLAVE_CLASS_SENSOR_STREAMING*) class->ux_slave_class_instance;

      /* Get the pointer to the transfer request associated with the control endpoint.  */
      transfer_request = &device->ux_slave_device_control_endpoint.ux_slave_endpoint_transfer_request;

      /* Extract all necessary fields of the request.  */

      request_type = *(transfer_request->ux_slave_transfer_request_setup + UX_SETUP_REQUEST_TYPE);
      request = *(transfer_request->ux_slave_transfer_request_setup + UX_SETUP_REQUEST);
      value = _ux_utility_short_get(transfer_request->ux_slave_transfer_request_setup + UX_SETUP_VALUE);
      request_length = _ux_utility_short_get(transfer_request->ux_slave_transfer_request_setup + UX_SETUP_LENGTH);
      request_index = _ux_utility_short_get(transfer_request->ux_slave_transfer_request_setup + UX_SETUP_INDEX);

      //      transmit_length = request_length ;

      if(request_length != 0U)
      {
        if((request_type & 0x80U) != 0U) /* GET */
        {
          /* Invoke the application.  */
          sensor_streaming->ux_slave_class_sensor_streaming_parameter->ux_slave_class_sensor_streaming_instance_control(
              sensor_streaming->ux_slave_class_sensor_streaming_parameter->app_obj_ctrl, 0, request, value, request_index,
              (uint8_t*) transfer_request->ux_slave_transfer_request_data_pointer, request_length);

          /* Set the phase of the transfer to data out.  */
          transfer_request->ux_slave_transfer_request_phase = UX_TRANSFER_PHASE_DATA_OUT;

          /* Perform the data transfer.  */
          ULONG transmit_length = request_length; //TODO check difference between transmit and request (here could be the same value)
          _ux_device_stack_transfer_request(transfer_request, transmit_length, request_length);

        }
        else /* SET */
        {

          transfer_request->ux_slave_transfer_request_phase = UX_TRANSFER_PHASE_DATA_IN;

          /* Perform the data transfer.  */
          _ux_device_stack_transfer_request(transfer_request, request_length, request_length);

          /* Invoke the application.  */
          sensor_streaming->ux_slave_class_sensor_streaming_parameter->ux_slave_class_sensor_streaming_instance_control(
              sensor_streaming->ux_slave_class_sensor_streaming_parameter->app_obj_ctrl, 1, request, value, request_index,
              (uint8_t*) transfer_request->ux_slave_transfer_request_data_pointer, request_length);
        }
      }
    }
//    }

    /* We need to suspend ourselves. We will be resumed by the
     device enumeration module.  */
    //    _ux_utility_thread_suspend(&class -> ux_slave_class_thread);
  }
}

