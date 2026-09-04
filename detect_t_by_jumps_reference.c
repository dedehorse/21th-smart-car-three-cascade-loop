/**
 * ============================================================================
 * 文件名：detect_t_by_jumps_reference.c
 * 功能：基于"跳变点"检测左T/右T路口的参考实现
 * 作者：Copilot AI
 * 日期：2026-06-08
 * 
 * 说明：
 *   本文件是对学长方案的具体代码实现。该方案不使用传统的 check_turn() 中的
 *   "白像素面积比" 判据，而是直接检测T字分支的结构特征：
 *   - 在中线左右侧若干列中查找"双跳变"（白→黑，黑→白）
 *   - 如果多列都发现此特征，判定为左T
 *   - 右T类似
 * 
 * 核心算法：
 *   1. 选定中线左右侧的若干扫描列
 *   2. 对每一列从上向下扫描，查找跳变点
 *   3. 跳变点1（白→黑）：横分支的下边界
 *   4. 跳变点2（黑→白）：主线的左/右边界
 *   5. 如果"双跳变"存在且间隔合理，计数+1
 *   6. 多列投票判定最终结果
 * ============================================================================
 */

#include "LQ_Track.h"

/* ============================================================================
   调试开关和配置
   ============================================================================ */

#define ENABLE_JUMP_DEBUG    1        /* 是否启用调试输出 */
#define ENABLE_JUMP_METHOD   1        /* 是否启用新方法 */

/* 参数配置 */
#define JUMP_SEARCH_START_OFFSET   0   /* 从 top_point 开始 */
#define JUMP_SEARCH_END_RATIO      2   /* 搜索到屏幕的 1/2 处 */
#define JUMP_CONFIRM_ROWS          3   /* 跳变后需要确认的行数 */
#define JUMP_GAP_MIN               4   /* 双跳变最小间隔(行) */
#define JUMP_GAP_MAX              20   /* 双跳变最大间隔(行) */
#define REQUIRED_VALID_COLS        2   /* 至少需要多少列确认T字 */

/* 扫描列的配置（距中线的像素距离） */
static const int scan_distances[] = {2, 4, 6, 8, 10};
#define SCAN_DISTANCES_COUNT  (sizeof(scan_distances) / sizeof(scan_distances[0]))

/* ============================================================================
   全局变量（用于调试和结果记录）
   ============================================================================ */

typedef struct {
    unsigned char first_jump_row;      /* 第一个跳变点行号 */
    unsigned char second_jump_row;     /* 第二个跳变点行号 */
    unsigned char is_valid;            /* 是否有效的双跳变 */
} JumpPair;

static JumpPair left_jump_pairs[SCAN_DISTANCES_COUNT];
static JumpPair right_jump_pairs[SCAN_DISTANCES_COUNT];
static unsigned char left_valid_count = 0;
static unsigned char right_valid_count = 0;

/* ============================================================================
   函数：is_noise_row - 判断某行某列的像素是否是噪声
   
   原理：
     如果某像素的上下相邻行都是黑色，则该像素很可能是噪声
   ============================================================================ */
static unsigned char is_noise_row(unsigned char col, int row, unsigned char pixel_val)
{
    #define NOISE_CHECK_RANGE  2
    
    if (pixel_val == 0) return 0;  /* 黑色像素不算噪声 */
    
    int white_neighbors = 0;
    
    for (int offset = -NOISE_CHECK_RANGE; offset <= NOISE_CHECK_RANGE; offset++) {
        if (offset == 0) continue;
        
        int check_row = row + offset;
        if (check_row >= 0 && check_row < LCDH) {
            if (Pixle[check_row][col] != 0) {
                white_neighbors++;
            }
        }
    }
    
    /* 如果相邻范围内白色像素过少，认为是孤立噪声 */
    return (white_neighbors <= 1) ? 1 : 0;
}

/* ============================================================================
   函数：find_first_jump - 在指定列中查找第一个跳变点（白→黑）
   
   参数：
     col: 列号
     start_row: 搜索起始行
     end_row: 搜索结束行
     
   返回：
     第一个跳变点的行号，未找到返回 0xFF
   
   原理：
     从 start_row 向下扫，找到第一个满足以下条件的行：
       Pixle[row-1][col] != 0 (上一行是白)
       Pixle[row][col] == 0   (当前行是黑)
       不是噪声
   ============================================================================ */
