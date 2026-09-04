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

#include "LQ_ADC_KEY.h"
#include "LQ_ADC.h"
#include "lq_include.h"
/*************************************************************************
 *  函数名称：void DMA_ADKey_Init()
 *  功能说明：ADC按键DMA初始化函数
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2026年1月12日
 *  备    注：建议用户将 ADKey_Scan()函数放到定时器或主循环定时运行,保证实时性
 *************************************************************************/
void ADC_Key_Init()
{
    ADC_InitConfig(AD_knob, 80000);                                   // ADC旋钮初始化
    ADC_InitConfig(AD_KEY_CH, 80000);                                 // ADC五向按键初始化
    UART_InitConfig(ADK_UART_RX_PIN, ADK_UART_TX_PIN, ADK_UART_BAUD); // 初始化 UART1下载口的串口,方便调试输出
}

/*************************************************************************
 *  函数名称：u8 Get_ADC_Key()
 *  功能说明：传入ADC 按键通道的ADC值，返回ADC按键键值
 *  参数说明：u16 key_ADval：ADC值
 *  函数返回：ADC按键的键值
 *  修改时间：2026年1月12日
 *  备    注：无
 *************************************************************************/
uint8 Get_ADC_Key()
{
    uint16 key_ADval = ADC_ReadAverage(AD_KEY_CH, 7); // 读取ADC按键通道的平均值，减少抖动影响
    // 根据ADC值范围判断按键,粗略计算
    // if(key_ADval < 500)        return KEY_PRESS;     // K0≈0
    // else if(key_ADval < 1500) return KEY_UP;        // Ku≈1028
    // else if(key_ADval < 2700) return KEY_RIGHT;     // Kr≈2345
    // else if(key_ADval < 3400) return KEY_DOWN;      // Kd≈3110
    // else if(key_ADval < 3900) return KEY_LEFT;      // Kl≈3630

    if (key_ADval < 2600 && key_ADval > 2250)
        return KEY_LEFT; // Kl≈3630
    else if (key_ADval < 2200 && key_ADval > 1800)
        return KEY_DOWN; // Kd≈3110
    else if (key_ADval < 1700 && key_ADval > 1000)
        return KEY_RIGHT; // Kr≈2345
    else if (key_ADval < 900 && key_ADval > 500)
        return KEY_UP; // Ku≈1028
    else if (key_ADval < 300)
        return KEY_PRESS; // K0≈0
    else
        return KEY_NONE; // 无按键≈4096
}

/*************************************************************************
 *  函数名称：void Test_DMA_ADKey(void)
 *  功能说明：ADC按键测试函数
 *  参数说明：无
 *  函数返回：按键状态
 *  修改时间：2026年1月12日
 *  备    注：建议用户将 ADKey_Scan()函数放到定时器或主循环定时运行,保证实时性
 *************************************************************************/
void Test_ADC_Key(void)
{
    char txt[64];
    uint8 rent_key;
    Display_Init(0); // 初始化LCD
    Display_CLS(U16_BLACK);

    ADC_Key_Init();
    /* 一体板上按键测 P33_9  ADC3 */
    ADC_InitConfig(AN3_KEY, 80000);           // 一体板上的AN3按键初始化
    PIN_InitConfig(M_KEY, PIN_MODE_INPUT, 1); // 一体板上的339按键初始化

    GPIO_LED_Init();

    Display_CLS(U16_BLACK);
    Delay_Ms(200);


    while (1)
    {
        rent_key = Get_ADC_Key(); // ADC按键扫描函数获取平均值，获取当前按键状态

        if (rent_key != KEY_NONE)
        {
            switch (rent_key)
            {
            case KEY_PRESS: // sprintf(txt,"KEY_PRESS  ");
                printf("KEY_PRESS  ");
                Display_showString(2, 4, "KEY_PRESS  ", U16_RED, U16_BLACK,16);
                // 处理K0按键(按下)
                break;
            case KEY_UP: // sprintf(txt,"KEY_UP     ");
                printf("KEY_UP     ");
                Display_showString(2, 4, "KEY_UP     ", U16_RED, U16_BLACK,16);
                // 处理Ku按键(上)
                break;
            case KEY_DOWN: // sprintf(txt,"KEY_DOWN    ");
                printf("KEY_DOWN    ");
                Display_showString(2, 4, "KEY_DOWN    ", U16_RED, U16_BLACK,16);
                // 处理Kd按键(下)
                break;
            case KEY_LEFT: // sprintf(txt,"KEY_LEFT    ");
                printf("KEY_LEFT    ");
                Display_showString(2, 4, "KEY_LEFT    ", U16_RED, U16_BLACK,16);
                // 处理Kl按键(左)
                break;
            case KEY_RIGHT: // sprintf(txt,"KEY_RIGHT  ");
                printf("KEY_RIGHT  ");
                Display_showString(2, 4, "KEY_RIGHT  ", U16_RED, U16_BLACK,16);
                // 处理Kr按键(右)
                break;
            }
            UART_PutStr(ADK_UART_PORT, txt); // 发送字符串
            //    while(ADKey_Scan() != KEY_NONE);  // 等待按键释放
        }

        sprintf(txt, "Kv=%04d,Ks=%04d ", ADC_Read(AD_knob), ADC_ReadAverage(AD_KEY_CH, 5)); // 显示当前ADC值和平均值
        UART_PutStr(ADK_UART_PORT, txt);
        printf(txt);
        Display_showString(1, 0, txt, U16_RED, U16_BLACK,16); // 显示16*12字符串

    

        if (!PIN_Read(M_KEY))                          // 读取一体板上的339按键状态，如果按下则退出
        {
            LED_Ctrl(LED0, ON);
        }
        if (ADC_Read(AN3_KEY)<500)                        // 读取一体板上的AN3按键状态，如果按下则退出
        {
            LED_Ctrl(LED0, OFF);
        }
        
        LED_Ctrl(LED1, RVS);
    }
}
