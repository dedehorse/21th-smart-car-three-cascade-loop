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

#include "LQ_UTM.h"

double rad(double d)
{
    return d * PI / 180.0;
}

int get_zone(double lon)
{
    int zone = (int)floor((lon + 180.0) / 6.0) + 1;
    if (zone < 1)
        zone = 1;
    if (zone > 60)
        zone = 60;
    return zone;
}

struct UTM latlon2utm(struct Point p)
{
    double a = 6378137.0;           // WGS84椭球长半轴
    double f = 1.0 / 298.257223563; // WGS84椭球扁率
    double k0 = 0.9996;             // UTM投影标准纵缩比
    double lon0, x0, y0, e, e2, n, t, c, A, M;
    int zone;

    zone = get_zone(p.lon);
    lon0 = (double)(zone - 1) * 6 - 180 + 3; // 中央经线
    x0 = 500000.0;
    y0 = p.lat >= 0 ? 0 : 10000000.0;

    e = sqrt(f * (2 - f));
    e2 = e * e;
    n = a / sqrt(1 - e2 * sin(rad(p.lat)) * sin(rad(p.lat)));
    t = tan(rad(p.lat)) * tan(rad(p.lat));
    c = e2 * cos(rad(p.lat)) * cos(rad(p.lat));
    A = cos(rad(p.lat)) * (rad(p.lon) - rad(lon0));
    M = a * ((1 - e2 / 4 - 3 * e2 * e2 / 64 - 5 * e2 * e2 * e2 / 256) * rad(p.lat) - (3 * e2 / 8 + 3 * e2 * e2 / 32 + 45 * e2 * e2 * e2 / 1024) * sin(2 * rad(p.lat)) + (15 * e2 * e2 / 256 + 45 * e2 * e2 * e2 / 1024) * sin(4 * rad(p.lat)) - (35 * e2 * e2 * e2 / 3072) * sin(6 * rad(p.lat)));

    struct UTM utm;
    utm.zone = zone;
    utm.x = k0 * n * (A + (1 - t + c) * A * A * A / 6 + (5 - 18 * t + t * t + 72 * c - 58 * e2) * A * A * A * A * A / 120) + x0;
    utm.y = k0 * (M + n * tan(rad(p.lat)) * (A * A / 2 + (5 - t + 9 * c + 4 * c * c) * A * A * A * A / 24 + (61 - 58 * t + t * t + 600 * c - 330 * e2) * A * A * A * A * A * A / 720));

    if (p.lat < 0)
    {
        utm.y += 10000000.0; //// 南半球加伪北偏移
    }

    return utm;
}

int Test_UTM(void)
{
    UART_InitConfig(UART0_RX_P14_1, UART0_TX_P14_0, 115200);
    struct Point p1 = {39.908722, 116.397499}; // 天安门广场坐标
    struct UTM utm1 = latlon2utm(p1);
    printf("UTM坐标(带号%d):%.2f, %.2f\n", utm1.zone, utm1.x, utm1.y);
    return 0;
}
