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

#include "lq_include.h"
#include "LQ_PWM_BLDC.h"

int BLDC_duty = 0;
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void MotorInit(void)
@功能说明：电机PWM初始化
@参数说明：无
@函数返回：无
@调用方法：MotorInit(100);
@备    注：无刷初始化
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void BLDCInit(void)
{
    ATOM_PWM_InitConfig(BLDC_AP, 0, BLDC_MOTOR_FREQUENCY);
    ATOM_PWM_InitConfig(BLDC_BP, 0, BLDC_MOTOR_FREQUENCY);
    ATOM_PWM_InitConfig(BLDC_CP, 0, BLDC_MOTOR_FREQUENCY);

    PIN_InitConfig(BLDC_AI, PIN_MODE_OUTPUT, 0);
    PIN_InitConfig(BLDC_BI, PIN_MODE_OUTPUT, 0);
    PIN_InitConfig(BLDC_CI, PIN_MODE_OUTPUT, 0);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void BLDC_HallInit(void);
@功能说明：初始化霍尔引脚及编码器接口
@参数说明：无
@函数返回：无
@修改时间：2022/03/15
@备    注：编码器接口同时为霍尔引脚状态读取接口,霍尔输入管脚可在LQ_PWM_BLDC.h中修改宏定义
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void BLDC_HallInit(void)
{
    //   PIN_InitConfig(BLDC_Hall_A, IfxPort_Mode_inputPullUp, 1);
    //   PIN_InitConfig(BLDC_Hall_B, IfxPort_Mode_inputPullUp, 1);
    PIN_InitConfig(BLDC_Hall_C, IfxPort_Mode_inputPullUp, 1);
    ENC_InitConfig(ENC4_InPut_P20_3, ENC4_Dir_P20_0); // BC相使用 编码器4接口
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：uint8_t Get_Hall_State(void);
@功能说明：读取霍尔状态
@参数说明：无
@函数返回：hall_state（霍尔状态）
@修改时间：2022/03/15
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8 Get_Hall_State(void)
{
    uint8 hall_state = 0;

    if (PIN_Read(BLDC_Hall_A)) // F11/* 读取霍尔传感器 U 的状态 */
    {
        hall_state |= 0x01U << 0;
    }
    if (PIN_Read(BLDC_Hall_B)) // F12/* 读取霍尔传感器 V 的状态 */
    {
        hall_state |= 0x01U << 1;
    }
    if (PIN_Read(BLDC_Hall_C)) // F13 /* 读取霍尔传感器 W 的状态 */
    {
        hall_state |= 0x01U << 2;
    }
    return hall_state;
}

/////**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
//@函数名称：void LQ_BLDCCorotation(void);
//@功能说明：控制无刷电机正转
//@参数说明：无
//@函数返回：无
//@修改时间：2024/4/5
//@备    注：
// QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
void LQ_BLDCCorotation(void)
{
    switch (Get_Hall_State())
    {
    case 1:
        // B->C
        ATOM_PWM_SetDuty(BLDC_AP, 0, BLDC_MOTOR_FREQUENCY);         // 0
        ATOM_PWM_SetDuty(BLDC_BP, BLDC_duty, BLDC_MOTOR_FREQUENCY); // BLDC_duty
        ATOM_PWM_SetDuty(BLDC_CP, 0, BLDC_MOTOR_FREQUENCY);         // 0
        PIN_Write(BLDC_AI, 0);                                 // 0
        PIN_Write(BLDC_BI, 0);                                 // 0
        PIN_Write(BLDC_CI, 1);                                 // 1
        break;

    case 2:
        // C->A
        ATOM_PWM_SetDuty(BLDC_AP, 0, BLDC_MOTOR_FREQUENCY);         // P1_PWM_P输出0
        ATOM_PWM_SetDuty(BLDC_BP, 0, BLDC_MOTOR_FREQUENCY);         // P3_PWM_P输出0
        ATOM_PWM_SetDuty(BLDC_CP, BLDC_duty, BLDC_MOTOR_FREQUENCY); // P5_PWM_P输出PWM
        PIN_Write(BLDC_AI, 1);                                 // 关闭
        PIN_Write(BLDC_BI, 0);                                 // 关闭
        PIN_Write(BLDC_CI, 0);                                 // 打开
        break;
        //
    case 3:
        // B->A
        ATOM_PWM_SetDuty(BLDC_AP, 0, BLDC_MOTOR_FREQUENCY);         // P1_PWM_P输出PWM
        ATOM_PWM_SetDuty(BLDC_BP, BLDC_duty, BLDC_MOTOR_FREQUENCY); // P3_PWM_P输出0
        ATOM_PWM_SetDuty(BLDC_CP, 0, BLDC_MOTOR_FREQUENCY);         // P5_PWM_P输出0
        PIN_Write(BLDC_AI, 1);                                 // 关闭
        PIN_Write(BLDC_BI, 0);                                 // 打开
        PIN_Write(BLDC_CI, 0);                                 // 关闭
        break;

    case 4:
        // A->B
        ATOM_PWM_SetDuty(BLDC_AP, BLDC_duty, BLDC_MOTOR_FREQUENCY); // P1_PWM_P输出0
        ATOM_PWM_SetDuty(BLDC_BP, 0, BLDC_MOTOR_FREQUENCY);         // P3_PWM_P输出PWM
        ATOM_PWM_SetDuty(BLDC_CP, 0, BLDC_MOTOR_FREQUENCY);         // P5_PWM_P输出0
        PIN_Write(BLDC_AI, 0);                                 // 打开
        PIN_Write(BLDC_BI, 1);                                 // 关闭
        PIN_Write(BLDC_CI, 0);                                 // 关闭
        break;

    case 5:
        // A->C
        ATOM_PWM_SetDuty(BLDC_AP, BLDC_duty, BLDC_MOTOR_FREQUENCY); // P1_PWM_P输出0
        ATOM_PWM_SetDuty(BLDC_BP, 0, BLDC_MOTOR_FREQUENCY);         // P3_PWM_P输出PWM
        ATOM_PWM_SetDuty(BLDC_CP, 0, BLDC_MOTOR_FREQUENCY);         // P5_PWM_P输出0
        PIN_Write(BLDC_AI, 0);                                 // 打开
        PIN_Write(BLDC_BI, 0);                                 // 关闭
        PIN_Write(BLDC_CI, 1);                                 // 关闭
        break;

    case 6:
        // C->B
        ATOM_PWM_SetDuty(BLDC_AP, 0, BLDC_MOTOR_FREQUENCY);         // P1_PWM_P输出0
        ATOM_PWM_SetDuty(BLDC_BP, 0, BLDC_MOTOR_FREQUENCY);         // P3_PWM_P输出0
        ATOM_PWM_SetDuty(BLDC_CP, BLDC_duty, BLDC_MOTOR_FREQUENCY); // P5_PWM_P输出PWM
        PIN_Write(BLDC_AI, 0);                                 // 关闭
        PIN_Write(BLDC_BI, 1);                                 // 关闭
        PIN_Write(BLDC_CI, 0);                                 // 打开
        break;

    default:
        LQ_BLDCStop();
        break;
    }
}

/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCReversal(void);
@功能说明：控制无刷电机反转
@参数说明：无
@函数返回：无
@修改时间：2024/4/5
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
void LQ_BLDCReversal(void)
{
    switch (Get_Hall_State())
    {
    case 1:
        // C->B
        ATOM_PWM_SetDuty(BLDC_AP, 0, BLDC_MOTOR_FREQUENCY);          // P1_PWM_P关闭
        ATOM_PWM_SetDuty(BLDC_BP, 0, BLDC_MOTOR_FREQUENCY);          // P3_PWM_P输出0
        ATOM_PWM_SetDuty(BLDC_CP, -BLDC_duty, BLDC_MOTOR_FREQUENCY); // P5_PWM_P输出PWM
        PIN_Write(BLDC_AI, 0);                                  // 关闭
        PIN_Write(BLDC_BI, 1);                                  // 关闭
        PIN_Write(BLDC_CI, 0);                                  // 打开
        break;

    case 2:
        // A->C
        ATOM_PWM_SetDuty(BLDC_AP, -BLDC_duty, BLDC_MOTOR_FREQUENCY); // P231输出PWM
        ATOM_PWM_SetDuty(BLDC_BP, 0, BLDC_MOTOR_FREQUENCY);          // P3_PWM_P关闭
        ATOM_PWM_SetDuty(BLDC_CP, 0, BLDC_MOTOR_FREQUENCY);          // P5_PWM_P输出0
        PIN_Write(BLDC_AI, 0);                                  // 打开
        PIN_Write(BLDC_BI, 0);                                  // 关闭
        PIN_Write(BLDC_CI, 1);                                  // 关闭
        break;

    case 3:
        // A->B
        ATOM_PWM_SetDuty(BLDC_AP, -BLDC_duty, BLDC_MOTOR_FREQUENCY); // P1_PWM_P输出0
        ATOM_PWM_SetDuty(BLDC_BP, 0, BLDC_MOTOR_FREQUENCY);          // P3_PWM_P输出PWM
        ATOM_PWM_SetDuty(BLDC_CP, 0, BLDC_MOTOR_FREQUENCY);          // P5_PWM_P输出0
        PIN_Write(BLDC_AI, 0);                                  // 打开
        PIN_Write(BLDC_BI, 1);                                  // 关闭
        PIN_Write(BLDC_CI, 0);                                  // 关闭
        break;

    case 4:
        // B->A
        ATOM_PWM_SetDuty(BLDC_AP, 0, BLDC_MOTOR_FREQUENCY);          // P1_PWM_P输出0
        ATOM_PWM_SetDuty(BLDC_BP, -BLDC_duty, BLDC_MOTOR_FREQUENCY); // P3_PWM_P输出PWM
        ATOM_PWM_SetDuty(BLDC_CP, 0, BLDC_MOTOR_FREQUENCY);          // P5_PWM_P关闭
        PIN_Write(BLDC_AI, 1);                                  // 关闭
        PIN_Write(BLDC_BI, 0);                                  // 打开
        PIN_Write(BLDC_CI, 0);                                  // 关闭
        break;

    case 5:
        // C->A
        ATOM_PWM_SetDuty(BLDC_AP, 0, BLDC_MOTOR_FREQUENCY);          // P1_PWM_P输出PWM
        ATOM_PWM_SetDuty(BLDC_BP, 0, BLDC_MOTOR_FREQUENCY);          // P3_PWM_P输出0
        ATOM_PWM_SetDuty(BLDC_CP, -BLDC_duty, BLDC_MOTOR_FREQUENCY); // P5_PWM_P输出0
        PIN_Write(BLDC_AI, 1);                                  // 关闭
        PIN_Write(BLDC_BI, 0);                                  // 关闭
        PIN_Write(BLDC_CI, 0);                                  // 打开
        break;

    case 6:
        // B->C
        ATOM_PWM_SetDuty(BLDC_AP, 0, BLDC_MOTOR_FREQUENCY);          // P1_PWM_P输出PWM
        ATOM_PWM_SetDuty(BLDC_BP, -BLDC_duty, BLDC_MOTOR_FREQUENCY); // P3_PWM_P输出0
        ATOM_PWM_SetDuty(BLDC_CP, 0, BLDC_MOTOR_FREQUENCY);          // P5_PWM_P输出0
        PIN_Write(BLDC_AI, 0);                                  // 关闭
        PIN_Write(BLDC_BI, 0);                                  // 打开
        PIN_Write(BLDC_CI, 1);                                  // 关闭
        break;
    default:
        LQ_BLDCStop();
        break;
    }
}

/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCStop(void);
@功能说明：关闭无刷电机，停止运行
@参数说明：无
@函数返回：无
@修改时间：2024/4/5
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
void LQ_BLDCStop(void)
{
    PIN_Write(BLDC_AI, 1); // 停止时，默认下桥打开，有刹车功能，推动时有阻力
    PIN_Write(BLDC_BI, 1); // 输出高电平，下桥打开，启用刹车，注意该刹车为停车后的刹车
    PIN_Write(BLDC_CI, 1); // 不启用刹车时，将电平改为0即可，即P2P4P6输出低电平时，刹车失效
    ATOM_PWM_SetDuty(BLDC_AP, 0, BLDC_MOTOR_FREQUENCY);
    ATOM_PWM_SetDuty(BLDC_BP, 0, BLDC_MOTOR_FREQUENCY);
    ATOM_PWM_SetDuty(BLDC_CP, 0, BLDC_MOTOR_FREQUENCY);
    BLDC_duty = 0;
}

/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCCtrl(void);
@功能说明：控制无刷电机
@参数说明：无
@函数返回：无
@修改时间：2024/4/5
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
void LQ_BLDCCtrl(void)
{

    if (BLDC_duty > 8000)
        BLDC_duty = 8000;
    else if (BLDC_duty < -8000)
        BLDC_duty = -8000;
    if (BLDC_duty > 0)
    {
        LQ_BLDCCorotation();
    }
    else if (BLDC_duty < 0)
    {
        LQ_BLDCReversal();
    }
    else
    {
        LQ_BLDCStop();
    }
}

int S_PWM = 0;
unsigned int RFx = 0;
int ENC = 0, S_ENC = 0;
float MPS = 0, Vel = 0;
int32_t NowTime = 0;
float EMC_MAX = 0;
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TestMotorBLDC (void);
@功能说明：无刷电机测试函数
@参数说明：无
@函数返回：无
@修改时间：2022/03/15
@备    注：
@接线说明：
驱动控制接线：驱动板     母板
              P1       23.1
              P2       21.4
              P3       32.4
              P4       21.3
              P5       21.2
              P6       20.8
霍尔接线：   驱动板      母板
             HA        20.3
             HB        20.0
             HC        00.5
电机接线：   驱动板      电机
             A          橙
             B          黄
             C          蓝
LQ_TC2&3MBGV7母板对应LQ_BLDC24VproV2驱动板只需要将信号线使用2.5灰排线连接，霍尔线使用2.3灰排线连接。
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_MotorBLDC(void)
{
    char txt[32];
    GPIO_LED_Init();
    GPIO_KEY_Init();
    BLDC_HallInit();
    Display_Init(0);
    Display_CLS(U16_WHITE);
    BLDCInit();
    CCU6_InitConfig(CCU60, CCU6_Channel1, 20 * 1000); // 20us进入一次中断
    Display_showString(0, 0, "   LQ BLDCmotor Test      ", U16_RED, U16_WHITE, 16);
    while (1)
    {
        if (KEY_Read(KEY0) == 0) // 按下KEY0键，占空比减小
        {
            if (BLDC_duty > -ATOM_PWM_MAX)
                BLDC_duty -= 50;
            while (KEY_Read(KEY0) == 0)
                ;
        }
        if (KEY_Read(KEY2) == 0) // 按下KEY2键，占空比加大
        {
            if (BLDC_duty < ATOM_PWM_MAX) // 满占空比为12500
                BLDC_duty += 50;
            while (KEY_Read(KEY2) == 0)
                ;
        }
        if (KEY_Read(KEY1) == 0) // 按下KEY1键，占空比中值
        {
            BLDC_duty = 500;
        }

        sprintf(txt, "HALL:%05d  BLDC:%05d  ", Get_Hall_State(), BLDC_duty);
        Display_showString(0, 3, txt, U16_RED, U16_BLUE, 16); // 将txt中 内容显示出来
    }
}
