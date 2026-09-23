# STM32F103 - Buổi 2

Repository chứa các bài thực hành STM32F103 trên Linux sử dụng Standard Peripheral Library (STD), Makefile và arm-none-eabi-gcc.

## Nội dung

- bai01_uart: UART TX/RX, nhận dữ liệu từ PC và gửi lại khi gặp ký tự !
- bai02_systick: Dùng ngắt SysTick điều khiển 3 LED ở 0.1 Hz, 1 Hz và 10 Hz
- bai03_adc: Đọc ADC, tính điện áp và gửi dữ liệu lên PC qua UART mỗi 1 giây
- bai04_pwm: TIM2 tạo 4 kênh PWM 1 kHz với duty 10%, 30%, 50%, 70%
- bai05_pwm_uart: Điều khiển PWM bằng UART với các lệnh ON, OFF, PWM và Status

## Công cụ

- STM32F103C8T6
- Ubuntu Linux
- Visual Studio Code
- STM32 Standard Peripheral Library
- GNU ARM Toolchain
- Makefile
- ST-Link

## Build

```bash
make clean
make
Nạp chương trình
st-flash write <file>.bin 0x08000000