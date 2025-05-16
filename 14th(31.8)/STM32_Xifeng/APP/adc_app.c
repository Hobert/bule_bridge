#include "adc_app.h"

uint32_t dma_buff[2][30];//DMA接收缓存
float adc_value[2];//ADC采样值数组

uint32_t pwm_tick;//PWM计时器
uint16_t freq_val = 4000;//PWM频率值

void adc_proc(void)
{
    for(uint8_t i=0;i<30;i++)
    {
        adc_value[0] += (float)dma_buff[0][i];
        adc_value[1] += (float)dma_buff[1][i];
    }

    adc_value[0] = adc_value[0] / 30 * 3.3f / 4096;
    adc_value[1] = adc_value[1] / 30 * 3.3f / 4096;

    if(duty_lock_flag == 0)
    {
        if(adc_value[1] <= 1.0f)
            duty_val = 10;
        else if(adc_value[1] >= 3.0f)
            duty_val = 85;
        else
            duty_val = (85-10) / (3.0f-1.0f) * (adc_value[1]-1.0f) + 10;
    }

    if(uwTick - pwm_tick >= 100)
    {
        if(freq_val != (pwm_output_mode?8000:4000))
        {
            ucLed[1] ^= 1;
            freq_val += (4000 / 50) * (pwm_output_mode?1:-1);
            pwm_set_frequency(freq_val);
        }
        else
        {
            ucLed[1] = 0;
        }
        pwm_tick = uwTick;
    }


    pwm_set_duty(duty_val);
}

