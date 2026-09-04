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

#include "LQ_STM.h"
#include "LQ_UART.h"
#include "LQ_GPIO_LED.h"
#include <stdio.h>

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Test_Bluetooth(void);
@功能说明：UART测试函数
@参数说明：无
@函数返回：无
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
void Test_Bluetooth(void)
{
    unsigned char cnt = 0;
    GPIO_LED_Init();    //LED初始化
    //UART初始化
    UART_InitConfig(UART0_RX_P14_1, UART0_TX_P14_0, 115200);
    UART_InitConfig(UART1_RX_P02_3, UART1_TX_P02_2, 9600);
    UART_InitConfig(UART2_RX_P14_3, UART2_TX_P14_2, 19200);
    UART_InitConfig(UART3_RX_P00_1, UART3_TX_P00_0, 115200);
    while (1)
    {
//        UART_PutChar(UART0, 'L'); // 发送 字节到UART口
//        Delay_Ms(500);             // 延时等待
//        UART_PutChar(UART0, 'Q'); // 发送 字节到UART口
//
        UART_PutStr(UART0, "UART0 LongQiu \r\n"); // 发送字符串到上位机
        UART_PutStr(UART1, "UART1 LongQiu \r\n"); // 发送字符串到上位机
        UART_PutStr(UART2, "UART2 LongQiu \r\n"); // 发送字符串到上位机
        UART_PutStr(UART3, "UART3 LongQiu \r\n"); // 发送字符串到上位机

        printf("LQ UART1 %05d...\r\n", cnt++);

        LED_Ctrl(LED0, RVS); // 电平翻转,LED闪烁
        Delay_Ms(500);        // 延时等待
    }
}
