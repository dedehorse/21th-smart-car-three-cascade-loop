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

#ifndef __LQ_DMA_H__
#define __LQ_DMA_H__

#include "lq_include.h"

#include "IfxCpu.h"
#include "IfxDma_Dma.h"
#include "IfxDma.h"
#include "IfxDma_regdef.h"

/**
 * 摄像头 DMA 配置结构体
 */
typedef struct
{
    Ifx_DMA_CH linkedList[2]; // 总是报错，去掉了IFX_ALIGN(256)，实测没有影响
    // IFX_ALIGN(256) Ifx_DMA_CH linkedList[2]; //链表存储。必须与256位地址对齐，否则DMA无法读取
    IfxDma_Dma_Channel chn; // Dma channel handle
} Dma_Camera_t;

/** 摄像头 数据采集完成标志位 */
extern unsigned char Camera_Flag;

/**
 * 摄像头 DMA 配置结构体
 */
extern Dma_Camera_t g_DmaCameraLinkedList;

/* DMA 中断服务函数  */
void DMA_IRQHandler(void);

/* 摄像头DMA传输初始化*/
void DMA_CameraInitConfig(unsigned long srcStartAddr, unsigned long dstStartAddr, unsigned long channel);

/*  摄像头DMA传输开启 */
void DMA_CameraStart(unsigned long channel);

/* DMA传输停止*/
void DMA_CameraStop(unsigned long channel);

/*  摄像头DMA传输中断处理函数*/
void DMA_CameraIRQ(unsigned long channel);

#endif /* __LQ_DMA_H__ */