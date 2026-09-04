# CrazyRoad CSU-21th 智能车项目

## 项目简介

本项目为中南大学（CSU）第21届智能车竞赛参赛工程，基于龙邱科技 LQ_TC387 核心板驱动库开发，用于智能车竞速赛道中的感知、控制与路径规划。

## 硬件平台

| 项目 | 参数 |
|------|------|
| 主控芯片 | Infineon AURIX TC387QP |
| 架构 | TriCore 三核 + 1 辅助核 @ 300MHz × 4 |
| Flash | 10 MB |
| RAM | 1568 KB |
| 开发板 | 龙邱科技 LQ_TC387 核心板 + 母板 |

## 开发环境

- **IDE**：AURIX Development Studio (ADS) 1.10.2
- **编译器**：Tasking TriCore Compiler
- **调试器**：winIDEA / DAP Miniwiggler
- **文件编码**：UTF-8

## 目录结构

```
workspace_6_8/
├── lq_6_7/                     # 主工程目录
│   ├── Main/                   # 主函数入口 (Cpu0~Cpu3)
│   ├── Src/
│   │   ├── APP/                # 应用层 — 摄像头、显示器、IMU、电机、编码器等外设驱动
│   │   ├── Core/               # 核心配置 — CPU初始化、中断配置
│   │   ├── Driver/             # 底层驱动 — ADC/DMA/SPI/QSPI/UART/GTM/PWM
│   │   └── User/               # 用户算法 — PID控制、图像处理、路径规划、四元数姿态解算
│   ├── Libraries/              # 官方库
│   │   ├── iLLD/               # Infineon 底层驱动库
│   │   └── Infra/Service/      # 基础设施与服务
│   └── Configurations/         # 芯片配置头文件
├── detect_t_by_jumps_reference.c  # T字路口检测参考实现
├── 方法对比和集成方案.md
├── 路口识别分析报告.md
├── 方案优化设计.md
└── 学长方案优化设计.md
```

## 主要功能

- **摄像头采集**：MT9V034 灰度图像采集与处理
- **图像处理**：赛道边缘提取、元素识别（十字、环岛、坡道等）
- **路径规划**：基于赛道中线的最优路径计算
- **姿态解算**：MPU6050 六轴陀螺仪 + 四元数解算
- **电机控制**：BLDC 无刷电机 PWM 驱动、舵机转向控制
- **编码器**：速度闭环反馈
- **无线通信**：WiFi 图传、蓝牙数传、SBUS 遥控
- **显示模块**：TFT/IPS LCD/OLED 屏幕调试显示
- **ADC 采集**：7 路麦克风、按键、电池电压

## 版本历史

| 版本 | 日期 | 更新内容 |
|------|------|----------|
| V2.0.0 | 2026-03-25 | 添加 MINI 驱动一体板、WiFi 模块接口选择、文件头声明 |
| V1.2.0 | 2026-03-11 | 新增16路模拟灰度光传感器、WiFi图传自动配置、修复 QSPI 引脚编码错误 |
| V1.0.0 | — | 基于 V7 通用母板扩展的初始版本 |

## 开源协议

本项目基础库为龙邱科技 LQ_TC387_Software_Library，遵循 **LGPL v3** 协议。商业用途需联系龙邱科技获得授权。

## 团队成员

中南大学 **CrazyRoad** 智能车队 第21届

---

> 仓库地址：https://gitee.com/betasigmajj/crazyroad_csu-21th
