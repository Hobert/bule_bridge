#include "Komichi.h"

typedef struct
{

	void (*task_func)(void);
	uint32_t rate_ms;
	uint32_t last_run;

} scheduler_task_t;

uint8_t task_num;

static scheduler_task_t scheduler_task[] =
	{
		{led_proc, 10, 0},
		{key_state, 10, 0},
		{lcd_proc, 10, 0},
		{adc_proc, 10, 0},
		{r38_collect, 10, 0},
		{r37_collect, 10, 0},

};

void scheduler_init(void)
{
	task_num = sizeof(scheduler_task) / sizeof(scheduler_task_t);
}

void scheduler_run(void)
{
	for (int i = 0; i < task_num; i++)
	{
		uint32_t now_time = HAL_GetTick();
		if (now_time >= scheduler_task[i].rate_ms + scheduler_task[i].last_run)
		{
			scheduler_task[i].last_run = now_time;
			scheduler_task[i].task_func();
		}
	}
}
