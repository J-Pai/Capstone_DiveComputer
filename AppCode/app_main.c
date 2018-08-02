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
*                                            EXAMPLE CODE
*
*                                         STM32F746G-DISCO
*                                         Evaluation Board
*
* Filename      : app_main.c
* Version       : V1.00
* Programmer(s) : FF
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  "stdarg.h"
#include  "stdio.h"
#include  "stm32f7xx_hal.h"

#include  "cpu.h"
#include  "lib_math.h"
#include  "lib_mem.h"
#include  "os.h"
#include  "os_app_hooks.h"
#include  "GUIDEMO_API.h"

#include  "app_cfg.h"
#include  "bsp.h"
#include  "bsp_led.h"
#include  "bsp_clock.h"
#include  "bsp_pb.h"
#include  "bsp_test.h"
#include  "GUI.h"
#include  "pushbutton.h"
#include  "adc.h"
#include  "alarm.h"
#include  "lcd.h"
#include  "switches.h"
#include  "diver.h"

static OS_MUTEX g_led_mutex;

// *****************************************************************
// Define storage for each Task Control Block (TCB) and task stacks
// *****************************************************************
static  OS_TCB   AppTaskGUI_TCB;
static  CPU_STK  AppTaskGUI_Stk[APP_CFG_TASK_GUI_STK_SIZE];

static OS_TCB led1_task_TCB;
static CPU_STK led1_task_Stk[APP_CFG_LED_TASK_STK_SIZE];

static OS_TCB debounce_task_TCB;
static CPU_STK debounce_task_Stk[APP_CFG_DEBOUNCE_TASK_STK_SIZE];

static OS_TCB debounce_react_task1_TCB;
static CPU_STK debounce_react_task_Stk1[APP_CFG_DEBOUNCE_REACT_TASK_STK_SIZE];

static OS_TCB debounce_react_task2_TCB;
static CPU_STK debounce_react_task_Stk2[APP_CFG_DEBOUNCE_REACT_TASK_STK_SIZE];

static OS_TCB adc_task_TCB;
static CPU_STK adc_task_Stk[APP_CFG_ADC_TASK_STK_SIZE];

static OS_TCB   diver_TCB;
static CPU_STK  diver_Stk[TASK_DIVER_STK_SIZE];

static OS_TCB alarm_task_TCB;
static CPU_STK alarm_task_Stk[APP_CFG_ALARM_TASK_STK_SIZE];

static OS_TCB lcd_task_TCB;
static CPU_STK lcd_task_Stk[APP_CFG_LCD_TASK_STK_SIZE];

static OS_TCB startup_task_TCB;
static CPU_STK startup_task_Stk[APP_CFG_DEBOUNCE_TASK_STK_SIZE];

