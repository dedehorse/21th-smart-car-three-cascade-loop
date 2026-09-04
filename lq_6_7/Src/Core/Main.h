/*******************************************************************************
 * @file                ���ļ���LQ_TC387_Software_Library �����Դ���ļ���һ����
 * @author              chiusir
 * @email               chiusir@163.com
 * @version             V2.0.0
 * @update              2026��3��24��
 * @copyright           ��Ȩ���� (C) 2025-2026 ��������Ƽ����޹�˾
 * @website             http://www.lqist.cn
 * @taobao              http://longqiu.taobao.com
 *
 * @brief               ����Ƽ� LQ_TC387���İ�����������
 *
 * �������ѭGPL-3.0��ԴЭ�鷢����ּ��ΪTC387оƬǶ��ʽϵͳ������ṩ�������֡���������TC387Ӧ�ó���Ĳο�
 * ��ҵ��;��������λʹ�ã�����ǰ��ϵ http://www.lqist.cn �����Ȩ
 *
 * ������������:
 *   - ����ƽ̨ : AURIX-Studio �汾-1.10.2  (���ADS 1.10.x)
 *   - �ļ����� : UTF-8  (AURIX-Studio-1.10.x Ĭ�ϱ���)
 *   - Ŀ��оƬ : TC387QP (TriCore? @ 300 MHz X4 10 Mbyte flash, 1568 KB of RAM)
 *   - ���þ��� : 20MHz
 *   - ϵͳPLL : 300MHz + 300MHz + 300MHz + 300MHz
 * 
 * GPL-3.0 ���֤����ժҪ:
 * 1. ��������ʹ�á��޸ġ��ַ������
 * 2. �ַ��޸ĺ�İ汾ʱ����������ͬ���֤����
 * 3. ���뱣��ԭʼ��Ȩ���������֤��Ϣ
 * 4. ���ṩ�κε�����ʹ�÷����Ը�
 * 5. ����Э���ı���μ���Ŀ��Ŀ¼ LICENSE �ļ�
 *******************************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdint.h>

extern volatile uint16_t g_debugFps;
extern volatile uint8_t g_lcdReady;

#if TRACK_OUTPUT_WIRELESS
/* CPU0→CPU3 共享: 无线图像传输 (188x120 字节) */
extern volatile uint8_t g_tx_img_buf[];
extern volatile uint8_t g_tx_img_ready;
#endif

#endif /* _MAIN_H_ */
