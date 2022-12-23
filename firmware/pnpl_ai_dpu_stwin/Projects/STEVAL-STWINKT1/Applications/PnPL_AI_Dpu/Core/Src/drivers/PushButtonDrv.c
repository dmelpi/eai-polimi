/*
 * PushButtonDrv.c
 *
 *  Created on: 27 nov 2021
 *      Author: stefano
 */


#include "drivers/PushButtonDrv.h"
#include "drivers/PushButtonDrv_vtbl.h"
#include "FreeRTOS.h"
#include "services/sysdebug.h"

#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_DRIVERS, level, message)

/**
 * Class type for the
 */
typedef struct _PushButtonDrvClass
{
	/**
	 * ::PushButtonDrv_t virtual table.
	 */
	IDriver_vtbl vptr;

} PushButtonDrvClass_t;

/**
 * Specify the only instance of the Class object shared by all PushButtonDrv_t instances.
 */
static PushButtonDrvClass_t sTheClass = {
	{
		PushButtonDrv_vtblInit,
	    PushButtonDrv_vtblStart,
	    PushButtonDrv_vtblStop,
	    PushButtonDrv_vtblDoEnterPowerMode,
	    PushButtonDrv_vtblReset
	}
};

/* Private member function declaration */
/***************************************/


/* Public API definition */
/*************************/

/* IDriver virtual functions definition */
/****************************************/

IDriver *PushButtonDrvAlloc(void)
{
  IDriver *p_new_obj = (IDriver*)pvPortMalloc(sizeof(PushButtonDrv_t));

  if (p_new_obj == NULL) {
    SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("PushButtonDrv - alloc failed.\r\n"));
  }
  else {
    p_new_obj->vptr = &sTheClass.vptr; //&sPushButtonDrv_vtbl;
  }

  return p_new_obj;
}

sys_error_code_t PushButtonDrv_vtblInit(IDriver *_this, void *p_params)
{
  assert_param(_this != NULL);
  assert_param(p_params != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  PushButtonDrv_t *p_obj = (PushButtonDrv_t*)_this;
  p_obj->mx_handle.p_mx_gpio_cfg = ((PushButtonDrvParams_t*)p_params)->p_mx_gpio_cfg;

  p_obj->mx_handle.p_mx_gpio_cfg->p_mx_init_f();

  return res;
}

sys_error_code_t PushButtonDrv_vtblStart(IDriver *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  PushButtonDrv_t *p_obj = (PushButtonDrv_t*)_this;

  HAL_NVIC_EnableIRQ(p_obj->mx_handle.p_mx_gpio_cfg->irq_n);

  return res;
}

sys_error_code_t PushButtonDrv_vtblStop(IDriver *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  PushButtonDrv_t *p_obj = (PushButtonDrv_t*)_this;

  HAL_NVIC_DisableIRQ(p_obj->mx_handle.p_mx_gpio_cfg->irq_n);

  return res;
}

sys_error_code_t PushButtonDrv_vtblDoEnterPowerMode(IDriver *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  PushButtonDrv_t *p_obj = (PushButtonDrv_t*)_this; */

  return res;
}

sys_error_code_t PushButtonDrv_vtblReset(IDriver *_this, void *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  PushButtonDrv_t *p_obj = (PushButtonDrv_t*)_this; */

  return res;
}


/* Private function definition */
/*******************************/
