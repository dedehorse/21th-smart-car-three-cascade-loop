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

#include <stdio.h>
#include "LQ_Encoder.h"
#include "LQ_GPT12_ENC.h"
#include "LQ_STM.h"
#include "LQ_UART.h"
#include "LQ_GPIO_LED.h"
#include "LQ_OLED096.h"
#include "LQ_TFT18.h"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void EncInit (void)
@功能说明：编码器初始化
@参数说明：无
@函数返回：无
@调用方法：EncInit();
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void EncInit(void)
{
    ENC_InitConfig(ENC1_InPut_P33_7, ENC1_Dir_P33_6);
    ENC_InitConfig(ENC2_InPut_P02_8, ENC2_Dir_P33_5);
//    ENC_InitConfig(ENC3_InPut_P10_3, ENC3_Dir_P10_1);
    ENC_InitConfig(ENC4_InPut_P20_3, ENC4_Dir_P20_0);
}

void Test_Encoder(void)
{
    char txt[32];
    sint16 ECPULSE1 = 0;
    sint16 ECPULSE2 = 0;
//    sint16 ECPULSE3 = 0;
    sint16 ECPULSE4 = 0;
    GPIO_LED_Init();
    GPIO_KEY_Init();
    Display_Init(0);         // LCD初始化
    Display_CLS(U16_BLACK); // 蓝色屏幕
    Display_CLS(U16_BLACK); // 清屏
    Display_showString(0, 0, "Test Encoder", U16_RED, U16_BLACK, 16);

    EncInit();
    while (1)
    {
        /* 获取编码器值 */
        ECPULSE1 = ENC_GetCounter(ENC1_InPut_P33_7);
        ECPULSE2 = ENC_GetCounter(ENC2_InPut_P02_8);
//        ECPULSE3 = ENC_GetCounter(ENC3_InPut_P10_3);
        ECPULSE4 = ENC_GetCounter(ENC4_InPut_P20_3);
        sprintf(txt, "Enc1: %05d;", ECPULSE1);
        Display_showString(0, 3, txt, U16_WHITE, U16_BLACK, 16); // 字符串显示
        sprintf(txt, "Enc2: %05d;", ECPULSE2);
        Display_showString(0, 4, txt, U16_WHITE, U16_BLACK, 16); // 字符串显示
//        sprintf(txt, "Enc3: %05d;", ECPULSE3);
//        Display_showString(0, 5, txt, U16_WHITE, U16_BLACK, 16); // 字符串显示
        sprintf(txt, "Enc4: %05d;", ECPULSE4);
        Display_showString(0, 6, txt, U16_WHITE, U16_BLACK, 16); // 字符串显示

        LED_Ctrl(LED0, RVS); // 电平翻转,LED闪烁
        Delay_Ms(200);       // 延时等待
    }
}

EncoderState encoder = {0};;

void Encoder_Update(void) {
    encoder.speedL = (float)ENC_GetCounter(ENC1_InPut_P33_7) * 100.0f / ONE_METER_L / DELTA_T;
    encoder.speedR = -(float)ENC_GetCounter(ENC4_InPut_P20_3) * 100.0f / ONE_METER_R / DELTA_T;

    encoder.distance += (encoder.speedL + encoder.speedR)/2.0f;
}

float Get_Current_SpeedL(void) {
    return encoder.speedL;
}

float Get_Current_SpeedR(void) {
    return encoder.speedR;
}

float Get_Distance(void) {
    return encoder.distance;
}

void Reset_Distance(void) {
    encoder.distance = 0;
}
