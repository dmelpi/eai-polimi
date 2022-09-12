/**
 ******************************************************************************
 * @file    sysmem.h
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 1.0.0
 * @date    Nov 11, 2020
 *
 * @brief   System memory management.
 *
 * This file declares API function to alloc and release block of memory.
 * The application can use its own memory allocation strategy.
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
#ifndef INCLUDE_SERVICES_SYSMEM_H_
#define INCLUDE_SERVICES_SYSMEM_H_

#include "services/systp.h"
#include "services/systypes.h"


/**
 * Allocate a block of memory of a specific size.
 *
 * @param nSize [IN] specifies the size in byte of the requested memory.
 * @return a pointer to the allocated memory if success, NULL otherwise.
 */
void *SysAlloc(size_t nSize);

/**
 * Release a block of memory.
 *
 * @param pvData [IN] specifies the start of teh block of memory to release.
 */
void SysFree(void *pvData);

#endif /* INCLUDE_SERVICES_SYSMEM_H_ */
