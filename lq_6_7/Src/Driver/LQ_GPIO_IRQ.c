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

#include "LQ_GPIO_IRQ.h"

/** 外部中断CPU标号 */
const unsigned char PinIrqVectabNum[8] = {PIN_INT0_VECTABNUM, PIN_INT1_VECTABNUM, PIN_INT2_VECTABNUM, PIN_INT3_VECTABNUM,
                                          PIN_INT4_VECTABNUM, PIN_INT5_VECTABNUM, PIN_INT6_VECTABNUM, PIN_INT7_VECTABNUM};
/** 外部中断优先级 */
const unsigned char PinIrqPriority[8] = {PIN_INT0_PRIORITY, PIN_INT1_PRIORITY, PIN_INT2_PRIORITY, PIN_INT3_PRIORITY,
                                         PIN_INT4_PRIORITY, PIN_INT5_PRIORITY, PIN_INT6_PRIORITY, PIN_INT7_PRIORITY};
/** 外部中断服务函数地址 */
const void *PinIrqFuncPointer[8] = {&PIN_INT0_IRQHandler, &PIN_INT1_IRQHandler, &PIN_INT2_IRQHandler, &PIN_INT3_IRQHandler,
                                    &PIN_INT4_IRQHandler, &PIN_INT5_IRQHandler, &PIN_INT6_IRQHandler, &PIN_INT7_IRQHandler};

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void PIN_Exti(GPIO_Name_t pin, IfxPort_InputMode mode)
@功能说明：GPIO外部中断初始化
@参数说明：pin:管脚标号    lq_gpio.h中定义
@      mode:GPIO 模式   lq_gpio.h中的宏定义中选择*
@函数返回：无
@举    例：PIN_Exti(P20_0, PIN_IRQ_MODE_FALLING);//配置P20_0 下降沿触发中断
@备    注：TC377只有17个固定的GPIO支持外部中断 这15个固定GPIO分为8组，每组只能选择其中一个作为外部中断管脚使用
@         0：P15_4  P10_7                                    1：P14_3  P10_8
@         2：P10_2  P02_1  P00_4                             3：P10_3  P14_1  P02_0
@         4：P15_5  P33_7                                    5：P15_8
@         6：P20_0  P11_10                                   7：P20_9  P15_1
*           外部中断服务函数在LQ_GPIO.c中  中断优先级配置在LQ_GPIO.h中 可以自行修改
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void PIN_Exti(GPIO_Name_t pin, IfxPort_InputMode mode)
{
    int i, j;
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    /* 关闭中断 */
    boolean interruptState = IfxCpu_disableInterrupts();

    IfxScu_Req_In *reqPin = 0;

    /* 检测管脚 是否是 固定的17个管脚 */
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (IfxScu_Req_In_pinTable[0][i][j] == NULL_PTR)
            {
            }
            else if ((uint32)port == (uint32)(IfxScu_Req_In_pinTable[0][i][j]->pin.port) && pinIndex == IfxScu_Req_In_pinTable[0][i][j]->pin.pinIndex)
            {
                reqPin = IfxScu_Req_In_pinTable[0][i][j];
            }
        }
    }

    if (reqPin == 0)
    {
        while (1)
            ; /* 请选择有外部中断功能的管脚 */
    }

    /* 配置上下拉电阻 */
    IfxScuEru_initReqPin(reqPin, mode);

    /* 根据输入引脚确定输入通道EICRm */
    IfxScuEru_InputChannel inputChannel = (IfxScuEru_InputChannel)reqPin->channelId;

    /* 配置中断模式 */
    IfxScuEru_disableFallingEdgeDetection(inputChannel); /* EICRm.FENn */
    IfxScuEru_disableRisingEdgeDetection(inputChannel);  /* EICRm.RENn */
    if (mode == IfxPort_InputMode_pullUp || mode == IfxPort_InputMode_noPullDevice)
    {
        IfxScuEru_enableFallingEdgeDetection(inputChannel); /* EICRm.FENn */
    }
    if (mode == IfxPort_InputMode_pullDown || mode == IfxPort_InputMode_noPullDevice)
    {
        IfxScuEru_enableRisingEdgeDetection(inputChannel); /* EICRm.RENn */
    }
    IfxScuEru_enableAutoClear(inputChannel);

    /* 信号的目的地 */
    IfxScuEru_InputNodePointer triggerSelect = (IfxScuEru_InputNodePointer)inputChannel;
    IfxScuEru_OutputChannel outputChannel = (IfxScuEru_OutputChannel)inputChannel;

    //  /* Connecting Matrix, Event Trigger Logic ETL block */
    IfxScuEru_enableTriggerPulse(inputChannel);            /* EICRM.EIENN */
    IfxScuEru_connectTrigger(inputChannel, triggerSelect); /* EICRm.INPn */

    /* output channels, OutputGating Unit OGU block */
    IfxScuEru_setFlagPatternDetection(outputChannel, inputChannel, FALSE);                             /* IGCRm.IPENn */
    IfxScuEru_disablePatternDetectionTrigger(outputChannel);                                           /* IGCRm.GEENn */
    IfxScuEru_setInterruptGatingPattern(outputChannel, IfxScuEru_InterruptGatingPattern_alwaysActive); /* IGCEm.IGPn */

    unsigned char vectabNum = PinIrqVectabNum[(int)inputChannel];
    unsigned char Priority = PinIrqPriority[(int)inputChannel];
    /* service request configuration */
    {
        volatile Ifx_SRC_SRCR *src = &MODULE_SRC.SCU.SCUERU[(int)outputChannel % 4];
        IfxSrc_init(src, vectabNum, Priority);
        IfxSrc_enable(src);
    }

    /* 安装管理中断的中断服务 */
    IfxCpu_Irq_installInterruptHandler((void *)PinIrqFuncPointer[(int)inputChannel], Priority);

    /* 开启中断 */
    IfxCpu_restoreInterrupts(interruptState);
}

