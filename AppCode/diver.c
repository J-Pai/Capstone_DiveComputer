


/** \file scuba.c

 $COPYRIGHT: $
 -    Copyright 2017 Barr Group, LLC. All rights reserved.
**/

#include <stdint.h>
#include "os.h"
#include "scuba.h"
#include "adc.h"
#include "diver.h"
#include "lcd.h"

#define test
#ifdef test
//int (*get_EDT)()= &OS_TS_GET;
#elif
//int (*get_EDT)()=
#endif
//Unit stored in milimeters

void diver_task(void){
  OS_ERR err;
  OS_MSG_SIZE size;
  uint32_t time, depth, air_cap, prev_time, adcVal = 0;
  
  prev_time= OS_TS_GET();//get_EDT();
  
  for(;;){
    //Read ADC 
    adcVal = (uint32_t)OSQPend(&g_adc_msg_queue, 0, OS_OPT_PEND_BLOCKING, &size, 0, &err);
    
    //Map Pot. Val to -50 to 50 m/s
    int32_t diveRate = ADC2RATE((int32_t)adcVal);
   
    if (diveRate >= 0) { 
      OSFlagPost(&g_direction, 0x1, OS_OPT_POST_FLAG_SET, &err);
    } else {
      OSFlagPost(&g_direction,0x1,OS_OPT_POST_FLAG_CLR,&err);
    }
   
    //Get Time
    time=OS_TS_GET();//get_EDT();

    if(depth == 0){
      OSFlagPost(&g_surface, 0x1u, OS_OPT_POST_FLAG_SET, &err);
            depth = (diveRate>=0) ? add_depth((uint32_t)diveRate*(time-prev_time)) : get_depth();
    } else {
      //Call scuba functions to get rate of gas consumption
      uint32_t airRate = gas_rate_in_cl(depth);
      //Rate of Gas consumption * time from last //High Granularity Intergral Approximation 
      //Subtract change from capacity Assumes that function perfoms the addition
      air_cap = sub_air(airRate * (time-prev_time));
      //Using rate and Elapsed time from last call remove/add depth
      depth = (diveRate>=0) ? add_depth((uint32_t)diveRate*(time-prev_time)) : sub_depth((uint32_t)-diveRate*(time-prev_time));
    }
    
    //Evaluate current Air Supply set flag accordingly 
    if(air_cap < (gas_to_surface_in_cl(depth)))        
      OSFlagPost(&g_alarm_flags, 0x1, OS_OPT_POST_FLAG_SET, &err);
    //Evaluate rate of ascent set flag accordingly  
    if(diveRate > ASCENT_RATE_LIMIT)
      OSFlagPost(&g_alarm_flags, 0x2, OS_OPT_POST_FLAG_SET, &err);
    //Evaluate Current Depth set flag accordingly 
    if(depth > MAX_DEPTH_IN_M)
      OSFlagPost(&g_alarm_flags, 0x4, OS_OPT_POST_FLAG_SET, &err);
  }
}