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

#include "LQ_SOFTSPI.h"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Soft_SPI_Init(void)
@功能说明：模拟SPI初始化
@参数说明：无
@函数返回：无
@调用方法：无
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Soft_SPI_Init(void)
{
    PIN_InitConfig(Soft_SPI_SCK_PIN, PIN_MODE_OUTPUT, 1);
    PIN_InitConfig(Soft_SPI_MISO_PIN, PIN_MODE_INPUT, 0);
    PIN_InitConfig(Soft_SPI_MOSI_PIN, PIN_MODE_OUTPUT, 1);
    PIN_InitConfig(Soft_SPI_CS_PIN, PIN_MODE_OUTPUT, 0);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Soft_SPI_ReadWriteNbyte(uint8_t *lqbuff, uint8_t len)
@功能说明：模拟SPI读写数据及长度
@参数说明：uint8_t *buf数据指针,u16 len长度
@函数返回：无
@调用方法：SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Soft_SPI_ReadWriteNbyte(uint8_t *lqbuff, uint8_t len)
{
    uint8_t i;

    Soft_SPI_CS_OUT_L;
    Soft_SPI_SCK_OUT_H;
    do
    {
        for (i = 0; i < 8; i++)
        {
            if ((*lqbuff) >= 0x80)
                Soft_SPI_MOSI_OUT_H;
            else
                Soft_SPI_MOSI_OUT_L;
            Soft_SPI_SCK_OUT_L;
            (*lqbuff) <<= 1;
            Soft_SPI_SCK_OUT_H;
            (*lqbuff) |= Soft_SPI_MISO_IN;
        }
        lqbuff++;
    } while (--len);
    Soft_SPI_CS_OUT_H;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf)
@功能说明：SPI从设备读取数据
@参数说明：unsigned char reg,设备起始地址unsigned char* buf,数据存放地址unsigned short len,待写入的数据长度
@函数返回：无
@调用方法：SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char *buf)
{
    unsigned short i;
    buf[0] = reg | 0x80;
    Soft_SPI_ReadWriteNbyte(buf, len + 1);
    for (i = 0; i < len; i++)
        buf[i] = buf[i + 1];
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_SPI_Write(unsigned char reg, unsigned short len, unsigned char* buf)
@功能说明：SPI向设备写入数据
@参数说明：unsigned char reg,设备起始地址unsigned char* buf,数据存放地址unsigned short len,待写入的数据长度
@函数返回：无
@调用方法：无
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_SPI_Write(unsigned char reg, unsigned char value)
{
    unsigned char buff[2];

    buff[0] = reg & 0x7f; // 先发送寄存器
    buff[1] = value;      // 再发送数据
    Soft_SPI_ReadWriteNbyte(buff, 2);
}
