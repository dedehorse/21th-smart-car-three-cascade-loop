/*******************************************************************************
 * @file                本文件是LQ_TC387_Software_Library 软件开源库文件的一部分
 * @author              chiusir
 * @email               chiusir@163.com
 * @version             V2.0.0
 * @update              2026年3月24日
 * @copyright           版权所有 (C) 2025-2026 北京龙邱科技有限公司
 * @website             http://www.lqist.cn
 * @taobao              http://longqiu.taobao.com
 *
 * @brief               龙邱科技 LQ_TC387核心板驱动库声明
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
#include "irq.h"

extern uint8_t g_motors_enabled;

/***************************定时器中断回调函数**********************************/
IFX_INTERRUPT(CCU60_CH0_IRQHandler, CCU60_VECTABNUM, CCU60_CH0_PRIORITY);
IFX_INTERRUPT(CCU60_CH1_IRQHandler, CCU60_VECTABNUM, CCU60_CH1_PRIORITY);
IFX_INTERRUPT(CCU61_CH0_IRQHandler, CCU61_VECTABNUM, CCU61_CH0_PRIORITY);
IFX_INTERRUPT(CCU61_CH1_IRQHandler, CCU61_VECTABNUM, CCU61_CH1_PRIORITY);

// CCU60_CH0中断服务函数
#if SPEED_LOOP_DEBUG
/* ============ 速度环调试版 ISR ============ */
volatile uint8_t vofa_flag = 0;

void CCU60_CH0_IRQHandler(void)
{
    IfxCpu_enableInterrupts();
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t12PeriodMatch);

    /* ─── 内环: 编码器 + 速度环 @ 1ms ─── */
    Encoder_Update();
    if (!g_test_active && g_motors_enabled)
        SpeedLoop_Update(Get_Current_SpeedL(), Get_Current_SpeedR());

    /* ─── VOFA发送标志: 每10ms置位 = 100Hz ─── */
    {
        static uint8_t tick = 0;
        if (++tick >= 10)
        {
            vofa_flag = 1;
            tick = 0;
        }
    }
}
#elif GYRO_LOOP_DEBUG
/* ============ 角速度环调试版 ISR ============ */
volatile uint8_t vofa_flag = 0;

void CCU60_CH0_IRQHandler(void)
{
    IfxCpu_enableInterrupts();
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t12PeriodMatch);

    /* ─── 内环: 编码器 + 速度环 @ 1ms ─── */
    Encoder_Update();
    if (!g_test_active && g_motors_enabled)
        SpeedLoop_Update(Get_Current_SpeedL(), Get_Current_SpeedR());

    /* ─── 姿态解算 + 角速度环 @ 2ms (500Hz, 与赛道模式一致) ─── */
    {
        static uint8_t gyro_cnt = 0;
        gyro_cnt++;
        if (gyro_cnt >= 2)
        {
            Attitude_get();
            if (!g_test_active && g_motors_enabled)
                Gyro_Loop_Update(Get_Current_Gyro());
            gyro_cnt = 0;
        }
    }

    /* ─── VOFA发送标志: 每5ms置位 = 200Hz ─── */
    {
        static uint8_t tick = 0;
        if (++tick >= 5)
        {
            vofa_flag = 1;
            tick = 0;
        }
    }
}
#elif DIR_LOOP_DEBUG
/* ============ 方向环调试版 ISR: 完整三级串级 ============ */
volatile uint8_t vofa_flag = 0;
volatile float g_dir_dev = 0;  /* 人工设定偏差(像素) */

