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

#ifndef _LQ_Track_H_
#define _LQ_Track_H_
#include "lq_include.h"

#define TRACK_CHANNEL_NUM (16)
extern unsigned int LQ_Tracking_Value[TRACK_CHANNEL_NUM];

#define TRACK_S3_PIN     ( P01_3  )
#define TRACK_S2_PIN     ( P02_10 )
#define TRACK_S1_PIN     ( P00_12 )
#define TRACK_S0_PIN     ( P00_10 )

void Tracking_init(void);
unsigned int Tracking_Value_once(unsigned char ch);
void Tracking_Value_Acquire(void);
void Test_Tracking(void);

void Dig_Binary(void);
void Dig_Tracking(void);
void Show_Bin(void);
float Get_error(void);

#define SCAN_NEAR_MARGIN  -30
#define SCAN_FAR_MARGIN   10
#define SCAN_DELTA        5
#define INVALID_BORDER    0xFF

#define CLAMP(x, min, max) ((x) <= (min) ? (min) : ((x) >= (max) ? (max) : (x)))
#define SWAP(type, a, b)  do { type temp = (a); (a) = (b); (b) = temp; } while(0)

#define White_Point 255
#define TOP_POINT   25
#define T_JUNCTION_TOP  10   /* T字路口top_point上限: 低于此值→中心线贯穿, T字路口; 高于→转弯/十字 */
#define ADD_LINE      3   /* 普通转弯拉线起始点距边界的距离 */
#define TURN_MARGIN  10   /* T字路口拉线起始点距边界的距离(仅用于左T/右T) */
#define LEFT_POINT_THRESHOLD   15   /* 左顶点阈值: 左边界最小列号低于此值→左侧有横向分支延伸到边界 */
#define RIGHT_POINT_THRESHOLD  78   /* 右顶点阈值: 右边界最大列号高于此值→右侧有横向分支延伸到边界 */
                                     /* (LCDW=94时, 94-1-15=78; LCDW=160时改为25/134) */

void ScanLimit_Init(void);
void Track_Process(void);
void DisplayTrack(void);
float Track_GetDiff(void);
void Track_DebugDisplay(uint16_t fps);
void Track_SetFPS(uint16_t fps);  /* 设置当前帧率, 由主循环每秒调用 */
unsigned char detect_t_by_jumps(void);   /* 跳变检测法: 4=左T 5=右T 3=对称T 0=非T字 */
void TR_Write_JumpDebug(void);           /* WiFi发送跳变检测调试数据, 每秒调用 */

extern double cha_l, cha_r;
extern double var_l, var_r;
extern double std_l, std_r;
extern double center_std;
extern uint8_t g_valid_border_rows;
void Track_OffTrackCheck(void);

/* 边界数组: Track_Process 计算，CPU0 主循环用于无线图像叠加 */
extern unsigned char left_border[];
extern unsigned char right_border[];
extern unsigned char center_line[];
extern unsigned char top_point;
extern unsigned char bottom_point;
extern uint16_t s_fps;

/* 边界叠加灰度值 */
#define GRAY_RED    255
#define GRAY_GREEN  255
#define GRAY_YELLOW 255

void border_statistics(void);
void DrawImgPointBuf(unsigned char x, unsigned char y,
                     unsigned char gray, unsigned char *buf, int stride);
void TR_Write_TrackStats(void);

#endif
