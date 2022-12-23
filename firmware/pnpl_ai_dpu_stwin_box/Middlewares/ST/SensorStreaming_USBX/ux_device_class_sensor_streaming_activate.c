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

/**
 * This function activates the USB sensor_streaming device.
 * @param _this [IN] Pointer to sensor_streaming command.
 *
 * @return Completion Status
 */
UINT _ux_device_class_sensor_streaming_activate(UX_SLAVE_CLASS_COMMAND *command)
{

  UX_SLAVE_INTERFACE *interface;
  UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming;
  UX_SLAVE_CLASS *class;
  UX_SLAVE_ENDPOINT *endpoint;

  /* Get the class container.  */
  class = command->ux_slave_class_command_class_ptr;

  /* Store the class instance in the container.  */
  sensor_streaming = (UX_SLAVE_CLASS_SENSOR_STREAMING*) class->ux_slave_class_instance;

  /* Get the interface that owns this instance.  */
  interface = (UX_SLAVE_INTERFACE*) command->ux_slave_class_command_interface;

  /* Store the class instance into the interface.  */
  interface->ux_slave_interface_class_instance = (VOID*) sensor_streaming;

  /* Now the opposite, store the interface in the class instance.  */
  sensor_streaming->ux_slave_class_sensor_streaming_interface = interface;

  /* Locate the endpoints.  Interrupt for Control and Bulk in/out for Data.  */
  endpoint = interface->ux_slave_interface_first_endpoint;

  uint16_t ep_index = 0;
  /* Parse all endpoints.  */
  while(endpoint != UX_NULL)
  {

    /* Check the endpoint direction, and type.  */
    if((endpoint->ux_slave_endpoint_descriptor.bEndpointAddress & UX_ENDPOINT_DIRECTION) == UX_ENDPOINT_IN)
    {

      /* Look at type.  */
      if((endpoint->ux_slave_endpoint_descriptor.bmAttributes & UX_MASK_ENDPOINT_TYPE) == UX_BULK_ENDPOINT)

        if(ep_index < N_IN_ENDPOINTS)
        {
          /* We have found the bulk in endpoint, save it.  */
          sensor_streaming->ux_slave_class_sensor_streaming_bulkin[ep_index].ep_param.endpoint = endpoint;
          ep_index++;
        }

    }
    else
    {
      /* Look at type for out endpoint.  */
      if((endpoint->ux_slave_endpoint_descriptor.bmAttributes & UX_MASK_ENDPOINT_TYPE) == UX_BULK_ENDPOINT)

        /* We have found the bulk out endpoint, save it.  */
        sensor_streaming->ux_slave_class_sensor_streaming_bulkout_endpoint = endpoint;
    }

    /* Next endpoint.  */
    endpoint = endpoint->ux_slave_endpoint_next_endpoint;
  }

  /* If there is a activate function call it.  */
  if(sensor_streaming->ux_slave_class_sensor_streaming_parameter->ux_slave_class_sensor_streaming_instance_activate != UX_NULL)
  {
    /* Invoke the application.  */
    sensor_streaming->ux_slave_class_sensor_streaming_parameter->ux_slave_class_sensor_streaming_instance_activate(
        sensor_streaming->ux_slave_class_sensor_streaming_parameter->app_obj_ctrl, sensor_streaming);
  }

  for(int i = 0; i < N_IN_ENDPOINTS; i++)
  {
    sensor_streaming->ux_slave_class_sensor_streaming_bulkin[i].ep_param.tx_state = 0;
  }

  for(int i = 0; i < N_CHANNELS_MAX; i++)
  {
    sensor_streaming->hwcid->tx_buff_status[i] = 0;
  }

  /* If trace is enabled, insert this event into the trace buffer.  */
  UX_TRACE_IN_LINE_INSERT(UX_TRACE_DEVICE_CLASS_SENSOR_STREAMING_ACTIVATE, sensor_streaming, 0, 0, 0, UX_TRACE_DEVICE_CLASS_EVENTS, 0, 0)

  /* If trace is enabled, register this object.  */
  UX_TRACE_OBJECT_REGISTER(UX_TRACE_DEVICE_OBJECT_TYPE_INTERFACE, sensor_streaming, 0, 0, 0)

  /* Return completion status.  */
  return (UX_SUCCESS);
}