void CCU60_CH0_IRQHandler(void)
{
    IfxCpu_enableInterrupts();
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t12PeriodMatch);

    static uint8_t cnt = 0;
    cnt++;

    /* ─── 内环: 编码器 + 速度环 @ 1ms ─── */
    Encoder_Update();
    if (!g_test_active && g_motors_enabled)
        SpeedLoop_Update(Get_Current_SpeedL(), Get_Current_SpeedR());

    /* ─── 中环: 姿态 + 角速度环 @ 2ms (500Hz) ─── */
    if (cnt % 2 == 0)
    {
        Attitude_get();
        if (!g_test_active && g_motors_enabled)
            Gyro_Loop_Update(Get_Current_Gyro());
    }

    /* ─── 外环: 方向环 @ 5ms (200Hz), 使用固定偏差 ─── */
    if (cnt % 5 == 0)
    {
        if (!g_test_active && g_motors_enabled)
            DirectionLoop_Update(g_dir_dev);
    }

    /* ─── VOFA发送: 每5ms置位 = 200Hz ─── */
    {
        static uint8_t tick = 0;
        if (++tick >= 5)
        {
            vofa_flag = 1;
            tick = 0;
        }
    }

    if (cnt % 10 == 0) cnt = 0;
}
#else
/* ============ 赛道模式: 三级串级控制器 ============ */
volatile uint8_t imu_vofa_flag = 0;

void CCU60_CH0_IRQHandler(void)
{
    IfxCpu_enableInterrupts();
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t12PeriodMatch);

    static uint8_t cnt = 0;
    cnt++;

    /* ═══════════════════════════════════════════════════════════════
     * 内环: 编码器 + 速度环 @ 1ms (1000Hz) — 增量式PID
     * ═══════════════════════════════════════════════════════════════ */
    Encoder_Update();
    if ((!g_test_active && g_motors_enabled) || Get_Angle_Mode())
        SpeedLoop_Update(Get_Current_SpeedL(), Get_Current_SpeedR());

    /* ═══════════════════════════════════════════════════════════════
     * 中环: 姿态 + 角速度环 @ 2ms (500Hz) — 位置式PID
     * ═══════════════════════════════════════════════════════════════ */
    if (cnt % 2 == 0)
    {
        Attitude_get();
        if ((!g_test_active && g_motors_enabled) || Get_Angle_Mode())
            Gyro_Loop_Update(Get_Current_Gyro());
    }

    /* ═══════════════════════════════════════════════════════════════
     * 外环 @ 5ms (200Hz) — 位置式PID
     * 模式A (Angle_Mode=0): 方向环 → Target_Gyro
     * 模式B (Angle_Mode=1): 角度环 → Target_Gyro
     * ═══════════════════════════════════════════════════════════════ */
    if (cnt % 5 == 0)
    {
        if (!g_test_active && g_motors_enabled)
        {
            if (Get_Angle_Mode())
                AngleLoop_Update(Get_Current_Yaw());
            else
                DirectionLoop_Update(Track_GetDiff());
        }
    }

    if (cnt % 10 == 0) cnt = 0;
}
#endif
// CCU60_CH1中断服务函数
void CCU60_CH1_IRQHandler(void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    // 清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t13PeriodMatch);

    /* 用户代码 */
    LQ_BLDCCtrl();
}

// CCU61_CH0中断服务函数
void CCU61_CH0_IRQHandler(void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    // 清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t12PeriodMatch);

    /* 用户代码 */
    LED_Ctrl(LED1, RVS); // LED点亮
}

