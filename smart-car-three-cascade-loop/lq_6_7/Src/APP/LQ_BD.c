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

#include "LQ_BD.h"
#include "lq_include.h"

Position_t point_p;
_SaveData Save_Data;

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void parseGpsBuffer(void);
@功能说明：数据解析
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@备    注：将串口收到的数据分门别类的保存到结构体中
@           $GNRMC,130416.000,A,2236.91843,N,11359.19128,E,0.001,306.28,090720,,,A*4F
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void parseGpsBuffer(void)
{
    char *subString;
    char *subStringNext;
    char i = 0;
    if (Save_Data.isGetData) // 如果字符串不为空
    {
        for (i = 0; i <= 8; i++) // 循环7次
        {
            if (i == 0)
            {
                if ((subString = strchr(Save_Data.GPS_Buffer, ',')) == NULL) // 没有检测到逗号
                    while (1)
                        ; // 解析错误
            }
            else // 检测到逗号，返回逗号的位置
            {
                subString++;                                          // 位置加1（定位到逗号的后一位）
                if ((subStringNext = strchr(subString, ',')) != NULL) // 定位下一个逗号的位置
                {
                    char usefullBuffer[2];
                    switch (i)
                    {
                    case 1:
                        memcpy(Save_Data.UTCTime, subString, subStringNext - subString); // 两个逗号之间为 时间信息 并转换成北京时间
                        Save_Data.UTCTime[1] = Save_Data.UTCTime[1] + 8;
                        if (Save_Data.UTCTime[1] > '9')
                        {
                            Save_Data.UTCTime[0]++;
                            if (Save_Data.UTCTime[0] == '3')
                                Save_Data.UTCTime[0] = '0';
                            Save_Data.UTCTime[1] = (Save_Data.UTCTime[1] % '9') + '0' - 1;
                        }
                        break; // 结束switch
                    case 2:
                        memcpy(usefullBuffer, subString, subStringNext - subString);
                        break; // 数据是否有效标志
                    case 3:
                        memcpy(Save_Data.latitude, subString, subStringNext - subString);
                        break; // 获取纬度信息
                    case 4:
                        memcpy(Save_Data.N_S, subString, subStringNext - subString);
                        break; // 获取N/S
                    case 5:
                        memcpy(Save_Data.longitude, subString, subStringNext - subString);
                        break; // 获取经度信息
                    case 6:
                        memcpy(Save_Data.E_W, subString, subStringNext - subString);
                        break; // 获取E/W
                    case 7:
                        memcpy(Save_Data.speed, subString, subStringNext - subString);
                        break; // 获取速度
                    case 8:
                        memcpy(Save_Data.direction, subString, subStringNext - subString);
                        break; // 获取方向
                    default:
                        break;
                    }
                    subString = subStringNext; // 下一个逗号位置给第一个指针，
                    Save_Data.isParseData = 1; // 手动给真值，（数据是否解析完成）
                    //          if(usefullBuffer[0] == 'A')
                    //            Save_Data.isUsefull = true;
                    //          else if(usefullBuffer[0] == 'V')
                    //            Save_Data.isUsefull = false;
                }
                else
                {
                    while (1)
                        ; // 解析错误
                }
            }
        }
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：char Get_IntData(int* Lon_Z, int* Lon_X, int* Lat_Z, int* Lat_X);
@功能说明：经纬度原始值
@参数说明：Lon_Z（经度整数），Lon_X（经度小数），Lat_Z（纬度整数）,Lat_X（纬度小数）
@函数返回：无
@修改时间：2022/02/24
@备    注：将结构体中的字符串转换成数据，有两种方式，#if 1：以小数点为分界。#if 0：自己手动调节
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
char Get_IntData(int *Lon_Z, int *Lon_X, int *Lat_Z, int *Lat_X)
{
#if 1
    // 判断纬度值中是否有‘.'有:说明有数据 例如：3946.99715  变化范围在后六位中变化，最后一位不稳定可舍去。取值为3946和99715
    if ((strstr(Save_Data.latitude, ".")) != NULL)
    {
        sscanf(Save_Data.latitude, "%d.%d", &(*Lat_Z), &(*Lat_X));
    }
    else
        return 0;
    // 经度原理同上 例如：11628.32198  取值为11628 和 32198
    if ((strstr(Save_Data.longitude, ".")) != NULL)
    {
        sscanf(Save_Data.longitude, "%d.%d", &(*Lon_Z), &(*Lon_X));
    }
    else
        return 0;
    return 1;
#else
    unsigned num = 0;
    // 判断纬度值中是否有‘.'有:说明有数据 例如：3946.99715  变化范围在后六位中变化，最后一位不稳定可舍去。
    if ((strstr(Save_Data.latitude, ".")) != NULL)
    {
        *Lat_Z = (Save_Data.latitude[0] - '0') * 100 + (Save_Data.latitude[1] - '0') * 10 + (Save_Data.latitude[2] - '0');
        num = (Save_Data.latitude[3] - '0') * 100000 + (Save_Data.latitude[5] - '0') * 10000 + (Save_Data.latitude[6] - '0') * 1000 +
              (Save_Data.latitude[7] - '0') * 100 + (Save_Data.latitude[8] - '0') * 10 + (Save_Data.latitude[9] - '0');
        *Lat_X = num; // 取值为394 和 699715
    }
    else
        return 0;
    // 经度原理同上 例如：11628.32198
    if ((strstr(Save_Data.longitude, ".")) != NULL)
    {
        *Lon_Z = (Save_Data.longitude[0] - '0') * 1000 + (Save_Data.longitude[1] - '0') * 100 +
                 (Save_Data.longitude[2] - '0') * 10 + (Save_Data.longitude[3] - '0');
        num = (Save_Data.longitude[4] - '0') * 100000 + (Save_Data.longitude[6] - '0') * 10000 + (Save_Data.longitude[7] - '0') * 1000 +
              (Save_Data.longitude[8] - '0') * 100 + (Save_Data.longitude[9] - '0') * 10 + (Save_Data.longitude[10] - '0');
        *Lon_X = num; // 取值为1162 和 832198
    }
    else
        return 0;
    return 1;
#endif
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：char Get_DoubleData(double* Lon, double* Lat)
@功能说明：经纬度字符串转化成浮点数
@参数说明：Lon（经度），Lat（纬度）
@函数返回：0:转化失败 1：转化成功
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
char Get_DoubleData(_SaveData *data, double *Lon, double *Lat)
{
    double W, J;
    J = strtod(data->longitude, NULL);
    W = strtod(data->latitude, NULL);
    if (W == 0.0)
        return 0;
    else
        *Lon = J / 100.0;
    if (J == 0.0)
        return 0;
    else
        *Lat = W / 100.0;
    return 1;
}

void Test_BD1202(void)
{

    char txt[32] /*, num=0*/;
    double Lon = 0.0, Lat = 0.0, tim = 0.0;
    Display_Init(0);
    GPIO_LED_Init();
    Display_CLS(U16_WHITE);
    UART_InitConfig(UART0_RX_P14_1, UART0_TX_P14_0, 115200); // 接收上位机指令
    UART_InitConfig(UART3_RX_P00_1, UART3_TX_P00_0, 115200); // 接收北斗模块坐标

    sprintf(txt, "BD Test");
    Display_showString(0, 0, txt, U16_RED, U16_BLUE, 12); // 将txt中 内容显示出来

    while (1)
    {
        if (Save_Data.isGetData) // 如果字符串不为空
        {
            if (EOF != sscanf(Save_Data.GPS_Buffer, "$BDRMC,%lf,A,%lf,N,%lf,E,", &tim, &Lat, &Lon))
            {
                sprintf(txt, "Parsing BeiDou");
                Display_showString(0, 1, txt, U16_RED, U16_BLUE, 16); // 将txt中 内容显示出来
            }
            else if (EOF != sscanf(Save_Data.GPS_Buffer, "$GNRMC,%lf,A,%lf,N,%lf,E,", &tim, &Lat, &Lon))
            {
                sprintf(txt, "Parsing GPS   ");
                Display_showString(0, 2, txt, U16_RED, U16_BLUE, 16); // 将txt中 内容显示出来
            }
            else
            {
                sprintf(txt, "Parsing error ");
                Display_showString(0, 1, txt, U16_RED, U16_BLUE, 16); // 将txt中 内容显示出来
            }
        }

        sprintf(txt, "Type:double");
        Display_showString(0, 3, txt, U16_RED, U16_BLUE, 16); // 将txt中 内容显示出来
        sprintf(txt, "T:%f", tim);
        Display_showString(0, 4, txt, U16_RED, U16_BLUE, 16); // 将txt中 内容显示出来
        sprintf(txt, "N:%f", Lat / 100);
        Display_showString(0, 5, txt, U16_RED, U16_BLUE, 16); // 将txt中 内容显示出来// 转化成数据
        sprintf(txt, "E:%f", Lon / 100);
        Display_showString(0, 6, txt, U16_RED, U16_BLUE, 16); // 将txt中 内容显示出来// 转化成数据
        LED_Ctrl(LED0, RVS);                                  // LED翻转闪烁
    }
}
