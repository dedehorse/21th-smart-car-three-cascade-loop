# 左T/右T路口识别改进方案

## 1. 当前方案及问题

### 1.1 当前识别逻辑

当前 `check_turn()` 函数的T字路口判定流程：

```
双方白像素比都高 (left_ratio > threshold1 && right_ratio > threshold1)
    ├─ top_point < T_JUNCTION_TOP(10)
    │   ├─ left_ratio > right_ratio * 1.15  → 返回4 (左T)
    │   ├─ right_ratio > left_ratio * 1.15  → 返回5 (右T)
    │   └─ 否则                              → 返回3 (对称T)
    └─ top_point ≥ T_JUNCTION_TOP
        └─ 按比例退化为转弯或十字
```

**核心判据**：仅依赖 `top_point`（上顶点行号）+ 左右白像素面积比。

### 1.2 存在的问题

| 问题 | 说明 |
|------|------|
| **左T/右T区分不准** | 仅靠 `left_ratio` vs `right_ratio` 的1.15倍差异区分，在图像噪声下极易误判 |
| **T字路口 vs 转弯混淆** | 左T路口（`-|`形）左侧有横向分支，右侧仅有纵向主线，但当前逻辑只看白像素面积，无法区分"横向白"和"纵向白" |
| **top_point 阈值脆弱** | `T_JUNCTION_TOP=10` 是硬编码阈值，不同光照/赛道条件下 top_point 波动大 |

### 1.3 根本原因

**左T路口 `-|` 的几何特征**：
- 上方：中心线贯穿到顶（top_point 低）
- **左侧**：存在横向分支，白线一直延伸到左边界
- **右侧**：只有纵向主线，白线不延伸到右边界

**右T路口 `|-` 的几何特征**：
- 上方：中心线贯穿到顶（top_point 低）
- **左侧**：只有纵向主线，白线不延伸到左边界
- **右侧**：存在横向分支，白线一直延伸到右边界

当前方案只统计了**白像素面积**，丢失了**白像素的空间分布信息**（横向 vs 纵向），因此无法区分。

---

## 2. 学长建议分析

学长提出两个思路：

### 思路A：延伸顶点概念 — 引入"左顶点"和"右顶点"

> "把上顶点的概念延伸到左右顶点，如果上顶点和左顶点都小于一定的值且右顶点大于一定的值，则判断是左T字路口"

**解读**：
- **上顶点 (top_point)**：已有，表示中心线在上方能延伸到的最高行号
- **左顶点 (left_point)**：新增，表示白线在**左侧**能延伸到的最左列号（越小表示越靠左）
- **右顶点 (right_point)**：新增，表示白线在**右侧**能延伸到的最右列号（越大表示越靠右）

**判定规则**：
- 左T路口 `-|`：上顶点低（中心线贯穿）+ 左顶点低（横向分支延伸到左边）+ 右顶点不高（右侧无分支）
- 右T路口 `|-`：上顶点低（中心线贯穿）+ 右顶点高（横向分支延伸到右边）+ 左顶点不低（左侧无分支）

### 思路B：边界框交点检测

> "拿一个比画面小一点的框框，如果上边界和左边界和白线有交点，但右边界没交点，就是左T字路口"

**解读**：
- 定义一个内缩矩形框（比画面略小）
- 检测白线与框的四条边是否有交点
- 左T路口：上边界有交点 + 左边界有交点 + 右边界无交点
- 右T路口：上边界有交点 + 右边界有交点 + 左边界无交点

---

## 3. 推荐方案：思路A（左顶点 + 右顶点）

### 3.1 为什么选思路A

| 对比项 | 思路A（左右顶点） | 思路B（边界框交点） |
|--------|-------------------|---------------------|
| 实现复杂度 | 低，复用现有边界检测框架 | 中，需额外定义框和交点检测 |
| 鲁棒性 | 高，基于连续边界统计 | 中，对框大小敏感 |
| 与现有代码兼容性 | 极好，只需在 `detect_border` 中新增两个变量 | 一般，需新增检测逻辑 |
| 计算量 | 小，在已有边界扫描中顺便计算 | 中，需额外遍历边界像素 |

### 3.2 具体实现方案

#### 3.2.1 新增变量定义

在 `LQ_Track.h` 中添加：

```c
#define LEFT_POINT_THRESHOLD   15   /* 左顶点阈值: 左边界最小列号低于此值→左侧有分支 */
#define RIGHT_POINT_THRESHOLD  78   /* 右顶点阈值: 右边界最大列号高于此值→右侧有分支 */
                                     /* (LCDW=94时, 94-1-15=78) */
```

