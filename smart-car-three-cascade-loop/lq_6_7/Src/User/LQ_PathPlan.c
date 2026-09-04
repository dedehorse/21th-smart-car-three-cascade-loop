/**
  ******************************************************************************
  * @file    LQ_PathPlan.c
  * @brief   节点法路径规划模块实现
  *          状态机: PP_NONE → (检测路口+查表) → PP_TURNING → (路口消失) → PP_NONE
  ******************************************************************************
  */

#include "LQ_PathPlan.h"
#include "lq_include.h"

/* ══════════════════════════════════════════════════════════════════════════
 * 参数 (可在 config.h 中覆盖)
 * ══════════════════════════════════════════════════════════════════════════ */
#ifndef PP_EXIT_FRAMES
#define PP_EXIT_FRAMES       8     /* 退出确认帧数: 连续N帧无路口→确认已通过  */
#endif
#ifndef PP_LOCK_TIMEOUT
#define PP_LOCK_TIMEOUT     100    /* 元素锁超时帧数: 超过此值→跳过该节点     */
#endif
#ifndef PP_TURNING_TIMEOUT
#define PP_TURNING_TIMEOUT  300    /* 转弯超时帧数: 超过此值→强制退出          */
#endif
#ifndef PP_COOLDOWN
#define PP_COOLDOWN          10    /* 退出后冷却帧数: 防止紧邻路口重复计数     */
#endif

#define ARRAY_LEN(a) ((uint8_t)(sizeof(a) / sizeof((a)[0])))

/* ══════════════════════════════════════════════════════════════════════════
 * 全局变量 — path_decision() 每帧读取
 * ══════════════════════════════════════════════════════════════════════════ */
uint8_t g_pp_action    = ACT_NONE;
uint8_t g_pp_lock_type = 0;        /* 0=不锁, 1-6=只接受该类型 */

/* ══════════════════════════════════════════════════════════════════════════
 * 内部状态
 * ══════════════════════════════════════════════════════════════════════════ */
static PathPlanState_t pp_state       = PP_NONE;
static uint8_t         pp_node_count  = 0;       /* 已通过的路口数            */
static uint8_t         pp_route_index = 0;       /* 路由表当前索引            */
static uint8_t         pp_action      = ACT_NONE; /* 当前节点规划动作          */
static uint8_t         pp_lock_type   = 0;       /* 当前节点元素锁            */
static uint16_t        pp_frame_cnt   = 0;       /* 当前状态已持续帧数        */
static uint8_t         pp_exit_cnt    = 0;       /* 连续退出条件满足帧数      */
static uint8_t         pp_cooldown_cnt = 0;      /* 冷却计数                  */

/* ══════════════════════════════════════════════════════════════════════════
 * 路由表 — 根据实际赛道配置 (expected_type, action)
 *
 * expected_type: 期望路口类型 (1=L90,2=R90,3=T字,4=左T,5=右T,6=十字, 0=任意)
 * action:        ACT_TURN_LEFT(左转), ACT_TURN_RIGHT(右转),
 *                ACT_GO_STRAIGHT(直行忽略路口)
 *
 * 【使用方法】:
 *   跑一圈记录每个路口类型 → 决定每个路口怎么走 → 填入下表
 *
 *   node_count 从0开始, 检测到第1个路口→node_count=1→查 route[node_count%length]
 *   多圈自动循环(取模)
 * ══════════════════════════════════════════════════════════════════════════ */

/* ══════════════════════════════════════════════════════════════════════════
 * 路由表: 按实际赛道配置, 每行 = {期望路口类型, 规划动作}
 *
 * 路口类型:  1=L90左直角, 2=R90右直角, 3=对称T, 4=左T, 5=右T, 6=十字, 0=任意
 * 规划动作:  ACT_TURN_LEFT(左拉线), ACT_TURN_RIGHT(右拉线), ACT_GO_STRAIGHT(直行)
 *
 * node_count 从0开始, 检测到第1个路口→node_count=1→查 route[node_count%length]
 * 多圈自动循环(取模)
 * ══════════════════════════════════════════════════════════════════════════ */

/* ── 0: 414路线 (15节点) ── */
static const RouteNode_t route_414_nodes[] = {
    /* {期望类型, 动作}                        说明                     */
    {1, ACT_TURN_LEFT},      /*  1: 左直角    → 左转                 */
    {4, ACT_TURN_LEFT},      /*  2: 左T       → 左转(左拉线)         */
    {5, ACT_TURN_RIGHT},     /*  3: 右T       → 右转(右拉线)         */
    {4, ACT_TURN_LEFT},      /*  4: 左T       → 左转(左拉线)         */
    {3, ACT_TURN_RIGHT},     /*  5: 对称T     → 右转(右拉线) ★决策点  */
    {2, ACT_TURN_RIGHT},     /*  6: 右直角    → 右转                 */
    {2, ACT_TURN_RIGHT},     /*  7: 右直角    → 右转                 */
    {5, ACT_TURN_RIGHT},     /*  8: 右T       → 右转(右拉线)         */
    {1, ACT_TURN_LEFT},      /*  9: 左直角    → 左转                 */
    {4, ACT_TURN_LEFT},      /* 10: 左T       → 左转(左拉线)         */
    {3, ACT_TURN_RIGHT},     /* 11: 对称T     → 右转(右拉线) ★决策点  */
    {5, ACT_TURN_RIGHT},     /* 12: 右T       → 右转(右拉线)         */
    {5, ACT_GO_STRAIGHT},    /* 13: 右T       → 直行通过   ★决策点    */
    {3, ACT_TURN_LEFT},      /* 14: 对称T     → 左转(左拉线) ★决策点  */
    {1, ACT_TURN_LEFT},      /* 15: 左直角    → 左转                 */
};

