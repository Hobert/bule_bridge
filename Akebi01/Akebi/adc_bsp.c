#include "adc_bsp.h"

#define window_time_size 3000
#define adc_buff_size 30

uint32_t dma_buff[2][30];
float adc_value[2]; // ADC采样值数组

uint8_t flag_ph_vh;

uint8_t head_point = 0;
uint8_t end_point = 0;

uint8_t flag_vd = 0;

uint32_t diff = 0;

typedef struct ringbuffer_adc
{
    uint32_t timestamp;
    uint32_t adc;
} ringbuffer_adc;

ringbuffer_adc adc_buff[adc_buff_size];

void ringbuffer_adc_init(ringbuffer_adc *buff, uint32_t nowtime, uint32_t adc)
{
    buff[end_point].timestamp = nowtime;
    buff[end_point].adc = adc;
    end_point = (end_point + 1) % adc_buff_size;

    if (end_point == head_point) // 缓冲区已满
    {
        head_point = (head_point + 1) % adc_buff_size;
    }
    while (nowtime - buff[head_point].timestamp > window_time_size) // 判断时间窗口是否超过
    {
        head_point = (head_point + 1) % adc_buff_size; // 删除最早的数据
    }
}

void cheak_adc_change(ringbuffer_adc *buff, uint8_t size)
{
    uint32_t adc_max = buff[head_point].adc;
    uint32_t adc_min = buff[head_point].adc;
    int index = head_point;
    while (index != end_point) //
    {
        if (buff[index].adc > adc_max) // 判断最大值
        {
            adc_max = buff[index].adc;
        }
        if (buff[index].adc < adc_min) // 判断最小值
        {
            adc_min = buff[index].adc;
        }
        index = (index + 1) % size;
    }

    diff = adc_max - adc_min;
    if (diff < pd)
    {
        flag_vd = 1;
        ucled[1] = 0;
    }
    else if (flag_vd == 1)
    {
        flag_vd = 0;
        vd++;
    }
    if (diff > pd)
    {
        ucled[1] = 1;
    }
	LcdSprintf(Line8, "    adc_max=%d     ", adc_max);
    LcdSprintf(Line0, "    adc_min=%d     ", adc_min);
}

void adc_proc(void)
{
    for (uint8_t i = 0; i < 30; i++)
    {
        adc_value[0] += (float)dma_buff[0][i]; // r38
        adc_value[1] += (float)dma_buff[1][i]; // r37
    }
    adc_value[0] = adc_value[0] / 31;
    adc_value[1] = adc_value[1] / 31;

    ringbuffer_adc_init(adc_buff, HAL_GetTick(), adc_value[0]);
    cheak_adc_change(adc_buff, adc_buff_size);

    if (adc_value[1] > ph && flag_ph_vh == 0)
    {
        vh++;
        flag_ph_vh = 1;
    }
    else if (adc_value[1] < ph && flag_ph_vh == 1)
    {
        flag_ph_vh = 0;
    }
	
	
}
