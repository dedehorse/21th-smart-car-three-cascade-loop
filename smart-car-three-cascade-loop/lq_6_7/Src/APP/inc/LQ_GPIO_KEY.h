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


#ifndef __LQ_KEY_H_
#define __LQ_KEY_H_

// 定义模块号
typedef enum
{
    KEY0 = 0, // 母板上按键0
    KEY1 = 1,
    KEY2 = 2,
    DSW0, // 母板上拨码开关0
    DSW1
} KEYn_e;

typedef enum
{
    LOW = 0,     // 按下
    HIGH = 1,    // 松开
    FAIL = 0xff, // 错误
} KEYs_e;

// 定义的管脚要对应实际按键
#define KEY0p P22_0  // 母板上按键0P22_0
#define KEY1p P33_9  // 母板上按键1
#define KEY2p P22_2  // 母板上按键2

#define DSW0p P33_9  // 母板上拨码开关0
#define DSW1p P33_11 // 母板上拨码开关1

void GPIO_KEY_Init(void);
unsigned char KEY_Read(KEYn_e KEYno);
void Test_GPIO_KEY(void);
#endif /* __LQ_KEY_H_ */