/* ── 1: 旧8节点路线 (保留备用) ── */
static const RouteNode_t route_8_nodes[] = {
    {1, ACT_TURN_LEFT},      /* 第1个: 左直角(L90) → 自然左转        */
    {4, ACT_GO_STRAIGHT},    /* 第2个: 左T → 直行通过                */
    {4, ACT_TURN_LEFT},      /* 第3个: 左T → 拐进岔路(左拉线左转)    */
    {6, ACT_TURN_RIGHT},     /* 第4个: 十字 → 右转(右拉线)           */
    {3, ACT_TURN_LEFT},      /* 第5个: 对称T字 → 左转(左拉线)        */
    {4, ACT_GO_STRAIGHT},    /* 第6个: 左T → 直行通过                */
    {4, ACT_GO_STRAIGHT},    /* 第7个: 左T → 直行通过                */
    {1, ACT_TURN_LEFT},      /* 第8个: 左直角(L90) → 自然左转        */
};

/* ── 2: 无锁路线 (expected_type=0, 只按顺序匹配) ── */
static const RouteNode_t route_nolock_nodes[] = {
    {0, ACT_TURN_LEFT},      /* 第1个: 任意 → 左转                  */
    {0, ACT_GO_STRAIGHT},    /* 第2个: 任意 → 直行                  */
    {0, ACT_TURN_RIGHT},     /* 第3个: 任意 → 右转                  */
    {0, ACT_TURN_LEFT},      /* 第4个: 任意 → 左转                  */
    {0, ACT_GO_STRAIGHT},    /* 第5个: 任意 → 直行                  */
    {0, ACT_TURN_RIGHT},     /* 第6个: 任意 → 右转                  */
    {0, ACT_TURN_LEFT},      /* 第7个: 任意 → 左转                  */
    {0, ACT_GO_STRAIGHT},    /* 第8个: 任意 → 直行                  */
    {0, ACT_TURN_LEFT},      /* 第9个: 任意 → 左转                  */
    {0, ACT_TURN_RIGHT},     /* 第10个: 任意 → 右转                 */
    {0, ACT_GO_STRAIGHT},    /* 第11个: 任意 → 直行                 */
    {0, ACT_TURN_LEFT},      /* 第12个: 任意 → 左转                 */
};

#define MAX_ROUTE_COUNT 4

static const PathPlan_t g_routes[MAX_ROUTE_COUNT] = {
    { route_414_nodes,   ARRAY_LEN(route_414_nodes)   },  /* 0 = 414路线(主)  */
    { route_8_nodes,     ARRAY_LEN(route_8_nodes)     },  /* 1 = 旧8节点路线  */
    { route_nolock_nodes, ARRAY_LEN(route_nolock_nodes) }, /* 2 = 无锁路线    */
    { NULL, 0 },                                          /* 3 = 预留        */
};

/* 默认使用414路线 */
static const PathPlan_t *current_plan = &g_routes[0];
static uint8_t current_route_index = 0;

/* ══════════════════════════════════════════════════════════════════════════
 * API 实现
 * ══════════════════════════════════════════════════════════════════════════ */

/**
 * @brief  初始化路径规划模块
 */
void PathPlan_Init(void)
{
    pp_state        = PP_NONE;
    pp_node_count   = 0;
    pp_route_index  = 0;
    pp_action       = ACT_NONE;
    pp_lock_type    = 0;
    pp_frame_cnt    = 0;
    pp_exit_cnt     = 0;
    pp_cooldown_cnt = 0;

    g_pp_action    = ACT_NONE;
    g_pp_lock_type = 0;
}

/**
 * @brief  切换到指定路线
 * @param  index: 路线索引
 */
void PathPlan_SetRoute(uint8_t index)
{
    if (index < MAX_ROUTE_COUNT &&
        g_routes[index].nodes != NULL &&
        g_routes[index].length > 0)
    {
        current_plan = &g_routes[index];
        current_route_index = index;
        PathPlan_Init();  /* 重置所有状态 */
    }
}

/**
 * @brief  获取当前已通过路口数 (即 node_count)
 */
uint8_t PathPlan_GetNode(void)
{
    return pp_node_count;
}

/**
 * @brief  获取当前状态机状态
 */
uint8_t PathPlan_GetState(void)
{
    return (uint8_t)pp_state;
}

/**
 * @brief  获取状态名 (调试用)
 */
const char* PathPlan_GetStateName(void)
{
    switch (pp_state) {
    case PP_NONE:    return "NONE";
    case PP_TURNING: return "TURN";
    default:         return "???";
    }
}

