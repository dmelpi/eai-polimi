/*
 * HelloTask.h
 *
 *  Created on: Sep 29, 2022
 *      Author: andre
 */

#ifndef CORE_INC_HELLOTASK_H_
#define CORE_INC_HELLOTASK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "services/AManagedTask.h"
#include "services/AManagedTask_vtbl.h"

typedef struct _HelloTask HelloTask;

struct _HelloTask {
	AManagedTask super;
	// Can add other things here to expand
};


// Public API declaration
//***********************

/**
 * Allocate an instance of AppTask.
 *
 * @return a pointer to the generic obejct ::AManagedTask if success,
 * or NULL if out of memory error occurs.
 */
AManagedTask *HelloTaskAlloc();




#ifdef __cplusplus
}
#endif

#endif /* CORE_INC_HELLOTASK_H_ */
