/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                         STM32F746G-DISCO
*                                         Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : DC
*********************************************************************************************************
*/

#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT


/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/

#define  APP_CFG_GUI_EN                          DEF_ENABLED

/*
*********************************************************************************************************
*                                    APP TASK START PRIO & STK SIZE
*                             Size of the Task Stacks (# of OS_STK Entries)
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_PRIO                    3u
#define  APP_CFG_TASK_START_STK_SIZE              256u


/*
*********************************************************************************************************
*                                        GUI PRIOs & STK SIZEs
*                             Size of the Task Stacks (# of OS_STK Entries)
*********************************************************************************************************
*/
// ------------- GUI TASK PRIO & STK SIZE -------------
#define  APP_CFG_TASK_GUI_PRIO                     25u
#define  APP_CFG_TASK_GUI_STK_SIZE            (4u * 1024u)

// ------------ TOUCH TASK PRIO & STK SIZE ------------
#define  APP_CFG_TOUCH_TASK_PRIO                   24u
#define  APP_CFG_TOUCH_TASK_STK_SIZE              128u

// ------------ LED TASK PRIO & STK SIZE ------------
#define  APP_CFG_LED_TASK_PRIO                   14u
#define  APP_CFG_LED_TASK_STK_SIZE              192u

// ------------ BUTTON DEBOUNCER TASK PRIO & STK SIZE ------------
#define  APP_CFG_DEBOUNCE_TASK_PRIO                   14u
#define  APP_CFG_DEBOUNCE_TASK_STK_SIZE              192u

// ------------ BUTTON DEBOUNCER REACTION TASK PRIO & STK SIZE ------------
#define  APP_CFG_DEBOUNCE_REACT_TASK_PRIO                   14u
#define  APP_CFG_DEBOUNCE_REACT_TASK_STK_SIZE              192u

// ------------ ADC TASK PRIO & STK SIZE ------------
#define  APP_CFG_ADC_TASK_PRIO                   14u
#define  APP_CFG_ADC_TASK_STK_SIZE              192u

// ------------ ADC TASK PRIO & STK SIZE ------------
#define  APP_CFG_ALARM_TASK_PRIO                   14u
#define  APP_CFG_ALARM_TASK_STK_SIZE              192u

// ------------ STARTUP TASK PRIO & STK SIZE ------------
#define  APP_CFG_STARTUP_TASK_PRIO                   3u
#define  APP_CFG_STARTUP_TASK_STK_SIZE              192u

/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                        0u
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                       1u
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                        2u
#endif

#define  APP_TRACE_LEVEL                        TRACE_LEVEL_OFF
#define  APP_TRACE                              printf

#define  APP_TRACE_INFO(x)               ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)                ((APP_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_TRACE x) : (void)0)

#endif
