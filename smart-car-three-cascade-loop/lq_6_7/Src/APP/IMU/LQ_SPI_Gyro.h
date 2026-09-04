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

#ifndef __LQ_SPI_IMU_H_
#define __LQ_SPI_IMU_H_

#include "stdint.h"
#include "LQ_SPI.h"

/* IMU模块SPI 引脚设置 */
#define IMU_QSPI_PORT   QSPI2
#define IMU_QSPI_SCLK   QSPI2_CLK_P15_3
#define IMU_QSPI_MOSI   QSPI2_MOSI_P15_6
#define IMU_QSPI_MISO   QSPI2_MISO_P15_7
#define IMU_QSPI_SLCS   QSPI2_CS_P15_2

#define GYRO_CS_        PIN P20_13
#define GYRO_CS_H       PIN_Write(GYRO_CS_PIN, 1)
#define GYRO_CS_L       PIN_Write(GYRO_CS_PIN, 0)

typedef enum // SPI通信陀螺仪类型
{
    error,
    SPI_ICM20602,
    SPI_ICM20689,
    SPI_MPU9250,
    SPI_ICM42605,
    SPI_ICM42688,
    SPI_LSM6DSR,
    SPI_LQ9AGMV
}GyroType_t;


//*!**************************************
// 定义ICM20602内部地址
//*!**************************************
// MPU6500的内部寄存器
#define ICM_SELF_TESTX_REG 0X0D  // 自检寄存器X
#define ICM_SELF_TESTY_REG 0X0E  // 自检寄存器Y
#define ICM_SELF_TESTZ_REG 0X0F  // 自检寄存器Z
#define ICM_SELF_TESTA_REG 0X10  // 自检寄存器A
#define ICM_SAMPLE_RATE_REG 0X19 // 采样频率分频器
#define ICM_CFG_REG 0X1A         // 配置寄存器
#define ICM_GYRO_CFG_REG 0X1B    // 陀螺仪配置寄存器
#define ICM_ACCEL_CFG_REG 0X1C   // 加速度计配置寄存器
#define ICM_MOTION_DET_REG 0X1F  // 运动检测阀值设置寄存器
#define ICM_FIFO_EN_REG 0X23     // FIFO使能寄存器

#define ICM_I2CMST_STA_REG 0X36 // IIC主机状态寄存器
#define ICM_INTBP_CFG_REG 0X37  // 中断/旁路设置寄存器
#define ICM_INT_EN_REG 0X38     // 中断使能寄存器
#define ICM_INT_STA_REG 0X3A    // 中断状态寄存器

#define ICM_ACCEL_XOUTH_REG 0X3B // 加速度值,X轴高8位寄存器
#define ICM_ACCEL_XOUTL_REG 0X3C // 加速度值,X轴低8位寄存器
#define ICM_ACCEL_YOUTH_REG 0X3D // 加速度值,Y轴高8位寄存器
#define ICM_ACCEL_YOUTL_REG 0X3E // 加速度值,Y轴低8位寄存器
#define ICM_ACCEL_ZOUTH_REG 0X3F // 加速度值,Z轴高8位寄存器
#define ICM_ACCEL_ZOUTL_REG 0X40 // 加速度值,Z轴低8位寄存器


#define ICM_ACCEL_DATA_X1     0X1F    //加速度值,X轴高8位寄存器  ICM42605
#define ICM_ACCEL_DATA_X0     0X20    //加速度值,X轴低8位寄存器
#define ICM_ACCEL_DATA_Y1     0X21    //加速度值,Y轴高8位寄存器
#define ICM_ACCEL_DATA_Y0     0X22    //加速度值,Y轴低8位寄存器
#define ICM_ACCEL_DATA_Z1     0X23    //加速度值,Z轴高8位寄存器
#define ICM_ACCEL_DATA_Z0     0X24    //加速度值,Z轴低8位寄存器

#define ICM_TEMP_OUTH_REG 0X41 // 温度值高八位寄存器
#define ICM_TEMP_OUTL_REG 0X42 // 温度值低8位寄存器

#define ICM_GYRO_XOUTH_REG 0X43 // 陀螺仪值,X轴高8位寄存器
#define ICM_GYRO_XOUTL_REG 0X44 // 陀螺仪值,X轴低8位寄存器
#define ICM_GYRO_YOUTH_REG 0X45 // 陀螺仪值,Y轴高8位寄存器
#define ICM_GYRO_YOUTL_REG 0X46 // 陀螺仪值,Y轴低8位寄存器
#define ICM_GYRO_ZOUTH_REG 0X47 // 陀螺仪值,Z轴高8位寄存器
#define ICM_GYRO_ZOUTL_REG 0X48 // 陀螺仪值,Z轴低8位寄存器

#define ICM_I2CSLV0_DO_REG 0X63 // IIC从机0数据寄存器
#define ICM_I2CSLV1_DO_REG 0X64 // IIC从机1数据寄存器
#define ICM_I2CSLV2_DO_REG 0X65 // IIC从机2数据寄存器
#define ICM_I2CSLV3_DO_REG 0X66 // IIC从机3数据寄存器

