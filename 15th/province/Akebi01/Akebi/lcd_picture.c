/*******************************************************************************
* Function Name  : LCD_ShowVerticalPicture
* Description    : 显示一个RGB565图片
* Input          : - picture: 图片数组，坐标 (P_y<320)
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_ShowVerticalPicture(const u8* picture,uint16_t P_x,uint16_t P_y)
{
    int index,flack=0;
    LCD_SetCursor(0x00, 0x0000); 
 
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
 
    for(index = 0; index < (P_x*P_y*2);index+=2)//76800; index++)
    {
            if(index%((P_y)*2)==0&&flack==1)
            {
                int i = 0;
                for(i=0;i<(320-P_y);i++)
                    LCD_WriteRAM((uint16_t)0xffff);
            }
            LCD_WriteRAM((picture[index+1]<<8)|picture[index]);
        flack = 1;
    }
}
/*******************************************************************************
* Function Name  : LCD_ShowVerticalPicture
* Description    : 显示一个RGB565图片
* Input          : - picture: 图片数组，坐标 (P_y<320)
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_ShowcrosswisePicture(const u8* picture,uint16_t P_x,uint16_t P_y)
{
    int index,flack=0;
    LCD_SetCursor(0, 265); 
 
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
 
    for(index = 0; index < (P_x*P_y*2);index+=2)//76800; index++)
    {
        if(index%((P_x)*2)==0&&flack==1)
        {
            int i = 0;
            for(i=0;i<(320-P_x);i++)
                LCD_WriteRAM((uint16_t)0xffff);
        }
        LCD_WriteRAM((picture[index+1]<<8)|picture[index]);
        flack = 1;
    }
}
