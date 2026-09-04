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

#ifndef LQ_SPI_H_
#define LQ_SPI_H_

/*
 * @file tc387普通SPI 枚举
 */
#include "lq_include.h"

/**
 * SPI CLK管脚枚举
 */
typedef enum
{
    SPI0_CLK_P14_0 = 0x0000,
    SPI0_CLK_P15_2 = 0x0001,
    SPI1_CLK_P15_0 = 0x0100,
    SPI1_CLK_P20_10 = 0x0101,
    SPI1_CLK_P33_11 = 0x0102,
    SPI1_CLK_P33_12 = 0x0103,
    SPI2_CLK_P02_4 = 0x0200,
    SPI2_CLK_P10_6 = 0x0201,
    SPI2_CLK_P14_2 = 0x0202,
    SPI2_CLK_P33_7 = 0x0203,
    SPI2_CLK_P33_9 = 0x0204,
    SPI3_CLK_P00_0 = 0x0300,
    SPI3_CLK_P00_2 = 0x0301,
    SPI3_CLK_P15_6 = 0x0302,
    SPI3_CLK_P15_8 = 0x0303,
    SPI3_CLK_P20_0 = 0x0304,
    SPI3_CLK_P21_5 = 0x0305,
} SPI_CLK_t;

/**
 * SPI MISO/MRST 管脚枚举
 */
typedef enum
{
    SPI0_MISO_P14_1 = 0x0000,
    SPI0_MISO_P15_3 = 0x0001,
    SPI1_MISO_P15_1 = 0x0100,
    SPI1_MISO_P15_5 = 0x0101,
    SPI1_MISO_P20_9 = 0x0102,
    SPI1_MISO_P11_10 = 0x0104,
    SPI1_MISO_P33_13 = 0x0105,
    SPI1_MISO_P02_3 = 0x0106,
    SPI2_MISO_P14_3 = 0x0200,
    SPI2_MISO_P02_1 = 0x0201,
    SPI2_MISO_P10_6 = 0x0203,
    SPI2_MISO_P33_8 = 0x0204,
    SPI2_MISO_P02_0 = 0x0206,
    SPI3_MISO_P15_7 = 0x0300,
    SPI3_MISO_P20_3 = 0x0302,
    SPI3_MISO_P00_1 = 0x0304,
    SPI3_MISO_P21_6 = 0x0305,
    SPI3_MISO_NULL = 0x0004
} SPI_MISO_t;

/**
 * SPI MOSI/MTSR 管脚枚举
 */
typedef enum
{
    SPI0_MOSI_P14_0 = 0x0000,
    SPI0_MOSI_P14_1 = 0x0001,
    SPI0_MOSI_P15_2 = 0x0002,
    SPI0_MOSI_P15_3 = 0x0003,
    SPI1_MOSI_P02_2 = 0x0100,
    SPI1_MOSI_P11_12 = 0x0101,
    SPI1_MOSI_P14_10 = 0x0102,
    SPI1_MOSI_P15_0 = 0x0103,
    SPI1_MOSI_P15_1 = 0x0104,
    SPI1_MOSI_P15_4 = 0x0105,
    SPI1_MOSI_P15_5 = 0x0106,
    SPI1_MOSI_P20_10 = 0x0107,
    SPI1_MOSI_P33_12 = 0x0108,
    SPI2_MOSI_P10_5 = 0x0200,
    SPI2_MOSI_P14_2 = 0x0201,
    SPI2_MOSI_P14_3 = 0x0202,
    SPI2_MOSI_P33_8 = 0x0203,
    SPI2_MOSI_P33_9 = 0x0204,
    SPI3_MOSI_P00_0 = 0x0300,
    SPI3_MOSI_P00_1 = 0x0301,
    SPI3_MOSI_P15_6 = 0x0302,
    SPI3_MOSI_P15_7 = 0x0303,
    SPI3_MOSI_P20_0 = 0x0304,
    SPI3_MOSI_P20_3 = 0x0305,
    SPI3_MOSI_NULL = 0x0004
} SPI_MOSI_t;

/**
 * SPI 片选管脚枚举
 */
typedef enum
{
    SPI0_CS_Null = 0x0000,
    SPI1_CS_P14_3 = 0x0100,
    SPI1_CS_P20_8 = 0x0101,
    SPI1_CS_P33_10 = 0x0102,
    SPI2_CS_P02_3 = 0x0200,
    SPI2_CS_P10_5 = 0x0201,
    SPI2_CS_P33_6 = 0x0202,
    SPI3_CS_P00_3 = 0x0300,
    SPI3_CS_P14_3 = 0x0301,
    SPI3_CS_P21_2 = 0x0302,
    SPI3_CS_P21_6 = 0x0303,
} SPI_CS_t;

/**
 * SPI 枚举
 */
typedef enum
{
    SPI0,
    SPI1,
    SPI2,
    SPI3
} SPI_t;

#define SPI_GetModule(pin) (uint8)(pin >> 8)
#define SPI_GetIndex(pin) (uint8)(pin & 0x0f)

void SPI_InitConfig(SPI_CLK_t clkPin, SPI_MISO_t misoPin, SPI_MOSI_t mosiPin, SPI_CS_t csPin, unsigned long baudrate);
uint8 SPI_ReadWriteNByte(SPI_t spi, unsigned char *txData, unsigned char *rxData, unsigned long len);

#endif /* LQ_SPI_H_ */
