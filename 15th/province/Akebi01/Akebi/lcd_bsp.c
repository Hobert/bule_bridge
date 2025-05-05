#include "lcd_bsp.h"

uint16_t pd = 1000; // 100Hz ~ 1000Hz
uint16_t ph = 5000; // 1000Hz ~ 10KHz
int16_t px = 0;     //-1000Hz ~ 1000Hz

// Ƶ��У׼
int16_t fa = 0;
int16_t fb = 0;

uint16_t vh, vd;
uint8_t nda, ndb, nha, nhb = 0;

uint8_t Lcd_display_mode = 0; // 切换数据、参数和记录三个界面flag

uint8_t Lcd_data_fulse_cycle_mode = 0; // 切换频率或周期显示模式flag

/**
 * @brief  ��ʽ���ַ�������ʾ��ָ����LCD���ϡ�
 *
 * �ú�������һ���кź�һ����ʽ���ַ�����������printf����
 * ��ʽ���ַ����󣬽�����ʾ��LCD��ָ�����ϡ�
 *
 * @param  Line    Ҫ��ʾ�ַ�����LCD�кš�
 * @param  format  ��ʽ���ַ���������?��ʽ���Ĳ�����
 *
 * �ú����ڲ�ʹ�� `vsprintf` ����ʽ���ַ�����Ȼ��
 * ���� `LCD_DisplayStringLine` ��LCD����ʾ��ʽ������ַ�����?
 *
 * ʾ���÷�:
 * @code
 * LcdSprintf(0, "Temperature: %d C", temperature);
 * @endcode
 */

void LcdSprintf(uint8_t Line, char *format, ...)
{
    char String[21];                           // ���������ڴ洢��ʽ������ַ���?
    va_list arg;                               // �����б����ڴ洢�ɱ����?
    va_start(arg, format);                     // ʹ�ø�ʽ���ַ�����ʼ�������б�
    vsprintf(String, format, arg);             // ��ʽ���ַ������洢�ڻ�������
    va_end(arg);                               // ���������б�
    LCD_DisplayStringLine(Line, (u8 *)String); // ��LCD��ָ������ʾ��ʽ������ַ���?
}

void limitation(void)
{
    if (pd < 100)
        pd = 100;
    if (pd > 1000)
        pd = 1000;
    if (ph < 1000)
        ph = 1000;
    if (ph > 10000)
        ph = 10000;
    if (px < -1000)
        px = -1000;
    if (px > 1000)
        px = 1000;
}

void lcd_proc(void)
{
    limitation();

    switch (Lcd_display_mode)
    {
    case 0: // 数据界面

        LcdSprintf(Line1, "        DATA");
        LcdSprintf(Line5, "           ");
        LcdSprintf(Line6, "           ");
        if (Lcd_data_fulse_cycle_mode == 0)
            // 频率显示模式
            if (fa > 1000 && fb < 1000)
            {

                LcdSprintf(Line3, "    A=%.2fKHz  ", fa / 1000.0f);
                LcdSprintf(Line4, "    B=%dHz   ", fb);
            }
            else if (fb > 1000 && fa < 1000)
            {
                LcdSprintf(Line3, "    A=%dHz     ", fa);
                LcdSprintf(Line4, "    B=%.2fKHz  ", fb / 1000.0f);
            }
            else if (fa > 1000 && fb > 1000)
            {
                LcdSprintf(Line3, "    A=%.2fKHz    ", fa / 1000.0f);
                LcdSprintf(Line4, "    B=%.2fKHz    ", fb / 1000.0f);
            }
            else if (fa < 0 && fb < 1000 && fb > 0)
            {
                LcdSprintf(Line3, "    A=NULL   ");
                LcdSprintf(Line4, "    B=%dHz   ", fb);
            }
            else if (fb < 0 && fa < 1000 && fa > 0)
            {
                LcdSprintf(Line3, "    A=%dHz   ", fa);
                LcdSprintf(Line4, "    B=NULL   ");
            }
            else if (fb < 0 && fa < 0)
            {
                LcdSprintf(Line3, "    A=NULL   ");
                LcdSprintf(Line4, "    B=NULL   ");
            }
            else if (fa > 0 && fb > 0 && fa < 1000 && fb < 1000)
            {
                LcdSprintf(Line3, "    A=%dHz   ", fa);
                LcdSprintf(Line4, "    B=%dHz   ", fb);
            }

            // 周期显示模式
            else if (Lcd_data_fulse_cycle_mode == 1)
            {
                if (fa > 1000 && fb < 1000)
                {

                    LcdSprintf(Line3, "    A=%.2fmS ", 1000.0f / fa);
                    LcdSprintf(Line4, "    B=%duS   ", 1000000 / fb);
                }
                else if (fb > 1000 && fa < 1000)
                {
                    LcdSprintf(Line3, "    A=%duS   ", 1000000 / fa);
                    LcdSprintf(Line4, "    B=%.2fmS ", 1000.0f / fb);
                }
                else if (fa > 1000 && fb > 1000)
                {
                    LcdSprintf(Line3, "    A=%.2fmS   ", 1000.0f / fa);
                    LcdSprintf(Line4, "    B=%.2fmS   ", 1000.0f / fb);
                }
                else if (fa < 0 && fb < 1000 && fb > 0)
                {
                    LcdSprintf(Line3, "    A=NULL   ");
                    LcdSprintf(Line4, "    B=%duS   ", 1000000 / fb);
                }
                else if (fb < 0 && fa < 1000 && fa > 0)
                {
                    LcdSprintf(Line3, "    A=%duS   ", 1000000 / fa);
                    LcdSprintf(Line4, "    B=NULL   ");
                }
                else if (fb < 0 && fa < 0)
                {
                    LcdSprintf(Line3, "    A=NULL   ");
                    LcdSprintf(Line4, "    B=NULL   ");
                }
                else if (fa > 0 && fb > 0 && fa < 1000 && fb < 1000)
                {
                    LcdSprintf(Line3, "    A=%duS   ", 1000000 / fa);
                    LcdSprintf(Line4, "    B=%duS   ", 1000000 / fb);
                }
            }

        break;

    case 1: // 参数界面
        LcdSprintf(Line1, "        PARA");
        LcdSprintf(Line3, "    PD=%dHz    ", pd); // 突变参数
        LcdSprintf(Line4, "    PH=%dHz    ", ph); // 超限参数
        LcdSprintf(Line5, "    PX=%dHz    ", px); // 校准值参�?
        LcdSprintf(Line6, "           ");
        break;

    case 2: // 统�?�界�?
        LcdSprintf(Line1, "        RECD");
        LcdSprintf(Line3, "    NDA=%d     ", nda); // A 通道频率突变次数
        LcdSprintf(Line4, "    NDB=%d     ", ndb); // B 通道频率突变次数
        LcdSprintf(Line5, "    NHA=%d     ", nha); // A 通道频率超限次数
        LcdSprintf(Line6, "    NHB=%d     ", nhb); // B 通道频率超限次数

        break;
    }
}
