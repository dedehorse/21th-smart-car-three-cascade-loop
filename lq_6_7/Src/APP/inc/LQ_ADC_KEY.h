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

#ifndef __LQ_ADC_KEY_H_
#define __LQ_ADC_KEY_H_

#include "lq_include.h"

/* 选择 按键和旋钮的ADC通道 */
#define AD_knob    ADC7   // ADC旋钮通道号
#define AD_KEY_CH  ADC9   // ADC按键通道

#define AN3_KEY    ADC3   // 一体板上的AN3按键
#define M_KEY      P33_9  // 一体板上的339按键


/* MCU 接口配置 */
#define ADK_UART_PORT       (UART1)             // 配置串口
#define ADK_UART_RX_PIN     (UART1_RX_P15_5)    // 配置串口引脚
#define ADK_UART_TX_PIN     (UART1_TX_P15_5)
#define ADK_UART_BAUD       (115200ul)          // 配置串口波特率

//定义模块号
typedef enum
{
    KEY_NONE = 0,  /* 没有按键按下 */
    KEY_UP,        /* 向上触发 */
    KEY_DOWN,	   /* 向下触发 */
    KEY_LEFT,	   /* 向左触发 */
    KEY_RIGHT,     /* 向右触发 */
    KEY_PRESS      /* 按下触发 */
}ADC_KEY_Name;


// =============================== 外部函数声明 =================================

void ADC_Key_Init(void);        // ADC初始化
uint8 Get_ADC_Key(void);        // 解析ADC按键键值
uint8 ADKey_Scan(void);         // ADC按键扫描，建议方定时器中断（推荐<=50ms）或主循环
void Test_ADC_Key(void);

#endif



