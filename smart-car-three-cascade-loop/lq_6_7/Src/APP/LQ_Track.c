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

#include "LQ_Track.h"
#include "lq_include.h"
#include "LQ_CAMERA.h"
#include "LQ_Element.h"

/********************************************************************************************************************

 * @模块名称：8/16 路模拟量灰度循迹模块
 * @模块概述：
    本模块主要用于实现 8/16 路模拟量灰度循迹功能，通过对 16 个不同通道的模拟量进行采集和处理，
        能够感知外部环境的灰度变化，常用于智能小车等设备的循迹应用场景。
 * @使用方法：
    1. 初始化阶段：
       在程序开始运行的初始化阶段，调用 “ Tracking_Adc_Init(); ” 函数。
       // （该函数的作用是对模块所需的硬件资源（ADC）进行初始化配置，为后续的数据采集操作做好准备。）
    2. 数据采集阶段：
       在主函数的 while(1) 循环中调用 “ Tracking_Value_Acquire(); ” 函数。
       // （此函数会触发 8/16 路模拟量的采集过程，从各个通道获取当前的模拟量数据，并进行必要的处理和转换。）
    3. 数据访问阶段：
       采集到的数据会存储在数组 “ LQ_Tracking_Value[] ” 中。可以直接访问 “ LQ_Tracking_Value[0] - LQ_Tracking_Value[15] ” 中的数据，以获取各个通道的模拟量值。例如，访问 “ LQ_Tracking_Value[0] ” 获取第一个通道的模拟量值，
       // （这些数组元素分别对应循迹模块从左至右各个通道的 ADC 数据。）
 * @注意事项：
    使用方法中未涉及的函数或变量主要供内部调用。非必要情况下，用户无需深入了解其实现细节，只需按照上述使用方法进行操作即可。
        若确实需要对模块进行定制或优化，建议在充分了解模块整体架构和硬件原理的基础上进行。
 ******************************************************************************************************************/

//-----------------16 路模拟量灰度循迹模块最终数据-----------------
unsigned int LQ_Tracking_Value[TRACK_CHANNEL_NUM] = {0};

void Tracking_init()
{
    PIN_InitConfig(TRACK_S0_PIN, PIN_MODE_OUTPUT, 1); // 初始化 TRACK_S0_PIN 引脚为输出模式，用于选择循迹模块的通道
    PIN_InitConfig(TRACK_S1_PIN, PIN_MODE_OUTPUT, 1);
    PIN_InitConfig(TRACK_S2_PIN, PIN_MODE_OUTPUT, 1);
    PIN_InitConfig(TRACK_S3_PIN, PIN_MODE_OUTPUT, 1);
    ADC_InitConfig(ADC8, 80000);
}

/********************************************************************************************************************
 * 【函数名称】 Tracking_Value_once
 * 【功能概述】 该函数用于获取循迹模块单通道的 ADC 数据。在获取过程中，会对数据进行一系列处理，
 *             包括防串扰、简单的均值滤波以及缩放操作，最终输出处理后的 ADC 数据。
 * 【输入参数】 unsigned char ch，循迹模块的 ADC 通道编号，取值范围应为 1 - 16
 *            不同的电平组合对应不同的循迹模块通道选择。具体对应关系如下：
 *            - (S3, S2, S1, S0) = (0, 0, 0, 0) 对应通道 1  --> S3|S2|S1|S0 = 0000B = 0
 *            - (S3, S2, S1, S0) = (0, 0, 0, 1) 对应通道 2  --> S3|S2|S1|S0 = 0001B = 1
 *            ***...（其他通道以此类推）
 *            - (S3, S2, S1, S0) = (1, 0, 0, 0) 对应通道 9
 *            ***...（其他通道以此类推）
 *            - (S3, S2, S1, S0) = (1, 1, 1, 1) 对应通道 16
 *            因此， S3、S2、S1、S0 电平状态组合成的二进制数（0-15）选择要采集的通道（1-16）。
 * 【返 回 值】 unsigned int，返回该 ADC 通道经过处理后的数据，数据范围为 0 - 100。
 * 【使用示例】 unsigned int value = Tracking_Value_once(1);  // 调用函数获取循迹模块第 1 通道的 ADC 数据
 * 【注意事项】 函数自带防串扰、简单的均值滤波以及缩放功能。其中，缩放操作会将原始的 0 - 4095 数据范围转换为 0 - 100。
 *             为了避免初始不稳定采样值（串扰）的影响，会抛弃前几次采样数据。
 ******************************************************************************************************************/
unsigned int Tracking_Value_once(unsigned char ch)
{
    unsigned int data = 0;             // 定义变量用于存储每次采样得到的原始 ADC 数据
    unsigned int sum = 0;              // 定义变量用于累加有效的采样数据，以便后续进行均值计算
    unsigned int value_out = 0;        // 定义变量用于存储最终经过处理后要输出的 ADC 数据
    unsigned char num_samples = 5;     // 定义采样次数，即对该通道进行 ADC 采样的总次数
    unsigned char discard_samples = 3; // 定义要抛弃的前几次采样次数，用于避免通道串扰的影响

    // 循环进行多次采样，以实现均值滤波
    for (unsigned char i = 0; i < num_samples; i++)
    {
        // 通过位运算直接计算引脚电平状态（通道值0-15对应4位二进制）
        PIN_Write(TRACK_S3_PIN, (ch >> 3) & 0x01); // 取最高位(S3)
        PIN_Write(TRACK_S2_PIN, (ch >> 2) & 0x01); // 次高位(S2)
        PIN_Write(TRACK_S1_PIN, (ch >> 1) & 0x01); // 次低位(S1)
        PIN_Write(TRACK_S0_PIN, ch & 0x01);        // 最低位(S0)

        // 调用 Tracking_Adc_once 函数获取当前通道的原始 ADC 数据，并进行缩放处理
        // 原始数据范围是 0 - 4095，缩放后范围变为 0 - 100
        data = ADC_Read(ADC8) * 0.02442;
        // 对 data 限幅，确保变量的值在 0 - 100 这个范围内
        data = data >= 100 ? 100 : (data <= 0 ? 0 : data);

        // 抛弃前 discard_samples 次采样数据，避免初始不稳定采样值的影响
        // 只累加有效的采样数据到 sum 中
        if (i >= discard_samples)
            sum += data;
    }

    // 计算有效采样数据的平均值，得到最终要输出的 ADC 数据
    value_out = sum / (num_samples - discard_samples);

    // 返回最终经过处理后的 ADC 数据
    return (unsigned int)value_out;
}

/********************************************************************************************************************
 * 【函数名称】 Tracking_Value_Acquire
 * 【功能概述】 该函数用于获取 16 路模拟量灰度循迹模块各通道的 ADC 数据。它会调用相应函数获取每个通道的原始 ADC 数据，
 *             并将其存储在特定数组中，同时数据会进行缩放处理。
 * 【输入参数】 无
 * 【返 回 值】 无
 * 【使用示例】 Tracking_Value_Acquire();  // 调用该函数获取 16 路模拟量灰度循迹模块各通道的 ADC 数据
 * 【注意事项】 函数获取的原始 ADC 数据范围为 0 - 4095，经过缩放后，最终存储在数组中的数据取值范围为 0 - 100。
 *             可以直接通过访问 LQ_Tracking_Value[i]（i 取值范围为 0 - 15）来获取各通道缩放后的数据。
 ******************************************************************************************************************/
