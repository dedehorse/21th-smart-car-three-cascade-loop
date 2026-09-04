//#include "image.h"
//
///**
// * @brief 左边界数组 - 存储每一行检测到的赛道左边界X坐标
// */
//unsigned char left_border[IMAGE_H];
//
///**
// * @brief 右边界数组 - 存储每一行检测到的赛道右边界X坐标
// */
//unsigned char right_border[IMAGE_H];
//
///**
// * @brief 环岛左边界数组 - 存储每一行检测到的环岛左边界X坐标
// */
//unsigned char left_round_border[IMAGE_H];
//
///**
// * @brief 环岛右边界数组 - 存储每一行检测到的环岛右边界X坐标
// */
//unsigned char right_round_border[IMAGE_H];
//
///**
// * @brief 底部边界数组 - 存储每一列检测到的赛道底部边界Y坐标
// */
//unsigned char bottom_border[IMAGE_W];
//
///**
// * @brief 赛道中心线数组 - 存储每一行计算得到的赛道中心线X坐标
// */
//unsigned char center_line[IMAGE_H];
//
///**
// * @brief 环岛中心线数组 - 存储每一行计算得到的环岛中心线X坐标
// */
//unsigned char round_line[IMAGE_H];
//
///**
// * @brief 赛道顶部有效点 - 标记赛道检测到的最顶部有效行
// */
//unsigned char top_point = INVALID_BORDER;
//
///**
// * @brief 赛道底部有效点 - 标记赛道检测到的最底部有效行
// */
//unsigned char bottom_point = INVALID_BORDER;
//
///**
// * @brief 左扫描限制表 - 存储每一行左侧扫描的起始位置
// */
//unsigned char left_limit_table[IMAGE_H];
//
///**
// * @brief 右扫描限制表 - 存储每一行右侧扫描的结束位置
// */
//unsigned char right_limit_table[IMAGE_H];
//
///**
// * @brief X方向区域边界划分
// * @note 将图像宽度(94像素)划分为9个区域，用于区域自适应阈值
// *       区域划分: [0,5), [5,10), [10,20), [20,35), [35,58), [58,73), [73,83), [83,88), [88,93]
// */
//static const uint8_t x_zone_bounds[X_ZONES+1] = {0, 5, 10, 20, 35, 58, 73, 83, 88, 93};
//
///**
// * @brief Y方向区域边界划分
// * @note 将图像高度(60像素)划分为6个区域，用于区域自适应阈值
// *       区域划分: [0,10), [10,20), [20,30), [30,40), [40,50), [50,60)
// */
//static const uint8_t y_zone_bounds[Y_ZONES+1] = {0, 10, 20, 30, 40, 50, 60};
//
///**
// * @brief 区域阈值偏移量表
// * @note 每个区域相对于全局阈值的偏移量，用于区域自适应二值化
// *       正值表示该区域阈值比全局阈值低（更容易判定为白色）
// *       负值表示该区域阈值比全局阈值高（更难判定为白色）
// */
//int16_t zone_threshold_offsets[Y_ZONES][X_ZONES];
//
///**
// * @brief 二值化图像缓冲区
// * @note 存储经过区域自适应二值化处理后的图像数据
// */
//uint8 bin_image[IMAGE_H][IMAGE_W];
//
///**
// * @brief 全局阈值变量
// * @note 通过Otsu算法计算得到的全局二值化阈值
// */
//uint8 image_thereshold = 0;
//
///**
// * @brief 垂直扫描范围下限数组
// * @note 当本行边界检测失败时，向下扫描的最小行数
// *       值越大表示扫描范围越小，值越小表示扫描范围越大
// *       设计原则：越靠近图像底部（j越大），扫描范围越小
// */
//const unsigned char scan_y1[IMAGE_H] = {
//  0, 1, 2, 3, 3, 3, 4, 4, 4, 4,
//  4, 4, 4, 5, 5, 5, 5, 6, 6, 6,
//  6, 6, 7, 7, 7, 8, 8, 8, 8, 9,
//  9, 9, 9, 9, 10, 10, 10, 11, 11, 11,
//  12, 12, 12, 13, 13, 13, 14, 14, 14, 15,
//  15, 16, 16, 16, 17, 17, 17, 17, 18, 18
//};
//
///**
// * @brief 垂直扫描范围上限数组
// * @note 当本行边界检测失败时，向下扫描的最大行数
// *       设计原则：中间区域扫描范围较大，顶部和底部扫描范围较小
// */
//const unsigned char scan_y2[IMAGE_H] = {
//  2, 2, 3, 3, 4, 4, 4, 4, 4, 4,
//  4, 5, 6, 6, 6, 7, 7, 7, 8, 8,
//  8, 9, 9, 9, 10, 10, 11, 11, 12, 12,
//  13, 13, 14, 14, 15, 16, 16, 17, 17, 17,
//  18, 18, 17, 16, 15, 14, 13, 12, 11, 10,
//  9, 8, 7, 6, 5, 4, 3, 2, 1, 0
//};
//
///**
// * @brief 水平扫描宽度下限数组
// * @note 存储每一行扫描宽度的下限值，用于边界检测时的水平范围限制
// *       值表示从当前边界位置向左/右扩展的最小宽度
// */
//const unsigned char scan_x1[IMAGE_H] = {
//     8,  9,  9,  9, 10, 10, 10, 10, 10, 10,
//    10, 11, 11, 12, 12, 12, 12, 12, 12, 12,
//    13, 14, 14, 14, 14, 14, 15, 15, 15, 16,
//    16, 17, 17, 17, 17, 17, 17, 18, 19, 19,
//    19, 19, 19, 20, 20, 20, 21, 21, 21, 22,
//    22, 22, 22, 22, 23, 24, 24, 24, 24, 24
//};
//
///**
// * @brief 水平扫描宽度上限数组
// * @note 存储每一行扫描宽度的上限值，用于边界检测时的水平范围限制
// *       值表示从当前边界位置向左/右扩展的最大宽度
// */
//const unsigned char scan_x2[IMAGE_H] = {
//     9,  9,  9, 10, 10, 10, 10, 10, 11, 11,
//    12, 12, 12, 12, 12, 13, 13, 13, 13, 13,
//    13, 13, 13, 14, 14, 14, 15, 15, 15, 15,
//    16, 16, 17, 17, 17, 18, 18, 18, 19, 19,
//    19, 19, 20, 20, 21, 21, 21, 21, 22, 23,
//    23, 23, 24, 24, 24, 25, 25, 25, 26, 26
//};
//
///**
// * @brief 区域阈值偏移量初始值
// * @note 经过调试优化的区域阈值偏移配置，用于区域自适应二值化
// *       每个区域的偏移量根据实际赛道光照情况调整
// */
//int16_t zone_threshold_offsets[Y_ZONES][X_ZONES] = {
//    {4, 2, 0, 0, 0, 2, 2, 3, 4},
//    {2, 0, 0, 0, 0, 0, 1, 1, 3},
//    {0, 0, 0, 0, 0, 0, 1, 2, 3},
//    {0, 0, 0, 0, 0, 0, 1, 2, 3},
//    {1, 0, 0, 0, 0, 0, 1, 2, 3},
//    {4, 1, 1, 1, 3, 0, 2, 3, 4}
//};
//
//#if 0
//// 备用的区域阈值偏移量配置（实验性）
//int16_t zone_threshold_offsets[Y_ZONES][X_ZONES] = {
//    {-255, -255, -255, 0, 0, 2, 0, -255, -255},
//    {-255, -255, 0, 0, 0, 0, 1, -255, -255},
//    {-255, 0, 0, 0, 0, 0, 1, 4, -255},
//    {0, 0, 0, 0, 0, 0, 1, 4, 5},
//    {1, 0, 0, 0, 0, 0, 1, 4, 5},
//    {4, 1, 1, 1, 3, 0, 4, 5, 6}
//};
//#endif
//
///**
// * @brief 区域二值化查找表
// * @note 三维数组: [y_zone][x_zone][pixel_value] -> binary_value
// *       预计算每个区域每个像素值对应的二值化结果，实现O(1)快速二值化
// */
//static uint8_t zone_lut[Y_ZONES][X_ZONES][256];
//
////------------------------------------------------------------------------------------------------------------------
////  @brief       Otsu阈值计算（最大类间方差法）
////  @details     自动计算图像二值化的最佳阈值，适用于光照变化场景
////  @param[in]   image   输入图像数据指针（一维数组）
////  @param[in]   col     图像宽度
////  @param[in]   row     图像高度
////  @return      计算得到的最佳阈值（0-255）
////------------------------------------------------------------------------------------------------------------------
//uint8 otsuThreshold(uint8* image, uint16 col, uint16 row)
//{
//#define GrayScale 256  // 灰度级数
//
//    uint16 Image_Width = col;   // 图像宽度
//    uint16 Image_Height = row;  // 图像高度
//    int X;
//    uint16 Y;
//    uint8* data = image;        // 图像数据指针
//
//    int HistGram[GrayScale] = {0};  // 灰度直方图数组
//
//    uint32 Amount = 0;              // 总像素数
//    uint32 PixelBack = 0;           // 背景像素数
//    uint32 PixelIntegralBack = 0;   // 背景灰度积分
//    uint32 PixelIntegral = 0;       // 总灰度积分
//    int32 PixelIntegralFore = 0;    // 前景灰度积分
//    int32 PixelFore = 0;            // 前景像素数
//
//    // 类间方差计算相关变量
//    double OmegaBack = 0;   // 背景像素占比
//    double OmegaFore = 0;   // 前景像素占比
//    double MicroBack = 0;   // 背景平均灰度
//    double MicroFore = 0;   // 前景平均灰度
//    double SigmaB = 0;      // 类间方差（最大）
//    double Sigma = 0;       // 当前阈值的类间方差
//
//    uint8 MinValue = 0;     // 最小有效灰度值
//    uint8 MaxValue = 0;     // 最大有效灰度值
//    uint8 Threshold = 0;    // 计算得到的最佳阈值
//
//    // Step 1: 计算灰度直方图
//    for (Y = 0; Y < Image_Height; Y++) {
//        for (X = 0; X < Image_Width; X++) {
//            HistGram[(int)data[Y * Image_Width + X]]++;
//        }
//    }
//
//    // Step 2: 找到最小和最大有效灰度值
//    for (MinValue = 0; MinValue < 255 && HistGram[MinValue] == 0; MinValue++);
//    for (MaxValue = 255; MaxValue > MinValue && HistGram[MaxValue] == 0; MaxValue--);
//
//    // 特殊情况处理：图像只有一种灰度
//    if (MaxValue == MinValue) {
//        return MaxValue;
//    }
//
//    // 特殊情况处理：图像只有两种灰度
//    if (MinValue + 1 == MaxValue) {
//        return MinValue;
//    }
//
//    // Step 3: 计算总像素数和总灰度积分
//    for (Y = MinValue; Y <= MaxValue; Y++) {
//        Amount += HistGram[Y];
//    }
//
//    PixelIntegral = 0;
//    for (Y = MinValue; Y <= MaxValue; Y++) {
//        PixelIntegral += (uint32)HistGram[Y] * Y;
//    }
//
//    // Step 4: 遍历所有可能的阈值，找到类间方差最大的阈值
//    SigmaB = -1;
//    for (Y = MinValue; Y < MaxValue; Y++) {
//        PixelBack += HistGram[Y];                    // 累加背景像素数
//        PixelFore = Amount - PixelBack;              // 前景像素数 = 总像素数 - 背景像素数
//
//        OmegaBack = (double)PixelBack / Amount;      // 背景占比
//        OmegaFore = (double)PixelFore / Amount;      // 前景占比
//
//        PixelIntegralBack += (uint32)HistGram[Y] * Y; // 累加背景灰度积分
//        PixelIntegralFore = PixelIntegral - PixelIntegralBack; // 前景灰度积分
//
//        MicroBack = (double)PixelIntegralBack / PixelBack; // 背景平均灰度
//        MicroFore = (double)PixelIntegralFore / PixelFore; // 前景平均灰度
//
//        // 计算类间方差: Sigma = ω0 * ω1 * (μ0 - μ1)^2
//        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);
//
//        // 更新最大类间方差和对应阈值
//        if (Sigma > SigmaB) {
//            SigmaB = Sigma;
//            Threshold = (uint8)Y;
//        }
//    }
//
//    return Threshold;
//}
//
///**
// * @brief 计算平均Otsu阈值
// * @details 连续采集50帧图像，计算平均Otsu阈值作为全局阈值
// *          用于初始化阶段的阈值校准，提高光照适应性
// */
//void get_av_ot()
//{
//    unsigned int image_thereshold_sum = 0;  // 阈值累加和
//    unsigned char i;
//
//    for (i = 0; i <= 50; i++) {
//        while (mt9v03x_finish_flag == 0);  // 等待图像采集完成
//        image_thereshold_sum += otsuThreshold(mt9v03x_image[0], IMAGE_W, IMAGE_H);
//        mt9v03x_finish_flag = 0;           // 重置采集标志
//    }
//
//    image_thereshold = image_thereshold_sum / 50;  // 计算平均值
//}
//
///**
// * @brief 初始化区域阈值查找表
// * @details 根据全局阈值和区域偏移量初始化LUT
// *          每个区域的实际阈值 = 全局阈值 - 区域偏移量
// */
//void init_zone_lut() {
//    for (int yz = 0; yz < Y_ZONES; yz++) {
//        for (int xz = 0; xz < X_ZONES; xz++) {
//            // 计算该区域的阈值
//            int16_t zone_thresh = image_thereshold - zone_threshold_offsets[yz][xz];
//
//            // 预计算该区域每个像素值的二值化结果
//            for (int i = 0; i < 256; i++) {
//                zone_lut[yz][xz][i] = (i >= zone_thresh) ? White_Point : Black_Point;
//            }
//        }
//    }
//}
//
///**
// * @brief 获取像素所在区域索引
// * @param[in] val       像素坐标值（x或y）
// * @param[in] bounds    区域边界数组
// * @param[in] zone_count 区域数量
// * @return 区域索引（0 ~ zone_count-1）
// */
//static inline uint8_t get_zone_index(uint8_t val, const uint8_t* bounds, uint8_t zone_count) {
//    for (uint8_t z = 0; z < zone_count; z++) {
//        if (val < bounds[z + 1]) return z;
//    }
//    return zone_count - 1;
//}
//
///**
// * @brief 快速区域二值化函数
// * @details 使用预计算的LUT表实现O(1)复杂度的二值化
// *          每个像素根据其所在区域使用不同的阈值
// * @param[in]  src 原始灰度图像
// * @param[out] dst 二值化图像
// */
//void fast_zone_binarize(uint8_t src[IMAGE_H][IMAGE_W], uint8_t dst[IMAGE_H][IMAGE_W]) {
//    // 预计算每一行所在的Y区域
//    uint8_t y_zone[IMAGE_H];
//    for (int y = 0; y < IMAGE_H; y++) {
//        y_zone[y] = get_zone_index(y, y_zone_bounds, Y_ZONES);
//    }
//
//    // 逐行处理
//    for (int y = 0; y < IMAGE_H; y++) {
//        uint8_t* src_row = src[y];
//        uint8_t* dst_row = dst[y];
//        uint8_t yz = y_zone[y];
//
//        // 逐像素二值化
//        for (int x = 0; x < IMAGE_W; x++) {
//            uint8_t xz = get_zone_index(x, x_zone_bounds, X_ZONES);
//            dst_row[x] = zone_lut[yz][xz][src_row[x]];
//        }
//    }
//}
//
///**
// * @brief 计算扫描限制表
// * @details 预计算每一行的左右扫描边界限制，实现梯形扫描窗口
// *          扫描窗口设计：底部窄、顶部宽，符合近小远大的透视原理
// */
//void calculate_limit_table()
//{
//    for (int j = 0; j < IMAGE_H; j++) {
//        // 计算左侧和右侧扫描限制
//        // SCAN_WID为扫描宽度参数，随着行号增加（越靠近底部），扫描范围变窄
//        left_limit_table[j] = SCAN_WID - (40 * j) / 59;
//        right_limit_table[j] = IMAGE_W - 1 - SCAN_WID + (40 * j) / 59;
//
//        // 边界保护
//        left_limit_table[j] = (left_limit_table[j] >= IMAGE_W) ? 0 : left_limit_table[j];
//        right_limit_table[j] = (right_limit_table[j] >= IMAGE_W) ? IMAGE_W - 1 : right_limit_table[j];
//    }
//}
//
//#if 1
//
///**
// * @brief 上次检测到的左边界位置
// * @note 用于动态扫描时的参考位置，实现连续帧之间的边界跟踪
// */
//static unsigned char last_left = IMAGE_W / 4;
//
///**
// * @brief 上次检测到的右边界位置
// * @note 用于动态扫描时的参考位置，实现连续帧之间的边界跟踪
// */
//static unsigned char last_right = IMAGE_W * 3 / 4;
//
///**
// * @brief 动态扫描行边界（带扩展搜索功能）
// * @param[in]  bin_image      二值化图像
// * @param[in]  j              当前行号
// * @param[in]  full_scan      是否全范围扫描（使用limit_table限制）
// * @param[in]  expand_scan    是否扩展扫描（1.5倍宽度）
// * @param[in]  expand_scan_pro 是否高级扩展扫描（2倍宽度，用于环岛）
// * @return     是否成功检测到左右边界
// */
//unsigned char dynamic_scan_row(unsigned char bin_image[IMAGE_H][IMAGE_W],
//                               unsigned char j,
//                               unsigned char full_scan,
//                               unsigned char expand_scan,
//                               unsigned char expand_scan_pro)
//{
//    unsigned char found_left = 0, found_right = 0;  // 边界检测标志
//    unsigned char left_start = 0, left_end = IMAGE_W - 1;    // 左边界扫描范围
//    unsigned char right_start = IMAGE_W - 1, right_end = 0;  // 右边界扫描范围
//
//    /* 确定扫描范围 */
//    if (full_scan) {
//        // 全范围扫描模式 - 使用limit_table限制
//        left_start = left_limit_table[j];
//        left_end = right_limit_table[j];
//        right_start = right_limit_table[j];
//        right_end = left_limit_table[j];
//    } else if (expand_scan) {
//        // 扩展扫描模式(1.5倍范围) + limit_table限制
//        left_start = MAX(left_limit_table[j],
//                   (last_left > 1.5 * scan_x1[j]) ? (last_left - 1.5 * scan_x1[j]) : 0);
//        left_end = MIN(right_limit_table[j],
//                      (last_left + 1.5 * scan_x1[j] < IMAGE_W - 1) ?
//                      (last_left + 1.5 * scan_x1[j]) : IMAGE_W - 1);
//
//        right_start = MIN(right_limit_table[j],
//                         (last_right + 1.5 * scan_x1[j] < IMAGE_W - 1) ?
//                         (last_right + 1.5 * scan_x1[j]) : IMAGE_W - 1);
//        right_end = MAX(left_limit_table[j],
//                       (last_right > 1.5 * scan_x1[j]) ?
//                       (last_right - 1.5 * scan_x1[j]) : 0);
//    } else if (expand_scan_pro) {
//        // 高级扩展扫描模式(2倍范围) + limit_table限制（用于环岛）
//        left_start = MAX(left_limit_table[j],
//                       (last_left > 2 * scan_x1[j]) ? (last_left - 2 * scan_x1[j]) : 0);
//        left_end = MIN(IMAGE_W / 2 - 1 + 20, right_limit_table[j]);
//
//        right_start = MIN(right_limit_table[j],
//                         (last_right + 2 * scan_x1[j] < IMAGE_W - 1) ?
//                         (last_right + 2 * scan_x1[j]) : IMAGE_W - 1);
//        right_end = MAX(IMAGE_W / 2 - 1 - 20, left_limit_table[j]);
//    } else {
//        // 正常扫描模式 + limit_table限制
//        left_start = MAX(left_limit_table[j],
//                        (last_left > scan_x1[j]) ? (last_left - scan_x1[j]) : 0);
//        left_end = MIN(right_limit_table[j],
//                      (last_left + scan_x1[j] < IMAGE_W - 1) ?
//                      (last_left + scan_x1[j]) : IMAGE_W - 1);
//
//        right_start = MIN(right_limit_table[j],
//                         (last_right + scan_x1[j] < IMAGE_W - 1) ?
//                         (last_right + scan_x1[j]) : IMAGE_W - 1);
//        right_end = MAX(left_limit_table[j],
//                       (last_right > scan_x1[j]) ?
//                       (last_right - scan_x1[j]) : 0);
//    }
//
//    /* 左边界扫描 */
//    for (unsigned char i = left_start; i <= left_end && !found_left; i++) {
//        if (i >= IMAGE_W - 1) break;  // 防止越界
//
//        // 检测边界模式: (黑或起始) -> 白 -> 白
//        if ((i == left_start || bin_image[j][i - 1] == Black_Point) &&
//            bin_image[j][i] == White_Point &&
//            bin_image[j][i + 1] == White_Point) {
//            left_border[j] = i;
//            found_left = 1;
//            last_left = i;  // 更新上次检测位置
//        }
//    }
//
//    /* 右边界扫描 */
//    for (unsigned char i = right_start; i >= right_end && !found_right; i--) {
//        if (i < 1) break;  // 防止越界
//
//        // 检测边界模式: (黑或起始) -> 白 -> 白
//        if ((i == right_start || bin_image[j][i + 1] == Black_Point) &&
//            bin_image[j][i] == White_Point &&
//            bin_image[j][i - 1] == White_Point) {
//            right_border[j] = i;
//            found_right = 1;
//            last_right = i;  // 更新上次检测位置
//        }
//    }
//
//    return (found_left && found_right);
//}
//
///**
// * @brief 赛道边界检测主函数
// * @details 从图像底部向上扫描，检测赛道的左右边界
// *          支持多级扩展搜索，提高鲁棒性
// * @return 是否成功检测到赛道边界
// */
//unsigned char detect_border()
//{
//    /* 初始化边界数组 */
//    memset(left_border, INVALID_BORDER, IMAGE_H);
//    memset(right_border, INVALID_BORDER, IMAGE_H);
//    memset(bottom_border, INVALID_BORDER, IMAGE_W);
//
//    top_point = INVALID_BORDER;
//    bottom_point = INVALID_BORDER;
//
//    // 重置上次检测位置为初始值（图像中心区域）
//    last_left = IMAGE_W / 4;
//    last_right = IMAGE_W * 3 / 4;
//
//    /* 阶段1：从底部向上扫描找到第一个有效边界 */
//    unsigned char has_boundary = 0;
//    unsigned char current_row = IMAGE_H - 1;
//
//    while (current_row < IMAGE_H) {
//        if (dynamic_scan_row(bin_image, current_row, 1, 0, 0)) {
//            bottom_point = current_row;  // 记录底部有效行
//            has_boundary = 1;
//            break;
//        }
//        if (current_row == 0) break;
//        current_row--;
//    }
//
//    /* 检查全图是否有有效边界 */
//    if (!has_boundary) {
//        return 0;
//    }
//
//    /* 阶段2：动态向上扫描 */
//    while (current_row < IMAGE_H) {
//        if (!dynamic_scan_row(bin_image, current_row, 0, 0, 0)) {
//            // 本行检测失败，尝试扩展搜索
//            unsigned char scan_lines = scan_y1[current_row];
//            unsigned char found = 0;
//            unsigned char k;
//
//            // 尝试1：1.5倍范围扫描scan_y1行
//            for (k = 1; k <= scan_lines && (current_row - k) < IMAGE_H; k++) {
//                if (dynamic_scan_row(bin_image, current_row - k, 0, 1, 0)) {
//                    current_row -= k;
//                    found = 1;
//                    break;
//                }
//            }
//
//            if (!found) {
//                // 尝试2：高级扩展范围扫描（2倍范围）
//                for (k = 1; k <= scan_lines && (current_row - k) < IMAGE_H; k++) {
//                    if (dynamic_scan_row(bin_image, current_row - k, 0, 0, 1)) {
//                        current_row -= k;
//                        found = 1;
//                        break;
//                    }
//                }
//            }
//
//            if (!found) {
//                // 所有扩展搜索都失败，记录顶部边界并退出
//                top_point = current_row + 1;
//                break;
//            } else {
//                top_point = INVALID_BORDER;
//            }
//        } else {
//            // 本行检测成功，继续向上扫描
//            if (current_row == 0) {
//                top_point = 0;
//                break;
//            }
//            current_row--;
//        }
//    }
//
//    /* 阶段3：检测底部边界（垂直方向） */
//    for (unsigned char i = 0; i < IMAGE_W; i++) {
//        for (unsigned char j = IMAGE_H - 1; j > top_point && j > 1; j--) {
//            // 检测垂直边界模式: (黑或底部) -> 白 -> 白
//            if ((j == IMAGE_H - 1 || bin_image[j][i] == Black_Point) &&
//                bin_image[j - 1][i] == White_Point &&
//                bin_image[j - 2][i] == White_Point) {
//                bottom_border[i] = j;
//                break;
//            }
//        }
//    }
//
//    return has_boundary;
//}
//
//#endif
//
//#if 0
//// 备用的单边界检测实现（简化版）
//unsigned char detect_single_row(unsigned char bin_image[IMAGE_H][IMAGE_W], unsigned char j)
//{
//    int left_limit = left_limit_table[j];
//    int right_limit = right_limit_table[j];
//
//    unsigned char found_left = 0, found_right = 0;
//
//    // 双边扫描：同时从左右向中间扫描
//    for (unsigned char i = left_limit; i < right_limit - 2; i++) {
//        // 左边界检测
//        if (!found_left && (i == left_limit || bin_image[j][i] == 0) &&
//            bin_image[j][i + 1] == 255 && bin_image[j][i + 2] == 255) {
//            left_border[j] = i;
//            found_left = 1;
//            if (found_right) break;
//        }
//
//        // 右边界检测（对称位置）
//        unsigned char ri = right_limit - (i - left_limit);
//        if (!found_right && (ri == right_limit || bin_image[j][ri] == 0) &&
//            bin_image[j][ri - 1] == 255 && bin_image[j][ri - 2] == 255) {
//            right_border[j] = ri;
//            found_right = 1;
//            if (found_left) break;
//        }
//    }
//
//    return (left_border[j] != INVALID_BORDER && right_border[j] != INVALID_BORDER);
//}
//
//// 备用的边界检测主函数
//unsigned char detect_border()
//{
//    unsigned char has_boundary = 0;
//    unsigned char current_row = IMAGE_H - 1;
//
//    memset(left_border, INVALID_BORDER, IMAGE_H);
//    memset(right_border, INVALID_BORDER, IMAGE_H);
//    memset(center_line, INVALID_BORDER, IMAGE_H);
//    memset(round_line, INVALID_BORDER, IMAGE_H);
//    memset(bottom_border, INVALID_BORDER, IMAGE_W);
//    top_point = INVALID_BORDER;
//    bottom_point = INVALID_BORDER;
//
//    // 阶段1：从底部向上找到第一个有效边界
//    while (current_row < IMAGE_H) {
//        if (detect_single_row(bin_image, current_row)) {
//            bottom_point = current_row;
//            has_boundary = 1;
//            break;
//        }
//        if (current_row == 0) break;
//        current_row--;
//    }
//
//    if (!has_boundary) return 0;
//
//    // 阶段2：向上扫描并处理检测失败
//    while (current_row < IMAGE_H) {
//        if (!detect_single_row(bin_image, current_row)) {
//            top_point = current_row;
//            unsigned char scan_lines = scan_y1[current_row];
//            unsigned char found = 0;
//
//            // 扩展扫描
//            for (unsigned char k = 1; k <= scan_lines && (current_row - k) < IMAGE_H; k++) {
//                if (detect_single_row(bin_image, current_row - k)) {
//                    current_row -= k;
//                    top_point = INVALID_BORDER;
//                    found = 1;
//                    break;
//                }
//            }
//            if (!found) break;
//        } else {
//            if (current_row == 0) {
//                top_point = 0;
//                break;
//            }
//            current_row--;
//        }
//    }
//
//    // 阶段3：垂直扫描底部边界
//    for (unsigned char i = 0; i < IMAGE_W; i++) {
//        for (unsigned char j = IMAGE_H - 1; j > top_point && j > 1; j--) {
//            if ((j == IMAGE_H - 1 || bin_image[j][i] == 0) &&
//                bin_image[j - 1][i] == 255 && bin_image[j - 2][i] == 255) {
//                bottom_border[i] = j;
//                break;
//            }
//        }
//    }
//
//    return has_boundary;
//}
//#endif
//
//#if 1
///**
// * @brief 环岛边界检测（独立于赛道边界）
// * @details 在环岛模式下使用不同的检测策略
// *          与普通赛道边界分离处理，便于切换模式
// * @return 是否成功检测到环岛边界
// */
//unsigned char detect_round_border()
//{
//    unsigned char has_boundary = 0;
//
//    // 初始化环岛边界数组
//    memset(left_round_border, INVALID_BORDER, IMAGE_H);
//    memset(right_round_border, INVALID_BORDER, IMAGE_H);
//
//    // 从底部向上逐行扫描
//    for (int j = IMAGE_H - 1; j >= 0; j--) {
//        int left_limit = left_limit_table[j];
//        int right_limit = right_limit_table[j];
//        unsigned char found_left = 0, found_right = 0;
//
//        // 左边界检测（寻找 0->255->255 模式）
//        for (int i = left_limit; i < right_limit - 2; i++) {
//            if (!found_left && (bin_image[j][i] == 0 || i == left_limit) &&
//                bin_image[j][i + 1] == 255 &&
//                bin_image[j][i + 2] == 255) {
//                left_round_border[j] = i + 1;  // 记录第一个白点位置
//                found_left = 1;
//                break;
//            }
//        }
//
//        // 右边界检测（寻找 0->255->255 模式，从右向左扫描）
//        for (int i = right_limit - 1; i >= left_limit + 2; i--) {
//            if (!found_right && (bin_image[j][i] == 0 || i == right_limit - 1) &&
//                bin_image[j][i - 1] == 255 &&
//                bin_image[j][i - 2] == 255) {
//                right_round_border[j] = i - 1;  // 记录第一个白点位置
//                found_right = 1;
//                break;
//            }
//        }
//
//        // 记录有效帧
//        if (found_left && found_right) {
//            has_boundary = 1;
//        }
//    }
//
//    return has_boundary;
//}
//#endif
//
///**
// * @brief 计算赛道中心线
// * @details 根据左右边界计算中心线，并进行有效性校验
// *          同时计算环岛中心线
// */
//void calculate_center_line()
//{
//    // 初始化中心线数组
//    memset(center_line, INVALID_BORDER, IMAGE_H);
//    memset(round_line, INVALID_BORDER, IMAGE_H);
//
//    for (int j = 0; j < IMAGE_H; j++) {
//        // 检查当前行边界是否有效
//        if (left_border[j] != 0xFF &&
//            right_border[j] != 0xFF &&
//            left_border[j] < right_border[j]) {
//
//            // 计算中心线（左右边界的平均值）
//            center_line[j] = (left_border[j] + right_border[j]) / 2;
//            round_line[j] = (left_round_border[j] + right_round_border[j]) / 2;
//
//            // 有效性校验
//            unsigned char road_width = right_border[j] - left_border[j];
//            unsigned char round_width = right_round_border[j] - left_round_border[j];
//
//            // 赛道中心线校验：中心线位置必须是白色，且宽度在合理范围内
//            if (bin_image[j][center_line[j]] == 0 ||
//                road_width > 1.2 * scan_x1[j] ||
//                road_width < scan_x1[j] / 4 + 1) {
//                center_line[j] = INVALID_BORDER;  // 标记为无效
//            }
//
//            // 环岛中心线校验：中心线位置必须是黑色，且宽度在合理范围内
//            if (bin_image[j][round_line[j]] == 255 ||
//                round_width < scan_x1[j] / 2.4 ||
//                round_width > 3 * scan_x2[j] ||
//                j > 50) {
//                round_line[j] = INVALID_BORDER;  // 标记为无效
//            }
//        }
//    }
//}
//
///**
// * @brief 图像处理主函数
// * @details 按顺序执行：二值化 -> 边界检测 -> 环岛边界检测 -> 中心线计算
// *          每帧图像处理的入口函数
// */
//void image_process()
//{
//    fast_zone_binarize(mt9v03x_image, bin_image);  // 区域自适应二值化
//    detect_border();                                // 赛道边界检测
//    detect_round_border();                          // 环岛边界检测
//    calculate_center_line();                        // 中心线计算
//}