// CCU61_CH1中断服务函数
void CCU61_CH1_IRQHandler(void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    // 清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t13PeriodMatch);

    /* 用户代码 */
    //    if(cnt % 1 == 0)
    //    {
    //        Encoder_Update();                                                        //注意改DELTA_T
    //        SpeedLoop_Update(Get_Current_SpeedL(),Get_Current_SpeedR());             //速度环 输入速度 输出pwm
    //    }
    //    if(cnt % 2 == 0)
    //    {
    //        Yaw_Update();
    //        Gyro_Loop_Update(Get_Current_Gyro());                                    //角速度环 输入角速度 输出速度
    //    }
    //    if(cnt % 4 == 0)
    //    {
    //        cnt = 0;
    //        if(Get_Target_yaw()) AngleLoop_Updata(Get_Current_Yaw());               //角度环 输入角度 输出角速度
    //        else DirectionLoop_Update(Get_error());                                 //方向环 输入偏差 输出角速度
    //    }
}
// /***************************串口中断回调函数**********************************/
/* UART中断 */
IFX_INTERRUPT(UART0_RX_IRQHandler, UART0_VECTABNUM, UART0_RX_PRIORITY);
IFX_INTERRUPT(UART1_RX_IRQHandler, UART1_VECTABNUM, UART1_RX_PRIORITY);
IFX_INTERRUPT(UART2_RX_IRQHandler, UART2_VECTABNUM, UART2_RX_PRIORITY);
IFX_INTERRUPT(UART3_RX_IRQHandler, UART3_VECTABNUM, UART3_RX_PRIORITY);
IFX_INTERRUPT(UART0_TX_IRQHandler, UART0_VECTABNUM, UART0_TX_PRIORITY);
IFX_INTERRUPT(UART1_TX_IRQHandler, UART1_VECTABNUM, UART1_TX_PRIORITY);
IFX_INTERRUPT(UART2_TX_IRQHandler, UART2_VECTABNUM, UART2_TX_PRIORITY);
IFX_INTERRUPT(UART3_TX_IRQHandler, UART3_VECTABNUM, UART3_TX_PRIORITY);
IFX_INTERRUPT(UART0_ER_IRQHandler, UART0_VECTABNUM, UART0_ER_PRIORITY);
IFX_INTERRUPT(UART1_ER_IRQHandler, UART1_VECTABNUM, UART1_ER_PRIORITY);
IFX_INTERRUPT(UART2_ER_IRQHandler, UART2_VECTABNUM, UART2_ER_PRIORITY);
IFX_INTERRUPT(UART3_ER_IRQHandler, UART3_VECTABNUM, UART3_ER_PRIORITY);

// 串口0 RX中断函数
void UART0_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[0]);

    /* 用户代码 */
}
// 串口0 TX中断函数
void UART0_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[0]);

    /* 用户代码 */
}
// 串口0 ER中断函数
void UART0_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[0]);

    /* 用户代码 */
}

unsigned char ReadBuff1[512];

// 串口1 RX中断函数
void UART1_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[1]);

    /* 用户代码 */
//    R9DS_Read();
}
// 串口1 TX中断函数
void UART1_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[1]);

    /* 用户代码 */
}
// 串口1 ER中断函数
void UART1_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[1]);

    /* 用户代码 */
}
// 串口2 RX中断函数
void UART2_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[2]);

    /* 用户代码 */
}
// 串口2 TX中断函数
void UART2_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[2]);

    /* 用户代码 */
}
// 串口2 ER中断函数
void UART2_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[2]);

    /* 用户代码 */
}
// 串口3 RX中断函数

#if SPEED_LOOP_DEBUG

/* ──────────── 目标速度发生器(全局) ──────────── */
TargetMode_t g_tgt_mode = TM_STEP;
float g_tgt_ramp_start   = 0, g_tgt_ramp_end = 0;
uint32_t g_tgt_ramp_ms  = 0, g_tgt_ramp_tick = 0;
float g_tgt_sine_freq    = 0, g_tgt_sine_amp = 0, g_tgt_sine_base = 0;

/* ──────────── VOFA调试: 命令接收 ──────────── */
char g_cmd_buf[32];
static uint8_t g_cmd_idx = 0;
volatile uint8_t g_cmd_ready = 0;
volatile uint32_t g_cmd_recv_cnt = 0;

void UART3_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[3]);
    while (UART_GetCount(UART3) > 0)
    {
        char c = UART_GetChar(UART3);
        g_cmd_recv_cnt++;
        if (c == '\r' || c == '\n')
        {
            if (g_cmd_idx == 0) continue;
            g_cmd_buf[g_cmd_idx] = '\0';
            g_cmd_idx = 0;
            g_cmd_ready = 1;
        }
        else if (g_cmd_idx < sizeof(g_cmd_buf) - 1)
        {
            g_cmd_buf[g_cmd_idx++] = c;
        }
    }
}

