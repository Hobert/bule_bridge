#ifndef BSP_SYSTEM_H
#define BSP_SYSTEM_H

#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#include "rtc.h"
#include "lcd.h"
#include "main.h"
#include "usart.h"
#include "system.h"
#include "filter.h"
#include "scheduler.h"
#include "led_app.h"
#include "key_app.h"
#include "lcd_app.h"
#include "uart_app.h"
#include "adc_app.h"
#include "rtc_app.h"
#include "tim_app.h"
#include "ringbuffer.h"

extern uint16_t uart_rx_index;//接收缓存索引
extern uint32_t uart_rx_ticks;//接收缓存时间戳
extern uint8_t uart_rx_buffer[128];//接收缓存
extern uint8_t ucLed[8];// LED 状态数组
extern uint8_t uart_rx_dma_buffer[128];//接收缓存
extern uint32_t dma_buff[2][30];//DMA接收缓存
extern float adc_value[2];//ADC采样值数组
extern RTC_TimeTypeDef time;//定义时间结构体
extern RTC_DateTypeDef date;//定义日期结构体
extern uint32_t tim_ic_buffer[64];// 定义存储输入捕获值的缓冲区
extern uint32_t tim_ic_val;// 最终计算得到的输入捕获值


extern uint8_t lcd_disp_mode;//显示模式 0-数据界面 1-参数界面 2-统计界面
extern uint8_t pwm_output_mode;//Pwm输出模式 0-低频 1-高频
extern uint8_t duty_val;//pwm输出占空比
extern float speed_val;//速度值
extern uint8_t set_val[2];//设置值 0-R 1-K
extern uint8_t set_val_set[2];//设置值 0-R 1-K
extern uint8_t set_index;//设置索引
extern uint8_t duty_lock_flag; // 占空比锁定标志 0-未锁定 1-已锁定
extern float speed_max[2];//最大速度值
extern uint8_t pwm_change_count;//PWM输出模式切换计数值


#endif


