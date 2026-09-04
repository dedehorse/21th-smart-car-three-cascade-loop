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

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@编    写：龙邱科技
@E-mail  ：chiusir@163.com
@编译IDE：AURIX Development Studio1.6及以上版本
@使用平台：北京龙邱智能科技TC297TX核心板和母板
@功能介绍：
@相关信息参考下列地址
@网    站：http://www.lqist.cn
@淘宝店铺：http://longqiu.taobao.com
@软件版本：V1.1 版权所有，单位使用请先联系授权
@程序配套视频地址：https://space.bilibili.com/95313236
**************************************************************
摄像头管脚  TC297DA管脚   说明              DMA方式采集所用管脚通过电阻跳线
D0        P02_0       摄像头数据bit0
D1        P02_1       摄像头数据bit1
D2        P02_2       摄像头数据bit2
D3        P02_3       摄像头数据bit3
D4        P02_4       摄像头数据bit4
D5        P02_5       摄像头数据bit5
D6        P02_6       摄像头数据bit6
D7        P02_7       摄像头数据bit7
CLK       P00_4       像素时钟          外部中断第2组：P00_4
VSNC      P15_1       场信号           外部中断第7组：P15_1

SCL       P11_2       兼容模拟SCCB SCL
SDA       P11_3       兼容模拟SCCB SDA
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "LQ_CAMERA.h"
#include <IfxCpu.h>
#include <IfxPort_reg.h>
#include "lq_include.h"
#include <stdio.h>

/** 图像原始数据存放 */
unsigned char Image_Data[IMAGEH][IMAGEW];
/** 压缩后之后用于存放屏幕显示数据  */
unsigned char Image_Use[LCDH][LCDW];
/** 二值化后用于OLED显示的数据 */
unsigned char Pixle[LCDH][LCDW];
/** 二值化阈值 */
unsigned char Threshold = 0;
unsigned char Base_Threshold = 0;

#define OTSU_INTERVAL   1
#define VAR_SAMPLE_X    30
#define VAR_SAMPLE_Y    20
static unsigned int VAR_THRESHOLD = 220;  // 初始化时自动标定，220为默认兜底值

static unsigned int Last_Variance = 0;

static unsigned char calc_variance_threshold(void)
{
    unsigned int sum = 0, sum_sq = 0;
    unsigned int step_x = LCDW / VAR_SAMPLE_X;
    unsigned int step_y = LCDH / VAR_SAMPLE_Y;
    unsigned int count = 0;

    for (int i = 0; i < LCDH; i += step_y)
    {
        for (int j = 0; j < LCDW; j += step_x)
        {
            unsigned char val = Image_Use[i][j];
            sum += val;
            sum_sq += (unsigned int)val * val;
            count++;
        }
    }

    if (count == 0) 
    {
        Last_Variance = 0;
        return Base_Threshold;
    }

    unsigned int mean = sum / count;
    unsigned int variance = sum_sq / count - mean * mean;
    Last_Variance = variance;

    return (variance < VAR_THRESHOLD) ? Base_Threshold : (unsigned char)GetOSTU(Image_Use);
}

unsigned int get_last_variance(void)
{
    return Last_Variance;
}

unsigned int get_var_threshold(void)
{
    return VAR_THRESHOLD;
}

