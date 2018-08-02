/** \file lcd.h
*
* @brief ADC Task interface
*/
#ifndef _LCD_H
#define _LCD_H

#define BIT(n)          (1UL << (n))

#define UNIT_M          BIT(0UL)
#define UNIT_FT         BIT(1UL)

#define AT_SURFACE      BIT(0UL)

#define NEUTRAL      BIT(0UL)
#define DESCEND      BIT(1UL)
#define ASCEND       BIT(2UL)

extern OS_FLAG_GRP g_unit;
extern OS_FLAG_GRP g_direction;
extern OS_FLAG_GRP g_surface;

extern void     init_lcd(void);
extern void     lcd_task (void * p_arg);
extern uint32_t get_depth(void);
extern uint32_t add_depth(uint32_t addition);
extern uint32_t sub_depth(uint32_t subtraction);
extern int32_t get_rate(void);
extern int32_t set_rate(int32_t new_rate);
extern uint32_t get_air(void);
extern uint32_t add_air(uint32_t addition);
extern uint32_t sub_air(uint32_t subtraction);

#endif /* _LCD_H */