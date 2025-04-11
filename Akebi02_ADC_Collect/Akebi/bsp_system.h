#ifndef __BSP_SYSTEM_H
#define __BSP_SYSTEM_H

#include "main.h"
#include "Komichi.h"
#include "akebi.h"
#include "led_bsp.h"
#include "key_bsp.h"
#include "adc_bsp.h"

#include "ringbuffer.h"

#include "usart_bsp.h"
#include "usart.h"

#include "lcd_bsp.h"
#include "lcd.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#include "math.h"

#include "adc.h"

#include "collect.h"

extern uint8_t ucled[8];
extern uint16_t uart_rx_index;
extern uint32_t uart_rx_ticks;
extern uint8_t uart_rx_buffer[128];
extern uint8_t uart_rx_dma_buffer[128];
extern uint8_t uart_rx_ringbuffer[128];

extern uint32_t dma_buff[2][30];
extern uint32_t adc_value[2]; // ADC采样值数组

extern uint8_t memu_choose;      // 界面选择
extern uint8_t memu_choose_last; // 上次界面选择
extern uint8_t flag_choose;      // 切换当前选择的参数(PD / PH)

extern uint32_t r37;
extern uint32_t r38;

extern uint32_t pd;
extern uint32_t ph;

extern float vh;
extern float vd;

extern int key4_test;

#endif