void CameraInt(void)
{
    CAMERA_Init(200);

    int cnt = 0;
    int threshold_sum = 0;
    unsigned int var_sum = 0;
    while (cnt < 50)
    {
        if (Camera_Flag == 2)
        {
            Camera_Flag = 0;
            Get_Use_Image();
            threshold_sum += GetOSTU(Image_Use);
            calc_variance_threshold();
            var_sum += Last_Variance;
            cnt++;
        }
    }
    Base_Threshold = threshold_sum / 50;
    Threshold = Base_Threshold;

    unsigned int avg_var = var_sum / 50;
    VAR_THRESHOLD = avg_var / 3;
    if (VAR_THRESHOLD < 30) VAR_THRESHOLD = 30;

    ScanLimit_Init();
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Test_CAMERA (void)
@功能说明：摄像头测试例程
@参数说明：void
@函数返回：void
@备    注：注意需要使用  带与非门版（白色）转接座，或者新版摄像头模块
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_CAMERA(void)
{
    char txt[50];
    uint32_t frame_count = 0;
    uint32_t last_time = 0;
    uint32 fps = 0;
    GPIO_LED_Init();
    Display_Init(2);
    LED_Ctrl(LED0, RVS);
    Delay_Ms(100);
    Display_CLS(U16_BLACK);
    /* 摄像头初始化 */
    CAMERA_Init(255);  // 初始化帧率
    LED_Ctrl(LED0, RVS);
    // 初始化STM定时器
    STM_InitConfig(STM0, STM_Channel_0, 1000); // 1ms中断
    last_time = STM_GetNowUs(STM0);

    while (1)
    {
//        uint8_t img[LCDH][LCDW];
        if (Camera_Flag == 2)
        {
            /* 提取部分使用的数据 */
//            Get_Use_Image();
            /* 清除摄像头采集完成标志位  如果不清除，则不会再次采集数据 */
            Camera_Flag = 0;
            frame_count++;

#if 1 // 显示原始图像 1/2
#ifdef IPS114
//            for(int y=0;y<60;y++)
//            {
//                for(int x=0;x<94;x++)
//                {
//                    img[y][x] = Image_Use[y][x];
//                }
//            }
//            Display_Show(0, 0, LCDH, LCDW, (unsigned char *)Image_Data);
//            Display_Show(0, 0, LCDH, LCDW, (unsigned char *)Image_Use);
#endif  // IPS114

#else // 显示二值化图像 v

            /* 二值化 */
            Get_Bin_Image();
            Display_Show(0, 0, LCDH, LCDW, (unsigned char *)Pixle);
#endif
//            LED_Ctrl(LED1, RVS);
        }
        
        // 每1秒计算一次帧率
        uint32_t current_time = STM_GetNowUs(STM0);
        if (current_time - last_time >= 1000000) // 1秒
        {
            fps = frame_count;
            frame_count = 0;
            last_time = current_time;
        }
        
        char buf[32];
        sprintf(buf, "time: %07ld\r\n", fps);
        UART_PutStr(WLS_UART_PORT, buf);  // 使用UART3打印
        sprintf(txt, "time:%07ld", fps);                       // 将变量填充到字符串的对应位置，并将字符串存放到txt[]中
        Display_showString(0, 1, txt, U16_WHITE, U16_BLACK, 24); // 将txt中 内容显示出来
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void CAMERA_Reprot (void)
@功能说明：串口上报上位机
@参数说明：void
@函数返回：void
@备    注：注意上位机的帧头可能有所区别
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void CAMERA_Reprot(void)
{
    short j, i;

    UART_PutChar(UART0, 0xfe); // 帧头
    UART_PutChar(UART0, 0xef); // 帧头

    for (i = 0; i < IMAGEH; i++)
    {
        for (j = 0; j < IMAGEW; j++)
        {
            if (Image_Data[i][j] == 0xfe) // 防止错误发送帧尾
            {
                Image_Data[i][j] = 0xff;
            }
            UART_PutChar(UART0, Image_Data[i][j]); // 发送数据
        }
    }
    UART_PutChar(UART0, 0xef); // 帧尾
    UART_PutChar(UART0, 0xfe); // 帧尾
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void CAMERA_Init (unsigned char fps)
@功能说明：摄像头初始化
@参数说明：fps:  帧率
@函数返回：void
@备    注：摄像头的一些参数，在LQ_MT9V034.c中的宏定义中修改
@调用方法：CAMERA_Init(50);   //初始化MT9V034  50帧 V2版的摄像头注意使用白色带与非门版转接座,
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void CAMERA_Init(unsigned char fps)
{
    Delay_Ms(1);
    // 关闭CPU总中断
    IfxCpu_disableInterrupts();

    /* 初始化摄像头 数据IO */
    PIN_InitConfig(P02_0, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P02_1, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P02_2, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P02_3, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P02_4, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P02_5, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P02_6, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P02_7, PIN_MODE_INPUT_PULLDOWN, 0);

    MT9V034_Init(fps);

    /* DMA 触发源初始化 */
    PIN_Exti(P00_4, PIN_IRQ_MODE_FALLING);

    /* DMA 初始化 */
    DMA_CameraInitConfig((unsigned long)(&MODULE_P02.IN.U), (unsigned long)Image_Data, PIN_INT2_PRIORITY);

    /* 使能场中断 */
    PIN_Exti(P15_1, PIN_IRQ_MODE_RISING);

    /* 初始化完成 打开中断 */
    IfxCpu_enableInterrupts();
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Get_Use_Image (void)
@功能说明：把摄像头采集到原始图像，缩放到赛道识别所需大小
@参数说明：无
@函数返回：void
@备    注：IMAGEW为原始图像的宽度，神眼为188，OV7725为320
@          IMAGEH为原始图像的高度，神眼为120，OV7725为240
@调用方法：Get_Use_Image();
@优化说明：直接计算索引映射，避免中间数组复制
@          1. 从原始图像(188x120)截取中间120x120区域
@          2. 等比例缩小到LCDHxLCDW(60x60)
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Get_Use_Image(void)
{
#ifdef LQMT9V034 // 使用神眼
    const unsigned int CROP_WIDTH = 188;   // 截取宽度
    const unsigned int CROP_HEIGHT = 120;  // 截取高度
    
    // 计算截取起始位置：从中心截取120x120区域
    unsigned int crop_start_w = (IMAGEW - CROP_WIDTH) / 2; // (188-120)/2 = 34
    unsigned int crop_start_h = (IMAGEH - CROP_HEIGHT) / 2; // (120-120)/2 = 0
    
    // 计算缩放比例
    unsigned int scale_w = CROP_WIDTH / LCDW;  // 120/60 = 2
    unsigned int scale_h = CROP_HEIGHT / LCDH; // 120/60 = 2
    
    // 直接索引映射：跳过不需要的行和列，避免中间数组
    for (int i = 0; i < LCDH; i++)
    {
        unsigned int src_h = crop_start_h + i * scale_h;
        unsigned int src_w_start = crop_start_w;
        for (int j = 0; j < LCDW; j++)
        {
            unsigned int src_w = src_w_start + j * scale_w;
            Image_Use[i][j] = Image_Data[src_h][src_w];
        }
    }
#endif
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Get_Bin_Image (void)
@功能说明：动态阈值二值化到Pixle[][]，先清零再置白，每隔OTSU_INTERVAL帧刷新阈值
@参数说明：无
@函数返回：无
@调用方法：Get_Bin_Image();
@备    注：使用memset清零后用OTSU动态阈值，仅对大于阈值的像素置255，省去else分支
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Get_Bin_Image(void)
{
    static unsigned char frame_cnt = 0;

    if (frame_cnt == 0)
    {
        Threshold = calc_variance_threshold();
    }
    frame_cnt = (frame_cnt + 1) % OTSU_INTERVAL;

    memset(Pixle, 0, sizeof(Pixle));

    for (int i = 0; i < LCDH; i++)
    {
        for (int j = 0; j < LCDW; j++)
        {
            if (Image_Use[i][j] > Threshold)
            {
                Pixle[i][j] = Image_WHITE;
            }
        }
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Image_Filter(unsigned char threshold)
@功能说明：图像滤波，去除小于阈值的白色连通域（8连通）
@参数说明：threshold-连通域像素数阈值，小于该值的白色区域被置黑
@函数返回：无
@备    注：使用并查集算法，支持8连通域检测，适配LCDHxLCDW尺寸
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Image_Filter(unsigned char threshold)
{
    static unsigned short labels[LCDH][LCDW];
    static unsigned short parent[LCDH * LCDW + 1];
    static unsigned short area[LCDH * LCDW + 1];
    
    memset(labels, 0, sizeof(labels));
    
    unsigned short cur_label = 0;
    
    for (int r = 0; r < LCDH; ++r)
    {
        for (int c = 0; c < LCDW; ++c)
        {
            if (Pixle[r][c] == 0) continue;
            
            unsigned short left   = (c > 0)          ? labels[r][c-1]   : 0;
            unsigned short up     = (r > 0)          ? labels[r-1][c]   : 0;
            unsigned short leftup = (r > 0 && c > 0) ? labels[r-1][c-1] : 0;
            unsigned short rightup = (r > 0 && c < LCDW-1) ? labels[r-1][c+1] : 0;
            
            unsigned short neighbors[4];
            int ncnt = 0;
            if (left)    neighbors[ncnt++] = left;
            if (up)      neighbors[ncnt++] = up;
            if (leftup)  neighbors[ncnt++] = leftup;
            if (rightup) neighbors[ncnt++] = rightup;
            
            if (ncnt == 0)
            {
                cur_label++;
                labels[r][c] = cur_label;
                parent[cur_label] = cur_label;
                area[cur_label] = 0;
            }
            else
            {
                unsigned short min_label = neighbors[0];
                labels[r][c] = min_label;
                
                for (int k = 1; k < ncnt; ++k)
                {
                    unsigned short other = neighbors[k];
                    while (parent[min_label] != min_label) min_label = parent[min_label];
                    while (parent[other] != other) other = parent[other];
                    if (min_label != other)
                    {
                        parent[other] = min_label;
                    }
                }
            }
        }
    }
    
    for (unsigned short i = 1; i <= cur_label; ++i)
    {
        unsigned short root = i;
        while (parent[root] != root) root = parent[root];
        parent[i] = root;
        area[i] = 0;
    }
    
    for (int r = 0; r < LCDH; ++r)
    {
        for (int c = 0; c < LCDW; ++c)
        {
            unsigned short lbl = labels[r][c];
            if (lbl)
            {
                area[parent[lbl]]++;
            }
        }
    }
    
    for (int r = 0; r < LCDH; ++r)
    {
        for (int c = 0; c < LCDW; ++c)
        {
            if (Pixle[r][c] == 0) continue;
            unsigned short lbl = labels[r][c];
            if (area[parent[lbl]] < threshold)
            {
                Pixle[r][c] = 0;
            }
        }
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：short GetOSTU (unsigned char tmImage[LCDH][LCDW])
@功能说明：大津法求阈值大小
@参数说明：tmImage ： 图像数据
@函数返回：无
@调用方法：GetOSTU(Image_Use);//大津法阈值
@备    注：
ostu方法又名最大类间差方法，通过统计整个图像的直方图特性来实现全局阈值T的自动选取，其算法步骤为：
1) 先计算图像的直方图，即将图像所有的像素点按照0~255共256个bin，统计落在每个bin的像素点数量
2) 归一化直方图，也即将每个bin中像素点数量除以总的像素点
3) i表示分类的阈值，也即一个灰度级，从0开始迭代 1
4) 通过归一化的直方图，统计0~i 灰度级的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像
的比例w0，        并统计前景像素的平均灰度u0；统计i~255灰度级的像素(假设像素值在此范围的像素叫做背
景像素)  * 所占整幅图像的比例w1，并统计背景像素的平均灰度u1；
5) 计算前景像素和背景像素的方差 g = w0*w1*(u0-u1) (u0-u1)
6) i++；转到4)，直到i为256时结束迭代
7) 将最大g相应的i值作为图像的全局阈值
缺陷:OSTU算法在处理光照不均匀的图像的时候，效果会明显不好，因为利用的是全局像素信息。
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int GetOSTU(unsigned char tmImage[LCDH][LCDW])
{
    signed short i, j;
    unsigned long Amount = 0;
    unsigned long PixelBack = 0;
    unsigned long PixelIntegralBack = 0;
    unsigned long PixelIntegral = 0;
    signed long PixelIntegralFore = 0;
    signed long PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    signed short MinValue, MaxValue;
    signed short Threshold = 0;
    unsigned char HistoGram[256]; //

    for (j = 0; j < 256; j++)
        HistoGram[j] = 0; // 初始化灰度直方图

    for (j = 0; j < LCDH; j++)
    {
        for (i = 0; i < LCDW; i++)
        {
            HistoGram[tmImage[j][i]]++; // 统计灰度级中每个像素在整幅图像中的个数
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++)
        ; // 获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--)
        ; // 获取最大灰度的值

    if (MaxValue == MinValue)
        return MaxValue; // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue)
        return MinValue; // 图像中只有二个颜色

    for (j = MinValue; j <= MaxValue; j++)
        Amount += HistoGram[j]; //  像素总数

    PixelIntegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j; // 灰度值总数
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];                                              // 前景像素点数
        PixelFore = Amount - PixelBack;                                                    // 背景像素点数
        OmegaBack = (float)PixelBack / Amount;                                             // 前景像素百分比
        OmegaFore = (float)PixelFore / Amount;                                             // 背景像素百分比
        PixelIntegralBack += HistoGram[j] * j;                                             // 前景灰度值
        PixelIntegralFore = PixelIntegral - PixelIntegralBack;                             // 背景灰度值
        MicroBack = (float)PixelIntegralBack / PixelBack;                                  // 前景灰度百分比
        MicroFore = (float)PixelIntegralFore / PixelFore;                                  // 背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore); // 计算类间方差
        if (Sigma > SigmaB)                                                                // 遍历最大的类间方差g //找出最大类间方差以及对应的阈值
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold; // 返回最佳阈值;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void lq_sobel (unsigned char imageIn[LCDH][LCDW],
@                         unsigned char imageOut[LCDH][LCDW], unsigned char Threshold)
@功能说明：基于soble边沿检测算子的一种边沿检测
@参数说明：imageIn（输入数组），imageOut（输出数组，保存的二值化后的边沿信息），Threshold（阈值）
@函数返回：无
@调用方法：lq_sobel(Image_Use, Bin_Image, (unsigned char) Threshold);
@备    注：注意需要使用  带与非门版（白色）转接座，或者新版摄像头模块
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void lq_sobel(unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW], unsigned char Threshold)
{
    /** 卷积核大小 */
    int KERNEL_SIZE = 3;
    int xStart = KERNEL_SIZE / 2;
    int xEnd = LCDW - KERNEL_SIZE / 2;
    int yStart = KERNEL_SIZE / 2;
    int yEnd = LCDH - KERNEL_SIZE / 2;
    int i, j, k;
    int temp[4];
    for (i = yStart; i < yEnd; i++)
    {
        for (j = xStart; j < xEnd; j++)
        {
            /* 计算不同方向梯度幅值  */
            temp[0] = -(int)imageIn[i - 1][j - 1] + (int)imageIn[i - 1][j + 1]   //{{-1, 0, 1},
                      - (int)imageIn[i][j - 1] + (int)imageIn[i][j + 1]          // {-1, 0, 1},
                      - (int)imageIn[i + 1][j - 1] + (int)imageIn[i + 1][j + 1]; // {-1, 0, 1}};

            temp[1] = -(int)imageIn[i - 1][j - 1] + (int)imageIn[i + 1][j - 1]   //{{-1, -1, -1},
                      - (int)imageIn[i - 1][j] + (int)imageIn[i + 1][j]          // { 0,  0,  0},
                      - (int)imageIn[i - 1][j + 1] + (int)imageIn[i + 1][j + 1]; // { 1,  1,  1}};

            temp[2] = -(int)imageIn[i - 1][j] + (int)imageIn[i][j - 1]           //  0, -1, -1
                      - (int)imageIn[i][j + 1] + (int)imageIn[i + 1][j]          //  1,  0, -1
                      - (int)imageIn[i - 1][j + 1] + (int)imageIn[i + 1][j - 1]; //  1,  1,  0

            temp[3] = -(int)imageIn[i - 1][j] + (int)imageIn[i][j + 1]           // -1, -1,  0
                      - (int)imageIn[i][j - 1] + (int)imageIn[i + 1][j]          // -1,  0,  1
                      - (int)imageIn[i - 1][j - 1] + (int)imageIn[i + 1][j + 1]; //  0,  1,  1

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);
            temp[2] = abs(temp[2]);
            temp[3] = abs(temp[3]);

            /* 找出梯度幅值最大值  */
            for (k = 1; k < 4; k++)
            {
                if (temp[0] < temp[k])
                {
                    temp[0] = temp[k];
                }
            }

            if (temp[0] > Threshold)
            {
                imageOut[i][j] = 0;
            }
            else
            {
                imageOut[i][j] = 255;
            }
        }
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void lq_sobelAutoThreshold (unsigned char imageIn[LCDH][LCDW],
@                                      unsigned char imageOut[LCDH][LCDW])
@功能说明：基于soble边沿检测算子的一种自动阈值边沿检测
@参数说明：imageIn（输入数组）imageOut（输出数组，保存的二值化后的边沿信息）
@函数返回：无
@调用方法：lq_sobelAutoThreshold(Image_Use, Bin_Image);  //动态调节阈值
@备    注：注意需要使用  带与非门版（白色）转接座，或者新版摄像头模块
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQLLLLLLLLLLLLQQQQQQQQQQQ*/
void lq_sobelAutoThreshold(unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW])
{
    /** 卷积核大小 */
    int KERNEL_SIZE = 3;
    int xStart = KERNEL_SIZE / 2;
    int xEnd = LCDW - KERNEL_SIZE / 2;
    int yStart = KERNEL_SIZE / 2;
    int yEnd = LCDH - KERNEL_SIZE / 2;
    int i, j, k;
    int temp[4];
    for (i = yStart; i < yEnd; i++)
    {
        for (j = xStart; j < xEnd; j++)
        {
            /* 计算不同方向梯度幅值  */
            temp[0] = -(int)imageIn[i - 1][j - 1] + (int)imageIn[i - 1][j + 1]   //{{-1, 0, 1},
                      - (int)imageIn[i][j - 1] + (int)imageIn[i][j + 1]          // {-1, 0, 1},
                      - (int)imageIn[i + 1][j - 1] + (int)imageIn[i + 1][j + 1]; // {-1, 0, 1}};

            temp[1] = -(int)imageIn[i - 1][j - 1] + (int)imageIn[i + 1][j - 1]   //{{-1, -1, -1},
                      - (int)imageIn[i - 1][j] + (int)imageIn[i + 1][j]          // { 0,  0,  0},
                      - (int)imageIn[i - 1][j + 1] + (int)imageIn[i + 1][j + 1]; // { 1,  1,  1}};

            temp[2] = -(int)imageIn[i - 1][j] + (int)imageIn[i][j - 1]           //  0, -1, -1
                      - (int)imageIn[i][j + 1] + (int)imageIn[i + 1][j]          //  1,  0, -1
                      - (int)imageIn[i - 1][j + 1] + (int)imageIn[i + 1][j - 1]; //  1,  1,  0

            temp[3] = -(int)imageIn[i - 1][j] + (int)imageIn[i][j + 1]           // -1, -1,  0
                      - (int)imageIn[i][j - 1] + (int)imageIn[i + 1][j]          // -1,  0,  1
                      - (int)imageIn[i - 1][j - 1] + (int)imageIn[i + 1][j + 1]; //  0,  1,  1

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);
            temp[2] = abs(temp[2]);
            temp[3] = abs(temp[3]);
            /* 找出梯度幅值最大值  */
            for (k = 1; k < 4; k++)
            {
                if (temp[0] < temp[k])
                {
                    temp[0] = temp[k];
                }
            }
            /* 使用像素点邻域内像素点之和的一定比例    作为阈值  */
            temp[3] = (int)imageIn[i - 1][j - 1] + (int)imageIn[i - 1][j] + (int)imageIn[i - 1][j + 1] + (int)imageIn[i][j - 1] + (int)imageIn[i][j] + (int)imageIn[i][j + 1] + (int)imageIn[i + 1][j - 1] + (int)imageIn[i + 1][j] + (int)imageIn[i + 1][j + 1];

            if (temp[0] > temp[3] / 12.0f)
            {
                imageOut[i][j] = 255;
            }
            else
            {
                imageOut[i][j] = 0;
            }
        }
    }
}
