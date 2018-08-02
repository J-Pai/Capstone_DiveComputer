/** \file switches.c
*
* @brief Switches Tasks
*/


#include <stdlib.h>  // NULL
#include <stdio.h>   // sprintf()
#include "os.h"
#include "project.h"
#include "GUIDEMO_API.h"
#include "lcd.h"
#include "switches.h"
#include "pushbutton.h"

// *****************************************************************
//  SW1 Task - Toggle the units of displayed depth and rate
// *****************************************************************
void toggle_unit_task(void * p_arg) {
    OS_ERR err;
    char p_str[14]; //test string
    (void)p_arg;
    for (;;) {
      OSSemPend(&g_sw1_sem, 0, OS_OPT_PEND_BLOCKING, 0, &err);
      my_assert(OS_ERR_NONE == err);
      // get flag about current units
      
      uint32_t flag = (uint32_t)OSFlagPend(&g_unit, UNIT_M | UNIT_FT, 0,
                          OS_OPT_PEND_BLOCKING | OS_OPT_PEND_FLAG_SET_ANY | OS_OPT_PEND_FLAG_CONSUME, 
                          NULL, &err);
      my_assert(OS_ERR_NONE == err);
      
      if( flag == UNIT_M) {
        // Post Flag - Feet
        OSFlagPost(&g_unit, UNIT_FT, OS_OPT_POST_FLAG_SET, &err);
        my_assert(OS_ERR_NONE == err);
        sprintf(p_str, "Unit was M"); //test string
      } else {
        // Post Flag - Meters
        OSFlagPost(&g_unit, UNIT_M, OS_OPT_POST_FLAG_SET, &err);
        my_assert(OS_ERR_NONE == err);
        sprintf(p_str, "Unit was FT"); //test string
      }
      // testing!
      GUIDEMO_API_writeLine(1u, p_str);
  }
  
}
// *****************************************************************
//  SW2 Task - Add air to the tank in 20 liter increments (surface)
// *****************************************************************
void add_air_task(void * p_arg) {
    OS_ERR err;
    char p_str[14]; //test string
    uint16_t count = 0;
    (void)p_arg;
    for (;;) {
      OSSemPend(&g_sw2_sem, 0, OS_OPT_PEND_BLOCKING, 0, &err);
      my_assert(OS_ERR_NONE == err);
      count++;
      sprintf(p_str, "Air was added %4u times", count); //test string
      // testing!
      GUIDEMO_API_writeLine(0u, p_str);
      // only add if at surface
      //uint32_t flag = (uint32_t)OSFlagPend(&g_unit, AT_SURFACE, 0,
      //                    OS_OPT_PEND_BLOCKING | OS_OPT_PEND_FLAG_SET_ANY | OS_OPT_PEND_FLAG_CONSUME, 
      //                    NULL, &err);
      //my_assert(OS_ERR_NONE == err);
      
      //shouldn't need if statement, but just to check
     // if( flag == AT_SURFACE) {
       // add_air(20UL);
        //sprintf(p_str, "Air was added"); //test string
      //}
      // testing!
      //GUIDEMO_API_writeLine(0u, p_str);
  }
}