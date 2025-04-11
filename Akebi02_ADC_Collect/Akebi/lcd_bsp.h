#ifndef __LCD_BSP_H
#define __LCD_BSP_H

#include "komichi.h"
void LcdSprintf(uint8_t Line, char *format, ...);
void lcd_proc(void);

void data_proc(void);

void para_proc(void);

void recd_proc(void);

#endif
