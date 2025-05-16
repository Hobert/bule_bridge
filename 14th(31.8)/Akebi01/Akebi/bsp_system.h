#ifndef __BSP_SYSTEM_H
#define __BSP_SYSTEM_H

#include "main.h"
#include "Komichi.h"
#include "akebi.h"
#include "led_bsp.h"
#include "key_bsp.h"
#include "adc_bsp.h"
#include "tim_app.h"
#include "filter.h"

#include "ringbuffer.h"

#include "usart_bsp.h"
#include "usart.h"

#include "lcd_bsp.h"
#include "lcd.h"

#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"

extern uint8_t ucled[8];
extern uint16_t uart_rx_index;
extern uint32_t uart_rx_ticks;
extern uint8_t uart_rx_buffer[128];
extern uint8_t uart_rx_dma_buffer[128];
extern uint8_t uart_rx_ringbuffer[128];

extern uint32_t tim_ic_buffer[64];

extern uint32_t dma_buff[2][30];
extern float adc_value[2]; // ADC采样值数组

extern uint8_t set_value[2];
extern uint8_t set_dis_value[2];
extern uint8_t n;
extern float v, reco_velocity;
extern uint8_t Lcd_display_mode;
extern uint8_t data_fulse_flag;
extern uint8_t value_r_k_flag;

extern uint8_t frequency_change_flag;

extern uint8_t duty_change_lock;

extern float pwm_duty;

extern uint16_t pwm_frequency;

extern uint32_t frequency_time;

extern uint8_t fulse_change_flag;

extern float speed_value[2];

#endif
