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

#ifndef __LQ_TFT2_H__
#define __LQ_TFT2_H__

#include "stdint.h"

#define TFT20W      220
#define TFT20H      176

/*******************接口定义,默认仅软件SPI ******************************/

// 龙邱TFT2 SPI口模块管脚顺序
#define TFT2SPI_CS  P20_11  // CS管脚 默认拉低，可以不用
#define TFT2SPI_SCK P20_14  // SPI SCK管脚
#define TFT2SPI_SDI P20_13  // SPI MOSI管脚
#define TFT2SPI_DC  P20_12  // D/C管脚
#define TFT2SPI_RST P20_10  // RESET管脚

// 龙邱TFT2 SPI 高电平的操作
#define TFT2SPI_CS_H    PIN_Write(TFT2SPI_CS,  1); /*!< CS管脚 */
#define TFT2SPI_SCK_H   PIN_Write(TFT2SPI_SCK, 1); /*!< SCL管脚 */
#define TFT2SPI_SDI_H   PIN_Write(TFT2SPI_SDI, 1); /*!< SDI管脚 */
#define TFT2SPI_DC_H    PIN_Write(TFT2SPI_DC,  1); /*!< DC管脚 */
#define TFT2SPI_RST_H   PIN_Write(TFT2SPI_RST, 1); /*!< RST管脚 */

#define TFT2SPI_CS_L    PIN_Write(TFT2SPI_CS,  0); /*!< CS管脚 */
#define TFT2SPI_SCK_L   PIN_Write(TFT2SPI_SCK, 0); /*!< SCL管脚 */
#define TFT2SPI_SDI_L   PIN_Write(TFT2SPI_SDI, 0); /*!< SDI管脚 */
#define TFT2SPI_DC_L    PIN_Write(TFT2SPI_DC,  0); /*!< DC管脚 */
#define TFT2SPI_RST_L   PIN_Write(TFT2SPI_RST, 0); /*!< RST管脚 */

#ifndef PRESET_U16_COLOR  /* 防止重复定义 */
#define PRESET_U16_COLOR

#define U16_BLACK       0x0000
#define U16_WHITE       0xFFFF
#define U16_RED         0xF000
#define U16_GREEN       0x0F00
#define U16_BLUE        0x00F0
#define U16_YELLOW      0xFF00
#define U16_CYAN        0x0FF0
#define U16_MAGENTA     0xF0F0
#define U16_GRAY        0x7777
#define U16_LIGHT_RED   0xF800
#define U16_LIGHT_GREEN 0x0800
#define U16_LIGHT_BLUE  0x0080
#define U16_DARK_RED    0x7000
#define U16_DARK_GREEN  0x0700
#define U16_DARK_BLUE   0x0070
#define U16_ORANGE      0xFC00

#endif

/*TFT2初始化  */
void TFT2SPI_Init(unsigned char DIR);
/* 写命令 */
void TFT2SPI_Write_Cmd(unsigned char cmd);
/*写字节  */
void TFT2SPI_Write_Byte(unsigned char dat);
/* 写半字 */
void TFT2SPI_Write_Word(unsigned short dat);
/*  重新定位输入信息位置 */
void TFT2SPI_Set_Pos(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye);
/*  填充指定区域  */
void TFT2SPI_Fill_Area(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color);
/* 全屏显示单色画面  */
void TFT2SPI_CLS(unsigned short color);
/*   填充矩形区域  */
void TFT2SPI_Draw_Part(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color_dat);
/* 画矩形边框 */
void TFT2SPI_Draw_Rectangle(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color_dat);
/* 画圆  */
void TFT2SPI_Draw_Circle(unsigned char x, unsigned char y, unsigned char r, unsigned short color_dat);
/* 画线 */
void TFT2SPI_Draw_Line(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color_dat);
/* 画点  */
void TFT2SPI_Draw_Dot(unsigned char x, unsigned char y, unsigned short color_dat);
/*  显示图片  */
void TFT2SPI_Show_Pic(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, const unsigned char *ppic);
/*  液晶字符串输出(6*8字体) */
void TFT2SPI_P6X8Str(unsigned char x, unsigned char y, char *s_dat, unsigned short word_color, unsigned short back_color);
/*  液晶字符串输出(8*8字体)*/
void TFT2SPI_P8X8Str(unsigned char x, unsigned char y, char *s_dat, unsigned short word_color, unsigned short back_color);
/*  液晶字符串输出(8*16字体)  */
void TFT2SPI_P8X16Str(unsigned char x, unsigned char y, char *s_dat, unsigned short word_color, unsigned short back_color);

/* 液晶汉字字符串输出(16*16字体) */
void TFT2SPI_P16x16Str(unsigned char x, unsigned char y, unsigned char *s_dat, unsigned short word_color, unsigned short back_color);
/*   显示图片  */
void TFT2SPI_Show_Pic2(unsigned char xs, unsigned char ys, unsigned char w, unsigned char h, const unsigned char *ppic);
/*  显示电池图标  */
void TFT2SPI_Show_Battery_Icon(void);
/*  显示标签  */
void TFT2SPI_Show_Title(void);
/*   显示龙邱LOG  */
void TFT2SPI_Show_Logo(unsigned char xs, unsigned char ys);
/*   TFT2屏 unsigned char 灰度数据显示  */
void TFT2SPI_Road(unsigned char high_start, unsigned char wide_start, unsigned char high, unsigned char wide, unsigned char *Pixle);
/* TFT2屏 unsigned char 二值化数据显示 */
void TFT2SPI_BinRoad(unsigned char high_start, unsigned char wide_start, unsigned char high, unsigned char wide, unsigned char *Pixle);
void write_command(unsigned char LCD_Reg, unsigned int LCD_RegValue);
void TFT2SPI_ShowStr(unsigned char x, unsigned char y, char *s_dat,
                     unsigned short fc, unsigned short pc, unsigned char size);

void Test_TFT2(void);

#endif
