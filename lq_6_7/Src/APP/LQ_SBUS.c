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

#include "LQ_SBUS.h"

int num3 = 0;
unsigned char ReadBuff3[512];
char Usart3_Rec_Fini_Flag = 0; // 串口3接收完成标志位

/***********************************************************
 *函 数 名:update_sbus
 *设计时间: 2025年02月10日
 *设计人(开发人)：
 *功能说明: 解析SUBS模块信息
 ***********************************************************/
unsigned int RIGHTH = 0; // 右手左右
unsigned int LEFTV = 0;  // 左手上下
unsigned int RIGHTV = 0; // 右手上下
unsigned int LEFTH = 0;  // 左手左右
unsigned int VRH = 0;    // H开关
unsigned int VRF = 0;    // F开关
unsigned int SWC = 0;    // C开关
unsigned int SWD = 0;    // D开关
unsigned int SWA = 0;    // A开关
unsigned int SWB = 0;    // B开关
unsigned int CHDATA[18];
void update_sbus(void)
{
    RIGHTV = ((ReadBuff3[1] | ReadBuff3[2] << 8) & 0x07FF);                        // 右手水平左中右    432---992---1552
    RIGHTH = ((ReadBuff3[2] >> 3 | ReadBuff3[3] << 5) & 0x07FF);                   // 右手下中上    432---992---1552
    LEFTH = (ReadBuff3[3] >> 6 | ReadBuff3[4] << 2 | ReadBuff3[5] << 10) & 0x07FF; // 左手左中右      432---992---1552
    LEFTV = (ReadBuff3[5] >> 1 | ReadBuff3[6] << 7) & 0x07FF;                      // 左手下中上      432---992---1552
    SWA = (ReadBuff3[6] >> 4 | ReadBuff3[7] << 4) & 0x07FF;                        // SWA-5上中下  192---992---1792
    SWB = (ReadBuff3[7] >> 7 | ReadBuff3[8] << 1 | ReadBuff3[9] << 9) & 0x07FF;    // SWB-6上下  192---1792
    SWC = (ReadBuff3[9] >> 2 | ReadBuff3[10] << 6) & 0x07FF;                       // SWC-7  192---1792
    SWD = (ReadBuff3[10] >> 5 | ReadBuff3[11] << 3) & 0x07FF;                      // SWD-8上中下  192---992---1792
    // SWB   =CHDATA[9] =(ReadBuff3[13] & 0X07)<<8  | ReadBuff3[12];                   //SWA左拉杆顺时针左右   0074-1925；      0000-2000
}

// 接收函数，需要放在uart1的RX回调函数里面
void R9DS_Read(void)
{
    unsigned char buf;
    buf = UART_GetChar(UART1);
    if (num3 == 0 && buf == 0x0F) // 帧头
    {
        ReadBuff3[num3] = buf;
        num3++;
        LED_Ctrl(LED1, RVS); // LED闪烁 指示程序运行状态
    }
    else if (num3 != 0)
    {
        LED_Ctrl(LED0, RVS); // LED闪烁 指示程序运行状态
        ReadBuff3[num3] = buf;
        num3++;
        if (num3 > 24) // 接收数据超过24，表示一帧接收完成
        {
            Usart3_Rec_Fini_Flag = 0; // 停止接收

            if (ReadBuff3[0] == 0x0F && ReadBuff3[24] == 0x00 && num3 == 25) // 接受完一帧数据
            {
                update_sbus(); // 数据解析
            }
            num3 = 0; // 清空
        }
    }
}

void Test_SBUS(void)
{
    char txt[32];
    unsigned char size = 12;
    Display_Init(0);
    Display_CLS(U16_WHITE);

    UART_InitConfig(UART1_RX_P20_9, UART1_TX_P15_0, 100000); // 初始化串口3 接收SBUS信号
    GPIO_LED_Init();                                         // LED初始化
    GPIO_KEY_Init();                                         // 按键初始化

    while (1)
    {

        Usart3_Rec_Fini_Flag = 1; // 开始接收
        // sprintf(txt, "variate:%05d", count);                  // 将变量填充到字符串的对应位置，并将字符串存放到txt[]中

        sprintf(txt, "RV:%04d RH:%04d", RIGHTV, RIGHTH);
        Display_showString(0, 2, txt, U16_RED, U16_BLUE, size); // 将txt中 内容显示出来
        sprintf(txt, "LH:%04d LV:%04d", LEFTH, LEFTV);
        Display_showString(0, 3, txt, U16_RED, U16_BLUE, size); // 将txt中 内容显示出来
        sprintf(txt, "SWA:%04d SWB:%04d", SWA, SWB);
        Display_showString(0, 4, txt, U16_RED, U16_BLUE, size); // 将txt中 内容显示出来
        sprintf(txt, "SWC:%04d SWD:%04d", SWC, SWD);
        Display_showString(0, 5, txt, U16_RED, U16_BLUE, size); // 将txt中 内容显示出来

        //        LED_Ctrl(LED0, RVS);
        Delay_Ms(10);
    }
}