#define ICM_I2CMST_DELAY_REG 0X67 // IIC主机延时管理寄存器
#define ICM_SIGPATH_RST_REG 0X68  // 信号通道复位寄存器
#define ICM_MDETECT_CTRL_REG 0X69 // 运动检测控制寄存器
#define ICM_USER_CTRL_REG 0X6A    // 用户控制寄存器
#define ICM_PWR_MGMT1_REG 0X6B    // 电源管理寄存器1
#define ICM_PWR_MGMT2_REG 0X6C    // 电源管理寄存器2
#define ICM_FIFO_CNTH_REG 0X72    // FIFO计数寄存器高八位
#define ICM_FIFO_CNTL_REG 0X73    // FIFO计数寄存器低八位
#define ICM_FIFO_RW_REG 0X74      // FIFO读写寄存器
#define WHO_AM_I 0X75             // 器件ID寄存器

//LSM6D陀螺仪寄存器定义
#define DRV_ID_LSM6D    0x6B    //设备ID
#define WHO_AM_LSM6D    0x0F    //设备ID寄存器

#define LSM6DSR_FUNC_CFG  0X01

#define LSM6DSR_INT1_CTRL 0X0D      //
#define LSM6DSR_INT2_CTRL 0X0E      //

#define LSM6DSR_CTRL1_XL 0X10       // 加速度计控制寄存器1 (r/w) bit1:0:一级数字滤波输出。1:LPF2第二级滤波输出 bit[2:3]:加速度计量程选择，默认为00:±2g 01:±16g 10:±4g 11:±8g
#define LSM6DSR_CTRL2_G 0X11        //
#define LSM6DSR_CTRL3_C 0X12        //
#define LSM6DSR_CTRL4_C 0X13        //
#define LSM6DSR_CTRL5_C 0X14        //
#define LSM6DSR_CTRL6_C 0X15        //
#define LSM6DSR_CTRL7_G 0X16        //
#define LSM6DSR_CTRL8_XL 0X17       //
#define LSM6DSR_CTRL9_XL 0X18       //
#define LSM6DSR_CTRL10_C 0X19       //

#define LSM6DSR_STATUS_REG 0X1E     //

#define LSM6DSR_OUT_TEMP_L 0X20     //
#define LSM6DSR_OUT_TEMP_H 0X21     //

#define LSM6DSR_OUTX_L_GYRO 0X22    //
#define LSM6DSR_OUTX_H_GYRO 0X23    //
#define LSM6DSR_OUTY_L_GYRO 0X24    //
#define LSM6DSR_OUTY_H_GYRO 0X25    //
#define LSM6DSR_OUTZ_L_GYRO 0X26    //
#define LSM6DSR_OUTZ_H_GYRO 0X27    //

#define LSM6DSR_OUTX_L_ACC 0X28     //
#define LSM6DSR_OUTX_H_ACC 0X29     //
#define LSM6DSR_OUTY_L_ACC 0X2A     //
#define LSM6DSR_OUTY_H_ACC 0X2B     //
#define LSM6DSR_OUTZ_L_ACC 0X2C     //
#define LSM6DSR_OUTZ_H_ACC 0X2D     //

#define LSM6DSR_I3C_BUS_AVB 0x62    //
#define PROPERTY_ENABLE (1U)        //
#define PROPERTY_DISABLE (0U)       //

#define Mag_ID  0x3D

/* @brief    测试20602 SPI接线方式*/
void Test_SPI_Gyro(void);
/*@brief    初始化ICM20602 或者 ICM20602*/
unsigned char SPI_Gryo_Init(void);
/*@brief    设置陀螺仪测量范围*/
unsigned char ICM_Set_Gyro_Fsr(unsigned char fsr);
/*@brief    设置加速度计测量范围*/
unsigned char ICM_Set_Accel_Fsr(unsigned char fsr);
/*@brief   设置数字低通滤波*/
unsigned char ICM_Set_LPF(unsigned short lpf);
/*@brief    设置采样率*/
unsigned char ICM_Set_Rate(unsigned short rate);
/*@brief    获取温度值*/
short ICM_Get_Temperature(void);
/*@brief    获取陀螺仪值*/
unsigned char ICM_Get_Gyroscope(short *gx, short *gy, short *gz);
/*brief    获取加速度值*/
unsigned char ICM_Get_Accelerometer(short *ax, short *ay, short *az);
/*@brief    获取 加速度值 角速度值*/
unsigned char ICM_Get_Raw_data(short *ax, short *ay, short *az, short *gx, short *gy, short *gz);
/*@brief    SPI 连续读*/
unsigned char ICM_Read_Len(unsigned char reg, unsigned char len, unsigned char *buf);
/*@brief    SPI 写一个寄存器*/
unsigned char ICM_Write_Byte(unsigned char reg, unsigned char value);
/* @brief    SPI 读一个寄存器*/
unsigned char ICM_Read_Byte(unsigned char reg);

void Gyro_Get_Mag_data(short *magx,short *magy,short *magz);
uint8_t Gyro_LSM6DSR_LQ9AGMV_Check(void);
uint8_t SPI_Mag_Read_Reg(uint8_t addr);
uint8_t SPI_Mag_Write_Reg(uint8_t addr, uint8_t data);

#endif
