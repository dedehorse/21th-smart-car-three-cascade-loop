/*******************************************************************************
 *  @file                 本文件是LQ_TC387_Software_Library 软件开源库文件的一部分
 *  @author               chiusir
 *  @email                chiusir@163.com
 *  @version              V2.0.0
 *  @update               2026年3月24日
 *  @copyright            版权所有 (C) 2025-2026 北京龙邱科技有限公司
 *  @website              http://www.lqist.cn
 *  @taobao               http://longqiu.taobao.com
 *
 *  @brief                龙邱科技 LQ_TC387核心板驱动库声明
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

#ifndef __LQ_UART_H__
#define __LQ_UART_H__

#include "lq_include.h"

#define ASC_TX_BUFFER_SIZE 64 // 发送缓冲区长度
#define ASC_RX_BUFFER_SIZE 64 // 接收缓冲区长度

extern IfxAsclin_Asc g_UartConfig[4];

/**
 * @brief UART 枚举
 */
typedef enum
{
    UART0 = 0,
    UART1,
    UART2,
    UART3
} UART_t;

// L.Q UART RX 管脚枚举 // GPIO 枚举端口编号，参考TC397datasheet更新，2023年2月5日
typedef enum
{
    UART0_RX_P14_1 = 0xAE01,
    UART0_RX_P15_3 = 0xAF03,

    UART1_RX_P02_3 = 0xA203,
    UART1_RX_P11_10 = 0xAB0A,
    UART1_RX_P15_1 = 0xAF01,
    UART1_RX_P15_5 = 0xAF05,
    UART1_RX_P20_9 = 0xB409,
    UART1_RX_P33_13 = 0xC10D,

    UART2_RX_P02_0 = 0xA200,
    UART2_RX_P02_1 = 0xA201,
    UART2_RX_P10_6 = 0xAA06,
    UART2_RX_P14_3 = 0xAE03,
    UART2_RX_P33_8 = 0xC108,

    UART3_RX_P00_1 = 0xA001,
    UART3_RX_P15_7 = 0xAF07,
    UART3_RX_P20_3 = 0xB403,
    UART3_RX_P21_6 = 0xB506

} UART_RX_t;

// L.Q UART TX 管脚枚举
typedef enum
{
    UART0_TX_P14_0 = 0xAE00,
    UART0_TX_P14_1 = 0xAE01,
    UART0_TX_P15_2 = 0xAF02,
    UART0_TX_P15_3 = 0xAF03,

    UART1_TX_P02_2 = 0xA202,
    UART1_TX_P11_12 = 0xAB0C,
    UART1_TX_P15_0 = 0xAF00,
    UART1_TX_P15_1 = 0xAF01,
    UART1_TX_P15_4 = 0xAF04,
    UART1_TX_P15_5 = 0xAF05,
    UART1_TX_P20_10 = 0xB40A,
    UART1_TX_P33_12 = 0xC10C,
    UART1_TX_P33_13 = 0xC10D,

    UART2_TX_P02_0 = 0xA200,
    UART2_TX_P10_5 = 0xAA05,
    UART2_TX_P14_2 = 0xAE02,
    UART2_TX_P14_3 = 0xAE03,
    UART2_TX_P33_8 = 0xC108,
    UART2_TX_P33_9 = 0xC109,

    UART3_TX_P00_0 = 0xA000,
    UART3_TX_P00_1 = 0xA001,
    UART3_TX_P15_6 = 0xAF06,
    UART3_TX_P15_7 = 0xAF07,
    UART3_TX_P20_0 = 0xB400,
    UART3_TX_P20_3 = 0xB403,
    UART3_TX_P21_7 = 0xB507

} UART_TX_t;


void UART_InitConfig(UART_RX_t RxPin, UART_TX_t TxPin, unsigned long baudrate);
void UART_PutChar(UART_t uratn, char ch);
void UART_PutStr(UART_t uratn, char *str);
void UART_PutBuff(UART_t uratn, unsigned char *buff, unsigned long len);
uint8 UART_GetCount(UART_t uratn);
char UART_GetChar(UART_t uratn);
char UART_GetBuff(UART_t uratn, unsigned char *data, unsigned char len);

#endif /* __LQ_UART_H__ */
