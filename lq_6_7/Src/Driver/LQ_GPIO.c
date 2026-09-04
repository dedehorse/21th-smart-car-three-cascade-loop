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

#include "LQ_GPIO.h"

/*************************************************************************
 *  函数名称：void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode mode, unsigned char output)
 *  功能说明：GPIO初始化函数
 *  参数说明：pin      ： 管脚标号    lq_gpio.h中定义
 *            mode     ： GPIO 模式   lq_gpio.h中的宏定义中选择
 *            output   ： 输出模式时，输出电平 1：高电平  0：低电平
 *  函数返回：无
 *  举    例： PIN_InitConfig(P00_0, PIN_MODE_OUTPUT, 1);  //P00_0初始化推挽输出 高电平
 *************************************************************************/
void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode mode, unsigned char output)
{

    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    /* 配置 GPIO模式 */
    IfxPort_setPinMode(port, pinIndex, mode);

    IfxPort_setPinPadDriver(port, pinIndex, IfxPort_PadDriver_cmosAutomotiveSpeed2);

    /* GPIO输出模式时 输出状态 */
    if (0 == output)
    {
        IfxPort_setPinState(port, pinIndex, IfxPort_State_low);
    }
    else
    {
        IfxPort_setPinState(port, pinIndex, IfxPort_State_high);
    }
}

/*************************************************************************
 *  函数名称：unsigned char PIN_Read(GPIO_Name_t pin)
 *  功能说明：读取GPIO电平函数
 *  参数说明：pin      ： 管脚标号    lq_gpio.h中定义*
 *  函数返回：无
 *  举    例：uint8_t pinState = PIN_Read(P00_0);  //读取P00_0 的电平状态
 *************************************************************************/
unsigned char PIN_Read(GPIO_Name_t pin)
{
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);
    return IfxPort_getPinState(port, pinIndex);
}

/*************************************************************************
 *  函数名称：void PIN_Write(GPIO_Name_t pin, unsigned char output)
 *  功能说明：设置GPIO输出状态
 *  参数说明：pin      ： 管脚标号    lq_gpio.h中定义*
 *            output   ： 输出模式时，输出电平 1：高电平  0：低电平
 *  函数返回：无
 *  举    例：PIN_Write(P00_0, 1);//P00_0置为高电平
 *************************************************************************/
void PIN_Write(GPIO_Name_t pin, unsigned char output)
{
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    /* GPIO输出模式时 输出状态 */
    if (0 == output)
    {
        IfxPort_setPinState(port, pinIndex, IfxPort_State_low);
    }
    else
    {
        IfxPort_setPinState(port, pinIndex, IfxPort_State_high);
    }
}

/*************************************************************************
 *  函数名称：void PIN_Dir(GPIO_Name_t pin, unsigned char mode)
 *  功能说明：设置GPIO 输入输出方向
 *  参数说明：pin      ： 管脚标号    lq_gpio.h中定义
 *            mode     ： GPIO 模式   lq_gpio.h中的宏定义中选择
 *  函数返回：无
 *  举    例：PIN_Dir(P00_0, 1);//P00_0设置为输出
 *************************************************************************/
void PIN_Dir(GPIO_Name_t pin, unsigned char mode)
{
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    if (0 == mode)
    {
        /* 配置 GPIO模式 */
        IfxPort_setPinMode(port, pinIndex, PIN_MODE_INPUT);
    }
    else
    {
        /* 配置 GPIO模式 */
        IfxPort_setPinMode(port, pinIndex, PIN_MODE_OUTPUT);
    }
}

/*************************************************************************
 *  函数名称：void PIN_Reverse(GPIO_Name_t pin)
 *  功能说明：GPIO电平翻转函数，使用前请先初始化
 *  参数说明：pin      ： 管脚标号    lq_gpio.h中定义*
 *  函数返回：无
 *  举    例：PIN_Reverse(P00_0); //翻转P00_0管脚的电平状态
 *************************************************************************/
void PIN_Reverse(GPIO_Name_t pin)
{
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    IfxPort_togglePin(port, pinIndex);
}