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
    adc_value[1] = adc_value[1] / 31 / 4096 * (float)3.3; // R37

    if (!duty_change_lock)
    {
        if (adc_value[1] < 1)
        {
            pwm_duty = 10;
        }
        else if (adc_value[1] > 1)
        {
            pwm_duty = (float)37.5 * adc_value[1] - (float)27.5; // ��37.5˫����ת��Ϊ������
        }
        else if (adc_value[1] > 3)
        {
            pwm_duty = 85;
        }
    }
    pwm_set_duty(pwm_duty);
}
