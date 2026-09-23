# STM32F103 - Buổi 2

Repository chứa các bài thực hành STM32F103 trên Linux, sử dụng Standard Peripheral Library (STD), Makefile và GNU ARM Toolchain.

## Nội dung bài tập

### Bài 1 - UART
Thư mục: bai01_uart

- Cấu hình UART ở chế độ TX và RX.
- Nhận dữ liệu từ PC và lưu vào bộ đệm.
- Khi nhận ký tự kết thúc !, STM32 gửi lại toàn bộ bản tin đã nhận về máy tính. :contentReference[oaicite:0]{index=0}

### Bài 2 - SysTick
Thư mục: bai02_systick

- Cấu hình SysTick.
- Sử dụng ngắt SysTick để điều khiển 3 LED độc lập.
- Tần số LED: 0.1 Hz, 1 Hz, 10 Hz.
- Không sử dụng delay để tạo các tần số LED. :contentReference[oaicite:1]{index=1}

### Bài 3 - ADC
Thư mục: bai03_adc

- Đọc tín hiệu Analog bằng ADC.
- Có thể sử dụng biến trở hoặc cảm biến Analog.
- Chuyển giá trị ADC sang điện áp.
- Gửi giá trị điện áp lên PC qua UART mỗi 1 giây. :contentReference[oaicite:2]{index=2}

### Bài 4 - PWM 4 kênh
Thư mục: bai04_pwm

- Sử dụng 4 kênh PWM của một Timer.
- Tần số PWM: 1 kHz.
- Duty Cycle:
  - CH1: 10%
  - CH2: 30%
  - CH3: 50%
  - CH4: 70%
- Có thể quan sát độ sáng của 4 LED tăng dần theo Duty Cycle. :contentReference[oaicite:3]{index=3}

### Bài 5 - PWM điều khiển bằng UART
Thư mục: bai05_pwm_uart

- Cấu hình một kênh PWM.
- UART hoạt động bằng ngắt.
- Điều khiển LED từ PC bằng các lệnh:
  - ON!
  - OFF!
  - PWM:Percent%!
  - Status!
- Khi LED đang OFF, thay đổi PWM chỉ cập nhật giá trị cấu hình.
- Khi bật lại, LED sử dụng mức PWM gần nhất. :contentReference[oaicite:4]{index=4}

## Công cụ sử dụng

- STM32F103C8T6
- Ubuntu Linux
- Visual Studio Code
- STM32 Standard Peripheral Library
- arm-none-eabi-gcc
- Makefile
- ST-Link
