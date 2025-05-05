#include "tim_app.h"

#define ringbuffer_fluse_buff_size 30
#define window_time_size 3000

uint8_t head_point_a = 0;
uint8_t end_point_a = 0;

uint8_t head_point_b = 0;
uint8_t end_point_b = 0;

uint8_t flag_pd_a = 0;
uint8_t flag_pd_b = 0;

uint16_t diff = 0;

uint8_t flag_ph_fa = 0;
uint8_t flag_ph_fb = 0;

typedef struct ringbuffer_fluse
{
    uint32_t timestamp;
    int16_t fluse;
} ringbuffer_fluse;

ringbuffer_fluse a_fluse_buff[ringbuffer_fluse_buff_size];
ringbuffer_fluse b_fluse_buff[ringbuffer_fluse_buff_size];

void ringbuffer_fluse_init(ringbuffer_fluse *buff, uint32_t nowtime, int16_t fluse)
{
    if (buff == a_fluse_buff)
    {
        buff[end_point_a].timestamp = nowtime;
        buff[end_point_a].fluse = fluse;
        end_point_a = (end_point_a + 1) % ringbuffer_fluse_buff_size;

        if (end_point_a == head_point_a) // 缓冲区已满
        {
            head_point_a = (head_point_a + 1) % ringbuffer_fluse_buff_size;
        }
        while (nowtime - buff[head_point_a].timestamp >= window_time_size) // 判断时间窗口是否超过
        {
            head_point_a = (head_point_a + 1) % ringbuffer_fluse_buff_size; // 删除最早的数据
        }
    }
    else if (buff == b_fluse_buff)
    {
        buff[end_point_b].timestamp = nowtime;
        buff[end_point_b].fluse = fluse;
        end_point_b = (end_point_b + 1) % ringbuffer_fluse_buff_size;

        if (end_point_b == head_point_b) // 缓冲区已满
        {
            head_point_b = (head_point_b + 1) % ringbuffer_fluse_buff_size;
        }
        while (nowtime - buff[head_point_b].timestamp >= window_time_size) // 判断时间窗口是否超过
        {
            head_point_b = (head_point_b + 1) % ringbuffer_fluse_buff_size; // 删除最早的数据
        }
    }
}
void cheak_fluse_change(ringbuffer_fluse *buff, uint8_t size)
{
    if (buff == a_fluse_buff)
    {
        int16_t fluse_max = buff[head_point_a].fluse;
        int16_t fluse_min = buff[head_point_a].fluse;
        int index = head_point_a;
        while (index != end_point_a) //
        {
            if (buff[index].fluse > fluse_max) // 判断最大值
            {
                fluse_max = buff[index].fluse;
            }
            if (buff[index].fluse < fluse_min) // 判断最小值
            {
                fluse_min = buff[index].fluse;
            }
            index = (index + 1) % size;
        }

        diff = fluse_max - fluse_min;
        if (diff < pd)
        {
            flag_pd_a = 1;
            ucled[1] = 0;
        }
        else if (flag_pd_a == 1)
        {
            flag_pd_a = 0;
            nda++;
        }
    }
    else if (buff == b_fluse_buff)
    {
        int16_t fluse_max = buff[head_point_b].fluse;
        int16_t fluse_min = buff[head_point_b].fluse;
        int index = head_point_b;
        while (index != end_point_b) //
        {
            if (buff[index].fluse > fluse_max) // 判断最大值
            {
                fluse_max = buff[index].fluse;
            }
            if (buff[index].fluse < fluse_min) // 判断最小值
            {
                fluse_min = buff[index].fluse;
            }
            index = (index + 1) % size;
        }

        diff = fluse_max - fluse_min;
        if (diff < pd)
        {
            flag_pd_b = 1;
            ucled[1] = 0;
        }
        else if (flag_pd_b == 1)
        {
            flag_pd_b = 0;
            ndb++;
        }
    }
    if (diff > pd)
    {
        ucled[1] = 1;
    }
    // LcdSprintf(Line7, "    diff=%d     ", diff);
    // LcdSprintf(Line8, "    fluse_max=%d     ", fluse_max);
    // LcdSprintf(Line0, "    fluse_min=%d     ", fluse_min);
}

/**
 * @brief ����PWMռ�ձ�
 *
 * �˺�����������TIM2��CH1ͨ����PWMռ�ձȡ�ռ�ձȵķ�ΧΪ0%��100%��
 *
 * @param Duty ռ�ձȣ���ΧΪ0.0��100.0��
 */
