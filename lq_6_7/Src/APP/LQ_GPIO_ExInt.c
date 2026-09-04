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

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@备    注：TC377只有17个固定的GPIO支持外部中断 这15个固定GPIO分为8组，每组只能选择其中一个作为外部中断管脚使用
@         0：P15_4  P10_7                                    1：P14_3  P10_8
@         2：P10_2  P02_1  P00_4                             3：P10_3  P14_1  P02_0
@         4：P15_5  P33_7                                    5：P15_8
@         6：P20_0  P11_10                                   7：P20_9  P15_1
*           外部中断服务函数在LQ_GPIO.c中  中断优先级配置在LQ_GPIO.h中 可以自行修改
*           外部中断服务函数在LQ_GPIO.c中  中断优先级配置在LQ_GPIO.h中 可以自行修改
*           外部中断服务函数在LQ_GPIO.c中  中断优先级配置在LQ_GPIO.h中 可以自行修改
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "LQ_GPIO.h"
#include "LQ_STM.h"
#include "LQ_GPIO_LED.h"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Test_GPIO_Extern_Int(void)
@功能说明：测试外部中断
@参数说明：无
@函数返回：无
@备   注：核心板上的LED固定时间P10.6翻转，LED灯闪烁，
当P11.10检测到下降沿时触发中断，P10.5翻转，LED闪烁，中断函数在LQ_GPIO.C中
可以把P11.10用杜邦线接到GND，触发中断后亮灯依次闪烁
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_GPIO_Extern(void)
{
    GPIO_LED_Init();
    PIN_Exti(P11_10, PIN_IRQ_MODE_FALLING); // 配置P11_10 下降沿触发中断
    while (1)
    {
        LED_Ctrl(LED0, RVS); // 电平翻转,LED闪烁
        Delay_Ms(500);       // 延时等待
    }
}
