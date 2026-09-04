///**
//  ******************************************************************************
//  * @file    LQ_Element.c
//  * @brief   赛道元素处理模块实现
//  *          实现轨迹追踪、赛道元素识别（圆环、十字路口等）的相关功能
//  ******************************************************************************
//  */
//
//#include "element.h"
//
//unsigned char ForwardLook = 25;  /**< 前瞻距离参数 */
//
//Point forwardlook_point = {0xFF, 0xFF};  /**< 前瞻点坐标 */
//Point left_point = {0xFF, 0xFF};         /**< 左侧点坐标 */
//Point right_point = {0xFF, 0xFF};        /**< 右侧点坐标 */
//
//float diff         = 0;     /**< 赛道偏差值 */
//float expect_speed = 0;     /**< 期望速度 */
//float expect_angle = 0;     /**< 期望转向角度（左负右正） */
//
//float total_angle;          /**< 累计转向角度 */
//float total_dis;            /**< 累计行驶距离 */
//unsigned char noway_flag = 0;  /**< 无路可走标志 */
//
//
//
//#if 1 // 加权数组参数配置
///**
//  * @brief  加权数组，用于计算赛道偏差时不同行的权重
//  *         越靠近前方的行权重越高，越靠近车的行权重相对较低
//  */
//static const float weight[IMAGE_H] = {
//    /* 0-4行 */        0.3f, 0.3f, 0.3f, 0.5f, 0.5f,      // 图像最上方
//
//    /* 5-9行 */        2.0f, 3.5f, 4.5f, 4.8f, 4.9f,      // 快速上升
//    /* 10行(峰值) */   5.0f,                               // 最高权重
//    /* 11-14行 */      4.9f, 4.8f, 4.5f, 4.2f,            // 平滑下降
//
//    /* 15-19行 */      3.8f, 3.5f, 3.2f, 2.9f, 2.6f,      // 参考区域
//
//    /* 20-24行 */      2.3f, 2.0f, 1.8f, 1.6f, 1.4f,      // 下降区
//    /* 25-29行 */      1.2f, 1.0f, 1.0f, 1.0f, 0.8f,      // 低速区
//
//    /* 30-39行 */      0.8f, 0.8f, 0.8f, 0.8f, 0.8f,      // 稳定区
//                      0.7f, 0.7f, 0.7f, 0.7f, 0.7f,
//
//    /* 40-49行 */      0.6f, 0.6f, 0.6f, 0.6f, 0.6f,      // 接近车辆区
//                      0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
//
//    /* 50-59行 */      0.4f, 0.4f, 0.4f, 0.4f, 0.4f,      // 车辆附近
//                      0.3f, 0.3f, 0.3f, 0.3f, 0.3f
//};
//#endif
//
//
///**
//  * @brief  计算加权偏差值
//  *         从图像底部向上扫描，结合权重数组计算整体赛道偏差
//  * @retval 加权平均后的赛道偏差值
//  */
//float get_weighted_diff() {
//    float diff_sum = 0.0f;
//    int valid_count = 0;
//    const float track_center = 46.5f; // 赛道中心参考位置
//
//    // 从底部向上扫描，充分利用每一行的有效信息
//    for (int j = IMAGE_H - 1; j >= 0; j--) {
//        if (center_line[j] != INVALID_BORDER) {
//            // 计算当前行的偏差并应用权重
//            float offset = (track_center - center_line[j]) * weight[j];
//            if(offset != 0){
//            diff_sum += offset;
//            valid_count++;
//            }
//        }
//    }
//
//    // 如果有有效行，返回平均值；否则返回0
//    if (valid_count > 0) {
//        return diff_sum / valid_count;
//    } else {
//        return 0.0f; // 没有有效中心线数据
//    }
//}
//
///**
//  * @brief  优化直线绘制，只更新必要的点，避免重复
//  * @param  x1: 起点x坐标
//  * @param  y1: 起点y坐标
//  * @param  x2: 终点x坐标
//  * @param  y2: 终点y坐标
//  */
//void Add_Line(int x1, int y1, int x2, int y2) {
//    // 边界检查，防止越界
//    x1 = CLAMP(x1, 0, IMAGE_W - 1);
//    y1 = CLAMP(y1, 0, IMAGE_H - 1);
//    x2 = CLAMP(x2, 0, IMAGE_W - 1);
//    y2 = CLAMP(y2, 0, IMAGE_H - 1);
//
//    // 确保y1 <= y2
//    if (y1 > y2) {
//        SWAP(int, x1, x2);
//        SWAP(int, y1, y2);
//    }
//
//    // 处理水平线
//    if (y1 == y2) {
//        center_line[y1] = (x1 + x2) / 2;
//        return;
//    }
//
//    // 处理垂直线
//    if (x1 == x2) {
//        for (int i = y1; i <= y2; ++i) {
//            center_line[i] = x1;
//        }
//        return;
//    }
//
//    // 一般情况：Bresenham算法优化
//    const int dx = abs(x2 - x1);
//    const int dy = y2 - y1;
//    const int sx = (x1 < x2) ? 1 : -1;
//
//    int err = dx - dy;
//    int x = x1;
//
//    for (int y = y1; y <= y2; ++y) {
//        center_line[y] = x;
//
//        int e2 = 2 * err;
//        if (e2 > -dy) {
//            err -= dy;
//            x += sx;
//        }
//        if (e2 < dx) {
//            err += dx;
//        }
//    }
//}
//
///**
//  * @brief  绘制转向线
//  *         包括垂直线段和斜线两部分
//  * @param  x1: 起点x坐标
//  * @param  y1: 起点y坐标
//  * @param  x2: 终点x坐标
//  * @param  y2: 终点y坐标
//  */
//void Add_turnline(int x1, int y1, int x2, int y2) {
//    // 1. 绘制垂直线段（到顶部方式）
//    x1 = CLAMP(x1, 0, IMAGE_W-1);
//    y1 = CLAMP(y1, 0, IMAGE_H-1);
//    for(int j = 0; j <= y1; j++) {
//        center_line[j] = x1;
//    }
//
//    // 2. 斜线部分，使用优化算法
//    Add_Line(x1, y1, x2, y2);
//}
//
///**
//  * @brief  获取特定类型的特征点
//  * @param  point: 点类型索引
//  * @param  type: 子类型
//  * @retval 获取到的特征点
//  */
//Point get_point(unsigned char point, unsigned char type)
//{
//    Point valid_point = {0xFF,0xFF};
//    unsigned char max_x = 0xFF;
//    unsigned char min_x = 0xFF;
//    unsigned char max_y = 0xFF;
//    unsigned char min_y = 0xFF;
//
//
//
//    switch (point)
//    {
//        case 0:  // 从前瞻点附近查找
//            for(int j = ForwardLook; j > ForwardLook - scan_y1[j]; j--){
//              if(center_line[j] != 0xFF){
//                valid_point.x = center_line[j];
//                valid_point.y = j;
//                return valid_point;
//              }
//            }
//            break;
//
//        case 1:  // 从左侧查找
//            for(int i = STAR_POINT ; i < STAR_POINT + scan_x2[IMAGE_H - 1]; i++){
//              if(bottom_border[i] != 0xFF && i < STAR_POINT + scan_x2[bottom_border[i]]){
//                  valid_point.x = i;
//                  valid_point.y = bottom_border[i];
//                  return valid_point;
//                  }
//            }
//            break;
//
//        case 2:  // 从右侧查找
//            for(int i = IMAGE_W - 1 - STAR_POINT; i > IMAGE_W - 1 - STAR_POINT - scan_x2[IMAGE_H -1]; i--){
//              if(bottom_border[i] != 0xFF && i > IMAGE_W - 1 - STAR_POINT - scan_x2[bottom_border[i]]){
//                  valid_point.x = i;
//                  valid_point.y = bottom_border[i];
//                  return valid_point;
//                  }
//            }
//            break;
//
//        case 3:  // 从圆环线查找极值点
//              if(type == 0) return valid_point;
//              if(type == 1)  // 找最大X点
//              {
//                for(int j = IMAGE_H - 1; j > 0; j--)
//                {
//                  if(round_line[j] != 0xFF)
//                    {
//                      if(max_x <= round_line[j] || max_x == 0xFF)
//                      {
//                        max_x = round_line[j];
//                        max_y = j;
//                       }
//                     }
//                 }
//                 valid_point.x = max_x;
//                 valid_point.y = max_y;
//                 return valid_point;
//              }
//              if(type == 2)  // 找最小X点
//              {
//                for(int j = IMAGE_H - 1; j > 0; j--)
//                {
//                  if(round_line[j] != 0xFF)
//                    {
//                      if(min_x >= round_line[j] || min_x == 0xFF)
//                      {
//                        min_x = round_line[j];
//                        min_y = j;
//                       }
//                     }
//                 }
//                 valid_point.x = min_x;
//                 valid_point.y = min_y;
//                 return valid_point;
//              }
//            break;
//
//
//        case 4:  // 从圆环边界查找
//              if(type == 0) return valid_point;
//              if(type == 1)  // 右侧圆环边界
//              {
//               for (unsigned char j =  0; j < 18; j++)  {
//
//                    if (right_round_border[j] != 0xFF && right_round_border[j] > 25 &&  right_round_border[j] < 93) {
//                      valid_point.x = right_round_border[j] - 1;
//                      valid_point.y = j;
//                      return valid_point;
//                    }
//                  }
//              }
//              if(type == 2)  // 左侧圆环边界
//              {
//               for (unsigned char j =  0; j < 18; j++)  {
//
//                    if (left_round_border[j] != 0xFF && left_round_border[j] < 68 &&  left_round_border[j] > 0) {
//                      valid_point.x = left_round_border[j] + 1;
//                      valid_point.y = j;
//                      return valid_point;
//                    }
//
//                }
//              }
//            break;
//
//        default: return valid_point;
//    }
//    return valid_point;
//}
//
///**
//  * @brief  扫描获取前方点位置，与参考点(46,59)计算cos得到偏差
//  * @param  x: 目标点x坐标
//  * @param  y: 目标点y坐标
//  * @retval 计算得到的偏差值
//  */
//float get_cos(unsigned char x, unsigned char y) {
//    // 固定参考点(图像左下角附近)
//    const unsigned char ref_x = 46;
//    const unsigned char ref_y = 59;
//
//    if(x == 0xFF || y == 0xFF) return 0;
//    else{
//    // 计算坐标差
//    int dx = x - ref_x;  // 水平距离
//    int dy = y - ref_y;  // 垂直距离
//
//    // 计算距离(斜边)
//    float distance = sqrtf(dx*dx + dy*dy);
//
//    // 防止分母为0或垂直距离为0
//    if (distance == 0.0f || dy == 0) {
//        return 1.0f;  // 夹角为0度，cos=1
//    }
//
//    // 计算cos值 = 邻边/斜边 = dx/distance
//    return -45*(dx / distance);
//    }
//}
//
//#if 0
//// 旧版偏差计算函数，已弃用
//float get_diff(unsigned char type)
//{
//   unsigned char L_y = 0;
//   unsigned char R_y = 0;
//   float forward_diff = 0;
//   float left_diff    = 0;
//   float right_diff   = 0;
//
//  switch (type)
//  {
//    case 0:
//       forward_diff = get_cos(forwardlook_point.x,forwardlook_point.y);
//       left_diff  = get_cos(left_point.x,L_y);
//       right_diff = get_cos(right_point.x,R_y);
//       L_y = (left_point.y  > ForwardLook)? left_point.y: 0xFF;
//       R_y = (right_point.y > ForwardLook)? right_point.y: 0xFF;
//
//      if(forwardlook_point.x != 0xFF && forwardlook_point.y != 0xFF && noway_flag != 2) {
//        noway_flag = 0;
//        return forward_diff;
//      } else if(L_y != 0xFF && R_y == 0xFF && noway_flag != 2) {
//        noway_flag = 0;
//        return left_diff;
//      } else if(L_y == 0xFF && R_y != 0xFF && noway_flag != 2) {
//        noway_flag = 0;
//        return right_diff;
//      } else {
//        noway_flag = 1;
//        return 0;
//      }
//      break;
//    case 1:
//        forward_diff = get_cos(forwardlook_point.x,forwardlook_point.y);
//        if(forwardlook_point.x != 0xFF && forwardlook_point.y != 0xFF && noway_flag != 2) {
//        noway_flag = 0;
//        return forward_diff;
//        }else return 0;
//        break;
//  }
//  return 0;
//}
//
//#endif
//
///**
//  * @brief  圆环检测
//  *         统计圆环线上的有效像素点数量
//  * @retval 有效点数量
//  */
//unsigned char check_round()
//{
//  int count = 0;
//  for(int j = 0; j < IMAGE_H; j++)
//  {
//    if(bin_image[j][round_line[j]] == 0 && round_line[j] != 0xFF)
//    {
//      count++;
//    }
//  }
//  return count;
//}
//
///**
//  * @brief  获取圆环类型（左转圆环或右转圆环）
//  * @retval 0-无圆环，1-左转圆环，2-右转圆环
//  */
//unsigned char get_round_type()
//{
//    left_point  = get_point(1,0);
//    right_point = get_point(2,0);
//
//    if(left_point.x != INVALID_BORDER && right_point.x == INVALID_BORDER)
//    {
//      return 1;
//    }
//    if(left_point.x == INVALID_BORDER && right_point.x != INVALID_BORDER)
//    {
//      return 2;
//    }
//    return 0;
//}
//
///**
//  * @brief  直角弯检测工作函数，停止线识别
//  * @param  threshold1: 第一阈值
//  * @param  threshold2: 第二阈值
//  * @retval 1-左转直角弯, 2-右转直角弯, 0-无直角弯
//  */
//unsigned char check_turn(float threshold1  ,float threshold2)
//{
//    // 1. 确定搜索基准行，防止越界
//    int8 base_row =  CLAMP(top_point, 0, IMAGE_H - 1);
//
//    // 2. 从基准向下找有效边界作为基准
//    int8 base_search_start = CLAMP(base_row + scan_y2[base_row], 0, IMAGE_H - 1);
//    int8 base_search_end = IMAGE_H - 1;
//    int8 valid_center = -1;
//    int8 current_row = 0;
//
//    // 寻找第一个有效center_line作为基准
//    for (int8 row = base_search_start; row <= base_search_end; row++) {
//        if (left_border[row] != INVALID_BORDER&& right_border[row] != INVALID_BORDER) {
//            valid_center = (left_border[row] + right_border[row])/2;
//            int8 wid = right_border[row] - left_border[row] ;
//            current_row = row;
//            if(bin_image[row][valid_center] == White_Point&& wid >= 0.2*scan_x1[row] && wid <= 0.42*scan_x1[row]){
//                current_row = row;
//            break;
//            }
//        }
//    }
//
//    // 无有效基准则返回无转弯
//    if (valid_center == -1) return 0;
//
//    int8 search_start = CLAMP(top_point, 0, IMAGE_H - 1);
//    int8 search_end = CLAMP(current_row, 0, IMAGE_H - 1);
//
//
//    // 3. 以基准为中心，向两侧展开扫描
//    unsigned int left_white = 0, right_white = 0;
//    unsigned int left_area = valid_center;          // 左侧扫描区域大小
//    unsigned int right_area = IMAGE_W - valid_center - 1; // 右侧扫描区域大小
//
//    // 4. 扫描左侧区域（从基准到图像左边界）
//    for (int8 i = 0; i < valid_center; i++) {
//        for (int8 k = search_start; k <= search_end; k++) {
//            if (bin_image[k][i] == White_Point&& (bin_image[k][i-1] == White_Point|| bin_image[k][i+1] == White_Point)) {
//                left_white++;
//                break; // 每行只统计一次
//            }
//        }
//    }
//
//    // 5. 扫描右侧区域（从基准到图像右边界）
//    for (int8 i = valid_center + 1; i < IMAGE_W; i++) {
//        for (int8 k = search_start; k <= search_end; k++) {
//            if (bin_image[k][i] == White_Point&& (bin_image[k][i-1] == White_Point|| bin_image[k][i+1] == White_Point)) {
//                right_white++;
//                break; // 每行只统计一次
//            }
//        }
//    }
//    // 6. 计算比例值
//    float left_ratio = (left_area > 0) ? (float)left_white / left_area : 0;
//    float right_ratio = (right_area > 0) ? (float)right_white / right_area : 0;
//
//
//
//    // 7. 判断逻辑，更精细的模式
//    if (left_ratio > threshold1 && right_ratio <= threshold1) {
//        return 1; // 左转直角弯
//    } else if (right_ratio > threshold1 && left_ratio <= threshold1) {
//        return 2; // 右转直角弯
//    } else if (left_ratio > threshold1 && right_ratio > threshold1&& (left_ratio < threshold2 || right_ratio < threshold2)) {
//        // 当两侧都超阈值时，选择比例更高的一侧
//        return (left_ratio > right_ratio) ? 1 : 2;
//    } else if (left_ratio > threshold2 && threshold2) {
//
//        return 0;
//    }
//    return 0; // 无直角弯
//}
//
//#if 0
///**
//  * @brief  优化转弯检测函数（旧版，已弃用）
//  * @param  type: 搜索方式，0-使用top_point，1-固定位置
//  * @retval 1-左转, 2-右转, 0-无转弯
//  */
//unsigned char check_turn(unsigned char type)
//{
//    // 1. 确定搜索j点位置
//    int8 j = (type == 0) ? top_point- 1 : 15;
//    j = CLAMP(j, 0, IMAGE_W - 1);
//
//    // 2. 预处理扫描范围
//    const int8 left_start = scan_x2[j];
//    const int8 left_end = left_start + scan_x2[j];
//    const int8 right_start = IMAGE_W - 1 - scan_x2[j];
//    const int8 right_end = right_start - scan_x2[j];
//
//    // 3. 优化扫描方式 - 使用更小的内存
//    unsigned char l_turn_num = 0, r_turn_num = 0;
//
//    // 左转弯检测优化
//    for (int8 i = left_start; i < left_end && i < IMAGE_W; i++) {
//        for (int8 k = j; k <= j +  0.8*scan_y2[top_point] ; k++) {
//            if (k >= 0 && k < IMAGE_H && bin_image[k][i] == White_Point) {
//                l_turn_num++;
//                break;  // 只要有一行检测到就退出
//            }
//        }
//    }
//
//    // 右转弯检测优化
//    for (int8 i = right_start; i > right_end && i >= 0; i--) {
//        for (int8 k = j; k <= j + 0.8*scan_y2[top_point]; k++) {
//            if (k >= 0 && k < IMAGE_H && bin_image[k][i] == White_Point) {
//                r_turn_num++;
//                break;  // 只要有一行检测到就退出
//            }
//        }
//    }
//
//    // 4. 判断结果
//    if (l_turn_num > r_turn_num && l_turn_num > 0.3*scan_x2[j]) {
//        return 1;  // 左转
//    } else if (r_turn_num > l_turn_num && r_turn_num > 0.3*scan_x2[j]) {
//        return 2;  // 右转
//    }
//    return 0;      // 无转弯
//}
//#endif
//
///**
//  * @brief  路径决策函数
//  *         根据检测到的弯道类型决定行驶路径
//  * @param  type: 决策类型
//  * @retval 0-直行，1-左转，2-右转
//  */
//unsigned char path_decision(unsigned char type)
//{
// static unsigned char path = 0;
// unsigned char turn_flag = check_turn(0.35,0.8);
//
// switch (type)
// {
//  case 0:
//  switch (path)
//  {
//    case 0:
//     if(turn_flag == 1 && top_point > TOP_POINT){
//       Add_turnline(ADD_LINE,top_point  ,46,59);
//       path = 1;
//       return 1;
//     }
//     if(turn_flag == 2 && top_point > TOP_POINT) {
//       Add_turnline(IMAGE_W - 1 - ADD_LINE, top_point ,47,59);
//       path = 2;
//       return 2;
//     }
//      return 0;
//      break;
//
//    case 1:
//           if(top_point < 3 || top_point == INVALID_BORDER|| check_turn(0.2,0.8)!=1){
//       path = 0;
//       return 0;
//     }
//     Add_turnline(ADD_LINE,top_point ,46,59);
//     return 1;
//     break;
//
//    case 2:
//       if(top_point < 3 || top_point == INVALID_BORDER||check_turn(0.2,0.8)!=2){
//       path = 0;
//       return 0;
//      }
//      Add_turnline(IMAGE_W - 1 - ADD_LINE, top_point ,47,59);
//    return 2;
//    break;
//  }
//  return 0;
//  break;
//
// case 1:
//   if(top_point> TOP_POINT){
//       Add_turnline(ADD_LINE ,top_point- 5  ,46,59);
//       return 1;
//     }
//   break;
//
// case 2:
//   if(top_point> TOP_POINT){
//       Add_turnline(IMAGE_W - 1 - ADD_LINE ,top_point- 5  ,47,59);
//       return 2;
//     }
//   break;
// }
// return 0;
//}
//
///**
//  * @brief  速度决策函数
//  *         根据赛道情况动态调整行驶速度
//  */
//void speed_decision()
//{
//#if 1
//  if(top_point != INVALID_BORDER){expect_speed = SPEED_MAX - 0.3*fabs(diff) - 0.01*fabs(diff*diff) - 0.5*top_point- 0.015*top_point*top_point;
//  expect_speed = (expect_speed < SPEED_MIN)?SPEED_MIN:expect_speed;
//  } else {
//    expect_speed = SPEED_MIN;
//  }
//#endif
//#if 0
//  if(top_point>5 || fabs(diff)>12)
//    expect_speed = SPEED_MIN;
//  else expect_speed= SPEED_MAX;
//#endif
//}
//
///**
//  * @brief  赛道元素处理主函数
//  *         状态机实现，处理直道、圆环、十字路口等各种赛道元素
//  */
//void element_proess()
//{
// static unsigned char ELEMENT    = 0;  /**< 当前元素状态 */
// static unsigned char round_type = 0;  /**< 圆环类型 */
// static unsigned char round_stap = 0;  /**< 圆环处理阶段 */
// static unsigned char round_max  = 0;  /**< 圆环检测最大值 */
// static unsigned char noway_stap = 0;  /**< 无路状态阶段 */
//
// Point round_point1= {0xFF};
// Point round_point2= {0xFF};
//
// speed_decision();
//
//  if(ELEMENT>4) ELEMENT = 0;
//  switch (ELEMENT)
//  {
//
//    case 0:  // 直道状态
//      round_type = get_round_type();
//#if 1 // 圆环检测
//      if(check_round() > ROUND_CHECK1 && top_point < ROUND_CHECK2 && fabs(diff) < ROUND_CHECK3&& get_round_type())
//      {
//        if(round_type)
//        {
//          ELEMENT = 1;  // 进入圆环处理
//          round_stap = 0;
//          round_max = 0;
//          ResetDisZero();
//        } else ELEMENT = 0;
//      }
//#endif
//#if CROSS // 十字路口检测
//      else if(top_point < left_point.y && top_point < right_point.y && top_point != INVALID_BORDER && left_point.x != INVALID_BORDER && right_point.x != INVALID_BORDER)
//      {
//         ELEMENT = 2;  // 进入十字路口处理
//      }
//#endif
//#if NOWAY // 无路检测
//      else if(top_point== INVALID_BORDER)
//      {
//        ELEMENT = 3;  // 进入无路处理
//        noway_stap = 0;
//        ResetDisZero();
//      }
//#endif
//
//        path_decision(0);
//        diff = get_weighted_diff();
//
//      break; // 直道
//
//    case 1:  // 圆环处理状态
//
//      switch(round_stap)
//      {
//
//      case 0:  // 圆环进入阶段
//        if(round_max < check_round()) round_max = check_round();
//        round_point1 = get_point(3,round_type);
//
//        if(round_point1.x != 0xFF && round_point1.y != 0xFF)
//        {
//        Add_turnline(round_point1.x + 2*(3 - 2*round_type) ,round_point1.y,46,59);
//        diff = get_weighted_diff();
//        }
//        else diff = 0;
//
//        if(round_point1.y > ForwardLook + 20 || total_dis> 60 || top_point > ROUND_CHECK2) ELEMENT = 0;
//        else if(round_point1.y > ForwardLook && round_point1.y != 0xFF)
//        {
//           ResetYawZero();
//           ResetDisZero();
//           if(ROUND){
//             round_stap = 1;
//           } else  round_stap = 3;
//        }
//        break;
//
//#if 0
//           case 1:
//
//            if(total_dis > 50 || round_max < ROUND_CHECK4 ) {
//              ELEMENT = 0;
//              expect_angle = 0;
//            }
//            else if(round_type == 1 && expect_angle == 0)
//            {
//              expect_angle = -30;
//            }
//            else if(round_type == 2 && expect_angle== 0)
//            {
//              expect_angle = 30;
//            }
//            if(fabs(total_dis) >= 35 && !check_round()) {
//              expect_angle = 0;
//              round_stap = 2;
//            }
//            break;
//#endif
//
//#if 1
//            case 1:  // 圆环绕行阶段
//            if(total_dis > 65 || round_max < ROUND_CHECK4 ) ELEMENT = 0;
//            else if(round_type == 1)
//            {
//              for(int j = ForwardLook- 5;j < 59; j++)
//                if(left_round_border[j] != 0xFF && left_round_border[j] < 38)
//                {
//                  Add_turnline(left_round_border[j] - 5,j,46,59);
//                  diff = get_weighted_diff();
//                  break;
//                }
//                else diff = 0;
//            }
//            else
//            {
//              for(int j = ForwardLook-5;j < 59; j++)
//                if(right_round_border[j] != 0xFF && right_round_border[j] > 54)
//                {
//                  Add_turnline(right_round_border[j] + 5,j,47,59);
//                  diff = get_weighted_diff();
//                   break;
//                }
//                else diff = 0;
//            }
//            if(fabs(total_angle) >= 50 && !check_round()) round_stap = 2;
//
//            break;
//#endif
//#if 0
//           if(total_dis > 65 || round_max < ROUND_CHECK4 ) ELEMENT = 0;
//           if(round_type == 1) diff = 35;
//           else diff = -35;
//           if(fabs(total_angle) >= 32 && !check_round()) round_stap = 2;
//#endif
//        case 2:  // 圆环出弯阶段
//
//            path_decision(round_type);
//            diff = get_weighted_diff();
//
//            if(fabs(total_angle) >= 305 &&  check_round())
//            {
//              round_stap = 3;
//            }
//            if(total_dis > 320 || fabs(total_angle) > 360 || round_max < 15) ELEMENT = 0;
//            break;
//
//        case 3:  // 圆环收尾阶段
//
//            round_point2 = get_point(4,round_type);
//            if(round_point2.y != 0xFF)
//            {
//              Add_turnline(round_point2.x,1,46,59);
//            }
//            diff = get_weighted_diff();
//            if((!check_round()  || total_dis >  50) && !ROUND)
//              {
//                round_stap = 0;
//                ELEMENT = 0;
//              } else if(!check_round() && fabs(total_angle) >= 332 && top_point<3 || total_dis > 400 || fabs(total_angle) > 380) {
//                round_stap = 0;
//                ELEMENT = 0;
//              }
//            break;
//       }
//       break; // 圆环
//
//    case 2:  // 十字路口处理
//            if(top_point != INVALID_BORDER)
//            Add_turnline(center_line[top_point + 3],top_point,46,59);
//            diff = get_weighted_diff();
//            left_point  = get_point(1,0);
//            right_point = get_point(2,0);
//            if(left_point.x == INVALID_BORDER && right_point.x == INVALID_BORDER)
//            ELEMENT = 0;
//      break; // 十字
//
//    case 3:  // 无路处理
//
//      switch (noway_stap)
//      {
//          case 0:
//
//            path_decision(0);
//            diff = get_weighted_diff();
//
//            if(total_dis > NOWAY_CHECK2 && top_point == 0xFF) {
//              noway_stap = 1;
//              ResetDisZero();
//            }
//            else if(top_point != 0xFF) {
//              ELEMENT = 0;
//              noway_stap = 0;
//            }
//            break;
//
//          case 1:
//
//                race_started = false;
//                break;
//
//      }
//        break; // 无路
//
//       case 4:
//
//         break;
//  }
//}


