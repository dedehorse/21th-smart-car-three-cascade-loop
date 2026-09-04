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
#ifndef _LQ_INCLUDE_H_
#define _LQ_INCLUDE_H_

/////////////c标准库和底层库文件//////////////////////////////////////////////////
/////////////c标准库和底层库文件//////////////////////////////////////////////////
#include <Bsp.h>
#include <Compilers.h>
#include <CompilerTasking.h>
#include <IfxQspi_PinMap.h>
#include <IfxQspi_SpiMaster.h>
#include <IfxCpu.h>
#include <IfxCpu_Irq.h>
#include <IfxScuEru.h>
#include <IfxScu_reg.h>
#include <IfxScu_cfg.h>
#include <IfxScuCcu.h>
#include <IfxScuWdt.h>
#include <IfxStm.h>
#include <IfxSrc.h>
#include <IfxSrc_reg.h>
#include <IfxSrc_regdef.h>
#include <IfxScu_PinMap.h>
#include <Ifxport.h>
#include <IfxPort_regdef.h>
#include <IfxCcu6.h>
#include <IfxCcu6_cfg.h>
#include <IfxCcu6_reg.h>
#include <IfxCcu6_regdef.h>
#include <IfxCcu6_Timer.h>
#include <IfxDma.h>
#include <IfxDma_Dma.h>
#include <IfxDma_reg.h>
#include <IfxGpt12.h>
#include <IfxGpt12_reg.h>
#include <IfxInt_reg.h>
#include <IfxFlash.h>
#include <IfxFlash_cfg.h>
#include <IfxSrc_cfg.h>
#include <Ifx_Fifo.h>
#include <Ifx_Types.h>
#include <IfxAsclin.h>
#include <IfxAsclin_Asc.h>
#include <IfxAsclin_PinMap.h>
#include <Ifx_FftF32.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "Platform_Types.h"

#include <IfxGtm_Atom_Pwm.h>
#include <IfxGtm_Tom_Pwm.h>
#include <IfxGtm_Tim_In.h>
#include <IfxAsclin_Spi.h>

/////////////////////龙邱库文件////////////////////////////////////////////
/**************APP*******************/
#include "LQ_ADC_7Mic.h"
#include "LQ_BD.h"
#include "LQ_CAMERA.h"
#include "LQ_CCU6_Timer.h"
#include "LQ_EEPROM_TEST.h"
#include "LQ_FFT_TEST.h"
#include "LQ_GPIO_ExInt.h"
#include "LQ_GPIO_LED.h"
#include <LQ_GPIO_KEY.h>
#include "LQ_IIC_Gyro.h"
#include "LQ_SPI_Gyro.h"
#include "LQ_MPU6050_DMP.h"
#include "LQ_MT9V034.h"

#include "LQ_STM_Timer.h"
#include "LQ_Display.h"
#include "LQ_OLED096.h"
#include "LQ_TFT18.h"
#include "LQ_TFT2.h"
#include "LQ_IPSLCD.h"
#include "LQ_PWM_BLDC.h"
#include "LQ_PWM_Moto.h"
#include "LQ_PWM_Servo.h"
#include "LQ_PWM_Blsmotor.h"
#include "LQ_Track.h"
#include "LQ_Encoder.h"
#include <LQ_UART_Bluetooth.h>
#include "LQ_SBUS.h"
// #include "LQ_I2C_VL53.h"
#include "LQ_UTM.h"
#include "LQ_Transfer_Image.h"
#include "LQ_ADC_KEY.h"

/**************Drv*******************/
#include "LQ_ADC.h"
#include "LQ_CCU6.h"
#include "LQ_DMA.h"
#include "LQ_EEPROM.h"
#include "LQ_GPIO.h"
#include "LQ_GPIO_IRQ.h"
#include "LQ_GPSR.h"
#include "LQ_GPT12_ENC.h"
#include "LQ_GTM_PWM.h"
#include "LQ_Hard_SPI.h"
#include "LQ_QSPI.h"
#include "LQ_SOFTI2C.h"
#include "LQ_SOFTSPI.h"
#include "LQ_SPI.h"
#include "LQ_STM.h"
#include "LQ_UART.h"
#include "LQ_Wireless_Config.h"

/**************Main*******************/
#include "config.h"
#include "Main.h"
#include "cpu_init.h"
#include "irq.h"

/**************User******************/
#include "LQ_PID.h"
#include "LQ_Quat.h"
#include "LQ_PathPlan.h"

typedef struct
{
    float32 sysFreq; // Actual SPB frequency
    float32 cpuFreq; // Actual CPU frequency
    float32 pllFreq; // Actual PLL frequency
    float32 stmFreq; // Actual STM frequency
} AppInfo;

// Application information
typedef struct
{
    AppInfo info; // Info object
} App_Cpu0;

#endif // _LQ_INCLUDE_H_
