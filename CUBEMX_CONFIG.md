# CubeMX 配置与呼吸灯代码说明

依据本地 `TIM_PWM_LIGHT.ioc` 和初始化代码整理。

## 关键配置

| 配置 | 当前设置 | 作用 |
| --- | --- | --- |
| 芯片 | STM32F103C8T6 | 本工程使用的单片机 |
| 系统时钟 | 工程按外部晶振 8 MHz、PLL ×9 配置，得到 72 MHz | 提供运行时钟 |
| 调试接口 | Serial Wire（SWD） | PA13、PA14 用于下载与调试 |
| PWM 引脚 | PA6 → TIM3_CH1 | 输出控制 LED 的 PWM 信号 |
| TIM3 时钟 | 72 MHz | APB1 为 36 MHz，当前分频下定时器时钟为其 2 倍 |
| Prescaler | `72-1`，即 71 | 将计数频率降到 1 MHz |
| Counter Period | `100-1`，即 99 | 每 100 次计数产生一个 PWM 周期 |
| PWM 模式 | PWM mode 1，高电平有效 | 用比较值控制每周期高电平的比例 |

PWM 频率：`72 MHz ÷ 72 ÷ 100 = 10 kHz`，每周期为 `100 μs`。

## main() 怎么工作

对应仓库中的 [main-function.c](main-function.c)：

1. `HAL_Init()`、`SystemClock_Config()`：初始化 HAL 和系统时钟。
2. `MX_GPIO_Init()`、`MX_TIM3_Init()`：配置引脚和定时器。
3. `HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1)`：启动 PWM 输出。
4. 在 `while (1)` 中反复改变比较值，让 LED 亮度往复变化。

关键的一行：

```c
__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, i);
```

它把 TIM3 通道 1 的比较值 CCR1 设置为 `i`。当前每周期计数 100 次，因此高电平占空比为 `i / 100`：

| i | 高电平占空比 |
| --- | --- |
| 0 | 0% |
| 50 | 50% |
| 99 | 99% |

第一个 `for` 让 `i` 从 **0 增加到 98**，第二个让 `i` 从 **99 减少到 0**。`HAL_Delay(10)` 控制每一步变化的等待时间。若 LED 高电平点亮，占空比越大通常越亮；低电平点亮时方向相反。

**10 kHz 决定 PWM 脉冲的快慢；循环中的延时决定亮度变化的快慢。**

## 一个需要注意的差异

CubeMX 的 `.ioc` 保存了 `Pulse = 50`，但当前 `MX_TIM3_Init()` 中是 `sConfigOC.Pulse = 0`。两者分别表示初始占空比 50% 和 0%；启动后，循环会继续用 `i` 更新占空比。重新生成代码前，应按希望的初始状态统一这个设置。

`main-function.c` 是主函数片段，编译还需要 CubeMX 生成的初始化代码、HAL 驱动及 `htim3` 定义。
