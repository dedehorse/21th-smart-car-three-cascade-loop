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

#ifndef __LQ_IIC_GYRO_H__
#define __LQ_IIC_GYRO_H__
#define uint16 uint16_t

extern signed short aac_x, aac_y, aac_z;    // 加速度传感器原始数据
extern signed short gyro_x, gyro_y, gyro_z; // 陀螺仪原始数据



// AK8963的内部寄存器 9250
#define MPU_MAG_WIA 0X00 // AK8963的器件ID寄存器地址
#define AK8963_ADDR 0x0C // AK8963的I2C地址
#define AK8963_ID 0x48   // AK8963的器件ID

#define MPU_MAG_XOUTL_REG 0X03 // 磁力计值,X轴低8位寄存器
#define MPU_MAG_XOUTH_REG 0X04 // 磁力计值,X轴高8位寄存器
#define MPU_MAG_YOUTL_REG 0X05 // 磁力计值,Y轴低8位寄存器
#define MPU_MAG_YOUTH_REG 0X06 // 磁力计值,Y轴高8位寄存器
#define MPU_MAG_ZOUTL_REG 0X07 // 磁力计值,Z轴低8位寄存器
#define MPU_MAG_ZOUTH_REG 0X08 // 磁力计值,Z轴高8位寄存器
#define MPU_MAG_ST1_REG 0X02   // 磁力计读取状态
#define MPU_MAG_ST2_REG 0X09   // 磁力计读取状态
#define MPU_MAG_CNTL1_REG 0X0A // 磁力计模式控制
#define MPU_MAG_CNTL2_REG 0X0B // 磁力计模式控制

//****************************************
// 定义MPU6050内部地址
//****************************************
// MPU6500的内部寄存器
#define MPU_SELF_TESTX_REG 0X0D  // 自检寄存器X
#define MPU_SELF_TESTY_REG 0X0E  // 自检寄存器Y
#define MPU_SELF_TESTZ_REG 0X0F  // 自检寄存器Z
#define MPU_SELF_TESTA_REG 0X10  // 自检寄存器A
#define MPU_SAMPLE_RATE_REG 0X19 // 采样频率分频器
#define MPU_CFG_REG 0X1A         // 配置寄存器
#define MPU_GYRO_CFG_REG 0X1B    // 陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG 0X1C   // 加速度计配置寄存器
#define MPU_MOTION_DET_REG 0X1F  // 运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG 0X23     // FIFO使能寄存器

#define MPU_I2CMST_STA_REG 0X36 // IIC主机状态寄存器
#define MPU_INTBP_CFG_REG 0X37  // 中断/旁路设置寄存器
#define MPU_INT_EN_REG 0X38     // 中断使能寄存器
#define MPU_INT_STA_REG 0X3A    // 中断状态寄存器

#define MPU_ACCEL_XOUTH_REG 0X3B // 加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG 0X3C // 加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG 0X3D // 加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG 0X3E // 加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG 0X3F // 加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG 0X40 // 加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH_REG 0X41 // 温度值高八位寄存器
#define MPU_TEMP_OUTL_REG 0X42 // 温度值低8位寄存器

#define MPU_GYRO_XOUTH_REG 0X43 // 陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG 0X44 // 陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG 0X45 // 陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG 0X46 // 陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG 0X47 // 陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG 0X48 // 陀螺仪值,Z轴低8位寄存器

#define MPU_I2CSLV0_DO_REG 0X63 // IIC从机0数据寄存器
#define MPU_I2CSLV1_DO_REG 0X64 // IIC从机1数据寄存器
#define MPU_I2CSLV2_DO_REG 0X65 // IIC从机2数据寄存器
#define MPU_I2CSLV3_DO_REG 0X66 // IIC从机3数据寄存器

#define MPU_I2CMST_DELAY_REG 0X67 // IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG 0X68  // 信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG 0X69 // 运动检测控制寄存器
#define MPU_USER_CTRL_REG 0X6A    // 用户控制寄存器
#define MPU_PWR_MGMT1_REG 0X6B    // 电源管理寄存器1
#define MPU_PWR_MGMT2_REG 0X6C    // 电源管理寄存器2
#define MPU_FIFO_CNTH_REG 0X72    // FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG 0X73    // FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG 0X74      // FIFO读写寄存器
#define WHO_AM_I 0X75             // 器件ID寄存器

