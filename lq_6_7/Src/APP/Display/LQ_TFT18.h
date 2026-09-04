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

#ifndef __LQ_TFT18_H__
#define __LQ_TFT18_H__

#include "stdint.h"
#include "LQ_SPI.h"
#include "LQ_QSPI.h"
/*******************接口定义，修改时注意 ******************************/

#define TFTUSE_QSPI        (0)      // 1: 使用硬件QSPI,0:使用普通SPI或软件SPI

// 龙邱TFT18 SPI口模块管脚顺序
/* 软硬件公共 引脚定义 */
#define TFTSPI_CS       P20_11  // IPS CS管脚
#define TFTSPI_DC       P20_12  // IPS DC管脚
#define TFTSPI_RST      P20_10  // IPS RST管脚

#if(1==TFTUSE_QSPI && 1 == USE_HARD_SPI)     /* 使用硬件QSPI */
#define TFT_QSPI_PORT        QSPI0
#define TFT_QSPI_SCLK        QSPI0_CLK_P20_11    /* 从 LQ_QSPI.h 里边选择 */
#define TFT_QSPI_MOSI        QSPI0_MOSI_P20_14
#define TFT_QSPI_MISO        QSPI0_MISO_P20_12
#define TFT_QSPI_SLSO        QSPI0_CS_P20_10
#elif(1 == USE_HARD_SPI)                    /* 使用硬件普通SPI引脚选择 */
#define TFTSPI_PORT     SPI3
#define TFTSPI_SCLK     SPI3_CLK_P00_2      /* 从LQ_SPI.h 的SPI引脚枚举里边选择 */
#define TFTSPI_MOSI     SPI3_MOSI_P00_1
#define TFTSPI_MISO     SPI3_MISO_NULL      /*表示CS不使用硬件挂载的，而使用软件定义的CS*/
#define TFTSPI_SLSO     SPI3_CS_P21_2       /*如果没有合适的CS选择，可以使用空引脚占位，软件定义*/
#else /* 否则软件SPI */
#define TFTSPI_SCK      P20_14  // IPS CK管脚
#define TFTSPI_SDI      P20_13  // IPS DI管脚

#define TFTSPI_SCK_H    PIN_Write(TFTSPI_SCK, 1);   /*!< SCL管脚 */
#define TFTSPI_SDI_H    PIN_Write(TFTSPI_SDI, 1);   /*!< SDI管脚 */
#define TFTSPI_SCK_L    PIN_Write(TFTSPI_SCK, 0);   /*!< SCL管脚 */
#define TFTSPI_SDI_L    PIN_Write(TFTSPI_SDI, 0);   /*!< SDI管脚 */
#endif /* IF使用硬件SPI */

#define TFTSPI_DC_H     PIN_Write(TFTSPI_DC, 1);    /*!< DC管脚 */
#define TFTSPI_DC_L     PIN_Write(TFTSPI_DC, 0);    /*!< DC管脚 */
#define TFTSPI_RST_H    PIN_Write(TFTSPI_RST, 1);   /*!< RST管脚 */
#define TFTSPI_RST_L    PIN_Write(TFTSPI_RST, 0);   /*!< RST管脚 */
#define TFTSPI_CS_H     PIN_Write(TFTSPI_CS, 1);    /*!< CS管脚 */
#define TFTSPI_CS_L     PIN_Write(TFTSPI_CS, 0);    /*!< CS管脚 */


/* tft18的屏幕固定窗口大小 */
#define TFT18W          162
#define TFT18H          132


#ifndef PRESET_U16_COLOR
#define PRESET_U16_COLOR
#define U16_BLACK       	0x0000
#define U16_WHITE       	0xFFFF
#define U16_RED         	0xF800
#define U16_GREEN       	0x0F00
#define U16_BLUE        	0x00F0
#define U16_YELLOW      	0xFF0F
#define U16_CYAN        	0x0FF0
#define U16_MAGENTA     	0xF0F0
#define U16_GRAY        	0x7777
#define U16_LIGHT_RED   	0xF800
#define U16_LIGHT_GREEN 	0x0800
#define U16_LIGHT_BLUE  	0x0080
#define U16_DARK_RED    	0x7000
#define U16_DARK_GREEN  	0x0700
#define U16_DARK_BLUE   	0x0070
#define U16_ORANGE      	0xFC00
#endif

void TFTSPI_Init(show_dir_t Dir);
void TFTSPI_Write_Cmd(unsigned char cmd);
void TFTSPI_Write_Byte(unsigned char dat);
void TFTSPI_Write_Word(unsigned short dat);
void TFTSPI_Set_Pos(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye);
void TFTSPI_Fill_Area(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color);
void TFTSPI_CLS(unsigned short color);
void TFTSPI_Draw_Dot(unsigned char x, unsigned char y, unsigned short color_dat);
void TFTSPI_Draw_Part(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color_dat);
void TFTSPI_Draw_Rectangle(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color_dat);
void TFTSPI_Draw_Circle(unsigned char x, unsigned char y, unsigned char r, unsigned short color_dat);
void TFTSPI_Draw_Line(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color_dat);
void TFTSPI_Show_Pic(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, const unsigned char *ppic);
void TFTSPI_P6X8Str(unsigned char x, unsigned char y, char *s_dat, unsigned short word_color, unsigned short back_color);
void TFTSPI_P8X8Str(unsigned char x, unsigned char y, char *s_dat, unsigned short word_color, unsigned short back_color);
void TFTSPI_P8X16Str(unsigned char x, unsigned char y, char *s_dat, unsigned short word_color, unsigned short back_color);
void TFTSPI_P16x16Str(unsigned char x, unsigned char y, unsigned char *s_dat, unsigned short word_color, unsigned short back_color);
void TFTSPI_Show_Pic2(unsigned char xs, unsigned char ys, unsigned char w, unsigned char h, const unsigned char *ppic);
void TFTSPI_Show_Battery_Icon(void);
void TFTSPI_Show_Title(void);
void TFTSPI_Show_Logo(unsigned char xs, unsigned char ys);
void TFTSPI_Road(unsigned char wide_start, unsigned char high_start, unsigned char high, unsigned char wide, unsigned char *Pixle);
void TFTSPI_BinRoad(unsigned char wide_start, unsigned char high_start, unsigned char high, unsigned char wide, unsigned char *Pixle);
void TFTSPI_ShowBeacon(void);
void TFTSPI_ShowStr(unsigned char x, unsigned char y, char *s_dat,
                    unsigned short fc, unsigned short pc, unsigned char size);
void Test_TFT18(void);

#endif /*SGP18T_ILI9163B.h*/
