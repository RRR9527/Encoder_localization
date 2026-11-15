# 码盘测速模块

基于 STM32F405 的双编码器测速模块：读取两路码盘（X/Y 轴测速轮）的脉冲，换算成车体速度，通过 CAN 总线周期广播给主控。

当前只做**测速 + 发速度**，不包含位置积分。

---

## 1. 硬件

| 项目 | 配置 |
| ---- | ---- |
| MCU | STM32F405RGTx（LQFP64，168 MHz） |
| X 轴编码器 | TIM3 编码器模式，PA6（CH1）/ PA7（CH2） |
| Y 轴编码器 | TIM5 编码器模式，PA0（CH1）/ PA1（CH2） |
| 采样定时器 | TIM2 内部时钟，100 µs 周期中断，每 10 ms 采样一次 |
| CAN | CAN1，PA11（RX）/ PA12（TX），标准帧 |
| 系统时基 | TIM6（1 ms HAL tick） |
| RTOS | FreeRTOS（CMSIS-RTOS v2） |

---

## 2. 目录结构

```
Core/        CubeMX 生成的外设与系统代码
IRQ/         中断回调（TIM_IRQHandler、CAN_IRQHandler）
Tasks/       FreeRTOS 任务（POSI_task、Led_task）
MDK-ARM/     Keil 工程（Encoder_localization.uvprojx）
```

---

## 3. 功能

- **采样**：TIM2 每 10 ms 锁存一次两路编码器的脉冲增量，存入 `odom.raw_vx_car / raw_vy_car`。
- **换算**：POSI 任务把脉冲增量换算为速度（**mm/s**），经一阶低通滤波后打包。
- **发送**：POSI 任务以 100 Hz（`osDelay(10)`）周期广播速度帧。

任务之间解耦：中断只负责刷新原始脉冲，任务按自己的节拍读最新值、发送。

上电即发送信息，不需要主控消息。

发送的速度是**小车坐标系下的X, Y方向的速度，并非世界坐标系**。TIM3对应X轴速度，TIM5对应Y轴速度。

---

## 4. 通信协议（当前实际发送）

只发一个速度帧，标准帧、DLC = 8、大端字节序、100 Hz：

| ID | 方向 | 内容 |
| ---- | ---- | ---- |
| `0x210` | 模块 → 主控 | 速度帧 |

**速度帧 `0x210` 字节布局：**

| 字节 | 字段 | 说明 |
| ---- | ---- | ---- |
| 0 ~ 1 | `vx` | X 轴速度，int16，mm/s，大端 |
| 2 ~ 3 | `vy` | Y 轴速度，int16，mm/s，大端 |
| 4 ~ 7 | — | 预留，填 `0x00` |

---

## 5. 速度换算与标定

```c
// 10 ms 采样窗口，PERIOD = 100（TIM2 100µs × 100）
v(mm/s) = Δcnt × CM_PER_CNT × 1000
```

- `Δcnt`：10 ms 窗口内的脉冲增量；
- `CM_PER_CNT`：轮径系数（cm / 计数），定义在 `Tasks/inc/POSI_task.h`，**前人数值，待标定**；
- `×1000`：同时完成「10 ms → 秒」和「cm → mm」两步单位换算。

---

## 6. 编译

1. Keil MDK 打开 `MDK-ARM/Encoder_localization.uvprojx`；
2. 编译、烧录到 STM32F405RGTx；
3. CAN 总线两端接 120 Ω 终端电阻，波特率与主控保持一致。
