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

#ifndef _LQ_PWM_BLSMOTOR_H_
#define _LQ_PWM_BLSMOTOR_H_

#include "lq_include.h"

#define Blsmotor_Max                1510     // PWM最大值
#define Blsmotor_Min                510      // PWM最小值

#define Blsmotor_Frequency          50 // PWM频率
#define ATOMBLS1 IfxGtm_ATOM0_5_TOUT15_P00_6_OUT
#define ATOMBLS2 IfxGtm_ATOM1_6_TOUT16_P00_7_OUT
#define ATOMBLS3 IfxGtm_ATOM0_0_TOUT53_P21_2_OUT  // MINI 负压风机pwm

// 定义模块号
typedef enum
{
    BLS1   = 0,  // 电调1
    BLS2   = 1,  // 电调2
    BLS3   = 2,  // 电调3，MINI负压风机
    BLSALL = 3   // 电调1&2

} Bls_e;

void BLSmotorInit(void);
void BlsmotorCtrl(Bls_e BLS, uint32 duty);
void BLS3_RampStartup(void);
void Test_BlsMotor(void);
#endif
