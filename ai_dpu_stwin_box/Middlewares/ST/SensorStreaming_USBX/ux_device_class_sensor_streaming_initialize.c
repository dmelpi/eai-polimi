/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** USBX Component                                                        */
/**                                                                       */
/**   Device Data Sensor Streaming Class                                  */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define UX_SOURCE_CODE

/* Include necessary system files.  */

#include "ux_device_class_sensor_streaming.h"

UCHAR _ux_system_slave_class_sensor_streaming_name[] = "ux_slave_class_streaming_name";

/**
 * @brief  _ux_device_class_sensor_streaming_initialize
 * This function initializes the USB sensor_streaming device.
 * @param _this [IN] Pointer to sensor_streaming command.
 *
 * @return Completion Status
 */
UINT _ux_device_class_sensor_streaming_initialize(UX_SLAVE_CLASS_COMMAND *command)
{
  UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming;
  UX_SLAVE_CLASS *class;
//  UX_SLAVE_CLASS_SENSOR_STREAMING_PARAMETER          *sensor_streaming_parameter;
  UINT status;
  /* Get the class container.  */
  class = command->ux_slave_class_command_class_ptr;

  /* Create an instance of the device sensor_streaming class.  */
  sensor_streaming = _ux_utility_memory_allocate(UX_NO_ALIGN, UX_REGULAR_MEMORY, sizeof(UX_SLAVE_CLASS_SENSOR_STREAMING));

  /* Check for successful allocation.  */
  if(sensor_streaming == UX_NULL)
    return (UX_MEMORY_INSUFFICIENT);

  sensor_streaming->hwcid = _ux_utility_memory_allocate(UX_NO_ALIGN, UX_REGULAR_MEMORY, sizeof(STREAMING_HandleTypeDef));

  /* Check for successful allocation.  */
  if(sensor_streaming == UX_NULL)
    return (UX_MEMORY_INSUFFICIENT);

  /* Save the address of the DPUMP instance inside the DPUMP container.  */
  class->ux_slave_class_instance = (VOID*) sensor_streaming;

  /* Allocate some memory for the thread stack. */
  class->ux_slave_class_thread_stack = _ux_utility_memory_allocate(UX_NO_ALIGN, UX_REGULAR_MEMORY, SS_CLASS_THREAD_STACK_SIZE);

  /* Check for successful allocation.  */
  if(class->ux_slave_class_thread_stack == UX_NULL)
  {
    return (UX_MEMORY_INSUFFICIENT);
  }

//  CHAR *pointer = _ux_utility_memory_allocate(UX_NO_ALIGN, UX_REGULAR_MEMORY, sizeof(TX_QUEUE));
//
//  /* Create the message queue shared by threads 1 and 2. */
//      tx_queue_create(&sensor_streaming ->queue_0, "queue 0", TX_1_ULONG, pointer, 200*sizeof(ULONG));

  /* Create a event flag group for the cdc_acm class to synchronize with the application writing event .  */
  status = _ux_utility_event_flags_create(&sensor_streaming->ux_slave_class_sensor_streaming_event_flags_group, "ux_device_class_sensor_streaming_event_flag");

//     _ux_utility_semaphore_create(&sensor_streaming ->semaphore_ctl, "semaphore_ctl", 1);

  status = _ux_utility_thread_create(&class->ux_slave_class_thread, "ux_slave_class_thread", _ux_device_class_sensor_streaming_thread,
                                     (ULONG) (ALIGN_TYPE) class, (VOID*) class->ux_slave_class_thread_stack,
                                     SS_CLASS_THREAD_STACK_SIZE,
                                     SS_CLASS_THREAD_PRIO,
                                     SS_CLASS_THREAD_PRIO,
                                     UX_NO_TIME_SLICE, UX_DONT_START);

  /* Check the creation of this thread.  */
  if(status != UX_SUCCESS)
    status = UX_THREAD_ERROR;

  UX_THREAD_EXTENSION_PTR_SET(&(class -> ux_slave_class_thread), class)

  for(int ii = 0; ii < N_IN_ENDPOINTS; ii++)
  {
    _ux_utility_semaphore_create(&sensor_streaming->ux_slave_class_sensor_streaming_bulkin[ii].ep_param.semaphore, "semaphore 0", 0);

    /* Allocate some memory for the bulk in thread stack. */
    sensor_streaming->ux_slave_class_sensor_streaming_bulkin[ii].thread_stack = _ux_utility_memory_allocate(UX_NO_ALIGN, UX_REGULAR_MEMORY,
                                                                                                            SS_CLASS_THREAD_STACK_SIZE);

    status = _ux_utility_thread_create(&sensor_streaming->ux_slave_class_sensor_streaming_bulkin[ii].thread, "ux_slave_bulkin_thread",
                                       _ux_device_class_sensor_streaming_bulkin_entry,
                                       (ULONG) (ALIGN_TYPE) &sensor_streaming->ux_slave_class_sensor_streaming_bulkin[ii].ep_param,
                                       (VOID*) sensor_streaming->ux_slave_class_sensor_streaming_bulkin[ii].thread_stack,
                                       SS_BULKIN_THREAD_STACK_SIZE,
                                       SS_BULKIN_THREAD_PRIO,
                                       SS_BULKIN_THREAD_PRIO,
                                       UX_NO_TIME_SLICE, UX_DONT_START);
  }

  /* There is error, free resources and return error.  */
  if(status != UX_SUCCESS)
  {

    /* The last resource, thread is not created or created error,
     no need to free.  */

    if(class->ux_slave_class_thread_stack)
      _ux_utility_memory_free(class->ux_slave_class_thread_stack);

    /* Detach instance and free memory.  */
    class->ux_slave_class_instance = UX_NULL;
    _ux_utility_memory_free(sensor_streaming);

    /* Return completion status.  */
    return (status);
  }

  /* Success, complete remaining settings.  */
  _ux_utility_thread_resume(&class->ux_slave_class_thread);

  /* Get the pointer to the application parameters for the cdc class.  */
  sensor_streaming->ux_slave_class_sensor_streaming_parameter = (UX_SLAVE_CLASS_SENSOR_STREAMING_PARAMETER*) command->ux_slave_class_command_parameter;
  /* Return completion status.  */
  return (UX_SUCCESS);
}

