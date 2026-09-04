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

#ifndef _LQ_PWM_MOTOR_H_
#define _LQ_PWM_MOTOR_H_
#include "lq_include.h"

// 电机频率
#define MOTOR_FREQUENCY 10000

// 电机PWM 宏定义
#define MOTOR1_P IfxGtm_ATOM0_6_TOUT42_P23_1_OUT
#define MOTOR1_N IfxGtm_ATOM0_5_TOUT40_P32_4_OUT
#define MOTOR1_I P32_4

#define MOTOR2_P IfxGtm_ATOM0_0_TOUT53_P21_2_OUT
#define MOTOR2_N IfxGtm_ATOM0_4_TOUT50_P22_3_OUT
#define MOTOR2_I P22_3

#define MOTOR3_P IfxGtm_ATOM0_7_TOUT64_P20_8_OUT
#define MOTOR3_N IfxGtm_ATOM0_3_TOUT56_P21_5_OUT
#define MOTOR3_I P21_5

#define MOTOR4_P IfxGtm_ATOM0_2_TOUT55_P21_4_OUT
#define MOTOR4_N IfxGtm_ATOM0_1_TOUT54_P21_3_OUT
#define MOTOR4_I P21_3

// 定义模块号
typedef enum
{
    Motor1 = 0, // 电机1
    Motor2 = 1, // 电机2
    Motor3 = 2, // 电机3
    Motor4      // 电机4
} Motor_e;

typedef struct {
    float  Target_Gyro;
    float  Target_SpeedL;
    float  Target_SpeedR;
    float  Target_Yaw;
    uint8_t Angle_Mode;
    short pwmL;
    short pwmR;
} MotorState;

void MotorCtrL(Motor_e Motor, sint32 motor);
void MotorInit(void);
void Test_Motor(void);
void Test_GyroLoop(void);
void Test_AngleLoop(void);
void TR_Write_AngleLoopData(float yaw_err, float cur_yaw, float target_yaw,
                            float turn_out, uint8_t state, uint8_t step,
                            float speedL_fb, float speedR_fb);

extern uint8_t g_test_active;
extern uint8_t g_motors_enabled;
extern MotorState motorState;

void MotorState_Reset(void);
void DirectionLoop_Update(float);
void Gyro_Loop_Update(float);
void AngleLoop_Update(float);
void SpeedLoop_Update(float, float);
void Set_Target_Yaw(float);
void Set_GyroBaseSpeed(float);
float Get_GyroBaseSpeed(void);
float Get_Target_Yaw(void);
uint8_t Get_Angle_Mode(void);
float Get_Target_SpeedL(void);
float Get_Target_SpeedR(void);
float Get_pwmL(void);
float Get_pwmR(void);
void Set_Target_SpeedR(float speed);
#endif // _LQ_PWM_MOTOR_H_
