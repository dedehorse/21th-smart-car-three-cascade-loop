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

#ifndef _LQ_MPU6050_DMP_
#define _LQ_MPU6050_DMP_

extern short gyro[3], accel[3], sensors;
extern float Pitch, Roll, Yaw;

unsigned short inv_row_2_scale(const signed char *row);

int LQ_DMP_Init(void);
int LQ_DMP_Read(void);
void Test_LQ6050_DMP(void);

extern int LQ_I2C_Write(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *dat);
extern int LQ_I2C_Read(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *buf);
// extern void delayms(u16 nms);
// extern void delayus(u16 nus);

// 初始化使用
int mpu_set_gyro_fsr(unsigned short fsr);
int mpu_set_accel_fsr(unsigned char fsr);
int mpu_set_lpf(unsigned short lpf);
int mpu_set_sample_rate(unsigned short rate);
int dmp_set_orientation(unsigned short orient);
int mpu_write_mem(unsigned short mem_addr, unsigned short length, unsigned char *lqda);
int mpu_read_mem(unsigned short mem_addr, unsigned short length, unsigned char *lqda);
int mpu_reset_fifo_in_dmp_enable_feature(void);
int mpu_reset_fifo_in_mpu_set_dmp_state(void);

// 读值使用
int dmp_read_fifo(short *gyro, short *accel, long *quat, short *sensors, unsigned char *more);

#endif
