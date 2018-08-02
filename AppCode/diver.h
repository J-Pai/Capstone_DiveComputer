
#ifndef _DIVER_H
#define _DIVER_H

void diver_task(void);

extern OS_FLAG_GRP g_alarm_flags;
extern OS_Q g_adc_msg_queue;
extern uint32_t currDepth;



#endif _DIVER_H