void Tracking_Value_Acquire()
{
    // 定义循环变量，用于遍历 16 个通道
    unsigned char i = 0;

    // 循环 16 次，依次获取 16 路模拟量灰度循迹模块各通道的 ADC 数据
    for (i = 0; i < 16; i++)
    {
        // 调用 Tracking_Value_once 函数获取第 i+1 个通道的 ADC 数据
        LQ_Tracking_Value[i] = Tracking_Value_once(i);
    }
}

/********************************************************************************************************************
 * 【函数名称】 Test_Tracking
 * 【功能概述】 本函数用于对 16 路模拟量灰度循迹模块进行测试。通过持续获取该模块各通道的 ADC 数据，
 *             并将这些数据显示在 lcd屏幕上，从而方便开发人员进行调试和观察。
 * 【输入参数】 无
 * 【返 回 值】 无
 * 【使用示例】 Test_Tracking();
 * 【注意事项】 无
 ******************************************************************************************************************/
void Test_Tracking()
{
    char txt[32];
    GPIO_LED_Init();
    Display_Init(0);        // LCD初始化
    Display_CLS(U16_BLACK); // 黑色屏幕
    Tracking_init();
    while (1)
    {
        // 获取 16 路模拟量灰度循迹模块各通道 ADC 数据CH[0,15]
        Tracking_Value_Acquire(); // 调用函数获取所有通道的 ADC 数据，并存储在 LQ_Tracking_Value 数组中
        for (size_t i = 0; i < TRACK_CHANNEL_NUM; i++)
        {
            if( i%2 == 0)  // 偶数通道显示在屏幕上半部分，奇数通道显示在屏幕下半部分
            {
                sprintf((char *)txt, "   A%02d:%03d  ", i, LQ_Tracking_Value[i]);
                Display_showString(0, i+1, txt, U16_WHITE, U16_BLACK, 12);
            }
            else
            {
                sprintf((char *)txt, "   A%02d:%03d  ", i, LQ_Tracking_Value[i]);
                Display_showString(64, i, txt, U16_WHITE, U16_BLACK, 12);
            }
        }
    }
}

float track_error = 0;
float last_error = 0;
int   binary[16 ] = {0};
/********************************************************************************************************************
 * 【函数名称】 Tracking
 * 【功能概述】 该函数用于实现16路模拟量灰度循迹，通过二值化处理和位置加权计算偏差值
 * 【输入参数】 无
 * 【返 回 值】 float，返回循迹偏差值，负值表示偏向左侧，正值表示偏向右侧
 * 【使用示例】 float error = Tracking();
 * 【注意事项】 函数会调用Tracking_Value_Acquire()获取最新的循迹数据
 ******************************************************************************************************************/
void Dig_Binary(void)
{
    // 获取16路循迹数据
    Tracking_Value_Acquire();
    
    // 固定阈值二值化
    static const int threshold[16] = {30,15,30,30,30,16,30,19,30,30,30,30,30,28,30,15};
    for (unsigned char i = 0; i < 16; i++) {
        binary[i] = (LQ_Tracking_Value[i] > threshold[i]) ? 0 : 1;
    }
}

/********************************************************************************************************************
 * 【函数名称】 Tracking
 * 【功能概述】 该函数用于实现16路模拟量灰度循迹，通过二值化处理和位置加权计算偏差值
 * 【输入参数】 无
 * 【返 回 值】 float，返回循迹偏差值，负值表示偏向左侧，正值表示偏向右侧
 * 【使用示例】 float error = Tracking();
 * 【注意事项】 函数会调用Tracking_Value_Acquire()获取最新的循迹数据
 ******************************************************************************************************************/
void Dig_Tracking()
{
    static const int weights[16] = {-15, -9, -6, -4, -3, -2, -1, 0, 0, 1, 2, 3, 4, 6, 9, 15};
    int weighted_sum = 0, detect_cnt = 0;
    for (unsigned char i = 0; i < 16; i++) {
        detect_cnt += binary[i];
        weighted_sum += binary[i] * weights[i];
    }
    track_error = detect_cnt ? (float)weighted_sum : (last_error < 0 )? -TURN_ERROR : TURN_ERROR;
    last_error = track_error;
}

void Show_Bin()
{
    char txt[32];
    for (size_t i = 0; i < TRACK_CHANNEL_NUM; i++)
    {
        if( i%2 == 0)  // 偶数通道显示在屏幕上半部分，奇数通道显示在屏幕下半部分
        {
            sprintf((char *)txt, "   A%02d:%03d  ", i, binary[i]);
            Display_showString(0, i+1, txt, U16_WHITE, U16_BLACK, 12);
        }
        else
        {
            sprintf((char *)txt, "   A%02d:%03d  ", i, binary[i]);
            Display_showString(64, i, txt, U16_WHITE, U16_BLACK, 12);
        }
    }

    sprintf(txt, "track_error: %.2f  ", track_error);
    Display_showString(0, 17, txt, U16_WHITE, U16_BLACK, 16);
}

float Get_error()
{
    return track_error;
}

/********************************************************************************************************************
 * 【变量名称】 left_limit、right_limit
 * 【功能概述】 定义扫描边界数组，用于限定每一行图像的扫描范围
 * 【变量说明】 - left_limit[LCDH]：每一行的左边界位置
 *            - right_limit[LCDH]：每一行的右边界位置
 *            这些边界值从近处到远处逐渐变化，形成梯形扫描区域，适应赛道透视效果
 ******************************************************************************************************************/
unsigned char left_limit[LCDH];
unsigned char right_limit[LCDH];

/********************************************************************************************************************
 * 【函数名称】 ScanLimit_Init
 * 【功能概述】 初始化扫描边界，创建梯形扫描区域
 * 【输入参数】 无
 * 【返 回 值】 无
 * 【使用示例】 ScanLimit_Init();
 * 【注意事项】 在使用 detect_border() 之前必须调用此函数初始化扫描边界
 *             近处（屏幕下方）扫描范围较宽，远处（屏幕上方）扫描范围较窄
 ******************************************************************************************************************/
void ScanLimit_Init(void)
{
    for (int j = 0; j < LCDH; j++) {
        // 计算每一行的左边界：从远处的 SCAN_FAR_MARGIN 逐渐过渡到近处的 SCAN_NEAR_MARGIN
        left_limit[j] = SCAN_FAR_MARGIN - (SCAN_FAR_MARGIN - SCAN_NEAR_MARGIN) * j / (LCDH - 1);
        // 计算每一行的右边界：从远处的 LCDW-1-SCAN_FAR_MARGIN 逐渐过渡到近处的 LCDW-1-SCAN_NEAR_MARGIN
        right_limit[j] = LCDW - 1 - SCAN_FAR_MARGIN + (SCAN_FAR_MARGIN - SCAN_NEAR_MARGIN) * j / (LCDH - 1);
        // 边界值安全检查，确保不超出图像范围
        if (left_limit[j] >= LCDW)  left_limit[j] = 0;
        if (right_limit[j] >= LCDW) right_limit[j] = LCDW - 1;
    }
}

const unsigned char scan_x1[LCDH] = {
     8,  9,  9,  9, 10, 10, 10, 10, 10, 10,
    10, 11, 11, 12, 12, 12, 12, 12, 12, 12,
    13, 14, 14, 14, 14, 14, 15, 15, 15, 16,
    16, 17, 17, 17, 17, 17, 17, 18, 19, 19,
    19, 19, 19, 20, 20, 20, 21, 21, 21, 22,
    22, 22, 22, 22, 23, 24, 24, 24, 24, 24
};

