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

#include "LQ_STM.h"

/** 外部中断CPU标号 */
const uint8 StmIrqVectabNum[2] = {STM0_VECTABNUM, STM1_VECTABNUM};

/** 外部中断优先级 */
const uint8 StmIrqPriority[4] = {STM0_CH0_PRIORITY, STM0_CH1_PRIORITY, STM1_CH0_PRIORITY, STM1_CH1_PRIORITY};

/** 外部中断服务函数地址 */
const void *StmIrqFuncPointer[4] = {&STM0_CH0_IRQHandler, &STM0_CH1_IRQHandler, &STM1_CH0_IRQHandler, &STM1_CH1_IRQHandler};

//	STM初始化配置结构体
IfxStm_CompareConfig g_StmCompareConfig[4];

/*************************************************************************
 *  函数名称：void STM_Init(STM_t stm, STM_Channel_t channel, uint32 us)
 *  功能说明：STM初始化
 *  参数说明：
 * @param    stm      ： STM标号     STM0 、 STM1
 * @param    channel  ： STM通道     STM_Channel_0  STM_Channel_1
 * @param    us       ： STM定时中断 时间
 *  函数返回：无
 *  举    例：STM_InitConfig(STM0, STM_Channel_0, 50000);  //STM0 通道0初始化 50000us 进入一次中断
 *  备    注：外部中断服务函数在LQ_STM.c中  中断优先级配置在LQ_STM.h中 可以自行修改
 *************************************************************************/
void STM_InitConfig(STM_t stm, STM_Channel_t channel, uint32 us)
{
    char interruptState;
    interruptState = disableInterrupts();

    Ifx_STM *STM = IfxStm_getAddress((IfxStm_Index)stm);
    unsigned char Index = (unsigned char)IfxStm_getIndex(STM) * 2 + (unsigned char)channel;

    IfxStm_CompareConfig *stmCompareConfig = &g_StmCompareConfig[Index];

    // 调用结构体实现配置
    IfxStm_initCompareConfig(stmCompareConfig);

    // 挂起
    IfxStm_enableOcdsSuspend(STM);

    // 获取STM模块的微秒计时 tick 数值
    sint32 ticks = IfxStm_getTicksFromMicroseconds((Ifx_STM *)STM, us);

    // 修改ticks数值，使能触发输出
    stmCompareConfig->ticks = ticks;
    stmCompareConfig->comparator = (IfxStm_Comparator)channel;
    stmCompareConfig->comparatorInterrupt = (IfxStm_ComparatorInterrupt)channel; // 中断选择
    stmCompareConfig->triggerPriority = StmIrqPriority[Index];                   // 中断优先级设置
    stmCompareConfig->typeOfService = StmIrqVectabNum[stm];                      // 服务内核

    // 比较功能初始化
    IfxStm_initCompare(STM, stmCompareConfig);
    IfxCpu_Irq_installInterruptHandler((void *)StmIrqFuncPointer[Index], StmIrqPriority[Index]); // 配置中断函数和中断号

    restoreInterrupts(interruptState);
}

/*************************************************************************
 *  函数名称：void STM_DelayUs(STM_t stm, uint32 us)
 *  功能说明：STM延时
 *  参数说明：
 * @param    stm      ： STM标号     STM0 、 STM1
 * @param    us       ： STM延时     时间 us
 *  函数返回：无
 *  举    例：STM_DelayUs(STM0, 5000);  //延时5000us
 *  备    注：使用延时函数时，请确保对应STM模块调用过STM_Init函数
 *************************************************************************/
void STM_DelayUs(STM_t stm, uint32 us)
{
    Ifx_STM *STM = IfxStm_getAddress((IfxStm_Index)stm);
    uint32 tick = IfxStm_getTicksFromMicroseconds(STM, us);

    IfxStm_waitTicks(STM, tick);
}

/*************************************************************************
 *  函数名称：void delayms(unsigned short stmms)
 *  功能说明：延时函数
 *  参数说明：无
 *  函数返回：无
 *  备    注：delayms(1);延时1 ms
 *************************************************************************/
void Delay_Ms(uint32 stmms) //
{
#if 0
    while (stmms--)
    {
        STM_DelayUs(STM0, 1000);
    }
#else // 不精确延时
    volatile unsigned long i = 0;
    while (stmms--)
    {
        for (i = 0; i < 42846; ++i)
        {
            __asm("NOP"); /* delay */
        }
    }
#endif
}

/*************************************************************************
 *  函数名称：void delayus(unsigned short us)
 *  功能说明：延时函数
 *  参数说明：无
 *  函数返回：无
 *  备    注：delayus(1);延时1 us
 *************************************************************************/
void Delay_Us(uint32 us)
{
#if 1

    STM_DelayUs(STM0, us);

#else // 不精确延时
    volatile unsigned long i = 0;
    while (us--)
    {
        for (i = 0; i < 41; ++i)
        {
            __asm("NOP"); /* delay */
        }
    }
#endif
}
/*************************************************************************
 *  函数名称：uint32  STM_GetNowUs(STM_t stm)
 *  功能说明：STM获取当前时间
 *  参数说明：
 * @param    stm      ： STM标号     STM0 、 STM1
 * @param    us       ： STM延时     时间 us
 *  函数返回：无
 *  举    例：int32_t NowTime =  STM_GetNowUs(STM0);  //获取STM0 当前时间
 *  备    注：使用延时函数时，请确保对应STM模块调用过STM_Init函数
 *************************************************************************/
uint32 STM_GetNowUs(STM_t stm)
{
    Ifx_STM *STM = IfxStm_getAddress((IfxStm_Index)stm);
    sint32 freq = (sint32)IfxStm_getFrequency(STM) / 1000000;
    Ifx_TickTime stmNow;
    char interruptState;

    interruptState = disableInterrupts();
    stmNow = (Ifx_TickTime)IfxStm_get(STM) & TIME_INFINITE;
    restoreInterrupts(interruptState);

    return (unsigned long)(stmNow / freq);
}

/*************************************************************************
 *  函数名称：STM_DisableInterrupt(STM_t stm, STM_Channel_t channel)
 *  功能说明：STM停止中断
 *  参数说明：
 * @param    stm      ： STM标号     STM0 、 STM1
 * @param    channel  ： STM通道     STM_Channel_0  STM_Channel_1
 *  函数返回：无
 *  举    例：
 *  备    注：
 *************************************************************************/
void STM_DisableInterrupt(STM_t stm, STM_Channel_t channel)
{
    Ifx_STM *STM = IfxStm_getAddress((IfxStm_Index)stm);
    IfxStm_clearCompareFlag(STM, (IfxStm_Comparator)channel);
    IfxStm_disableComparatorInterrupt(STM, (IfxStm_Comparator)channel);
}

/*************************************************************************
 *  函数名称：STM_EnableInterrupt(STM_t stm, STM_Channel_t channel)
 *  功能说明：STM使能中断
 *  参数说明：
 * @param    stm      ： STM标号     STM0 、 STM1
 * @param    channel  ： STM通道     STM_Channel_0  STM_Channel_1
 *  函数返回：无
 *  举    例：
 *  备    注：
 *************************************************************************/
void STM_EnableInterrupt(STM_t stm, STM_Channel_t channel)
{
    Ifx_STM *STM = IfxStm_getAddress((IfxStm_Index)stm);
    IfxStm_clearCompareFlag(STM, (IfxStm_Comparator)channel);
    IfxStm_enableComparatorInterrupt(STM, (IfxStm_Comparator)channel);
}
