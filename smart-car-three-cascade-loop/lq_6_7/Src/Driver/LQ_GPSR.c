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

#include "LQ_GPSR.h"

/** cpu软件中断优先级 */
const uint8 CpuSoftIrqPriority[12] = {
    CPU0_SOFT_IRQ0_PRIORITY, CPU0_SOFT_IRQ1_PRIORITY, CPU0_SOFT_IRQ2_PRIORITY, CPU0_SOFT_IRQ3_PRIORITY,
    CPU1_SOFT_IRQ0_PRIORITY, CPU1_SOFT_IRQ1_PRIORITY, CPU1_SOFT_IRQ2_PRIORITY, CPU1_SOFT_IRQ3_PRIORITY,
    CPU2_SOFT_IRQ0_PRIORITY, CPU2_SOFT_IRQ1_PRIORITY, CPU2_SOFT_IRQ2_PRIORITY, CPU2_SOFT_IRQ3_PRIORITY};

/**
 * @brief    CPU0_SOFT_IRQ0Handler中断服务函数
 * @note     cpu0 软件中断0中断服务函数
 */
void CPU0_SOFT_IRQ0Handler(void)
{
    SRC_GPSR00.B.SRR = 0;
    /* 用户代码 */
}

void CPU0_SOFT_IRQ1Handler(void)
{
    SRC_GPSR01.B.SRR = 0;
    /* 用户代码 */
}

void CPU0_SOFT_IRQ2Handler(void)
{
    SRC_GPSR02.B.SRR = 0;
    /* 用户代码 */
}

void CPU0_SOFT_IRQ3Handler(void)
{
    SRC_GPSR03.B.SRR = 0;
    /* 用户代码 */
}

void CPU1_SOFT_IRQ0Handler(void)
{
    SRC_GPSR04.B.SRR = 0;
    /* 用户代码 */
}

void CPU1_SOFT_IRQ1Handler(void)
{
    SRC_GPSR05.B.SRR = 0;
    /* 用户代码 */
}

void CPU1_SOFT_IRQ2Handler(void)
{
    SRC_GPSR06.B.SRR = 0;
    /* 用户代码 */
}

void CPU1_SOFT_IRQ3Handler(void)
{
    SRC_GPSR07.B.SRR = 0;
    /* 用户代码 */
}

void CPU2_SOFT_IRQ0Handler(void)
{
    SRC_GPSR10.B.SRR = 0;
    /* 用户代码 */
}

void CPU2_SOFT_IRQ1Handler(void)
{
    SRC_GPSR11.B.SRR = 0;
    /* 用户代码 */
}

void CPU2_SOFT_IRQ2Handler(void)
{
    SRC_GPSR12.B.SRR = 0;
    /* 用户代码 */
}

void CPU2_SOFT_IRQ3Handler(void)
{
    SRC_GPSR13.B.SRR = 0;
    /* 用户代码 */
}
/* cpu软中断 */
IFX_INTERRUPT(CPU0_SOFT_IRQ0Handler, 0, CPU0_SOFT_IRQ0_PRIORITY);
IFX_INTERRUPT(CPU0_SOFT_IRQ1Handler, 0, CPU0_SOFT_IRQ1_PRIORITY);
IFX_INTERRUPT(CPU0_SOFT_IRQ2Handler, 0, CPU0_SOFT_IRQ2_PRIORITY);
IFX_INTERRUPT(CPU0_SOFT_IRQ3Handler, 0, CPU0_SOFT_IRQ3_PRIORITY);

IFX_INTERRUPT(CPU1_SOFT_IRQ0Handler, 1, CPU1_SOFT_IRQ0_PRIORITY);
IFX_INTERRUPT(CPU1_SOFT_IRQ1Handler, 1, CPU1_SOFT_IRQ1_PRIORITY);
IFX_INTERRUPT(CPU1_SOFT_IRQ2Handler, 1, CPU1_SOFT_IRQ2_PRIORITY);
IFX_INTERRUPT(CPU1_SOFT_IRQ3Handler, 1, CPU1_SOFT_IRQ3_PRIORITY);

