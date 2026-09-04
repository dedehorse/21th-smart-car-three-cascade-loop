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

#include "LQ_Hard_SPI.h"

#define MAX_BAUD 50000000

/*************************************************************************
 * @brief   QSPI初始化
 * @param   hard_SPIx,    : 选择SPI模块(SPI_1-SPI_4)
 * @param   cs_pin   : 选择SPI片选引脚
 * @param   sck_pin  : 选择SPI时钟引脚
 * @param   mosi_pin : 选择SPI MOSI引脚
 * @param   miso_pin : 选择SPI MISO引脚
 * @param   mode_x     : SPI模式 0：CPOL=0 CPHA=0    1：CPOL=0 CPHA=1   2：CPOL=1 CPHA=0   3：CPOL=1 CPHA=1 //具体细节可自行百度
 * @param   baud     : 设置SPI的波特率
 * @return  none.
 * @example HARD_QSPI_Init(IfxQspi_Index hard_SPIx,IfxQspi0_SCLK_P20_11_OUT, IfxQspi0_MTSR_P20_14_OUT, IfxQspi0_MRSTA_P20_12_IN, IfxQspi0_SLSO6_P20_10_OUT, 0, 30 * 1000 * 1000);//硬件SPI初始化  模式0 波特率为30Mhz
 *************************************************************************/
void HARD_QSPI_Init(IfxQspi_Index hard_SPIx, IfxQspi_Sclk_Out sck_pin, IfxQspi_Mtsr_Out mosi_pin, IfxQspi_Mrst_In miso_pin, IfxQspi_Slso_Out cs_pin, uint8 mode_x, uint32 baud)
{
    IfxQspi_SpiMaster_Config MasterConfig;
    IfxQspi_SpiMaster MasterHandle;
    IfxQspi_SpiMaster_Channel MasterChHandle;
    IfxQspi_SpiMaster_Pins MasterPins;
    IfxQspi_SpiMaster_Output SlsoPin;
    volatile Ifx_QSPI *moudle;

    moudle = IfxQspi_getAddress(hard_SPIx);

    MasterPins.mrstMode = IfxPort_InputMode_pullDown;
    MasterPins.mtsrMode = IfxPort_OutputMode_pushPull;
    MasterPins.sclkMode = IfxPort_OutputMode_pushPull;
    MasterPins.pinDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1;

    MasterPins.sclk = &sck_pin;
    MasterPins.mtsr = &mosi_pin;
    MasterPins.mrst = &miso_pin;
    SlsoPin.pin = &cs_pin;
    SlsoPin.driver = IfxPort_PadDriver_cmosAutomotiveSpeed1;
    SlsoPin.mode = IfxPort_OutputMode_pushPull;

    IfxQspi_SpiMaster_initModuleConfig(&MasterConfig, moudle);
    MasterConfig.base.mode = SpiIf_Mode_master;
    MasterConfig.base.maximumBaudrate = 50000000;
    MasterConfig.base.isrProvider = IfxSrc_Tos_cpu0;

    MasterConfig.pins = &MasterPins;
    IfxQspi_SpiMaster_initModule(&MasterHandle, &MasterConfig);

    IfxQspi_SpiMaster_ChannelConfig MasterChConfig;
    IfxQspi_SpiMaster_initChannelConfig(&MasterChConfig, &MasterHandle);

    MasterChConfig.base.baudrate = (float)baud;
    switch ((QSPI_Mode_t)mode_x)
    {
    case QSPI_MODE_0:
    {
        MasterChConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleLow;                   // CPOL  ->0
        MasterChConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnTrailingEdge; // CPHA  ->0
    }
    break;
    case QSPI_MODE_1:
    {
        MasterChConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleLow;
        MasterChConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnLeadingEdge;
    }
    break;
    case QSPI_MODE_2:
    {
        MasterChConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleHigh;
        MasterChConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnTrailingEdge;
    }
    break;
    case QSPI_MODE_3:
    {
        MasterChConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleHigh;
        MasterChConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnLeadingEdge;
    }
    break;
    }

    MasterChConfig.base.mode.dataHeading = SpiIf_DataHeading_msbFirst;
    MasterChConfig.base.mode.dataWidth = 8;

    MasterChConfig.base.mode.csActiveLevel = Ifx_ActiveState_low;
    MasterChConfig.sls.output = SlsoPin;
    IfxQspi_SpiMaster_initChannel(&MasterChHandle, &MasterChConfig);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI发送接收函数
//  @param      modata          发送的数据缓冲区地址
//  @param      midata          发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
//  @param      len             发送的字节数
//  @param      continuous      本次通信是CS是否持续保持有效状态 1:持续保持  0:每传输完一个字节关闭CS(一般设置为1 即可)
//  @return     void
//  @since      v2.0
//  Sample usage:               HARD_QSPI_Wirte_Read(IfxQspi_Index_0,buf,buf,1,1);    //发送buff的内容，并接收到buf里，长度为1字节 通信期间CS持续拉低
//-------------------------------------------------------------------------------------------------------------------
void HARD_QSPI_Wirte_Read(IfxQspi_Index hard_SPIx, uint8 *modata, uint8 *midata, uint32 len, uint8 continuous)
{
    uint32 i;
    Ifx_QSPI_BACON bacon;
    volatile Ifx_QSPI *moudle;

    moudle = IfxQspi_getAddress(hard_SPIx);  // QSPI_Index

    bacon.U = moudle->BACON.U;

    bacon.B.DL = 7;
    bacon.B.IDLE = 1;
    bacon.B.IPRE = 1;
    bacon.B.LEAD = 1;
    bacon.B.LPRE = 1;
    bacon.B.MSB = 1;
    bacon.B.PARTYP = 0;
    bacon.B.BYTE = 0;
    bacon.B.TRAIL = 1;
    bacon.B.TPRE = 1;
    bacon.B.CS = 6;
    if (continuous)
        IfxQspi_writeBasicConfigurationBeginStream(moudle, bacon.U); // 发送数据后CS继续保持为低
    else
        IfxQspi_writeBasicConfigurationEndStream(moudle, bacon.U); // 每发送一个字节CS信号拉高一次

    if (len > 1)
    {
        i = 0;
        while (i < (len - 1))
        {
            while (moudle->STATUS.B.TXFIFOLEVEL != 0)
                ;
            IfxQspi_write8(moudle, IfxQspi_ChannelId_0, modata, 1);
            while (moudle->STATUS.B.RXFIFOLEVEL == 0)
                ;
            if (NULL != midata)
            {
                IfxQspi_read8(moudle, midata, 1);
                midata++;
            }
            else
                (void)moudle->RXEXIT.U;
            modata++;

            i++;
        }
    }

    // 发送最后一个数据
    if (continuous)
        IfxQspi_writeBasicConfigurationEndStream(moudle, bacon.U);
    IfxQspi_writeTransmitFifo(moudle, *modata);
    while (moudle->STATUS.B.TXFIFOLEVEL != 0)
        ;

    while (moudle->STATUS.B.RXFIFOLEVEL == 0)
        ;
    if (NULL != midata)
        IfxQspi_read8(moudle, midata, 1);
    else
        (void)moudle->RXEXIT.U;
}