void pwm_set_duty(float Duty)
{
    // ����ռ�ձȼ��㲶��/�ȽϼĴ�����ֵ
    TIM2->CCR2 = (TIM2->ARR + 1) * (Duty / 100.0f);
}

/**
 * @brief ����PWMƵ��
 *
 * �˺�����������TIM2��PWMƵ�ʡ�Ƶ�ʵĵ�λΪHz��
 *
 * @param Frequency Ƶ�ʣ���λΪHz��
 */
void pwm_set_frequency(int Frequency)
{
    // ��ȡ��ʱ����ʱ��Ƶ�ʣ�����TIM2ʹ�õ�ʱ��Ƶ��ΪTIM2_CLK��
    uint32_t TIM2_CLK = 72000000; // ����72MHz, ��Ҫ����ʵ���������?

    // ���������Ƶ�ʼ����Զ���װ�ؼĴ������?
    uint32_t ARR_Value = (TIM2_CLK / Frequency) - 1;

    // �����Զ���װ�ؼĴ���
    TIM2->ARR = ARR_Value;

    // ���²���/�ȽϼĴ���CCR2�����ֵ�ǰռ�ձȲ���
    TIM2->CCR2 = (ARR_Value + 1) * (TIM2->CCR2 / (float)(TIM2->ARR + 1));

    // ���������¼���ˢ�¼Ĵ���
    TIM2->EGR = TIM_EGR_UG;
}

uint32_t tim_ic_buffer1[64]; // �����?���벶��ֵ�Ļ�����
uint32_t tim_ic_buffer2[64]; // �����?���벶��ֵ�Ļ�����

uint32_t tim_ic_val1 = 0; // ���ռ���õ������벶���?
uint32_t tim_ic_val2 = 0; // ���ռ���õ������벶���?

uint32_t tim_ic_temp1 = 0; // ��ʱ�洢���벶�������м�ֵ
uint32_t tim_ic_temp2 = 0; // ��ʱ�洢���벶�������м�ֵ

/**
 * @brief �������벶������
 *
 * �˺������ڴ������벶�����ݡ�������tim_ic_buffer�����в���ֵ��ƽ��ֵ��
 * Ȼ����ת��Ϊ��Ӧ��Ƶ��ֵ����λΪHz�������洢��tim_ic_val�С�
 * ���?����limit_value������tim_ic_val�������ƣ�ȷ������1��20000֮�䡣
 */

// 频率测量范围：400Hz – 20KHz。
void ic_proc(void)
{
    tim_ic_temp1 = 0;
    tim_ic_temp2 = 0;

    // �Ի������еĲ���ֵ���?
    for (int i = 0; i < 64; i++)
    {
        tim_ic_temp1 += tim_ic_buffer1[i];
        tim_ic_temp2 += tim_ic_buffer2[i];
    }

    // ����ƽ��ֵ
    tim_ic_temp1 /= 64;
    tim_ic_temp2 /= 64;

    // ����Ƶ��ֵ����λΪHz
    tim_ic_val1 = (int)((float)(1000.0f * 1000.0f) / (float)tim_ic_temp1);
    tim_ic_val2 = (int)((float)(1000.0f * 1000.0f) / (float)tim_ic_temp2);

    // ������õ���Ƶ��ֵ������?1��20000֮��
    limit_value(&tim_ic_val1, 1, 400, 20000);
    limit_value(&tim_ic_val2, 1, 400, 20000);

    ringbuffer_fluse_init(a_fluse_buff, HAL_GetTick(), tim_ic_val1 + px);
    ringbuffer_fluse_init(b_fluse_buff, HAL_GetTick(), tim_ic_val2 + px);
    cheak_fluse_change(a_fluse_buff, ringbuffer_fluse_buff_size);
    cheak_fluse_change(b_fluse_buff, ringbuffer_fluse_buff_size);

    fa = (int16_t)tim_ic_val1 + px;
    fb = (int16_t)tim_ic_val2 + px;
    // Ƶ�ʳ���
    if (fa > ph && flag_ph_fa == 0)
    {
        nha++;
        flag_ph_fa = 1;
    }
    else if (fa < ph && flag_ph_fa == 1)
    {
        flag_ph_fa = 0;
    }
    if (fb > ph && flag_ph_fb == 0)
    {
        nhb++;
        flag_ph_fb = 1;
    }
    else if (fb < ph && flag_ph_fb == 1)
    {
        flag_ph_fb = 0;
    }
}
