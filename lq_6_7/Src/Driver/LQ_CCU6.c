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

#include "LQ_CCU6.h"

/** CCU6中断CPU标号 */
const uint8 Ccu6IrqVectabNum[2] = {CCU60_VECTABNUM, CCU61_VECTABNUM};

/** CCU6中断优先级 */
const uint8 Ccu6IrqPriority[4] = {CCU60_CH0_PRIORITY, CCU60_CH1_PRIORITY, CCU61_CH0_PRIORITY, CCU61_CH1_PRIORITY};

/** CCU6中断服务函数地址 */
const void *Ccu6IrqFuncPointer[4] = {&CCU60_CH0_IRQHandler, &CCU60_CH1_IRQHandler, &CCU61_CH0_IRQHandler, &CCU61_CH1_IRQHandler};

/*************************************************************************
 *  函数名称：CCU6_InitConfig CCU6
 *  功能说明：定时器周期中断初始化
 *  参数说明：ccu6    ： ccu6模块            CCU60 、 CCU61
 *  参数说明：channel ： ccu6模块通道  CCU6_Channel0 、 CCU6_Channel1
 *  参数说明：us      ： ccu6模块  中断周期时间  单位us
 *  函数返回：无
 *  备    注：    CCU6_InitConfig(CCU60, CCU6_Channel0, 100);  //100us进入一次中断
 *************************************************************************/
void CCU6_InitConfig(CCU6_t ccu6, CCU6_Channel_t channel, uint32 us)
{
    IfxCcu6_Timer_Config timerConfig;
    IfxCcu6_Timer Ccu6Timer;

    Ifx_CCU6 *module = IfxCcu6_getAddress((IfxCcu6_Index)ccu6);
    uint8 Index = ccu6 * 2 + channel;
    uint32 period = 0;
    uint64 clk = 0;

    /* 关闭中断 */
    boolean interrupt_state = disableInterrupts();

    IfxCcu6_Timer_initModuleConfig(&timerConfig, module);
    clk = IfxScuCcu_getSpbFrequency();

    /* 设置时钟频率  */
    uint8 i = 0;
    while (i++ < 16)
    {
        period = (uint32)(clk * us / 1000000);
        if (period < 0xffff)
        {
            break;
        }
        else
        {
            clk = clk / 2;
        }
    }

    switch (channel)
    {
    case CCU6_Channel0:
        timerConfig.base.t12Frequency = (float)clk;
        timerConfig.base.t12Period = period; // 设置定时中断
        timerConfig.timer = IfxCcu6_TimerId_t12;
        timerConfig.interrupt1.source = IfxCcu6_InterruptSource_t12PeriodMatch;
        timerConfig.interrupt1.priority = Ccu6IrqPriority[Index];
        timerConfig.interrupt1.typeOfService = Ccu6IrqVectabNum[ccu6];
        timerConfig.interrupt1.serviceRequest = IfxCcu6_ServiceRequest_1;
        timerConfig.clock.t12countingInputMode = IfxCcu6_CountingInputMode_internal;
        timerConfig.timer12.counterValue = 0;
        break;

    case CCU6_Channel1:
        timerConfig.base.t13Frequency = (float)clk;
        timerConfig.base.t13Period = period;
        timerConfig.timer = IfxCcu6_TimerId_t13;
        timerConfig.interrupt2.source = IfxCcu6_InterruptSource_t13PeriodMatch;
        timerConfig.interrupt2.priority = Ccu6IrqPriority[Index];
        timerConfig.interrupt2.typeOfService = Ccu6IrqVectabNum[ccu6];
        timerConfig.interrupt2.serviceRequest = IfxCcu6_ServiceRequest_2;
        timerConfig.clock.t13countingInputMode = IfxCcu6_CountingInputMode_internal;
        timerConfig.timer13.counterValue = 0;
        break;
    }

    timerConfig.trigger.t13InSyncWithT12 = FALSE;
    IfxCcu6_Timer_initModule(&Ccu6Timer, &timerConfig);
    IfxCpu_Irq_installInterruptHandler((void *)Ccu6IrqFuncPointer[Index], Ccu6IrqPriority[Index]); // 配置中断函数和中断号
    restoreInterrupts(interrupt_state);
    IfxCcu6_Timer_start(&Ccu6Timer);
}

/*************************************************************************
 *  函数名称：CCU6_DisableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel)
 *  功能说明：停止CCU6通道中断
 *  参数说明：ccu6    ： ccu6模块            CCU60 、 CCU61
 *  参数说明：channel ： ccu6模块通道  CCU6_Channel0 、 CCU6_Channel1
 *  函数返回：无
 *  备    注：
 *************************************************************************/
void CCU6_DisableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel)
{
    Ifx_CCU6 *module = IfxCcu6_getAddress((IfxCcu6_Index)ccu6);
    IfxCcu6_clearInterruptStatusFlag(module, (IfxCcu6_InterruptSource)(7 + channel * 2));
    IfxCcu6_disableInterrupt(module, (IfxCcu6_InterruptSource)(7 + channel * 2));
}

/*************************************************************************
 *  函数名称：CCU6_DisableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel)
 *  功能说明：使能CCU6通道中断
 *  参数说明：ccu6    ： ccu6模块            CCU60 、 CCU61
 *  参数说明：channel ： ccu6模块通道  CCU6_Channel0 、 CCU6_Channel1
 *  函数返回：无
 *  备    注：
 *************************************************************************/
void CCU6_EnableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel)
{
    Ifx_CCU6 *module = IfxCcu6_getAddress((IfxCcu6_Index)ccu6);
    IfxCcu6_clearInterruptStatusFlag(module, (IfxCcu6_InterruptSource)(7 + channel * 2));
    IfxCcu6_enableInterrupt(module, (IfxCcu6_InterruptSource)(7 + channel * 2));
}