const unsigned char scan_y1[LCDH] = {
     0,  1,  2,  3,  3,  3,  4,  4,  4,  4,
     4,  4,  4,  5,  5,  5,  5,  6,  6,  6,
     6,  6,  7,  7,  7,  8,  8,  8,  8,  9,
     9,  9,  9,  9, 10, 10, 10, 11, 11, 11,
    12, 12, 12, 13, 13, 13, 14, 14, 14, 15,
    15, 16, 16, 16, 17, 17, 17, 17, 18, 18
};

const unsigned char scan_y2[LCDH] = {
     2,  2,  3,  3,  4,  4,  4,  4,  4,  4,
     4,  5,  6,  6,  6,  7,  7,  7,  8,  8,
     8,  9,  9,  9, 10, 10, 11, 11, 12, 12,
    13, 13, 14, 14, 15, 16, 16, 17, 17, 17,
    18, 18, 17, 16, 15, 14, 13, 12, 11, 10,
     9,  8,  7,  6,  5,  4,  3,  2,  1,  0
};

/********************************************************************************************************************
 * 【变量名称】 left_border、right_border、top_point、bottom_point
 * 【功能概述】 存储检测到的赛道边界信息
 * 【变量说明】 - left_border[LCDH]：每一行的左边界坐标
 *            - right_border[LCDH]：每一行的右边界坐标
 *            - top_point：检测到有效边界的最顶部行号
 *            - bottom_point：检测到有效边界的最底部行号
 ******************************************************************************************************************/
unsigned char left_border[LCDH];
unsigned char right_border[LCDH];
unsigned char top_point  = INVALID_BORDER;
unsigned char bottom_point = INVALID_BORDER;
unsigned char left_point  = INVALID_BORDER;  /* 左顶点: 所有行左边界的最小列号 (越小越靠左) */  //新逻辑 扫屏幕最左端五行（从最左下角往上扫 第一次）（没扫到 从上往下 左往右再扫5或10行）
unsigned char right_point = 0;               /* 右顶点: 所有行右边界的最大列号 (越大越靠右) */

/********************************************************************************************************************
 * 【变量名称】 last_left、last_right
 * 【功能概述】 记录上一次检测到的边界位置，用于优化当前行的边界搜索
 * 【变量说明】 - last_left：上一次检测到的左边界位置
 *            - last_right：上一次检测到的右边界位置
 *            这些变量用于加速边界搜索，先在上一次位置附近搜索，失败后再全范围搜索
 ******************************************************************************************************************/
static unsigned char last_left;
static unsigned char last_right;

/********************************************************************************************************************
 * 【函数名称】 find_left_edge
 * 【功能概述】 在指定行和范围内查找左边界
 * 【输入参数】 row：行号
 *            from：起始列号（包含）
 *            to：结束列号（不包含）
 * 【返 回 值】 找到的左边界列号，未找到则返回 INVALID_BORDER
 * 【使用示例】 unsigned char left = find_left_edge(50, 10, 100);
 * 【注意事项】 边界检测条件：当前点和下一点非零，且前一点为零（或处于起始位置）
 *             该算法用于检测从黑色背景到白色赛道的左边缘跳变
 ******************************************************************************************************************/
static unsigned char find_left_edge(unsigned char row, unsigned char from, unsigned char to)
{
    for (unsigned char i = from; i < to; i++) {
        // 边界判定条件：
        // 1. 当前位置是起始点，或者前一个像素是黑色（背景）
        // 2. 当前像素是白色（赛道）
        // 3. 下一个像素也是白色（确保不是噪点）
        if ((i == from || Pixle[row][i - 1] == 0) &&
            Pixle[row][i] != 0 && Pixle[row][i + 1] != 0)
            return i;
    }
    return INVALID_BORDER;
}

/********************************************************************************************************************
 * 【函数名称】 find_right_edge
 * 【功能概述】 在指定行和范围内查找右边界（从右向左搜索）
 * 【输入参数】 row：行号
 *            from：起始列号（包含）
 *            to：结束列号（不包含）
 * 【返 回 值】 找到的右边界列号，未找到则返回 INVALID_BORDER
 * 【使用示例】 unsigned char right = find_right_edge(50, 100, 10);
 * 【注意事项】 边界检测条件：当前点和前一点非零，且后一点为零（或处于起始位置）
 *             该算法用于检测从白色赛道到黑色背景的右边缘跳变
 ******************************************************************************************************************/
static unsigned char find_right_edge(unsigned char row, unsigned char from, unsigned char to)
{
    for (unsigned char i = from; i > to; i--) {
        // 边界判定条件：
        // 1. 当前位置是起始点，或者后一个像素是黑色（背景）
        // 2. 当前像素是白色（赛道）
        // 3. 前一个像素也是白色（确保不是噪点）
        if ((i == from || Pixle[row][i + 1] == 0) &&
            Pixle[row][i] != 0 && Pixle[row][i - 1] != 0)
            return i;
    }
    return INVALID_BORDER;
}

/********************************************************************************************************************
 * 【函数名称】 scan_row
 * 【功能概述】 扫描指定行，检测左右边界
 * 【输入参数】 j：要扫描的行号
 * 【返 回 值】 1表示成功检测到左右边界，0表示至少有一个边界未检测到
 * 【使用示例】 unsigned char result = scan_row(50);
 * 【注意事项】 采用两级搜索策略：先在上一次边界附近搜索，失败后再在全范围内搜索
 *             检测结果存储在 left_border[j] 和 right_border[j] 中
 ******************************************************************************************************************/
static unsigned char scan_row(unsigned char j)
{
    unsigned char l = left_limit[j];
    unsigned char r = right_limit[j];
    unsigned char sx = scan_x1[j];
    unsigned char nl, nr, found;

    nl = (last_left  > l + SCAN_DELTA) ? (last_left  - SCAN_DELTA) : l;
    nr = (last_right + SCAN_DELTA < r) ? (last_right + SCAN_DELTA) : r;

    found = find_left_edge(j, nl, nr);
    if (found == INVALID_BORDER) {
        nl = (last_left  > l + sx) ? (last_left  - sx) : l;
        nr = (last_right + sx < r) ? (last_right + sx) : r;
        found = find_left_edge(j, nl, nr);
    }
    if (found == INVALID_BORDER)
        found = find_left_edge(j, l, r);
    if (found != INVALID_BORDER) {
        left_border[j] = found;
        last_left = found;
    }

    found = find_right_edge(j, nr, nl);
    if (found == INVALID_BORDER) {
        nr = (last_right + sx < r) ? (last_right + sx) : r;
        nl = (last_left  > l + sx) ? (last_left  - sx) : l;
        found = find_right_edge(j, nr, nl);
    }
    if (found == INVALID_BORDER)
        found = find_right_edge(j, r, l);
    if (found != INVALID_BORDER) {
        right_border[j] = found;
        last_right = found;
    }

    return (left_border[j] != INVALID_BORDER && right_border[j] != INVALID_BORDER);
}

