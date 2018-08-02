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

OS_FLAG_GRP g_unit;
OS_FLAG_GRP g_direction;
OS_FLAG_GRP g_surface;

static uint32_t g_depth;
static uint32_t g_air;

static OS_MUTEX g_depth_mutex;
static OS_MUTEX g_air_mutex;

void init_lcd() {
  OS_ERR err;

  OSMutexCreate(&g_depth_mutex, "Protects Depth Variable", &err);
  my_assert(OS_ERR_NONE == err);
  
  OSMutexCreate(&g_air_mutex, "Protects Air Variable", &err);
  my_assert(OS_ERR_NONE == err);
}

void lcd_task(void * p_arg) {
  OS_ERR err;
  OSTimeDlyHMSM(0, 0, 0, 10, OS_OPT_TIME_HMSM_STRICT, &err);
  my_assert(OS_ERR_NONE == err);

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
  OSMutexPend(&g_air_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  my_assert(OS_ERR_NONE == err);
  g_air = g_air + addition;
  uint32_t temp = g_air;
  OSMutexPost(&g_depth_mutex, OS_OPT_POST_NONE, &err);
  my_assert(OS_ERR_NONE == err);
  return temp;
}

uint32_t sub_air(uint32_t subtraction) {
  OS_ERR err;
  OSMutexPend(&g_air_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  my_assert(OS_ERR_NONE == err);
  g_air = g_air - subtraction;
  uint32_t temp = g_air;
  OSMutexPost(&g_depth_mutex, OS_OPT_POST_NONE, &err);
  my_assert(OS_ERR_NONE == err);
  return temp;
}