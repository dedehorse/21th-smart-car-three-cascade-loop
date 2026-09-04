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

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Test_LQDMP(void)
@功能说明：dmp测试函数
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@调用方法：Test_LQDMP();
@备    注：主函数调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "lq_include.h"
void Test_LQ6050_DMP(void)
{
    char txt[32];

    Display_Init(0); // 屏幕初始化

    IIC_Init(); // 初始化IIC
    Delay_Ms(200);
    Display_CLS(U16_BLACK); // 蓝色屏幕

    Display_showString(15, 0, "LQ MPU6050_DMP_Test", U16_WHITE, U16_BLACK, 24);
    Delay_Ms(100); // 字符串显示
    LQ_DMP_Init();
    Delay_Ms(100);

    while (1)
    {
        LQ_DMP_Read();

        sprintf((char *)txt, "Pitch:%.2f", Pitch);
        Display_showString(0, 1, txt, U16_WHITE, U16_BLACK, 24);
        sprintf((char *)txt, "Roll:%.2f", Roll);
        Display_showString(0, 2, txt, U16_WHITE, U16_BLACK, 24);
        sprintf((char *)txt, "Yaw:%.2f", Yaw);
        Display_showString(0, 3, txt, U16_WHITE, U16_BLACK, 24);
        sprintf((char *)txt, "ax:%06d", accel[0]);
        Display_showString(0, 4, txt, U16_WHITE, U16_BLACK, 24);
        sprintf((char *)txt, "ay:%06d", accel[1]);
        Display_showString(0, 5, txt, U16_WHITE, U16_BLACK, 24);
        sprintf((char *)txt, "az:%06d", accel[2]);
        Display_showString(0, 6, txt, U16_WHITE, U16_BLACK, 24);
        sprintf((char *)txt, "gx:%06d", gyro[0]);
        Display_showString(0, 7, txt, U16_WHITE, U16_BLACK, 24);
        sprintf((char *)txt, "gy:%06d", gyro[1]);
        Display_showString(0, 8, txt, U16_WHITE, U16_BLACK, 24);
        sprintf((char *)txt, "gz:%06d", gyro[2]);
        Display_showString(0, 9, txt, U16_WHITE, U16_BLACK, 24);
    }
}