/*************************************************************************
 *  函数名称：void PIN_ExtiEnable(GPIO_Name_t pin,  boolean bool)
 *  功能说明：GPIO外部中断使能
 *  参数说明： pin      ： 管脚标号    lq_gpio.h中定义
 *            bool     ： 是否开启中断
 *  函数返回：无
 *  举    例：PIN_ExtiEnable(P20_0, FALSE);//关闭P20_0 外部中断
 *  备    注：TC377只有15个固定的GPIO支持外部中断 这15个固定GPIO分为4组，每组只能选择其中一个作为外部中断管脚使用
 *           0：P15_4  P33_7  P15_5                             1：P14_3  P15_8
 *           2：P10_2  P02_1  P00_4  P20_0  P11_10              3：P10_3  P14_1  P02_0  P20_9  P15_1
 *           PIN_Exti()函数初始化后，默认GPIO开启外部中断，如果想要关闭，则可以调用PIN_ExtiEnable()
 *************************************************************************/
void PIN_ExtiEnable(GPIO_Name_t pin, boolean bool)
{
    int i, j;
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    /* 关闭中断 */
    boolean interruptState = IfxCpu_disableInterrupts();

    IfxScu_Req_In *reqPin = 0;

    /* 检测管脚 是否是 固定的17个管脚 */
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (IfxScu_Req_In_pinTable[0][i][j] == NULL_PTR)
            {
            }
            else if ((uint32)port == (uint32)(IfxScu_Req_In_pinTable[0][i][j]->pin.port) && pinIndex == IfxScu_Req_In_pinTable[0][i][j]->pin.pinIndex)
            {
                reqPin = IfxScu_Req_In_pinTable[0][i][j];
            }
        }
    }

    if (reqPin == 0)
    {
        while (1)
            ; /* 请选择有外部中断功能的管脚 */
    }

    /* 根据输入引脚确定输入通道EICRm */
    IfxScuEru_InputChannel inputChannel = (IfxScuEru_InputChannel)reqPin->channelId;

    volatile Ifx_SRC_SRCR *src = &MODULE_SRC.SCU.SCUERU[(int)inputChannel % 4];

    IfxSrc_clearOverrun(src);
    IfxSrc_clearRequest(src);

    if (bool)
    {
        IfxSrc_enable(src);
    }
    else
    {
        IfxSrc_disable(src);
    }

    /* 开启中断 */
    IfxCpu_restoreInterrupts(interruptState);
}
