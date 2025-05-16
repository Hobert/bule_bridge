#include "lcd_bsp.h"

uint8_t n = 0;

uint8_t set_value[2] = {1, 1}; // r = 1,k = 1
uint8_t set_dis_value[2] = {1, 1};

float speed_value[2];

float v, reco_velocity = 0.0;

uint8_t Lcd_display_mode = 0;

uint8_t data_fulse_flag = 0;

uint8_t value_r_k_flag = 0;

uint32_t reco_velocity_time = 0;

float pwm_duty = 0.00;

/**
 * @brief  ��ʽ���ַ�������ʾ��ָ����LCD���ϡ�
 *
 * �ú�������һ���кź�һ����ʽ���ַ�����������printf����
 * ��ʽ���ַ����󣬽�����ʾ��LCD��ָ�����ϡ�
 *
 * @param  Line    Ҫ��ʾ�ַ�����LCD�кš�
 * @param  format  ��ʽ���ַ��������Ҫ��ʽ���Ĳ�����
 *
 * �ú����ڲ�ʹ�� `vsprintf` ����ʽ���ַ�����Ȼ��
 * ���� `LCD_DisplayStringLine` ��LCD����ʾ��ʽ������ַ�����
 *
 * ʾ���÷�:
 * @code
 * LcdSprintf(0, "Temperature: %d C", temperature);
 * @endcode
 */

void LcdSprintf(uint8_t Line, char *format, ...)
{
    char String[21];                           // ���������ڴ洢��ʽ������ַ���
    va_list arg;                               // �����б����ڴ洢�ɱ����
    va_start(arg, format);                     // ʹ�ø�ʽ���ַ�����ʼ�������б�
    vsprintf(String, format, arg);             // ��ʽ���ַ������洢�ڻ�������
    va_end(arg);                               // ���������б�
    LCD_DisplayStringLine(Line, (u8 *)String); // ��LCD��ָ������ʾ��ʽ������ַ���
}

void lcd_proc(void)
{

    switch (Lcd_display_mode)
    {
    case 0:
        LcdSprintf(Line1, "        DATA");

        LcdSprintf(Line3, "    M=%s   ", data_fulse_flag == 0 ? "L" : "H");

        LcdSprintf(Line4, "    P=%.1f%%\n  ", pwm_duty);

        LcdSprintf(Line5, "    V=%.1f   ", v);

        LcdSprintf(Line6, "    frequency=%d   ", pwm_frequency);

        break;
    case 1:
        LcdSprintf(Line1, "        PARA");
        LcdSprintf(Line3, "    R=%d     ", set_dis_value[0]);
        LcdSprintf(Line4, "    K=%d     ", set_dis_value[1]);
        LcdSprintf(Line5, "             ");

        break;
    case 2:
        LcdSprintf(Line1, "        RECD");
        LcdSprintf(Line3, "    N=%d     ", n);
        LcdSprintf(Line4, "    MH=%.1f     ", speed_value[1]);
        LcdSprintf(Line5, "    ML=%.1f     ", speed_value[0]);
        break;
    }
}
