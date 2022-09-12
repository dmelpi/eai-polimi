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

  /* Get the class container.  */
  class = command->ux_slave_class_command_class_ptr;

  /* Get the class instance in the container.  */
  sensor_streaming = (UX_SLAVE_CLASS_SENSOR_STREAMING*) class->ux_slave_class_instance;

  status = _ux_utility_event_flags_set(&sensor_streaming->ux_slave_class_sensor_streaming_event_flags_group, UX_DEVICE_CLASS_SENSOR_STREAMING_WRITE_EVENT,
  UX_OR);

  /* It's handled.  */
  return (status);
}

