/** \file adc.h
*
* @brief ADC Task interface
*
* @par
* COPYRIGHT NOTICE: (c) 2017 Barr Group, LLC.
* All rights reserved.
*/
#ifndef _ADC_H
#define _ADC_H
#define TIME_DIVER_STK_SIZE 100
#define TIME_CFG_DIVER_TASK_PRIO 14

extern void     adc_task (void * p_arg);
extern void     ADC_IRQHandler(void);
extern uint32_t get_time(void);
extern void m_time(void * p_arg);
#endif /* _ADC_H */
