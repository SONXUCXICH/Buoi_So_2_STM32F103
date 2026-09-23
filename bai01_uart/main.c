#include "stm32f10x.h"

#define BUFFER_SIZE 100

char buffer[BUFFER_SIZE];
uint16_t buffer_index = 0;

/* =========================
   Gửi 1 ký tự qua USART1
   ========================= */
void USART1_SendChar(char c)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {
    }

    USART_SendData(USART1, c);
}

/* =========================
   Gửi chuỗi qua USART1
   ========================= */
void USART1_SendString(char *str)
{
    while (*str != '\0')
    {
        USART1_SendChar(*str);
        str++;
    }
}

/* =========================
   Cấu hình USART1
   PA9  -> TX
   PA10 -> RX
   ========================= */
void USART1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* Enable clock GPIOA */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* Enable clock USART1 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* =========================
       PA9 - USART1 TX
       ========================= */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* =========================
       PA10 - USART1 RX
       ========================= */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* =========================
       USART1 configuration
       ========================= */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;

    USART_InitStructure.USART_Mode =
        USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);

    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);
}

/* =========================
   MAIN
   ========================= */
int main(void)
{
    USART1_Config();

    /* Thông báo khi STM32 khởi động */
    USART1_SendString("STM32 UART READY\r\n");

    while (1)
    {
        /* Kiểm tra có dữ liệu nhận được không */
        if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
        {
            char c = USART_ReceiveData(USART1);

            /* Nếu chưa gặp ! thì lưu vào buffer */
            if (c != '!')
            {
                if (buffer_index < BUFFER_SIZE - 1)
                {
                    buffer[buffer_index] = c;
                    buffer_index++;
                }
            }
            else
            {
                /* Gửi mã lớp + mã nhóm */
                USART1_SendString("D23DTM01<11>: ");

                /* Gửi lại nội dung đã nhận */
                for (uint16_t i = 0; i < buffer_index; i++)
                {
                    USART1_SendChar(buffer[i]);
                }

                USART1_SendString("\r\n");

                /* Xóa buffer */
                buffer_index = 0;
            }
        }
    }
}