//****************42605寄存器**********************************************************************************

#define ICM42605_TEMP_OUTH_REG 0X1D // 温度值高八位寄存器
#define ICM42605_TEMP_OUTL_REG 0X1E // 温度值低8位寄存器

#define ICM42605_ACCEL_XOUTH_REG 0X1F // 加速度值,X轴高8位寄存器
#define ICM42605_ACCEL_XOUTL_REG 0X20 // 加速度值,X轴低8位寄存器
#define ICM42605_ACCEL_YOUTH_REG 0X21 // 加速度值,Y轴高8位寄存器
#define ICM42605_ACCEL_YOUTL_REG 0X22 // 加速度值,Y轴低8位寄存器
#define ICM42605_ACCEL_ZOUTH_REG 0X23 // 加速度值,Z轴高8位寄存器
#define ICM42605_ACCEL_ZOUTL_REG 0X24 // 加速度值,Z轴低8位寄存器

#define ICM42605_GYRO_XOUTH_REG 0X25 // 陀螺仪值,X轴高8位寄存器
#define ICM42605_GYRO_XOUTL_REG 0X26 // 陀螺仪值,X轴低8位寄存器
#define ICM42605_GYRO_YOUTH_REG 0X27 // 陀螺仪值,Y轴高8位寄存器
#define ICM42605_GYRO_YOUTL_REG 0X28 // 陀螺仪值,Y轴低8位寄存器
#define ICM42605_GYRO_ZOUTH_REG 0X29 // 陀螺仪值,Z轴高8位寄存器
#define ICM42605_GYRO_ZOUTL_REG 0X2A // 陀螺仪值,Z轴低8位寄存器

#define reg_bank_sel 0x76
#define device_config_reg 0x11
#define bit_spi_mode 0x10
#define bit_soft_reset_chip_config 0x01

#define intf_config4 0x7A
#define pwr_mgmt0_reg 0x4E
// #define bit_temp_dis               0x20
#define bit_idle 0x10
#define bit_gyro_mode_mask ((0x03) << 2)
#define bit_gyro_mode_off ((0x00) << 2)
#define bit_gyro_mode_standby ((0x01) << 2)
// #define bit_gyro_mode_lp           ((0x02)<<2)
#define bit_gyro_mode_ln ((0x03) << 2)
#define bit_accel_mode_mask ((0x03) << 0)
#define bit_accel_mode_off 0x00
#define bit_accel_mode_lp 0x02
#define bit_accel_mode_ln 0x03

#define gyro_config0_reg 0x4F
#define bit_gyro_ui_fs_sel_shift 5
#define bit_gyro_ui_fs_sel_2000dps ((0x00) << bit_gyro_ui_fs_sel_shift)
#define bit_gyro_ui_fs_sel_1000dps ((0x01) << bit_gyro_ui_fs_sel_shift)
#define bit_gyro_ui_fs_sel_mask ((0x07) << bit_gyro_ui_fs_sel_shift)
#define bit_gyro_odr_100hz ((0x08) << 0)
#define bit_gyro_odr_50hz ((0x09) << 0)
#define bit_gyro_odr_nonflame_mask ((0x0F) << 0)

#define accel_config0_reg 0x50
#define bit_accel_ui_fs_sel_shift 5
#define bit_accel_ui_fs_sel_8g ((0x01) << bit_accel_ui_fs_sel_shift)
#define bit_accel_ui_fs_sel_mask ((0x07) << bit_accel_ui_fs_sel_shift)
#define bit_accel_odr_100hz ((0x08) << 0)
#define bit_accel_odr_50hz ((0x09) << 0)
#define bit_accel_odr_nonflame_mask ((0x0F) << 0)

#define int_source0_reg 0x65
#define bit_int_ui_fsync_int1_en 0x40
#define bit_int_pll_rdy_int1_en 0x20
#define bit_int_reset_done_int1_en 0x10
#define bit_int_ui_drdy_int1_en 0x08
#define bit_int_fifo_ths_int1_en 0x04 // FIFO threshold interrupt
#define bit_int_fifo_full_int1_en 0x02
#define bit_int_ui_agc_rdy_int1_en 0x01

#define sensor_selftest_reg 0x6B
#define bit_accel_st_result 0x08
#define bit_gyro_st_result 0x04
#define bit_accel_st_status 0x02
#define bit_gyro_st_status 0x01

