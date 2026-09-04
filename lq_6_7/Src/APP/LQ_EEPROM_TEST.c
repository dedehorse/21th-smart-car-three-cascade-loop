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

#include "LQ_EEPROM.h"
#include "lq_include.h"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Test_EEPROM(void)
@功能说明：测试演示
@参数说明：无
@函数返回：无
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_EEPROM(void)
{
    int i;

    GPIO_LED_Init();
    Display_Init(0);         // LCD初始化
    Display_CLS(U16_BLACK); // 黑色屏幕
    Display_showString(0, 0, "eeprom Test", U16_WHITE, U16_BLACK, 16);

    unsigned long u32wBuff[24];
    unsigned long u32rBuff[24];
    float f32wBuff[24];
    float f32rBuff[24];

    /* 赋值 */
    for (i = 0; i < 24; i++)
    {
        u32wBuff[i] = i * 100;
        f32wBuff[i] = i * 2.5f;
    }

    EEPROM_EraseSector(0);
    EEPROM_EraseSector(1);
    Display_showString(0, 1, "eeporm u write", U16_WHITE, U16_BLACK, 16);
    /* 写入 */
    EEPROM_Write(0, 0, u32wBuff, 24);
    Display_showString(0, 2, "eeporm f write", U16_WHITE, U16_BLACK, 16);
    EEPROM_Write(1, 0, (unsigned long *)f32wBuff, 24);
    Display_showString(0, 3, "eeporm u read", U16_WHITE, U16_BLACK, 16);
    /* 读出 */
    EEPROM_Read(0, 0, u32rBuff, 24);
    Display_showString(0, 4, "eeporm f read", U16_WHITE, U16_BLACK, 16);
    EEPROM_Read(1, 0, (unsigned long *)f32rBuff, 24);
    /* 比较 */
    for (i = 0; i < 24; i++)
    {
        if (u32wBuff[i] != u32rBuff[i])
        {
            Display_showString(0, 5, "u32error", U16_WHITE, U16_BLACK, 16);
        }
        if (f32wBuff[i] != f32rBuff[i])
        {
            Display_showString(0, 6, "ferror", U16_WHITE, U16_BLACK, 16);
        }
    }
    Display_showString(0, 7, "eeprom is ok", U16_WHITE, U16_BLACK, 16);
#pragma warning 557 // 屏蔽警告
    while (1)
        ;
#pragma warning default // 打开警告
}