IFX_INTERRUPT(CPU2_SOFT_IRQ0Handler, 1, CPU2_SOFT_IRQ0_PRIORITY);
IFX_INTERRUPT(CPU2_SOFT_IRQ1Handler, 1, CPU2_SOFT_IRQ1_PRIORITY);
IFX_INTERRUPT(CPU2_SOFT_IRQ2Handler, 1, CPU2_SOFT_IRQ2_PRIORITY);
IFX_INTERRUPT(CPU2_SOFT_IRQ3Handler, 1, CPU2_SOFT_IRQ3_PRIORITY);
/** cpu软中断服务函数地址 */
const void *CpuSoftIrqFuncPointer[12] = {&CPU0_SOFT_IRQ0Handler, &CPU0_SOFT_IRQ1Handler, &CPU0_SOFT_IRQ2Handler,
                                         &CPU0_SOFT_IRQ3Handler, &CPU1_SOFT_IRQ0Handler, &CPU1_SOFT_IRQ1Handler, &CPU1_SOFT_IRQ2Handler,
                                         &CPU1_SOFT_IRQ3Handler, &CPU2_SOFT_IRQ0Handler, &CPU2_SOFT_IRQ1Handler, &CPU2_SOFT_IRQ2Handler,
                                         &CPU2_SOFT_IRQ3Handler};

/*************************************************************************
 *  函数名称：GPSR_InitConfig(IfxSrc_Tos cpu, uint8 softIrq)
 *  功能说明：软件中的初始化
 *  参数说明：cpu     ： IfxSrc_Tos_cpu0   IfxSrc_Tos_cpu1
 *  参数说明：softIrq ： 每个CPU有4个软件中断, irq序号
 *  函数返回：无
 *  修改时间：2020年5月6日
 *  备    注：
 *************************************************************************/
void GPSR_InitConfig(IfxSrc_Tos cpu, SOFT_IRQ index)
{
    /* 关闭中断 */
    boolean interruptState = IfxCpu_disableInterrupts();

    uint8 temp = cpu * 4 + index;
    SRC_GPSR00.B.SRR = 0;
    SRC_GPSR01.B.SRR = 0;
    SRC_GPSR02.B.SRR = 0;
    SRC_GPSR03.B.SRR = 0;

    SRC_GPSR04.B.SRR = 0;
    SRC_GPSR05.B.SRR = 0;
    SRC_GPSR06.B.SRR = 0;
    SRC_GPSR07.B.SRR = 0;

    SRC_GPSR10.B.SRR = 0;
    SRC_GPSR11.B.SRR = 0;
    SRC_GPSR12.B.SRR = 0;
    SRC_GPSR13.B.SRR = 0;

    /* 开启中断 */
    volatile Ifx_SRC_SRCR *src = &MODULE_SRC.GPSR.GPSR[(int)cpu].SR[0];
    IfxSrc_init(src, (int)cpu, (uint32)CpuSoftIrqPriority[temp]);
    IfxSrc_enable(src);

    src = &MODULE_SRC.GPSR.GPSR[(int)cpu].SR[1];
    IfxSrc_init(src, (int)cpu, (uint32)CpuSoftIrqPriority[temp]);
    IfxSrc_enable(src);

    src = &MODULE_SRC.GPSR.GPSR[(int)cpu].SR[2];
    IfxSrc_init(src, (int)cpu, (uint32)CpuSoftIrqPriority[temp]);
    IfxSrc_enable(src);

    src = &MODULE_SRC.GPSR.GPSR[(int)cpu].SR[3];
    IfxSrc_init(src, (int)cpu, (uint32)CpuSoftIrqPriority[temp]);
    IfxSrc_enable(src);

    /* 安装中断服务函数 */
    IfxCpu_Irq_installInterruptHandler((void *)CpuSoftIrqFuncPointer[temp], (uint32)CpuSoftIrqPriority[temp]);

    /* 开启中断 */
    IfxCpu_restoreInterrupts(interruptState);
}

/*************************************************************************
 *  函数名称：CPSR_Trig(IfxSrc_Tos cpu, uint8 softIrq)
 *  功能说明：触发软件中断
 *  参数说明：cpu     ： IfxSrc_Tos_cpu0   IfxSrc_Tos_cpu1
 *  参数说明：softIrq ： 每个CPU有4个软件中断, irq序号
 *  函数返回：无
 *  修改时间：2020年5月6日
 *  备    注：
 *************************************************************************/
void CPSR_Trig(IfxSrc_Tos cpu, SOFT_IRQ index)
{
    if (cpu == IfxSrc_Tos_cpu0)
    {
        INT_SRB0.U = 1 << (uint8)index;
    }
    else if (cpu == IfxSrc_Tos_cpu1)
    {
        INT_SRB1.U = 1 << (uint8)index;
    }
    else if (cpu == IfxSrc_Tos_cpu2)
    {
        INT_SRB2.U = 1 << (uint8)index;
    }
    else if (cpu == IfxSrc_Tos_cpu3)
    {
        INT_SRB3.U = 1 << (uint8)index;
    }
}