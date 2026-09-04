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

#include "LQ_QSPI.h"

/** QSPI中断CPU标号 */
const unsigned char QSpiIrqVectabNum[5] =
{
        QSPI0_VECTABNUM,
        QSPI1_VECTABNUM,
        QSPI2_VECTABNUM,
        QSPI3_VECTABNUM,
        QSPI4_VECTABNUM
};

/** QSPI中断优先级 */
const unsigned char QSpiIrqPriority[15] =
{
        QSPI0_RX_PRIORITY, QSPI0_TX_PRIORITY, QSPI0_ER_PRIORITY,
        QSPI1_RX_PRIORITY, QSPI1_TX_PRIORITY, QSPI1_ER_PRIORITY,
        QSPI2_RX_PRIORITY, QSPI2_TX_PRIORITY, QSPI2_ER_PRIORITY,
        QSPI3_RX_PRIORITY, QSPI3_TX_PRIORITY, QSPI3_ER_PRIORITY,
        QSPI4_RX_PRIORITY, QSPI4_TX_PRIORITY, QSPI4_ER_PRIORITY
};

/** QSPI 配置结构体 */
IfxQspi_SpiMaster g_QSPIConfig[5];
IfxQspi_SpiMaster_Channel g_QspiMasterChannel[5];

/**
 * @brief    QSPI0_RX_IRQHandler中断服务函数
 * @note     QSPI0 接收中断服务函数
 */
void QSPI0_RX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[0]);

    /* 用户代码 */
}

void QSPI0_TX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[0]);
    /* 用户代码 */
}

void QSPI0_ER_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrError(&g_QSPIConfig[0]);
    /* 用户代码 */
}

void QSPI1_RX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[1]);
    /* 用户代码 */
}

void QSPI1_TX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[1]);
    /* 用户代码 */
}

void QSPI1_ER_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrError(&g_QSPIConfig[1]);
    /* 用户代码 */
}

void QSPI2_RX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[2]);
    /* 用户代码 */
}

void QSPI2_TX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[2]);
    /* 用户代码 */
}

void QSPI2_ER_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrError(&g_QSPIConfig[2]);
    /* 用户代码 */
}

void QSPI3_RX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[3]);
    /* 用户代码 */
}

void QSPI3_TX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[3]);
    /* 用户代码 */
}

void QSPI3_ER_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrError(&g_QSPIConfig[3]);
    /* 用户代码 */
}

void QSPI4_RX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[4]);
    /* 用户代码 */
}

void QSPI4_TX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[4]);
    /* 用户代码 */
}

void QSPI4_ER_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrError(&g_QSPIConfig[4]);
    /* 用户代码 */
}

/* QSPI中断 */
IFX_INTERRUPT(QSPI0_RX_IRQHandler, QSPI0_VECTABNUM, QSPI0_RX_PRIORITY);
IFX_INTERRUPT(QSPI1_RX_IRQHandler, QSPI1_VECTABNUM, QSPI1_RX_PRIORITY);
IFX_INTERRUPT(QSPI2_RX_IRQHandler, QSPI2_VECTABNUM, QSPI2_RX_PRIORITY);
IFX_INTERRUPT(QSPI3_RX_IRQHandler, QSPI3_VECTABNUM, QSPI3_RX_PRIORITY);
IFX_INTERRUPT(QSPI4_RX_IRQHandler, QSPI4_VECTABNUM, QSPI4_RX_PRIORITY);

IFX_INTERRUPT(QSPI0_TX_IRQHandler, QSPI0_VECTABNUM, QSPI0_TX_PRIORITY);
IFX_INTERRUPT(QSPI1_TX_IRQHandler, QSPI1_VECTABNUM, QSPI1_TX_PRIORITY);
IFX_INTERRUPT(QSPI2_TX_IRQHandler, QSPI2_VECTABNUM, QSPI2_TX_PRIORITY);
IFX_INTERRUPT(QSPI3_TX_IRQHandler, QSPI3_VECTABNUM, QSPI3_TX_PRIORITY);
IFX_INTERRUPT(QSPI4_TX_IRQHandler, QSPI4_VECTABNUM, QSPI4_TX_PRIORITY);

IFX_INTERRUPT(QSPI0_ER_IRQHandler, QSPI0_VECTABNUM, QSPI0_ER_PRIORITY);
IFX_INTERRUPT(QSPI1_ER_IRQHandler, QSPI1_VECTABNUM, QSPI1_ER_PRIORITY);
IFX_INTERRUPT(QSPI2_ER_IRQHandler, QSPI2_VECTABNUM, QSPI2_ER_PRIORITY);
IFX_INTERRUPT(QSPI3_ER_IRQHandler, QSPI3_VECTABNUM, QSPI3_ER_PRIORITY);
IFX_INTERRUPT(QSPI4_ER_IRQHandler, QSPI4_VECTABNUM, QSPI4_ER_PRIORITY);



