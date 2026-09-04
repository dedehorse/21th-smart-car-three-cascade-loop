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

#include "LQ_Font.h"
#include "lq_include.h"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void lq_tft_delayms(unsigned short ms)
@功能说明：简单延时函数
@参数说明：Del_1ms ：延时时间
@函数返回：无
@调用方法：lq_tft_delayms(1);
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
static void lq_tft_delayms(unsigned short ms)
{
    volatile unsigned long i = 0;
    while (ms--)
    {
        for (i = 0; i < 15000; ++i)
        {
            __asm("NOP"); /* delay */
        }
    }
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Init(unsigned short type)
@功能说明：TFT初始化
@参数说明：DIR： 0/1:横屏  2/3：竖屏
@函数返回：无
@调用方法：TFTSPI_Init(unsigned char DIR);
@备    注：初始化时调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Init(show_dir_t Dir)
{
    //-----端口初始化----//
    PIN_InitConfig(TFTSPI_CS, PIN_MODE_OUTPUT, 0);
    PIN_InitConfig(TFTSPI_DC, PIN_MODE_OUTPUT, 0);
    PIN_InitConfig(TFTSPI_RST, PIN_MODE_OUTPUT, 0);

#if(1 == TFTUSE_QSPI && 1 == USE_HARD_SPI)     /* 使用硬件QSPI */
        QSPI_InitConfig(QSPI0_CLK_P20_11, QSPI_MISO_NULL, QSPI0_MOSI_P20_12, QSPI0_CS_P15_0, 15000000, 0);
#elif(1 == USE_HARD_SPI) /* 使用硬件SPI */
        SPI_InitConfig(TFTSPI_SCLK, TFTSPI_MISO, TFTSPI_MOSI, TFTSPI_SLSO, 10000000);  // 默认模式0
#else                   /* 使用软件SPI */
    PIN_InitConfig(TFTSPI_SCK, PIN_MODE_OUTPUT, 0);
    PIN_InitConfig(TFTSPI_SDI, PIN_MODE_OUTPUT, 0);

#endif

    TFTSPI_RST_L;
    lq_tft_delayms(50);
    TFTSPI_RST_H;
    lq_tft_delayms(50);
    TFTSPI_Write_Cmd(0x11); // 关闭睡眠，振荡器工作
    lq_tft_delayms(10);
    TFTSPI_Write_Cmd(0x3a); // 每次传送16位数据(VIPF3-0=0101)，每个像素16位(IFPF2-0=101)
    TFTSPI_Write_Byte(0x55);
    TFTSPI_Write_Cmd(0x26);
    TFTSPI_Write_Byte(0x04);
    TFTSPI_Write_Cmd(0xf2); // Driver Output Control(1)
    TFTSPI_Write_Byte(0x01);
    TFTSPI_Write_Cmd(0xe0); // Driver Output Control(1)
    TFTSPI_Write_Byte(0x3f);
    TFTSPI_Write_Byte(0x25);
    TFTSPI_Write_Byte(0x1c);
    TFTSPI_Write_Byte(0x1e);
    TFTSPI_Write_Byte(0x20);
    TFTSPI_Write_Byte(0x12);
    TFTSPI_Write_Byte(0x2a);
    TFTSPI_Write_Byte(0x90);
    TFTSPI_Write_Byte(0x24);
    TFTSPI_Write_Byte(0x11);
    TFTSPI_Write_Byte(0x00);
    TFTSPI_Write_Byte(0x00);
    TFTSPI_Write_Byte(0x00);
    TFTSPI_Write_Byte(0x00);
    TFTSPI_Write_Byte(0x00);
    TFTSPI_Write_Cmd(0xe1); // Driver Output Control(1)
    TFTSPI_Write_Byte(0x20);
    TFTSPI_Write_Byte(0x20);
    TFTSPI_Write_Byte(0x20);
    TFTSPI_Write_Byte(0x20);
    TFTSPI_Write_Byte(0x05);
    TFTSPI_Write_Byte(0x00);
    TFTSPI_Write_Byte(0x15);
    TFTSPI_Write_Byte(0xa7);
    TFTSPI_Write_Byte(0x3d);
    TFTSPI_Write_Byte(0x18);
    TFTSPI_Write_Byte(0x25);
    TFTSPI_Write_Byte(0x2a);
    TFTSPI_Write_Byte(0x2b);
    TFTSPI_Write_Byte(0x2b);
    TFTSPI_Write_Byte(0x3a);
    TFTSPI_Write_Cmd(0xb1);  // 0xb1       //设置屏幕刷新频率
    TFTSPI_Write_Byte(0x00); // 0x08       //DIVA=8
    TFTSPI_Write_Byte(0x00); // 0x08      //VPA =8，约90Hz
    TFTSPI_Write_Cmd(0xb4);  // LCD Driveing control
    TFTSPI_Write_Byte(0x07); // NLA=1,NLB=1,NLC=1
    TFTSPI_Write_Cmd(0xc0);  // LCD Driveing control  Power_Control1
    TFTSPI_Write_Byte(0x0a);
    TFTSPI_Write_Byte(0x02);
    TFTSPI_Write_Cmd(0xc1); // LCD Driveing control
    TFTSPI_Write_Byte(0x02);
    TFTSPI_Write_Cmd(0xc5); // LCD Driveing control
    TFTSPI_Write_Byte(0x4f);
    TFTSPI_Write_Byte(0x5a);
    TFTSPI_Write_Cmd(0xc7); // LCD Driveing control
    TFTSPI_Write_Byte(0x40);
    TFTSPI_Write_Cmd(0x2a);  // 配置MCU可操作的LCD内部RAM横坐标起始、结束参数
    TFTSPI_Write_Byte(0x00); // 横坐标起始地址0x0000
    TFTSPI_Write_Byte(0x00);
    TFTSPI_Write_Byte(0x00); // 横坐标结束地址0x007f(127)
    TFTSPI_Write_Byte(0xa8); // 7f
    TFTSPI_Write_Cmd(0x2b);  // 配置MCU可操作的LCD内部RAM纵坐标起始结束参数
    TFTSPI_Write_Byte(0x00); // 纵坐标起始地址0x0000
    TFTSPI_Write_Byte(0x00);
    TFTSPI_Write_Byte(0x00); // 纵坐标结束地址0x009f(159)
    TFTSPI_Write_Byte(0xb3); // 9f
    TFTSPI_Write_Cmd(0x36);  // 配置MPU和DDRAM对应关系
    switch (Dir)
    {
    case Screen_Dir0:
        TFTSPI_Write_Byte(0xc0);
        break; // 竖屏模式
    case Screen_Dir1:
        TFTSPI_Write_Byte(0x00);
        break; // 竖屏模式旋转180°
    case Screen_Dir2:
        TFTSPI_Write_Byte(0xa0);
        break; // 横屏模式
    case Screen_Dir3:
        TFTSPI_Write_Byte(0x60);
        break; // 横屏模式旋转180°
    }

    TFTSPI_Write_Cmd(0xb7);  // LCD Driveing control
    TFTSPI_Write_Byte(0x00); // CRL=0
    TFTSPI_Write_Cmd(0x29);  // 开启屏幕显示
    TFTSPI_Write_Cmd(0x2c);  // 设置为LCD接收数据/命令模式
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void tft18delay_1us(unsigned int Del)
@功能说明：简单延时函数
@参数说明：Del ：延时时间
@函数返回：无
@调用方法：tft18delay_1us(1);
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void tft18delay_1us(unsigned int Del) //
{
    while (Del--)
    {
        __asm("NOP");
    }
}

#if USE_HARD_SPI
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Write_Cmd(unsigned short cmd)
@功能说明：写命令
@参数说明：cmd ：命令
@函数返回：无
@调用方法：TFTSPI_Write_Cmd(0xb7); //LCD Driveing control
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Write_Cmd(unsigned char cmd)
{

    TFTSPI_DC_L; // A0=0 ILI9163_A0=0;  //A0=0发送命令
#if(1==TFTUSE_QSPI && 1 == USE_HARD_SPI)     /* 使用硬件QSPI */
      QSPI_ReadWriteNByte(TFTQSPI_PORT, &cmd, &cmd, 1);
#elif(1 == USE_HARD_SPI)
    SPI_ReadWriteNByte(TFTSPI_PORT, &cmd, &cmd, 1);
#endif
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Write_Byte(unsigned short dat)
@功能说明：写字节
@参数说明：dat ：数据
@函数返回：无
@调用方法：TFTSPI_Write_Byte(0x00);  //CRL=0
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Write_Byte(unsigned char dat)
{

    TFTSPI_DC_H; // A0=1  ILI9163_A0=1;          //A0=1发送数据
#if(1==TFTUSE_QSPI && 1 == USE_HARD_SPI)     /* 使用硬件QSPI */
    QSPI_ReadWriteNByte(TFT_QSPI_PORT, &dat, &dat, 1);   // QSPI读取
#elif(1 == USE_HARD_SPI)
    SPI_ReadWriteNByte(TFTSPI_PORT, &dat, &dat, 1);
#endif
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Write_Word(unsigned short dat)
@功能说明：写半字
@参数说明：dat ：数据
@函数返回：无
@调用方法：TFTSPI_Write_Word(0xFFFF);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Write_Word(unsigned short dat)
{

    TFTSPI_DC_H; // A0=1  ILI9163_A0=1;

    uint8 buff[2];
    buff[0] = dat >> 8;
    buff[1] = (uint8)dat;

#if(1==TFTUSE_QSPI && 1 == USE_HARD_SPI)     /* 使用硬件QSPI */
    QSPI_ReadWriteNByte(TFT_QSPI_PORT, buff , buff, 2);
#elif(1 == USE_HARD_SPI)
    SPI_ReadWriteNByte(TFTSPI_PORT, buff, buff, 2);
#endif
}

#else
// ==========================   软件SPI 操作屏幕相关函数 ============================
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Write_Cmd(unsigned short cmd)
@功能说明：写命令
@参数说明：cmd ：命令
@函数返回：无
@调用方法：TFTSPI_Write_Cmd(0xb7); //LCD Driveing control
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Write_Cmd(unsigned char cmd)
{
    unsigned char i;

    TFTSPI_DC_L; // A0=0 ILI9163_A0=0;  //A0=0发送命令

    for (i = 0; i < 8; i++)
    {
        TFTSPI_SCK_L;
        tft18delay_1us(1); // SCK=0  ILI9163_SCK=0;
        if (cmd & 0x80)
        {
            TFTSPI_SDI_H;
        } // SDI=1
        else
        {
            TFTSPI_SDI_L;
        } // SDI=0
        TFTSPI_SCK_H;
        tft18delay_1us(1); // SCK=1  ILI9163_SCK = 1;
        cmd = (cmd << 1);
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Write_Byte(unsigned short dat)
@功能说明：写字节
@参数说明：dat ：数据
@函数返回：无
@调用方法：TFTSPI_Write_Byte(0x00);  //CRL=0
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Write_Byte(unsigned char dat)
{
    unsigned char i;

    TFTSPI_DC_H; // A0=1  ILI9163_A0=1;          //A0=1发送数据

    for (i = 0; i < 8; i++)
    {
        TFTSPI_SCK_L;
        tft18delay_1us(1); // SCK=0  ILI9163_SCK=0;
        if (dat & 0x80)
        {
            TFTSPI_SDI_H;
        } // SDI=1
        else
        {
            TFTSPI_SDI_L;
        } // SDI=0
        TFTSPI_SCK_H;
        tft18delay_1us(1); // SCK=1 ILI9163_SCK = 1;
        dat = (dat << 1);
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Write_Word(unsigned short dat)
@功能说明：写半字
@参数说明：dat ：数据
@函数返回：无
@调用方法：TFTSPI_Write_Word(0xFFFF);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Write_Word(unsigned short dat)
{
    unsigned char i;

    TFTSPI_DC_H; // A0=1  ILI9163_A0=1;

    for (i = 0; i < 16; i++)
    {
        TFTSPI_SCK_L;
        tft18delay_1us(1); // SCK=0  ILI9163_SCK=0;
        if (dat & 0x8000)
        {
            TFTSPI_SDI_H;
        } // SDI=1
        else
        {
            TFTSPI_SDI_L;
        } // SDI=0
        TFTSPI_SCK_H;
        tft18delay_1us(1); // SCK=1  ILI9163_SCK=1;
        dat <<= 1;
    }
}

#endif  // Sofit SPI

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Set_Pos(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye)
@功能说明：重新定位输入信息位置
@参数说明：xs:起始x ys:起始y xe:结束x ys:结束y
@函数返回：
@调用方法：TFTSPI_Set_Pos(10, 20, 30, 40);
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Set_Pos(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye)
{
    TFTSPI_Write_Cmd(0x2A);  // Colulm addRSTs set
    TFTSPI_Write_Byte(0x00); // 行起始坐标高8位，始终为零
    TFTSPI_Write_Byte(xs);   // 行起始坐标低8位
    TFTSPI_Write_Byte(0x00); // 行终止坐标高8位，始终为零
    TFTSPI_Write_Byte(xe);   // 行终止坐标低8位
    TFTSPI_Write_Cmd(0x2B);  // Colulm addRSTs set
    TFTSPI_Write_Byte(0x00); // 列起始坐标高8位，始终为零
    TFTSPI_Write_Byte(ys);   // 列起始坐标低8位
    TFTSPI_Write_Byte(0x00); // 列终止坐标高8位，始终为零
    TFTSPI_Write_Byte(ye);   // 列终止坐标低8位
    TFTSPI_Write_Cmd(0x2C);  // GRAM接收MCU数据或命令
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Addr_Rst(void)
@功能说明：重置地址
@参数说明：无
@函数返回：无
@调用方法：TFTSPI_Addr_Rst();
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Addr_Rst(void)
{
    TFTSPI_Write_Cmd(0x2a);  // 配置MCU可操作的LCD内部RAM横坐标起始、结束参数
    TFTSPI_Write_Byte(0x00); // 横坐标起始地址0x0000
    TFTSPI_Write_Byte(0x00);
    TFTSPI_Write_Byte(0x00); // 横坐标结束地址0x007f(127)
    TFTSPI_Write_Byte(0xa8); // 7f
    TFTSPI_Write_Cmd(0x2b);  // 配置MCU可操作的LCD内部RAM纵坐标起始结束参数
    TFTSPI_Write_Byte(0x00); // 纵坐标起始地址0x0000
    TFTSPI_Write_Byte(0x00);
    TFTSPI_Write_Byte(0x00); // 纵坐标结束地址0x009f(159)
    TFTSPI_Write_Byte(0xb3); // 9f
    TFTSPI_Write_Cmd(0x2C);  // GRAM接收MCU数据或命令
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Fill_Area(unsigned short xs,unsigned short ys,unsigned short xe,
@                                   unsigned short ye,unsigned short color)
@功能说明：填充指定区域
@参数说明：xs ：起始xys ：起始yxe ：结束xys ：结束ycolor ：填充的颜色
@函数返回：无
@调用方法：TFTSPI_Fill_Area(10, 20, 30, 40, u16YELLOW);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Fill_Area(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color)
{
    unsigned int i, j;

    TFTSPI_Set_Pos(xs, ys, xe, ye);
    for (i = 0; i < (xe - xs); i++)
    {
        for (j = 0; j < (ye - ys); j++)
        {
            TFTSPI_Write_Word(color);
        }
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_CLS(unsigned short color)
@功能说明：全屏显示单色画面
@参数说明：color ：填充的颜色
@函数返回：无
@调用方法： TFTSPI_CLS(u16YELLOW);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_CLS(unsigned short color)
{
    unsigned int i, j;

    TFTSPI_Addr_Rst();

    for (i = 0; i < TFT18W; i++) // 160
    {
        for (j = 0; j < TFT18H; j++)
        {
            TFTSPI_Write_Word(color);
        }
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Draw_Part(unsigned short xs,unsigned short ys,
@                               unsigned short xe,unsigned short ye,unsigned short color_dat)
@功能说明：填充矩形区域
@参数说明：xs ：起始xys ：起始yxe ：结束xys ：结束ycolor_dat ：填充的颜色
@函数返回：无
@调用方法：TFTSPI_Draw_Part(10, 20, 30, 40, u16YELLOW);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Draw_Part(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color_dat)
{
    unsigned char i, j;

    TFTSPI_Set_Pos(xs, ys, xe, ye);
    for (j = 0; j < (ye - ys + 1); j++)
    {
        for (i = 0; i < (xe - xs + 1); i++)
        {
            TFTSPI_Write_Word(color_dat);
        }
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Draw_Rectangle(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye,unsigned short color_dat)
@功能说明：画矩形边框
@参数说明：xs ：起始xys ：起始yxe ：结束xys ：结束ycolor_dat ：颜色
@函数返回：无
@调用方法：TFTSPI_Draw_Rectangle(10, 20, 30, 40, u16YELLOW);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Draw_Rectangle(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color_dat)
{
    TFTSPI_Draw_Line(xs, ys, xs, ye, color_dat); // 画矩形左边
    TFTSPI_Draw_Line(xe, ys, xe, ye, color_dat); // 画矩形右边
    TFTSPI_Draw_Line(xs, ys, xe, ys, color_dat); // 画矩形上边
    TFTSPI_Draw_Line(xs, ye, xe, ye, color_dat); // 画矩形下边
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Draw_Circle(unsigned short x,unsigned short y,unsigned short r,unsigned short color_dat)
@功能说明：画圆
@参数说明：x ：圆心x   (0-127)y ：圆心y   (0-159)r ：半径    (0-128)color_dat ：颜色
@函数返回：无
@调用方法：TFTSPI_Draw_Circle(50, 50, 30, u16YELLOW);
@备    注：圆心坐标不要超出屏幕范围
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Draw_Circle(unsigned char x, unsigned char y, unsigned char r, unsigned short color_dat)
{
    unsigned char dx, dy = r;

    if ((x >= r) && ((TFT18W - x) >= r) && (y >= r) && ((TFT18H - y) >= r)) // 确定所画圆在屏幕范围内，没有超出最外边，(暂不支持与屏幕边相交)
    {
        for (dx = 0; dx <= r; dx++)
        {
            while ((r * r + 1 - dx * dx) < (dy * dy))
                dy--;
            TFTSPI_Draw_Dot(x + dx, y - dy, color_dat);
            TFTSPI_Draw_Dot(x - dx, y - dy, color_dat);
            TFTSPI_Draw_Dot(x - dx, y + dy, color_dat);
            TFTSPI_Draw_Dot(x + dx, y + dy, color_dat);

            TFTSPI_Draw_Dot(x + dy, y - dx, color_dat);
            TFTSPI_Draw_Dot(x - dy, y - dx, color_dat);
            TFTSPI_Draw_Dot(x - dy, y + dx, color_dat);
            TFTSPI_Draw_Dot(x + dy, y + dx, color_dat);
        }
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Draw_Line(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye,unsigned short color_dat)
@功能说明：画线
@参数说明：xs ：起始xys ：起始yxe ：结束xys ：结束ycolor_dat ：颜色
@函数返回：无
@调用方法：TFTSPI_Draw_Line(10, 20, 30, 40, u16YELLOW);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Draw_Line(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color_dat)
{
    int i, ds;
    int dx, dy, inc_x, inc_y;
    int xerr = 0, yerr = 0; // 初始化变量

    if (xs == xe) // 如果是画垂直线则只需对竖直坐标计数
    {
        TFTSPI_Set_Pos(xs, ys, xe, ye);
        for (i = 0; i < (ye - ys + 1); i++)
        {
            TFTSPI_Write_Word(color_dat);
        }
    }
    else if (ys == ye) // 如果是水平线则只需要对水平坐标计数
    {
        TFTSPI_Set_Pos(xs, ys, xe, ye);
        for (i = 0; i < (xe - xs + 1); i++)
        {
            TFTSPI_Write_Word(color_dat);
        }
    }
    else // 如果是斜线，则重新计算，使用画点函数画出直线
    {
        dx = xe - xs; // 计算坐标增量
        dy = ye - ys;

        if (dx > 0)
            inc_x = 1; // 设置单步方向
        else
        {
            inc_x = -1;
            dx = -dx;
        }
        if (dy > 0)
            inc_y = 1; // 设置单步方向
        else
        {
            inc_y = -1;
            dy = -dy;
        }

        if (dx > dy)
        {
            ds = dx;
        } // 选取基本增量坐标轴
        else
        {
            ds = dy;
        }

        for (i = 0; i <= ds + 1; i++) // 画线输出
        {
            TFTSPI_Draw_Dot(xs, ys, color_dat); // 画点
            xerr += dx;
            yerr += dy;
            if (xerr > ds)
            {
                xerr -= ds;
                xs += inc_x;
            }
            if (yerr > ds)
            {
                yerr -= ds;
                ys += inc_y;
            }
        }
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：float constrain_float(float amt, float low, float high)
@功能说明：画点
@参数说明：x ：x   y ：y   color_dat ：颜色
@函数返回：无
@调用方法：TFTSPI_Draw_Dot(10, 20, u16YELLOW);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Draw_Dot(unsigned char x, unsigned char y, unsigned short color_dat)
{
    TFTSPI_Set_Pos(x, y, x, y);
    TFTSPI_Write_Word(color_dat);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Show_Pic(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye,const unsigned char *ppic)
@功能说明：显示图片
@参数说明：ys ：起始y  xs ：起始x  xe ：结束x  ys ：结束y  ppic ：图片数据
@函数返回：无
@调用方法：TFTSPI_Show_Pic(138,2,20,10,gImage_Battery_S);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Show_Pic(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, const unsigned char *ppic)
{
    unsigned int i, j, k = 0;

    TFTSPI_Set_Pos(xs, ys, xe - 1, ye);
    for (i = 0; i < ye - ys; i++)
    {
        for (j = 0; j < xe - xs; j++)
        {
            TFTSPI_Write_Word(((unsigned short)ppic[k]) << 8 | ppic[k + 1]);
            k++;
            k++;
        }
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_P6X8(unsigned short x, unsigned short y, unsigned short c_dat,unsigned short word_color,unsigned short back_color)
@功能说明：液晶字符输出(6*8字体)
@参数说明：x: 0 - 20(行)  y: 0 - 19(列)  word_color: 字体颜色  back_color: 背景颜色
@函数返回：无
@调用方法：TFTSPI_P6X8(0, 6, txt, u16RED, u16BLUE);
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_P6X8(unsigned char x, unsigned char y, unsigned char c_dat, unsigned short word_color, unsigned short back_color)
{
    unsigned char i, j;

    TFTSPI_Set_Pos(x * 6, y * 8, (x + 1) * 6 - 1, (y + 1) * 8 - 1); // 定位字符显示区域

    for (j = 0; j < 8; j++)
    {
        for (i = 0; i < 6; i++)
        {
            if ((Font_code8[c_dat - 32][i]) & (0x01 << j))
                TFTSPI_Write_Word(word_color);
            else
                TFTSPI_Write_Word(back_color);
        }
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_P8X8(unsigned short x, unsigned short y, char *s_dat,unsigned short word_color,unsigned short back_color)
@功能说明：液晶汉字字符串输出(8*8字体)
@参数说明：x: 0 - 7(行) y: 0 - 9(列) word_color: 字体颜色 back_color: 背景颜色
@函数返回：无
@调用方法：TFTSPI_P8x8(1, 1, "123456", u16YELLOW, u16RED);
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_P8X8(unsigned char x, unsigned char y, unsigned char c_dat, unsigned short word_color, unsigned short back_color)
{
    unsigned char i, j;

    TFTSPI_Set_Pos(x * 8, y * 8, (x + 1) * 8 - 1, (y + 1) * 8 - 1); // 定位字符显示区域

    for (j = 0; j < 8; j++)
    {
        TFTSPI_Write_Word(back_color);
        for (i = 0; i < 6; i++)
        {
            if ((Font_code8[c_dat - 32][i]) & (0x01 << j))
                TFTSPI_Write_Word(word_color);
            else
                TFTSPI_Write_Word(back_color);
        }
        TFTSPI_Write_Word(back_color);
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_P8X16(unsigned short x, unsigned short y, char *s_dat,unsigned short word_color,unsigned short back_color)
@功能说明：液晶汉字字符串输出(8*16字体)
@参数说明：x: 0 - 7(行) y: 0 - 9(列) word_color: 字体颜色 back_color: 背景颜色
@函数返回：无
@调用方法：TFTSPI_P8x16(1, 1, "123456", u16YELLOW, u16RED);
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_P8X16(unsigned char x, unsigned char y, unsigned char c_dat, unsigned short word_color, unsigned short back_color)
{
    unsigned char i, j;

    TFTSPI_Set_Pos(x * 8, y * 16, (x + 1) * 8 - 1, (y + 1) * 16 - 1); // 定位字符显示区域

    for (j = 0; j < 16; j++)
    {
        for (i = 0; i < 8; i++)
        {
            if ((Font_code16[c_dat - 32][j]) & (0x01 << i))
                TFTSPI_Write_Word(word_color);
            else
                TFTSPI_Write_Word(back_color);
        }
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_P6X8Str(unsigned short x, unsigned short y, char *s_dat,unsigned short word_color,unsigned short back_color)
@功能说明：液晶字符串输出(6*8字体)
@参数说明：x: x:0 -15(行)  y: y:0 -9 (列)  word_color:字体颜色  back_color:背景颜色
@函数返回：无
@调用方法：TFTSPI_P6X8Str(1, 1, "123456", u16YELLOW, u16RED);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_P6X8Str(unsigned char x, unsigned char y, char *s_dat, unsigned short word_color, unsigned short back_color)
{
    while (*s_dat)
        TFTSPI_P6X8(x++, y, *s_dat++, word_color, back_color);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_P8X8Str(unsigned short x, unsigned short y, char *s_dat,unsigned short word_color,unsigned short back_color)
@功能说明：液晶字符串输出(8*8字体)
@参数说明：x: x:0 -15(行)  y: y:0 -9 (列)  word_color:字体颜色  back_color:背景颜色
@函数返回：无
@调用方法：TFTSPI_P8X8Str(1, 1, "123456", u16YELLOW, u16RED);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_P8X8Str(unsigned char x, unsigned char y, char *s_dat, unsigned short word_color, unsigned short back_color)
{
    while (*s_dat)
        TFTSPI_P8X8(x++, y, *s_dat++, word_color, back_color);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_P8X16Str(unsigned short x, unsigned short y, char *s_dat,unsigned short word_color,unsigned short back_color)
@功能说明：液晶汉字字符串输出(8*16字体)
@参数说明：x: 0 - 7(行) y: 0 - 9(列) word_color: 字体颜色 back_color: 背景颜色
@函数返回：无
@调用方法：TFTSPI_P8x16Str(1, 1, "123456", u16YELLOW, u16RED);
@备    注：汉字只能是字库里的 字库没有的需要自行添加
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_P8X16Str(unsigned char x, unsigned char y, char *s_dat, unsigned short word_color, unsigned short back_color)
{
    while (*s_dat)
        TFTSPI_P8X16(x++, y, *s_dat++, word_color, back_color);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_P16X16Str(unsigned short x, unsigned short y, char *s_dat,unsigned short word_color,unsigned short back_color)
@功能说明：液晶汉字字符串输出(16*16字体)
@参数说明：x: 0 - 7(行) y: 0 - 9(列) word_color: 字体颜色 back_color: 背景颜色
@函数返回：无
@调用方法：TFTSPI_P16x16Str(1, 1, "123456", u16YELLOW, u16RED);
@备    注：汉字只能是字库里的 字库没有的需要自行添加
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_P16x16Str(unsigned char x, unsigned char y, unsigned char *s_dat, unsigned short word_color, unsigned short back_color)
{
    unsigned char wm = 0, ii = 0, i, j;
    int adder = 1;

    while (s_dat[ii] != '\0')
    {
        wm = 0;
        adder = 1;
        while (hanzi_Idx[wm] > 127)
        {
            if (hanzi_Idx[wm] == (uint8)s_dat[ii])
            {
                if (hanzi_Idx[wm + 1] == s_dat[ii + 1])
                {
                    adder = wm * 16;
                    break;
                }
            }
            wm += 2;
        }

        if (adder != 1) // 显示汉字
        {
            TFTSPI_Set_Pos(x * 16, y * 16, (x + 1) * 16 - 1, (y + 1) * 16 - 1); // 定位字符显示区域
            for (j = 0; j < 32; j++)
            {
                for (i = 0; i < 8; i++)
                {
                    if ((hanzi16x16[adder]) & (0x80 >> i))
                        TFTSPI_Write_Word(word_color);
                    else
                        TFTSPI_Write_Word(back_color);
                }
                adder += 1;
            }
        }
        else // 显示空白字符
        {
        }
        // y+=1;//左右方向
        x += 1; // 上下方向
        ii += 2;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Show_Pic2(unsigned short xs,unsigned short ys,unsigned short w,unsigned short h,const unsigned char *ppic)
@功能说明：显示图片
@参数说明：xs ：起始x ys ：起始y xe ：结束x ys ：结束y ppic ：图片数据
@函数返回：无
@调用方法：TFTSPI_Show_Pic2(138,2,20,10,gImage_Battery_S);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Show_Pic2(unsigned char xs, unsigned char ys, unsigned char w, unsigned char h, const unsigned char *ppic)
{
    unsigned int i;
    TFTSPI_Set_Pos(xs, ys, xs + w - 1, ys + h);
    for (i = 0; i < w * h; i++)
    {
        TFTSPI_Write_Word((ppic[2 * i] << 8) + (ppic[2 * i + 1])); // 高位在前，且两个数据组合成一个16位数据表示像素值
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Show_Battery_Icon(void)
@功能说明：显示电池图标
@参数说明：无
@函数返回：无
@调用方法： TFTSPI_Show_Battery_Icon();
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Show_Battery_Icon(void)
{
    TFTSPI_Show_Pic2(138, 2, 20, 10, gImage_Battery_S); // 显示电池图标20*10像素
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Show_Title(void)
@功能说明：显示标签
@参数说明：无
@函数返回：无
@调用方法：TFTSPI_Show_Title();
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Show_Title(void)
{
    TFTSPI_Show_Pic2(0, 0, 92, 28, gImage_title2);
    TFTSPI_Draw_Line(0, 24, 160, 24, U16_BLACK); // 补齐分割线
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Show_Logo(unsigned short xs,unsigned short ys)
@功能说明：显示龙邱LOG
@参数说明：无
@函数返回：无
@调用方法：TFTSPI_Show_Logo(0,37);//显示龙邱LOGO
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Show_Logo(unsigned char xs, unsigned char ys)
{
    TFTSPI_Show_Pic2(xs, ys, 160, 54, gImage_lo); // 80,gImage_ftflogo);     //
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Road(unsigned short wide_start, unsigned short high_start, unsigned short high, unsigned short wide, unsigned char *Pixle)
@功能说明：TFT18屏 unsigned short 灰度数据显示
@参数说明：high_start ： 显示图像开始位置wide_start：显示图像开始位置 high： 显示图像高度 wide：显示图像宽度 Pixle：显示图像数据地址
@函数返回：无
@调用方法：TFTSPI_Road(0, 0, LCDH, LCDW, (unsigned char *)Image_Use);
@备    注：注意 屏幕左上为 （0，0）
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Road(unsigned char wide_start, unsigned char high_start, unsigned char high, unsigned char wide, unsigned char *Pixle)
{
    unsigned long i;
    /* 设置显示范围 */
    TFTSPI_Set_Pos(wide_start, high_start, wide_start + wide - 1, high_start + high - 1);
    unsigned short color;
    unsigned long temp = high * wide;
    /* 显示图像 */
    for (i = 0; i < temp; i++)
    {

        /* 将灰度转化为 RGB565 */
        color = (Pixle[i] >> 3) << 11;
        color |= (Pixle[i] >> 2) << 5;
        color |= Pixle[i] >> 3;
        /* 显示 */
        TFTSPI_Write_Word(color);
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_BinRoad(unsigned short wide_start, unsigned short high_start, unsigned short high, unsigned short wide, unsigned char *Pixle)
@功能说明：TFT18屏 unsigned short 二值化数据显示
@参数说明：high_start ： 显示图像开始位置wide_start：显示图像开始位置 high： 显示图像高度 wide：显示图像宽度 Pixle：显示图像数据地址
@函数返回：无
@调用方法：TFTSPI_BinRoad(0, 0, LCDH, LCDW, (unsigned char *) Bin_Image);// 显示摄像头图像
@备    注：注意 屏幕左上为 （0，0）
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_BinRoad(unsigned char wide_start, unsigned char high_start, unsigned char high, unsigned char wide, unsigned char *Pixle)
{
    unsigned long i;
    /* 设置显示范围 */
    TFTSPI_Set_Pos(wide_start, high_start, wide_start - 1 + wide, high_start - 1 + high);

    unsigned long temp = high * wide;
    /* 显示图像 */
    for (i = 0; i < temp; i++)
    {
        if (Pixle[i])
        {
            /* 显示 */
            TFTSPI_Write_Word(0xffff);
        }
        else
        {
            /* 显示 */
            TFTSPI_Write_Word(0);
        }
    }
}
/*!
 * @brief    液晶字符串输出
 *
 * @param    x:   (行)
 * @param    y:   (列)
 * @param    word_color: 字体颜色
 * @param    back_color: 背景颜色
 * @param    size :字号 可选12 16 24 32
 * @return   无
 *
 * @note
 *
 * @see      TFTSPI_P16x16Str(1, 1, "123456", u16YELLOW, u16RED);
 *
 */
void TFTSPI_ShowStr(unsigned char x, unsigned char y, char *s_dat,
                    unsigned short fc, unsigned short pc, unsigned char size)
{
    if (size == 12)
        TFTSPI_P8X8Str(x, y, s_dat, fc, pc);
    else if (size == 16)
        TFTSPI_P8X16Str(x, y, s_dat, fc, pc);
    //  else if(size==24)TFTSPI_P16x16Str(x,y,s_dat,fc,pc);
    //  else if(size==32)TFTSPI_P16X16Str(x,y,*s_dat,fc,pc);

    // 16*16没有字库 需要自己生成
}

/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////以下部分为功能测试/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Test_TFT18(void)
@功能说明：测试函数
@参数说明：无
@函数返回：无
@调用方法：
@备    注：主函数调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_TFT18(void)
{
     GPIO_LED_Init();
     TFTSPI_Init(0);                                                                          // LCD初始化  0:横屏  1：竖屏
     TFTSPI_CLS(U16_BLUE);                                                                   // 蓝色屏幕
     TFTSPI_Show_Logo(0, 37);                                                                // 显示龙邱LOGO
     TFTSPI_P16x16Str(0, 0, (unsigned char *)"北京龙邱丘智能科技淘宝店", U16_RED, U16_BLUE); // 字符串显示
     TFTSPI_P8X16Str(0, 1, "Long Qiu i.s.t.", U16_WHITE, U16_BLACK);                         // 字符串显示
     Delay_Ms(1000);                                                                         // 延时等待
     char txt[32];
     unsigned short count = 1;
     while (1)
     {
         sprintf(txt, "variate:%05d", count);           // 将变量填充到字符串的对应位置，并将字符串存放到txt[]中
         TFTSPI_P8X16Str(0, 6, txt, U16_RED, U16_BLUE); // 将txt中 内容显示出来
         LED_Ctrl(LED0, RVS);                           // 电平翻转,LED闪烁
         Delay_Ms(500);                                 // 延时等待
         count++;
     }
}
