#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "misc.h"

/* =========================================================
   PWM:
   PA0 -> TIM2_CH1 -> LED

   Các mức PWM:
   10%, 30%, 50%, 70%

   UART:
   PA9  -> TX
   PA10 -> RX
   ========================================================= */

#define PWM_10   10
#define PWM_30   30
#define PWM_50   50
#define PWM_70   70


/* =========================================================
   BIẾN TRẠNG THÁI
   ========================================================= */

volatile uint8_t led_state = 0;
volatile uint8_t pwm_percent = PWM_10;

volatile char rx_buffer[32];
volatile uint8_t rx_index = 0;
volatile uint8_t command_ready = 0;


/* =========================================================
   UART GỬI KÝ TỰ
   ========================================================= */

void USART1_SendChar(char c)
{
    while (USART_GetFlagStatus(
               USART1,
               USART_FLAG_TXE) == RESET)
    {
    }

    USART_SendData(USART1, c);
}


/* =========================================================
   UART GỬI CHUỖI
   ========================================================= */

void USART1_SendString(char *str)
{
    while (*str != '\0')
    {
        USART1_SendChar(*str);
        str++;
    }
}


/* =========================================================
   UART GỬI SỐ
   ========================================================= */

void USART1_SendNumber(uint32_t number)
{
    char buffer[10];
    uint8_t i = 0;

    if (number == 0)
    {
        USART1_SendChar('0');
        return;
    }

    while (number > 0)
    {
        buffer[i] = '0' + (number % 10);
        number = number / 10;
        i++;
    }

    while (i > 0)
    {
        i--;
        USART1_SendChar(buffer[i]);
    }
}


/* =========================================================
   CẤU HÌNH PWM
   PA0 = TIM2_CH1
   ========================================================= */

void PWM_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;


    /* Clock GPIOA */
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA,
        ENABLE
    );

    /* Clock TIM2 */
    RCC_APB1PeriphClockCmd(
        RCC_APB1Periph_TIM2,
        ENABLE
    );


    /* PA0 = Alternate Function Push Pull */

    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_0;

    GPIO_InitStructure.GPIO_Mode =
        GPIO_Mode_AF_PP;

    GPIO_InitStructure.GPIO_Speed =
        GPIO_Speed_50MHz;

    GPIO_Init(
        GPIOA,
        &GPIO_InitStructure
    );


    /*
       TIM2 clock = 72 MHz

       PSC = 71
       72 MHz / 72 = 1 MHz

       ARR = 999
       1 MHz / 1000 = 1 kHz
    */

    TIM_TimeBaseStructure.TIM_Prescaler = 71;

    TIM_TimeBaseStructure.TIM_CounterMode =
        TIM_CounterMode_Up;

    TIM_TimeBaseStructure.TIM_Period = 999;

    TIM_TimeBaseStructure.TIM_ClockDivision =
        TIM_CKD_DIV1;

    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(
        TIM2,
        &TIM_TimeBaseStructure
    );


    /* PWM Channel 1 */

    TIM_OCStructInit(
        &TIM_OCInitStructure
    );

    TIM_OCInitStructure.TIM_OCMode =
        TIM_OCMode_PWM1;

    TIM_OCInitStructure.TIM_OutputState =
        TIM_OutputState_Enable;

    TIM_OCInitStructure.TIM_OCPolarity =
        TIM_OCPolarity_High;

    /* Ban đầu LED OFF */
    TIM_OCInitStructure.TIM_Pulse = 0;

    TIM_OC1Init(
        TIM2,
        &TIM_OCInitStructure
    );

    TIM_OC1PreloadConfig(
        TIM2,
        TIM_OCPreload_Enable
    );


    TIM_ARRPreloadConfig(
        TIM2,
        ENABLE
    );


    /* Bật Timer */

    TIM_Cmd(
        TIM2,
        ENABLE
    );
}


/* =========================================================
   ĐẶT DUTY PWM
   ========================================================= */

void PWM_SetPercent(uint8_t percent)
{
    uint16_t pulse;

    /*
       ARR = 999

       10% -> 100
       30% -> 300
       50% -> 500
       70% -> 700
    */

    pulse =
        ((uint32_t)percent * 1000) / 100;

    TIM_SetCompare1(
        TIM2,
        pulse
    );
}


