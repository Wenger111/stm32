# PWM_BREATH_LIGHT — CubeMX 配置

芯片：STM32F103C8T6。当前值依据 `TIM_PWM_LIGHT.ioc`，并核对初始化代码。

**★ 粗体表示非默认配置。** TIM3 参数的默认值以你提供的截图为对照；Mode、SYS 和时钟树只记录当前选择，不推测未确认的默认值。

## Timers → TIM3

### Mode

| 项目 | 当前选择 |
| --- | --- |
| Internal Clock | ☑ 勾选 |
| Channel1 | PWM Generation CH1 |

输出引脚：PA6 → TIM3_CH1。

### Parameter Settings → Counter Settings

| 参数 | 当前设置 | 截图默认值 |
| --- | --- | --- |
| **Prescaler (PSC - 16 bits value)** | **★ 72-1（71）** | 0 |
| Counter Mode | Up | Up |
| **Counter Period (AutoReload Register - 16 bits value)** | **★ 100-1（99）** | 65535 |
| Internal Clock Division (CKD) | No Division | No Division |
| auto-reload preload | Disable | Disable |

### Trigger Output (TRGO) Parameters

以下保持截图中的默认值。

| 参数 | 当前设置 |
| --- | --- |
| Master/Slave Mode (MSM bit) | Disable (Trigger input effect not delayed) |
| Trigger Event Selection | Reset (UG bit from TIMx_EGR) |

### PWM Generation Channel 1

| 参数 | 当前设置 | 截图默认值 |
| --- | --- | --- |
| Mode | PWM mode 1 | PWM mode 1 |
| **Pulse (16 bits value)** | **★ 50（.ioc 保存值）** | 0 |
| Output compare preload | Enable | Enable |
| Fast Mode | Disable | Disable |
| CH Polarity | High | High |

> 注意：当前 `main.c` 中的初始化语句是 `sConfigOC.Pulse = 0;`，与 `.ioc` 保存的 50 不一致。上表记录 CubeMX 已保存的配置；当前代码则从 0% 初始化。重新生成代码前，应按希望的初始状态统一 Pulse。

## System Core → RCC / SYS

| 页面 | 项目 | 当前设置 |
| --- | --- | --- |
| RCC | **High Speed Clock (HSE)** | **★ Crystal/Ceramic Resonator**（未启用时为 Disable） |
| SYS | Debug | Serial Wire |
| SYS | Timebase Source | SysTick |

HSE 的非默认判断已核对本机与工程匹配的 CubeMX 数据库。外部晶振使用 PD0/PD1，SWD 使用 PA13/PA14。

## Clock Configuration

| 项目 | 当前设置 / 结果 |
| --- | --- |
| HSE | 工程按 8 MHz 配置 |
| PLL Source / PLLMUL | HSE / ×9 |
| System Clock Mux / SYSCLK | PLLCLK / 72 MHz |
| AHB Prescaler / HCLK | /1 / 72 MHz |
| APB1 Prescaler / PCLK1 | /2 / 36 MHz |
| APB1 Timer clocks（TIM3） | 72 MHz |
| APB2 Prescaler / PCLK2 | /1 / 72 MHz |

## 对应代码的含义

- `72 MHz ÷ (71+1) ÷ (99+1) = 10 kHz`：PWM 周期为 100 μs。
- `__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, i)`：设置比较值 CCR1；当前高电平占空比为 `i / 100`。
- 两个 `for` 使 `i` 先从 0 增至 98，再从 99 减至 0；`HAL_Delay(10)` 控制亮度变化的速度。LED 亮暗方向取决于接线。

主函数见 [main-function.c](main-function.c)。
