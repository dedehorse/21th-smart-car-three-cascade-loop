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

#include "LQ_EEPROM.h"

/*************************************************************************
 *  函数名称：void EEPROM_EraseSector(unsigned char sector)
 *  功能说明：eeprom擦出扇区
 *  参数说明：sector   ：  扇区范围  0-11
 *  函数返回：无
 *  备    注：EEPROM_EraseSector(0);   //擦出扇区0
 *************************************************************************/
void EEPROM_EraseSector(unsigned char sector)
{
    /* 计算扇区起始地址 */
    unsigned long sector_addr = IfxFlash_dFlashTableEepLog[sector].start;

    unsigned short endinitSfty_pw;

    endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPassword();

    /* 擦出扇区 */
    IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
    IfxFlash_eraseSector(sector_addr);
    IfxScuWdt_setSafetyEndinit(endinitSfty_pw);

    IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);
}

/*************************************************************************
 *  函数名称：void EEPROM_EraseSector(unsigned char sector)
 *  功能说明：eeprom写入数据
 *  参数说明：
 * @param    sector   ：  扇区   范围  0-11
 * @param    page     ：  页     范围  0-1023
 * @param    buff     ：  存放写入数据
 * @param    len      ：  写入数据个数
 *  函数返回：无
 *  备    注：EEPROM_Write(0, 0, u32wBuff, 24);   //向扇区0  第0页  写入24个unsigned long数据
 *************************************************************************/
void EEPROM_Write(unsigned char sector, unsigned short page, unsigned long *buff, unsigned short len)
{
    int i;
    /* 计算扇区起始地址 */
    unsigned long sector_addr = IfxFlash_dFlashTableEepLog[sector].start;

    unsigned short endinitSfty_pw;

    endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPassword();

    for (i = 0; i < len / 2; i++)
    {
        /* 要写入页地址 */
        unsigned long pageAddr = sector_addr + (page + i) * IFXFLASH_DFLASH_PAGE_LENGTH;

        IfxFlash_enterPageMode(pageAddr);

        /* 察忙 */
        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

        /* 写入缓冲区  */
        IfxFlash_loadPage2X32(pageAddr, buff[2 * i], buff[2 * i + 1]);

        /* 写入eeprom  */
        IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
        IfxFlash_writePage(pageAddr);
        IfxScuWdt_setSafetyEndinit(endinitSfty_pw);

        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);
    }

    if (len % 2)
    {
        /* 要写入页地址 */
        unsigned long pageAddr = sector_addr + (page + len / 2) * IFXFLASH_DFLASH_PAGE_LENGTH;

        IfxFlash_enterPageMode(pageAddr);

        /* 察忙 */
        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

        /* 写入缓冲区  */
        IfxFlash_loadPage2X32(pageAddr, buff[len], 0);

        /* 写入eeprom  */
        IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
        IfxFlash_writePage(pageAddr);
        IfxScuWdt_setSafetyEndinit(endinitSfty_pw);

        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);
    }
}

/*************************************************************************
 *  函数名称：void EEPROM_Read(unsigned char sector, unsigned short page, unsigned long * rbuff, unsigned short len)
 *  功能说明：eeprom读取
 *  参数说明：
 * @param    sector   ：  扇区   范围  0-11
 * @param    page     ：  页     范围  0-1023
 * @param    buff     ：  存放写入数据
 * @param    len      ：  写入数据个数
 *  函数返回：无
 *  备    注：EEPROM_Read(0, 0, u32rBuff, 24);   //读取扇区0  第0页  开始的24个unsigned long数据
 *************************************************************************/
void EEPROM_Read(unsigned char sector, unsigned short page, unsigned long *rbuff, unsigned short len)
{
    int i;
    /* 计算扇区起始地址 */
    unsigned long sector_addr = IfxFlash_dFlashTableEepLog[sector].start;

    /* 要读页地址 */
    volatile unsigned long *pageAddr = (unsigned long *)(sector_addr + page * IFXFLASH_DFLASH_PAGE_LENGTH);

    for (i = 0; i < len; i++)
    {
        rbuff[i] = *(pageAddr++);
    }
}