/** QSPI中断服务函数地址 */
const void *QSPIIrqFuncPointer[15] = {
    &QSPI0_RX_IRQHandler,
    &QSPI0_TX_IRQHandler,
    &QSPI0_ER_IRQHandler,
    &QSPI1_RX_IRQHandler,
    &QSPI1_TX_IRQHandler,
    &QSPI1_ER_IRQHandler,
    &QSPI2_RX_IRQHandler,
    &QSPI2_TX_IRQHandler,
    &QSPI2_ER_IRQHandler,
    &QSPI3_RX_IRQHandler,
    &QSPI3_TX_IRQHandler,
    &QSPI3_ER_IRQHandler,
    &QSPI4_RX_IRQHandler,
    &QSPI4_TX_IRQHandler,
    &QSPI4_ER_IRQHandler,
};

/*************************************************************************
 *  函数名称：QSPI_InitConfig(QSPI_CLK_t clkPin, QSPI_MISO_t misoPin, QSPI_MOSI_t mosiPin, QSPI_CS_t cQSPIn, unsigned long baudrate, unsigned char mode)
 *  功能说明：QSPI 主机模式初始化
 *  参数说明：
 * @param    clkPin      :  QSPI  clk管脚
 * @param    misoPin     :  QSPI  MOSI管脚
 * @param    misoPin     :  QSPI  MISO管脚
 * @param    misoPin     :  QSPI  CS管脚
 * @param    baudrate    :  QSPI  波特率
 * @param      mode            0: CLK空闲时刻为低电平 在第一个CLK跳边沿 捕获数据
 * @param      mode            1: CLK空闲时刻为低电平 在第二个CLK跳边沿 捕获数据
 * @param      mode            2: CLK空闲时刻为高电平 在第一个CLK跳边沿 捕获数据
 * @param      mode            3: CLK空闲时刻为高电平 在第二个CLK跳边沿 捕获数据
 *  函数返回：无
 *  备    注：QSPI_InitConfig(QSPI2_CLK_P15_8, QSPI2_MISO_P15_7, QSPI2_MOSI_P15_5, QSPI2_CS_P15_2, 5000000, 3);
 *
 *************************************************************************/
