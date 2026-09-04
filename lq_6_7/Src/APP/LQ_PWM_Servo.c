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

#include "LQ_PWM_Servo.h"


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void ServoInit(void)
@功能说明：舵机初始化
@参数说明：无
@函数返回：无
@调用方法：ServoInit();
@备    注：主函数调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void ServoInit(void)
{
    ATOM_PWM_InitConfig(ATOMSERVO1, Servo_Center_Mid, Servo_Frequency); // 舵机频率为100HZ，初始值为1.5ms中值
    ATOM_PWM_InitConfig(ATOMSERVO2, Servo_Center_Mid, Servo_Frequency); // 舵机理论范围为：0.5ms--2.5ms，大多舵机实际比这个范围小
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void ServoCtrl (Servo_e Servo,uint32 duty)
@功能说明：舵机控制函数
@参数说明：Servo:选择舵机1还是2  duty：舵机占空比
@函数返回：无
@调用方法：ServoCtrl (Servo1,duty);
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void ServoCtrl(Servo_e Servo, uint32 duty)
{
    //    if (duty >= Servo_Left_Max) // 限制幅值
    //        duty = Servo_Left_Max;
    //    else if (duty <= Servo_Right_Min) // 限制幅值
    //        duty = Servo_Right_Min;
    switch (Servo)
    {
    case Servo1:
        ATOM_PWM_InitConfig(ATOMSERVO1, duty, Servo_Frequency); // 舵机频率为50HZ，初始值为1.5ms中值
        break;
    case Servo2:
        ATOM_PWM_InitConfig(ATOMSERVO2, duty, Servo_Frequency); // 舵机理论范围为：0.5ms--2.5ms，大多舵机实际比这个范围小
        break;

    case ServoALL:
        ATOM_PWM_InitConfig(ATOMSERVO1, duty, Servo_Frequency); // 舵机频率为50HZ，初始值为1.5ms中值
        ATOM_PWM_InitConfig(ATOMSERVO2, duty, Servo_Frequency); // 舵机理论范围为：0.5ms--2.5ms，大多舵机实际比这个范围小
        break;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TestServo (void)
@功能说明：舵机测试函数
@参数说明：无
@函数返回：无
@调用方法：TestServo();
@备    注：注意，一定要对舵机打角进行限制
使用龙邱母板测试流程：
1.先使用万用表测量电池电压，务必保证电池电压在7V以上，否则无力不反应！
2.然后确定舵机供电电压，SD5舵机用5V供电，S3010用6-7V供电！！！
3.把舵机的舵盘去掉，让舵机可以自由转动；
4.烧写程序并运行，让舵机转动到中值附近；如果没反应重复1-2步，或者调整舵机的PWM频率计占空比，能受控为准；
5.舵机受控后用手轻转，舵机会吱吱响，对抗转动，此时可以装上舵盘；
6.按键K0/K1确定舵机的左右转动极限，并记下来，作为后续限幅防止舵机堵转烧毁！
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_Servo(void)
{
    char txt[16];
    signed short duty = 760;
    GPIO_LED_Init();
    GPIO_KEY_Init();
    Display_Init(0);         // LCD初始化
    Display_CLS(U16_BLACK); // 蓝色屏幕
    Display_showString(0, 0, "   LQ Servo Test      ", U16_RED, U16_BLACK, 16);

    ServoInit();
    ServoCtrl(ServoALL, Servo_Center_Mid); // 中值
    while (1)
    {
        if (!KEY_Read(KEY0))
        {
            if (duty > 10) // 防止duty超
            {
                duty -= 10;
            }
        }
        if (!KEY_Read(KEY1))
        {
            duty += 10;
        }
        if (!KEY_Read(KEY2))
        {
            duty = Servo_Center_Mid;
        }
        ServoCtrl(ServoALL, duty); // 中值
        sprintf(txt, "Servo duty:%04d ", duty);
        Display_showString(1, 2, txt, U16_BLACK, U16_GREEN, 16); // 显示出库实际脉冲数，以便灵活调整
        LED_Ctrl(LEDALL, RVS);                                   // 四个LED同时闪烁;
        Delay_Ms(100);
    }
}
