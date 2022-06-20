/**
 ******************************************************************************
 * @file    UsbCdcTask.h
 * @author  STMicroelectronics
 * @version 1.0.0
 * @date    May 21, 2022
 *
 * @brief USB_CDC Class
 *
 * This managed task is responsible to manage the USB_CDC communication
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */
#ifndef USBCDCTASK_H_
#define USBCDCTASK_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/systp.h"
#include "services/syserror.h"
#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "IUsbDelegate.h"
#include "IUsbDelegate_vtbl.h"

#define APP_RX_DATA_SIZE                         2048

/**
 * Create  type name for _UsbCdcTask.
 */
typedef struct _UsbCdcTask UsbCdcTask_t;

/**
 *  UsbCdcTask internal structure.
 */
struct _UsbCdcTask {
  /**
   * Base class object.
   */
  AManagedTaskEx super;

  /**
   * Task input message queue. The task receives message of type struct AIMessage_t in this queue.
   * This is one of the way the task expose its services at application level.
   */
  TX_QUEUE in_queue;

  /*
   * Mutex to protect USB Write function
   */
  TX_MUTEX write_mutex;

  /*
   * Delegate interface for received USB data.
   */
  IUsbDelegate *p_delegate;

  /*
   * Data received over USB is stored in this buffer
   */
  uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

};

// Public API declaration
//***********************

/**
 * Allocate an instance of UsbCdcTask.
 *
 * @return a pointer to the generic obeject ::AManagedTask if success,
 * or NULL if out of memory error occurs.
 */
AManagedTaskEx *UsbCdcTaskAlloc(void);

/**
 * Static function to send data to the USB interface.
 * If the buffer is too big to be sent in a single shot, the function will take care of
 * splitting it into smaller chunks.
 *
 * @param buffer [IN] Pointer to data to write.
 * @param requested_length [IN] Length of bytes to write.
 * @param actual_length [OUT] Pointer to save the actual number of bytes written
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t UsbCdcTask_Write(UCHAR *buffer, ULONG requested_length, ULONG *actual_length);

/**
 * Set the USB delegate to manage received USB data
 *
 * @param _this [IN] specifies a pointer to the object.
 * @param delegate [IN] specifies a pointer to the delegate instance.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t UsbCdcTask_SetDelegate(UsbCdcTask_t *_this, IUsbDelegate *delegate);


// Inline functions definition
// ***************************

#ifdef __cplusplus
}
#endif

#endif /* USBCDCTASK_H_ */
