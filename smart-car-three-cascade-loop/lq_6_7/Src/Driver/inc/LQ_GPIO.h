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

#ifndef __LQ_GPIO_H__
#define __LQ_GPIO_H__

#include "lq_include.h"

// GPIO 枚举端口编号，参考TC397datasheet更新，2020年11月16日 10:11:47
typedef enum //
{
    P00_0 = 0xA000,
    P00_1 = 0xA001,
    P00_2 = 0xA002,
    P00_3 = 0xA003,
    P00_4 = 0xA004,
    P00_5 = 0xA005,
    P00_6 = 0xA006,
    P00_7 = 0xA007,
    P00_8 = 0xA008,
    P00_9 = 0xA009,
    P00_10 = 0xA00A,
    P00_11 = 0xA00B,
    P00_12 = 0xA00C,

    P01_3 = 0xA103,
    P01_4 = 0xA104,
    P01_5 = 0xA105,
    P01_6 = 0xA106,
    P01_7 = 0xA107,

    P02_0 = 0xA200,
    P02_1 = 0xA201,
    P02_2 = 0xA202,
    P02_3 = 0xA203,
    P02_4 = 0xA204,
    P02_5 = 0xA205,
    P02_6 = 0xA206,
    P02_7 = 0xA207,
    P02_8 = 0xA208,
    P02_9 = 0xA209,
    P02_10 = 0xA20A,
    P02_11 = 0xA20B,

    P10_0 = 0xAA00,
    P10_1 = 0xAA01,
    P10_2 = 0xAA02,
    P10_3 = 0xAA03,
    P10_4 = 0xAA04,
    P10_5 = 0xAA05,
    P10_6 = 0xAA06,
    P10_7 = 0xAA07,
    P10_8 = 0xAA08,

    P11_0 = 0xAB00,
    P11_1 = 0xAB01,
    P11_2 = 0xAB02,
    P11_3 = 0xAB03,
    P11_4 = 0xAB04,
    P11_5 = 0xAB05,
    P11_6 = 0xAB06,
    P11_7 = 0xAB07,
    P11_8 = 0xAB08,
    P11_9 = 0xAB09,
    P11_10 = 0xAB0A,
    P11_11 = 0xAB0B,
    P11_12 = 0xAB0C,
    P11_13 = 0xAB0D,
    P11_14 = 0xAB0E,
    P11_15 = 0xAB0F,

    P12_0 = 0xAC00,
    P12_1 = 0xAC01,
    P13_0 = 0xAD00,
    P13_1 = 0xAD01,
    P13_2 = 0xAD02,
    P13_3 = 0xAD03,

    P14_0 = 0xAE00,
    P14_1 = 0xAE01,
    P14_2 = 0xAE02,
    P14_3 = 0xAE03,
    P14_4 = 0xAE04,
    P14_5 = 0xAE05,
    P14_6 = 0xAE06,
    P14_7 = 0xAE07,
    P14_8 = 0xAE08,
    P14_9 = 0xAE09,
    P14_10 = 0xAE0A,

    P15_0 = 0xAF00,
    P15_1 = 0xAF01,
    P15_2 = 0xAF02,
    P15_3 = 0xAF03,
    P15_4 = 0xAF04,
    P15_5 = 0xAF05,
    P15_6 = 0xAF06,
    P15_7 = 0xAF07,
    P15_8 = 0xAF08,

    P20_0 = 0xB400,
    P20_1 = 0xB401,
    P20_2 = 0xB402,
    P20_3 = 0xB403,
    P20_4 = 0xB404,
    P20_5 = 0xB405,
    P20_6 = 0xB406,
    P20_7 = 0xB407,
    P20_8 = 0xB408,
    P20_9 = 0xB409,
    P20_10 = 0xB40A,
    P20_11 = 0xB40B,
    P20_12 = 0xB40C,
    P20_13 = 0xB40D,
    P20_14 = 0xB40E,

    P21_0 = 0xB500,
    P21_1 = 0xB501,
    P21_2 = 0xB502,
    P21_3 = 0xB503,
    P21_4 = 0xB504,
    P21_5 = 0xB505,
    P21_6 = 0xB506,
    P21_7 = 0xB507,

    P22_0 = 0xB600,
    P22_1 = 0xB601,
    P22_2 = 0xB602,
    P22_3 = 0xB603,
    P22_4 = 0xB604,
    P22_5 = 0xB605,
    P22_6 = 0xB606,
    P22_7 = 0xB607,
    P22_8 = 0xB608,
    P22_9 = 0xB609,
    P22_10 = 0xB60A,
    P22_11 = 0xB60B,
    P22_12 = 0xB60C,

    P23_1 = 0xB701,
    P23_2 = 0xB702,
    P23_3 = 0xB703,
    P23_4 = 0xB704,
    P23_5 = 0xB705,
    P23_6 = 0xB706,
    P23_7 = 0xB707,

    P32_0 = 0xC000,
    P32_1 = 0xC001,
    P32_2 = 0xC002,
    P32_3 = 0xC003,
    P32_4 = 0xC004,
    P32_5 = 0xC005,
    P32_6 = 0xC006,
    P32_7 = 0xC007,

    P33_0 = 0xC100,
    P33_1 = 0xC101,
    P33_2 = 0xC102,
    P33_3 = 0xC103,
    P33_4 = 0xC104,
    P33_5 = 0xC105,
    P33_6 = 0xC106,
    P33_7 = 0xC107,
    P33_8 = 0xC108,
    P33_9 = 0xC109,
    P33_10 = 0xC10A,
    P33_11 = 0xC10B,
    P33_12 = 0xC10C,
    P33_13 = 0xC10D,
    P33_14 = 0xC10E,
    P33_15 = 0xC10F,

    P34_0 = 0xC200,
    P34_1 = 0xC201,
    P34_2 = 0xC202,
    P34_3 = 0xC203,
    P34_4 = 0xC204,
    P34_5 = 0xC205,

    P40_0 = 0xC800,
    P40_1 = 0xC801,
    P40_2 = 0xC802,
    P40_3 = 0xC803,
    P40_4 = 0xC804,
    P40_5 = 0xC805,
    P40_6 = 0xC806,
    P40_7 = 0xC807,
    P40_8 = 0xC808,
    P40_9 = 0xC809,
    P40_10 = 0xC80A,
    P40_11 = 0xC80B,
    P40_12 = 0xC80C,
    P40_13 = 0xC80D,
    P40_14 = 0xC80E,

} GPIO_Name_t;

