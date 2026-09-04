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

#include "LQ_SOFTI2C.h"
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void iic_delay(u16 us)
@功能说明：IIC延时
@参数说明：us：延时时间
@函数返回：无
@调用方法：iic_delay();  //简单的延时
@备    注：修改这里可以调整IIC速率
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void iic_delay()
{
    /* 200MHz 系统时钟下 模拟IIC速度为 400Khz */

    unsigned char i = 0;
    for (i = 0; i < 30; i++) // 修改这里可以调整IIC速率
    {
        __asm("NOP"); /* delay */
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void IIC_Init(void)
@功能说明：模拟IIC初始化
@参数说明：无
@函数返回：无
@调用方法：IIC_Init();  //模拟IIC初始化
@备    注：IIC管脚在LQ_SOFTI2C.h中定义
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void IIC_Init(void)
{
    IIC_SCL_INIT;
    IIC_SDA_INIT;
    IIC_SCL_H;
    IIC_SDA_H;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void SOFT_IIC_Start(void)
@功能说明：模拟IIC起始信号
@参数说明：无
@函数返回：无
@调用方法：IIC_Start();
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void IIC_Start(void)
{
    SDA_OUT; // sda线输出
    IIC_SDA_H;
    IIC_SCL_H;
    iic_delay();
    iic_delay();
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SDA_L; // START:when CLK is high,DATA change form high to low
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SCL_L; // 钳住I2C总线，准备发送或接收数据
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void SOFT_IIC_Stop(void)
@功能说明：模拟IIC停止信号
@参数说明：无
@函数返回：无
@调用方法：IIC_Stop();
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void IIC_Stop(void)
{
    SDA_OUT; // sda线输出
    IIC_SCL_L;
    IIC_SDA_L; // STOP:when CLK is high DATA change form low to high
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SCL_H;
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SDA_H; // 发送I2C总线结束信号
    iic_delay();
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：unsigned char SOFT_IIC_Wait_Ack(void)
@功能说明：模拟IIC等待应答信号
@参数说明：无
@函数返回：1，接收应答失败    0，接收应答成功
@调用方法：SOFT_IIC_Wait_Ack();
@备    注：内部调用 有效应答：从机第9个 SCL=0 时 SDA 被从机拉低,并且 SCL = 1时 SDA依然为低
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char IIC_WaitAck(void)
{
    unsigned char ucErrTime = 0;
    SDA_IN; // SDA设置为输入  （从机给一个低电平做为应答）
    IIC_SDA_H;
    iic_delay();
    IIC_SCL_H;
    iic_delay();
    while (IIC_SDA_READ)
    {
        ucErrTime++;
        if (ucErrTime > 100)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL_L; // 时钟输出0
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void IIC_Ack(void)
@功能说明：模拟IIC产生ACK应答
@参数说明：无
@函数返回：无
@调用方法：IIC_Ack();
@备    注：内部调用 主机接收完一个字节数据后，主机产生的ACK通知从机一个字节数据已正确接收
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void IIC_Ack(void)
{
    IIC_SCL_L;
    SDA_OUT;
    IIC_SDA_L;
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SCL_H;
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SCL_L;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void IIC_NAck(void)
@功能说明：模拟IIC不产生ACK应答
@参数说明：无
@函数返回：无
@调用方法：IIC_NAck();
@备    注：内部调用 主机接收完最后一个字节数据后，主机产生的NACK通知从机发送结束，
@           释放SDA,以便主机产生停止信号
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void IIC_NAck(void)
{
    IIC_SCL_L;
    SDA_OUT;
    IIC_SDA_H;
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SCL_H;
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SCL_L;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void IIC_Send_Byte(unsigned char data_t)
@功能说明：模拟IIC发送一个字节
@参数说明：data: 发送的字节
@函数返回：无
@调用方法：IIC_SendByte(0x12);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void IIC_SendByte(unsigned char data_t)
{
    unsigned char t;
    SDA_OUT;
    IIC_SCL_L; // 拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {
        //        IIC_SDA_READ = data_t&0x80;
        if (data_t & 0x80)
        {
            IIC_SDA_H;
        }
        else
        {
            IIC_SDA_L;
        }

        IIC_SCL_H;
        ;
        iic_delay();
        data_t <<= 1;
        iic_delay();
        iic_delay();
        IIC_SCL_L;
        iic_delay();
    }
    iic_delay();
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：unsigned char  IIC_Read_Byte(unsigned char ack)
@功能说明：模拟IIC读取一个字节
@参数说明：ack=1 时，主机数据还没接收完 ack=0 时主机数据已全部接收完成
@函数返回：接收到的字节
@调用方法：IC_ReadByte(0x12);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char IIC_ReadByte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN; // SDA设置为输入模式 等待接收从机返回数据
    for (i = 0; i < 8; i++)
    {
        IIC_SCL_L;
        iic_delay();
        iic_delay();
        iic_delay();
        IIC_SCL_H;
        receive <<= 1;
        if (IIC_SDA_READ)
            receive++; // 从机发送的电平
        iic_delay();
    }
    if (ack)
        IIC_Ack(); // 发送ACK
    else
        IIC_NAck(); // 发送nACK
    return receive;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：unsigned char IIC_ReadByteFromSlave(unsigned char I2C_Addr,unsigned char reg,unsigned char *buf)
@功能说明：模拟IIC读取指定设备 指定寄存器的一个值
@参数说明：I2C_Addr：目标设备地址 reg ：目标寄存器 buf ：存放读出字节
@函数返回：1失败 0成功
@调用方法：IIC_ReadByteFromSlave(0xD0, 0x75, buf);   //读 IIC地址为 0xD0器件（SOFT_I2C）寄存器0x75
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char IIC_ReadByteFromSlave(unsigned char I2C_Addr, unsigned char reg, unsigned char *buf)
{
    IIC_Start();
    IIC_SendByte(I2C_Addr); // 发送从机地址
    if (IIC_WaitAck())      // 如果从机未应答则数据发送失败
    {
        IIC_Stop();
        return 1;
    }
    IIC_SendByte(reg); // 发送寄存器地址
    IIC_WaitAck();

    IIC_Start();
    IIC_SendByte(I2C_Addr + 1); // 进入接收模式
    IIC_WaitAck();
    *buf = IIC_ReadByte(0);
    IIC_Stop(); // 产生一个停止条件
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：unsigned char IIC_WriteByteToSlave(unsigned char I2C_Addr,unsigned char reg,unsigned char data_t)
@功能说明：模拟IIC写指定设备 指定寄存器的一个值
@参数说明：I2C_Addr：目标设备地址 reg：目标寄存器data：写入的数据
@函数返回：1失败 0成功
@调用方法：IIC_ReadByteFromSlave(0xD0, 0X6B, 0X80);   //IIC地址为 0xD0器件（SOFT_I2C）寄存器0x6B 写入0x80
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char IIC_WriteByteToSlave(unsigned char I2C_Addr, unsigned char reg, unsigned char data_t)
{
    IIC_Start();
    IIC_SendByte(I2C_Addr); // 发送从机地址
    if (IIC_WaitAck())
    {
        IIC_Stop();
        return 1; // 从机地址写入失败
    }
    IIC_SendByte(reg); // 发送寄存器地址
    IIC_WaitAck();
    IIC_SendByte(data_t);
    if (IIC_WaitAck())
    {
        IIC_Stop();
        return 1; // 数据写入失败
    }
    IIC_Stop(); // 产生一个停止条件

    // return 1; //status == 0;
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：unsigned char IIC_ReadMultByteFromSlave(unsigned char dev, unsigned char reg, unsigned char len, unsigned char *data_t)
@功能说明：模拟IIC读取指定设备 指定寄存器的n个值
@参数说明：dev：目标设备地址  reg：目标寄存器  len：读取长度  data：存放读出数据
@函数返回：1失败 0成功
@调用方法：IIC_ReadByteFromSlave(0xD0, 0X3B, 14, &data);   //读 14个字节
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char IIC_ReadMultByteFromSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char *data_t)
{
    unsigned char count = 0;
    unsigned char temp;
    IIC_Start();
    IIC_SendByte(dev); // 发送从机地址
    if (IIC_WaitAck())
    {
        IIC_Stop();
        return 1; // 从机地址写入失败
    }
    IIC_SendByte(reg); // 发送寄存器地址
    IIC_WaitAck();
    IIC_Start();
    IIC_SendByte(dev + 1); // 进入接收模式
    IIC_WaitAck();
    for (count = 0; count < length; count++)
    {
        if (count != (length - 1))
            temp = IIC_ReadByte(1); // 带ACK的读数据
        else
            temp = IIC_ReadByte(0); // 最后一个字节NACK

        data_t[count] = temp;
    }
    IIC_Stop(); // 产生一个停止条件
    // return count;
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：unsigned char IIC_WriteMultByteToSlave(unsigned char dev, unsigned char reg, unsigned char len, unsigned char* data_t)
@功能说明：模拟IIC写指定设备 指定寄存器的n个值
@参数说明：dev：目标设备地址  reg：目标寄存器  len：写入长度  data：存放写入数据
@函数返回：1失败 0成功
@调用方法：IIC_WriteMultByteToSlave(0xD0, 0X6B, 1, 0X80);   //向寄存器0x6B写入0x80
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char IIC_WriteMultByteToSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char *data_t)
{

    unsigned char count = 0;
    IIC_Start();
    IIC_SendByte(dev); // 发送从机地址
    if (IIC_WaitAck())
    {
        IIC_Stop();
        return 1; // 从机地址写入失败
    }
    IIC_SendByte(reg); // 发送寄存器地址
    IIC_WaitAck();
    for (count = 0; count < length; count++)
    {
        IIC_SendByte(data_t[count]);
        if (IIC_WaitAck()) // 每一个字节都要等从机应答
        {
            IIC_Stop();
            return 1; // 数据写入失败
        }
    }
    IIC_Stop(); // 产生一个停止条件

    return 0;
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：int LQ_I2C_Write(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *dat)
@功能说明：模拟SPI读写数据及长度
@参数说明：addr：标设备地址  reg：目标寄存器  len：写入长度  *dat：存放写入数据
@函数返回：1失败 0成功
@调用方法：LQ_I2C_Write(0x68, 0x38, 1, tmp)；
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int LQ_I2C_Write(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *dat)
{
    unsigned int i = 0;

    IIC_Start();
    IIC_SendByte(addr << 1);
    IIC_WaitAck();

    IIC_SendByte(reg);
    IIC_WaitAck();

    for (i = 0; i < len; i++)
    {
        IIC_SendByte(dat[i]);
        IIC_WaitAck();
    }
    IIC_Stop();
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：int LQ_I2C_Read(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *buf)
@功能说明：模拟SPI读写数据及长度
@参数说明：addr ：标设备地址  reg ：目标寄存器  len ：写入长度  *buf：存放写入数据
@函数返回：1失败 0成功
@调用方法：LQ_I2C_Write(0x68, 0x38, 1, tmp)
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int LQ_I2C_Read(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *buf)
{
    IIC_Start();
    IIC_SendByte(addr << 1);
    IIC_WaitAck();

    IIC_SendByte(reg);
    IIC_WaitAck();
    iic_delay();
    IIC_Start();

    IIC_SendByte((addr << 1) + 1);
    IIC_WaitAck();
    while (len)
    {
        if (len == 1)
            *buf = IIC_ReadByte(0);
        else
            *buf = IIC_ReadByte(1);
        buf++;
        len--;
    }
    IIC_Stop();
    return 0;
}
