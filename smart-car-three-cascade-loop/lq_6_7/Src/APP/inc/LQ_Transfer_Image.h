#ifndef _LQ_TRANSFER_IMAGE_H_
#define _LQ_TRANSFER_IMAGE_H_
#include "lq_include.h"


#if (1 == TC3x7_MINI_VC)  // TC3x7 mini驱动一体板
#define TR_SPIX      QSPI4
#define TR_CLK       QSPI4_CLK_P33_11
#define TR_MISO      QSPI4_MISO_P33_13
#define TR_MOSI      QSPI4_MOSI_P33_12
#define TR_CSX       QSPI4_CS_P02_15  // 占位使用
#define TR_CS        P00_9
#define IO2          P00_7
#else                       //  V7通用板子
#define TR_SPIX      QSPI1
#define TR_CLK       QSPI1_CLK_P11_6
#define TR_MISO      QSPI1_MISO_P11_3
#define TR_MOSI      QSPI1_MOSI_P11_9
#define TR_CSX       QSPI1_CS_P33_10  // 占位使用，选一个空闲的引脚
#define TR_CS        P13_0
#define IO2          P13_3   
#endif //ENDIF  TC3x7 mini驱动一体板


#define TR_IMG_W    188
#define TR_IMG_H    120

#define TR_CS_H     PIN_Write(TR_CS, 1)
#define TR_CS_L     PIN_Write(TR_CS, 0)

#define TR_IO2  PIN_Read(IO2)

extern unsigned char FH[4];
extern unsigned char FE[4];

void TR_driver_init(void);
void IR_Write_byte_4000(unsigned char *dat);
void IR_Wirte_byte(unsigned char *dat, uint16_t len);

void TR_Write_Image(unsigned char high, unsigned char wide, unsigned char *dat);
void TR_Write_Image_Pixle(unsigned char height, unsigned char width, unsigned char *Pixle);
void Test_CAMERA_TR(void);

#endif
