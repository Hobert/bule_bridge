#include "filter.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief �����ݽ����޷�������������������ָ������Сֵ�����ֵ֮�䡣
 *
 * @param data ������������ָ�롣
 * @param size ����Ĵ�С��
 * @param min_val ��С����ֵ��
 * @param max_val �������ֵ��
 */
void limit_value(uint32_t *data, int size, uint32_t min_val, uint32_t max_val)
{
    for (int i = 0; i < size; i++)
    {
        if (data[i] < min_val)
        {
            data[i] = min_val;
        }
        else if (data[i] > max_val)
        {
            data[i] = max_val;
        }
    }
}

/**
 * @brief ����qsort�����ıȽϺ������Ƚ�����uint32_t���͵�ֵ��
 *
 * @param a ��һ���Ƚϵ�Ԫ�ء�
 * @param b �ڶ����Ƚϵ�Ԫ�ء�
 * @return int ����ֵС��0��ʾaС��b������ֵ����0��ʾa����b������ֵ����0��ʾa����b��
 */
int compare(const void *a, const void *b)
{
    return (*(uint32_t *)a - *(uint32_t *)b);
}

/**
 * @brief �������������ݵ���λ����
 *
 * @param data ������������ָ�롣
 * @param size ����Ĵ�С��
 * @return uint32_t ���ؼ��������λ����
 */
uint32_t mid_value(uint32_t *data, int size)
{
    uint32_t *copy = (uint32_t *)malloc(size * sizeof(uint32_t));
    memcpy(copy, data, size * sizeof(uint32_t));
    qsort(copy, size, sizeof(uint32_t), compare);
    uint32_t median = (size % 2 == 0) ? (copy[size / 2 - 1] + copy[size / 2]) / 2 : copy[size / 2];
    free(copy);
    return median;
}

/**
 * @brief �������������ݵ�����ƽ��ֵ��
 *
 * @param data ������������ָ�롣
 * @param size ����Ĵ�С��
 * @return uint32_t ���ؼ����������ƽ��ֵ��
 */
uint32_t avg_value(uint32_t *data, int size)
{
    uint32_t sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += data[i];
    }
    return sum / size;
}

/**
 * @brief һ���ͺ��˲���������ƽ���������ݡ�
 *
 * @param CurrValue ��ǰ����ֵ��
 * @return uint32_t �����˲����ֵ��
 */
uint32_t adc_filter(uint32_t CurrValue)
{
    static uint32_t LastValue = 0; // �˲�����һ�ε����ֵ����ʼΪ0
    uint32_t tmp = (32 * CurrValue + 96 * LastValue) >> 7;
    LastValue = tmp;
    return LastValue;
}
