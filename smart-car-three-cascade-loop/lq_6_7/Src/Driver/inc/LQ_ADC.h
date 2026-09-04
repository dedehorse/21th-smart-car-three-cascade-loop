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

#ifndef __LQ_ADC_H__
#define __LQ_ADC_H__

#include "lq_include.h"

/*
 * adc 通道 枚举TC3X7 ADC0对应管脚 AN0 file IfxEvadc_PinMap.c
 */
typedef enum
{
    ADC0 = 0x0000,
    ADC1 = 0x0101,
    ADC2 = 0x0202,
    ADC3 = 0x0303,
    ADC4 = 0x0404,
    ADC5 = 0x0505,
    ADC6 = 0x0606,
    ADC7 = 0x0707,

    ADC8 = 0x1008,
    ADC9 = 0x1109,
    ADC10 = 0x120A,
    ADC11 = 0x130B,
    ADC12 = 0x140C,
    ADC13 = 0x150D,
    ADC14 = 0x160E,
    ADC15 = 0x170F,

    ADC16 = 0x2010,
    ADC17 = 0x2111,
    ADC18 = 0x2212,
    ADC19 = 0x2313,
    ADC20 = 0x2414,
    ADC21 = 0x2515,
    ADC22 = 0x2616,
    ADC23 = 0x2717,

    ADC24 = 0x3018,
    ADC25 = 0x3119,
    ADC26 = 0x321A,
    ADC27 = 0x331B,
    ADC28 = 0x341C,
    ADC29 = 0x351D,
    ADC30 = 0x361E,
    ADC31 = 0x371F,

    ADC32 = 0x8020,
    ADC33 = 0x8121,
    ADC34 = 0x8222,
    ADC35 = 0x8323,
    ADC36 = 0x8424,
    ADC37 = 0x8525,
    ADC38 = 0x8626,
    ADC39 = 0x8727,

    ADC40 = 0x8828,
    ADC41 = 0x8929,
    ADC42 = 0x8A2A,
    ADC43 = 0x8B2B,
    ADC44 = 0x8C2C,
    ADC45 = 0x8D2D,
    ADC46 = 0x8E2E,
    ADC47 = 0x8F2F,

    ADC48 = 0x5030,
    ADC49 = 0x5131,
    ADC50 = 0x5232,
    ADC51 = 0x5333,
    ADC52 = 0x5434,
    ADC53 = 0x5535,
    ADC54 = 0x5636,
    ADC55 = 0x5737,
    ADC56 = 0x6038,
    ADC57 = 0x6139,
    ADC58 = 0x623A,
    ADC59 = 0x633B,
    ADC60 = 0x643C,
    ADC61 = 0x653D,
    ADC62 = 0x663E,
    ADC63 = 0x673F,
    ADC64 = 0x7040,
    ADC65 = 0x7141,
    ADC66 = 0x7242,
    ADC67 = 0x7343,
    ADC68 = 0x7444,
    ADC69 = 0x7545,
    ADC70 = 0x7646,
    ADC71 = 0x7747,

} ADC_Channel_t;
#define ADC_GROUPS_NUM 12
#define ADC_CHANNELS_NUM 72
/* 获取ADC对应组 */
#define ADC_GetGroup(ADC_Channel) ((ADC_Channel & 0xF000) >> 12)
/* 获取ADC对应通道 */
#define ADC_GetChannel(ADC_Channel) ((ADC_Channel & 0x0F00) >> 8)
/* 获取ADC对应管脚序列号 宏定义 */
#define ADC_GetIndex(ADC_Channel) (unsigned char)(ADC_Channel & 0x00FF)

void ADC_InitConfig(ADC_Channel_t channel, unsigned long Freq_Hz);
unsigned short ADC_Read(ADC_Channel_t chn);
unsigned short ADC_ReadAverage(ADC_Channel_t chn, unsigned char count);

#endif /* __LQ_ADC_H__ */