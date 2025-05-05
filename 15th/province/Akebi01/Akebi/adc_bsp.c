#include "adc_bsp.h"

uint32_t dma_buff[2][30];
float adc_value[2]; // ADC采样值数组

void adc_proc(void)
{
    for (uint8_t i = 0; i < 30; i++)
    {
        adc_value[0] += (float)dma_buff[0][i]; // r38
        adc_value[1] += (float)dma_buff[1][i]; // r37
    }
    adc_value[0] = adc_value[0] / 31;
    adc_value[1] = adc_value[1] / 31;

}