// *****************************************************************
// Flash LED1 at 3 Hz
// *****************************************************************
static void led1_task(void * p_arg)
{
    OS_ERR err;
  
    uint32_t delay = 1500UL;
    OSMutexPend(&g_led_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
    my_assert(OS_ERR_NONE == err);
    
    BSP_LED_On(LED1);
    
    OSMutexPost(&g_led_mutex,OS_OPT_POST_NONE, &err);
    my_assert(OS_ERR_NONE == err);
    
    // Task main loop
    for (;;)
    {
        // TODO: Toggle LED1
        OSMutexPend(&g_led_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        my_assert(OS_ERR_NONE == err);
        
        BSP_LED_Toggle(LED1);
        
        OSMutexPost(&g_led_mutex,OS_OPT_POST_NONE, &err);
        my_assert(OS_ERR_NONE == err);
            
        // TODO: Sleep for 500ms
        OSTimeDlyHMSM(0, 0, 0, delay, OS_OPT_TIME_HMSM_NON_STRICT, &err);
    }
}


// *****************************************************************
// Startup Task
// *****************************************************************
static void startup_task(void * p_arg) 
{
  OS_ERR err;
  
  // Initialize BSP
  BSP_Init();
  
  debounce_task_init();
  
  GUIDEMO_API_init();
    
  OSFlagCreate(&g_alarm_flags, "ADC Alarm Flag", ALARM_NONE, &err);
  my_assert(OS_ERR_NONE == err);
  
  // Create flag to determine units in (M or FT)
  OSFlagCreate(&g_unit, "Unit Flag", UNIT_M, &err);
  my_assert(OS_ERR_NONE == err);
  
  // Create flag to determine at surface or not
  OSFlagCreate(&g_surface, "Surface Flag", AT_SURFACE, &err);
  my_assert(OS_ERR_NONE == err);
  
  #if OS_CFG_STAT_TASK_EN > 0u
    // Compute CPU capacity with no other task running
    OSStatTaskCPUUsageInit(&err);
    my_assert(OS_ERR_NONE == err);
    OSStatReset(&err);
    my_assert(OS_ERR_NONE == err);
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
    my_assert(OS_ERR_NONE == err);
#endif
    
    OSMutexCreate(&g_led_mutex, "Protects LED Driver", &err);
    my_assert(OS_ERR_NONE == err);
    init_lcd();
    
    // Create the GUI task
    OSTaskCreate(&AppTaskGUI_TCB, "uC/GUI Task", (OS_TASK_PTR ) GUI_DemoTask,
                 0, APP_CFG_TASK_GUI_PRIO,
                 &AppTaskGUI_Stk[0], (APP_CFG_TASK_GUI_STK_SIZE / 10u),
                  APP_CFG_TASK_GUI_STK_SIZE, 0u, 0u, 0,
                  (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
    my_assert(OS_ERR_NONE == err);
 
    // TODO: Create task to blink LED1
    OSTaskCreate(&led1_task_TCB, "LED1 Task", (OS_TASK_PTR ) led1_task,
                 0, APP_CFG_LED_TASK_PRIO,
                 &led1_task_Stk[0], (APP_CFG_LED_TASK_STK_SIZE / 10u),
                  APP_CFG_LED_TASK_STK_SIZE, 0u, 0u, 0,
                  (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
    my_assert(OS_ERR_NONE == err);
    
    // TODO: Create task to do button debouncer
    OSTaskCreate(&debounce_task_TCB, "Debounce Task", (OS_TASK_PTR ) debounce_task,
                 0, APP_CFG_DEBOUNCE_TASK_PRIO,
                 &debounce_task_Stk[0], (APP_CFG_DEBOUNCE_TASK_STK_SIZE / 10u),
                  APP_CFG_DEBOUNCE_TASK_STK_SIZE, 0u, 0u, 0,
                  (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
    my_assert(OS_ERR_NONE == err);
    
    // Switch 1 Reaction Task
    OSTaskCreate(&debounce_react_task1_TCB, "React SW1 Task", (OS_TASK_PTR ) toggle_unit_task,
                 0, APP_CFG_DEBOUNCE_REACT_TASK_PRIO,
                 &debounce_react_task_Stk1[0], (APP_CFG_DEBOUNCE_REACT_TASK_STK_SIZE / 10u),
                  APP_CFG_DEBOUNCE_REACT_TASK_STK_SIZE, 0u, 0u, 0,
                  (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
    my_assert(OS_ERR_NONE == err);
    
    // Switch 2 Reaction Task
    OSTaskCreate(&debounce_react_task2_TCB, "React SW2 Task", (OS_TASK_PTR ) add_air_task,
                 0, APP_CFG_DEBOUNCE_REACT_TASK_PRIO,
                 &debounce_react_task_Stk2[0], (APP_CFG_DEBOUNCE_REACT_TASK_STK_SIZE / 10u),
                  APP_CFG_DEBOUNCE_REACT_TASK_STK_SIZE, 0u, 0u, 0,
                  (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
    my_assert(OS_ERR_NONE == err);
    
    /*
    // ADC Task
    OSTaskCreate(&adc_task_TCB, "ADC Task", (OS_TASK_PTR ) adc_task,
                 0, APP_CFG_ADC_TASK_PRIO,
                 &adc_task_Stk[0], (APP_CFG_ADC_TASK_STK_SIZE / 10u),
                  APP_CFG_ADC_TASK_STK_SIZE, 0u, 0u, 0,
                  (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
    my_assert(OS_ERR_NONE == err);
    */
    
    // Alarm Task
    OSTaskCreate(&alarm_task_TCB, "Alarm Task", (OS_TASK_PTR ) alarm_task,
                 0, APP_CFG_ALARM_TASK_PRIO,
                 &alarm_task_Stk[0], (APP_CFG_ALARM_TASK_STK_SIZE / 10u),
                  APP_CFG_ALARM_TASK_STK_SIZE, 0u, 0u, 0,
                  (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
    my_assert(OS_ERR_NONE == err);
    
    // LCD Task
    OSTaskCreate(&lcd_task_TCB, "LCD Task", (OS_TASK_PTR ) lcd_task,
                 0, APP_CFG_LCD_TASK_PRIO,
                 &lcd_task_Stk[0], (APP_CFG_LCD_TASK_STK_SIZE / 10u),
                 APP_CFG_LCD_TASK_STK_SIZE, 0u, 0u, 0,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
    my_assert(OS_ERR_NONE == err);

    //Diver Task
    /*
    OSTaskCreate(&diver_TCB, "diver Task", (OS_TASK_PTR ) diver_task,
                 0, APP_CFG_DIVER_TASK_PRIO ,
                 &diver_Stk[0], (TASK_DIVER_STK_SIZE/ 10u),
                 TASK_DIVER_STK_SIZE, 0u, 0u, 0,
                (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
    my_assert(OS_ERR_NONE == err);*/
}

// *****************************************************************
int main(void)
{
    OS_ERR   err;

    HAL_Init();
    BSP_SystemClkCfg();   // Init. system clock frequency to 200MHz
    CPU_Init();           // Initialize the uC/CPU services
    Mem_Init();           // Initialize Memory Managment Module
    Math_Init();          // Initialize Mathematical Module
    CPU_IntDis();         // Disable all Interrupts.

    // TODO: Init uC/OS-III.
    OSInit(&err);
    my_assert(OS_ERR_NONE == err);
    
    // Startup Task
    OSTaskCreate(&startup_task_TCB, "Startup Task", (OS_TASK_PTR ) startup_task,
             0, APP_CFG_STARTUP_TASK_PRIO,
             &startup_task_Stk[0], (APP_CFG_STARTUP_TASK_STK_SIZE / 10u),
              APP_CFG_STARTUP_TASK_STK_SIZE, 0u, 0u, 0,
              (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
    // TODO: Start multitasking (i.e. give control to uC/OS-III)
    OSStart(&err);
    
    // Should never get here
    my_assert(0);
}

