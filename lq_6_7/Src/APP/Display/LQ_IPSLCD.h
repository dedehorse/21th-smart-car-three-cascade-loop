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

#ifndef __LQ_IPSLCD_H__
#define __LQ_IPSLCD_H__

#include "stdint.h"
#include "IfxQspi_PinMap.h"
#include "LQ_Display.h"
#include "config.h"


extern uint16_t LCD_W, LCD_H;   // LCD宽度和高度，初始化设置屏幕类型时被更改,主要用于IPS LCD

/******************* 接口定义 直接调用官方底层库内容，提高响应速度 ******************************/
// 龙邱IPS 模块管脚定义
#ifndef IPS_PIN   /* 龙邱IPS 模块管脚定义 */
#define IPS_PIN

#if(1 == USE_HARD_SPI)   /* 使用硬件高速QSPI 总线最高50MHz,IPS最高40MHz*/
#define IPS_QSPI_index      IfxQspi_Index_0  /*QSPI0*/
#define IPS_SPI_MODE    	QSPI_MODE_0
#define IPS_SCLK        	IfxQspi0_SCLK_P20_11_OUT    /* 从 IfxQspi_PinMap.h 里边选择 */
#define IPS_MOSI        	IfxQspi0_MTSR_P20_14_OUT
#define IPS_MISO        	IfxQspi0_MRSTA_P20_12_IN
#define IPS_SLSO        	IfxQspi0_SLSO6_P20_10_OUT   /* 如果没有合适的CS选择，可以使用空引脚占位，软件定义 */
#else   /* 软件SPI */
#define IPS_CK          P20_11  // IPS CK管脚
#define IPS_DI          P20_14  // IPS DI管脚

#define IPS_DI_H        PIN_Write(IPS_DI, 1)
#define IPS_DI_L        PIN_Write(IPS_DI, 0)
#define IPS_CK_H        PIN_Write(IPS_CK, 1)
#define IPS_CK_L        PIN_Write(IPS_CK, 0)
#endif

// 管脚操作宏定义
#define IPS_RST         P20_13 // IPS RST管脚
#define IPS_DC          P20_12  // IPS DC管脚
#define IPS_CS          P20_10  // IPS CS管脚

#define IPS_DC_H        PIN_Write(IPS_DC, 1)
#define IPS_DC_L        PIN_Write(IPS_DC, 0)
#define IPS_RST_H       PIN_Write(IPS_RST, 1)
#define IPS_RST_L       PIN_Write(IPS_RST, 0)
#define IPS_CS_H        PIN_Write(IPS_CS, 1)
#define IPS_CS_L        PIN_Write(IPS_CS, 0)

#endif    /* IPS_PIN龙邱IPS 模块管脚定义 */

// 16位RGB颜色常用预设
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


/* 设置屏幕宽高 */
void LCD_set_WH(show_dir_t Dir);
/*写入命令*/
void LCD_Write_Cmd(unsigned char cmd);
/*写入8位数据*/
void LCD_Write_Byte(unsigned char dat);
/*写入16位数据*/
void LCD_Write_Word(unsigned int dat);
/*设置起始和结束地址*/
void LCD_Address_Set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

/*LQ IPS 屏幕初始化*/
void LCD_Init(show_dir_t Dir);
/*在指定位置画点*/
void LCD_DrawPoint(unsigned int x, unsigned int y, unsigned int color);
/*指定位置填充指定颜色*/
void LCD_Fill(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color);
/*清屏*/
void LCD_CLS(unsigned int color);
/*画线*/
void LCD_Draw_Line(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color_dat);
/*画矩形框*/
void LCD_Draw_Rectangle(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color_dat);
/*画圆*/
void LCD_Draw_Circle(unsigned char x, unsigned char y, unsigned char r, unsigned short color_dat);
/*显示图片*/
void LCD_Show_Picture(unsigned int x, unsigned int y, unsigned int length, unsigned int width, const unsigned char pic[]);
void LCD_Show_Logo(unsigned char xs, unsigned char ys);
/*显示单个字符*/
void LCD_ShowChar(unsigned int x, unsigned int y, unsigned char num, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);
/*显示字符串*/
void LCD_ShowString(unsigned int x, unsigned int y, char *p, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);
/*显示12*6大小的字符串*/
void LCD_ShowStr_12(unsigned int x, unsigned int y, char *p, unsigned int fc, unsigned int bc);
/*显示16*8大小的字符串*/
void LCD_ShowStr_16(unsigned int x, unsigned int y, char *p, unsigned int fc, unsigned int bc);
/*显示24*12大小的字符串*/
void LCD_ShowStr_24(unsigned int x, unsigned int y, char *p, unsigned int fc, unsigned int bc);
/*显示32*16大小的字符串*/
void LCD_ShowStr_32(unsigned int x, unsigned int y, char *p, unsigned int fc, unsigned int bc);
/*显示汉字*/
void LCD_ShowChinese(unsigned int x, unsigned int y, char *s, unsigned int fc, unsigned int bc, unsigned char sizey);
void LCDSPI_Road(unsigned char wide_start, unsigned char high_start, unsigned char high, unsigned char wide, unsigned char *Pixle);

//void Test_IPSLCD(void); // 2025——V7.0 统一为 Test_Display();

#endif /*LQ_IPSLCD.h*/
