/**
  ******************************************************************************
  * @file    radio.c
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
/*!
    \file  radio.c
    \brief 
*/

#include "radio.h"
#include "rda5820.h"

void radio_preinit(void)
{
	RDA5820_CommInit();
}

void radio_rx_init(uint16_t freq,uint8_t vol)
{
	static bool flag_init=false;
	
	if(!flag_init){
		RDA5820_RX_Intialization();
	}else{
		flag_init=true;
	}
	RDA5820_SetVolumeLevel(vol);
	RDA5820_SetFreq(freq);
	RDA5820_MuteHW(0);
}

void radio_tx_init(uint16_t freq)
{
	static bool flag_init=false;
	
	if(!flag_init){
		RDA5820_TX_Intialization();
	}else{
		flag_init=true;
	}
	RDA5820_SetFreq(freq);
	RDA5820_MuteHW(0);
}

void radio_txi2s_init(uint16_t freq)
{
	static bool flag_init=false;
	
	if(!flag_init){
		RDA5820_TXI2S_Intialization();
	}else{
		flag_init=true;
	}
	RDA5820_SetFreq(freq);
	RDA5820_MuteHW(0);
}

void radio_dac_init(uint8_t vol)
{
	static bool flag_init=false;
	
	if(!flag_init){
		RDA5820_DAC_Intialization();
	}else{
		flag_init=true;
	}
	RDA5820_SetVolumeLevel(vol);
	RDA5820_MuteHW(0);
}

bool radio_seek(seek_typedef_enum seekdir)
{
	return false;
}

void radio_setVolume(uint8_t vol)
{
	RDA5820_SetVolumeLevel(vol&0xF);
}