/* =========================================================
   TÌM MỨC PWM GẦN NHẤT

   10 / 30 / 50 / 70
   ========================================================= */

uint8_t FindNearestPWM(uint8_t value)
{
    uint8_t nearest;
    uint8_t diff;
    uint8_t temp;


    /* Mặc định 10% */

    nearest = PWM_10;

    if (value >= PWM_10)
        diff = value - PWM_10;
    else
        diff = PWM_10 - value;


    /* 30% */

    if (value >= PWM_30)
        temp = value - PWM_30;
    else
        temp = PWM_30 - value;

    if (temp < diff)
    {
        diff = temp;
        nearest = PWM_30;
    }


    /* 50% */

    if (value >= PWM_50)
        temp = value - PWM_50;
    else
        temp = PWM_50 - value;

    if (temp < diff)
    {
        diff = temp;
        nearest = PWM_50;
    }


    /* 70% */

    if (value >= PWM_70)
        temp = value - PWM_70;
    else
        temp = PWM_70 - value;

    if (temp < diff)
    {
        nearest = PWM_70;
    }


    return nearest;
}


/* =========================================================
   LỆNH ON
   ========================================================= */

void Command_ON(void)
{
    led_state = 1;

    /* LED sáng theo PWM đã cấu hình */

    PWM_SetPercent(
        pwm_percent
    );

    USART1_SendString(
        "LED ON\r\n"
    );

    USART1_SendString(
        "PWM = "
    );

    USART1_SendNumber(
        pwm_percent
    );

    USART1_SendString(
        "%\r\n"
    );
}


/* =========================================================
   LỆNH OFF
   ========================================================= */

void Command_OFF(void)
{
    led_state = 0;

    /*
       Tắt LED.
       Không xóa pwm_percent.
    */

    TIM_SetCompare1(
        TIM2,
        0
    );

    USART1_SendString(
        "LED OFF\r\n"
    );
}


/* =========================================================
   LỆNH PWM:Percent%
   ========================================================= */

void Command_PWM(void)
{
    uint32_t value = 0;
    uint8_t i = 4;


    /*
       Ví dụ:

       PWM:28%

       [0] P
       [1] W
       [2] M
       [3] :
       [4] 2
       [5] 8
       [6] %
    */


    while (
        rx_buffer[i] >= '0' &&
        rx_buffer[i] <= '9'
    )
    {
        value =
            value * 10 +
            (rx_buffer[i] - '0');

        i++;
    }


    /* Không cho lớn hơn 100% */

    if (value > 100)
    {
        USART1_SendString(
            "PWM ERROR\r\n"
        );

        return;
    }


    /*
       Chọn mức gần nhất:
       10 / 30 / 50 / 70
    */

    pwm_percent =
        FindNearestPWM(
            (uint8_t)value
        );


    /*
       Nếu ON:
       LED thay đổi ngay.

       Nếu OFF:
       chỉ lưu cấu hình.
    */

    if (led_state == 1)
    {
        PWM_SetPercent(
            pwm_percent
        );
    }


    USART1_SendString(
        "PWM CONFIG = "
    );

    USART1_SendNumber(
        pwm_percent
    );

    USART1_SendString(
        "%\r\n"
    );
}


/* =========================================================
   LỆNH STATUS
   ========================================================= */

void Command_Status(void)
{
    USART1_SendString(
        "Status: "
    );


    if (led_state == 1)
    {
        USART1_SendString(
            "ON"
        );
    }
    else
    {
        USART1_SendString(
            "OFF"
        );
    }


    USART1_SendString(
        "\r\nPWM: "
    );

    USART1_SendNumber(
        pwm_percent
    );

    USART1_SendString(
        "%\r\n"
    );
}


/* =========================================================
   XỬ LÝ LỆNH
   ========================================================= */

