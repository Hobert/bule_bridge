#include "collect.h"
void r37_collect(void)
{
    if (r37 > ph)
    {
        vh += 1;
    }
}

typedef struct
{
    uint32_t collect_now_time;
    uint32_t collect_max;
    uint32_t collect_max_last;
    uint32_t collect_min;
    uint32_t collect_min_last;
    uint32_t collect_last_time;
} collect_voltage;

collect_voltage my_voltage;

void r38_collect_init(void)
{

    my_voltage.collect_now_time = HAL_GetTick();
    my_voltage.collect_last_time = HAL_GetTick();
    my_voltage.collect_max = 0;

    my_voltage.collect_max_last = 0;

    my_voltage.collect_min = r38;

    my_voltage.collect_min_last = 0;
}

void r38_collect(void)
{

    my_voltage.collect_now_time = HAL_GetTick(); // 反复更新当前时间

    if (r38 > my_voltage.collect_max) // 确定最大值
    {
        my_voltage.collect_max = r38;
    }
    if (r38 < my_voltage.collect_min) // 确定最小值
    {
        my_voltage.collect_min = r38;
    }

    if (my_voltage.collect_now_time - my_voltage.collect_last_time >= 3000) // 时间窗口设置为3秒
    {
        if (my_voltage.collect_max - my_voltage.collect_min > pd &&
            my_voltage.collect_max != my_voltage.collect_max_last &&
            my_voltage.collect_min != my_voltage.collect_min_last)
        {
            vd += 1;

            my_voltage.collect_max_last = my_voltage.collect_max;
            my_voltage.collect_min_last = my_voltage.collect_min;
        }
        my_voltage.collect_last_time = my_voltage.collect_now_time;
    }
}
