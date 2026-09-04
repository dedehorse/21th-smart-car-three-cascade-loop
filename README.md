# TC387 Smart Car · Three Cascade Loops

中南大学 CrazyRoad 第 21 届智能车工程的三串级控制环版本。项目基于 AURIX TC387QP，围绕“方向/角度外环 + 角速度中环 + 速度内环”构建车辆控制系统，并配套摄像头赛道识别、路径规划和 VOFA 调试输出。

## 控制架构

```text
赛道偏差 / 目标偏航角
          │
          ▼
方向环或角度环（外环，约 200 Hz）
          │  输出目标角速度
          ▼
角速度环（中环，约 500 Hz）
          │  输出差速/速度修正
          ▼
左右速度环（内环，约 1 kHz）
          │
          ▼
编码器反馈 → PWM 电机输出
```

外环可在方向控制和目标偏航角控制之间切换；中环使用陀螺仪反馈；内环使用左右编码器实现速度闭环。

## 主要功能

- MT9V034 摄像头采集和赛道边缘处理
- 赛道中线提取、节点识别、十字/T 字/环岛处理
- 方向环、角度环、角速度环和左右速度环
- MPU6050/陀螺仪姿态解算
- 编码器测速、无刷电机 PWM 和舵机控制
- WiFi/蓝牙数传、TFT/OLED 调试显示
- VOFA FireWater 波形输出和串口调参接口
- 冲出赛道自动停车、节点调试和运行状态保护

## 硬件与开发环境

| 类别 | 配置 |
| --- | --- |
| MCU | Infineon AURIX TC387QP |
| 开发板 | 龙邱 LQ_TC387 核心板 + V7 通用母板 |
| IDE | AURIX Development Studio 1.10.x |
| 编译器 | Tasking TriCore Compiler |
| 下载调试 | DAP MiniWiggler / winIDEA |

## 工程结构

```text
lq_6_7/
├── Main/                  Cpu0~Cpu3 入口、配置和中断
├── Src/APP/               摄像头、电机、编码器、IMU 和外设应用
├── Src/Driver/            ADC、DMA、SPI、QSPI、UART、GTM/PWM
├── Src/User/              PID、路径规划、图像和元素识别算法
├── Libraries/             Infineon iLLD 与基础服务库
└── Configurations/        芯片启动及链接配置
```

## 关键文件

- `lq_6_7/Main/irq.c`：控制环调度、中断和调试命令
- `lq_6_7/Main/config.h`：功能开关及 PID 初始参数
- `lq_6_7/Src/User/LQ_PID.c`：位置式/增量式 PID 实现
- `lq_6_7/Src/APP/LQ_Track.c`：赛道和元素识别
- `lq_6_7/Src/APP/LQ_PWM_Moto.c`：电机状态和 PWM 控制
- `detect_t_by_jumps_reference.c`：T 字路口跳变检测参考代码

## 编译与调试

1. 在 AURIX Development Studio 导入 `lq_6_7` 工程。
2. 检查 `Main/config.h`、引脚映射和链接脚本。
3. 先在静止或抬轮状态验证传感器、编码器和 PWM。
4. 依次调试速度环、角速度环，再调试方向/角度外环。
5. 低速上车测试，确认冲出赛道停车和人工停车均有效。

## 调参建议

建议由内到外调试：先让左右速度环稳定跟踪，再加入角速度环，最后启用方向环或角度环。每次只改变少量参数，并记录速度、角速度、赛道偏差和 PWM 波形。

## 版本说明

本仓库对应 `workspace_6_8`，与 `smart-car-vofa` 是两个独立控制方案：本仓库突出三串级控制和算法验证，VOFA 仓库突出最终车辆工程和在线调参集成。

## 许可与致谢

工程包含龙邱 LQ_TC387 软件库和 Infineon iLLD。请遵守仓库内上游许可证、版权声明及相关硬件厂商授权条款。