void Process_Command(void)
{
    /* ON */

    if (
        rx_buffer[0] == 'O' &&
        rx_buffer[1] == 'N'
    )
    {
        Command_ON();
    }


    /* OFF */

    else if (
        rx_buffer[0] == 'O' &&
        rx_buffer[1] == 'F' &&
        rx_buffer[2] == 'F'
    )
    {
        Command_OFF();
    }


    /* PWM */

    else if (
        rx_buffer[0] == 'P' &&
        rx_buffer[1] == 'W' &&
        rx_buffer[2] == 'M'
    )
    {
        Command_PWM();
    }


    /* Status */

    else if (
        rx_buffer[0] == 'S' &&
        rx_buffer[1] == 't' &&
        rx_buffer[2] == 'a'
    )
    {
        Command_Status();
    }


    /* Không nhận dạng */

    else
    {
        USART1_SendString(
            "Unknown command\r\n"
        );
    }
}


/* =========================================================
   USART1 INTERRUPT
   ========================================================= */

void USART1_IRQHandler(void)
{
    char c;


    if (
        USART_GetITStatus(
            USART1,
            USART_IT_RXNE
        ) != RESET
    )
    {
        c = USART_ReceiveData(
            USART1
        );


        /* Dấu ! kết thúc lệnh */

        if (c == '!')
        {
            rx_buffer[rx_index] = '\0';

            command_ready = 1;

            rx_index = 0;
        }

        else
        {
            /* Chống tràn bộ đệm */

            if (rx_index < 30)
            {
                rx_buffer[rx_index] = c;

                rx_index++;
            }
            else
            {
                rx_index = 0;
            }
        }
    }
}


/* =========================================================
   CẤU HÌNH USART1
   PA9  = TX
   PA10 = RX
   115200 8N1
   ========================================================= */

void USART1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


    /* Clock GPIOA + USART1 + AFIO */

    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA |
        RCC_APB2Periph_USART1 |
        RCC_APB2Periph_AFIO,
        ENABLE
    );


    /* PA9 = TX */

    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_9;

    GPIO_InitStructure.GPIO_Mode =
        GPIO_Mode_AF_PP;

    GPIO_InitStructure.GPIO_Speed =
        GPIO_Speed_50MHz;

    GPIO_Init(
        GPIOA,
        &GPIO_InitStructure
    );


    /* PA10 = RX */

    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_10;

    GPIO_InitStructure.GPIO_Mode =
        GPIO_Mode_IN_FLOATING;

    GPIO_Init(
        GPIOA,
        &GPIO_InitStructure
    );


    /* USART 115200 8N1 */

    USART_InitStructure.USART_BaudRate =
        115200;

    USART_InitStructure.USART_WordLength =
        USART_WordLength_8b;

    USART_InitStructure.USART_StopBits =
        USART_StopBits_1;

    USART_InitStructure.USART_Parity =
        USART_Parity_No;

    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;

    USART_InitStructure.USART_Mode =
        USART_Mode_Tx |
        USART_Mode_Rx;

    USART_Init(
        USART1,
        &USART_InitStructure
    );


    /* Bật ngắt RX */

    USART_ITConfig(
        USART1,
        USART_IT_RXNE,
        ENABLE
    );


    /* NVIC USART1 */

    NVIC_InitStructure.NVIC_IRQChannel =
        USART1_IRQn;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =
        0;

    NVIC_InitStructure.NVIC_IRQChannelSubPriority =
        0;

    NVIC_InitStructure.NVIC_IRQChannelCmd =
        ENABLE;

    NVIC_Init(
        &NVIC_InitStructure
    );


    /* Bật USART1 */

    USART_Cmd(
        USART1,
        ENABLE
    );
}


/* =========================================================
   MAIN
   ========================================================= */

int main(void)
{
    /* PWM */
    PWM_Config();

    /* UART */
    USART1_Config();


    /* Trạng thái ban đầu */

    led_state = 0;

    /* PWM mặc định = 10% */

    pwm_percent = PWM_10;


    /* LED ban đầu tắt */

    TIM_SetCompare1(
        TIM2,
        0
    );


    /* Thông báo UART */

    USART1_SendString(
        "\r\n"
    );

    USART1_SendString(
        "PWM UART READY\r\n"
    );

    USART1_SendString(
        "PWM levels: 10 30 50 70%\r\n"
    );

    USART1_SendString(
        "Commands: ON! OFF! PWM:Percent%! Status!\r\n"
    );


    /* Vòng lặp chính */

    while (1)
    {
        if (command_ready == 1)
        {
            command_ready = 0;

            Process_Command();
        }
    }
}