void QSPI_InitConfig(QSPI_CLK_t clkPin, QSPI_MISO_t misoPin, QSPI_MOSI_t mosiPin, QSPI_CS_t cQSPIn, unsigned long baudrate, QSPI_Mode_t mode)
{
    unsigned char QSPINum = QSPI_GetModule(clkPin);

    IfxQspi_Sclk_Out *QSPI_Sclk = IfxQspi_Sclk_Out_pinTable[QSPINum][QSPI_GetIndex(clkPin)];
    IfxQspi_Mrst_In *QSPI_MISO = IfxQspi_Mrst_In_pinTable[QSPINum][QSPI_GetIndex(misoPin)];
    IfxQspi_Mtsr_Out *QSPI_MOSI = IfxQspi_Mtsr_Out_pinTable[QSPINum][QSPI_GetIndex(mosiPin)];
    IfxQspi_Slso_Out *QSPI_CS = IfxQspi_Slso_Out_pinTable[QSPINum][QSPI_GetIndex(cQSPIn)][0];

    IfxQspi_SpiMaster_Config spiMasterConfig;
    IfxQspi_SpiMaster_ChannelConfig spiMasterChannelConfig;

    {
        /* create module config */
        IfxQspi_SpiMaster_initModuleConfig(&spiMasterConfig, IfxQspi_getAddress((IfxQspi_Index)QSPINum));

        /* set the maximum baudrate */
        spiMasterConfig.base.maximumBaudrate = 50000000;

        /* ISR priorities and interrupt target */
        spiMasterConfig.base.rxPriority = QSpiIrqPriority[QSPINum * 3];
        spiMasterConfig.base.txPriority = QSpiIrqPriority[QSPINum * 3 + 1];
        spiMasterConfig.base.erPriority = QSpiIrqPriority[QSPINum * 3 + 2];
        spiMasterConfig.base.isrProvider = (IfxSrc_Tos)QSpiIrqVectabNum[QSPINum];

        /* pin configuration */
        const IfxQspi_SpiMaster_Pins pins = {
            QSPI_Sclk, /* SCLK */
            IfxPort_OutputMode_pushPull,
            QSPI_MOSI, IfxPort_OutputMode_pushPull, /* MTSR */
            QSPI_MISO, IfxPort_InputMode_pullDown,  /* MRST */
            IfxPort_PadDriver_cmosAutomotiveSpeed3  /* pad driver mode */
        };
        spiMasterConfig.pins = &pins;

        /* initialize module */
        IfxQspi_SpiMaster_initModule(&g_QSPIConfig[QSPINum], &spiMasterConfig);
    }

    {
        /* create channel config */
        IfxQspi_SpiMaster_initChannelConfig(&spiMasterChannelConfig,
                                            &g_QSPIConfig[QSPINum]);

        const IfxQspi_SpiMaster_Output slsOutput = {QSPI_CS,
                                                    IfxPort_OutputMode_pushPull,
                                                    IfxPort_PadDriver_cmosAutomotiveSpeed1};

        spiMasterChannelConfig.sls.output.pin = slsOutput.pin;
        spiMasterChannelConfig.sls.output.mode = slsOutput.mode;
        spiMasterChannelConfig.sls.output.driver = slsOutput.driver;
        spiMasterChannelConfig.base.mode.csInactiveDelay = 1;
        spiMasterChannelConfig.base.mode.csLeadDelay = 10; /* CS前置延时 */
        spiMasterChannelConfig.base.mode.csTrailDelay = 1;
        spiMasterChannelConfig.base.mode.dataHeading = SpiIf_DataHeading_msbFirst; /* MSB在前      */
        spiMasterChannelConfig.base.mode.dataWidth = 8;                            /* 8bit     */
        spiMasterChannelConfig.base.baudrate = (float)baudrate;                    /* 设置速度    */
        spiMasterChannelConfig.base.mode.csActiveLevel = Ifx_ActiveState_low;      /* CS为低有效*/
        switch (mode)
                {
                    case QSPI_MODE_0:
                        spiMasterChannelConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleLow;                       // 时钟空闲时为低电平
                        spiMasterChannelConfig.base.mode.shiftClock    = (QSPINum < QSPI2) ? SpiIf_ShiftClock_shiftTransmitDataOnTrailingEdge :
                                                                                             SpiIf_ShiftClock_shiftTransmitDataOnLeadingEdge;
                        break;
                    case QSPI_MODE_1:
                        spiMasterChannelConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleLow;                       // 时钟空闲时为低电平
                        spiMasterChannelConfig.base.mode.shiftClock    = (QSPINum < QSPI2) ? SpiIf_ShiftClock_shiftTransmitDataOnLeadingEdge :
                                                                                             SpiIf_ShiftClock_shiftTransmitDataOnTrailingEdge;
                        break;
                    case QSPI_MODE_2:
                        spiMasterChannelConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleHigh;                      // 时钟空闲时为高电平
                        spiMasterChannelConfig.base.mode.shiftClock    = (QSPINum < QSPI2) ? SpiIf_ShiftClock_shiftTransmitDataOnTrailingEdge :
                                                                                             SpiIf_ShiftClock_shiftTransmitDataOnLeadingEdge;
                        break;
                    case QSPI_MODE_3:
                        spiMasterChannelConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleHigh;                      // 时钟空闲时为高电平
                        spiMasterChannelConfig.base.mode.shiftClock    = (QSPINum < QSPI2) ? SpiIf_ShiftClock_shiftTransmitDataOnLeadingEdge :
                                                                                             SpiIf_ShiftClock_shiftTransmitDataOnTrailingEdge;
                        break;
                }
        // 接收，发送和错误中断配置
        IfxCpu_Irq_installInterruptHandler((void *)QSPIIrqFuncPointer[QSPINum * 3], (unsigned long)QSpiIrqPriority[QSPINum * 3]);
        IfxCpu_Irq_installInterruptHandler((void *)QSPIIrqFuncPointer[QSPINum * 3 + 1], (unsigned long)QSpiIrqPriority[QSPINum * 3 + 1]);
        IfxCpu_Irq_installInterruptHandler((void *)QSPIIrqFuncPointer[QSPINum * 3 + 2], (unsigned long)QSpiIrqPriority[QSPINum * 3 + 2]);

        IfxQspi_SpiMaster_initChannel(&g_QspiMasterChannel[QSPINum],
                                      &spiMasterChannelConfig);
    }
}

/*************************************************************************
 *  函数名称：unsigned char QSPI_ReadWriteNByte(QSPI_t QSPI, unsigned char *txData, unsigned char *rxData, unsigned long len)
 *  功能说明：QSPI读写N个字节
 *  参数说明：
 * @param    QSPI     ：  QSPI0 - QSPI4
 * @param    txData  ：  写入的buff
 * @param    rxData  ：  读出的buff
 * @param    len     ：  长度
 *  函数返回：无
 *  备    注：
 *  			QSPI_ReadWriteNByte(QSPI3, buff, buff, 2); //使用QSPI3 读写两个字节
 *************************************************************************/
unsigned char QSPI_ReadWriteNByte(QSPI_t QSPI, unsigned char *txData, unsigned char *rxData, unsigned short len)
{
    /* 开始传输 */
    IfxQspi_SpiMaster_exchange(&g_QspiMasterChannel[QSPI], txData, rxData, len);
    /* 等待传输结束  */
    while (IfxQspi_SpiMaster_getStatus(&g_QspiMasterChannel[QSPI]) == SpiIf_Status_busy)
        ;
    return 0;
}