在 `LQ_Track.c` 中添加全局变量：

```c
unsigned char left_point  = INVALID_BORDER;  /* 左顶点: 所有行左边界的最小列号 */
unsigned char right_point = INVALID_BORDER;  /* 右顶点: 所有行右边界的最大列号 */
```

#### 3.2.2 在 `detect_border()` 中计算左右顶点

在现有的 `detect_border()` 函数中，遍历左右边界时顺便统计极值：

```c
void detect_border(void)
{
    unsigned char j, y1;

    memset(left_border,  INVALID_BORDER, LCDH);
    memset(right_border, INVALID_BORDER, LCDH);
    top_point    = INVALID_BORDER;
    bottom_point = INVALID_BORDER;
    left_point   = INVALID_BORDER;   /* 新增 */
    right_point  = 0;                /* 新增 */
    last_left  = LCDW / 4;
    last_right = LCDW * 3 / 4;

    for (j = LCDH - 1; j < LCDH; j--) {
        if (scan_row(j)) {
            if (bottom_point == INVALID_BORDER) bottom_point = j;
            top_point = j;

            /* 新增: 更新左右顶点 */
            if (left_border[j] != INVALID_BORDER) {
                if (left_point == INVALID_BORDER || left_border[j] < left_point)
                    left_point = left_border[j];
            }
            if (right_border[j] != INVALID_BORDER) {
                if (right_border[j] > right_point)
                    right_point = right_border[j];
            }
        } else {
            /* ... 原有的 scan_y1 回退逻辑 ... */
            y1 = scan_y1[j];
            for (unsigned char k = 1; k <= y1 && k <= j; k++) {
                if (scan_row(j - k)) {
                    if (bottom_point == INVALID_BORDER) bottom_point = j - k;
                    top_point = j - k;

                    /* 新增: 回退扫描时也更新左右顶点 */
                    if (left_border[j - k] != INVALID_BORDER) {
                        if (left_point == INVALID_BORDER || left_border[j - k] < left_point)
                            left_point = left_border[j - k];
                    }
                    if (right_border[j - k] != INVALID_BORDER) {
                        if (right_border[j - k] > right_point)
                            right_point = right_border[j - k];
                    }

                    j -= k;
                    break;
                }
                if (k == y1 || k == j) {
                    top_point = j;
                    return;
                }
            }
            if (y1 == 0) { top_point = j; return; }
        }
    }
}
```

#### 3.2.3 改进 `check_turn()` 的T字路口判定逻辑

```c
} else if (left_ratio > threshold1 && right_ratio > threshold1) {
    /* 双方都有白像素 → 路口特征 */
    if (top_point < T_JUNCTION_TOP) {
        /* top_point低 → 中心线贯穿至顶端 → T字路口特征 */

        /* 使用左右顶点区分左T/右T */
        unsigned char has_left_branch  = (left_point  < LEFT_POINT_THRESHOLD)  ? 1 : 0;
        unsigned char has_right_branch = (right_point > RIGHT_POINT_THRESHOLD) ? 1 : 0;

        if (has_left_branch && !has_right_branch) {
            return 4;   /* 左T路口 (-|): 左侧有横向分支, 右侧无 */
        }
        if (has_right_branch && !has_left_branch) {
            return 5;   /* 右T路口 (|-): 右侧有横向分支, 左侧无 */
        }
        if (has_left_branch && has_right_branch) {
            return 3;   /* 对称T字路口: 两侧都有分支 */
        }
        /* 两侧都无分支 → 退化为普通转弯 */
        return (left_ratio > right_ratio) ? 1 : 2;
    }
    /* top_point高 → 中心线已断开 → 转弯/十字特征 */
    /* ... 后续逻辑不变 ... */
}
```

### 3.3 判定逻辑对比

#### 改进前

```
左T/右T区分依据: left_ratio vs right_ratio 的1.15倍差异
问题: 白像素面积比无法反映空间分布
```

#### 改进后

```
左T/右T区分依据: left_point(左顶点) 和 right_point(右顶点) 的位置

场景分析:
┌─────────────────────┐  ┌─────────────────────┐  ┌─────────────────────┐
│     左T路口 (-|)     │  │     右T路口 (|-)     │  │    对称T路口         │
│  ┌──────────┐       │  │       ┌──────────┐  │  │  ┌──────────────┐   │
│  │  白线延伸 │  中心线│  │  │中心线  │  白线延伸 │  │  │  白线   白线  │   │
│  │  到左边界 │   │   │  │  │  │     │  到右边界 │  │  │  到左   到右  │   │
│  └──────────┘   │   │  │  │  │     └──────────┘  │  └──────────────┘   │
│                 │   │  │  │  │                    │         │           │
│                 │   │  │  │  │                    │         │           │
└─────────────────────┘  └─────────────────────┘  └─────────────────────┘
  left_point: 很低(≈0)     left_point: 不低(≈30)     left_point: 很低(≈0)
  right_point: 不高(≈60)   right_point: 很高(≈93)    right_point: 很高(≈93)
  → has_left=1, has_right=0 → has_left=0, has_right=1 → has_left=1, has_right=1
  → 判定: 左T(4)           → 判定: 右T(5)            → 判定: 对称T(3)
```