/**
 * @brief  获取路线数量
 */
uint8_t PathPlan_GetRouteCount(void)
{
    return MAX_ROUTE_COUNT;
}

uint8_t PathPlan_GetRouteLength(void)
{
    return (current_plan) ? current_plan->length : 0;
}

uint8_t PathPlan_GetRouteEntry(uint8_t index, uint8_t *expected_type, uint8_t *action)
{
    if (!current_plan || index >= current_plan->length) return 0;
    *expected_type = current_plan->nodes[index].expected_type;
    *action        = current_plan->nodes[index].action;
    return 1;
}

/**
 * @brief  每帧调用, 推进路径规划状态机
 * @param  stable_turn: get_stable_turn() 输出的稳定路口类型 (0-6)
 * @param  pd_ret:      path_decision() 的返回值 (0=无路口处理, 非0=正在处理路口)
 *
 * @note   必须在 path_decision() 之后, s_turn_flag 计算之后调用
 *         本函数更新 g_pp_action / g_pp_lock_type, 供下一帧 path_decision() 读取
 */
void PathPlan_Process(uint8_t stable_turn, uint8_t pd_ret)
{
    pp_frame_cnt++;

    switch (pp_state)
    {
    /* ── 等待下一个路口 ── */
    case PP_NONE:
        pp_exit_cnt = 0;

        /* 预设元素锁: 提前锁定下一个期望类型, 让 path_decision 对匹配类型立即响应,
         * 同时过滤掉误检的其他类型(如十字误检→元素锁不匹配→视为直行) */
        if (pp_route_index < current_plan->length) {
            uint8_t idx = pp_node_count % current_plan->length;
            g_pp_lock_type = current_plan->nodes[idx].expected_type;
        } else {
            g_pp_lock_type = 0;
        }

        /* 冷却期: 路口必须完全消失后才接受新路口 */
        if (pp_cooldown_cnt > 0) {
            g_pp_lock_type = 0;  /* 冷却期间不锁, 封锁所有路口 */
            if (stable_turn == 0) {
                pp_cooldown_cnt--;          /* 路口持续消失 → 冷却递减  */
            } else {
                pp_cooldown_cnt = PP_COOLDOWN;  /* 又出现 → 重置冷却  */
            }
            return;
        }

        /* 没有检测到路口 → 保持 NONE, 重置超时 */
        if (stable_turn == 0) {
            pp_frame_cnt = 0;
            return;
        }

        /* 路由表耗尽 → 回退到自然行为 */
        if (pp_route_index >= current_plan->length) {
            g_pp_action    = ACT_NONE;
            g_pp_lock_type = 0;
            return;
        }

        /* 查表 */
        {
            uint8_t idx = pp_node_count % current_plan->length;
            const RouteNode_t *node = &current_plan->nodes[idx];

            /* 元素锁: 如果 expected_type!=0 且不匹配当前路口, 暂不触发 */
            if (node->expected_type != 0 && stable_turn != node->expected_type) {
                /* 超时跳过 */
                if (pp_frame_cnt > PP_LOCK_TIMEOUT) {
                    pp_node_count++;
                    pp_route_index++;
                    pp_frame_cnt = 0;
                }
                return;
            }

            /* 确认: node_count+1, 设置动作和元素锁 */
            pp_node_count++;
            pp_route_index++;
            pp_action    = node->action;
            pp_lock_type = node->expected_type;  /* 0=不锁, 非0=锁 */

            /* 写入全局变量 → 下一帧 path_decision() 读取 */
            g_pp_action    = pp_action;
            g_pp_lock_type = pp_lock_type;

            pp_state     = PP_TURNING;
            pp_frame_cnt = 0;
        }
        break;

    /* ── 正在通过路口 ── */
    case PP_TURNING:
        /* 保持动作和锁, 供 path_decision() 持续读取 */
        g_pp_action    = pp_action;
        g_pp_lock_type = pp_lock_type;

        /* 退出检测: path_decision 已退出 且 稳定路口类型为0 */
        if (pd_ret == 0 && stable_turn == 0) {
            pp_exit_cnt++;
            if (pp_exit_cnt >= PP_EXIT_FRAMES) {
                /* 确认通过 → 回到 NONE */
                pp_action    = ACT_NONE;
                pp_lock_type = 0;
                g_pp_action    = ACT_NONE;
                g_pp_lock_type = 0;
                pp_state       = PP_NONE;
                pp_frame_cnt   = 0;
                pp_exit_cnt    = 0;
                pp_cooldown_cnt = PP_COOLDOWN;
            }
        } else {
            pp_exit_cnt = 0;  /* 还在路口里, 清零 */
        }

        /* 超时强制退出 */
        if (pp_frame_cnt > PP_TURNING_TIMEOUT) {
            pp_action    = ACT_NONE;
            pp_lock_type = 0;
            g_pp_action    = ACT_NONE;
            g_pp_lock_type = 0;
            pp_state       = PP_NONE;
            pp_frame_cnt   = 0;
            pp_cooldown_cnt = PP_COOLDOWN;
        }
        break;
    }
}
