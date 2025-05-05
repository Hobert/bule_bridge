#include "Komichi.h"

uint32_t now_time;

typedef struct
{

	void (*task_func)(void);
	uint32_t rate_ms;
	uint32_t last_run;

} scheduler_task_t;

uint8_t task_num;

// 频率数据更新频次：10 次/秒
// LCD 显示数据刷新时间 0.1 秒，显示效果清晰、稳定，无噪点
static scheduler_task_t scheduler_task[] =
	{
		{led_proc, 1, 0},
		{key_proc, 5, 0},
		{ic_proc, 100, 0},
		{usart_proc, 1000, 0},
		{adc_proc, 100, 0},
		
};

void scheduler_init(void)
{
	task_num = sizeof(scheduler_task) / sizeof(scheduler_task_t);
}

void scheduler_run(void)
{
	for (int i = 0; i < task_num; i++)
	{
		now_time = HAL_GetTick();
		if (now_time >= scheduler_task[i].rate_ms + scheduler_task[i].last_run)
		{
			scheduler_task[i].last_run = now_time;
			scheduler_task[i].task_func();
		}
	}
}
