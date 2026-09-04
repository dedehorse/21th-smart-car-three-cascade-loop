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

#include "LQ_ADC_7Mic.h"

#include "LQ_ADC.h"
#include "LQ_STM.h"
#include "LQ_UART.h"
#include "LQ_GPIO_LED.h"
#include "LQ_TFT18.h"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL*
@函数名称：void TestADC(void)
@功能说明：ADC采集测试
@参数说明：无
@函数返回：无
@备    注：ADC悬空不为零，随机的
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_ADC(void)
{
    char txt[32];
    short adc_val[30] = {0};
    GPIO_LED_Init();
    Display_Init(0);         // LCD初始化
    Display_CLS(U16_BLACK); // 黑色屏幕

    Display_showString(0, 0, "   ADC_Test      ", U16_WHITE, U16_BLACK, 16);
    // ADC初始化
    ADC_InitConfig(ADC0, 80000);
    ADC_InitConfig(ADC1, 80000);
    ADC_InitConfig(ADC2, 80000);
    ADC_InitConfig(ADC3, 80000);
    ADC_InitConfig(ADC4, 80000);
    ADC_InitConfig(ADC5, 80000);
    ADC_InitConfig(ADC6, 80000);
    ADC_InitConfig(ADC7, 80000);
    ADC_InitConfig(ADC8, 80000);
    ADC_InitConfig(ADC9, 80000);

    // 需要去config.h 文件里面打开对于自己的屏幕和设置适合自己的显示方向

    while (1) // 主循环
    {
        // adc读取
        adc_val[0] = ADC_Read(ADC0);
        adc_val[1] = ADC_Read(ADC1);
        adc_val[2] = ADC_Read(ADC2);
        adc_val[3] = ADC_Read(ADC3);
        adc_val[4] = ADC_Read(ADC4);
        adc_val[5] = ADC_Read(ADC5);
        adc_val[6] = ADC_Read(ADC6);
        adc_val[7] = ADC_Read(ADC7);
        adc_val[8] = ADC_Read(ADC8);
        adc_val[9] = ADC_Read(ADC9);

        // 屏幕显示adc读取的值
        sprintf((char *)txt, "   ADC0:%05d  ", adc_val[0]);
        Display_showString(0, 1, txt, U16_WHITE, U16_BLACK, 16);
        sprintf((char *)txt, "   ADC1:%05d  ", adc_val[1]);
        Display_showString(0, 2, txt, U16_WHITE, U16_BLACK, 16);
        sprintf((char *)txt, "   ADC2:%05d  ", adc_val[2]);
        Display_showString(0, 3, txt, U16_WHITE, U16_BLACK, 16);
        sprintf((char *)txt, "   ADC3:%05d  ", adc_val[3]);
        Display_showString(0, 4, txt, U16_WHITE, U16_BLACK, 16);
        sprintf((char *)txt, "   ADC4:%05d  ", adc_val[4]);
        Display_showString(0, 5, txt, U16_WHITE, U16_BLACK, 16);
        sprintf((char *)txt, "   ADC5:%05d  ", adc_val[5]);
        Display_showString(0, 6, txt, U16_WHITE, U16_BLACK, 16);
        sprintf((char *)txt, "   ADC6:%05d  ", adc_val[6]);
        Display_showString(0, 7, txt, U16_WHITE, U16_BLACK, 16);
        sprintf((char *)txt, "   ADC7:%05d  ", adc_val[7]);
        Display_showString(0, 8, txt, U16_WHITE, U16_BLACK, 16);
        sprintf((char *)txt, "   ADC8:%05d  ", adc_val[8]);
        Display_showString(0, 9, txt, U16_WHITE, U16_BLACK, 16);
        sprintf((char *)txt, "   ADC9:%05d  ", adc_val[9]);
        Display_showString(0, 9, txt, U16_WHITE, U16_BLACK, 16);
        LED_Ctrl(LEDALL, RVS);
        Delay_Ms(100);
    }
}