/* ──────────── 命令解析 (由主循环调用) ──────────── */
void VOFA_Cmd_Process(void)
{
    char cmd_char = g_cmd_buf[0];
    uint16_t saved = IfxCpu_disableInterrupts();

    if (cmd_char == 'Z')
    {
        incpidL.out = 0; incpidR.out = 0;
        incpidL.integrator = 0; incpidR.integrator = 0;
        incpidL.last_error = 0; incpidR.last_error = 0;
        incpidL.last_derivative = 0; incpidR.last_derivative = 0;
        g_tgt_mode   = TM_STEP;
        motorState.Target_SpeedR = 0;
    }
    else if (cmd_char == 'K' && g_cmd_buf[1] == '=')
    {
        incpidR.kp = (float)atof(&g_cmd_buf[2]);
    }
    else if (cmd_char == 'I' && g_cmd_buf[1] == '=')
    {
        incpidR.ki = (float)atof(&g_cmd_buf[2]);
    }
    else if (cmd_char == 'D' && g_cmd_buf[1] == '=')
    {
        incpidR.kd = (float)atof(&g_cmd_buf[2]);
    }
    else if (cmd_char == 'S' && g_cmd_buf[1] == '=')
    {
        float val = (float)atof(&g_cmd_buf[2]);
        g_tgt_mode = TM_STEP;
        g_tgt_sine_base = val;
        motorState.Target_SpeedR = val;
    }
    else if (cmd_char == 'R' && g_cmd_buf[1] == '=')
    {
        char *p = &g_cmd_buf[2];
        g_tgt_ramp_start = (float)atof(p);
        while (*p && *p != ',') p++; if (*p==',') p++;
        g_tgt_ramp_end   = (float)atof(p);
        while (*p && *p != ',') p++; if (*p==',') p++;
        g_tgt_ramp_ms    = (uint32_t)atoi(p);
        g_tgt_ramp_tick  = 0;
        g_tgt_mode       = TM_RAMP;
        g_tgt_sine_base  = g_tgt_ramp_end;
        motorState.Target_SpeedR = g_tgt_ramp_start;
    }
    else if (cmd_char == 'W' && g_cmd_buf[1] == '=')
    {
        char *p = &g_cmd_buf[2];
        g_tgt_sine_freq = (float)atof(p);
        while (*p && *p != ',') p++; if (*p==',') p++;
        g_tgt_sine_amp  = (float)atof(p);
        g_tgt_sine_base = motorState.Target_SpeedR;
        g_tgt_mode      = TM_SINE;
    }
    IfxCpu_enableInterrupts();
}

#elif GYRO_LOOP_DEBUG

/* ──────────── 角速度环调试: 命令接收 ──────────── */
char g_cmd_buf[32];
static uint8_t g_cmd_idx = 0;
volatile uint8_t g_cmd_ready = 0;
volatile uint32_t g_cmd_recv_cnt = 0;
volatile float g_tgt_gyro = 0;

void UART3_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[3]);
    while (UART_GetCount(UART3) > 0)
    {
        char c = UART_GetChar(UART3);
        g_cmd_recv_cnt++;
        if (c == '\r' || c == '\n')
        {
            if (g_cmd_idx == 0) continue;
            g_cmd_buf[g_cmd_idx] = '\0';
            g_cmd_idx = 0;
            g_cmd_ready = 1;
        }
        else if (g_cmd_idx < sizeof(g_cmd_buf) - 1)
        {
            g_cmd_buf[g_cmd_idx++] = c;
        }
    }
}

/* ──────────── 命令解析: 角速度环专用 (统一 °/s) ──────────── */
/* Z     = 重置 (目标角速度清零, 积分清零)
 * G=<f> = 设置目标角速度 (°/s)
 * P=<f> = 设置 KP
 * I=<f> = 设置 KI
 * D=<f> = 设置 KD                           */

void VOFA_Cmd_Process(void)
{
    char cmd_char = g_cmd_buf[0];

    if (cmd_char == 'Z')
    {
        gyropid.integrator = 0;
        gyropid.last_error = 0;
        gyropid.last_derivative = 0;
        g_tgt_gyro = 0;
        motorState.Target_Gyro = 0;
    }
    else if (cmd_char == 'G' && g_cmd_buf[1] == '=')
    {
        float val = (float)atof(&g_cmd_buf[2]);   /* 直接 °/s */
        g_tgt_gyro = val;
        motorState.Target_Gyro = val;
    }
    else if (cmd_char == 'P' && g_cmd_buf[1] == '=')
    {
        gyropid.kp = (float)atof(&g_cmd_buf[2]);
    }
    else if (cmd_char == 'I' && g_cmd_buf[1] == '=')
    {
        gyropid.ki = (float)atof(&g_cmd_buf[2]);
    }
    else if (cmd_char == 'D' && g_cmd_buf[1] == '=')
    {
        gyropid.kd = (float)atof(&g_cmd_buf[2]);
    }
    IfxCpu_enableInterrupts();
}

