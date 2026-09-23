#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

/*
Bai 2:
Dung ngat SysTick dieu khien 3 LED doc lap.

PC13: 0.1 Hz
PB0 : 1 Hz
PB1 : 10 Hz

SysTick tao ngat moi 1 ms.
*/

volatile uint32_t count_01Hz = 0;
volatile uint32_t count_1Hz = 0;
volatile uint32_t count_10Hz = 0;


/*
Cau hinh GPIO cho 3 LED.
*/
void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /*
    Cap clock cho GPIOB va GPIOC.
    */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC,
                           ENABLE);

    /*
    PC13 dung cho LED 0.1 Hz.
    */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*
    PB0 dung cho LED 1 Hz.
    PB1 dung cho LED 10 Hz.
    */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*
    Trang thai ban dau.
    */
    GPIO_SetBits(GPIOC, GPIO_Pin_13);

    GPIO_ResetBits(GPIOB, GPIO_Pin_0);
    GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}


/*
Ham ngat SysTick.
Ham nay duoc goi moi 1 ms.
*/
void SysTick_Handler(void)
{
    count_01Hz++;
    count_1Hz++;
    count_10Hz++;

    /*
    0.1 Hz:
    Chu ky = 10 giay.
    Dao LED moi 5 giay = 5000 ms.
    */
    if (count_01Hz >= 5000)
    {
        GPIOC->ODR ^= GPIO_Pin_13;
        count_01Hz = 0;
    }

    /*
    1 Hz:
    Chu ky = 1 giay.
    Dao LED moi 500 ms.
    */
    if (count_1Hz >= 500)
    {
        GPIOB->ODR ^= GPIO_Pin_0;
        count_1Hz = 0;
    }

    /*
    10 Hz:
    Chu ky = 100 ms.
    Dao LED moi 50 ms.
    */
    if (count_10Hz >= 50)
    {
        GPIOB->ODR ^= GPIO_Pin_1;
        count_10Hz = 0;
    }
}


int main(void)
{
    /*
    Cap nhat gia tri SystemCoreClock.
    */
    SystemCoreClockUpdate();

    /*
    Cau hinh GPIO.
    */
    GPIO_Config();

    /*
    SysTick tao ngat moi 1 ms.
    */
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        while (1);
    }

    /*
    Khong can xu ly LED trong while.
    LED duoc dieu khien trong SysTick_Handler.
    */
    while (1)
    {
    }
}