/********************************************************************************************************************
 * 【函数名称】 detect_border
 * 【功能概述】 检测整幅图像中的赛道边界
 * 【输入参数】 无
 * 【返 回 值】 无
 * 【使用示例】 detect_border();
 * 【注意事项】 扫描顺序从下往上（从近处到远处），这样可以利用下一行的边界位置来优化上一行的搜索
 *             检测结果存储在 left_border、right_border、top_point、bottom_point 中
 *             使用前需确保已调用 ScanLimit_Init() 初始化扫描边界 //可以复用改为从上往下扫去识别左t和右t
 ******************************************************************************************************************/
void detect_border(void)
{
    unsigned char j, y1;

    memset(left_border,  INVALID_BORDER, LCDH);
    memset(right_border, INVALID_BORDER, LCDH);
    top_point    = INVALID_BORDER;
    bottom_point = INVALID_BORDER;
    left_point   = INVALID_BORDER;
    right_point  = 0;
    last_left  = LCDW / 4;
    last_right = LCDW * 3 / 4;

    for (j = LCDH - 1; j < LCDH; j--) {
        if (scan_row(j)) {
            if (bottom_point == INVALID_BORDER) bottom_point = j;
            top_point = j;
        } else {
            y1 = scan_y1[j];
            for (unsigned char k = 1; k <= y1 && k <= j; k++) {
                if (scan_row(j - k)) {
                    if (bottom_point == INVALID_BORDER) bottom_point = j - k;
                    top_point = j - k;
                    j -= k;
                    break;
                }
                if (k == y1 || k == j) {
                    top_point = j;
                    return;
                }
            }
            if (y1 == 0) { top_point = j; return; }
        }
    }

    /* 从上往下扫描顶点: 在T字路口中,从上往下扫能更准确地捕捉
       横向分支的左右边界最宽处,避免从下往上扫时因搜索窗口
       跳变导致的早期返回漏扫问题 */
    if (top_point != INVALID_BORDER && top_point < LCDH) {
        for (j = top_point; j < LCDH; j++) {
            if (left_border[j] != INVALID_BORDER) {
                if (left_point == INVALID_BORDER || left_border[j] < left_point)
                    left_point = left_border[j];
            }
            if (right_border[j] != INVALID_BORDER) {
                if (right_border[j] > right_point)
                    right_point = right_border[j];
            }
        }
    }
}

unsigned char center_line[LCDH];

void calculate_center_line(void)
{
    memset(center_line, INVALID_BORDER, LCDH);

    for (int j = 0; j < LCDH; j++) {
        unsigned char lb = left_border[j];
        unsigned char rb = right_border[j];

        if (lb == INVALID_BORDER || rb == INVALID_BORDER || lb >= rb)
            continue;

        unsigned char cx = (lb + rb) / 2;
        unsigned char w  = rb - lb;

        if (Pixle[j][cx] != 0 &&
            w >= scan_x1[j] / 4 + 1 &&
            w <= (scan_x1[j] * 6) / 5) {
            center_line[j] = cx;
        }
    }
}

/* TRACK_OUTPUT_LCD / TRACK_OUTPUT_WIRELESS 已移至 config.h */

#define TRACK_STATS_OUTPUT     1

uint16_t s_fps = 0;  /* 当前图像帧率, 由主循环每秒更新 */

void TR_Write_TrackStats(void)
{
#if TRACK_STATS_OUTPUT
    char buf[128];
    unsigned short len = (unsigned short)sprintf(buf,
        "TK|FPS:%u|chaL:%.1f|chaR:%.1f|varL:%.1f|varR:%.1f|stdL:%.1f|stdR:%.1f\r\n",
        s_fps, cha_l, cha_r, var_l, var_r, std_l, std_r);
    IR_Wirte_byte((unsigned char *)buf, len);
#endif
}

#define JUMP_DEBUG_OUTPUT  1
#define REQUIRED_JUMP_COLS        20  /* 左T/右T: 需要较多列确认 */
#define REQUIRED_JUMP_COLS_CROSS  10  /* 十字/对称T: 放宽, 双方各需10列 */

extern unsigned char left_valid_count;
extern unsigned char right_valid_count;

static unsigned char s_res_state = 0;
static unsigned char s_turn_flag = 0;
static float s_left_ratio  = 0.0f;   /* 缓存最近一帧的左右白像素面积比, 供WiFi调试输出 */
static float s_right_ratio = 0.0f;

static int g_jump_gap_sum   = 0;  /* 命中列间隙累加, 供 LCD 显示平均间隙 */
static int g_jump_gap_count = 0;

void TR_Write_JumpDebug(void)
{
#if JUMP_DEBUG_OUTPUT
    extern unsigned char left_valid_count;
    extern unsigned char right_valid_count;

    float buf[7];
    buf[0] = (float)top_point;
    buf[1] = (float)left_point;
    buf[2] = (float)right_point;
    buf[3] = (float)left_valid_count;
    buf[4] = (float)right_valid_count;
    buf[5] = s_left_ratio;
    buf[6] = s_right_ratio;

    unsigned char frame[32], *p = frame;
    for (int i = 0; i < 7; i++) {
        unsigned char *fp = (unsigned char *)&buf[i];
        *p++ = fp[0]; *p++ = fp[1]; *p++ = fp[2]; *p++ = fp[3];
    }
    *p++ = 0x00; *p++ = 0x00; *p++ = 0x80; *p++ = 0x7F;
    IR_Wirte_byte(frame, 32);
#endif
}

/* 设置当前帧率(由主循环每秒调用一次) */
void Track_SetFPS(uint16_t fps)
{
    s_fps = fps;
}

static void DrawThickPoint(unsigned char x, unsigned char y, uint16_t color)
{
    if (x >= LCDW || y >= LCDH) return;
    for (char dy = -1; dy <= 1; dy++)
        for (char dx = -1; dx <= 1; dx++)
            if (x + dx < LCDW && y + dy < LCDH)
                Display_DrawPoint(x + dx, y + dy, color);
}

static void DrawImgPoint(unsigned char x, unsigned char y, unsigned char gray, unsigned char *buf)
{
    if (x >= LCDW || y >= LCDH) return;
    for (char dy = -1; dy <= 1; dy++)
        for (char dx = -1; dx <= 1; dx++)
            if (x + dx < LCDW && y + dy < LCDH)
                buf[(y + dy) * LCDW + (x + dx)] = gray;
}

void DrawImgPointBuf(unsigned char x, unsigned char y, unsigned char gray, unsigned char *buf, int stride)
{
    if (x >= LCDW || y >= LCDH) return;
    for (char dy = -1; dy <= 1; dy++)
        for (char dx = -1; dx <= 1; dx++)
            if (x + dx < LCDW && y + dy < LCDH)
                buf[(y + dy) * stride + (x + dx)] = gray;
}

