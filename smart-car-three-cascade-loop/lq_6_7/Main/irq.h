/*******************************************************************************
 * @file                本文件是LQ_TC387_Software_Library 软件开源库文件的一部分
 * @author              chiusir
 * @email               chiusir@163.com
 * @version             V2.0.0
 * @update              2026年3月24日
 * @copyright           版权所有 (C) 2025-2026 北京龙邱科技有限公司
 * @website             http://www.lqist.cn
 * @taobao              http://longqiu.taobao.com
 *
 * @brief               龙邱科技 LQ_TC387核心板驱动库声明
 *
 * 本软件遵循GPL-3.0开源协议发布，旨在为TC387芯片嵌入式系统设计者提供快速上手、开发基于TC387应用程序的参考
 * 商业用途（包括单位使用）需提前联系 http://www.lqist.cn 获得授权
 *
 * 开发环境配置:
 *   - 开发平台 : AURIX-Studio 版本-1.10.2  (简称ADS 1.10.x)
 *   - 文件编码 : UTF-8  (AURIX-Studio-1.10.x 默认编码)
 *   - 目标芯片 : TC387QP (TriCore™ @ 300 MHz X4 10 Mbyte flash, 1568 KB of RAM)
 *   - 外置晶振 : 20MHz
 *   - 系统PLL : 300MHz + 300MHz + 300MHz + 300MHz
 * 
 * GPL-3.0 许可证声明摘要:
 * 1. 允许自由使用、修改、分发本软件
 * 2. 分发修改后的版本时，必须以相同许可证发布
 * 3. 必须保留原始版权声明和许可证信息
 * 4. 不提供任何担保，使用风险自负
 * 5. 完整协议文本请参见项目根目录 LICENSE 文件
 *******************************************************************************/
#ifndef _IRQ_H_
#define _IRQ_H_
#include "lq_include.h"
// 中断归哪个核管理，对应的初始化函数需要放在对应的核
//////////////////////////////////////////////////////////////////////////////////

// GPIO外部中断 组0中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define PIN_INT0_PRIORITY 100

// GPIO外部中断 组0中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define PIN_INT0_VECTABNUM 0

// GPIO外部中断 组1中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define PIN_INT1_PRIORITY 101

// GPIO外部中断 组1中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define PIN_INT1_VECTABNUM 0

// GPIO外部中断 组2中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define PIN_INT2_PRIORITY 40 // 摄像头像素信号

// GPIO外部中断 组2中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define PIN_INT2_VECTABNUM 1

// GPIO外部中断 组3中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define PIN_INT3_PRIORITY 253 // 摄像头场信号

// GPIO外部中断 组3中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define PIN_INT3_VECTABNUM 0

// GPIO外部中断 组4中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define PIN_INT4_PRIORITY 105

// GPIO外部中断 组4中断归哪个内核管理？ 范围：0：CPU0  1：DMA  2：CPU1 3：CPU2...
#define PIN_INT4_VECTABNUM 0

// GPIO外部中断 组5中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define PIN_INT5_PRIORITY 102

// GPIO外部中断 组5中断归哪个内核管理？ 范围：0：CPU0  1：DMA  2：CPU1 3：CPU2...
#define PIN_INT5_VECTABNUM 0

// GPIO外部中断 组6中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define PIN_INT6_PRIORITY 41 // 摄像头像素信号

// GPIO外部中断 组6中断归哪个内核管理？ 范围：0：CPU0  1：DMA  2：CPU1 3：CPU2...
#define PIN_INT6_VECTABNUM 0

// GPIO外部中断 组7中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define PIN_INT7_PRIORITY 253

// GPIO外部中断 组7中断归哪个内核管理？ 范围：0：CPU0  1：DMA  2：CPU1 3：CPU2...
#define PIN_INT7_VECTABNUM 0

//////////////////////////////////////////////////////////////////////////////////

// CCU6定时器中断 CCU60  channel0 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CCU60_CH0_PRIORITY 80

// CCU6定时器中断 CCU60  channel1 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CCU60_CH1_PRIORITY 81

// CCU6定时器中断 CCU60   中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define CCU60_VECTABNUM 0

// CCU6定时器中断 CCU61  channel0 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CCU61_CH0_PRIORITY 82

// CCU6定时器中断 CCU61  channel1 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CCU61_CH1_PRIORITY 83

