/*******************************************************************************
 * @file                本文件为LQ_TC387_Software_Library 的源文件之一
 * @author              chiusir
 * @email               chiusir@163.com
 * @version             V2.0.0
 * @update              2026年3月24日
 * @copyright           版权所有 (C) 2025-2026 长秋科技有限公司
 * @website             http://www.lqist.cn
 * @taobao              http://longqiu.taobao.com
 *
 * @brief               长秋科技 LQ_TC387系列的PID控制模块
 *
 * 遵循GPL-3.0开源协议发布，本代码为TC387芯片开发板系统学习提供参考代码。可作为TC387应用开发的参考
 * 商业版;未经授权使用，使用前请联系 http://www.lqist.cn 获取授权
 *
 * 开发环境:
 *   - 开发平台 : AURIX-Studio 版本-1.10.2  (兼容ADS 1.10.x)
 *   - 文件编码 : UTF-8  (AURIX-Studio-1.10.x 默认编码)
 *   - 目标芯片 : TC387QP (TriCore? @ 300 MHz X4 10 Mbyte flash, 1568 KB of RAM)
 *   - 外部时钟 : 20MHz
 *   - 系统PLL : 300MHz + 300MHz + 300MHz + 300MHz
 * 
 * GPL-3.0 开源协议声明摘要:
 * 1. 允许自由使用、修改、分发和销售
 * 2. 分发修改后的版本时必须使用相同的协议
 * 3. 必须保留原始的版权和协议信息
 * 4. 提供任何修改都必须注明
 * 5. 开源协议的完整文本请参考项目目录 LICENSE 文件
 *******************************************************************************/

#include "LQ_PID.h"

pid_param_t locpid;
pid_param_t incpidL;
pid_param_t incpidR;
pid_param_t anglepid;
pid_param_t gyropid;

/*!
  * @brief    限幅函数
  *
  * @param    amt   输入值
  * @param    low   最小值
  * @param    high  最大值
  *
  * @return   输出值
  *
  * @note     无
  *
  * @see      无
  *
  * @date     2020/6/8
  */
float constrain_float(float amt, float low, float high)
{
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

short constrain_short(short amt, short low, short high)
{
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

/*!
  * @brief    pid位置式初始化函数
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      无
  *
  * @date     2020/6/8
  */
void PidLocInit(pid_param_t * pid)
{
    pid->kp        = LocKP;
    pid->ki        = LocKI;
    pid->kd        = LocKD;
    pid->imax      = LocIgr;
    pid->out_p     = 0;
    pid->out_i     = 0;
    pid->out_d     = 0;
    pid->out       = 0;
    pid->integrator= 0;
    pid->last_error= 0;
    pid->last_derivative   = 0;
    pid->last_t    = 0;
}

void PidIncInit(pid_param_t * pid)
{
    pid->kp        = IncKP;
    pid->ki        = IncKI;
    pid->kd        = IncKD;
    pid->imax      = IncIgr;
    pid->out_p     = 0;
    pid->out_i     = 0;
    pid->out_d     = 0;
    pid->out       = 0;
    pid->integrator= 0;
    pid->last_error= 0;
    pid->last_derivative   = 0;
    pid->last_t    = 0;
}

void PidAngleInit(pid_param_t * pid)
{
    pid->kp        = AngleKP;
    pid->ki        = AngleKI;
    pid->kd        = AngleKD;
    pid->imax      = AngleIgr;
    pid->out_p     = 0;
    pid->out_i     = 0;
    pid->out_d     = 0;
    pid->out       = 0;
    pid->integrator= 0;
    pid->last_error= 0;
    pid->last_derivative = 0;
    pid->last_t    = 0;
}

void PidGyroInit(pid_param_t * pid)
{
    pid->kp        = GyroKP;
    pid->ki        = GyroKI;
    pid->kd        = GyroKD;
    pid->imax      = GyroIgr;
    pid->out_p     = 0;
    pid->out_i     = 0;
    pid->out_d     = 0;
    pid->out       = 0;
    pid->integrator= 0;
    pid->last_error= 0;
    pid->last_derivative = 0;
    pid->last_t    = 0;
}

void PidInit(void)
{
    PidLocInit(&locpid);
    PidIncInit(&incpidL);
    PidIncInit(&incpidR);
    PidAngleInit(&anglepid);
    PidGyroInit(&gyropid);
}

/*!
  * @brief    pid位置式控制函数
  *
  * @param    pid     pid参数
  * @param    error   pid误差值
  *
  * @return   PID输出值
  *
  * @note     无
  *
  * @see      无
  *
  * @date     2020/6/8
  */
float PidLocCtrl(pid_param_t * pid, float error)
{
    /* 积分计算 */
    pid->integrator += error;

    /* 积分限幅 */
    pid->integrator = constrain_float(pid->integrator, -pid->imax, pid->imax);


    pid->out_p = pid->kp * error;
    pid->out_i = pid->ki * pid->integrator;
    pid->out_d = pid->kd * (error - pid->last_error);

    pid->last_error = error;

    pid->out = pid->out_p + pid->out_i + pid->out_d;

    return pid->out;
}


/*!
  * @brief    pid增量式控制函数
  *
  * @param    pid     pid参数
  * @param    error   pid误差值
  *
  * @return   PID输出值   注：输出值已经加上前一次输出
  *
  * @note     无
  *
  * @see      无
  *
  * @date     2020/6/8
  */
float PidIncCtrl(pid_param_t * pid, float error)
{

    pid->out_p = pid->kp * error;
    pid->out_i = pid->ki * (error - pid->last_error);
    pid->out_d = pid->kd * ((error - pid->last_error) - pid->last_derivative);

    pid->last_derivative = error - pid->last_error;
    pid->last_error = error;

    pid->out += pid->out_p + pid->out_i + pid->out_d;

    return pid->out;
}