void DisplayTrack(void)
{
#if TRACK_OUTPUT_LCD
    static unsigned char disp_skip = 0;
    char txt[32];

    /* 全图渲染极慢, 每10帧才画一次, 其余帧只更新文字 */
    if (++disp_skip >= 10) {
        disp_skip = 0;
        Display_Show(0, 0, LCDH, LCDW, (unsigned char *)Pixle);

        for (unsigned char j = 0; j < LCDH; j++) {
            if (left_border[j]  != INVALID_BORDER) DrawThickPoint(left_border[j],  j, U16_RED);
            if (right_border[j] != INVALID_BORDER) DrawThickPoint(right_border[j], j, U16_GREEN);
            if (center_line[j]  != INVALID_BORDER) DrawThickPoint(center_line[j],  j, U16_YELLOW);
        }
    }

    sprintf(txt, "Top:%-3d", top_point);
    Display_showString(100, 0, txt, U16_YELLOW, U16_BLACK, 16);
    sprintf(txt, "Bot:%-3d", bottom_point);
    Display_showString(100, 1, txt, U16_YELLOW, U16_BLACK, 16);
//    {
//        if (g_jump_gap_count > 0)
//            sprintf(txt, "Gap:%-3d   ", g_jump_gap_sum / g_jump_gap_count);
//        else
//            sprintf(txt, "Gap:--   ");
//        Display_showString(100, 3, txt, U16_GREEN, U16_BLACK, 16);
//    }
//    {
//        unsigned char jmp;
//        if (left_valid_count  >= REQUIRED_JUMP_COLS && right_valid_count <  REQUIRED_JUMP_COLS)
//            jmp = 4;
//        else if (right_valid_count >= REQUIRED_JUMP_COLS && left_valid_count  <  REQUIRED_JUMP_COLS)
//            jmp = 5;
//        else if (left_valid_count  >= REQUIRED_JUMP_COLS && right_valid_count >= REQUIRED_JUMP_COLS) {
//            if (top_point != INVALID_BORDER && top_point >= T_JUNCTION_TOP)
//                jmp = 3;
//            else
//                jmp = 6;
//        } else
//            jmp = 0;
//        sprintf(txt, "Jmp:%d Lc:%-2d Rc:%-2d  ", jmp, left_valid_count, right_valid_count);
//        Display_showString(100, 4, txt, U16_GREEN, U16_BLACK, 16);
//    }

    {
        const char *ename = Element_GetName(s_turn_flag, RES_NONE);
        sprintf(txt, "E:%-5s  ", ename);
        Display_showString(100, 3, txt, U16_RED, U16_BLACK, 16);
    }
#if PATH_PLAN_ENABLE
    {
        const char *act_str;
        switch (g_pp_action) {
        case ACT_TURN_LEFT:   act_str = "LFT"; break;
        case ACT_TURN_RIGHT:  act_str = "RGT"; break;
        case ACT_GO_STRAIGHT: act_str = "STR"; break;
        default:              act_str = "NAT"; break;
        }
        sprintf(txt, "N:%02d A:%-3s L:%d",
                PathPlan_GetNode(), act_str, g_pp_lock_type);
        Display_showString(100, 4, txt, U16_GREEN, U16_BLACK, 16);

        sprintf(txt, "S:%-4s", PathPlan_GetStateName());
        Display_showString(100, 6, txt, U16_YELLOW, U16_BLACK, 16);
    }
#endif
#endif

#if TRACK_OUTPUT_WIRELESS
    {
        // 图像已在 Cpu0_Main 主循环中拷贝到共享缓冲区并通知 CPU3 发送
        // 统计也在主循环中更新共享缓冲区，由 CPU3 统一发送
    }
#endif
}

static const float weight[LCDH] = {
    0.3f, 0.3f, 0.3f, 0.5f, 0.5f,
    2.0f, 3.5f, 4.5f, 4.8f, 4.9f,
    4.9f, 4.8f, 4.5f, 4.2f, 5.0f,
    3.8f, 3.5f, 3.2f, 2.9f, 2.6f,
    2.3f, 2.0f, 1.8f, 1.6f, 1.4f,
    1.2f, 1.0f, 1.0f, 1.0f, 0.8f,
    0.8f, 0.8f, 0.8f, 0.8f, 0.8f,
    0.7f, 0.7f, 0.7f, 0.7f, 0.7f,
    0.6f, 0.6f, 0.6f, 0.6f, 0.6f,
    0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    0.4f, 0.4f, 0.4f, 0.4f, 0.4f,
    0.3f, 0.3f, 0.3f, 0.3f, 0.3f
};

float get_weighted_diff(void)
{
    float diff_sum = 0.0f;
    int valid_count = 0;
    const float track_center = (LCDW - 1) / 2.0f;

    for (int j = LCDH - 1; j >= 0; j--) {
        if (center_line[j] != INVALID_BORDER) {
            float offset = (track_center - center_line[j]) * weight[j];
            if (offset != 0.0f) {
                diff_sum += offset;
                valid_count++;
            }
        }
    }

    if (valid_count > 0) {
        return diff_sum / valid_count;
    }
    return 0.0f;
}

void Add_Line(int x1, int y1, int x2, int y2)
{
    x1 = CLAMP(x1, 0, LCDW - 1);
    y1 = CLAMP(y1, 0, LCDH - 1);
    x2 = CLAMP(x2, 0, LCDW - 1);
    y2 = CLAMP(y2, 0, LCDH - 1);

    if (y1 > y2) {
        SWAP(int, x1, x2);
        SWAP(int, y1, y2);
    }

    if (y1 == y2) {
        center_line[y1] = (unsigned char)((x1 + x2) / 2);
        return;
    }

    if (x1 == x2) {
        for (int i = y1; i <= y2; ++i) {
            center_line[i] = (unsigned char)x1;
        }
        return;
    }

    const int dx = (x2 > x1) ? (x2 - x1) : (x1 - x2);
    const int dy = y2 - y1;
    const int sx = (x1 < x2) ? 1 : -1;

    int err = dx - dy;
    int x = x1;

    for (int y = y1; y <= y2; ++y) {
        center_line[y] = (unsigned char)x;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
        }
    }
}

void Add_turnline(int x1, int y1, int x2, int y2)
{
    x1 = CLAMP(x1, 0, LCDW - 1);
    y1 = CLAMP(y1, 0, LCDH - 1);
    for (int j = 0; j <= y1; j++) {
        center_line[j] = (unsigned char)x1;
    }

    Add_Line(x1, y1, x2, y2);
}

/*╔════════════════════════════════════════════════════════════════════════════════╗
 *║                     跳变检测法 — 左T/右T识别                                      ║
 *║  原理: 在图像中线左右侧的列中从上向下扫描 "白→黑→白" 双跳变,                          ║
 *║        第1跳(白→黑)=T字横分支下边界, 第2跳(黑→白)=主线边界                             ║
 *╚════════════════════════════════════════════════════════════════════════════════╝*/

static unsigned char is_noise_pixel(unsigned char col, int row)
{
    if (Pixle[row][col] == 0) return 0;
    int white_cnt = 0;
    for (int dr = -2; dr <= 2; dr++) {
        if (dr == 0) continue;
        int r = row + dr;
        if (r >= 0 && r < LCDH && Pixle[r][col] != 0) white_cnt++;
    }
    return (white_cnt <= 1);
}

static unsigned char find_double_jump_in_col(unsigned char col)
{
    int search_end   = LCDH * 2 / 3;
    int first_jump   = -1;

    /* 阶段1: 从图像最顶部往下找 黑→白 (第1跳, 进入横分支) */
    for (int row = 1; row < search_end; row++) {
        if (Pixle[row - 1][col] == 0 && Pixle[row][col] != 0) {
            first_jump = row;
            break;
        }
    }
    if (first_jump < 0) return 0;

    /* 阶段2: 从第1跳之后继续向下, 找下一个 白→黑 (第2跳, 离开横分支) */
    {
        for (int row = first_jump + 1; row < search_end; row++) {
            if (Pixle[row - 1][col] != 0 && Pixle[row][col] == 0) {
                int gap = row - first_jump;  /* 横分支的纵向厚度(行数) */
                if (gap >= 3) {  /* 过滤太薄的噪点, 有效分支至少3行厚 */
                    g_jump_gap_sum   += gap;
                    g_jump_gap_count += 1;
                    return 1;
                }
                /* gap<3 不命中, 继续往下找下一个黑→白→黑 */
            }
        }
    }
    return 0;
}