// CCU6定时器中断 CCU60   中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define CCU61_VECTABNUM 0

//////////////////////////////////////////////////////////////////////////////////

// STM定时器中断 STM0  channel0 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define STM0_CH0_PRIORITY 110

// STM定时器中断 STM0  channel1 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define STM0_CH1_PRIORITY 111

// STM定时器中断 STM0  中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define STM0_VECTABNUM 0

// STM定时器中断 STM1  channel0 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define STM1_CH0_PRIORITY 112

// STM定时器中断 STM1  channel1 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define STM1_CH1_PRIORITY 113

// STM定时器中断 STM1   中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define STM1_VECTABNUM 0

//////////////////////////////////////////////////////////////////////////////////

// UART0接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define UART0_RX_PRIORITY 120

// UART0发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define UART0_TX_PRIORITY 121

// UART0错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define UART0_ER_PRIORITY 122

// UART0中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define UART0_VECTABNUM 0

// UART1接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define UART1_RX_PRIORITY 130

// UART1发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define UART1_TX_PRIORITY 131

// UART1错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define UART1_ER_PRIORITY 132

// UART1中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define UART1_VECTABNUM 0

// UART2接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define UART2_RX_PRIORITY 133

// UART2发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define UART2_TX_PRIORITY 134

// UART2错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define UART2_ER_PRIORITY 135

// UART2中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define UART2_VECTABNUM 0

// UART3接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define UART3_RX_PRIORITY 136

// UART3发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define UART3_TX_PRIORITY 137

// UART3错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define UART3_ER_PRIORITY 138

// UART3中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define UART3_VECTABNUM 0

//////////////////////////////////////////////////////////////////////////////////

// QSPI0接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define QSPI0_RX_PRIORITY 150

// QSPI0发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define QSPI0_TX_PRIORITY 151

// QSPI0错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define QSPI0_ER_PRIORITY 152

// QSPI0中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define QSPI0_VECTABNUM 0

// QSPI1接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define QSPI1_RX_PRIORITY 153

// QSPI1发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define QSPI1_TX_PRIORITY 154

// QSPI1错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define QSPI1_ER_PRIORITY 155

// QSPI1中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define QSPI1_VECTABNUM 0

// QSPI2接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define QSPI2_RX_PRIORITY 156

// QSPI2发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define QSPI2_TX_PRIORITY 157

// QSPI2错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define QSPI2_ER_PRIORITY 158

// QSPI2中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define QSPI2_VECTABNUM 0

// QSPI3接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define QSPI3_RX_PRIORITY 159

// QSPI3发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define QSPI3_TX_PRIORITY 160

// QSPI3错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define QSPI3_ER_PRIORITY 161

// QSPI3中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define QSPI3_VECTABNUM 0

// QSPI4接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define QSPI4_RX_PRIORITY 162

// QSPI4发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define QSPI4_TX_PRIORITY 163

// QSPI4错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define QSPI4_ER_PRIORITY 164

// QSPI4中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define QSPI4_VECTABNUM 4   // CPU3

//////////////////////////////////////////////////////////////////////////////////

// SPI0接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define SPI0_RX_PRIORITY 180

// SPI0发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define SPI0_TX_PRIORITY 181

// SPI0错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define SPI0_ER_PRIORITY 182

// SPI0中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define SPI0_VECTABNUM 0

// SPI1接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define SPI1_RX_PRIORITY 183

// SPI1发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define SPI1_TX_PRIORITY 184

// SPI1错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define SPI1_ER_PRIORITY 185

// SPI1中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define SPI1_VECTABNUM 0

// SPI2接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define SPI2_RX_PRIORITY 186

// SPI2发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define SPI2_TX_PRIORITY 187

// SPI2错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define SPI2_ER_PRIORITY 188

// SPI2中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define SPI2_VECTABNUM 0

// SPI3接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define SPI3_RX_PRIORITY 190

// SPI3发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define SPI3_TX_PRIORITY 191

// SPI3错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define SPI3_ER_PRIORITY 192

//////////////////////////////////////////////////////////////////////////////////

// SPI3中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define SPI3_VECTABNUM 0

// CPU0软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU0_SOFT_IRQ0_PRIORITY 220

// CPU0软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU0_SOFT_IRQ1_PRIORITY 221

