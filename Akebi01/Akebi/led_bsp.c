#include "led_bsp.h"

uint8_t ucled[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void led_disp(uint8_t *ucled)
{
	uint8_t temp = 0x00;

	static uint8_t temp_old = 0xff;

	for (int i = 0; i < 8; i++)
	{

		temp |= (ucled[i] << (i));
	}
	if (temp != temp_old)
	{
		GPIOC->ODR &= 0x00ff;
		GPIOC->ODR |= ~(temp << 8);
		GPIOD->BSRR |= 0x01 << 2;
		GPIOD->BRR |= 0x01 << 2;
		temp_old = temp;
	}
}

void led_proc(void)
{
	led_disp(ucled);
	ucled[0] = (Lcd_display_mode == 0);
	ucled[7] = (adc_value[1] > ph);
}
