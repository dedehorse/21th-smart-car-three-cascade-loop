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
#include "lq_include.h"


IfxCpu_syncEvent g_cpuSyncEvent = 0;
volatile uint16_t g_debugFps = 0;
volatile uint8_t g_lcdReady = 0;

#if TRACK_OUTPUT_WIRELESS
/* CPU0→CPU3 共享: 无线图像传输 (188x120) */
volatile uint8_t g_tx_img_buf[TR_IMG_H * TR_IMG_W];
volatile uint8_t g_tx_img_ready = 0;
#endif

extern char Flag_1s;
extern uint8_t g_test_active;

int core0_main(void)
{

    //================================ 系统代码 ================================//
    cpu_init();                        // 等待cpu初始化、
    IfxCpu_emitEvent(&g_cpuSyncEvent); // 等待cpu同步
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);
    UART_InitConfig(UART0_RX_P14_1, UART0_TX_P14_0, 115200);  // 下载口的串口,默认重定向PRINTF 在config中选择
    UART_InitConfig(UART3_RX_P00_1, UART3_TX_P00_0, 115200); //  调试 UART 初始化
    Display_Init(2);        // LCD初始化
    Display_CLS(U16_BLACK); // 黑色屏幕
    g_lcdReady = 1;
    Tracking_init();
    MotorInit();
    EncInit();
    PidInit();
#if PATH_PLAN_ENABLE
    PathPlan_Init();
    PathPlan_SetRoute(0);   /* 0=414路线, 1=旧8节点, 2=无锁路线 */
#endif
    SPI_Gryo_Init();
    CameraInt();
    IMU_GetOffset(&icm_offset);       /* 静止校准零偏: 200次采样取平均, 之后 offset_flag=1 */
//  g_test_active = 1;  //角度环测试开启
#if MENU_ENABLE
    GPIO_KEY_Init();                  /* 初始化按键 GPIO (KEY1=P33_9)              */
    Display_Menu();                   /* 阻塞式参数查看菜单, 长按P33_9退出           */
    EncInit();                        /* 恢复 P33_5 编码器引脚配置                  */
#endif
#if BLS_RAMP_ENABLE
    BLS3_RampStartup();             /* BLS3风扇缓启动: 510→650, 完成后使能运动电机 */
#endif
    CCU6_InitConfig(CCU60, CCU6_Channel0, 1000); // CCU6初始化
    STM_InitConfig(STM0, STM_Channel_0, 1000000);//1s中断



   // ================================ 外设以及模块驱动测试函数 ================================//