unsigned char left_valid_count  = 0;
unsigned char right_valid_count = 0;

unsigned char detect_t_by_jumps(void)
{
    int center = LCDW / 2;
    left_valid_count  = 0;
    right_valid_count = 0;
    g_jump_gap_sum    = 0;
    g_jump_gap_count  = 0;

    /* ── 左T扫描: 从左半边扫, col=2 到 col=center-2, 逐列 ──
     *   左T路口 (-|): 横分支在左, 间隙也在左半边
     *   从左往中线方向扫, 穿过"横分支→间隙→主线"区域 */
    for (int col = 2; col < center - 2; col++) {
        if (find_double_jump_in_col((unsigned char)col))
            left_valid_count++;
    }

    /* ── 右T扫描: 从右半边扫, col=center+2 到 col=LCDW-3, 逐列 ──
     *   右T路口 (|-): 横分支在右, 间隙也在右半边
     *   从中线往右扫, 穿过"主线→间隙→横分支"区域 */
    for (int col = center + 2; col < LCDW - 2; col++) {
        if (find_double_jump_in_col((unsigned char)col))
            right_valid_count++;
    }

    if (left_valid_count  >= REQUIRED_JUMP_COLS && right_valid_count <  REQUIRED_JUMP_COLS)
        return 4;
    if (right_valid_count >= REQUIRED_JUMP_COLS && left_valid_count  <  REQUIRED_JUMP_COLS)
        return 5;
    if (left_valid_count  >= REQUIRED_JUMP_COLS && right_valid_count >= REQUIRED_JUMP_COLS)
        return 3;

    return 0;
}

unsigned char check_turn(float threshold1, float threshold2)//顶点往下找5cm 扫中线，以中线的x坐标区分左右，在左右的基础上，左t右t不能复用（框选找分支）
{
    int base_row = CLAMP(top_point, 0, LCDH - 1);
    int base_search_start = CLAMP(base_row + scan_y2[base_row], 0, LCDH - 1);
    int base_search_end = LCDH - 1;
    int valid_center = -1;
    int current_row = 0;

    for (int row = base_search_start; row <= base_search_end; row++) {
        if (left_border[row] != INVALID_BORDER && right_border[row] != INVALID_BORDER) {
            valid_center = (left_border[row] + right_border[row]) / 2;
            int wid = right_border[row] - left_border[row];
            current_row = row;
            if (Pixle[row][valid_center] != 0 &&
                wid >= scan_x1[row] / 5 &&
                wid <= (scan_x1[row] * 21) / 50) {
                current_row = row;
                break;
            }
        }
    }

    if (valid_center == -1) return 0;

    int search_start = CLAMP(top_point, 0, LCDH - 1);
    int search_end = CLAMP(current_row, 0, LCDH - 1);
    if (search_end < LCDH / 2) search_end = LCDH / 2;

    unsigned int left_white = 0, right_white = 0;
    unsigned int left_area = valid_center;
    unsigned int right_area = LCDW - valid_center - 1;

    for (int i = 0; i < valid_center; i++) {
        for (int k = search_start; k <= search_end; k++) {
            if (Pixle[k][i] != 0 &&
                ((i > 0 && Pixle[k][i - 1] != 0) || Pixle[k][i + 1] != 0)) {
                left_white++;
                break;
            }
        }
    }

    for (int i = valid_center + 1; i < LCDW; i++) {
        for (int k = search_start; k <= search_end; k++) {
            if (Pixle[k][i] != 0 &&
                (Pixle[k][i - 1] != 0 || (i + 1 < LCDW && Pixle[k][i + 1] != 0))) {
                right_white++;
                break;
            }
        }
    }

    float left_ratio = (left_area > 0) ? (float)left_white / left_area : 0.0f;
    float right_ratio = (right_area > 0) ? (float)right_white / right_area : 0.0f;

    /* 缓存面积比供 WiFi 调试输出 */
    s_left_ratio  = left_ratio;
    s_right_ratio = right_ratio;

    /* 跳变检测 */
    detect_t_by_jumps();

    /* 从底部5行获取参考边界 */
    unsigned char ref_left  = INVALID_BORDER;
    unsigned char ref_right = 0;
    for (int row = LCDH - 1; row >= LCDH - 5 && row >= 0; row--) {
        if (left_border[row] != INVALID_BORDER && right_border[row] != INVALID_BORDER) {
            if (ref_left == INVALID_BORDER || left_border[row] < ref_left)
                ref_left = left_border[row];
            if (right_border[row] > ref_right)
                ref_right = right_border[row];
        }
    }

#define T_BRANCH_DEVIATION  8

    unsigned char has_left_branch, has_right_branch;
    if (ref_left != INVALID_BORDER && ref_right != 0 && ref_left < ref_right) {
        has_left_branch  = (left_point  != INVALID_BORDER && (ref_left - left_point) >= T_BRANCH_DEVIATION);
        has_right_branch = (right_point != 0 && (right_point - ref_right) >= T_BRANCH_DEVIATION);
    } else {
        has_left_branch  = (left_point  < LEFT_POINT_THRESHOLD);
        has_right_branch = (right_point > RIGHT_POINT_THRESHOLD);
    }

    /*╔══════════════════════════════════════════════════════════════╗
     *║                    路口类型判定                                ║
     *╠══════════════════════════════════════════════════════════════╣
     *║ 左T(4): 跳变法(左≥20) + 中心线贯穿 + 左分支 + 无右分支 + gap≥4  ║
     *║ 右T(5): 跳变法(右≥20) + 中心线贯穿 + 右分支 + 无左分支 + gap≥4  ║
     *║ 十字(6): 跳变法(双方≥20) + 中心线贯穿 (不用面积比)              ║
     *║ 对称T(3): 跳变法(双方≥20) + 面积比(双方>0.7) + 中心线断开        ║
     *║ 左转(1): 面积比 + 中心线断开 (top≥10)                        ║
     *║ 右转(2): 面积比 + 中心线断开 (top≥10)                        ║
     *║ 直行(0): 面积比双方都低 或 中心线贯穿但跳变未确认               ║
     *╚══════════════════════════════════════════════════════════════╝*/

    /* ── 左T: 跳变法(左≥20) + 中心线贯穿 + 左分支 + 无右分支 + gap≥4 ── */
    if (left_valid_count >= REQUIRED_JUMP_COLS && right_valid_count < REQUIRED_JUMP_COLS) {
        if (top_point != INVALID_BORDER && top_point < T_JUNCTION_TOP
            && has_left_branch && !has_right_branch
            && g_jump_gap_count > 0 && (g_jump_gap_sum / g_jump_gap_count) >= 4) {
            return 4;
        }
    }

    /* ── 右T: 跳变法(右≥20) + 中心线贯穿 + 右分支 + 无左分支 + gap≥4 ── */
    if (right_valid_count >= REQUIRED_JUMP_COLS && left_valid_count < REQUIRED_JUMP_COLS) {
        if (top_point != INVALID_BORDER && top_point < T_JUNCTION_TOP
            && has_right_branch && !has_left_branch
            && g_jump_gap_count > 0 && (g_jump_gap_sum / g_jump_gap_count) >= 4) {
            return 5;
        }
    }

    /* ── 十字(6): 跳变法(双方≥10) + 面积比(双方>0.7) + 中心线贯穿 ── */
    if (left_valid_count >= REQUIRED_JUMP_COLS_CROSS && right_valid_count >= REQUIRED_JUMP_COLS_CROSS) {
        if (left_ratio > threshold1 && right_ratio > threshold1
            && top_point != INVALID_BORDER && top_point < T_JUNCTION_TOP)
            return 6;
    }

    /* ── 对称T(3): 跳变法(双方≥10) + 面积比(双方>0.7) + 中心线断开 ── */
    if (left_valid_count >= REQUIRED_JUMP_COLS_CROSS && right_valid_count >= REQUIRED_JUMP_COLS_CROSS) {
        if (left_ratio > threshold1 && right_ratio > threshold1
            && top_point != INVALID_BORDER && top_point >= T_JUNCTION_TOP)
            return 3;
    }

    /* ── 以下为非T字场景, 仅用面积比 ── */

    /* 左转: 左侧白多右侧白少 + 中心线断开 */
    if (left_ratio > threshold1 && right_ratio <= threshold1) {
        if (top_point != INVALID_BORDER && top_point >= T_JUNCTION_TOP)
            return 1;
    }

    /* 右转: 右侧白多左侧白少 + 中心线断开 */
    if (right_ratio > threshold1 && left_ratio <= threshold1) {
        if (top_point != INVALID_BORDER && top_point >= T_JUNCTION_TOP)
            return 2;
    }

    /* 双方都多但跳变法未命中 → 退化为转弯 */
    if (left_ratio > threshold1 && right_ratio > threshold1) {
        if (left_ratio > threshold2 && right_ratio > threshold2
            && top_point != INVALID_BORDER && top_point >= T_JUNCTION_TOP) {
            return 3;  /* 双方都强 + 中心线断开 */
        }
        if (top_point != INVALID_BORDER && top_point >= T_JUNCTION_TOP)
            return (left_ratio > right_ratio) ? 1 : 2;
    }

    /* 直行 */
    return 0;
}

