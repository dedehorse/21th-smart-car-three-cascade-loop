/**
  ******************************************************************************
  * @file    LQ_PathPlan.h
  * @brief   节点法路径规划模块头文件
  *          基于路口计数 + 预配路由表, 在每个节点决定转向动作,
  *          配合元素锁过滤非期望路口类型的误检
  ******************************************************************************
  */

#ifndef __PATHPLAN_H
#define __PATHPLAN_H

#include <stdint.h>

/* ── 规划动作 ── */
typedef enum {
    ACT_NONE       = 0,  /* 无动作, 正常循线                     */
    ACT_TURN_LEFT  = 1,  /* 左转 (强制画左转拉线)                */
    ACT_TURN_RIGHT = 2,  /* 右转 (强制画右转拉线)                */
    ACT_GO_STRAIGHT = 3  /* 直行 (忽略路口, 不画拉线)            */
} PathAction_t;

/* ── 规划状态机 ── */
typedef enum {
    PP_NONE    = 0,  /* 等待下一个路口                           */
    PP_TURNING = 1   /* 正在通过路口, 执行规划动作                */
} PathPlanState_t;

/* ── 路由表条目 ── */
typedef struct {
    uint8_t expected_type;  /* 期望路口类型 1-6, 0=任意(不锁)     */
    uint8_t action;         /* PathAction_t: 该节点的规划动作      */
} RouteNode_t;

/* ── 路线计划 ── */
typedef struct {
    const RouteNode_t *nodes;  /* 路由表数组                       */
    uint8_t length;            /* 路由表长度(节点数)               */
} PathPlan_t;

/* ── 全局变量, path_decision() 每帧读取 ── */
extern uint8_t g_pp_action;     /* 当前动作 (PathAction_t)        */
extern uint8_t g_pp_lock_type;  /* 当前元素锁 (0=不锁, 1-6=锁类型)*/

/* ── API ── */
void PathPlan_Init(void);
void PathPlan_Process(uint8_t stable_turn, uint8_t pd_ret);
void PathPlan_SetRoute(uint8_t index);
uint8_t PathPlan_GetNode(void);
uint8_t PathPlan_GetState(void);
const char* PathPlan_GetStateName(void);
uint8_t PathPlan_GetRouteCount(void);
uint8_t PathPlan_GetRouteLength(void);
uint8_t PathPlan_GetRouteEntry(uint8_t index, uint8_t *expected_type, uint8_t *action);

#endif
