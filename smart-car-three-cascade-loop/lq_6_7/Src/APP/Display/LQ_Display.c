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

#include "lq_include.h"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@功能说明：根据预定义的不同屏幕类型宏，对相应的屏幕进行初始化操作。
@参数说明：无
@函数返回：无
@备    注：需确保在调用该函数前，已正确定义了所需的屏幕类型宏，否则对应的屏幕初始化操作不会执行。
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Display_Init(show_dir_t Dir)
{

#ifdef TFT18
    TFTSPI_Init(Dir);
#endif

#ifdef TFT20
    TFT2SPI_Init(Dir);
#endif

#ifdef IPS114
    LCD_Init(Dir);
#endif

#ifdef IPS130
    LCD_Init(Dir);
#endif

#ifdef IPS154
    LCD_Init(Dir);
#endif

#ifdef IPS200
    LCD_Init(Dir);
#endif

#ifdef OLED
    OLED_Init(Dir);
#endif
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@功能说明：在屏幕上显示字符串
@参数说明：
    - x：显示字符的横坐标位置，类型：unsigned int。
    - y：显示字符的纵坐标位置，类型：unsigned int。
    - num：指向要显示的字符字符串的指针，类型：char *。
    - fc：前景色设置，类型：unsigned int。
    - bc：背景色设置，类型：unsigned int。
    - sizey：设置字符的大小 类型：unsigned char        可选大小：12 16 24 32
@函数返回：无
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

void Display_showString(unsigned int x, unsigned int y, char *num, unsigned int fc, unsigned int bc, unsigned char sizey)
{
// TFT屏幕由于像素点较少，所以字体大小只有12与16，这里加了限制
#ifdef TFT18
    if (sizey >= 16)
        sizey = 16;
    TFTSPI_ShowStr((unsigned char)x, (unsigned char)y, (char *)num, (unsigned short)fc, (unsigned short)bc, sizey);
#endif

#ifdef TFT20
    if (sizey >= 24)
        sizey = 24;
    TFT2SPI_ShowStr((unsigned char)x, (unsigned char)y, (char *)num, (unsigned char)fc, (unsigned char)bc, sizey);
#endif

#ifdef IPS114
    LCD_ShowString(x, y, (char *)num, fc, bc, sizey, 0);
#endif

#ifdef IPS130
    LCD_ShowString(x, y, (char *)num, fc, bc, sizey, 0);
#endif

#ifdef IPS154
    LCD_ShowString(x, y, (char *)num, fc, bc, sizey, 0);
#endif

#ifdef IPS200
    LCD_ShowString(x, y, (char *)num, fc, bc, sizey, 0);
#endif

#ifdef OLED
    OLED_ShowStr((unsigned char)x, (unsigned char)y, (char *)num, (unsigned short)fc, (unsigned short)bc, sizey);
#endif
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@功能说明：在屏幕中填充置顶小大的矩形色块
@参数说明：
    - xs：填充区域的起始横坐标，类型为unsigned char。
    - ys：填充区域的起始纵坐标，类型为unsigned char。
    - xe：填充区域的结束横坐标，类型为unsigned char。
    - ye：填充区域的结束纵坐标，类型为unsigned char。
    - color：用于填充区域的颜色值，类型为unsigned short。
@函数返回：无
@备    注：注意坐标越界
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Display_Fill(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color)
{

#ifdef TFT18
    TFTSPI_Fill_Area(xs, ys, xe, ye, color);
#endif

#ifdef TFT20
    TFT2SPI_Fill_Area(xs, ys, xe, ye, color);
#endif

#ifdef IPS114
    LCD_Fill(xs, ys, xe, ye, color);
#endif

#ifdef IPS130
    LCD_Fill(xs, ys, xe, ye, color);
#endif

#ifdef IPS154
    LCD_Fill(xs, ys, xe, ye, color);
#endif

#ifdef IPS200
    LCD_Fill(xs, ys, xe, ye, color);
#endif

#ifdef OLED
    OLED_Fill();
#endif
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@功能说明：对相应屏幕执行清屏操作，并将屏幕设置为指定的颜色(color)。
@参数说明：
    - color：清屏后的背景色
@函数返回：无
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

void Display_CLS(unsigned int color)
{

#ifdef TFT18
    TFTSPI_CLS((unsigned short)color);
#endif

#ifdef TFT20
    TFT2SPI_CLS((unsigned short)color);
#endif

#ifdef IPS114
    LCD_CLS(color);
#endif

#ifdef IPS130
    LCD_CLS(color);
#endif

#ifdef IPS154
    LCD_CLS(color);
#endif

#ifdef IPS200
    LCD_CLS(color);
#endif

#ifdef OLED
    OLED_CLS();
#endif
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@功能说明：在指定的坐标位置(x, y)绘制一个指定颜色(color)的点。不同屏幕类型通过各自对应的画点函数来实现该功能。
@参数说明：
    - x：要绘制点的横坐标位置，类型为unsigned int
    - y：要绘制点的纵坐标位置，类型为unsigned int
    - color：绘制点的颜色值，类型为unsigned int
@函数返回：无
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Display_DrawPoint(unsigned int x, unsigned int y, unsigned int color)
{

#ifdef TFT18
    TFTSPI_Draw_Dot((unsigned char)x, (unsigned char)y, (unsigned short)color);
#endif

#ifdef TFT20
    TFT2SPI_Draw_Dot((unsigned char)x, (unsigned char)y, (unsigned short)color);
#endif

#ifdef IPS114
    LCD_DrawPoint(x, y, color);
#endif

#ifdef IPS130
    LCD_DrawPoint(x, y, color);
#endif

#ifdef IPS154
    LCD_DrawPoint(x, y, color);
#endif

#ifdef IPS200
    LCD_DrawPoint(x, y, color);
#endif

#ifdef OLED
    OLED_PutPixel((unsigned char)x, (unsigned char)y);
#endif
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@功能说明：根据预定义的不同屏幕类型宏，在指定的起始坐标(xs, ys)和结束坐标(xe, ye)之间绘制一条指定颜色(color)的直线。不同屏幕类型调用各自对应的画线函数完成此操作。
@参数说明：
    - xs：直线起始点的横坐标，类型为unsigned char。
    - ys：直线起始点的纵坐标，类型为unsigned char。
    - xe：直线结束点的横坐标，类型为unsigned char。
    - ye：直线结束点的纵坐标，类型为unsigned char。
    - color：直线的颜色值，类型为unsigned short。
@函数返回：无
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Display_DrawLine(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color)
{

#ifdef TFT18
    TFTSPI_Draw_Line(xs, ys, xe, ye, color);
#endif

#ifdef TFT20
    TFT2SPI_Draw_Line(xs, ys, xe, ye, color);
#endif

#ifdef IPS114
    LCD_Draw_Line(xs, ys, xe, ye, color);
#endif

#ifdef IPS130
    LCD_Draw_Line(xs, ys, xe, ye, color);
#endif

#ifdef IPS154
    LCD_Draw_Line(xs, ys, xe, ye, color);
#endif

#ifdef IPS200
    LCD_Draw_Line(xs, ys, xe, ye, color);
#endif

#ifdef OLED
    OLED_PutPixel(xs, ys);
    OLED_PutPixel(xe, ye);
#endif
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Display_DrawRectangle(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color)
@功能说明：在由指定的左上角坐标(xs, ys)和右下角坐标(xe, ye)确定的区域内绘制一个指定颜色(color)的矩形
@参数说明：
    - xs：矩形左上角的横坐标，类型为unsigned char。
    - ys：矩形左上角的纵坐标，类型为unsigned char。
    - xe：矩形右下角的横坐标，类型为unsigned char。
    - ye：矩形右下角的纵坐标，类型为unsigned char。
    - color：矩形的颜色值，类型为unsigned short。
@函数返回：无
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Display_DrawRectangle(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned short color)
{

#ifdef TFT18
    TFTSPI_Draw_Rectangle(xs, ys, xe, ye, color);
#endif

#ifdef TFT20
    TFT2SPI_Draw_Rectangle(xs, ys, xe, ye, color);
#endif

#ifdef IPS114
    LCD_Draw_Rectangle(xs, ys, xe, ye, color);
#endif

#ifdef IPS130
    LCD_Draw_Rectangle(xs, ys, xe, ye, color);
#endif

#ifdef IPS154
    LCD_Draw_Rectangle(xs, ys, xe, ye, color);
#endif

#ifdef IPS200
    LCD_Draw_Rectangle(xs, ys, xe, ye, color);
#endif

#ifdef OLED
    OLED_Rectangle(xs, ys, xe, ye, 0);
#endif
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@功能说明：以指定的坐标(x, y)为圆心，指定的半径(r)绘制一个指定颜色(color)的圆。
@参数说明：
    - x：圆心的横坐标，类型为unsigned char。
    - y：圆心的纵坐标，类型为unsigned char。
    - r：圆的半径，类型为unsigned char。
    - color：圆的颜色值，类型为unsigned short。
@函数返回：无
@备    注：画出的圆为空心圆
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
// 画圆
void Display_DrawCircle(unsigned char x, unsigned char y, unsigned char r, unsigned short color)
{

#ifdef TFT18
    TFTSPI_Draw_Circle(x, y, r, color);
#endif

#ifdef TFT20
    TFT2SPI_Draw_Circle(x, y, r, color);
#endif

#ifdef IPS114
    LCD_Draw_Circle(x, y, r, color);
#endif

#ifdef IPS130
    LCD_Draw_Circle(x, y, r, color);
#endif

#ifdef IPS154
    LCD_Draw_Circle(x, y, r, color);
#endif

#ifdef IPS200
    LCD_Draw_Circle(x, y, r, color);
#endif
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@功能说明：在由指定的左上角坐标(xs, ys)和右下角坐标(xe, ye)确定的区域内显示指定的图片(pic)
@参数说明：
    - xs：显示图片区域的左上角横坐标，类型为unsigned char。
    - ys：显示图片区域的左上角纵坐标，类型为unsigned char。
    - xe：显示图片区域的右下角横坐标，类型为unsigned char。
    - ye：显示图片区域的右下角纵坐标，类型为unsigned char。
    - pic：指向要显示图片数据的指针，类型为unsigned char *。
@函数返回：无
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

void Display_Show(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye, unsigned char *pic)
{

#ifdef TFT18
    TFTSPI_Road(xs, ys, xe, ye, (unsigned char *)pic);
#endif

#ifdef TFT20
    TFT2SPI_Show_Pic2(xs, ys, xe, ye, pic);
#endif

#ifdef IPS114
    LCDSPI_Road(xs, ys, xe, ye, (unsigned char *)pic);
#endif

#ifdef IPS130
    LCDSPI_Road(xs, ys, xe, ye, (unsigned char *)pic);
#endif

#ifdef IPS154
    LCDSPI_Road(xs, ys, xe, ye, (unsigned char *)pic);
#endif

#ifdef IPS200
    LCDSPI_Road(xs, ys, xe, ye, (unsigned char *)pic);

#endif

#ifdef OLED
    OLED_Show_BMP(xs, ys, xe, xe, pic);
#endif
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@功能说明：在指定的坐标位置(x, y)以指定的前景色(fc)、背景色(bc)和字符大小(sizey)显示给定的汉字字符串(s)
@参数说明：
    - x：显示汉字的横坐标位置，类型为unsigned int。
    - y：显示汉字的纵坐标位置，类型为unsigned int。
    - s：指向要显示的汉字字符串的指针，类型为char *。
    - fc：前景色设置，类型为unsigned int。
    - bc：背景色设置，类型为unsigned int。
    - sizey：汉字在y方向上的大小设置，类型为unsigned char。
@函数返回：无
@备    注：只有LQ_Font.c中定义的汉字才可以调用，如果需要其他的汉字，请自行添加
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Display_ShowChinese(unsigned int x, unsigned int y, char *s, unsigned int fc, unsigned int bc, unsigned char sizey)
{

    // TFT屏幕由于像素点较少，所以字体大小只有12与16，这里加了限制
#ifdef TFT18
    if (sizey >= 16)
        sizey = 16;
    TFTSPI_P16x16Str((unsigned char)x, (unsigned char)y, (unsigned char *)s, (unsigned short)fc, (unsigned short)bc); // 字符串显示
    //    TFTSPI_P16x16Str((unsigned char)x, (unsigned char)y, s, (unsigned short)fc, (unsigned short)bc);
    //    TFTSPI_ShowStr((unsigned char)x, (unsigned char)y, s, (unsigned short)fc, (unsigned short)bc, sizey);

#endif

#ifdef TFT20
    if (sizey >= 24)
        sizey = 24;
    TFT2SPI_ShowStr((unsigned char)x, (unsigned char)y, s, (unsigned short)fc, (unsigned short)bc, sizey);
#endif

#ifdef IPS114
    LCD_ShowChinese(x, y, s, fc, bc, sizey);
#endif

#ifdef IPS130
    LCD_ShowChinese(x, y, s, fc, bc, sizey);
#endif

#ifdef IPS154
    LCD_ShowChinese(x, y, s, fc, bc, sizey);
#endif

#ifdef IPS200
    LCD_ShowChinese(x, y, s, fc, bc, sizey);
#endif

#ifdef OLED
//    OLED_Show_BMP(xs, ys, xe, xe, *pic);
//    OLED_P14x16Str((unsigned char)x, (unsigned char)y,(unsigned char)*s);      //显示汉字
#endif
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@功能显示：在指定的坐标位置(xs, ys)显示相应屏幕的logo
@参数说明：
    - xs：显示logo的横坐标位置，类型为unsigned char
    - ys：显示logo的纵坐标位置，类型为unsigned char
@函数返回：无
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Display_Show_Logo(unsigned char xs, unsigned char ys)
{

#ifdef TFT18
    TFTSPI_Show_Logo(xs, ys);
#endif

#ifdef TFT20
    TFT2SPI_Show_Logo(xs, ys);
#endif

#ifdef IPS114
    LCD_Show_Logo(xs, ys);
#endif

#ifdef IPS130
    LCD_Show_Logo(xs, ys);
#endif

#ifdef IPS154
    LCD_Show_Logo(xs, ys);
#endif

#ifdef IPS200
    LCD_Show_Logo(xs, ys);
#endif

#ifdef OLED
    OLED_Show_LQLogo();
#endif
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@功能说明：用于测试屏幕相关功能的函数。首先进行屏幕初始化和清屏操作，然后在循环中不断更新并显示变量值、改变字符大小、显示logo、绘制图形（圆、线、矩形）等操作，以验证屏幕显示的各项功能是否正常工作。
@参数说明：无
@函数返回：无
@备    注：调用此函数可方便测试屏幕，这里也有常见屏幕函数的使用方法
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_Display(void)
{
    char txt[50];
    int count = 0;
    Display_Init(Screen_Dir0);  // eg:方向0竖屏，排线在下面
    Delay_Ms(200);
    Display_CLS(U16_BLACK);
    Delay_Ms(100);
    while (1)
    {
#if 0
        sprintf(txt, "variate:%05d", count);                     // 将变量填充到字符串的对应位置，并将字符串存放到txt[]中
        Display_showString(0, 1, txt, U16_WHITE, U16_BLACK, 12); // 将txt中 内容显示出来
        sprintf(txt, "variate:%05d", count);                     // 将变量填充到字符串的对应位置，并将字符串存放到txt[]中
        Display_showString(0, 2, txt, U16_WHITE, U16_BLACK, 16); // 将txt中 内容显示出来
        sprintf(txt, "variate:%05d", count);                     // 将变量填充到字符串的对应位置，并将字符串存放到txt[]中
        Display_showString(0, 3, txt, U16_WHITE, U16_BLACK, 24); // 将txt中 内容显示出来
        sprintf(txt, "variate:%05d", count);                     // 将变量填充到字符串的对应位置，并将字符串存放到txt[]中
        Display_showString(0, 4, txt, U16_WHITE, U16_BLACK, 32); // 将txt中 内容显示出来

        Delay_Ms(1000); // 延时等待
        Display_CLS(U16_BLACK);
        count++;
        printf("lq %5d",count);
//        Display_Show_Logo(50, 0);
        Display_DrawCircle(50, 50, 20, U16_WHITE);
        Display_DrawLine(80, 80, 20, 20, U16_WHITE);
        Display_DrawRectangle(10, 20, 30, 40, U16_WHITE);

        Delay_Ms(1000); // 延时等待

        Display_CLS(U16_BLACK);
#else
        Delay_Ms(500); // 延时等待
        Display_CLS(U16_BLACK);
        Delay_Ms(200); // 延时等待
        Display_CLS(U16_RED);
        Delay_Ms(200); // 延时等待
        Display_CLS(U16_GREEN);
        Delay_Ms(200); // 延时等待
        Display_CLS(U16_BLUE);
        Delay_Ms(200); // 延时等待
        Display_CLS(U16_WHITE);
        count++;
        sprintf(txt, "variate:%05d", count);                     // 将变量填充到字符串的对应位置，并将字符串存放到txt[]中
        Display_showString(0, 4, txt, U16_WHITE, U16_BLACK, 16); // 将txt中 内容显示出来

#endif
    }
}

/*╔══════════════════════════════════════════════════════════════════════════════╗
 *║                        调试菜单 (MENU_ENABLE=1 时启用)                        ║
 *╚══════════════════════════════════════════════════════════════════════════════╝*/
#if MENU_ENABLE

#define NODES_PER_PAGE  4
#define NODE_START_PAGE 4

static const char *const g_junction_names[] = {
    "ANY", "L90", "R90", "T-junc", "Left-T", "Right-T", "Cross"
};

static const char *const g_action_names[] = {
    "NONE", "TURN_LEFT", "TURN_RIGHT", "GO_STRAIGHT"
};

static uint8_t Menu_GetTotalPages(void)
{
    uint8_t node_pages = 0;
#if PATH_PLAN_ENABLE
    uint8_t len = PathPlan_GetRouteLength();
    node_pages = (len + NODES_PER_PAGE - 1) / NODES_PER_PAGE;
#endif
    return NODE_START_PAGE + node_pages;
}

static void Menu_RenderPage(uint8_t page)
{
    char txt[32];
    uint8_t row = 2;

    switch (page)
    {
    case 0:
        sprintf(txt, "SPEED:%.1f  MAX:%.1f cm/s", (double)SPEED, (double)MAX_SPEED);
        Display_showString(0, row++, txt, U16_WHITE, U16_BLACK, 16);

        sprintf(txt, "TgtL:%.1f TgtR:%.1f cm/s",
                (double)Get_Target_SpeedL(), (double)Get_Target_SpeedR());
        Display_showString(0, row++, txt, U16_WHITE, U16_BLACK, 16);

        sprintf(txt, "TURN_ERR:%.1f px", (double)TURN_ERROR);
        Display_showString(0, row++, txt, U16_WHITE, U16_BLACK, 16);

        sprintf(txt, "OutRatio:%.2f InRatio:%.2f",
                (double)TURN_OUTER_RATIO, (double)TURN_INNER_RATIO);
        Display_showString(0, row++, txt, U16_WHITE, U16_BLACK, 16);

        sprintf(txt, "IncDltMax:%.1f DeadZn:%.1f",
                (double)SPEED_INC_DELTA_MAX, (double)SPEED_DEAD_ZONE);
        Display_showString(0, row++, txt, U16_WHITE, U16_BLACK, 16);

        sprintf(txt, "GyroBaseSpd:%.1f cm/s", (double)Get_GyroBaseSpeed());
        Display_showString(0, row++, txt, U16_WHITE, U16_BLACK, 16);
        break;

    case 1:
        Display_showString(0, row++, "== Speed Loop PID ==", U16_GREEN, U16_BLACK, 16);

        sprintf(txt, "KP:%.4f  KI:%.4f", (double)incpidL.kp, (double)incpidL.ki);
        Display_showString(0, row++, txt, U16_WHITE, U16_BLACK, 16);

        sprintf(txt, "KD:%.4f  Imax:%.1f", (double)incpidL.kd, (double)incpidL.imax);
        Display_showString(0, row++, txt, U16_WHITE, U16_BLACK, 16);
        break;

    case 2:
        Display_showString(0, row++, "== Gyro Loop PID ==", U16_GREEN, U16_BLACK, 16);

        sprintf(txt, "KP:%.2f  KI:%.4f", (double)gyropid.kp, (double)gyropid.ki);
        Display_showString(0, row++, txt, U16_WHITE, U16_BLACK, 16);

        sprintf(txt, "KD:%.2f  Imax:%.1f", (double)gyropid.kd, (double)gyropid.imax);
        Display_showString(0, row++, txt, U16_WHITE, U16_BLACK, 16);
        break;

    case 3:
        Display_showString(0, row++, "== Dir & Angle PID ==", U16_GREEN, U16_BLACK, 16);

        sprintf(txt, "Loc KP:%.2f KI:%.3f", (double)locpid.kp, (double)locpid.ki);
        Display_showString(0, row++, txt, U16_WHITE, U16_BLACK, 16);

        sprintf(txt, "  KD:%.3f Imax:%.1f", (double)locpid.kd, (double)locpid.imax);
        Display_showString(0, row++, txt, U16_WHITE, U16_BLACK, 16);

        sprintf(txt, "Ang KP:%.1f KI:%.3f", (double)anglepid.kp, (double)anglepid.ki);
        Display_showString(0, row++, txt, U16_WHITE, U16_BLACK, 16);

        sprintf(txt, "  KD:%.1f Imax:%.1f", (double)anglepid.kd, (double)anglepid.imax);
        Display_showString(0, row++, txt, U16_WHITE, U16_BLACK, 16);
        break;

    default:
    {
#if PATH_PLAN_ENABLE
        uint8_t base = (page - NODE_START_PAGE) * NODES_PER_PAGE;
        uint8_t len  = PathPlan_GetRouteLength();
        uint8_t end  = base + NODES_PER_PAGE;
        if (end > len) end = len;

        sprintf(txt, "== Nodes (passed:%d/%d) ==", PathPlan_GetNode(), len);
        Display_showString(0, row++, txt, U16_GREEN, U16_BLACK, 16);

        for (uint8_t i = base; i < end; i++)
        {
            uint8_t type = 0, act = 0;
            PathPlan_GetRouteEntry(i, &type, &act);

            const char *tname = (type <= 6) ? g_junction_names[type] : "?";
            const char *aname = (act  <= 3) ? g_action_names[act]   : "?";

            sprintf(txt, "#%02d %-7s %s", i + 1, tname, aname);
            Display_showString(0, row++, txt, U16_WHITE, U16_BLACK, 16);
        }

        if (len > NODES_PER_PAGE)
        {
            uint8_t sub = page - NODE_START_PAGE + 1;
            uint8_t tot = (len + NODES_PER_PAGE - 1) / NODES_PER_PAGE;
            sprintf(txt, "NodePg %d/%d", sub, tot);
            Display_showString(0, row, txt, U16_YELLOW, U16_BLACK, 16);
        }
#else
        Display_showString(0, row++, "PathPlan DISABLED", U16_RED, U16_BLACK, 16);
#endif
        break;
    }
    }
}

void Display_Menu(void)
{
    uint8_t page = 0;
    uint8_t total_pages;

    PIN_InitConfig(P33_5, PIN_MODE_INPUT, 1);

    for (;;)
    {
        total_pages = Menu_GetTotalPages();
        if (total_pages == 0) total_pages = 1;

        Display_CLS(U16_BLACK);

        {
            char header[28];
            sprintf(header, "== CONFIG MENU %d/%d ==", page + 1, total_pages);
            Display_showString(0, 0, header, U16_WHITE, U16_BLACK, 16);
        }

        Display_Fill(0, 16, 239, 17, U16_GREEN);

        Menu_RenderPage(page);

        Display_showString(0, 7, "5:NXT 9:PRV HOLD9:EXIT", U16_YELLOW, U16_BLACK, 16);

        /* ── 按键轮询 ── */
        {
            uint8_t handled = 0;
            Delay_Ms(50);

            while (!handled)
            {
                /* P33_5: 下一页 */
                if (PIN_Read(P33_5) == 0)
                {
                    Delay_Ms(50);
                    if (PIN_Read(P33_5) == 0)
                    {
                        page++;
                        if (page >= total_pages) page = 0;
                        handled = 1;
                        while (PIN_Read(P33_5) == 0) {}
                        Delay_Ms(50);
                    }
                }

                /* P33_9 (KEY1): 短按=上一页, 长按>1s=退出 */
                if (KEY_Read(KEY1) == 0)
                {
                    uint16_t hold = 0;
                    Delay_Ms(50);
                    if (KEY_Read(KEY1) != 0) continue;

                    while (KEY_Read(KEY1) == 0)
                    {
                        Delay_Ms(50);
                        hold += 50;
                        if (hold >= 1000)
                        {
                            while (KEY_Read(KEY1) == 0) {}
                            Delay_Ms(50);
                            Display_CLS(U16_BLACK);
                            return;
                        }
                    }

                    if (page == 0) page = total_pages - 1;
                    else page--;
                    handled = 1;
                    Delay_Ms(50);
                }
            }
        }
    }
}

#endif /* MENU_ENABLE */