/* ================================================================== */
/*                    电阻元素识别模块 (新增)                          */
/* ================================================================== */
/*
 * 基于边线统计量(cha/var/std)的电阻元素检测
 *
 * 五阶段实测数据标定:
 *   Stage1(远处直道): cha~0.9, var~1.0, std~1.0
 *   Stage2(接近中):   cha~1.3, var~2.6, std~1.8
 *   Stage3(进入):     cha~2.5, var~7.8, std~2.8
 *   Stage4(内部):     cha~4.4, var~18.8, std~4.4
 *   Stage5(中心):     cha~5.4, var~30.4, std~5.5
 */

#include "LQ_Element.h"
#include <math.h>
#include <stdint.h>
#include <string.h>

#define RESISTOR_ENABLE        1

#if RESISTOR_ENABLE

#define R_VAR_ENTER           10.0f    /* var阈值: 进入电阻区 */
#define R_VAR_EXIT            5.0f     /* var阈值: 离开电阻区 (迟滞) */
#define R_STD_ENTER           3.2f     /* std阈值: 进入电阻区 */
#define R_STD_EXIT            2.0f     /* std阈值: 离开电阻区 (迟滞) */
#define R_CONFIRM_FRAMES      3        /* 连续确认帧数: 防误检 */
#define R_EXIT_FRAMES         5        /* 连续离开帧数: 确认离开 */

