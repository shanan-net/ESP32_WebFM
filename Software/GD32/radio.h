/**
  ******************************************************************************
  * @file    radio.h
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
/*!
    \file  radio.h
    \brief 
*/

#ifndef __RADIO_H
#define __RADIO_H

#include "gd32f1x0.h"
#include <stdbool.h>

/* exported types */
typedef enum 
{
    SEEK_UP = 0,
    SEEK_DOWN = 1
} seek_typedef_enum;

typedef enum 
{
    volume_up = 0,
    volume_down = 1
} volume_typedef_enum;

void radio_preinit(void);
void radio_rx_init(uint16_t freq,uint8_t vol);
void radio_tx_init(uint16_t freq);
void radio_txi2s_init(uint16_t freq);
void radio_dac_init(uint8_t vol);
bool radio_seek(seek_typedef_enum seekdir);
void radio_setVolume(uint8_t vol);
	
#endif

