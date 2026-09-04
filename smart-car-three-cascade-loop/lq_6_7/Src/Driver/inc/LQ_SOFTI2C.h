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

#ifndef __LQ_SOFTIIC_H
#define __LQ_SOFTIIC_H

#include "lq_include.h"

/* 注意 IIC总线规定，IIC空闲时 SCL和SDA都为高电平 最好外部上拉（一定不能下拉） */
/* 模拟 IIC需要注意，IIC地址左移一位 例如MPU6050 模拟就是地址 0xD0 */
/* 想换用别的IO 直接修改宏定义 SOFT_IIC_SCL_PIN 、 SOFT_IIC_SDA_PIN 即可 */
#define SOFT_IIC_SCL_PIN P13_1 /*!< P13_1  作为 SCL */
#define SOFT_IIC_SDA_PIN P13_2 /*!< P13_2  作为 SDA */

#define SDA_OUT PIN_Dir(SOFT_IIC_SDA_PIN, 1);
#define SDA_IN PIN_Dir(SOFT_IIC_SDA_PIN, 0);

#define IIC_SCL_INIT PIN_InitConfig(SOFT_IIC_SCL_PIN, PIN_MODE_OUTPUT, 1);
#define IIC_SDA_INIT PIN_InitConfig(SOFT_IIC_SDA_PIN, PIN_MODE_OUTPUT, 1);

#define IIC_SCL_H PIN_Write(SOFT_IIC_SCL_PIN, 1);
#define IIC_SCL_L PIN_Write(SOFT_IIC_SCL_PIN, 0);

#define IIC_SDA_H PIN_Write(SOFT_IIC_SDA_PIN, 1);
#define IIC_SDA_L PIN_Write(SOFT_IIC_SDA_PIN, 0);

#define IIC_SDA_READ PIN_Read(SOFT_IIC_SDA_PIN)

void IIC_Start(void);                          // 发送IIC开始信号
void IIC_Stop(void);                           // 发送IIC停止信号
void IIC_Ack(void);                            // IIC发送ACK信号
void IIC_NAck(void);                           // IIC不发送ACK信号
unsigned char IIC_WaitAck(void);               // IIC等待ACK信号
void IIC_SendByte(unsigned char data_t);       // IIC发送一个字节
unsigned char IIC_ReadByte(unsigned char ack); // IIC读取一个字节
int LQ_I2C_Write(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *dat);
int LQ_I2C_Read(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *buf);
void IIC_Init(void);
unsigned char IIC_ReadByteFromSlave(unsigned char I2C_Addr, unsigned char reg, unsigned char *buf);
unsigned char IIC_ReadMultByteFromSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char *data_t);
unsigned char IIC_WriteByteToSlave(unsigned char I2C_Addr, unsigned char reg, unsigned char buf);
unsigned char IIC_WriteMultByteToSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char *data_t);

#endif
