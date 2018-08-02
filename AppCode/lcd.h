/** \file lcd.h
*
* @brief ADC Task interface
*/
#ifndef _ADC_H
#define _ADC_H

#define BIT(n) (1UL << (n))

#define UNIT_M BIT(0UL);
#define UNIT_F BIT(1UL);

#define AT_SURFACE BIT(0UL);

extern OS_FLAG_GRP g_unit;
extern OS_FLAG_GRP g_direction;
extern OS_FLAG_GRP g_surface;

extern void     lcd_task (void * p_arg);
extern uint32_t get_depth(void);
extern uint32_t add_depth(uint32_t addition);
extern uint32_t sub_depth(uint32_t subtraction);
extern uint32_t get_air(void);
extern uint32_t add_air(uint32_t addition);
extern uint32_t sub_air(uint32_t subtraction);

#endif /* _ADC_H */