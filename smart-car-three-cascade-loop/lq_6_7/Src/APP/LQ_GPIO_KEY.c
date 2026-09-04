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

#include <LQ_GPIO_KEY.h>
#include "LQ_GPIO.h"
#include "LQ_STM.h"
#include "LQ_GPIO_LED.h"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void GPIO_KEY_Init(void)
@功能说明：按键GPIO初始化函数
@参数说明：无
@函数返回：无
@调用方法：GPIO_KEY_Init();
@备    注：管脚：GPIOA12
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GPIO_KEY_Init(void)
{
    // 初始化,输入口，高电平
    PIN_InitConfig(KEY0p, PIN_MODE_INPUT, 1);
    PIN_InitConfig(KEY1p, PIN_MODE_INPUT, 1);
    PIN_InitConfig(KEY2p, PIN_MODE_INPUT, 1);
    PIN_InitConfig(DSW0p, PIN_MODE_INPUT, 1);
    PIN_InitConfig(DSW1p, PIN_MODE_INPUT, 1);
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：unsigned char KEY_Read(KEYn_e KEYno)
@功能说明：读取按键状态
@参数说明：KEYn_e KEYno按键编号
@函数返回：按键状态，0/1
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char KEY_Read(KEYn_e KEYno)
{
    switch (KEYno)
    {
    case KEY0:
        return PIN_Read(KEY0p); // 母板上按键0
        break;

    case KEY1:
        return PIN_Read(KEY1p); // 母板上按键1
        break;

    case KEY2:
        return PIN_Read(KEY2p); // 母板上按键2
        break;

    case DSW0:
        return PIN_Read(DSW0p); // 母板上拨码开关0
        break;

    case DSW1:
        return PIN_Read(DSW1p); // 母板上拨码开关1
        break;
    default:
        return 0XFF;
    }
    return 0;
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Test_GPIO_KEY(void)
@功能说明：测试程序
@参数说明：无
@函数返回：无
@备    注：按下KEY0/1/2任意键 LED全亮
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_GPIO_KEY(void)
{
    unsigned char k = 0xFF;

    GPIO_LED_Init();       // LED初始化
    LED_Ctrl(LEDALL, OFF); // 关闭所有的LED
    GPIO_KEY_Init();       // 按键初始化
    while (1)
    {
        k = KEY_Read(KEY0) & KEY_Read(KEY1) & KEY_Read(KEY2);

        if (k == 0)
            LED_Ctrl(LEDALL, ON); // 按下KEY0/1/2任意键 LED全亮
        else
            LED_Ctrl(LEDALL, OFF); // 松开KEY0/1/2任意键 LED全灭

        Delay_Ms(100); // 延时等待
    }
}
