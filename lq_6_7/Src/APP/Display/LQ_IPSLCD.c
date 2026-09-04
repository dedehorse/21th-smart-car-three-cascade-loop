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


#include "LQ_IPSLCD.h"
#include "lq_include.h"

#include "LQ_Font.h"


uint16_t LCD_W = 0, LCD_H = 0; // LCD宽度和高度，初始化和宏设置屏幕类型时被更改
uint8_t USE_HORIZONTAL = 0;
/*!
 * @brief    IPS屏幕初始化
 * @return   无
 * @参数说明：  Dir 屏幕方向参数0-3，旋转0-270度，步进90度
 * @note     如果修改管脚 需要修改初始化的管脚
 * @see      LCD_Init();
 * @date     2026/01/28
 */
void LCD_set_WH(show_dir_t Dir)
{
   USE_HORIZONTAL = Dir;
#ifdef IPS114
  if (Dir == Screen_Dir0 || Dir == Screen_Dir1)
  {
    LCD_W = 135;
    LCD_H = 240;
  }
  else
  {
    LCD_W = 240;
    LCD_H = 135;
  }
#endif  // IPS114

#ifdef IPS130
  LCD_W = 240;
  LCD_H = 240;
#endif // IPS130

#ifdef IPS154
  LCD_W = 240;
  LCD_H = 240;
#endif // IPS154

#ifdef IPS200
  if (Dir == Screen_Dir0 || Dir == Screen_Dir1)
  {
    LCD_W = 240;
    LCD_H = 320;
  }
  else
  {
    LCD_W = 320;
    LCD_H = 240;
  }
#endif

}
/*!
 * @brief    IPS屏幕初始化
 *
 * @return   无
 *
 * @参数说明：Dir 屏幕方向参数0-3，旋转0-270度，步进90度
 *
 * @note     如果修改管脚 需要修改初始化的管脚
 *
 * @see      LCD_Init(show_dir_t Dir);
 *
 * @date     2026/03/10
 */
void LCD_Init(show_dir_t Dir)
{
    LCD_set_WH(Dir);  /* 根据传入的方向和宏定义的屏幕类型确定屏幕窗口大小 */
    #if(1 == USE_HARD_SPI)   /* 使用硬件SPI */
    HARD_QSPI_Init(IPS_QSPI_index,IPS_SCLK, IPS_MOSI, IPS_MISO, IPS_SLSO, IPS_SPI_MODE, 40000000u);
//    QSPI_InitConfig(IPS_SCLK, IPS_MISO, IPS_MOSI, IPS_SLSO, 30000000u, IPS_SPI_MODE);
    #else
        PIN_InitConfig(IPS_CK, PIN_MODE_OUTPUT, 1);
        PIN_InitConfig(IPS_DI, PIN_MODE_OUTPUT, 1);
    #endif
    PIN_InitConfig(IPS_CS, PIN_MODE_OUTPUT, 1);
    PIN_InitConfig(IPS_DC, PIN_MODE_OUTPUT, 1);
    PIN_InitConfig(IPS_RST, PIN_MODE_OUTPUT, 1);

    /*复位*/
    IPS_RST_L;
    Delay_Ms(100);
    IPS_RST_H;
    Delay_Ms(100);

    IPS_CS_L;
    Delay_Ms(150);
    LCD_Write_Cmd(0x11);
    Delay_Ms(120);
    LCD_Write_Cmd(0x36);
    if (Dir==Screen_Dir0)
        LCD_Write_Byte(0x00);
    else if (Dir==Screen_Dir1)
        LCD_Write_Byte(0xC0);
    else if (Dir==Screen_Dir2)
        LCD_Write_Byte(0x70);
    else if (Dir==Screen_Dir3)
        LCD_Write_Byte(0xA0);

    LCD_Write_Cmd(0x3A);
    LCD_Write_Byte(0x05);
    LCD_Write_Cmd(0xB2);
    LCD_Write_Byte(0x0C);
    LCD_Write_Byte(0x0C);
    LCD_Write_Byte(0x00);
    LCD_Write_Byte(0x33);
    LCD_Write_Byte(0x33);
    LCD_Write_Cmd(0xB7);
    LCD_Write_Byte(0x35);
    LCD_Write_Cmd(0xBB);
    LCD_Write_Byte(0x19);
    LCD_Write_Cmd(0xC0);
    LCD_Write_Byte(0x2C);
    LCD_Write_Cmd(0xC2);
    LCD_Write_Byte(0x01);
    LCD_Write_Cmd(0xC3);
    LCD_Write_Byte(0x12);
    LCD_Write_Cmd(0xC4);
    LCD_Write_Byte(0x20);
    LCD_Write_Cmd(0xC6);
    LCD_Write_Byte(0x0F);
    LCD_Write_Cmd(0xD0);
    LCD_Write_Byte(0xA4);
    LCD_Write_Byte(0xA1);
    LCD_Write_Cmd(0xE0);
    LCD_Write_Byte(0xD0);
    LCD_Write_Byte(0x04);
    LCD_Write_Byte(0x0D);
    LCD_Write_Byte(0x11);
    LCD_Write_Byte(0x13);
    LCD_Write_Byte(0x2B);
    LCD_Write_Byte(0x3F);
    LCD_Write_Byte(0x54);
    LCD_Write_Byte(0x4C);
    LCD_Write_Byte(0x18);
    LCD_Write_Byte(0x0D);
    LCD_Write_Byte(0x0B);
    LCD_Write_Byte(0x1F);
    LCD_Write_Byte(0x23);
    LCD_Write_Cmd(0xE1);
    LCD_Write_Byte(0xD0);
    LCD_Write_Byte(0x04);
    LCD_Write_Byte(0x0C);
    LCD_Write_Byte(0x11);
    LCD_Write_Byte(0x13);
    LCD_Write_Byte(0x2C);
    LCD_Write_Byte(0x3F);
    LCD_Write_Byte(0x44);
    LCD_Write_Byte(0x51);
    LCD_Write_Byte(0x2F);
    LCD_Write_Byte(0x1F);
    LCD_Write_Byte(0x1F);
    LCD_Write_Byte(0x20);
    LCD_Write_Byte(0x23);
    LCD_Write_Cmd(0x21);
    LCD_Write_Cmd(0x29);
}

