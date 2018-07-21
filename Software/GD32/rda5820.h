/**
  ******************************************************************************
  * @file    rda5820.h
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
#ifndef _RDA5820_H_
#define _RDA5820_H_

#include "gd32f1x0.h"
#include <stdbool.h>

#define SEEK_OK	0
#define SEEK_NG	1

void RDA5820_RX_Intialization(void);
void RDA5820_TX_Intialization(void);
void RDA5820_TXI2S_Intialization(void);
void RDA5820_DAC_Intialization(void);
void RDA5820_Deinit(void);
void RDA5820_MuteHW(unsigned char mute);
void RDA5820_MuteSW(unsigned char mute);
uint32_t RDA5820_FreqToChan(uint32_t freq);
uint32_t RDA5820_ChanToFreq(uint32_t chan);
void RDA5820_SetFreq(uint32_t curFreq);
uint8_t RDA5820_Seek(uint8_t dir);
bool RDA5820_ValidStop(uint32_t freq);
uint8_t RDA5820_GetSigLvl(uint32_t curf);
void RDA5820_SetVolumeLevel(unsigned char level);
void RDA5820_CommInit(void);
void RDA5820_SetModu(uint8_t level);
#endif