static unsigned char find_first_jump(unsigned char col, unsigned char start_row, unsigned char end_row)
{
    for (int row = start_row + 1; row <= end_row; row++) {
        unsigned char prev_pixel = Pixle[row - 1][col];
        unsigned char curr_pixel = Pixle[row][col];
        
        /* 检测白→黑跳变 */
        if (prev_pixel != 0 && curr_pixel == 0) {
            /* 验证不是噪声 */
            if (!is_noise_row(col, row - 1, prev_pixel)) {
                return (unsigned char)row;
            }
        }
    }
    
    return INVALID_BORDER;
}

/* ============================================================================
   函数：find_second_jump - 在指定列中查找第二个跳变点（黑→白）
   
   参数：
     col: 列号
     start_row: 搜索起始行（第一个跳变点之后）
     end_row: 搜索结束行
     
   返回：
     第二个跳变点的行号，未找到返回 0xFF
   ============================================================================ */
static unsigned char find_second_jump(unsigned char col, unsigned char start_row, unsigned char end_row)
{
    for (int row = start_row; row <= end_row; row++) {
        if (row == 0) continue;
        
        unsigned char prev_pixel = Pixle[row - 1][col];
        unsigned char curr_pixel = Pixle[row][col];
        
        /* 检测黑→白跳变 */
        if (prev_pixel == 0 && curr_pixel != 0) {
            if (!is_noise_row(col, row, curr_pixel)) {
                return (unsigned char)row;
            }
        }
    }
    
    return INVALID_BORDER;
}

/* ============================================================================
   函数：confirm_white_after_first_jump - 确认第一个跳变后仍有白色
   
   原理：
     第一个跳变是"白→黑"，但在这个黑色区域下方应该再次出现白色
     （这代表T字分支的延续）
   ============================================================================ */
static unsigned char confirm_white_after_first_jump(unsigned char col, unsigned char first_jump_row)
{
    for (int offset = 1; offset <= JUMP_CONFIRM_ROWS; offset++) {
        int check_row = first_jump_row + offset;
        
        if (check_row >= LCDH) break;
        
        if (Pixle[check_row][col] != 0) {
            return 1;  /* 确认在黑色区域下方有白色 */
        }
    }
    
    return 0;  /* 黑色区域下方没有白色 */
}

/* ============================================================================
   函数：detect_double_jump_in_column - 在指定列中检测双跳变
   
   参数：
     col: 列号
     
   返回：
     JumpPair 结构体，is_valid=1 表示发现有效的双跳变
     
   左T的双跳变序列：
     行0:  中线贯穿（白）
     ...
     行N:  白→黑（跳变1：横分支下边界）
     行N+1 至 行N+k: 仍有白（分支延续）
     行M:  黑→白（跳变2：主线左边界）
     行M+1 至 底部: 白（主线延续）
   ============================================================================ */
static JumpPair detect_double_jump_in_column(unsigned char col)
{
    JumpPair result;
    result.is_valid = 0;
    result.first_jump_row = INVALID_BORDER;
    result.second_jump_row = INVALID_BORDER;
    
    unsigned char search_start = (top_point != INVALID_BORDER) ? top_point : 0;
    unsigned char search_end = LCDH / JUMP_SEARCH_END_RATIO;
    
    if (search_start >= LCDH) search_start = 0;
    if (search_end >= LCDH) search_end = LCDH - 1;

    /* ===== 第一阶段：寻找第一个跳变点（白→黑） ===== */
    unsigned char first_jump = find_first_jump(col, search_start, search_end);
    
    if (first_jump == INVALID_BORDER) {
        return result;  /* 未找到第一个跳变 */
    }
    
    result.first_jump_row = first_jump;

    /* ===== 第二阶段：确认第一个跳变点下方仍有白色 ===== */
    if (!confirm_white_after_first_jump(col, first_jump)) {
        return result;  /* 跳变点下方没有白色，不是分支 */
    }

    /* ===== 第三阶段：寻找第二个跳变点（黑→白） ===== */
    unsigned char search_start_2 = first_jump + JUMP_GAP_MIN;
    unsigned char search_end_2 = first_jump + JUMP_GAP_MAX;
    
    if (search_end_2 >= LCDH) search_end_2 = LCDH - 1;
    
    unsigned char second_jump = find_second_jump(col, search_start_2, search_end_2);
    
    if (second_jump == INVALID_BORDER) {
        return result;  /* 未找到第二个跳变 */
    }
    
    result.second_jump_row = second_jump;

    /* ===== 第四阶段：验证双跳变间隔 ===== */
    int jump_gap = second_jump - first_jump;
    
    if (jump_gap >= JUMP_GAP_MIN && jump_gap <= JUMP_GAP_MAX) {
        result.is_valid = 1;  /* 确认为有效的双跳变 */
    }

    return result;
}

