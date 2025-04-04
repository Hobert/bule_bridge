#include "lcd_bsp.h"

uint16_t pd = 1000;
uint16_t ph = 2000;
uint16_t vh, vd;

uint8_t Lcd_display_mode = 0;

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
        LcdSprintf(Line3, "    R37=%d     ", (int)adc_value[1]);
        LcdSprintf(Line4, "    R38=%d     ", (int)adc_value[0]);
        break;
    case 1:
        LcdSprintf(Line1, "        PARA");
        LcdSprintf(Line3, "    PD=%d     ", pd);
        LcdSprintf(Line4, "    PH=%d     ", ph);

        break;
    case 2:
        LcdSprintf(Line1, "        RECD");
        LcdSprintf(Line3, "    VH=%d     ", vh);
        LcdSprintf(Line4, "    VD=%d     ", vd);
        LcdSprintf(Line5, "    diff=%d     ", diff);
        break;
    }
}