// CPU0软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU0_SOFT_IRQ2_PRIORITY 222

// CPU0软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU0_SOFT_IRQ3_PRIORITY 223

// CPU1软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU1_SOFT_IRQ0_PRIORITY 224

// CPU1软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU1_SOFT_IRQ1_PRIORITY 225

// CPU1软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU1_SOFT_IRQ2_PRIORITY 226

// CPU1软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU1_SOFT_IRQ3_PRIORITY 227

// CPU2软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU2_SOFT_IRQ0_PRIORITY 228

// CPU2软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU2_SOFT_IRQ1_PRIORITY 229

// CPU2软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU2_SOFT_IRQ2_PRIORITY 230

// CPU2软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU2_SOFT_IRQ3_PRIORITY 231
// CPU2软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU3_SOFT_IRQ0_PRIORITY 232

// CPU2软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU3_SOFT_IRQ1_PRIORITY 233

// CPU2软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU3_SOFT_IRQ2_PRIORITY 234

// CPU2软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define CPU3_SOFT_IRQ3_PRIORITY 235
//////////////////////////////////////////////////////////////////////////////////

// DMA中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define DMA_PRIORITY 255

// DMA中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define DMA_VECTABNUM 0

void CCU60_CH0_IRQHandler(void);
void CCU60_CH1_IRQHandler(void);
void CCU61_CH0_IRQHandler(void);
void CCU61_CH1_IRQHandler(void);
void STM0_CH0_IRQHandler(void);
void STM0_CH1_IRQHandler(void);
void STM1_CH0_IRQHandler(void);
void STM1_CH1_IRQHandler(void);

void PIN_INT0_IRQHandler(void);
void PIN_INT1_IRQHandler(void);
void PIN_INT2_IRQHandler(void);
void PIN_INT3_IRQHandler(void);
void PIN_INT4_IRQHandler(void);
void PIN_INT5_IRQHandler(void);
void PIN_INT6_IRQHandler(void);
void PIN_INT7_IRQHandler(void);

void UART0_RX_IRQHandler(void);
void UART0_TX_IRQHandler(void);
void UART0_ER_IRQHandler(void);
void UART1_RX_IRQHandler(void);
void UART1_TX_IRQHandler(void);
void UART1_ER_IRQHandler(void);
void UART2_RX_IRQHandler(void);
void UART2_TX_IRQHandler(void);
void UART2_ER_IRQHandler(void);
void UART3_RX_IRQHandler(void);
void UART3_TX_IRQHandler(void);
void UART3_ER_IRQHandler(void);

#if SPEED_LOOP_DEBUG
/* VOFA 速度环调试: 全局标志和变量 */
extern volatile uint8_t vofa_flag;
extern volatile uint8_t g_cmd_ready;
extern volatile uint32_t g_cmd_recv_cnt;
extern char g_cmd_buf[];

typedef enum { TM_STEP, TM_RAMP, TM_SINE } TargetMode_t;
extern TargetMode_t g_tgt_mode;
extern float g_tgt_ramp_start, g_tgt_ramp_end;
extern uint32_t g_tgt_ramp_ms, g_tgt_ramp_tick;
extern float g_tgt_sine_freq, g_tgt_sine_amp, g_tgt_sine_base;

void VOFA_Cmd_Process(void);
#elif GYRO_LOOP_DEBUG
/* VOFA 角速度环调试: 全局标志和变量 */
extern volatile uint8_t vofa_flag;
extern volatile uint8_t g_cmd_ready;
extern volatile uint32_t g_cmd_recv_cnt;
extern volatile float g_tgt_gyro;
extern char g_cmd_buf[];

void VOFA_Cmd_Process(void);
#elif DIR_LOOP_DEBUG
/* VOFA 方向环调试: 全局标志和变量 */
extern volatile uint8_t vofa_flag;
extern volatile uint8_t g_cmd_ready;
extern volatile uint32_t g_cmd_recv_cnt;
extern volatile float g_dir_dev;
extern char g_cmd_buf[];

void VOFA_Cmd_Process(void);
#endif

/* 赛道模式: VOFA姿态数据发送标志 */
#if !SPEED_LOOP_DEBUG && !GYRO_LOOP_DEBUG && !DIR_LOOP_DEBUG
extern volatile uint8_t imu_vofa_flag;
#endif

#endif /* _IRQ_H_ */