/* 获取GPIO对应 MODULE 宏定义L */
#define PIN_GetModule(GPIO_NAME) (Ifx_P *)(0xF0030000u | (GPIO_NAME & 0xFF00))

/* 获取GPIO对应 管脚序列号 宏定义Q */
#define PIN_GetIndex(GPIO_NAME) (uint8)(GPIO_NAME & 0x000F)

/* GPIO模式 */
#define PIN_MODE_OUTPUT IfxPort_Mode_outputPushPullGeneral     /*!< 推挽输出  */
#define PIN_MODE_OUTPUT_OD IfxPort_Mode_outputOpenDrainGeneral /*!< 开漏输出  */
#define PIN_MODE_INPUT IfxPort_Mode_inputNoPullDevice          /*!< 浮空输入  */
#define PIN_MODE_INPUT_PULLUP IfxPort_Mode_inputPullUp         /*!< 上拉输入  */
#define PIN_MODE_INPUT_PULLDOWN IfxPort_Mode_inputPullDown     /*!< 下拉输入  */

void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode mode, uint8 output);
uint8 PIN_Read(GPIO_Name_t pin);
void PIN_Write(GPIO_Name_t pin, uint8 output);
void PIN_Dir(GPIO_Name_t pin, uint8 mode);
void PIN_Reverse(GPIO_Name_t pin);

void PIN_Exti(GPIO_Name_t pin, IfxPort_InputMode mode);
void PIN_ExtiEnable(GPIO_Name_t pin, boolean bool);

#endif /* __LQ_GPIO_H__ */