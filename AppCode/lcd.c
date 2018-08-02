/** \file lcd.c
*
* @brief LCD Task
*/


#include <stdlib.h>  // NULL
#include <stdio.h>   // sprintf()
#include "os.h"
#include "project.h"
#include "GUIDEMO_API.h"
#include "lcd.h"
#include "alarm.h"
#include "scuba.h"

OS_FLAG_GRP g_unit;
OS_FLAG_GRP g_direction;
OS_FLAG_GRP g_surface;

uint32_t g_depth;
uint32_t g_rate;
uint32_t g_air;

OS_MUTEX g_depth_mutex;
OS_MUTEX g_rate_mutex;
OS_MUTEX g_air_mutex;

void init_lcd() {
  OS_ERR err;

  g_depth = 0;
  g_air = 50U;
  
  OSMutexCreate(&g_depth_mutex, "Protects Depth Variable", &err);
  my_assert(OS_ERR_NONE == err);
  OSMutexCreate(&g_rate_mutex, "Protects Rate Variable", &err);
  my_assert(OS_ERR_NONE == err);
  OSMutexCreate(&g_air_mutex, "Protects Air Variable", &err);
  my_assert(OS_ERR_NONE == err);
}

void lcd_task(void * p_arg) {
  OS_ERR err;
  char p_str[128]; //test string
  
  OS_TICK start = OSTimeGet(&err);
  my_assert(OS_ERR_NONE == err);
  
  for (;;) {
    OSTimeDlyHMSM(0, 0, 0, 20, OS_OPT_TIME_HMSM_STRICT, &err);
    my_assert(OS_ERR_NONE == err);
      
    sprintf(p_str, "Company Name"); //test string
    GUIDEMO_API_writeLine(0u, p_str);
    sprintf(p_str, "DEPTH: %4u", get_depth()); //test string
    GUIDEMO_API_writeLine(1u, p_str);
    
    uint32_t dir_flag = (uint32_t)OSFlagPend(&g_direction, ASCEND | DESCEND | NEUTRAL, 0,
                        OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME, NULL, &err);
    my_assert(OS_ERR_NONE == err);
    char * flag = (dir_flag == ASCEND) ? "ASCEND" : ((dir_flag == DESCEND) ? "DESCEND" : "NEUTRAL");
    sprintf(p_str, "RATE: %s %4d", flag, get_rate()); //test string
    GUIDEMO_API_writeLine(2u, p_str);
    sprintf(p_str, "AIR: %4u", get_air()); //test string
    GUIDEMO_API_writeLine(3u, p_str);
    sprintf(p_str, "EDT: %4u", (OSTimeGet(&err) - start) / 1000); //test string
    GUIDEMO_API_writeLine(4u, p_str);

  }
}

uint32_t get_depth() {
  OS_ERR err;
  OSMutexPend(&g_depth_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  my_assert(OS_ERR_NONE == err);
  uint32_t temp = g_depth;
  OSMutexPost(&g_depth_mutex, OS_OPT_POST_NONE, &err);
  my_assert(OS_ERR_NONE == err);
  return temp;
}

uint32_t add_depth(uint32_t addition) {
  OS_ERR err;
  OSMutexPend(&g_depth_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  my_assert(OS_ERR_NONE == err);
  g_depth = g_depth + addition;
  uint32_t temp = g_depth;
  OSMutexPost(&g_depth_mutex, OS_OPT_POST_NONE, &err);
  my_assert(OS_ERR_NONE == err);
  return temp;
}

uint32_t sub_depth(uint32_t subtraction) {
  OS_ERR err;
  OSMutexPend(&g_depth_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  my_assert(OS_ERR_NONE == err);
  g_depth = g_depth - subtraction;
  uint32_t temp = g_depth;
  OSMutexPost(&g_depth_mutex, OS_OPT_POST_NONE, &err);
  my_assert(OS_ERR_NONE == err);
  return temp;
}

int32_t get_rate() {
  OS_ERR err;
  OSMutexPend(&g_depth_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  my_assert(OS_ERR_NONE == err);
  int32_t temp = g_rate;
  OSMutexPost(&g_depth_mutex, OS_OPT_POST_NONE, &err);
  my_assert(OS_ERR_NONE == err);
  return temp;
}

int32_t set_rate(int32_t new_rate) {
  OS_ERR err;
  OSMutexPend(&g_depth_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  my_assert(OS_ERR_NONE == err);
  g_rate = new_rate;
  int32_t temp = g_rate;
  OSMutexPost(&g_depth_mutex, OS_OPT_POST_NONE, &err);
  my_assert(OS_ERR_NONE == err);
  return temp;
}

uint32_t get_air() {
  OS_ERR err;
  OSMutexPend(&g_depth_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  my_assert(OS_ERR_NONE == err);
  uint32_t temp = g_air;
  OSMutexPost(&g_depth_mutex, OS_OPT_POST_NONE, &err);
  my_assert(OS_ERR_NONE == err);
  return temp;
}

uint32_t add_air(uint32_t addition) {
  OS_ERR err;
  uint32_t max = 2000UL;
  uint32_t temp;
  OSMutexPend(&g_air_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  my_assert(OS_ERR_NONE == err);
  g_air = g_air + addition;
  if (g_air < max) {
    temp = g_air;
  } else {
    temp = max;
    g_air = max;
  }
  OSMutexPost(&g_air_mutex, OS_OPT_POST_NONE, &err);
  my_assert(OS_ERR_NONE == err);
  return temp;
}

uint32_t sub_air(uint32_t subtraction) {
  OS_ERR err;
  OSMutexPend(&g_air_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  my_assert(OS_ERR_NONE == err);
  g_air = g_air - subtraction;
  uint32_t temp = g_air;
  OSMutexPost(&g_air_mutex, OS_OPT_POST_NONE, &err);
  my_assert(OS_ERR_NONE == err);
  return temp;
}