/* ============================================================================
   函数：scan_left_t_jumps - 在中线左侧扫描T字特征
   
   策略：
     1. 在中线左侧选择若干列（距离为 2, 4, 6, 8, 10 像素）
     2. 对每列检测双跳变
     3. 计数有效列数
     4. 如果有效列数 >= REQUIRED_VALID_COLS，判定为左T
   ============================================================================ */
static unsigned char scan_left_t_jumps(void)
{
    int center_col = LCDW / 2;
    left_valid_count = 0;

    for (int d = 0; d < SCAN_DISTANCES_COUNT; d++) {
        int col = center_col - scan_distances[d];
        
        if (col <= 0) continue;  /* 保证列在有效范围 */

        /* 在这一列中查找双跳变 */
        left_jump_pairs[d] = detect_double_jump_in_column((unsigned char)col);
        
        if (left_jump_pairs[d].is_valid) {
            left_valid_count++;
        }
    }

    return (left_valid_count >= REQUIRED_VALID_COLS) ? 1 : 0;
}

/* ============================================================================
   函数：scan_right_t_jumps - 在中线右侧扫描T字特征（对称） ============================================================================ */
static unsigned char scan_right_t_jumps(void)
{
    int center_col = LCDW / 2;
    right_valid_count = 0;

    for (int d = 0; d < SCAN_DISTANCES_COUNT; d++) {
        int col = center_col + scan_distances[d];
        
        if (col >= LCDW - 1) continue;

        right_jump_pairs[d] = detect_double_jump_in_column((unsigned char)col);
        
        if (right_jump_pairs[d].is_valid) {
            right_valid_count++;
        }
    }

    return (right_valid_count >= REQUIRED_VALID_COLS) ? 1 : 0;
}

/* ============================================================================
   函数：detect_t_junction_by_jumps - 基于跳变点的T字识别主函数
   
   返回值：
     0 = 非T字
     3 = 对称T（左右都有分支）
     4 = 左T（仅左侧有分支）
     5 = 右T（仅右侧有分支）
   ============================================================================ */
unsigned char detect_t_junction_by_jumps(void)
{
#if !ENABLE_JUMP_METHOD
    return 0;
#endif

    unsigned char has_left = scan_left_t_jumps();
    unsigned char has_right = scan_right_t_jumps();

    if (has_left && !has_right) {
        return 4;  /* 左T */
    } else if (has_right && !has_left) {
        return 5;  /* 右T */
    } else if (has_left && has_right) {
        return 3;  /* 对称T */
    }

    return 0;  /* 非T字 */
}

/* ============================================================================
   函数：display_jump_debug_info - 显示调试信息
   
   说明：在屏幕上显示跳变检测的过程信息，用于参数调优
   ============================================================================ */
void display_jump_debug_info(void)
{
#if !ENABLE_JUMP_DEBUG
    return;
#endif

    char buf[64];
    unsigned char t_result = detect_t_junction_by_jumps();

    /* 第一行：T字类型和有效列数 */
    sprintf(buf, "T-Type:%d L-Cols:%d R-Cols:%d",
            t_result, left_valid_count, right_valid_count);
    Display_showString(0, 0, buf, U16_WHITE, U16_BLACK, 12);

    /* 第二行：左侧跳变点位置（第一个有效列的数据） */
    if (left_valid_count > 0) {
        for (int d = 0; d < SCAN_DISTANCES_COUNT; d++) {
            if (left_jump_pairs[d].is_valid) {
                sprintf(buf, "L-Jump1:%d J2:%d Gap:%d",
                        left_jump_pairs[d].first_jump_row,
                        left_jump_pairs[d].second_jump_row,
                        left_jump_pairs[d].second_jump_row - left_jump_pairs[d].first_jump_row);
                Display_showString(0, 1, buf, U16_WHITE, U16_BLACK, 12);
                break;
            }
        }
    }

    /* 第三行：右侧跳变点位置 */
    if (right_valid_count > 0) {
        for (int d = 0; d < SCAN_DISTANCES_COUNT; d++) {
            if (right_jump_pairs[d].is_valid) {
                sprintf(buf, "R-Jump1:%d J2:%d Gap:%d",
                        right_jump_pairs[d].first_jump_row,
                        right_jump_pairs[d].second_jump_row,
                        right_jump_pairs[d].second_jump_row - right_jump_pairs[d].first_jump_row);
                Display_showString(0, 2, buf, U16_WHITE, U16_BLACK, 12);
                break;
            }
        }
    }
}

