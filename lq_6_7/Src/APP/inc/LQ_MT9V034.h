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

#ifndef __LQ_MT9V034_H_
#define __LQ_MT9V034_H_

#define MT9V034_IMAGEH 120 /*!< 行 HEIGHT 待采集摄像头图像高度行数 */
#define MT9V034_IMAGEW 188 /*!< 列 WIDTH  待采集摄像头图像宽度列数 */

// 几个模式选择，用户可以根据使用环境自行调节
// #define LQREFERENCEMODE0
#define LQREFERENCEMODE1
// #define LQREFERENCEMODE2
// #define LQREFERENCEMODE3

// 低对比度，
// 自动曝光，
// 曝光时间适当减小
// 效果一般
#ifdef LQREFERENCEMODE0
// 对比度 高对比度0x03c7  低对比度0x01c7  注意 高对比度会使图像变暗
#define CAMERA_CONTRAST 0x01c7 // 龙邱默认0x01c7

// 自动曝光开关 默认打开  设置为0 关闭自动曝光
#define CAMERA_AUTO_EXPOSURE 1 // 龙邱默认1

// 自动曝光模式下 亮度 调节范围 1-64
#define CAMERA_AUTO_EXPOSURE_BRIGHTNESS 50 // 龙邱默认50

// 可以在自动曝光的情况下设置固定帧率
// 自动曝光模式需要设置曝光时间上限 调节范围 1–32765
// 注意 当帧率过高时 如果设置的曝光时间过长 帧率可能会自适应下调
// 例如使用100帧时 最大曝光时间超过317 会导致帧率下降
// 曝光时间越长 图像越亮
#define CAMERA_MAX_EXPOSURE_TIME 300 // 龙邱默认300
#define CAMERA_MIN_EXPOSURE_TIME 1

// 非自动曝光模式下 可以调节曝光时间来调节图像整体亮度 调节范围 0–32765
// 注意 当帧率过高时 如果设置的曝光时间过长 帧率可能会自适应下调
// 曝光时间越长 图像越亮
#define CAMERA_EXPOSURE_TIME 160 // 龙邱默认160

// 低对比度，
// 手动曝光，
// 曝光时间适当加长
// 效果良好：
#elif defined LQREFERENCEMODE1
#define CAMERA_CONTRAST 0x01c7             // 龙邱默认0x01c7，对比度 高对比度0x03c7  低对比度0x01c7  注意 高对比度会使图像变暗
#define CAMERA_AUTO_EXPOSURE 0             // 龙邱默认1，自动曝光开关 默认打开  设置为0 关闭自动曝光
#define CAMERA_AUTO_EXPOSURE_BRIGHTNESS 40 // 龙邱默认50，自动曝光模式下 亮度 调节范围 1-64
#define CAMERA_MAX_EXPOSURE_TIME 60       // 龙邱默认300， 100
#define CAMERA_MIN_EXPOSURE_TIME 20        // 龙邱默认1，   50
#define CAMERA_EXPOSURE_TIME 40            // 龙邱默认160， 80

// 高对比度，
// 手动曝光，
// 曝光时间适当加长
// 效果：太暗
#elif defined LQREFERENCEMODE2
#define CAMERA_CONTRAST 0x03c7             // 龙邱默认0x03c7
#define CAMERA_AUTO_EXPOSURE 0             // 龙邱默认1
#define CAMERA_AUTO_EXPOSURE_BRIGHTNESS 50 // 龙邱默认50
#define CAMERA_MAX_EXPOSURE_TIME 300       // 龙邱默认300
#define CAMERA_MIN_EXPOSURE_TIME 1         // 龙邱默认1
#define CAMERA_EXPOSURE_TIME 80            // 龙邱默认50

// 高对比度，
// 手动曝光，
// 曝光时间适当加长
// 效果：良好。
#elif defined LQREFERENCEMODE3
#define CAMERA_CONTRAST 0x03c7             // 龙邱默认0x03c7
#define CAMERA_AUTO_EXPOSURE 0             // 龙邱默认1
#define CAMERA_AUTO_EXPOSURE_BRIGHTNESS 50 // 龙邱默认50
#define CAMERA_MAX_EXPOSURE_TIME 480       // 龙邱默认300
#define CAMERA_MIN_EXPOSURE_TIME 1         // 龙邱默认1
#define CAMERA_EXPOSURE_TIME 480           // 龙邱默认50
#endif

/* 摄像头使用SCCB通信 SCCB和IIC基本类似 */
//#define MT9V034_SCL_PIN P11_2 /*!< SCCB SCL 管脚 */
//#define MT9V034_SDA_PIN P11_3 /*!< SCCB SDA 管脚 */
 #define MT9V034_SCL_PIN   P11_12   /*!< SCCB SCL 管脚 */
 #define MT9V034_SDA_PIN   P11_11   /*!< SCCB SDA 管脚 */