#elif DIR_LOOP_DEBUG

/* ──────────── 方向环调试: 命令接收 ──────────── */
char g_cmd_buf[32];
static uint8_t g_cmd_idx = 0;
volatile uint8_t g_cmd_ready = 0;
volatile uint32_t g_cmd_recv_cnt = 0;

void UART3_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[3]);
    while (UART_GetCount(UART3) > 0)
    {
        char c = UART_GetChar(UART3);
        g_cmd_recv_cnt++;
        if (c == '\r' || c == '\n')
        {
            if (g_cmd_idx == 0) continue;
            g_cmd_buf[g_cmd_idx] = '\0';
            g_cmd_idx = 0;
            g_cmd_ready = 1;
        }
        else if (g_cmd_idx < sizeof(g_cmd_buf) - 1)
        {
            g_cmd_buf[g_cmd_idx++] = c;
        }
    }
}

/* ──────────── 命令解析: 方向环专用 ──────────── */
/* Z     = 重置 (偏差清零, 积分清零)
 * S=<f> = 设定偏差 (像素)
 * P=<f> = 设置 KP
 * I=<f> = 设置 KI
 * D=<f> = 设置 KD                           */

void VOFA_Cmd_Process(void)
{
    char cmd_char = g_cmd_buf[0];

    if (cmd_char == 'Z')
    {
        locpid.integrator = 0;
        locpid.last_error = 0;
        locpid.last_derivative = 0;
        g_dir_dev = 0;
    }
    else if (cmd_char == 'S' && g_cmd_buf[1] == '=')
    {
        g_dir_dev = (float)atof(&g_cmd_buf[2]);   /* 像素 */
    }
    else if (cmd_char == 'P' && g_cmd_buf[1] == '=')
    {
        locpid.kp = (float)atof(&g_cmd_buf[2]);
    }
    else if (cmd_char == 'I' && g_cmd_buf[1] == '=')
    {
        locpid.ki = (float)atof(&g_cmd_buf[2]);
    }
    else if (cmd_char == 'D' && g_cmd_buf[1] == '=')
    {
        locpid.kd = (float)atof(&g_cmd_buf[2]);
    }
    IfxCpu_enableInterrupts();
}

#else
/* ──────────── 赛道模式: 保留原UART3 RX(预留) ──────────── */
void UART3_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[3]);
}
#endif

void UART3_TX_IRQHandler(void)
{

    IfxAsclin_Asc_isrTransmit(&g_UartConfig[3]);

    /* 用户代码 */
}
// 串口3 ER中断函数
void UART3_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[3]);
    /* 用户代码 */
}

/***************************外部中断中断回调函数**********************************/
/* GPIO外部中断 */
IFX_INTERRUPT(PIN_INT0_IRQHandler, PIN_INT0_VECTABNUM, PIN_INT0_PRIORITY);
IFX_INTERRUPT(PIN_INT1_IRQHandler, PIN_INT1_VECTABNUM, PIN_INT1_PRIORITY);
IFX_INTERRUPT(PIN_INT2_IRQHandler, PIN_INT2_VECTABNUM, PIN_INT2_PRIORITY);
IFX_INTERRUPT(PIN_INT3_IRQHandler, PIN_INT3_VECTABNUM, PIN_INT3_PRIORITY);
IFX_INTERRUPT(PIN_INT4_IRQHandler, PIN_INT4_VECTABNUM, PIN_INT4_PRIORITY);
IFX_INTERRUPT(PIN_INT5_IRQHandler, PIN_INT5_VECTABNUM, PIN_INT5_PRIORITY);
IFX_INTERRUPT(PIN_INT6_IRQHandler, PIN_INT6_VECTABNUM, PIN_INT6_PRIORITY);
IFX_INTERRUPT(PIN_INT7_IRQHandler, PIN_INT7_VECTABNUM, PIN_INT7_PRIORITY);

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：PIN_INT0_IRQHandler中断服务函数
@功能说明：
@参数说明：无
@函数返回：无
@备    注：外部中断0组管脚 使用的中断服务函数
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void PIN_INT0_IRQHandler(void)
{

    /* 用户代码 */
    // LED_Ctrl(LED1, RVS); // 电平翻转,LED闪烁
}

