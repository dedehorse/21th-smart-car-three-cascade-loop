/*******************************************************************************
 * @file                本文件是LQ_TC387_Software_Library 软件开源库文件的一部分
 * @author              chiusir
 * @email               chiusir@163.com
 * @version             V2.0.0
 * @update              2026年3月24日
 * @copyright           版权所有 (C) 2025-2026 北京龙邱科技有限公司
 * @website             http://www.lqist.cn
 * @taobao              http://longqiu.taobao.com
 *
 * @brief               龙邱科技 LQ_TC387核心板驱动库声明
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
#include "lq_include.h"

extern IfxCpu_syncEvent g_cpuSyncEvent;

void core3_main(void)
{

    SafetyWatchdog();         // 关闭看门狗
    enableInterruptLatency(); // 开启全局中断

    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    while (!g_lcdReady)
    {
    }

#if TRACK_OUTPUT_WIRELESS
    /* ── 修复 CPU3 中断向量表: 256 个优先级槽全部指向分发器 ── */
    {
        extern void IfxCpu_Irq_intVecTable3(void);
        uint32_t *biv = (uint32_t *)0x809FE000; /* CPU3 BIV 基址 */
        for (int p = 1; p < 256; p++) {
            uint32_t *e = &biv[p * 8];           /* 每槽 8 word = 32 byte */
            e[4] = (uint32_t)&IfxCpu_Irq_intVecTable3;
        }
    }

    /* QSPI4 初始化 (ISR 安装到 CPU3, 中断路由 VECTABNUM=4) */
    TR_driver_init();

    while (1)
    {
        if (g_tx_img_ready)
        {
            TR_Write_Image_Pixle(TR_IMG_H, TR_IMG_W, (uint8_t *)g_tx_img_buf);
            g_tx_img_ready = 0;
        }
    }
#else
    while (1) { ; }
#endif
}
