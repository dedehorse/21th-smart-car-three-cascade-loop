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

#ifndef _LQ_BD_H_
#define _LQ_BD_H_

typedef enum
{
    N = 0,
    S
} lat;
typedef enum
{
    E = 0,
    W
} lon;

typedef struct
{
    double PX; // 坐标值
    double PY;
    double Direction; // 车头朝向（方向）
} Position_t;

typedef struct SaveData
{
    char GPS_Buffer[128]; // 完整数据
    char isGetData;       // 是否获取到GPS数据
    char UTCTime[11];     // UTC时间
    char isParseData;     // 是否解析完成
    char latitude[11];    // 纬度
    char N_S[2];          // N/S
    char longitude[12];   // 经度
    char E_W[2];          // E/W
    char isUsefull;       // 定位信息是否有效
    char speed[6];        // 速度 单位：节
    char direction[6];    // 方向
} _SaveData;

extern _SaveData Save_Data;
extern Position_t point_p;
char Get_DoubleData(_SaveData *data, double *Lon, double *Lat);
char Get_IntData(int *Lon_Z, int *Lon_X, int *Lat_Z, int *Lat_X);
int BD_getdata(Position_t *Current_Point);
void Test_BD1202(void);

#endif
