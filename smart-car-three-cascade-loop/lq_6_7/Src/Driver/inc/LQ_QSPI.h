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

#ifndef __LQ_QSPI_H__
#define __LQ_QSPI_H__

#include "lq_include.h"
//#include "IfxQspi_PinMap.h"
/***********************************************************************************************/
/***********************************************************************************************/
/*****************    QSPI中断优先级 和 指向内核配置 用户可以自行修改*******************************/
/***********************************************************************************************/
/***********************************************************************************************/

/**
 * QSPI 枚举
 */
typedef enum
{
    QSPI0,
    QSPI1,
    QSPI2,
    QSPI3,
    QSPI4
} QSPI_t;

/**
 * QSPI 模式
 */

typedef enum
{
    QSPI_MODE_0 = 0x00, // CLK空闲时刻为低电平 在第一个CLK跳边沿 捕获数据
    QSPI_MODE_1,        // CLK空闲时刻为低电平 在第二个CLK跳边沿 捕获数据
    QSPI_MODE_2,        // CLK空闲时刻为高电平 在第一个CLK跳边沿 捕获数据
    QSPI_MODE_3        // CLK空闲时刻为高电平 在第二个CLK跳边沿 捕获数据
}QSPI_Mode_t;

/**
 * QSPI CLK管脚枚举
 */
typedef enum
{
    QSPI0_CLK_P20_11 = 0x0000,   /* QSPI0 SCLK的第0组引脚 */
    QSPI0_CLK_P20_13 = 0x0001,   /* QSPI0 SCLK的第1组引脚 */
    QSPI0_CLK_P22_7  = 0x0002,
    QSPI0_CLK_P22_8  = 0x0003,

    QSPI1_CLK_P10_2  = 0x0100,
    QSPI1_CLK_P11_6  = 0x0101,

    QSPI2_CLK_P13_0  = 0x0200,
    QSPI2_CLK_P13_1  = 0x0201,
    QSPI2_CLK_P15_3  = 0x0202,
    QSPI2_CLK_P15_6  = 0x0203,
    QSPI2_CLK_P15_8  = 0x0204,
    QSPI2_CLK_P33_1  = 0x0205,
    QSPI2_CLK_P33_14 = 0x0206,

    QSPI3_CLK_P01_7  = 0x0300,
    QSPI3_CLK_P02_7  = 0x0301,
    QSPI3_CLK_P10_8  = 0x0302,

    QSPI4_CLK_P22_0  = 0x0400,
    QSPI4_CLK_P22_1  = 0x0401,
    QSPI4_CLK_P22_3  = 0x0402,
    QSPI4_CLK_P33_11 = 0x0403,

    QSPI_CLK_NULL    = 0xFFFF
}QSPI_CLK_t;

/**
 * QSPI MOSI/MTSR 管脚枚举
 */
typedef enum
{
    QSPI0_MOSI_P20_12 = 0x0000,
    QSPI0_MOSI_P20_14 = 0x0001,
    QSPI0_MOSI_P22_5  = 0x0002,
    QSPI0_MOSI_P22_10 = 0x0003,

    QSPI1_MOSI_P10_1  = 0x0100,
    QSPI1_MOSI_P10_3  = 0x0101,
    QSPI1_MOSI_P10_4  = 0x0102,
    QSPI1_MOSI_P11_9  = 0x0103,

//    QSPI2_MOSI_P13_2  = 0x0200,   // 该引脚测试暂不可用
    QSPI2_MOSI_P13_3  = 0x0201,
    QSPI2_MOSI_P15_5  = 0x0202,
    QSPI2_MOSI_P15_6  = 0x0203,
    QSPI2_MOSI_P34_5  = 0x0204,

    // QSPI3 在传输数据时, 会偶尔出现数据不对的情况, 建议优先选择其他 QSPI
    QSPI3_MOSI_P01_6  = 0x0300,
    QSPI3_MOSI_P02_6  = 0x0301,
    QSPI3_MOSI_P10_6  = 0x0302,

    QSPI4_MOSI_P22_0  = 0x0400,
    QSPI4_MOSI_P22_2  = 0x0401,
    QSPI4_MOSI_P22_3  = 0x0402,
    QSPI4_MOSI_P33_12 = 0x0403,

    QSPI_MOSI_NULL    = 0xFFFF
} QSPI_MOSI_t;


/**
 * QSPI MISO/MRST 管脚枚举
 */