#define T_TURN_DELAY     20   /* T字路口转弯延迟行数, 避免从顶部就开始拉线转弯 */

unsigned char path_decision()
{
    static unsigned char path = 0;
    unsigned char turn_raw = check_turn(0.7f, 0.8f);
    unsigned char turn_flag = turn_raw;

#if PATH_PLAN_ENABLE
    /* 元素锁过滤: 不匹配期望类型 → 视为直行(0), 不触发路口处理 */
    if (g_pp_lock_type != 0 && turn_raw != g_pp_lock_type)
        turn_flag = 0;
#endif

    switch (path)
    {
    case 0:
#if PATH_PLAN_ENABLE
        /* ── 路径规划覆盖: 在原始路口检测之前执行 ── */
        if (g_pp_action == ACT_GO_STRAIGHT) {
            return 0;  /* 强制直行: 不画拉线, 沿自然中线通过 */
        }
        if (g_pp_action == ACT_TURN_LEFT && top_point != INVALID_BORDER) {
            Add_turnline(ADD_LINE, top_point, LCDW / 2 - 1, LCDH - 1);
            path = 1;
            return 1;
        }
        if (g_pp_action == ACT_TURN_RIGHT && top_point != INVALID_BORDER) {
            Add_turnline(LCDW - 1 - ADD_LINE, top_point, LCDW / 2, LCDH - 1);
            path = 2;
            return 2;
        }
#endif
        if (turn_flag == 1 && top_point > TOP_POINT) {
            Add_turnline(ADD_LINE, top_point, LCDW / 2 - 1, LCDH - 1);
            path = 1;
            return 1;
        }
        if (turn_flag == 2 && top_point > TOP_POINT) {
            Add_turnline(LCDW - 1 - ADD_LINE, top_point, LCDW / 2, LCDH - 1);
            path = 2;
            return 2;
        }
        if (turn_flag == 3 && top_point > TOP_POINT) {
            Add_turnline(ADD_LINE, top_point, LCDW / 2 - 1, LCDH - 1);
            path = 3;
            return 3;
        }
        if (turn_flag == 6 && top_point < T_JUNCTION_TOP) {
            Add_turnline(ADD_LINE, top_point, LCDW / 2 - 1, LCDH - 1);
            path = 6;
            return 6;
        }
        if (turn_flag == 4 && top_point < T_JUNCTION_TOP) {
            Add_Line(TURN_MARGIN, CLAMP(top_point + T_TURN_DELAY, 0, LCDH - 1),
                     LCDW / 2 - 1, LCDH - 1);
            path = 4;
            return 4;
        }
        if (turn_flag == 5 && top_point < T_JUNCTION_TOP) {
            Add_Line(LCDW - 1 - TURN_MARGIN, CLAMP(top_point + T_TURN_DELAY, 0, LCDH - 1),
                     LCDW / 2, LCDH - 1);
            path = 5;
            return 5;
        }
        return 0;

    case 1:
#if PATH_PLAN_ENABLE
        if (g_pp_action == ACT_GO_STRAIGHT || g_pp_action == ACT_TURN_RIGHT) {
            path = 0; return 0;  /* 规划方向不符 → 强制退出 */
        }
#endif
#if PATH_PLAN_ENABLE
        if (g_pp_action == ACT_TURN_LEFT) {
            /* 覆盖模式: 只靠边线丢失退出, 不检查路口类型 */
            if (top_point < 3 || top_point == INVALID_BORDER) {
                path = 0; return 0;
            }
        } else
#endif
        {
            /* 自然模式: 检查路口类型是否匹配 */
            if (top_point < 3 || top_point == INVALID_BORDER || check_turn(0.2f, 0.8f) != 1) {
                path = 0; return 0;
            }
        }
        Add_turnline(ADD_LINE, top_point, LCDW / 2 - 1, LCDH - 1);
        return 1;

    case 2:
#if PATH_PLAN_ENABLE
        if (g_pp_action == ACT_GO_STRAIGHT || g_pp_action == ACT_TURN_LEFT) {
            path = 0; return 0;
        }
#endif
#if PATH_PLAN_ENABLE
        if (g_pp_action == ACT_TURN_RIGHT) {
            /* 覆盖模式: 只靠边线丢失退出, 不检查路口类型 */
            if (top_point < 3 || top_point == INVALID_BORDER) {
                path = 0; return 0;
            }
        } else
#endif
        {
            /* 自然模式: 检查路口类型是否匹配 */
            if (top_point < 3 || top_point == INVALID_BORDER || check_turn(0.2f, 0.8f) != 2) {
                path = 0; return 0;
            }
        }
        Add_turnline(LCDW - 1 - ADD_LINE, top_point, LCDW / 2, LCDH - 1);
        return 2;

    case 3:
#if PATH_PLAN_ENABLE
        if (g_pp_action == ACT_GO_STRAIGHT || g_pp_action == ACT_TURN_RIGHT) {
            path = 0; return 0;
        }
#endif
        if (top_point < 3 || top_point == INVALID_BORDER || (check_turn(0.2f, 0.8f) != 3 && check_turn(0.2f, 0.8f) != 1)) {
            path = 0;
            return 0;
        }
        Add_turnline(ADD_LINE, top_point, LCDW / 2 - 1, LCDH - 1);
        return 3;

    case 4:
#if PATH_PLAN_ENABLE
        if (g_pp_action == ACT_GO_STRAIGHT || g_pp_action == ACT_TURN_RIGHT) {
            path = 0; return 0;
        }
#endif
        if (top_point == INVALID_BORDER) {
            path = 0;
            return 0;
        } else {
            unsigned char tf = check_turn(0.2f, 0.8f);
            if (tf != 4 && tf != 1 && tf != 3) {
                path = 0;
                return 0;
            }
        }
        Add_Line(TURN_MARGIN, CLAMP(top_point + T_TURN_DELAY, 0, LCDH - 1),
                 LCDW / 2 - 1, LCDH - 1);
        return 4;

    case 5:
#if PATH_PLAN_ENABLE
        if (g_pp_action == ACT_GO_STRAIGHT || g_pp_action == ACT_TURN_LEFT) {
            path = 0; return 0;
        }
#endif
        if (top_point == INVALID_BORDER) {
            path = 0;
            return 0;
        } else {
            unsigned char tf = check_turn(0.2f, 0.8f);
            if (tf != 5 && tf != 2) {
                path = 0;
                return 0;
            }
        }
        Add_Line(LCDW - 1 - TURN_MARGIN, CLAMP(top_point + T_TURN_DELAY, 0, LCDH - 1),
                 LCDW / 2, LCDH - 1);
        return 5;

    case 6:
#if PATH_PLAN_ENABLE
        if (g_pp_action == ACT_GO_STRAIGHT || g_pp_action == ACT_TURN_RIGHT) {
            path = 0; return 0;
        }
#endif
        if (top_point < 3 || top_point == INVALID_BORDER || (check_turn(0.2f, 0.8f) != 6 && check_turn(0.2f, 0.8f) != 3 && check_turn(0.2f, 0.8f) != 1)) {
            path = 0;
            return 0;
        }
        Add_turnline(ADD_LINE, top_point, LCDW / 2 - 1, LCDH - 1);
        return 6;
    }
    return 0;
}