/* ============================================================================
   函数：check_turn_with_jump_method - 改进的路口判断（集成跳变法）
   
   说明：
     这个函数是 check_turn() 的改进版本，首先用跳变法检测T字，
     如果不是T字再用传统方法检测转弯/直行
     
   参数：
     threshold1, threshold2: 传统方法的面积比阈值（如果启用）
     
   返回值：
     0 - 直行
     1 - 左转弯
     2 - 右转弯
     3 - 对称T或十字
     4 - 左T路口
     5 - 右T路口
   ============================================================================ */
unsigned char check_turn_with_jump_method(float threshold1, float threshold2)
{
    /* 第一步：用跳变法检测T字 */
    unsigned char jump_result = detect_t_junction_by_jumps();
    
    if (jump_result == 4 || jump_result == 5) {
        /* 确认左T或右T */
        if (top_point != INVALID_BORDER && top_point < T_JUNCTION_TOP) {
            return jump_result;
        }
    }
    
    if (jump_result == 3) {
        /* 对称T或十字，需要进一步判断 */
        if (top_point != INVALID_BORDER && top_point < T_JUNCTION_TOP) {
            return 3;  /* 对称T */
        }
        /* 否则可能是十字或其他，继续用传统方法 */
    }

    /* 第二步：如果不是明确的T字，使用传统的 check_turn 方法 */
    /* 这里可以调用原有的 check_turn() 或者实现简化的转弯判断逻辑 */
    
    /* 示例：简化的转弯判断 */
    int center_col = LCDW / 2;
    int left_white_count = 0, right_white_count = 0;
    
    for (int col = 0; col < center_col; col++) {
        for (int row = top_point; row < LCDH / 2; row++) {
            if (Pixle[row][col] != 0) {
                left_white_count++;
                break;
            }
        }
    }
    
    for (int col = center_col; col < LCDW; col++) {
        for (int row = top_point; row < LCDH / 2; row++) {
            if (Pixle[row][col] != 0) {
                right_white_count++;
                break;
            }
        }
    }
    
    if (left_white_count > right_white_count + 5) {
        return 1;  /* 左转 */
    } else if (right_white_count > left_white_count + 5) {
        return 2;  /* 右转 */
    } else {
        return 0;  /* 直行 */
    }
}

/* ============================================================================
   说明和使用指南
   ============================================================================

   1. 集成方式：
      - 在 LQ_Track.c 中添加本文件的内容（或单独编译）
      - 在需要检测T字的地方调用 detect_t_junction_by_jumps()
      - 在主循环中调用 display_jump_debug_info() 进行实时调试

   2. 参数调整：
      修改文件顶部的宏定义：
      - JUMP_CONFIRM_ROWS：增大→更严格，更不易误检；减小→更灵敏
      - JUMP_GAP_MIN/MAX：调整双跳变的间隔范围
      - REQUIRED_VALID_COLS：增大→需要更多列确认；减小→更快判定

   3. 性能优化：
      - 可采用部分列跳过策略减少计算量
      - 可采用ROI（感兴趣区）限制扫描范围
      - 可采用多帧投票机制提高稳定性

   4. 调试步骤：
      a. 启用 ENABLE_JUMP_DEBUG 和 ENABLE_JUMP_METHOD
      b. 在屏幕上观察各场景的 T-Type, L-Cols, R-Cols 值
      c. 记录理想的参数范围
      d. 在特殊场景（阴影、光线不足）下调整参数
      e. 最终确定参数后，关闭调试输出以节省CPU

   5. 已知限制：
      - 无法处理极端斜角的T字（>45度倾斜）
      - 在极端光照不均下可能需要预处理
      - 对于分支被部分遮挡的T字，需要多帧确认

   ============================================================================ */
