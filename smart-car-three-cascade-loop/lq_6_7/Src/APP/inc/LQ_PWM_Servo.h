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

#ifndef _LQ_PWM_SERRVO_H_
#define _LQ_PWM_SERRVO_H_
#include "lq_include.h"

#define Servo_Delta 100                                  // 舵机左右转动的差值，与舵机型号，拉杆和舵机臂长有关
#define Servo_Center_Mid 760                             // 舵机直行中值， 710
#define Servo_Left_Max (Servo_Center_Mid + Servo_Delta)  // 舵机左转极限值
#define Servo_Right_Min (Servo_Center_Mid - Servo_Delta) // 舵机右转极限值，此值跟舵机放置方式有关，立式
#define Servo_Frequency 50                               // 舵机PWM频率

#define ATOMSERVO1 IfxGtm_ATOM2_0_TOUT32_P33_10_OUT
#define ATOMSERVO2 IfxGtm_ATOM2_5_TOUT35_P33_13_OUT

// 定义模块号
typedef enum
{
    Servo1 = 0,  // 舵机1
    Servo2 = 1,  // 舵机2
    ServoALL = 2 // 舵机1&2

} Servo_e;

void ServoInit(void);
void ServoCtrl(Servo_e Servo, uint32 duty);
void Test_Servo(void);
#endif // _LQ_PWM_SERRVO_H_