#define int_config_reg 0x14
#define bit_int2_mode 0x20
#define bit_int2_drive_circuit 0x10
#define bit_int2_polarity 0x08
#define bit_int1_mode 0x04
#define bit_int1_drive_circuit 0x02
#define bit_int1_polarity 0x01

#define fifo_config_reg 0x16
#define bit_fifo_mode_ctrl_mask ((0x03) << 6)
#define bit_fifo_mode_ctrl_bypass ((0x00) << 6)
#define bit_fifo_mode_ctrl_stream ((0x01) << 6)
#define bit_fifo_mode_ctrl_snapshot ((0x02) << 6)

#define tmst_config_reg 0x54
#define bit_fifo_ram_iso_ena 0x40
#define bit_en_dreg_fifo_d2a 0x20
#define bit_tmst_to_regs_en 0x10
#define bit_tmst_resol 0x08
#define bit_tmst_delta_en 0x04
#define bit_tmst_fsync_en 0x02
#define bit_tmst_en 0x01

#define fifo_config2_reg 0x60
#define fifo_config3_reg 0x61

#define fsync_config_reg 0x62
#define bit_fsync_ui_sel_mask ((0x07) << 4)
#define bit_fsync_ui_sel_tag_temp ((0x01) << 4)
#define bit_fsync_ui_flag_clear_sel 0x02

#define fifo_config1_reg 0x5F
#define bit_fifo_resume_partial 0x40
#define bit_fifo_wm_gt_th 0x20
#define bit_fifo_hires_en 0x10
#define bit_fifo_tmst_fsync_en 0x08
#define bit_fifo_temp_en 0x04
#define bit_fifo_gyro_en 0x02
#define bit_fifo_accel_en 0x01

#define int_config0_reg 0x63
#define int_config1_reg 0x64
#define bit_int_asy_rst_disable 0x10

#define fifo_byte_count_h_res 0x2E
#define fifo_byte_count_l_res 0x2F

#define fifo_accel_en 0x40
#define fifo_gyro_en 0x20

#define fifo_data_port 0x30

//****************42605寄存器**********************************************************************************

unsigned char MPU9250_Get_Magnetometer(short *mx, short *my, short *mz); // 获取9250磁力计的原始值
/**
 * @brief    读取MPU6050或者ICM20602原始数据 测试
 *
 * @param
 *
 * @return
 *
 * @note
 *
 * @see      Test_MPU6050();
 *
 * @date     2019/6/12 星期三
 */
void Test_IIC_Gyro(void);

/**
 * @brief    初始化MPU6050 或者 ICM20602
 *
 * @param    无
 *
 * @return   0：初始化成功   1：失败
 *
 * @note     使用前先初始化IIC接口
 *
 * @see      MPU6050_Init();
 *
 * @date     2019/6/12 星期三
 */
unsigned char IIC_Gyro_init(void);

/**
 * @brief    设置陀螺仪测量范围
 *
 * @param    fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
 *
 * @return   0 ：设置成功
 *
 * @note     无
 *
 * @see      MPU_Set_Gyro_Fsr(3);        //陀螺仪传感器,±2000dps
 *
 * @date     2019/6/12 星期三
 */
unsigned char MPU_Set_Gyro_Fsr(unsigned char fsr);

/**
 * @brief    设置加速度计测量范围
 *
 * @param    fsr:0,±2g;1,±4g;2,±8g;3,±16g
 *
 * @return   0：设置成功
 *
 * @note     无
 *
 * @see      MPU_Set_Accel_Fsr(1);       //加速度传感器,±4g
 *
 * @date     2019/6/12 星期三
 */
unsigned char MPU_Set_Accel_Fsr(unsigned char fsr);

/**
 * @brief    设置数字低通滤波
 *
 * @param    lpf:数字低通滤波频率(Hz)
 *
 * @return   0：设置成功
 *
 * @note     无
 *
 * @see      MPU_Set_LPF(100);
 *
 * @date     2019/6/12 星期三
 */
unsigned char MPU_Set_LPF(uint16 lpf);

/**
 * @brief    设置采样率
 *
 * @param    rate:4~1000(Hz)
 *
 * @return   0：设置成功
 *
 * @note     无
 *
 * @see      MPU_Set_Rate(1000);              //设置采样率1000Hz
 *
 * @date     2019/6/12 星期三
 */
