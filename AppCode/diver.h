#ifndef _DIVER_H
#define _DIVER_H

#define TASK_DIVER_STK_SIZE 192
#define APP_CFG_DIVER_TASK_PRIO 14
void diver_task(void);

extern OS_FLAG_GRP g_alarm_flags;
extern OS_Q g_adc_msg_queue;
extern uint32_t currDepth;

#endif 