#define MT9V034_SCL_Out PIN_Dir(MT9V034_SCL_PIN, 1);    // 配置输出作为SCL_Out
#define MT9V034_SDA_Out PIN_Dir(MT9V034_SDA_PIN, 1);    // 配置作为输出作为SDA_Out
#define MT9V034_SDA_In PIN_Dir(MT9V034_SDA_PIN, 0);     // 配置作为输入作为SDA_In
#define MT9V034_SCL_High PIN_Write(MT9V034_SCL_PIN, 1); // 配置输出高电平
#define MT9V034_SCL_Low PIN_Write(MT9V034_SCL_PIN, 0);  // 配置输出低电平
#define MT9V034_SDA_High PIN_Write(MT9V034_SDA_PIN, 1); // 配置输出高电平
#define MT9V034_SDA_Low PIN_Write(MT9V034_SDA_PIN, 0);  // 配置输出低电平
#define MT9V034_SDA_Data PIN_Read(MT9V034_SDA_PIN)      // 读取引脚上的引脚状态

/*  MT9V034寄存器初始化*/
void MT9V034_Init(unsigned char fps);
/*帧率设置*/
void MT9V034_SetFrameRate(unsigned char fps);
/* 设置图像窗口大小 */
static void MT9V034_SetFrameResolution(unsigned short height, unsigned short width);
/* 设置自动曝光时间  */
void MT9V034_SetAutoExposure(char enable);
/*********************************************************************
 *摄像头SCCB底层驱动
 *内部调用
 ***********************************************************************/
void SCCB_Init(void);
void SCCB_Wait(void);
void SCCB_Stop(void);
void SCCB_Star(void);
unsigned char SCCB_SendByte(unsigned char Data);
void SCCB_RegWrite(unsigned char Device, unsigned char Address, unsigned short Data);
unsigned char SCCB_RegRead(unsigned char Device, unsigned char Address, unsigned short *Data);
void MTV_IICWriteReg16(unsigned char reg, unsigned short val);

#define MT9V034_I2C_ADDR 0xB8 //(0xB8 >> 1)=0x5C
#define MAX_IMAGE_HEIGHT 480
#define MAX_IMAGE_WIDTH 752
#define MT9V034_PIXEL_ARRAY_HEIGHT 492
#define MT9V034_PIXEL_ARRAY_WIDTH 782
#define MT9V034_CHIP_VERSION 0x00
#define MT9V034_CHIP_ID 0x1324

#define MT9V034_COLUMN_START 0x01
#define MT9V034_COLUMN_START_MIN 1
#define MT9V034_COLUMN_START_DEF 1
#define MT9V034_COLUMN_START_MAX 752

#define MT9V034_ROW_START 0x02
#define MT9V034_ROW_START_MIN 4
#define MT9V034_ROW_START_DEF 4
#define MT9V034_ROW_START_MAX 482

#define MT9V034_WINDOW_HEIGHT 0x03 //0x03
#define MT9V034_WINDOW_HEIGHT_MIN 1
#define MT9V034_WINDOW_HEIGHT_DEF 64
#define MT9V034_WINDOW_HEIGHT_MAX 480

#define MT9V034_WINDOW_WIDTH 0x04 //0x04
#define MT9V034_WINDOW_WIDTH_MIN 1
#define MT9V034_WINDOW_WIDTH_DEF 64
#define MT9V034_WINDOW_WIDTH_MAX 752

#define MINIMUM_HORIZONTAL_BLANKING 91 // see datasheet

#define MT9V034_HORIZONTAL_BLANKING 0x05
#define MT9V034_HORIZONTAL_BLANKING_MIN 43
#define MT9V034_HORIZONTAL_BLANKING_MAX 1023

#define MT9V034_VERTICAL_BLANKING 0x06
#define MT9V034_VERTICAL_BLANKING_MIN 4
#define MT9V034_VERTICAL_BLANKING_MAX 3000

#define MT9V034_CHIP_CONTROL 0x07
#define MT9V034_CHIP_CONTROL_MASTER_MODE (1 << 3)
#define MT9V034_CHIP_CONTROL_DOUT_ENABLE (1 << 7)
#define MT9V034_CHIP_CONTROL_SEQUENTIAL (1 << 8)

#define MT9V034_SHUTTER_WIDTH1 0x08
#define MT9V034_SHUTTER_WIDTH2 0x09
#define MT9V034_SHUTTER_WIDTH_CONTROL 0x0A
#define MT9V034_TOTAL_SHUTTER_WIDTH 0x0B
#define MT9V034_TOTAL_SHUTTER_WIDTH_MIN 1
#define MT9V034_TOTAL_SHUTTER_WIDTH_DEF 480
#define MT9V034_TOTAL_SHUTTER_WIDTH_MAX 32767

#define MT9V034_RESET 0x0C