#define R_TOPPOINT_THRESHOLD     5     /* top_point > 此值: 前方有断点 */
#define R_CENTER_STD_THRESHOLD   1.0f  /* center_std < 此值: 中心线直 → 电阻 */

static ResistorState_t resistor_state = RES_NONE;
static uint16_t res_confirm_cnt = 0;
static uint16_t res_exit_cnt = 0;
static float res_peak_var = 0.0f;

static float _res_avg_var(double vl, double vr)
{
    return (float)((vl + vr) / 2.0);
}

static float _res_avg_std(double sl, double sr)
{
    return (float)((sl + sr) / 2.0);
}

static float _res_avg_cha(double cl, double cr)
{
    return (float)((cl + cr) / 2.0);
}

unsigned char Resistor_Check(double cha_l, double cha_r,
                             double var_l, double var_r,
                             double std_l, double std_r,
                             unsigned char top_point,
                             double center_std)
{
    float avg_var = _res_avg_var(var_l, var_r);
    float avg_std = _res_avg_std(std_l, std_r);
    float avg_cha = _res_avg_cha(cha_l, cha_r);

    if (avg_var > res_peak_var)
        res_peak_var = avg_var;

    /* 交叉验证: top_point > 5 且 center_std >= 1.0 → 真实路口/转弯，非电阻 */
    unsigned char is_junction = (top_point > R_TOPPOINT_THRESHOLD &&
                                 center_std >= R_CENTER_STD_THRESHOLD);

    switch (resistor_state)
    {
    case RES_NONE:
        if (is_junction)
        {
            /* 前方有断点且中心线在偏移 → 不可能是电阻，重置计数 */
            res_confirm_cnt = 0;
        }
        else if (avg_var > R_VAR_ENTER || avg_std > R_STD_ENTER)
        {
            /* top_point > 5 时增加确认帧数，防止路口误触发 */
            unsigned char needed = (top_point > R_TOPPOINT_THRESHOLD)
                                   ? R_CONFIRM_FRAMES + 4 : R_CONFIRM_FRAMES;
            res_confirm_cnt++;
            if (res_confirm_cnt >= needed)
            {
                resistor_state = RES_APPROACHING;
                res_confirm_cnt = 0;
                printf("[RES] APPROACHING var=%.1f std=%.1f cha=%.1f\r\n",
                       avg_var, avg_std, avg_cha);
            }
        }
        else
        {
            res_confirm_cnt = 0;
        }
        break;

    case RES_APPROACHING:
        if (is_junction)
        {
            /* 中心线在偏移 → 快速退出，减少确认帧数 */
            res_exit_cnt += 2;
            if (res_exit_cnt >= R_EXIT_FRAMES)
            {
                resistor_state = RES_NONE;
                res_exit_cnt = 0;
                res_peak_var = 0.0f;
                printf("[RES] EXIT(junction) peak_var=%.1f\r\n", res_peak_var);
            }
        }
        else if (avg_var > R_VAR_ENTER * 1.5f && avg_std > R_STD_ENTER * 1.3f)
        {
            resistor_state = RES_INSIDE;
            printf("[RES] INSIDE var=%.1f std=%.1f\r\n", avg_var, avg_std);
        }
        else if (avg_var < R_VAR_EXIT && avg_std < R_STD_EXIT)
        {
            res_exit_cnt++;
            if (res_exit_cnt >= R_EXIT_FRAMES)
            {
                resistor_state = RES_NONE;
                res_exit_cnt = 0;
                res_peak_var = 0.0f;
                printf("[RES] EXIT peak_var=%.1f\r\n", res_peak_var);
            }
        }
        else
        {
            res_exit_cnt = 0;
        }
        break;

    case RES_INSIDE:
        if (is_junction)
        {
            /* 进入路口区域 → 直接退到 EXITING */
            resistor_state = RES_EXITING;
            printf("[RES] EXITING(junction) var=%.1f std=%.1f\r\n", avg_var, avg_std);
        }
        else if (avg_var < R_VAR_ENTER && avg_std < R_STD_ENTER)
        {
            resistor_state = RES_EXITING;
            printf("[RES] EXITING var=%.1f std=%.1f\r\n", avg_var, avg_std);
        }
        break;

    case RES_EXITING:
        if (is_junction)
        {
            /* 路口区域 → 加速退出 */
            res_exit_cnt += 2;
        }
        if (avg_var < R_VAR_EXIT && avg_std < R_STD_EXIT)
        {
            res_exit_cnt++;
            if (res_exit_cnt >= R_EXIT_FRAMES)
            {
                resistor_state = RES_NONE;
                res_exit_cnt = 0;
                printf("[RES] GONE peak_var=%.1f\r\n", res_peak_var);
                res_peak_var = 0.0f;
            }
        }
        else if (avg_var > R_VAR_ENTER && !is_junction)
        {
            resistor_state = RES_INSIDE;
            res_exit_cnt = 0;
        }
        else
        {
            res_exit_cnt = 0;
        }
        break;

    default:
        resistor_state = RES_NONE;
        break;
    }

    return (unsigned char)resistor_state;
}

const char* Resistor_StateName(unsigned char state)
{
    switch (state)
    {
    case RES_NONE:      return "NONE";
    case RES_APPROACHING: return "APPROACH";
    case RES_INSIDE:    return "INSIDE";
    case RES_EXITING:   return "EXITING";
    default:            return "???";
    }
}

void Resistor_Reset(void)
{
    resistor_state = RES_NONE;
    res_confirm_cnt = 0;
    res_exit_cnt = 0;
    res_peak_var = 0.0f;
}

const char* Element_GetName(unsigned char turn_flag, unsigned char res_state)
{
    if (res_state != RES_NONE)
        return "RES";

    switch (turn_flag)
    {
    case 0:  return "STR";
    case 1:  return "L90";
    case 2:  return "R90";
    case 3:  return "T-JN";
    case 4:  return "L-TJ";
    case 5:  return "R-TJ";
    case 6:  return "CROS";
    default: return "---";
    }
}

#endif
