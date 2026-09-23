#ifndef __STM32F10X_CONF_H
#define __STM32F10X_CONF_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

/*
Macro assert_param duoc thu vien STM32 Standard Peripheral Library su dung
de kiem tra tham so.

O bai nay khong can che do debug Full Assert,
nen assert_param se khong lam gi.
*/
#ifdef USE_FULL_ASSERT

#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, _LINE_))

void assert_failed(uint8_t *file, uint32_t line);

#else

#define assert_param(expr) ((void)0)

#endif

#endif