//     Test_GPIO_OUT();    // PASS,测试GPIO，P10.6和P10.5闪灯
//     Test_GPIO_KEY();    // PASS,测试外部按键输入，P22.0--2   按下按键   LED亮
//     Test_GPIO_Extern(); // PASS,测试外部第1组中断P15.4，P10.6和P10.5闪灯
//     Test_Display();     // PASS,测试显示屏，需要去Main/config.h中选择屏幕类型和接口类型
//     Test_CCU6_Timer();  // PASS,测试CCU6定时器
//     Test_ADC();         // PASS,测试ADC, 并把值显示在屏幕上
//     Test_IIC_Gyro();    // PASS,测试陀螺仪模块,包括MPU6050 9250或者ICM20602 20689 IIC接线   P13_1接SCL  P13_2接SDA
//       Test_SPI_Gyro();    // PASS,测试陀螺仪模块,SPI接线，可直接插到母板陀螺仪接口
//     Test_LQ6050_DMP();  // PASS,测试6050DMP,IIC接线   P13_1接SCL  P13_2接SDA
//     Test_Quat();        // PASS,测试四元数运算，陀螺仪姿态解算,采用spi接线，需要把解算和读取函数放在一个1ms的定时器中
//       Test_Encoder();     // PASS,测试编码器，兼容正交解码以及带方向龙邱编码器
//     Test_EEPROM();      // PASS,测试内部EEPROM擦写功能  屏幕提示是否写入成功
//     Test_SoftFft();     // PASS,测试ILLD库的软件FFT函数
//     Test_Motor();       // PASS,测试4路电机PWM控制
//     Test_GyroLoop();    // 测试角速度环,用手旋转车身观察电机是否有阻尼力
//     Test_AngleLoop();    //测试角度环,每3秒自动+90度旋转
//     Test_MotorBLDC();   // PASS,测试2134无刷驱动，接线说明在例程测试函数下
//     Test_Servo();       // PASS,测试两路舵机
//      Test_BlsMotor();    // PASS,测试无感无刷电机驱动,新增P21_2 BLS3
//     Test_Bluetooth();   // PASS,测试UART0(P14.0RX/P14.1TX), lq_WLS_config()初始化串口配置无线通信模块，参数修改详见 函数内宏
//     Test_SBUS();        // PASS,测试遥控器接收数据
//     Test_STM_Timer();   // PASS,利用STM模块去完成代码运行时间记录
//     Test_UTM();         // PASS,测试UTM坐标转换函数
//     Test_BD1202();      // PASS,测试GPS模块。 注意，测试的时候一定要把对应串口中断里面加入读取程序，要不然不能正常收到GPS信号, 一定要看一下函数说明
//     Test_CAMERA();      // PASS,测试龙邱神眼摄像头并在屏幕上显示  LQ_CAMERA.h 中选择屏幕
//       Test_CAMERA_TR();   // 改为赛道模式: CPU2 负责无线传输,调用 lq_WLS_config()初始化串口配置无线通信模块，参数修改详见 函数内宏
//      Test_Tracking()     // PASS,测试龙16路模拟量灰度循迹模块，默认串口输出并再IPS屏幕上显示
//     Test_ADC_Key();     // PASS,测试ADC按键和旋钮,在LCD上显示按键状态和ADC值
//   先解决图像（上位机颜色 初始化方差阈值 ） 在解决节点（左t 右t 十字 一套逻辑）（左直角 右直角 t 一套逻辑） 节点识别（根据前方路口特征 节点数组标志位+1） 接着调整角速度环
//   cpu0 所有业务逻辑  cpu3 wifi 屏幕初始化
//  TR_driver_init();  // QSPI4 已移至 CPU3 初始化
    uint16_t fps = 0;

     while (1)
     {
#if SPEED_LOOP_DEBUG
        if (g_cmd_ready)
        {
            g_cmd_ready = 0;
            VOFA_Cmd_Process();
        }

        /* ═══════════════════════════════════════════════════════════
         * VOFA FireWater 速度环调试: 100Hz (ISR每10ms置位vofa_flag)
         * 8通道: TargetSpeed/CurSpeed/KP/KI/KP_Out/KI_Out/PWM/RxCnt
         * RxCnt=收到字节数, 用于验证UART3 RX是否工作 ═════ */
        if (vofa_flag)
        {
            vofa_flag = 0;

            /* ─── 快照: 关中断读速度环数据 ─── */
            float buf[8];
            uint16_t saved = IfxCpu_disableInterrupts();
            {
                buf[0] = motorState.Target_SpeedR;
                buf[1] = Get_Current_SpeedR();
                buf[2] = incpidR.kp;
                buf[3] = incpidR.ki;
                buf[4] = incpidR.out_p;
                buf[5] = incpidR.out_i;
                buf[6] = (float)motorState.pwmR;
                buf[7] = (float)g_cmd_recv_cnt;
            }
            IfxCpu_enableInterrupts();

            /* ─── JustFloat帧: 8×float + 帧尾 = 36字节 ─── */
            {
                unsigned char frame[36], *p = frame;
                for (int i = 0; i < 8; i++) {
                    unsigned char *fp = (unsigned char *)&buf[i];
                    *p++ = fp[0]; *p++ = fp[1]; *p++ = fp[2]; *p++ = fp[3];
                }
                *p++ = 0x00; *p++ = 0x00; *p++ = 0x80; *p++ = 0x7F;  /* 帧尾 */
                UART_PutBuff(UART3, frame, 36);
            }

            /* ─── 目标速度发生器推进 (100Hz, 每10ms) ─── */
            {
                uint16_t s = IfxCpu_disableInterrupts();
                if (g_tgt_mode == TM_RAMP)
                {
                    g_tgt_ramp_tick += 10;
                    if (g_tgt_ramp_tick >= g_tgt_ramp_ms)
                    {
                        g_tgt_ramp_tick = g_tgt_ramp_ms;
                        g_tgt_mode = TM_STEP;
                    }
                    float t = (float)g_tgt_ramp_tick / (float)g_tgt_ramp_ms;
                    motorState.Target_SpeedR = g_tgt_ramp_start
                        + (g_tgt_ramp_end - g_tgt_ramp_start) * t;
                }
                else if (g_tgt_mode == TM_SINE)
                {
                    static uint32_t sine_tick = 0;
                    sine_tick += 10;
                    float t_sec = sine_tick * 0.001f;
                    motorState.Target_SpeedR = g_tgt_sine_base
                        + g_tgt_sine_amp * sinf(2.0f * (float)M_PI * g_tgt_sine_freq * t_sec);
                }
                IfxCpu_enableInterrupts();
            }
        }
#elif GYRO_LOOP_DEBUG
        /* ═══════════════════════════════════════════════════════════
         * VOFA 角速度环调试模式
         * ═══════════════════════════════════════════════════════════ */
        if (g_cmd_ready)
        {
            g_cmd_ready = 0;
            VOFA_Cmd_Process();
        }

        if (vofa_flag)
        {
            vofa_flag = 0;

            /* ─── 快照: 14通道, 统一 °/s 量纲 ─── */
            enum { V_CH = 9 };
            float buf[V_CH];
            uint16_t saved = IfxCpu_disableInterrupts();
            {
                buf[0]  = motorState.Target_Gyro;            /* CH1:  目标角速度 (°/s) */
                buf[1]  = Get_Current_Gyro();                /* CH3:  当前角速度 (°/s) */
                buf[2]  = gyropid.kp;                         /* CH4:  KP              */
                buf[3]  = gyropid.ki;                         /* CH5:  KI              */
                buf[4]  = gyropid.kd;                         /* CH6:  KD              */
                buf[5]  = gyropid.out_p;                      /* CH7:  P项输出         */
                buf[6]  = gyropid.out_i;                      /* CH8:  I项输出         */
                buf[7]  = gyropid.out_d;                      /* CH9:  D项输出         */
                buf[8] = motorState.Target_SpeedR;           /* CH12: 速度环输入(cm/s) */
            }
            IfxCpu_enableInterrupts();

            /* ─── JustFloat帧: 14×float + 帧尾 = 60字节 ─── */
            {
                unsigned char frame[60], *p = frame;
                for (int i = 0; i < V_CH; i++) {
                    unsigned char *fp = (unsigned char *)&buf[i];
                    *p++ = fp[0]; *p++ = fp[1]; *p++ = fp[2]; *p++ = fp[3];
                }
                *p++ = 0x00; *p++ = 0x00; *p++ = 0x80; *p++ = 0x7F;
                UART_PutBuff(UART3, frame, 60);
            }
        }
#elif DIR_LOOP_DEBUG
        /* ═══════════════════════════════════════════════════════════
         * VOFA 方向环调试: 完整三级串级, 9通道 JustFloat @200Hz
         * ═══════════════════════════════════════════════════════════ */
        if (g_cmd_ready)
        {
            g_cmd_ready = 0;
            VOFA_Cmd_Process();
        }

        if (vofa_flag)
        {
            vofa_flag = 0;

            enum { V_CH = 9 };
            float buf[V_CH];
            {
                buf[0] = g_dir_dev;                   /* CH0: 设定偏差(像素) */
                buf[1] = locpid.kp;                   /* CH2: KP           */
                buf[2] = locpid.ki;                   /* CH3: KI           */
                buf[3] = locpid.kd;                   /* CH4: KD           */
                buf[4] = locpid.out_p;                /* CH5: P项输出      */
                buf[5] = locpid.out_i;                /* CH6: I项输出      */
                buf[6] = locpid.out_d;                /* CH7: D项输出      */
                buf[7] = motorState.Target_Gyro;      /* CH8: 角速度环(°/s) */
            }

            unsigned char frame[40], *p = frame;
            for (int i = 0; i < V_CH; i++) {
                unsigned char *fp = (unsigned char *)&buf[i];
                *p++ = fp[0]; *p++ = fp[1]; *p++ = fp[2]; *p++ = fp[3];
            }
            *p++ = 0x00; *p++ = 0x00; *p++ = 0x80; *p++ = 0x7F;
            UART_PutBuff(UART3, frame, 40);
        }
#else
        /* ═══════════════════════════════════════════════════════════
         * 赛道模式: 摄像头 → 图像处理 → 赛道识别 → 控制
         * ═══════════════════════════════════════════════════════════ */
         if (Camera_Flag == 2)
         {
             Camera_Flag = 0;
             Get_Use_Image();
             fps++;
             Get_Bin_Image();

#if TRACK_OUTPUT_WIRELESS
            /* ── 准备无线图像: 拷贝 + 叠加边界 → 共享缓冲区 → 通知 CPU3 ── */
            {
                uint8_t *p = (uint8_t *)g_tx_img_buf;
                for (int j = 0; j < TR_IMG_H; j++)
                    for (int i = 0; i < TR_IMG_W; i++)
                        *p++ = (j < LCDH && i < LCDW) ? Pixle[j][i] : 0;

                for (unsigned char j = 0; j < LCDH; j++) {
                    if (left_border[j]  != INVALID_BORDER)
                        DrawImgPointBuf(left_border[j],  j, GRAY_RED,    (uint8_t *)g_tx_img_buf, TR_IMG_W);
                    if (right_border[j] != INVALID_BORDER)
                        DrawImgPointBuf(right_border[j], j, GRAY_GREEN,  (uint8_t *)g_tx_img_buf, TR_IMG_W);
                    if (center_line[j]  != INVALID_BORDER)
                        DrawImgPointBuf(center_line[j],  j, GRAY_YELLOW, (uint8_t *)g_tx_img_buf, TR_IMG_W);
                }

                __dsync();
                g_tx_img_ready = 1;  /* 通知 CPU3: SPI 发送 */
            }
#endif

          /* ── 赛道分析 ── */
             Image_Filter(30);
             Track_Process();
         }

         if (Flag_1s && !g_test_active)
         {
             Flag_1s = 0;
             g_debugFps = fps;
             Track_SetFPS(fps);
             fps = 0;
         }

#endif
     }
}

