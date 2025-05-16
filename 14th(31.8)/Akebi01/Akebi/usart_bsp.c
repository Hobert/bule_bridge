#include "usart_bsp.h"

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


    // 清空读取缓冲区
  memset(usart_read_buffer, 0, sizeof(uint8_t) * BUUFER_SIZE);
}