typedef enum
{
    QSPI0_MISO_P20_12 = 0x0000,
    QSPI0_MISO_P22_9  = 0x0001,
    QSPI0_MISO_P22_6  = 0x0002,

    QSPI1_MISO_P10_1  = 0x0100,
    QSPI1_MISO_P11_3  = 0x0101,

    QSPI2_MISO_P15_4  = 0x0200,
    QSPI2_MISO_P15_7  = 0x0201,
    QSPI2_MISO_P21_2  = 0x0202,
    QSPI2_MISO_P34_4  = 0x0203,
    QSPI2_MISO_P15_2  = 0x0204,
    QSPI2_MISO_P14_9  = 0x0205,

    QSPI3_MISO_P02_5  = 0x0300,
    QSPI3_MISO_P10_7  = 0x0301,
    QSPI3_MISO_P01_5  = 0x0302,

    QSPI4_MISO_P33_13 = 0x0400,
    QSPI4_MISO_P22_1  = 0x0401,
    QSPI4_MISO_P21_2  = 0x0402,
    QSPI4_MISO_P21_0  = 0x0403,

    QSPI_MISO_NULL    = 0xFFFF
} QSPI_MISO_t;


/**
 * QSPI 片选管脚枚举
 */
typedef enum
{
    QSPI0_CS_P20_8  = 0x0000,
    QSPI0_CS_P20_9  = 0x0001,
    QSPI0_CS_P20_13 = 0x0002,
    QSPI0_CS_P11_10 = 0x0003,
    QSPI0_CS_P11_11 = 0x0004,
    QSPI0_CS_P11_2  = 0x0005,
    QSPI0_CS_P20_10 = 0x0006,
    QSPI0_CS_P33_5  = 0x0007,
    QSPI0_CS_P20_6  = 0x0008,
    QSPI0_CS_P20_3  = 0x0009,
    QSPI0_CS_P22_11 = 0x000A,
    QSPI0_CS_P23_6  = 0x000B,
    QSPI0_CS_P22_4  = 0x000C,
    QSPI0_CS_P15_0  = 0x000D,

    QSPI1_CS_P20_8  = 0x0100,
    QSPI1_CS_P20_9  = 0x0101,
    QSPI1_CS_P20_13 = 0x0102,
    QSPI1_CS_P11_10 = 0x0103,
    QSPI1_CS_P11_11 = 0x0104,
    QSPI1_CS_P11_2  = 0x0105,
    QSPI1_CS_P33_10 = 0x0106,
    QSPI1_CS_P33_5  = 0x0107,
    QSPI1_CS_P10_4  = 0x0108,
    QSPI1_CS_P10_5  = 0x0109,
    QSPI1_CS_P10_0  = 0x010A,

    QSPI2_CS_P15_2  = 0x0200,
    QSPI2_CS_P14_2  = 0x0201,
    QSPI2_CS_P14_6  = 0x0202,
    QSPI2_CS_P14_3  = 0x0203,
    QSPI2_CS_P14_7  = 0x0204,
    QSPI2_CS_P15_1  = 0x0205,
    QSPI2_CS_P33_13 = 0x0206,
    QSPI2_CS_P20_10 = 0x0207,
    QSPI2_CS_P20_6  = 0x0208,
    QSPI2_CS_P20_3  = 0x0209,
    QSPI2_CS_P33_2  = 0x020A,
    QSPI2_CS_P33_6  = 0x020B,
    QSPI2_CS_P32_6  = 0x020C,

    QSPI3_CS_P02_4  = 0x0300,
    QSPI3_CS_P02_0  = 0x0301,
    QSPI3_CS_P02_1  = 0x0302,
    QSPI3_CS_P00_5  = 0x0303,
    QSPI3_CS_P00_2  = 0x0304,
    QSPI3_CS_P02_8  = 0x0305,
    QSPI3_CS_P00_8  = 0x0306,
    QSPI3_CS_P00_9  = 0x0307,
    QSPI3_CS_P10_5  = 0x0308,
    QSPI3_CS_P01_3  = 0x0309,
    QSPI3_CS_P01_4  = 0x030A,

    QSPI4_CS_P33_10 = 0x0408,
    QSPI4_CS_P33_9  = 0x040C,
    QSPI4_CS_P33_3  = 0x0409,
    QSPI4_CS_P22_2  = 0x0404,
    QSPI4_CS_P02_12 = 0x0401,
    QSPI4_CS_P02_15 = 0x0403,
    QSPI4_CS_P02_13 = 0x0402,
    QSPI4_CS_P02_1  = 0x0400,

    QSPI_CS_NULL    = 0xFFFF
} QSPI_CS_t;



#define QSPI_GetModule(pin) (uint8)(pin >> 8)
#define QSPI_GetIndex(pin) (uint8)(pin & 0x0f)

void  QSPI_InitConfig(QSPI_CLK_t clkPin, QSPI_MISO_t misoPin, QSPI_MOSI_t mosiPin, QSPI_CS_t cQSPIn, unsigned long baudrate, QSPI_Mode_t mode);
uint8 QSPI_ReadWriteNByte(QSPI_t QSPI, unsigned char *txData, unsigned char *rxData, uint16 len);

#endif /* __LQ_QSPI_H__ */
