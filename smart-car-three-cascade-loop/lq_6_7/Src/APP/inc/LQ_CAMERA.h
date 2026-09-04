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



#ifndef __LQ_CAMERA_H
#define __LQ_CAMERA_H
#include "config.h"
#include "LQ_MT9V034.h"

#define LQMT9V034 // 默认为神眼摄像头
#define USETFT1_8

#define IMAGEH MT9V034_IMAGEH /*!< 摄像头采集高度 */
#define IMAGEW MT9V034_IMAGEW /*!< 摄像头采集宽度 */

#define Image_WHITE 255
#define Image_BLACK 0

/* 使用数组宽高 修改这里即可 */
#ifdef TFT18
#define LCDH 120 /*!< TFT显示高度（用户使用）高度 */
#define LCDW 160 /*!< TFT显示宽度（用户使用）宽度 */
#else
#define LCDH  60 /*!< TFT显示高度（用户使用）高度 */
#define LCDW 94 /*!< TFT显示宽度（用户使用）宽度 */
#endif

/** 图像原始数据存放 */
extern unsigned char Image_Data[IMAGEH][IMAGEW];

/** 压缩后之后用于存放屏幕显示数据  */
extern unsigned char Image_Use[LCDH][LCDW];

/** 二值化后用于OLED显示的数据 */
extern unsigned char Pixle[LCDH][LCDW];

/** 二值化阈值 */
extern unsigned char Threshold;
/** 基础阈值（初始化50帧OTSU均值，脱轨保护用） */
extern unsigned char Base_Threshold;

void CameraInt(void);
void CAMERA_Reprot(void);                       // 串口上报上位机
void CAMERA_Init(unsigned char fps);            // 摄像头测试例程
void Get_Use_Image(void);                       // 获取需要使用的图像数组
void Get_Bin_Image(void);                         // 动态阈值二值化
void Image_Filter(unsigned char threshold);       // 并查集8连通域滤波，threshold为连通域像素数阈值
int GetOSTU(unsigned char tmImage[LCDH][LCDW]); // 大津法求阈值大小
unsigned int get_last_variance(void);             // 获取上一帧计算的图像方差
unsigned int get_var_threshold(void);             // 获取自动标定的方差阈值
void Test_CAMERA(void);                         // 摄像头测试例程
// 基于soble边沿检测算子的一种边沿检测
void lq_sobel(unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW], unsigned char Threshold);
// 基于soble边沿检测算子的一种自动阈值边沿检测
void lq_sobelAutoThreshold(unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW]);

#endif