// PIN_INT1_IRQHandler中断服务函数
void PIN_INT1_IRQHandler(void)
{
    /* 用户代码 */
    // LED_Ctrl(LED1, RVS); // 电平翻转,LED闪烁
}

// PIN_INT2_IRQHandler中断服务函数
void PIN_INT2_IRQHandler(void)
{
    // 用户代码
//     LED_Ctrl(LED0, RVS); // 电平翻转,LED闪烁
}

// PIN_INT3_IRQHandler中断服务函数
void PIN_INT3_IRQHandler(void)
{
    // 用户代码
    // LED_Ctrl(LED1, RVS); // 电平翻转,LED闪烁
}
// PIN_INT4_IRQHandler中断服务函数
void PIN_INT4_IRQHandler(void)
{
    // 用户代码
}
// PIN_INT5_IRQHandler中断服务函数
void PIN_INT5_IRQHandler(void)
{
    // 用户代码
}
// PIN_INT6_IRQHandler中断服务函数
void PIN_INT6_IRQHandler(void)
{
    // 用户代码
//    LED_Ctrl(LED1, RVS); // 电平翻转,LED闪烁
}
// PIN_INT7_IRQHandler中断服务函数
void PIN_INT7_IRQHandler(void)
{
    // 已被摄像头占用，不可使用
//    LED_Ctrl(LED1, RVS); // 电平翻转,LED闪烁
    DMA_CameraStart(PIN_INT2_PRIORITY);
}

char Flag_1s = 0;

// STM0_CH1中断服务函数
IFX_INTERRUPT(STM0_CH0_IRQHandler, STM0_VECTABNUM, STM0_CH0_PRIORITY);
IFX_INTERRUPT(STM0_CH1_IRQHandler, STM0_VECTABNUM, STM0_CH1_PRIORITY);
IFX_INTERRUPT(STM1_CH0_IRQHandler, STM1_VECTABNUM, STM1_CH0_PRIORITY);
IFX_INTERRUPT(STM1_CH1_IRQHandler, STM1_VECTABNUM, STM1_CH1_PRIORITY);

// STM0_CH0中断服务函数
void STM0_CH0_IRQHandler(void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();
    // 清除中断标志
    IfxStm_clearCompareFlag(&MODULE_STM0, g_StmCompareConfig[0].comparator);
    // 开启新的中断配置，开始下次中断
    IfxStm_increaseCompare(&MODULE_STM0, g_StmCompareConfig[0].comparator, g_StmCompareConfig[0].ticks);
    /* 用户代码 */
    Flag_1s = 1;
}

void STM0_CH1_IRQHandler(void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();
    // 清除中断标志
    IfxStm_clearCompareFlag(&MODULE_STM0, g_StmCompareConfig[1].comparator);
    // 开启新的中断配置，开始下次中断
    IfxStm_increaseCompare(&MODULE_STM0, g_StmCompareConfig[1].comparator, g_StmCompareConfig[1].ticks);
    /* 用户代码 */
}

// STM1_CH0中断服务函数
void STM1_CH0_IRQHandler(void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();
    // 清除中断标志
    IfxStm_clearCompareFlag(&MODULE_STM1, g_StmCompareConfig[2].comparator);
    // 开启新的中断配置，开始下次中断
    IfxStm_increaseCompare(&MODULE_STM1, g_StmCompareConfig[2].comparator, g_StmCompareConfig[2].ticks);
    /* 用户代码 */
}

// STM1_CH1中断服务函数
void STM1_CH1_IRQHandler(void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();
    // 清除中断标志
    IfxStm_clearCompareFlag(&MODULE_STM1, g_StmCompareConfig[3].comparator);
    // 开启新的中断配置，开始下次中断
    IfxStm_increaseCompare(&MODULE_STM1, g_StmCompareConfig[3].comparator, g_StmCompareConfig[3].ticks);
    /* 用户代码 */
}
