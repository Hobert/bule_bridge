#include "led_bsp.h"

uint8_t ucled[8] = {0, 1, 0, 1, 0, 1, 0, 1};

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

void led_LD1(void) // LD1:处于数据界面，指示灯LD1点亮，否则熄灭。
{
	if (memu_choose == 0)
	{
		ucled[0] = 0;
	}
	else if (memu_choose == 1 || memu_choose == 2)
	{
		ucled[0] = 1;
	}
}

void led_LD2(void)
{
	if (r37 > ph)
	{
		ucled[1] = 0;
		ucled[2] = 1;
		ucled[3] = 1;
		ucled[4] = 1;
		ucled[5] = 1;
		ucled[6] = 1;
		ucled[7] = 1;
	}
	else if (r37 < ph)
	{
		ucled[1] = 1;
	}
}

void led_LD3(void)
{
	if (r38 > pd)
	{

		ucled[2] = 0;
		ucled[3] = 1;
		ucled[4] = 1;
		ucled[5] = 1;
		ucled[6] = 1;
		ucled[7] = 1;
	}
	else if (r38 < pd)
	{
		ucled[2] = 1;
	}
}

void led_proc(void)
{
	led_LD1();
	led_LD2();
	led_LD3();
	led_disp(ucled);
}
