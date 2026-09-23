#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

/*
Bai 4:

Dung TIM2 tao PWM tan so 1 kHz.

PA0 - TIM2 CH1 - Duty 10%
PA1 - TIM2 CH2 - Duty 30%
PA2 - TIM2 CH3 - Duty 50%
PA3 - TIM2 CH4 - Duty 70%
*/


void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /*
    Cap clock cho GPIOA.
    */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /*
    PA0, PA1, PA2, PA3 la cac chan PWM cua TIM2.

    GPIO_Mode_AF_PP:
    Alternate Function Push Pull.
    Timer se dieu khien truc tiep cac chan nay.
    */
    GPIO_InitStruct.GPIO_Pin =
        GPIO_Pin_0 |
        GPIO_Pin_1 |
        GPIO_Pin_2 |
        GPIO_Pin_3;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStruct);
}


void TIM2_PWM_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_OCInitTypeDef TIM_OCStruct;

    /*
    Cap clock cho TIM2.
    */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


    /*
    TIM2 clock = 72 MHz.

    Prescaler = 72 - 1

    72 MHz / 72 = 1 MHz

    Timer dem voi tan so 1 MHz.
    Moi lan dem = 1 us.
    */
    TIM_TimeBaseStruct.TIM_Prescaler = 71;

    /*
    Period = 1000 - 1

    1000 lan dem x 1 us = 1000 us
                          = 1 ms

    1 / 1 ms = 1 kHz.
    */
    TIM_TimeBaseStruct.TIM_Period = 999;

    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);


    /*
    Cau hinh che do PWM.
    */
    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;

    /*
    Output enable.
    */
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;

    /*
    Xung PWM muc HIGH trong khoang duty.
    */
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;


    /*
    CH1 - PA0

    Period = 1000

    10% x 1000 = 100
    */
    TIM_OCStruct.TIM_Pulse = 100;

    TIM_OC1Init(TIM2, &TIM_OCStruct);

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);


    /*
    CH2 - PA1

    30% x 1000 = 300
    */
    TIM_OCStruct.TIM_Pulse = 300;

    TIM_OC2Init(TIM2, &TIM_OCStruct);

    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);


    /*
    CH3 - PA2

    50% x 1000 = 500
    */
    TIM_OCStruct.TIM_Pulse = 500;

    TIM_OC3Init(TIM2, &TIM_OCStruct);

    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);


    /*
    CH4 - PA3

    70% x 1000 = 700
    */
    TIM_OCStruct.TIM_Pulse = 700;

    TIM_OC4Init(TIM2, &TIM_OCStruct);

    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);


    /*
    Cho phep preload ARR.
    */
    TIM_ARRPreloadConfig(TIM2, ENABLE);


    /*
    Bat TIM2.
    */
    TIM_Cmd(TIM2, ENABLE);
}


int main(void)
{
    /*
    Cap nhat gia tri clock he thong.
    */
    SystemCoreClockUpdate();

    /*
    Cau hinh cac chan PA0 - PA3.
    */
    GPIO_Config();

    /*
    Cau hinh TIM2 PWM.
    */
    TIM2_PWM_Config();


    while (1)
    {
        /*
        Khong can xu ly gi trong while.

        TIM2 tu dong tao PWM bang phan cung.
        */
    }
}