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

#ifndef __LQ_WIFI_CONFIG_H
#define __LQ_WIFI_CONFIG_H

#include "lq_include.h"
/* MCU 接口配置 */
#if (1 == TC3x7_MINI_VC)  // TC3x7 mini驱动一体板
#define WLS_UART_PORT       (UART3)             // 配置串口
#define WLS_UART_RX_PIN     (UART3_RX_P00_1)    // 配置串口引脚
#define WLS_UART_TX_PIN     (UART3_TX_P00_0)
#define WLS_WIFI_IO1        (P00_5)             // 图传模块 IO1 引脚定义
#else                    // V7通用板子
#define WLS_UART_PORT       (UART0)             // 配置串口
#define WLS_UART_RX_PIN     (UART0_RX_P14_1)    // 配置串口引脚
#define WLS_UART_TX_PIN     (UART0_TX_P14_0)
#define WLS_WIFI_IO1        (P15_0)             // 图传模块 IO1 引脚定义 
#endif    // if end TC3x7 mini驱动一体板
#define WLS_UART_BAUD       (115200)            // 配置串口波特率

#define WLS_BUF_LEN         (32)                // 发送数组的长度，默认固定为 32 字节，用户无需修改
/* WiFi WLAN 连接配置 */
#define WLS_WIFI_SSID       ("LQwifi001")       // 将要连接的 WIFI 名称
#define WLS_WIFI_PASSWORD   ("longqiu123")      // 将要连接的 WIFI 密码 必须长度8字符以上.
/* 通信方式配置 */
#define WLS_SEND_MODE       ("WIFI")            // 设置模块无线工作模式. "WIFI":设置发送模式为 WIFI,  "BLUEATOOTH": 设置发送模式为蓝牙.
#define WLS_REV_MODE        ("SPI")             // 设置模块接收通信模式. "SPI":设置接收模式为SPI,  "UART": 设置接收模式为UART.
#define WLS_BT_NAME         ("LQ_BLUETOOTH")    // 将要连接的蓝牙名称.
#define WLS_UART_BAUDRATE   ("115200")          // 将要连接的 UART 波特率.
#define WLS_WIFI_TRAN_MODE  ("udp")             // 设置WIFI传输模式. "tcp_client":TCP 客户端模式, "tcp_server":TCP 服务器模式, "udp":UDP 传输模式。
/* UDP通信配置 */
#define WLS_UDP_IP          ("192.168.137.1")   // 将要连接的 UDP IP
#define WLS_UDP_PORT        ("8080")            // 将要连接的 UDP 远端端口
#define WLS_UDP_MY_PORT     ("4321")            // 将要连接的 UDP 本地(模块上)端口
/* TCP通信配置 */
#define WLS_CLIENT_IP       ("192.168.137.1")   // 将要连接的 TCP 客户端 IP
#define WLS_CLIENT_PORT     ("8080")            // 将要连接的 TCP 客户端端口
#define WLS_SERVER_PORT     ("8080")            // 将要连接的 TCP 服务器端端口


// ----以上为配置模块可能需要根据自己的情况修改的配置项，以下为模块代码实现，用户无需修改----

void lq_WLS_config(void);

#endif
