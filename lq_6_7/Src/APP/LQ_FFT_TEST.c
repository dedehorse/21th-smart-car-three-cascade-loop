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


#include <CompilerTasking.h>
#include <Ifx_FftF32.h>
#include <Ifx_Types.h>
#include <math.h>

#include "LQ_GPIO_LED.h"

// 源波形，256位对齐
IFX_ALIGN(256)
cfloat32 sfftIn[1024];
// IFFT变换结果，256位对齐
IFX_ALIGN(256)
cfloat32 sifftOut[1024];
// FFT变换结果，256位对齐
IFX_ALIGN(256)
cfloat32 sfftOut[1024];
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Test_SoftFft(void)
@功能说明：测试软件FFT变换
@参数说明：无
@函数返回：无
@备    注：测试FFT变换和IFFT变换
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_SoftFft(void)
{
    unsigned short Fs = 1024; // 信号频率
    float T = 1.0f / Fs;      // 信号周期
    unsigned short L = 1024;  // 长度
    /* 做一个50Hz和120Hz的正玄叠加信号  */
    int i = 0;
    GPIO_LED_Init();
    for (i = 0; i < L; i++)
    {
        sfftIn[i].real = 70 * sin(2 * IFX_PI * 50 * T * i) + 100 * sin(2 * IFX_PI * 120 * T * i);
        sfftIn[i].imag = 0;
    }
    while (1)
    {
        // FFT
        Ifx_FftF32_radix2(sfftOut, sfftIn, 1024);
        // IFFT  注意 这里IFFT输出结果是实际结果的1024倍
        Ifx_FftF32_radix2I(sifftOut, sfftOut, 1024);
        LED_Ctrl(LED0, RVS); // 电平翻转,LED闪烁
    }
}
