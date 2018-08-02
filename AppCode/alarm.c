/** \file alarm.c
*
* @brief Alarm Manager
*
* @par
* COPYRIGHT NOTICE: (c) 2017 Barr Group, LLC.
* All rights reserved.
*/

#include <stdint.h>
#include <stdio.h>
            
#include "project.h"
#include "os.h"
#include "alarm.h"
#include "GUIDEMO_API.h"

OS_FLAG_GRP g_alarm_flags;
/*!
*
* @brief Alarm Task
*/
void alarm_task(void * p_arg)
{
  OS_ERR err;
  // TODO: This task should pend on events from adc_task(),
  //       and then update the LCD background color accordingly.
  char * msg;

  for (;;) {
    char p_str[128];

    uint32_t flag = (uint32_t)OSFlagPend(&g_alarm_flags, ALARM_NONE | ALARM_LOW | ALARM_MEDIUM | ALARM_HIGH,
               0, OS_OPT_PEND_BLOCKING | OS_OPT_PEND_FLAG_SET_ANY | OS_OPT_PEND_FLAG_CONSUME, NULL, &err);
    my_assert(OS_ERR_NONE == err);
    
    switch(flag) {
      case ALARM_NONE:
        msg = "NONE";
        GUIDEMO_SetColorBG(BG_COLOR_GREEN);
        break;
      case ALARM_LOW:
        msg = "LOW"; 
        GUIDEMO_SetColorBG(BG_COLOR_BLUE);
        break;
     case ALARM_MEDIUM:
        msg = "MEDIUM"; 
        GUIDEMO_SetColorBG(BG_COLOR_YELLOW);
        break;
      case ALARM_HIGH:
        msg = "HIGH"; 
        GUIDEMO_SetColorBG(BG_COLOR_RED);
        break;
    }
    
    // Generate string
    sprintf(p_str, "Alarm: %s", msg);
  
    GUIDEMO_API_writeLine(5u, p_str);
  }
}