### 3.4 阈值选取指南

| 参数 | 含义 | 建议值(LCDW=94) | 建议值(LCDW=160) | 调参依据 |
|------|------|-----------------|------------------|----------|
| `LEFT_POINT_THRESHOLD` | 左边界最小列号低于此值→左侧有分支 | 15 | 25 | 观察直行时 left_point 的典型值，取其一半左右 |
| `RIGHT_POINT_THRESHOLD` | 右边界最大列号高于此值→右侧有分支 | LCDW-1-15=78 | LCDW-1-25=134 | 与左阈值对称 |
| `T_JUNCTION_TOP` | top_point低于此值→中心线贯穿 | 10 | 10 | 保持不变 |

**调参方法**：
1. 在 `DisplayTrack()` 中临时显示 `left_point` 和 `right_point` 的值
2. 分别在直行、左T、右T场景下记录典型值
3. 根据数据设置阈值，确保各类场景之间有足够间隔

### 3.5 可选增强：思路B作为辅助验证

如果思路A在某些边界情况下仍不够鲁棒，可以叠加思路B作为辅助：

```c
/* 边界框交点检测 (辅助) */
#define BOX_MARGIN  10  /* 内缩边距 */

static unsigned char box_intersect_top(void) {
    /* 检测白线与上边界(row=BOX_MARGIN)是否有交点 */
    for (int i = BOX_MARGIN; i < LCDW - BOX_MARGIN; i++) {
        if (Pixle[BOX_MARGIN][i] != 0) return 1;
    }
    return 0;
}

static unsigned char box_intersect_left(void) {
    /* 检测白线与左边界(col=BOX_MARGIN)是否有交点 */
    for (int j = BOX_MARGIN; j < LCDH - BOX_MARGIN; j++) {
        if (Pixle[j][BOX_MARGIN] != 0) return 1;
    }
    return 0;
}

static unsigned char box_intersect_right(void) {
    /* 检测白线与右边界(col=LCDW-1-BOX_MARGIN)是否有交点 */
    for (int j = BOX_MARGIN; j < LCDH - BOX_MARGIN; j++) {
        if (Pixle[j][LCDW - 1 - BOX_MARGIN] != 0) return 1;
    }
    return 0;
}
```

融合判定：
```c
/* 思路A + 思路B 融合 */
unsigned char left_branch  = has_left_branch  || box_intersect_left();
unsigned char right_branch = has_right_branch || box_intersect_right();
```

---

## 4. 实施步骤

| 步骤 | 内容 | 涉及文件 |
|------|------|----------|
| 1 | 在 `LQ_Track.h` 添加 `LEFT_POINT_THRESHOLD`、`RIGHT_POINT_THRESHOLD` 宏 | LQ_Track.h |
| 2 | 在 `LQ_Track.c` 添加 `left_point`、`right_point` 全局变量 | LQ_Track.c |
| 3 | 修改 `detect_border()` 在边界扫描中计算 `left_point` 和 `right_point` | LQ_Track.c |
| 4 | 修改 `check_turn()` 中T字路口判定逻辑，使用左右顶点替代面积比 | LQ_Track.c |
| 5 | 在 `DisplayTrack()` 中临时显示 `left_point` 和 `right_point` 用于调参 | LQ_Track.c |
| 6 | 实车测试，记录各场景数据，微调阈值 | - |
| 7 | (可选) 叠加思路B边界框交点检测增强鲁棒性 | LQ_Track.c |

---

## 5. 预期效果

- **左T路口 `-|`**：左侧有横向分支 → `left_point` 很低 → 准确识别为左T(4)
- **右T路口 `|-`**：右侧有横向分支 → `right_point` 很高 → 准确识别为右T(5)
- **对称T路口**：两侧都有分支 → 两个条件都满足 → 识别为对称T(3)
- **普通转弯**：仅一侧有白像素面积变化，但边界不延伸到边缘 → 不会误判为T字路口
- **直行**：左右边界都在中间区域 → 两个条件都不满足 → 正确识别为直行(0)
