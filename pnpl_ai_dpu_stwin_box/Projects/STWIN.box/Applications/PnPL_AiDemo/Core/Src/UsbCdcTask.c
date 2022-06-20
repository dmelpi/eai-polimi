/**
 ******************************************************************************
 * @file    UsbCdcTask.c
 * @author  STMicroelectronics
 * @version $Version$
 * @date    May 21, 2022
 *
 * @brief USB_CDC Class
 *
 * This managed task is responsible to manage the USB_CDC communication
 *
 *********************************************************************************
 * @attention
 *
 * <h2><center>COPYRIGHT &copy; 2021 STMicroelectronics</center></h2>
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */

#include "UsbCdcTask.h"
#include "UsbCdcTask_vtbl.h"
#include "drivers/PushButtonDrv.h"
#include "drivers/PushButtonDrv_vtbl.h"
#include "services/sysdebug.h"
#include "ux_api.h"
#include "app_messages_parser.h"
#include "app_usbx_device.h"
#include "ux_device_class_cdc_acm.h"

#ifndef USB_CDC_TASK_CFG_STACK_DEPTH
#define USB_CDC_TASK_CFG_STACK_DEPTH            TX_MINIMUM_STACK
#endif

#ifndef USB_CDC_TASK_CFG_PRIORITY
#define USB_CDC_TASK_CFG_PRIORITY               (TX_MAX_PRIORITIES-2)
#endif

#define USB_CDC_TASK_ANTI_DEBOUNCH_PERIOD_TICK  7U

#define SYS_DEBUGF(level, message)              SYS_DEBUGF3(SYS_DBG_HW, level, message)

#define USB_CDC_TX_MAX_LENGTH                   (UX_SLAVE_REQUEST_DATA_MAX_LENGTH-1)

/**
 * The only instance of the task object.
 */
static UsbCdcTask_t sTaskObj;

extern PCD_HandleTypeDef HPCD_USB_OTG_FS;

// Private member function declaration
// ***********************************

/**
 * Execute one step of the task control loop while the system is in RUN mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t UsbCdcTaskExecuteStepRun(AManagedTask *_this);

/**
 * Class object declaration
 */
typedef struct _UsbCdcClass {
  /**
   * UsbCdcTask class virtual table.
   */
  AManagedTaskEx_vtbl m_xVTBL;

  /**
   * UsbCdcTask (PM_STATE, ExecuteStepFunc) map.
   */
  pExecuteStepFunc_t m_pfPMState2FuncMap[];
} UsbCdcClass;

/**
 * The class object.
 */
static const UsbCdcClass s_xTheClass = {
    /* Class virtual table */
    {
        UsbCdcTask_vtblHardwareInit,
        UsbCdcTask_vtblOnCreateTask,
        UsbCdcTask_vtblDoEnterPowerMode,
        UsbCdcTask_vtblHandleError,
        UsbCdcTask_vtblOnEnterTaskControlLoop,
        UsbCdcTask_vtblForceExecuteStep,
        UsbCdcTask_vtblOnEnterPowerMode
    },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        UsbCdcTaskExecuteStepRun,
        NULL,
        UsbCdcTaskExecuteStepRun,
    }
};


// Inline function forward declaration
// ***********************************

#if defined (__GNUC__)
#endif


// Public API definition
// *********************

AManagedTaskEx *UsbCdcTaskAlloc(void)
{
  // This allocator implements the singleton design pattern.

  // Initialize the super class
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &s_xTheClass.m_xVTBL;

  return (AManagedTaskEx*)&sTaskObj;
}

// AManagedTaskEx virtual functions definition
// ***********************************************

sys_error_code_t UsbCdcTask_vtblHardwareInit(AManagedTask *_this, void *pParams)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  UNUSED(_this);
  UINT status = TX_SUCCESS;

  status = MX_USBX_Device_Init(0);

  if(status != UX_SUCCESS)
  {
    while(1);
  }

  return res;
}

sys_error_code_t UsbCdcTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pTaskCode, CHAR **pName,
    VOID **pStackStart, ULONG *pStackDepth,
    UINT *pPriority, UINT *pPreemptThreshold,
    ULONG *pTimeSlice, ULONG *pAutoStart,
    ULONG *pParams)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  _this->m_pfPMState2FuncMap = s_xTheClass.m_pfPMState2FuncMap;

  /* Initialization of USB device */
  MX_USB_Device_Init();

  /* Start device USB */
  HAL_PCD_Start(&HPCD_USB_OTG_FS);

//  *pvTaskCode = UsbCdcTaskRun;
  *pTaskCode = AMTRun;
  *pName = "USB_CDC";
  *pStackStart = NULL; // allocate the task stack in the system memory pool.
  *pStackDepth = USB_CDC_TASK_CFG_STACK_DEPTH;
  *pParams = (ULONG)_this;
  *pPriority = USB_CDC_TASK_CFG_PRIORITY;
  *pPreemptThreshold = USB_CDC_TASK_CFG_PRIORITY;
  *pTimeSlice = TX_NO_TIME_SLICE;
  *pAutoStart = TX_AUTO_START;

  return res;
}

sys_error_code_t UsbCdcTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode eActivePowerMode, const EPowerMode eNewPowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  return res;
}

sys_error_code_t UsbCdcTask_vtblHandleError(AManagedTask *_this, SysEvent xError)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  return res;
}

