/*
 * HelloTask.c
 *
 *  Created on: Sep 29, 2022
 *      Author: andre
 */


#include "HelloTask.h"
#include "HelloTask_vtbl.h"
#include "services/sysdebug.h"


#ifndef HELLO_TASK_CFG_STACK_DEPTH
#define HELLO_TASK_CFG_STACK_DEPTH           TX_MINIMUM_STACK*10
#endif

#ifndef HELLO_TASK_CFG_PRIORITY
#define HELLO_TASK_CFG_PRIORITY              (TX_MAX_PRIORITIES-2)
#endif

#define SYS_DEBUGF(level, message)         SYS_DEBUGF3(SYS_DBG_HW, level, message)

static HelloTask sTaskObj;

// Private member function declaration
// ***********************************

/**
 * Execute one step of the task control loop while the system is in RUN mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t HelloTaskExecuteStepRun(AManagedTask *_this);

typedef struct _HelloClass {
	AManagedTask_vtbl vtbl;
	pExecuteStepFunc_t pPMState2FuncMap[];
} HelloClass;

static const HelloClass sTheClass = {
		{
			HelloTask_vtblHardwareInit,
			HelloTask_vtblOnCreateTask,
			HelloTask_vtblDoEnterPowerMode,
			HelloTask_vtblHandleError,
			HelloTask_vtblOnEnterTaskControlLoop
		},
	    {
	        HelloTaskExecuteStepRun,
	        NULL,
	        NULL,
	        NULL,
	        NULL,
	        NULL,
	    }
};

AManagedTask *HelloTaskAlloc(void) {
	AMTInit(&sTaskObj.super);
	sTaskObj.super.vptr = &sTheClass.vtbl;

	return (AManagedTask*) &sTaskObj;
}

sys_error_code_t HelloTask_vtblHardwareInit(AManagedTask *this, void *pParams)
{
	assert_param(this != NULL);
	sys_error_code_t xRes = SYS_NO_ERROR_CODE;
	HelloTask *pObj = (HelloTask*)this;
	// init some drivers and such
	UNUSED(pObj);

	return xRes;
}
sys_error_code_t HelloTask_vtblOnCreateTask(AManagedTask *_this,
		tx_entry_function_t *pTaskCode,
		CHAR **pName,
		VOID **pStackStart,
		ULONG *pStackDepth,
		UINT *pPriority,
		UINT *pPreemptThreshold,
		ULONG *pTimeSlice,
		ULONG *pAutoStart,
		ULONG *pParams)
{
	assert_param(_this != NULL);
	sys_error_code_t xRes = SYS_NO_ERROR_CODE;

	_this->m_pfPMState2FuncMap = sTheClass.pPMState2FuncMap;

	*pTaskCode = AMTRun;
	*pName = "HW";
	*pStackStart = NULL;
	*pStackDepth = HELLO_TASK_CFG_STACK_DEPTH;
	*pParams = (ULONG)_this;
	*pPriority = HELLO_TASK_CFG_PRIORITY;
	*pPreemptThreshold = HELLO_TASK_CFG_PRIORITY;
	*pTimeSlice = TX_NO_TIME_SLICE;
	*pAutoStart = TX_AUTO_START;

	return xRes;
}
sys_error_code_t HelloTask_vtblDoEnterPowerMode(AManagedTask *this, const EPowerMode eActivePowerMode, const EPowerMode eNewPowerMode)
{
	assert_param(this);
	sys_error_code_t xRes = SYS_NO_ERROR_CODE;

	return xRes;
}
sys_error_code_t HelloTask_vtblHandleError(AManagedTask *this, SysEvent xError)
{
	assert_param(this);
	sys_error_code_t xRes = SYS_NO_ERROR_CODE;

	return xRes;
}
sys_error_code_t HelloTask_vtblOnEnterTaskControlLoop(AManagedTask *this)
{
	assert_param(this);
	sys_error_code_t xRes = SYS_NO_ERROR_CODE;

	HelloTask *pObj = (HelloTask*)this;

	UNUSED(pObj);
	SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: start.\r\n"));

	return xRes;
}


// Private function definition
// ***************************

static sys_error_code_t HelloTaskExecuteStepRun(AManagedTask *_this)
{
	assert_param(_this);
	sys_error_code_t xRes = SYS_NO_ERROR_CODE;

	SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: Hello!\r\n"));

	tx_thread_sleep(5000);

	return xRes;
}