#define MT9V034_READ_MODE 0x0D
#define MT9V034_READ_MODE_ROW_BIN_MASK (3 << 0)
#define MT9V034_READ_MODE_ROW_BIN_SHIFT 0
#define MT9V034_READ_MODE_COLUMN_BIN_MASK (3 << 2)
#define MT9V034_READ_MODE_COLUMN_BIN_SHIFT 2
#define MT9V034_READ_MODE_ROW_BIN_2 (1 << 0)
#define MT9V034_READ_MODE_ROW_BIN_4 (1 << 1)
#define MT9V034_READ_MODE_COL_BIN_2 (1 << 2)
#define MT9V034_READ_MODE_COL_BIN_4 (1 << 3)
#define MT9V034_READ_MODE_ROW_FLIP (1 << 4)
#define MT9V034_READ_MODE_COLUMN_FLIP (1 << 5)
#define MT9V034_READ_MODE_DARK_COLUMNS (1 << 6)
#define MT9V034_READ_MODE_DARK_ROWS (1 << 7)

#define MT9V034_PIXEL_OPERATION_MODE 0x0F
#define MT9V034_PIXEL_OPERATION_MODE_COLOR (1 << 2)
#define MT9V034_PIXEL_OPERATION_MODE_HDR (1 << 6)

#define MT9V034_V1_CTRL_REG_A 0x31
#define MT9V034_V2_CTRL_REG_A 0x32
#define MT9V034_V3_CTRL_REG_A 0x33
#define MT9V034_V4_CTRL_REG_A 0x34

#define MT9V034_ANALOG_GAIN 0x35
#define MT9V034_ANALOG_GAIN_MIN 16
#define MT9V034_ANALOG_GAIN_DEF 16
#define MT9V034_ANALOG_GAIN_MAX 64

#define MT9V034_MAX_ANALOG_GAIN 0x36
#define MT9V034_MAX_ANALOG_GAIN_MAX 127

#define MT9V034_FRAME_DARK_AVERAGE 0x42
#define MT9V034_DARK_AVG_THRESH 0x46
#define MT9V034_DARK_AVG_LOW_THRESH_MASK (255 << 0)
#define MT9V034_DARK_AVG_LOW_THRESH_SHIFT 0
#define MT9V034_DARK_AVG_HIGH_THRESH_MASK (255 << 8)
#define MT9V034_DARK_AVG_HIGH_THRESH_SHIFT 8

#define MT9V034_ROW_NOISE_CORR_CONTROL 0x70
#define MT9V034_ROW_NOISE_CORR_ENABLE (1 << 5)
#define MT9V034_ROW_NOISE_CORR_USE_BLK_AVG (1 << 7)

#define MT9V034_PIXEL_CLOCK 0x74
#define MT9V034_PIXEL_CLOCK_INV_LINE (1 << 0)
#define MT9V034_PIXEL_CLOCK_INV_FRAME (1 << 1)
#define MT9V034_PIXEL_CLOCK_XOR_LINE (1 << 2)
#define MT9V034_PIXEL_CLOCK_CONT_LINE (1 << 3)
#define MT9V034_PIXEL_CLOCK_INV_PXL_CLK (1 << 4)

#define MT9V034_TEST_PATTERN 0x7f
#define MT9V034_TEST_PATTERN_DATA_MASK (1023 << 0)
#define MT9V034_TEST_PATTERN_DATA_SHIFT 0
#define MT9V034_TEST_PATTERN_USE_DATA (1 << 10)
#define MT9V034_TEST_PATTERN_GRAY_MASK (3 << 11)
#define MT9V034_TEST_PATTERN_GRAY_NONE (0 << 11)
#define MT9V034_TEST_PATTERN_GRAY_VERTICAL (1 << 11)
#define MT9V034_TEST_PATTERN_GRAY_HORIZONTAL (2 << 11)
#define MT9V034_TEST_PATTERN_GRAY_DIAGONAL (3 << 11)
#define MT9V034_TEST_PATTERN_ENABLE (1 << 13)
#define MT9V034_TEST_PATTERN_FLIP (1 << 14)

#define MT9V034_AEC_AGC_ENABLE 0xAF
#define MT9V034_AEC_ENABLE (1 << 0)
#define MT9V034_AGC_ENABLE (1 << 1)
#define MT9V034_THERMAL_INFO 0xc1
#define MT9V034_ANALOG_CTRL (0xC2)
#define MT9V034_ANTI_ECLIPSE_ENABLE (1 << 7)
#define MT9V034_MAX_GAIN (0xAB)
#define MT9V034_FINE_SHUTTER_WIDTH_TOTAL_A (0xD5)
#define MT9V034_HDR_ENABLE_REG 0x0F
#define MT9V034_ADC_RES_CTRL_REG 0x1C
#define MT9V034_ROW_NOISE_CORR_CTRL_REG 0x70
#define MT9V034_TEST_PATTERN_REG 0x7F
#define MT9V034_TILED_DIGITAL_GAIN_REG 0x80
#define MT9V034_AGC_AEC_DESIRED_BIN_REG 0xA5
#define MT9V034_MAX_GAIN_REG 0xAB
#define MT9V034_MIN_EXPOSURE_REG 0xAC // datasheet min coarse shutter width
#define MT9V034_MAX_EXPOSURE_REG 0xAD // datasheet max coarse shutter width
#define MT9V034_AEC_AGC_ENABLE_REG 0xAF
#define MT9V034_AGC_AEC_PIXEL_COUNT_REG 0xB0

#endif
