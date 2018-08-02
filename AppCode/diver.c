


/** \file scuba.c

 $COPYRIGHT: $
 -    Copyright 2017 Barr Group, LLC. All rights reserved.
**/

#include <stdint.h>
#include "os.h"
#include "scuba.h"

#define TASK_DIVER_STK_SIZE 192

static  OS_TCB   diver_TCB;
static  CPU_STK  diver_Stk[TASK_DIVER_STK_SIZE];