/*******************************************************************************
 * @file                本文件是LQ_TC387_Software_Library 软件开源库文件的一部分
 * @author              chiusir
 * @email               chiusir@163.com
 * @version             V2.0.0
 * @update              2026年3月24日
 * @copyright           版权所有 (C) 2025-2026 北京龙邱科技有限公司
 * @website             http://www.lqist.cn
 * @taobao              http://longqiu.taobao.com
 *
 * @brief               龙邱科技 LQ_TC387核心板驱动库声明
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
#ifndef _LQ_QUAT_H_
#define _LQ_QUAT_H_
#define M_PI 3.14159265358979323846

#include "lq_include.h"
typedef struct
{
    short accX;
    short accY;
    short accZ;
    short gyroX;
    short gyroY;
    short gyroZ;
} ICM_DATA;

typedef struct
{
    float roll;
    float pitch;
    float yaw;
} ICM_Angle;

extern ICM_DATA icm_data;

extern ICM_DATA icm_offset;

extern ICM_Angle icm_angle;

extern float j_z;    // 当前Z轴角速度 (rad/s)
extern float j_a;    // 当前Z轴角加速度 (rad/s^2)

void IMU_GetOffset(ICM_DATA *offset);

// 姿态更新函数
void IMUupdate(ICM_DATA *pMpu, ICM_Angle *pAngle);

// 获取姿态数据
void Attitude_get(void);

// 获取角速度与角加速度
float Get_Current_Gyro(void);
float Get_Current_GyroAccel(void);
float Get_Current_Yaw(void);

void Test_Quat(void);

#endif // _LQ_QUAT_H_
