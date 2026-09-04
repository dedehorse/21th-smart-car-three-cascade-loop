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
#ifndef SRC_APPSW_TRICORE_USER_LQ_PID_H_
#define SRC_APPSW_TRICORE_USER_LQ_PID_H_

#include "lq_include.h"

typedef struct
{
    float                kp;         //P
    float                ki;         //I
    float                kd;         //D
    float                imax;       //积分限幅

    float                out_p;  //KP输出
    float                out_i;  //KI输出
    float                out_d;  //KD输出
    float                out;    //pid输出

    float                integrator; //< 积分值
    float                last_error; //< 上次误差
    float                last_derivative;//< 上次微分(上次误差与上上次误差之差)
    unsigned long        last_t;     //< 上次时间
}pid_param_t;

extern pid_param_t locpid;
extern pid_param_t incpidL;
extern pid_param_t incpidR;
extern pid_param_t anglepid;
extern pid_param_t gyropid;

void PidInit(void);
void PidAngleInit(pid_param_t *pid);
void PidGyroInit(pid_param_t *pid);

float constrain_float(float amt, float low, float high);
short constrain_short(short amt, short low, short high);

float PidLocCtrl(pid_param_t * pid, float error);
float PidIncCtrl(pid_param_t * pid, float error);

#endif /* SRC_APPSW_TRICORE_USER_LQ_PID_H_ */
