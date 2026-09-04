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

#include "LQ_PWM_BLSmotor.h"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void BLSmotorInit(void)
@功能说明：无感无刷初始化
@参数说明：无
@函数返回：无
@调用方法：BLSmotorInit();
@备    注：主函数调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void BLSmotorInit(void)
{
    ATOM_PWM_InitConfig(ATOMBLS1, 500, Blsmotor_Frequency); // 无刷频率为50HZ，初始值为1ms
    ATOM_PWM_InitConfig(ATOMBLS2, 500, Blsmotor_Frequency); // 无刷理论范围为：1ms--2ms
    ATOM_PWM_InitConfig(ATOMBLS3, 500, Blsmotor_Frequency); // 无刷理论范围为：1ms--2ms
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void BlsmotorCtrl(Bls_e BLS, uint32 duty)
@功能说明：无感无刷控制函数
@参数说明：BLS:选择无刷1还是2  duty：舵机占空比
@函数返回：无
@调用方法：BlsmotorCtrl (Servo1,duty);
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void BlsmotorCtrl(Bls_e BLS, uint32 duty)
{
    if (duty >= Blsmotor_Max) // 限制幅值
        duty = Blsmotor_Max;
    else if (duty <= Blsmotor_Min) // 限制幅值
        duty = Blsmotor_Min;
    switch (BLS)
    {
    case BLS1:
        ATOM_PWM_InitConfig(ATOMBLS1, duty, Blsmotor_Frequency); // 舵机频率为50HZ，初始值为1ms中值
        break;
    case BLS2:
        ATOM_PWM_InitConfig(ATOMBLS2, duty, Blsmotor_Frequency); // 无刷理论范围为：1ms--2ms
        break;
    case BLS3:
        ATOM_PWM_InitConfig(ATOMBLS3, duty, Blsmotor_Frequency); // 无刷理论范围为：1ms--2ms
        break;
    case BLSALL:
        ATOM_PWM_InitConfig(ATOMBLS1, duty, Blsmotor_Frequency); // 无刷理论范围为：1ms--2ms
        ATOM_PWM_InitConfig(ATOMBLS2, duty, Blsmotor_Frequency); // 无刷理论范围为：1ms--2ms
        ATOM_PWM_InitConfig(ATOMBLS3, duty, Blsmotor_Frequency); // 无刷理论范围为：1ms--2ms
        break;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void BLS3_RampStartup(void)
@功能说明：BLS3风扇缓启动, duty 510→650, 完成后使能运动电机
@参数说明：无
@函数返回：无
@调用方法：BLS3_RampStartup();
@备    注：需在 CCU6_InitConfig 之前调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void BLS3_RampStartup(void)
{
    uint32 duty;

    BLSmotorInit();
    BlsmotorCtrl(BLS3, BLS_RAMP_START_DUTY);

    Display_showString(1, 0, "  BLS3 Ramping...     ", U16_WHITE, U16_BLACK, 16);

    for (duty = BLS_RAMP_START_DUTY; duty <= BLS_RAMP_END_DUTY; duty += BLS_RAMP_STEP)
    {
        char txt[24];
        BlsmotorCtrl(BLS3, duty);
        sprintf(txt, "  Duty: %lu / %d        ", duty, BLS_RAMP_END_DUTY);
        Display_showString(1, 2, txt, U16_WHITE, U16_BLACK, 16);
        Delay_Ms(BLS_RAMP_INTERVAL_MS);
    }

    BlsmotorCtrl(BLS3, BLS_RAMP_END_DUTY);
    g_motors_enabled = 1;

    Display_showString(1, 0, "  BLS3: READY         ", U16_GREEN, U16_BLACK, 16);
    Display_showString(1, 1, "  Motors: ENABLED     ", U16_GREEN, U16_BLACK, 16);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TestBlsMotor(void)
@功能说明：无感无刷测试函数
@参数说明：无
@函数返回：无
@调用方法：TestBlsMotor();    BLS3 --》 P21_2
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_BlsMotor(void)
{
    char txt[16];
    signed short duty = 650;
    GPIO_LED_Init();
    GPIO_KEY_Init();

    Display_Init(2);         // LCD初始化 
    Display_CLS(U16_BLACK);  // 黑色屏幕 
    Display_showString(0, 0, "   LQ BLSmotor Test      ", U16_RED, U16_BLACK, 16);
    BLSmotorInit();
    while (1)
    {
        if (!KEY_Read(KEY0))
        {
            if (duty > 490) // 防止duty超
            {
                duty -= 10;
            }
        }
        if (!KEY_Read(KEY1))
        {
            duty = 510;
        }
        if (!KEY_Read(KEY2))
        {
            if (duty < 1510) // 防止duty超
            {
                duty += 10;
            }
        }
        BlsmotorCtrl(BLS3, duty);
        sprintf(txt, "BLS duty:%04d ", duty);
        Display_showString(1, 2, txt, U16_BLACK, U16_GREEN, 16); // 显示出库实际脉冲数，以便灵活调整
        LED_Ctrl(LEDALL, RVS);                                   // 四个LED同时闪烁;
        Delay_Ms(100);
    }
}