unsigned char MPU_Set_Rate(uint16 rate);

/**
 * @brief    获取温度值
 *
 * @param    无
 *
 * @return   温度值(扩大了100倍)
 *
 * @note     无
 *
 * @see      signed short temp = MPU_Get_Temperature();
 *
 * @date     2019/6/12 星期三
 */
short MPU_Get_Temperature(void);

/**
 * @brief    获取陀螺仪值
 *
 * @param    gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
 *
 * @return   0：读取成功
 *
 * @note     无
 *
 * @see      signed short data[3];
 * @see      MPU_Get_Gyroscope(&data[0], &data[1], &data[2]);
 *
 * @date     2019/6/12 星期三
 */
unsigned char MPU_Get_Gyroscope(signed short *gx, signed short *gy,
                                signed short *gz);

/**
 * @brief    获取加速度值
 *
 * @param    ax,ay,az:陀螺仪x,y,z轴的原始读数(带符号)
 *
 * @return   0：读取成功
 *
 * @note     无
 *
 * @see      signed short data[3];
 * @see      MPU_Get_Accelerometer(&data[0], &data[1], &data[2]);
 *
 * @date     2019/6/12 星期三
 */
unsigned char MPU_Get_Accelerometer(signed short *ax, signed short *ay,
                                    signed short *az);

/**
 * @brief    获取 加速度值 角速度值
 *
 * @param    ax,ay,az:陀螺仪x,y,z轴的原始读数(带符号)
 * @param    gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
 *
 * @return   0：读取成功
 *
 * @note     无
 *
 * @see      signed short data[6];
 * @see      MPU_Get_Raw_data(&data[0], &data[1], &data[2],&data[3], &data[4], &data[5]);
 *
 * @date     2019/6/12 星期三
 */
unsigned char MPU_Get_Raw_data(signed short *ax, signed short *ay,
                               signed short *az, signed short *gx, signed short *gy, signed short *gz);

/**
 * @brief    IIC 连续读
 *
 * @param    addr:器件地址
 * @param    reg :要读取的寄存器地址
 * @param    len :要读取的长度
 * @param    buf :读取到的数据存储区
 *
 * @return   0 ：读取成功
 *
 * @note     底层驱动 移植时需要修改
 *
 * @see      unsigned char buf[14];
 * @see      MPU_Read_Len(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
 *
 * @date     2019/6/12 星期三
 */
unsigned char MPU_Read_Len(unsigned char addr, unsigned char reg,
                           unsigned char len, unsigned char *buf);

/**
 * @brief    IIC 连续写
 *
 * @param    addr:器件地址
 * @param    reg :要写入的寄存器地址
 * @param    len :要写入的长度
 * @param    buf :写入到的数据存储区
 *
 * @return   0 ：写入成功
 *
 * @note     底层驱动 移植时需要修改
 *
 * @see      unsigned char buf[14];
 * @see      MPU9250_Write_Len(MPU9250_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
 *
 * @date     2021/7/1 星期四
 */
unsigned char MPU_Write_Len(unsigned char addr, unsigned char reg,
                            unsigned char len, unsigned char *buf);

/**
 * @brief    IIC 写一个寄存器
 *
 * @param    addr  :器件地址
 * @param    reg   :寄存器地址
 * @param    value :要写入的值
 *
 * @return   0 ：读取成功
 *
 * @note     底层驱动 移植时需要修改
 *
 * @see      MPU_Write_Byte(MPU6050_ADDR,MPU_SAMPLE_RATE_REG,1);
 *
 * @date     2019/6/12 星期三
 */
unsigned char MPU_Write_Byte(unsigned char addr, unsigned char reg,
                             unsigned char value);

/**
 * @brief    IIC 读一个寄存器
 *
 * @param    addr  :器件地址
 * @param    reg   :寄存器地址
 *
 * @return   读取的值
 *
 * @note     底层驱动 移植时需要修改
 *
 * @see      MPU_Read_Byte(MPU6050_ADDR,WHO_AM_I);
 *
 * @date     2019/6/12 星期三
 */
unsigned char MPU_Read_Byte(unsigned char addr, unsigned char reg);
unsigned char Gyro_Chose(void);

#endif
