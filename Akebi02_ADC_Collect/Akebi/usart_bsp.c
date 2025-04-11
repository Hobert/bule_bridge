#include "usart_bsp.h"
uint8_t Akebi;
#define BUUFER_SIZE 64
// 定义环形缓冲区和接收缓冲区
ringbuffer_t usart_rb;
uint8_t usart_read_buffer[BUUFER_SIZE];

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//  if (huart->Instance == USART1)
//  {
//    uart_rx_ticks = uwTick;
//    uart_rx_index++;
//    HAL_UART_Receive_IT(&huart1, &uart_rx_buffer[uart_rx_index], 1);
//  }
//
//}

void usart_analysis(void)
{
  uint8_t temp1[10];
  for (int i = 0; i < 3; i++)
  {
    temp1[i] = usart_read_buffer[i];
  }
  if (strcmp((const char *)temp1, "PH:") == 0)
  {
    int cout = 0;
    int ph_temper = 0;
    cout = strlen((const char *)usart_read_buffer) - 4;
    for (int i = cout; i >= 0; i--)
    {
      ph_temper += (usart_read_buffer[strlen((const char *)usart_read_buffer) - i - 1] - 48) * pow(10, i);
    }
    ph = ph_temper;
  }
  else if (strcmp((const char *)temp1, "PD:") == 0)
  {
    int cout = 0;
    int pd_temper = 0;
    cout = strlen((const char *)usart_read_buffer) - 4;
    for (int i = cout; i >= 0; i--)
    {
      pd_temper += (usart_read_buffer[strlen((const char *)usart_read_buffer) - i - 1] - 48) * pow(10, i);
    }
    pd = pd_temper;
  }

  uint8_t temp2[10];
  for (int i = 0; i < 2; i++)
  {
    temp2[i] = usart_read_buffer[i];
  }
  if (strcmp((const char *)temp2, "VH") == 0)
  {
    printf("VH:%.2f\r\n", vh);
  }
  else if (strcmp((const char *)temp2, "VD") == 0)
  {
    printf("VD:%.2f\r\n", vd);
  }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

  // if (huart->Instance == USART1)
  // {
  //   Akebi++;
  //   printf("Akebi:%d\r\n",Akebi);
  //   printf("dma data:%s\r\n", uart_rx_dma_buffer);
  //   memset(uart_rx_dma_buffer, 0, sizeof(uart_rx_dma_buffer));
  // }

  // 如果环形缓冲区未满
  if (!ringbuffer_is_full(&usart_rb))
  {
    // 将DMA缓冲区中的数据写入环形缓冲区
    ringbuffer_write(&usart_rb, uart_rx_dma_buffer, Size);
  }

  // 清空DMA缓冲区
  memset(uart_rx_dma_buffer, 0, sizeof(uart_rx_dma_buffer));
}

void usart_proc(void)
{
  // if (uwTick - uart_rx_ticks > 100)
  // {
  //   printf("uart_rx_buffer: %s\n", uart_rx_buffer);
  //   uart_rx_index = 0;
  //   memset(uart_rx_buffer, 0, uart_rx_index);
  //   huart1.pRxBuffPtr = uart_rx_buffer;
  // }
  // else if (uart_rx_index == 0)
  // {
  //   return;
  // }

  // 如果环形缓冲区为空，直接返回
  if (ringbuffer_is_empty(&usart_rb))
    return;

  // 从环形缓冲区读取数据到读取缓冲区
  ringbuffer_read(&usart_rb, usart_read_buffer, usart_rb.itemCount); // usart_rb.itemCount读取长度

  // 打印读取缓冲区中的数据
  printf("ringbuffer data: %s\n", usart_read_buffer);

  usart_analysis(); // 数据解析
  // 清空读取缓冲区
  memset(usart_read_buffer, 0, sizeof(uint8_t) * BUUFER_SIZE);
}
