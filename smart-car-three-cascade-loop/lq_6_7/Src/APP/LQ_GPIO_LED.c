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

#include "LQ_GPIO_LED.h"
#include "LQ_GPIO.h"
#include "LQ_STM.h"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LED_Init(void)
@功能说明：GPIO初始化函数 LED灯所用P10_5 P10_6 P20_6和P20_7初始化
@参数说明：无
@函数返回：无
@调用方法：GPIO_LED_Init();
@备    注：高电平灯灭，低电平灯亮
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GPIO_LED_Init(void)
{
    // 初始化,输出口，高电平
    PIN_InitConfig(LED0p, PIN_MODE_OUTPUT, 1);
    PIN_InitConfig(LED1p, PIN_MODE_OUTPUT, 1);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
@功能说明：LED控制
@参数说明：LEDn_e LEDno编号,LEDs_e sta状态亮/灭/翻转
@函数返回：按键状态，0/1
@调用方法：LED_Ctrl(LEDALL,RVS);     //LED翻转闪烁
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
{
    switch (LEDno)
    {
    case LED0:
        if (sta == ON)
            PIN_Write(LED0p, 0);
        else if (sta == OFF)
            PIN_Write(LED0p, 1);
        else if (sta == RVS)
            PIN_Reverse(LED0p);
        break;

    case LED1:
        if (sta == ON)
            PIN_Write(LED1p, 0);
        else if (sta == OFF)
            PIN_Write(LED1p, 1);
        else if (sta == RVS)
            PIN_Reverse(LED1p);
        break;
    case LEDALL:
        if (sta == ON)
        {
            PIN_Write(LED0p, 0);
            PIN_Write(LED1p, 0);
        }
        else if (sta == OFF)
        {
            PIN_Write(LED0p, 1);
            PIN_Write(LED1p, 1);
        }
        else if (sta == RVS)
        {
            PIN_Reverse(LED0p);
            PIN_Reverse(LED1p);
        }
        break;
    default:
        break;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void BEEP_Init(void)
@功能说明：BEEP初始化函数 BEEP所用P33_17初始化
@参数说明：无
@函数返回：无
@调用方法：GPIO_BEEP_Init();
@备    注：高电平蜂鸣器响，低电平不响
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GPIO_BEEP_Init(void)
{
    // 初始化,输出口，低电平
    PIN_InitConfig(Beep, PIN_MODE_OUTPUT, 0);
    PIN_Write(Beep, 0);
}

void Test_GPIO_OUT(void)
{
    GPIO_LED_Init();
    // GPIO_BEEP_Init();
    while (1)
    {
        LED_Ctrl(LEDALL, RVS);
        BEEP_RVS;
        Delay_Ms(200);
    }
}
