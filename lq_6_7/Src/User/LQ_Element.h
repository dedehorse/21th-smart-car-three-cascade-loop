/**
  ******************************************************************************
  * @file    LQ_Element.h
  * @brief   赛道元素处理模块头文件
  *          包含电阻元素识别、元素名称获取等相关定义和声明
  ******************************************************************************
  */

#ifndef __ELEMENT_H
#define __ELEMENT_H

#include <stdint.h>

/**
 * @brief  电阻状态枚举
 */
typedef enum {
    RES_NONE = 0,             /* 无电阻 */
    RES_APPROACHING = 1,      /* 接近中 (提前减速) */
    RES_INSIDE = 2,           /* 在电阻区域内 */
    RES_EXITING = 3           /* 正在离开 */
} ResistorState_t;

/**
 * @brief  电阻元素检测
 * @param  cha_l: 左侧cha统计量
 * @param  cha_r: 右侧cha统计量
 * @param  var_l: 左侧var统计量
 * @param  var_r: 右侧var统计量
 * @param  std_l: 左侧std统计量
 * @param  std_r: 右侧std统计量
 * @param  top_point: 顶端中心点位置
 * @param  center_std: 中心线std值
 * @retval 电阻状态 (ResistorState_t)
 */
unsigned char Resistor_Check(double cha_l, double cha_r,
                             double var_l, double var_r,
                             double std_l, double std_r,
                             unsigned char top_point,
                             double center_std);

/**
 * @brief  获取当前识别到的元素名称
 * @param  turn_flag: 转弯/路口标志 (0=直行, 1=左转, 2=右转, 3=T字, 4=左T, 5=右T)
 * @param  res_state: 电阻状态
 * @retval 元素名称字符串指针
 */
const char* Element_GetName(unsigned char turn_flag, unsigned char res_state);

#endif