/*!
 * @brief    简单延时函数
 *
 * @param    Del ：延时时间
 *
 * @return   无
 *
 * @note     内部调用
 *
 * @see      IPSdelay_1us(1);
 *
 * @date     2019/6/13 星期四
 */
void IPSdelay_1us(unsigned int Del) //
{
    while (Del--)
    {
        __asm("NOP");
    }
}

#if (1 == USE_HARD_SPI)  /* IPS使用硬件QSPI */
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LCD_Write_Cmd(unsigned short cmd)
@功能说明：写命令
@参数说明：cmd ：命令
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Write_Cmd(0xb7); //LCD Driveing control
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LCD_Write_Cmd(unsigned char cmd)
{
    IPS_DC_L; // A0=0 ILI9163_A0=0;    //A0=0发送命令
    HARD_QSPI_Wirte_Read(IPS_QSPI_index,&cmd, &cmd, 1, 1);
//    QSPI_ReadWriteNByte(IPS_QSPI_PORT,&cmd, &cmd, 1);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Write_Byte(unsigned short dat)
@功能说明：写字节
@参数说明：dat ：数据
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Write_Byte(0x00);  //CRL=0
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LCD_Write_Byte(unsigned char dat)
{
    IPS_DC_H; // A0=1  ILI9163_A0=1;          //A0=1发送数据
    HARD_QSPI_Wirte_Read(IPS_QSPI_index,&dat, &dat, 1, 1);
//    QSPI_ReadWriteNByte(IPS_QSPI_PORT,&dat, &dat,1);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Write_Word(unsigned short dat)
@功能说明：写半字
@参数说明：dat ：数据
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Write_Word(0xFFFF);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LCD_Write_Word(unsigned int dat)
{
    IPS_DC_H; // A0=1  ILI9163_A0=1;
    unsigned char buff[2];
    buff[0] = dat >> 8;
    buff[1] = (unsigned char)dat;
    HARD_QSPI_Wirte_Read(IPS_QSPI_index,buff, buff, 2, 1);
//    QSPI_ReadWriteNByte(IPS_QSPI_PORT,buff, buff, 2);
}
#else

/*!
 * @brief    写命令
 *
 * @param    cmd ：命令
 *
 * @return   无
 *
 * @note     内部调用
 *
 * @see      LCD_Write_Cmd(0xb7); //LCD Driveing control
 *
 * @date     2019/6/13 星期四
 */
void LCD_Write_Cmd(unsigned char cmd)
{
    unsigned char i;

    IPS_DC_L; // A0=0 ILI9163_A0=0;     //A0=0发送命令

    for (i = 0; i < 8; i++)
    {
        IPS_CK_L; // SCK=0
        if (cmd & 0x80)
        {
            IPS_DI_H; // SDI=1
        }
        else
        {
            IPS_DI_L; // SDI=0
        }
        IPS_CK_H; // SCK=1
        cmd <<= 1;
    }
    IPS_DC_H;
}

/*!
 * @brief    写字节
 *
 * @param    dat ：数据
 *
 * @return   无
 *
 * @note     内部调用
 *
 * @see      LCD_Write_Byte(0x00); //CRL=0
 *
 * @date     2019/6/13 星期四
 */
void LCD_Write_Byte(unsigned char dat)
{
    unsigned char i;

    IPS_DC_H; // A0=1  ILI9163_A0=1;         //A0=1发送数据

    for (i = 0; i < 8; i++)
    {
        IPS_CK_L; // SCK=0  ILI9163_SCK=0;
        if (dat & 0x80)
        {
            IPS_DI_H;
        } // SDI=1
        else
        {
            IPS_DI_L;
        } // SDI=0
        IPS_CK_H; // SCK=1 ILI9163_SCK = 1;
        dat = (dat << 1);
    }
}

/*!
 * @brief    写半字
 *
 * @param    dat ：数据
 *
 * @return   无
 *
 * @note     无
 *
 * @see      LCD_Write_Word(0xFFFF);
 *
 * @date     2019/6/13 星期四
 */
void LCD_Write_Word(unsigned int dat)
{
    unsigned char i;

    IPS_DC_H; // A0=1  ILI9163_A0=1;

    for (i = 0; i < 16; i++)
    {
        IPS_CK_L; // SCK=0  ILI9163_SCK=0;
        if (dat & 0x8000)
        {
            IPS_DI_H;
        } // SDI=1
        else
        {
            IPS_DI_L;
        } // SDI=0
        IPS_CK_H; // SCK=1  ILI9163_SCK=1;
        dat <<= 1;
    }
}
#endif
/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
             y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    int W_offset = 0, H_offset = 0;
    if (USE_HORIZONTAL == 0)
    {
#ifdef IPS114
        W_offset = 52;
        H_offset = 40;
#endif
#ifdef IPS130
        W_offset = 0;
        H_offset = 0;
#endif
#ifdef IPS154
        W_offset = 0;
        H_offset = 0;
#endif
#ifdef IPS200
        W_offset = 0;
        H_offset = 0;
#endif
    }
    else if (USE_HORIZONTAL == 1)
    {
#ifdef IPS114
        W_offset = 53;
        H_offset = 40;
#endif
#ifdef IPS130
        W_offset = 0;
        H_offset = 80;
#endif
#ifdef IPS154
        W_offset = 0;
        H_offset = 80;
#endif
#ifdef IPS200
        W_offset = 0;
        H_offset = 0;
#endif
    }
    else if (USE_HORIZONTAL == 2)
    {
#ifdef IPS114
        W_offset = 40;
        H_offset = 53;
#endif
#ifdef IPS130
        W_offset = 0;
        H_offset = 0;
#endif
#ifdef IPS154
        W_offset = 0;
        H_offset = 0;
#endif
#ifdef IPS200
        W_offset = 0;
        H_offset = 0;
#endif
    }
    else
    {
#ifdef IPS114
        W_offset = 40;
        H_offset = 52;
#endif
#ifdef IPS130
        W_offset = 80;
        H_offset = 0;
#endif
#ifdef IPS154
        W_offset = 80;
        H_offset = 0;
#endif
#ifdef IPS200
        W_offset = 0;
        H_offset = 0;
#endif
    }
    LCD_Write_Cmd(0x2a); // 列地址设置
    LCD_Write_Word(x1 + W_offset);
    LCD_Write_Word(x2 + W_offset);
    LCD_Write_Cmd(0x2b); // 行地址设置
    LCD_Write_Word(y1 + H_offset);
    LCD_Write_Word(y2 + H_offset);
    LCD_Write_Cmd(0x2c); // 储存器写
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(unsigned int x, unsigned int y, unsigned int color)
{
    LCD_Address_Set(x, y, x, y); // 设置光标位置
    LCD_Write_Word(color);
}

/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号：可选12 16 24 32
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChar(unsigned int x, unsigned int y, unsigned char num, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    unsigned char temp, sizex, t, m = 0;
    unsigned int i, TypefaceNum; // 一个字符所占字节大小
    unsigned int x0 = x;
    sizex = sizey / 2;
    TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
    num = num - ' '; // 得到偏移后的值
    //    LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置
    if (sizey == 12)
        LCD_Address_Set(x, y * 12, (x + sizex) - 1, (y * 12 + sizey) - 1); // 设置光标位置;
    else if (sizey == 16)
        LCD_Address_Set(x, y * 16, (x + sizex) - 1, (y * 16 + sizey) - 1);
    else if (sizey == 24)
        LCD_Address_Set(x, y * 24, (x + sizex) - 1, (y * 24 + sizey) - 1);
    else if (sizey == 32)
        LCD_Address_Set(x, y * 32, (x + sizex) - 1, (y * 32 + sizey) - 1);
    for (i = 0; i < TypefaceNum; i++)
    {
        if (sizey == 12)
            temp = ascii_1206[num][i]; // 调用6x12字体
        else if (sizey == 16)
            temp = ascii_1608[num][i]; // 调用8x16字体
        else if (sizey == 24)
            temp = ascii_2412[num][i]; // 调用12x24字体
        else if (sizey == 32)
            temp = ascii_3216[num][i]; // 调用16x32字体
        else
            return;
        for (t = 0; t < 8; t++)
        {
            if (!mode) // 非叠加模式
            {
                if (temp & (0x01 << t))
                    LCD_Write_Word(fc);
                else
                    LCD_Write_Word(bc);
                m++;
                if (m % sizex == 0)
                {
                    m = 0;
                    break;
                }
            }
            else // 叠加模式
            {
                if (temp & (0x01 << t))
                    LCD_DrawPoint(x, y, fc); // 画一个点
                x++;
                if ((x - x0) == sizex)
                {
                    x = x0;
                    y++;
                    break;
                }
            }
        }
    }
}

/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号：可选12 16 24 32
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowString(unsigned int x, unsigned int y, char *p, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    while (*p != '\0')
    {
        LCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
        x += sizey / 2;
        p++;
    }
}

/******************************************************************************
      函数说明：显示12*06大小的字符串
      入口数据：x    IPS114: 横坐标（横屏时：0-39     竖屏时：0-21） y 纵坐标（横屏时：0-10     竖屏时：0-19）
             x  IPS130: 横坐标（横屏时：0-39      竖屏时：0-39） y 纵坐标（横屏时：0-19     竖屏时：0-19）
             x  IPS154: 横坐标（横屏时：0-39      竖屏时：0-39） y 纵坐标（横屏时：0-19     竖屏时：0-19）
             x  IPS200: 横坐标（横屏时：0-52      竖屏时：0-21） y 纵坐标（横屏时：0-19     竖屏时：0-21）
                *p  要显示的字符串
                fc  字的颜色
                bc  字的背景色
      返回值：  无
******************************************************************************/
void LCD_ShowStr_12(unsigned int x, unsigned int y, char *p, unsigned int fc, unsigned int bc)
{
    while (*p != '\0')
    {
        LCD_ShowChar(x * 6, y * 12, *p, fc, bc, 12, 0);
        x++;
        p++;
    }
}
/******************************************************************************
      函数说明：显示16*08大小的字符串
      入口数据：x  IPS114: 横坐标（横屏时：0-29      竖屏时：0-15） y 纵坐标（横屏时：0-7      竖屏时：0-14）
             x  IPS130: 横坐标（横屏时：0-29      竖屏时：0-29） y 纵坐标（横屏时：0-14     竖屏时：0-14）
             x  IPS154: 横坐标（横屏时：0-29      竖屏时：0-29） y 纵坐标（横屏时：0-14     竖屏时：0-14）
             x  IPS200: 横坐标（横屏时：0-39      竖屏时：0-21） y 纵坐标（横屏时：0-14     竖屏时：0-21）
      返回值：  无
******************************************************************************/
void LCD_ShowStr_16(unsigned int x, unsigned int y, char *p, unsigned int fc, unsigned int bc)
{
    while (*p != '\0')
    {
        LCD_ShowChar(x * 8, y * 16, *p, fc, bc, 16, 0);
        x++;
        p++;
    }
}
/******************************************************************************
      函数说明：显示24*12大小的字符串
      入口数据：x    IPS114: 横坐标（横屏时：0-19     竖屏时：0-10） y 纵坐标（横屏时：0-4     竖屏时：0-9）
             x  IPS130: 横坐标（横屏时：0-19      竖屏时：0-21） y 纵坐标（横屏时：0-9     竖屏时：0-21）
             x  IPS154: 横坐标（横屏时：0-39      竖屏时：0-21） y 纵坐标（横屏时：0-19     竖屏时：0-21）
             x  IPS200: 横坐标（横屏时：0-25      竖屏时：0-21） y 纵坐标（横屏时：0-9     竖屏时：0-21）
                *p  要显示的字符串
                fc  字的颜色
                bc  字的背景色
      返回值：  无
******************************************************************************/
void LCD_ShowStr_24(unsigned int x, unsigned int y, char *p, unsigned int fc, unsigned int bc)
{
    while (*p != '\0')
    {
        LCD_ShowChar(x * 12, y * 24, *p, fc, bc, 24, 0);
        x++;
        p++;
    }
}
/******************************************************************************
      函数说明：x    IPS114: 横坐标（横屏时：0-14     竖屏时：0-7） y 纵坐标（横屏时：0-3     竖屏时：0-6）
             x  IPS130: 横坐标（横屏时：0-14      竖屏时：0-21） y 纵坐标（横屏时：0-6     竖屏时：0-21）
             x  IPS154: 横坐标（横屏时：0-14      竖屏时：0-21） y 纵坐标（横屏时：0-6     竖屏时：0-21）
             x  IPS200: 横坐标（横屏时：0-19      竖屏时：0-21） y 纵坐标（横屏时：0-6     竖屏时：0-21）
                *p  要显示的字符串
                fc  字的颜色
                bc  字的背景色
      返回值：  无
******************************************************************************/
void LCD_ShowStr_32(unsigned int x, unsigned int y, char *p, unsigned int fc, unsigned int bc)
{
    while (*p != '\0')
    {
        LCD_ShowChar(x * 16, y * 32, *p, fc, bc, 32, 0);
        x++;
        p++;
    }
}
/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
                                color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color)
{
    unsigned int i, j;
    LCD_Address_Set(xsta, ysta, xend - 1, yend - 1); // 设置显示范围
    for (i = ysta; i < yend; i++)
    {
        for (j = xsta; j < xend; j++)
        {
            LCD_Write_Word(color);
        }
    }
}
/******************************************************************************
      函数说明：清屏
      入口数据：color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_CLS(unsigned int color)
{
    LCD_Fill(0, 0, LCD_W, LCD_H, color);
}

/******************************************************************************
      函数说明：画线
      入口数据：xs ：起始x
                                ys ：起始y
                                xe ：结束x IPS114   (横屏x:0-240, 竖屏x:0-135)
                                xe ：结束x IPS130  (横屏x:0-240, 竖屏x:0-240)
                                xe ：结束x IPS154  (横屏x:0-240, 竖屏x:0-240)
                                xe ：结束x IPS200  (横屏x:0-320, 竖屏x:0-240)
                                ys ：结束y IPS114   (横屏y:0-135, 竖屏x:0-240)
                                ys ：结束y IPS130  (横屏y:0-240, 竖屏x:0-240)
                                ys ：结束y IPS154  (横屏y:0-240, 竖屏x:0-240)
                                ys ：结束y IPS200  (横屏y:0-240, 竖屏x:0-320)
                                color_dat ：颜色
      返回值：  无
******************************************************************************/
void LCD_Draw_Line(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color_dat)
{
    int i, ds;
    int dx, dy, inc_x, inc_y;
    int xerr = 0, yerr = 0; // 初始化变量

    if (xs == xe) // 如果是画垂直线则只需对竖直坐标计数
    {
        LCD_Address_Set(xs, ys, xe, ye);
        for (i = 0; i < (ye - ys + 1); i++)
        {
            LCD_Write_Word(color_dat);
        }
    }
    else if (ys == ye) // 如果是水平线则只需要对水平坐标计数
    {
        LCD_Address_Set(xs, ys, xe, ye);
        for (i = 0; i < (xe - xs + 1); i++)
        {
            LCD_Write_Word(color_dat);
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
            LCD_DrawPoint(xs, ys, color_dat); // 画点
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

/******************************************************************************
      函数说明：画矩形框
      入口数据：xs ：起始x
                                ys ：起始y
                                xe ：结束x IPS114   (横屏x:0-240, 竖屏x:0-135)
                                xe ：结束x IPS130  (横屏x:0-240, 竖屏x:0-240)
                                xe ：结束x IPS154  (横屏x:0-240, 竖屏x:0-240)
                                xe ：结束x IPS200  (横屏x:0-320, 竖屏x:0-240)
                                ys ：结束y IPS114   (横屏y:0-135, 竖屏x:0-240)
                                ys ：结束y IPS130  (横屏y:0-240, 竖屏x:0-240)
                                ys ：结束y IPS154  (横屏y:0-240, 竖屏x:0-240)
                                ys ：结束y IPS200  (横屏y:0-240, 竖屏x:0-320)
                                color_dat ：颜色
      返回值：  无
******************************************************************************/
void LCD_Draw_Rectangle(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color_dat)
{
    LCD_Draw_Line(xs, ys, xs, ye, color_dat); // 画矩形左边
    LCD_Draw_Line(xe, ys, xe, ye, color_dat); // 画矩形右边
    LCD_Draw_Line(xs, ys, xe, ys, color_dat); // 画矩形上边
    LCD_Draw_Line(xs, ye, xe, ye, color_dat); // 画矩形下边
}

/******************************************************************************
      函数说明：画圆
      入口数据：x ：圆心x IPS114    (横屏x:0-240, 竖屏x:0-135)
                      IPS130  (横屏x:0-240, 竖屏x:0-240)
                      IPS154  (横屏x:0-240, 竖屏x:0-240)
                       IPS200  (横屏x:0-320, 竖屏x:0-240)
            y ：圆心y  IPS114   (横屏y:0-135, 竖屏x:0-240)
                     IPS130  (横屏y:0-240, 竖屏x:0-240)
                     IPS154  (横屏y:0-240, 竖屏x:0-240)
                     IPS200  (横屏y:0-240, 竖屏x:0-320)
                                r ：半径
                                color_dat ：颜色
      返回值：  无
******************************************************************************/
void LCD_Draw_Circle(unsigned char x, unsigned char y, unsigned char r, unsigned short color_dat)
{
    unsigned char dx, dy = r;

    if ((x >= r) && ((LCD_W - x) >= r) && (y >= r) && ((LCD_H - y) >= r)) // 确定所画圆在屏幕范围内，没有超出最外边，(暂不支持与屏幕边相交)
    {
        for (dx = 0; dx <= r; dx++)
        {
            while ((r * r + 1 - dx * dx) < (dy * dy))
                dy--;
            LCD_DrawPoint(x + dx, y - dy, color_dat);
            LCD_DrawPoint(x - dx, y - dy, color_dat);
            LCD_DrawPoint(x - dx, y + dy, color_dat);
            LCD_DrawPoint(x + dx, y + dy, color_dat);

            LCD_DrawPoint(x + dy, y - dx, color_dat);
            LCD_DrawPoint(x - dy, y - dx, color_dat);
            LCD_DrawPoint(x - dy, y + dx, color_dat);
            LCD_DrawPoint(x + dy, y + dx, color_dat);
        }
    }
}

/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组
      返回值：  无
******************************************************************************/
void LCD_Show_Picture(unsigned int x, unsigned int y, unsigned int length, unsigned int width, const unsigned char pic[])
{
    unsigned int i, j;
    unsigned long k = 0;
    LCD_Address_Set(x, y, x + length - 1, y + width - 1);
    for (i = 0; i < length; i++)
    {
        for (j = 0; j < width; j++)
        {
            LCD_Write_Byte(pic[k * 2]);
            LCD_Write_Byte(pic[k * 2 + 1]);
            k++;
        }
    }
}
/******************************************************************************
      函数说明：显示单个12x12汉字
      入口数据：x,y显示的行数和列数
                *s_dat 要显示的汉字
                word_color 字的颜色
                back_color 字的背景色
      返回值：  无
******************************************************************************/
void LCD_ShowChinese12x12(unsigned int x, unsigned int y, char *s_dat, unsigned int word_color, unsigned int back_color)
{
    unsigned char wm = 0, ii = 0, i, j;
    int adder = 1;
    while (s_dat[ii] != '\0')
    {
        wm = 0;
        adder = 1;
        while (tfont_Idx[wm] > 127)
        {
            if (tfont_Idx[wm] == (uint8)s_dat[ii])
            {
                if (tfont_Idx[wm + 1] == (uint8)s_dat[ii + 1])
                {
                    adder = wm * 12;
                    break;
                }
            }
            wm += 2;
        }

        if (adder != 1) // 显示汉字
        {
            LCD_Address_Set(x * 12, y * 12, (x + 1) * 12 - 1, (y + 1) * 12 - 1); // 定位字符显示区域
            for (j = 0; j < 24; j++)
            {
                if ((j % 2) == 0)
                {
                    for (i = 0; i < 8; i++)
                    {
                        if ((tfont_12x12[adder]) & (0x80 >> i))
                            LCD_Write_Word(word_color);
                        else
                            LCD_Write_Word(back_color);
                    }
                }
                else
                {
                    for (i = 0; i < 4; i++)
                    {
                        if ((tfont_12x12[adder]) & (0x80 >> i))
                            LCD_Write_Word(word_color);
                        else
                            LCD_Write_Word(back_color);
                    }
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

/******************************************************************************
      函数说明：显示单个16x16汉字
      入口数据：x,y显示的行数和列数
                *s_dat 要显示的汉字
                word_color 字的颜色
                back_color 字的背景色
      返回值：  无
******************************************************************************/
void LCD_ShowChinese16x16(unsigned int x, unsigned int y, char *s_dat, unsigned int word_color, unsigned int back_color)
{
    unsigned char wm = 0, ii = 0, i, j;
    int adder = 1;
    while (s_dat[ii] != '\0')
    {
        wm = 0;
        adder = 1;
        while (tfont_Idx[wm] > 127)
        {
            if (tfont_Idx[wm] == (uint8)s_dat[ii])
            {
                if (tfont_Idx[wm + 1] == (uint8)s_dat[ii + 1])
                {
                    adder = wm * 16;
                    break;
                }
            }
            wm += 2;
        }

        if (adder != 1) // 显示汉字
        {
            LCD_Address_Set(x * 16, y * 16, (x + 1) * 16 - 1, (y + 1) * 16 - 1); // 定位字符显示区域
            for (j = 0; j < 32; j++)
            {
                for (i = 0; i < 8; i++)
                {
                    if ((tfont_16x16[adder]) & (0x80 >> i))
                        LCD_Write_Word(word_color);
                    else
                        LCD_Write_Word(back_color);
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

/******************************************************************************
      函数说明：显示单个24x24汉字
      入口数据：x,y显示的行数和列数
                *s_dat 要显示的汉字
                word_color 字的颜色
                back_color 字的背景色
      返回值：  无
******************************************************************************/
void LCD_ShowChinese24x24(unsigned int x, unsigned int y, char *s_dat, unsigned int word_color, unsigned int back_color)
{
    unsigned char wm = 0, ii = 0, i, j;
    int adder = 1;
    while (s_dat[ii] != '\0')
    {
        wm = 0;
        adder = 1;
        while (tfont_Idx[wm] > 127)
        {
            if (tfont_Idx[wm] == (uint8)s_dat[ii])
            {
                if (tfont_Idx[wm + 1] == (uint8)s_dat[ii + 1])
                {
                    adder = wm * 36;
                    break;
                }
            }
            wm += 2;
        }

        if (adder != 1) // 显示汉字
        {
            LCD_Address_Set(x * 24, y * 24, (x + 1) * 24 - 1, (y + 1) * 24 - 1); // 定位字符显示区域
            for (j = 0; j < 72; j++)
            {
                for (i = 0; i < 8; i++)
                {
                    if ((tfont_24x24[adder]) & (0x80 >> i))
                        LCD_Write_Word(word_color);
                    else
                        LCD_Write_Word(back_color);
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

/******************************************************************************
      函数说明：显示单个32x32汉字
      入口数据：x,y显示的行数和列数
                *s_dat 要显示的汉字
                word_color 字的颜色
                back_color 字的背景色
      返回值：  无
******************************************************************************/
void LCD_ShowChinese32x32(unsigned int x, unsigned int y, char *s_dat, unsigned int word_color, unsigned int back_color)
{
    unsigned char wm = 0, ii = 0, i, j;
    int adder = 1;
    while (s_dat[ii] != '\0')
    {
        wm = 0;
        adder = 1;
        while (tfont_Idx[wm] > 127)
        {
            if (tfont_Idx[wm] == (uint8)s_dat[ii])
            {
                if (tfont_Idx[wm + 1] == (uint8)s_dat[ii + 1])
                {
                    adder = wm * 64;
                    break;
                }
            }
            wm += 2;
        }

        if (adder != 1) // 显示汉字
        {
            LCD_Address_Set(x * 32, y * 32, (x + 1) * 32 - 1, (y + 1) * 32 - 1); // 定位字符显示区域
            for (j = 0; j < 128; j++)
            {
                for (i = 0; i < 8; i++)
                {
                    if ((tfont_32x32[adder]) & (0x80 >> i))
                        LCD_Write_Word(word_color);
                    else
                        LCD_Write_Word(back_color);
                }
                adder += 1;
            }
        }
        else // 显示空白字符
        {
        }
        x += 1; // 上下方向
        ii += 2;
    }
}
/******************************************************************************
      函数说明：显示汉字串
      入口数据：x,y显示的起始行数和列数
                *s 要显示的汉字串
                fc 字的颜色
                bc 字的背景色
                sizey 字号 可选 12 16 24 32
      返回值：  无
            备注  ：  字库在font.h中，取模选项：宋体，阴码，逐行式，点阵16*16，顺向，十六进制，C51格式
******************************************************************************/
void LCD_ShowChinese(unsigned int x, unsigned int y, char *s, unsigned int fc, unsigned int bc, unsigned char sizey)
{
    while (*s != 0)
    {
        if (sizey == 12)
            LCD_ShowChinese12x12(x, y, s, fc, bc);
        else if (sizey == 16)
            LCD_ShowChinese16x16(x, y, s, fc, bc);
        else if (sizey == 24)
            LCD_ShowChinese24x24(x, y, s, fc, bc);
        else if (sizey == 32)
            LCD_ShowChinese32x32(x, y, s, fc, bc);
        else
            return;
        s += 2;
        x++;
    }
}

/*!
 * @brief    显示龙邱LOG
 *
 * @param    无
 *
 * @return   无
 *
 * @note     无
 *
 * @see      LCD_Show_Logo();
 *
 * @date     2019/6/13 星期四
 */
void LCD_Show_Logo(unsigned char xs, unsigned char ys)
{
    LCD_Show_Picture(xs, ys, 160, 54, gImage_lo); // 80,gImage_ftflogo);     //
}

/*!
 * @brief    IPS114屏 unsigned char 灰度数据显示
 *
 * @param    high_start ： 显示图像开始位置
 * @param    wide_start ： 显示图像开始位置
 * @param    high ： 显示图像高度
 * @param    wide ： 显示图像宽度
 * @param    Pixle： 显示图像数据地址
 *
 * @return   无
 *
 * @note     注意 屏幕左上为 （0，0）
 *
 * @see
 *
 * @date     2019/12/3 星期二
 */
void LCDSPI_Road(unsigned char wide_start, unsigned char high_start, unsigned char high, unsigned char wide, unsigned char *Pixle)
{
    unsigned long i;
    /* 设置显示范围 */
    LCD_Address_Set(wide_start, high_start, wide_start - 1 + wide, high_start - 1 + high);
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
        LCD_Write_Word(color);
    }
}

/*!
 * @brief    IPS114屏 unsigned char 二值化数据显示
 *
 * @param    high_start ： 显示图像开始位置
 * @param    wide_start ： 显示图像开始位置
 * @param    high ： 显示图像高度
 * @param    wide ： 显示图像宽度
 * @param    Pixle： 显示图像数据地址
 *
 * @return   无
 *
 * @note     注意 屏幕左上为 （0，0）
 *
 * @see
 *
 * @date     2019/12/3 星期二
 */
void LCDSPI_BinRoad(unsigned char wide_start, unsigned char high_start, unsigned char high, unsigned char wide, unsigned char *Pixle)
{
    unsigned long i;
    /* 设置显示范围 */
    LCD_Address_Set(wide_start, high_start, wide_start - 1 + wide, high_start - 1 + high);

    unsigned long temp = high * wide;
    /* 显示图像 */
    for (i = 0; i < temp; i++)
    {
        if (Pixle[i])
        {
            /* 显示 */
            LCD_Write_Word(0xffff);
        }
        else
        {
            /* 显示 */
            LCD_Write_Word(0);
        }
    }
}

////////////////////////////////////////////////////////
//////////////////////////////////以下部分为功能测试//////////
/////IPS屏幕测试函数，屏幕型号在LQ_IPSLCD.h中通过宏定义选择//////////

void Test_IPSLCD(void)
{
     char txt[32];
     unsigned short count = 1;
     GPIO_LED_Init();
     LCD_Init(0); // LCD初始化  0:横屏  1：竖屏
     LCD_CLS(U16_BLACK);
     LCD_Draw_Line(50, 50, 200, 130, U16_RED);      // 画线
     LCD_Draw_Rectangle(50, 50, 200, 130, U16_RED); // 画矩形框
     LCD_Draw_Circle(50, 50, 50, U16_RED);          // 画圆
     Delay_Ms(1000);                                // 延时等待
     while (1)
     {
         sprintf(txt, "Val:%05d", count++);            // 将变量填充到字符串的对应位置，并将字符串存放到txt[]中
         LCD_ShowStr_24(0, 4, txt, U16_RED, U16_BLUE); // 显示24*12字符串
         LCD_Show_Picture(200, 95, 40, 40, gImage_1);  // 显示图片
         LCD_Show_Logo(0, 0);                          // 显示LOGO
         LED_Ctrl(LED0, RVS);
         Delay_Ms(250); // 延时等待
     }
}