sys_error_code_t UsbCdcTask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  UsbCdcTask_t *p_obj = (UsbCdcTask_t*)_this;

  UINT status;

  /* Call ThreadX to create the Mutex object. */
  status = tx_mutex_create(&p_obj->write_mutex, "UsbWrite_Mutex", TX_INHERIT);

  /* Check for status.  */
  if(status != UX_SUCCESS)
  {
    res = SYS_BASE_ERROR_CODE;
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("UsbCdcTask: Error creating mutex.\r\n"));
  }

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("UsbCdcTask: start.\r\n"));

  return res;
}


/* AManagedTaskEx virtual functions definition */
/***********************************************/

sys_error_code_t UsbCdcTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode active_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  UNUSED(_this);
  UNUSED(active_power_mode);

  UX_SLAVE_DEVICE        *device;
  UX_SLAVE_CLASS_CDC_ACM *cdc_acm;

  device = &_ux_system_slave->ux_system_slave_device;

  if(device->ux_slave_device_state == UX_DEVICE_CONFIGURED)
  {
    cdc_acm = device->ux_slave_device_first_interface->ux_slave_interface_next_interface->ux_slave_interface_class_instance;

    /* need to abort RX transfer to unlock the thread and allow the change of state */
    ux_device_class_cdc_acm_ioctl(cdc_acm, UX_SLAVE_CLASS_CDC_ACM_IOCTL_ABORT_PIPE, (void*)UX_SLAVE_CLASS_CDC_ACM_ENDPOINT_RCV);
  }

  return res;
}

sys_error_code_t UsbCdcTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  UNUSED(_this);

  return res;
}

sys_error_code_t UsbCdcTask_SetDelegate(UsbCdcTask_t *_this, IUsbDelegate *p_delegate)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  _this->p_delegate = p_delegate;

  return res;
}

sys_error_code_t UsbCdcTask_Write(UCHAR *buffer, ULONG requested_length, ULONG *actual_length)
{
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  UX_SLAVE_CLASS_CDC_ACM *cdc_acm;
  ULONG buffer_offset = 0;

  /* Get the cdc Instance */
  cdc_acm = _ux_system_slave->ux_system_slave_device.ux_slave_device_first_interface->ux_slave_interface_next_interface->ux_slave_interface_class_instance;

  /* Get the mutex as this is a critical section.  */
  tx_mutex_get(&sTaskObj.write_mutex, TX_WAIT_FOREVER);

#ifndef UX_DEVICE_CLASS_CDC_ACM_TRANSMISSION_DISABLE
  cdc_acm->ux_slave_class_cdc_acm_transmission_status = UX_FALSE;
#endif /* UX_DEVICE_CLASS_CDC_ACM_TRANSMISSION_DISABLE */

  /* Split (if needed) and send the buffer */
  while(requested_length > 0 && res == SYS_NO_ERROR_CODE)
  {
    uint32_t chunk_size;
    uint32_t actual_size;

    chunk_size = (requested_length < USB_CDC_TX_MAX_LENGTH) ? requested_length : USB_CDC_TX_MAX_LENGTH;

    /* Send data over the class cdc_acm_write */
    if(ux_device_class_cdc_acm_write(cdc_acm, buffer+buffer_offset, chunk_size, &actual_size) != UX_SUCCESS)
    {
      res = SYS_BASE_ERROR_CODE;
    }
    buffer_offset += actual_size;
    requested_length -= actual_size;
  }
  *actual_length = buffer_offset;

  /* Release the mutex */
  tx_mutex_put(&sTaskObj.write_mutex);

  return res;
}

// Private function definition
// ***************************

static sys_error_code_t UsbCdcTaskExecuteStepRun(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  UX_SLAVE_DEVICE *device;
  UX_SLAVE_INTERFACE *data_interface;
  UX_SLAVE_CLASS_CDC_ACM *cdc_acm;
  uint32_t actual_length;
  uint32_t ux_status = UX_SUCCESS;
  UsbCdcTask_t *p_obj = (UsbCdcTask_t*)_this;

  /* set stdout to be unbuffered */
  setvbuf(stdout, NULL, _IONBF, 0);

  /* Get device */
  device = &_ux_system_slave->ux_system_slave_device;

  /* Check if device is configured */
  if(device->ux_slave_device_state == UX_DEVICE_CONFIGURED)
  {
    /* Get Data interface */
    data_interface = device->ux_slave_device_first_interface->ux_slave_interface_next_interface;

    /* Compares two memory blocks ux_slave_class_name and _ux_system_slave_class_cdc_acm_name */
    ux_status = ux_utility_memory_compare(data_interface->ux_slave_interface_class->ux_slave_class_name, _ux_system_slave_class_cdc_acm_name,
                                          ux_utility_string_length_get(_ux_system_slave_class_cdc_acm_name));

    /* Check Compares success */
    if(ux_status == UX_SUCCESS)
    {
      cdc_acm = data_interface->ux_slave_interface_class_instance;

#ifndef UX_DEVICE_CLASS_CDC_ACM_TRANSMISSION_DISABLE
      /* Set transmission_status to UX_FALSE for the first time */
      cdc_acm -> ux_slave_class_cdc_acm_transmission_status = UX_FALSE;
#endif

      /* Read the received data in blocking mode */
      ux_device_class_cdc_acm_read(cdc_acm, (UCHAR*)p_obj->UserRxBufferFS, 64, &actual_length);

      if(p_obj->p_delegate != NULL && actual_length != 0)
      {
        IUsbDelegateOnNewData(p_obj->p_delegate, (UCHAR*)p_obj->UserRxBufferFS, actual_length);
      }
    }
  }
  else
  {
    tx_thread_sleep(MS_TO_TICK(10));
  }

//  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("Hello STWIN.box!!\r\n"));
  __NOP();
  __NOP();

  return res;
}

