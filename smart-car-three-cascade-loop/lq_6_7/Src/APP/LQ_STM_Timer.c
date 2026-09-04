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

#include "LQ_STM_Timer.h"
#include "LQ_STM.h"
#include <stdio.h>
/*************************************************************************
 *  函数名称：void Test_STM(void)
 *  功能说明：测试程序
 *  参数说明：无
 *  函数返回：无
 *  备    注：核心板上的LED灯闪烁，中断时P10.5/P10.6闪灯
 *************************************************************************/
void Test_STM_Timer(void)
{
    uint32 time = 0;
    char txt[50];
    Display_Init(0);
    Delay_Ms(200);
    Display_CLS(U16_BLACK);

    GPIO_LED_Init(); // 初始化LED
    UART_InitConfig(WLS_UART_RX_PIN, WLS_UART_TX_PIN, WLS_UART_BAUD); //  WiFi UART 初始化

    // 中断服务函数中翻转LED
    while (1)
    {
        time = STM_GetNowUs(STM0);
        Delay_Us(1000);
        LED_Ctrl(LEDALL, RVS);
        time = STM_GetNowUs(STM0) - time;

        char buf[32];
        sprintf(buf, "time: %07ld\r\n", time);
        UART_PutStr(WLS_UART_PORT, buf);  // 使用UART3打印
        sprintf(txt, "time:%07ldUs", time);                       // 将变量填充到字符串的对应位置，并将字符串存放到txt[]中
        Display_showString(0, 1, txt, U16_WHITE, U16_BLACK, 24); // 将txt中 内容显示出来
    }
}
