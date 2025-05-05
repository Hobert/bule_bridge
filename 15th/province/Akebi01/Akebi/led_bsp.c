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
	ucled[0] = (Lcd_display_mode == 0); // 处于数据界面，指示灯 LD1 点亮，否则熄灭
	ucled[1] = (fa > ph);				// A 通道频率 fA ＞ PH 时，指示灯 LD2 点亮，否则熄灭
	ucled[2] = (fb > ph);				// B 通道频率 fB ＞ PH 时，指示灯 LD3 点亮，否则熄灭
	if (nda >= 3 || ndb >= 3)
	{
		ucled[7] = 1; // A 或 B 通道连续 3 次检测到频率 fA 或 fB ＞ PH 时，指示灯 LD4 点亮，否则熄灭
	}
}
