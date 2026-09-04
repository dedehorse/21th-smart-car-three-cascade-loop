//#ifndef __IMAGE_H
//#define __IMAGE_H
//
//#include "zf_common_headfile.h"
//
///**
// * @brief 最大值宏定义
// * @param a 第一个比较值
// * @param b 第二个比较值
// * @return 返回较大的值
// */
//#define MAX(a, b) (((a) > (b)) ? (a) : (b))
//
///**
// * @brief 最小值宏定义
// * @param a 第一个比较值
// * @param b 第二个比较值
// * @return 返回较小的值
// */
//#define MIN(a, b) (((a) < (b)) ? (a) : (b))
//
///**
// * @brief 图像宽度（像素）
// * @note 图像分辨率：94x60
// */
//#define IMAGE_W            94
//
///**
// * @brief 图像高度（像素）
// */
//#define IMAGE_H            60
//
///**
// * @brief 无效边界标记
// * @note 用于标记边界数组中未检测到有效边界的位置
// */
//#define INVALID_BORDER     0xFF
//
///**
// * @brief 黑点像素值（二值化后）
// * @note 对应赛道背景（黑色）
// */
//#define Black_Point        0
//
///**
// * @brief 白点像素值（二值化后）
// * @note 对应赛道引导线（白色）
// */
//#define White_Point        255
//
///**
// * @brief X方向区域划分数量
// * @note 将图像宽度方向划分为9个区域，用于区域自适应阈值
// */
//#define X_ZONES 9
//
///**
// * @brief Y方向区域划分数量
// * @note 将图像高度方向划分为6个区域，用于区域自适应阈值
// */
//#define Y_ZONES 6
//
///**
// * @brief 全局阈值变量
// * @note 通过Otsu算法计算得到的全局二值化阈值
// */
//extern uint8 image_thereshold;
//
///**
// * @brief 左边界数组
// * @note 存储每一行检测到的赛道左边界X坐标
// */
//extern unsigned char left_border[IMAGE_H];
//
///**
// * @brief 右边界数组
// * @note 存储每一行检测到的赛道右边界X坐标
// */
//extern unsigned char right_border[IMAGE_H];
//
///**
// * @brief 环岛左边界数组
// * @note 存储每一行检测到的环岛左边界X坐标（用于环岛模式）
// */
//extern unsigned char left_round_border[IMAGE_H];
//
///**
// * @brief 环岛右边界数组
// * @note 存储每一行检测到的环岛右边界X坐标（用于环岛模式）
// */
//extern unsigned char right_round_border[IMAGE_H];
//
///**
// * @brief 底部边界数组
// * @note 存储每一列检测到的赛道底部边界Y坐标
// */
//extern unsigned char bottom_border[IMAGE_W];
//
///**
// * @brief 赛道中心线数组
// * @note 存储每一行计算得到的赛道中心线X坐标
// */
//extern unsigned char center_line[IMAGE_H];
//
///**
// * @brief 环岛中心线数组
// * @note 存储每一行计算得到的环岛中心线X坐标
// */
//extern unsigned char round_line[IMAGE_H];
//
///**
// * @brief 赛道顶部有效点
// * @note 标记赛道检测到的最顶部有效行
// */
//extern unsigned char top_point;
//
///**
// * @brief 赛道底部有效点
// * @note 标记赛道检测到的最底部有效行
// */
//extern unsigned char bottom_point;
//
///**
// * @brief 二值化图像数组
// * @note 存储经过区域自适应二值化处理后的图像数据
// */
//extern uint8 bin_image[IMAGE_H][IMAGE_W];
//
///**
// * @brief 扫描宽度下限数组
// * @note 存储每一行扫描宽度的下限值，用于边界检测时的范围限制
// */
//extern const unsigned char scan_x1[IMAGE_H];
//
///**
// * @brief 扫描宽度上限数组
// * @note 存储每一行扫描宽度的上限值，用于边界检测时的范围限制
// */
//extern const unsigned char scan_x2[IMAGE_H];
//
///**
// * @brief 垂直扫描范围下限数组
// * @note 当本行检测失败时，向下扫描的最大行数下限
// */
//extern const unsigned char scan_y1[IMAGE_H];
//
///**
// * @brief 垂直扫描范围上限数组
// * @note 当本行检测失败时，向下扫描的最大行数上限
// */
//extern const unsigned char scan_y2[IMAGE_H];
//
///**
// * @brief 计算平均Otsu阈值
// * @note 连续采集50帧图像，计算平均Otsu阈值作为全局阈值
// */
//void get_av_ot();
//
///**
// * @brief 初始化区域阈值查找表
// * @note 根据全局阈值和区域偏移量初始化LUT，用于快速二值化
// */
//void init_zone_lut();
//
///**
// * @brief 计算扫描限制表
// * @note 预计算每一行的左右扫描边界限制，实现梯形扫描窗口
// */
//void calculate_limit_table();
//
///**
// * @brief 图像处理主函数
// * @note 执行二值化、边界检测、中心线计算的完整流程
// */
//void image_process();
//
//#endif