double cha_l = 0.0, cha_r = 0.0;
double var_l = 0.0, var_r = 0.0;
double std_l = 0.0, std_r = 0.0;
double center_std = 0.0;
uint8_t g_valid_border_rows = LCDH;

static float s_diff = 0.0f;

#define TURN_VOTE_ENABLE  1  /* 多帧投票: 1=启用 0=禁用 */

#if TURN_VOTE_ENABLE
static unsigned char get_stable_turn(unsigned char cur)
{
    static unsigned char hist[3] = {0};
    static unsigned char idx = 0;
    hist[idx++ % 3] = cur;
    int v[6] = {0};
    for (int i = 0; i < 3; i++) v[hist[i]]++;
    int max_v = 0;
    unsigned char best = 0;
    for (int i = 0; i < 6; i++) {
        if (v[i] > max_v) { max_v = v[i]; best = (unsigned char)i; }
    }
    return (max_v >= 2) ? best : cur;
}
#endif

#if BLS_RAMP_ENABLE
void Track_OffTrackCheck(void)
{
    static uint8_t off_track_sec = 0;
    static uint8_t off_track_triggered = 0;

    if (off_track_triggered) return;

    extern char Flag_1s;
    if (Flag_1s)
    {
        if (g_valid_border_rows < 5)
            off_track_sec++;
        else
            off_track_sec = 0;

        if (off_track_sec >= 2)
        {
            extern uint8_t g_motors_enabled;
            g_motors_enabled = 0;
            off_track_triggered = 1;
        }
    }
}
#endif

void Track_Process(void)
{
    detect_border();
    calculate_center_line();
    unsigned char pd_ret = path_decision();
    border_statistics();
#if BLS_RAMP_ENABLE
    Track_OffTrackCheck();
#endif
    s_turn_flag = check_turn(0.7f, 0.8f);
#if TURN_VOTE_ENABLE
    s_turn_flag = get_stable_turn(s_turn_flag);
#endif
    s_res_state = Resistor_Check(cha_l, cha_r, var_l, var_r, std_l, std_r,
                                  top_point, center_std);
    s_diff = -get_weighted_diff();

#if PATH_PLAN_ENABLE
    PathPlan_Process(s_turn_flag, pd_ret);
#endif

     DisplayTrack();
}

float Track_GetDiff(void)
{
    return s_diff;
}

void Track_DebugDisplay(uint16_t fps)
{
    char txt[32];
    sprintf(txt, "FPS:%03d", fps);
    Display_showString(10 * 8, LCDH / 16 + 1, txt, U16_WHITE, U16_BLACK, 16);
    sprintf(txt, "Thr:%03d", Threshold);
    Display_showString(0, LCDH / 16 + 1, txt, U16_WHITE, U16_BLACK, 16);
    sprintf(txt, "Err:%5.1f", s_diff);
    Display_showString(0, LCDH / 16 + 2, txt, U16_WHITE, U16_BLACK, 16);
    sprintf(txt, "Var:%4u", get_last_variance());
    Display_showString(10*8, LCDH / 16 + 2, txt, U16_WHITE, U16_BLACK, 16);
    sprintf(txt, "VTh:%4u", get_var_threshold());
    Display_showString(0, LCDH / 16 + 3, txt, U16_WHITE, U16_BLACK, 16);
}

void border_statistics(void) //计算左右边线的均差/方差/标准差
{
    unsigned char n = 0;
    double sum_l = 0.0, sum_r = 0.0;

    for (unsigned char j = 0; j < LCDH; j++)
    {
        if (left_border[j] != INVALID_BORDER && right_border[j] != INVALID_BORDER)
        {
            sum_l += left_border[j];
            sum_r += right_border[j];
            n++;
        }
    }

    if (n == 0) return;

    double avg_l = sum_l / n;
    double avg_r = sum_r / n;

    double sum_abs_l = 0.0, sum_abs_r = 0.0;
    double sum_sq_l  = 0.0, sum_sq_r  = 0.0;

    for (unsigned char j = 0; j < LCDH; j++)
    {
        if (left_border[j] != INVALID_BORDER && right_border[j] != INVALID_BORDER)
        {
            double dev_l = left_border[j] - avg_l;
            double dev_r = right_border[j] - avg_r;

            sum_abs_l += fabs(dev_l);
            sum_abs_r += fabs(dev_r);
            sum_sq_l  += dev_l * dev_l;
            sum_sq_r  += dev_r * dev_r;
        }
    }

    cha_l = sum_abs_l / n;
    cha_r = sum_abs_r / n;

    var_l = sum_sq_l / n;
    var_r = sum_sq_r / n;

    std_l = sqrt(var_l);
    std_r = sqrt(var_r);

    double center_sum = 0.0, center_sum_sq = 0.0;
    for (unsigned char j = 0; j < LCDH; j++)
    {
        if (left_border[j] != INVALID_BORDER && right_border[j] != INVALID_BORDER)
        {
            double c = (left_border[j] + right_border[j]) * 0.5;
            center_sum += c;
            center_sum_sq += c * c;
        }
    }
    double center_avg = center_sum / n;
    double center_var = center_sum_sq / n - center_avg * center_avg;
    center_std = sqrt(center_var);
    g_valid_border_rows = n;
}

