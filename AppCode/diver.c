


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

#define TASK_DIVER_STK_SIZE 192


OS_TCB   diver_TCB;
CPU_STK  diver_Stk[TASK_DIVER_STK_SIZE];

//Unit stored in milimeters

void diver_task(void){
  OS_ERR err;
  uint32_t size,time,depth,air_cap,prev_time,adcVal=0;
  
  
  int32_t diveRate,airRate;
  CPU_TS tick=5;
  prev_time= get_EDT();
  for(;;){
  //Read ADC 
    adcVal =(uint32_t)OSQPend(&g_adc_msg_queue,0,OS_OPT_PEND_BLOCKING,(OS_MSG_SIZE*)&size,0,&err);
    
  //Map Pot. Val to -50 to 50 m/s
   diveRate = ADC2RATE(adcVal);
  //Get Time
    time=get_EDT();

   //Unneeded if implementation is as expected
  //Get Current Postion 
      //depth=get_DEPTH();

    //Unneeded if implementation is as expected
  //Get Current capacity ()
    //air_cap=get_AIR();
    
 
    if(depth==0){
    if(OSFlagPend(&g_alarm_flags, 0x8u,0, OS_OPT_PEND_BLOCKING | OS_OPT_PEND_FLAG_SET_ANY | OS_OPT_PEND_FLAG_CONSUME, &tick, &err))
      {
        if(get_air()+20<=MAX_AIR_IN_CL)
          air_cap=add_air(20);
        else
          add_air(MAX_AIR_IN_CL-get_air());
      }
    
    }else
    {
       //Call scuba functions to get rate of gas consumption
   airRate=gas_rate_in_cl(depth);
  //Rate of Gas consumption * time from last //High Granularity Intergral Approximation 
  //Subtract change from capacity Assumes that function perfoms the addition
   air_cap=(airRate>=0)?add_air((uint32_t)airRate * (time-prev_time)):sub_air((uint32_t)-airRate * (time-prev_time));
  //Using rate and Elapsed time from last call remove/add depth
    depth=(diveRate>=0)? add_depth((uint32_t)diveRate*(time-prev_time)):sub_depth((uint32_t)-diveRate*(time-prev_time));
    }
    
    
  
  //Evaluate current Air Supply set flag accordingly 
    if(air_cap<(gas_to_surface_in_cl(depth)))        
        OSFlagPost(&g_alarm_flags,0x1,OS_OPT_POST_FLAG_SET,&err);
  //Evaluate rate of ascent set flag accordingly  
    if(diveRate>ASCENT_RATE_LIMIT)
      OSFlagPost(&g_alarm_flags,0x2,OS_OPT_POST_FLAG_SET,&err);
  //Evaluate Current Depth set flag accordingly 
    if(depth>MAX_DEPTH_IN_M)
      OSFlagPost(&g_alarm_flags,0x4,OS_OPT_POST_FLAG_SET,&err);
  
    
  }
}