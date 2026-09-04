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

#ifndef __LQ_DISPLAY_H__
#define __LQ_DISPLAY_H__


/* !!!注意，如果使用龙邱TFT屏和母板，则现有母板屏幕接口仅支持软件SPI，若是IPS屏则支持硬件SPI */

extern uint16_t LCD_W, LCD_H;   // LCD宽度和高度，初始化设置屏幕类型时被更改,主要用于IPS LCD
/* 枚举屏幕 Dir 屏幕方向参数0-3，旋转0-270度，步进90度 */

typedef enum
{
    Screen_Dir0 = 0u, // 竖屏，排线在屏幕的下侧
    Screen_Dir1 = 1u, // 竖屏，排线在屏幕的上侧
    Screen_Dir2 = 2u, // 横屏，排线在屏幕的右侧
    Screen_Dir3 = 3u  // 横屏，排线在屏幕的左侧
}show_dir_t;


// 初始化
void Display_Init(show_dir_t Dir);
// 清屏函数
void Display_CLS(unsigned int color);
// 填充矩形
void Display_Fill(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color);
// 画点
void Display_DrawPoint(unsigned int x, unsigned int y, unsigned int color);
// 输出字符
void Display_showString(unsigned int x, unsigned int y, char *num, unsigned int fc, unsigned int bc, unsigned char sizey);
// 画线
void Display_DrawLine(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color);
// 画矩形
void Display_DrawRectangle(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color);
// 显示图片
void Display_Show(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned char *pic);
// 显示汉字
void Display_ShowChinese(unsigned int x, unsigned int y, char *s, unsigned int fc, unsigned int bc, unsigned char sizey);
// 显示LOGO
void Display_Show_Logo(unsigned char xs, unsigned char ys);
// 画圆
void Display_DrawCircle(unsigned char x, unsigned char y, unsigned char r, unsigned short color);
// 显示测试
void Test_Display(void);
#if MENU_ENABLE
void Display_Menu(void);
#endif
#endif
