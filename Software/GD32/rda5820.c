/**
  ******************************************************************************
  * @file    rda5820.c
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
	
#include "rda5820.h"
#include "systick.h"

#define RDA5820_SDIO_PIN                        GPIO_PIN_10
#define RDA5820_SDIO_GPIO_PORT                  GPIOA
#define RDA5820_SDIO_GPIO_CLK                   RCU_GPIOA
        
#define RDA5820_SCLK_PIN                        GPIO_PIN_9
#define RDA5820_SCLK_GPIO_PORT                  GPIOA
#define RDA5820_SCLK_GPIO_CLK                   RCU_GPIOA

#define _SHARE_CRYSTAL_32KHz_
#define RDA5820_I2C_ADDR 0x22

#define MCU_OWN_ADDRESS7   0xAA

uint16_t RDA5820_tab[][2] = {
	{0x02, 0x0002},//SOFT_RESET
	{0x02, 0x8001},//Audio Output,ENABLE
	{0x02, 0x8001},
	/*****************************/
	{0x03, 0x0000},
	{0x04, 0x0400},
	{0x05, 0x8840},//LNAN
};

uint16_t RDA5820_Intialization_tab[][2] = {
	#if defined(_FM_STEP_50K_)
	{0x03, 0x0002},
	#else
	{0x03, 0x0000},
	#endif
	{0x04, 0x0400},
	{0x05, 0x8840},
	/**********************************/
	{0x07, 0x7800},
};

uint16_t RDA5820NS_RX_Intialization_tab[][2]=
{
#if defined(_FM_STEP_50K_)
	{0x03, 0x0002},
#else
	{0x03, 0x0000},
#endif
	{0x04, 0x0400},
	{0x05, 0x886F},
	/**********************************/
	{0x40, 0x0000},//FM Receive
};

uint16_t RDA5820NS_TX_Intialization_tab[][2]=
{
	#if defined(_FM_STEP_50K_)
	{0x03, 0x0002},
	#else
	{0x03, 0x0000},
	#endif
	{0x04, 0x0C00},//De-emphasis:50us;
	{0x05, 0x884f},
	/**********************************/
	{0x68, 0x1CF0},
	{0x40, 0x0001},//FM Transmit
};

uint16_t RDA5820NS_TXI2S_Intialization_tab[][2]=
{
	#if defined(_FM_STEP_50K_)
	{0x03, 0x0002},
	#else
	{0x03, 0x0000},
	#endif
	{0x04, 0x0C00},//De-emphasis:50us;
	{0x05, 0x8840},
	/**********************************/
	{0x06, 0x1270},//I2S Slave mode,44.1KHz,signed 16-bit
	{0x04, 0x0C40},//De-emphasis:50us;I2S Enable
	{0x68, 0x00F0},
	{0x40, 0x0001},//FM Transmit
};

uint16_t RDA5820NS_DAC_Intialization_tab[][2]=
{
	#if defined(_FM_STEP_50K_)
	{0x03, 0x0002}, //0
	#else
	{0x03, 0x0000}, //0
	#endif
	{0x04, 0x0C40},//De-emphasis:50us;I2S Enable //1
	{0x05, 0x884f},	//2
	/**********************************/
	{0x06, 0x1270},//I2S slave mode,44.1KHz,signed 16-bit
	{0x68, 0x1DFF},
	{0x40, 0x000C},//Codec
};

void RDA5820_Delay_ms(uint16_t ms)
{
	delay_1ms(ms);
}

void i2c_write_data(uint8_t dev, uint8_t addr, uint8_t *data, uint8_t len)
{
	uint8_t i;
	
  /* wait until I2C bus is idle */
  while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
  /* send a start condition to I2C bus */
  i2c_start_on_bus(I2C0);
  /* wait until SBSEND bit is set */
  while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
	
  /* send slave address to I2C bus */
  i2c_master_addressing(I2C0, dev, I2C_TRANSMITTER);
  /* wait until ADDSEND bit is set */
  while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
  /* clear ADDSEND bit */
  i2c_flag_clear(I2C0, I2C_STAT0_ADDSEND);
	
	i2c_data_transmit(I2C0, addr);
	/* wait until the TBE bit is set */
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

  for(i=0; i<len; i++){
      /* data transmission */
      i2c_data_transmit(I2C0, *(data+i));
      /* wait until the TBE bit is set */
      while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));
  }
  /* send a stop condition to I2C bus */
  i2c_stop_on_bus(I2C0);
  while(I2C_CTL0(I2C0)&0x0200);
}

void FM_RDA5820_Write(uint16_t aAddr, uint16_t aData)
{
	uint8_t tmp[2] = {0x0};
	
	tmp[0] = (aData >> 8);
	tmp[1] = (aData & 0x00FF);

	i2c_write_data(RDA5820_I2C_ADDR, aAddr, &tmp[0], 2);
}

void i2c_read_data(uint8_t dev, uint8_t addr, uint16_t *pData)
{
  /* wait until I2C bus is idle */
  while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
  /* send a start condition to I2C bus */
  i2c_start_on_bus(I2C0);
  /* wait until SBSEND bit is set */
  while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
  
  /* send slave address to I2C bus */
  i2c_master_addressing(I2C0, dev, I2C_TRANSMITTER);

  /* wait until ADDSEND bit is set */
  while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
  /* clear ADDSEND bit */
  i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
	
	/* data transmission */
	i2c_data_transmit(I2C0, addr);
	/* wait until the TBE bit is set */
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));	
	
  /* send a start condition to I2C bus */
  i2c_start_on_bus(I2C0);
  /* wait until SBSEND bit is set */
  while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
  
  /* send slave address to I2C bus */
  i2c_master_addressing(I2C0, dev, I2C_RECEIVER);
	
	 while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
  /* clear ADDSEND bit */
  i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
	
	/* wait until the RBNE bit is set */
	while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));
	/* disable acknowledge */
	i2c_ack_config(I2C0, I2C_ACK_DISABLE);
	/* read a data from I2C_DATA */
	*pData = i2c_data_receive(I2C0)<<8;

	/* wait until the RBNE bit is set */
	while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));
	/* read a data from I2C_DATA */
	*pData |= i2c_data_receive(I2C0);
					
  /* send a stop condition to I2C bus */
  i2c_stop_on_bus(I2C0);
	
  while(I2C_CTL0(I2C0)&0x0200);
  /* enable acknowledge */
  i2c_ack_config(I2C0, I2C_ACK_ENABLE);
}

void FM_RDA5820_Read(uint8_t aAddr, uint16_t *pData)
{
	i2c_read_data(RDA5820_I2C_ADDR, aAddr, pData);
}

/**
* @brief RDA5820 RX initialize
* @author RDA Ri'an Zeng
* @date 2010-09-01
* @param void
* @return void
* @retval
*/

void RDA5820_RX_Intialization(void)
{
	uint16_t DeviceID = 0x0000;
	uint16_t cnt = 0;

	FM_RDA5820_Write(RDA5820_tab[0][0], RDA5820_tab[0][1]);
	RDA5820_Delay_ms(50);
	FM_RDA5820_Read(0x00, &DeviceID);
	RDA5820_Delay_ms(50);

	if (DeviceID == 0x5805){
		FM_RDA5820_Write(RDA5820_tab[1][0], RDA5820_tab[1][1]);
		RDA5820_Delay_ms(500);

		for(cnt=0;cnt< ((sizeof(RDA5820_Intialization_tab))/(sizeof(RDA5820_Intialization_tab[0])));cnt++)
			FM_RDA5820_Write(RDA5820_Intialization_tab[cnt][0], RDA5820_Intialization_tab[cnt][1]);
		
		for(cnt=0;cnt<3;cnt++)
			RDA5820_tab[cnt+3][1] = RDA5820_Intialization_tab[cnt][1];
	}else if(DeviceID == 0x5820){
		#if defined(_SHARE_CRYSTAL_24MHz_)
		RDA5820_tab[1][1] = 0xC451;
		#elif defined(_SHARE_CRYSTAL_12MHz_)
		RDA5820_tab[1][1] = 0xC411;
		#elif defined(_SHARE_CRYSTAL_32KHz_)
		RDA5820_tab[1][1] = 0xC401;
		#endif
		
		FM_RDA5820_Write(RDA5820_tab[1][0], RDA5820_tab[1][1]);
		RDA5820_Delay_ms(600);
		for(cnt = 0;cnt< ((sizeof(RDA5820NS_RX_Intialization_tab))/(sizeof(RDA5820NS_RX_Intialization_tab[0])));cnt++)
			FM_RDA5820_Write(RDA5820NS_RX_Intialization_tab[cnt][0], RDA5820NS_RX_Intialization_tab[cnt][1]);
		
		for(cnt=0;cnt<3;cnt++)
			RDA5820_tab[cnt+3][1] = RDA5820NS_RX_Intialization_tab[cnt][1];
	}
}

/**
* @brief RDA5820 TX initialize
* @author RDA Ri'an Zeng
* @date 2010-09-01
* @param void
* @return void
* @retval
*/

void RDA5820_TX_Intialization(void)
{
	uint16_t DeviceID = 0x0000;
	uint16_t cnt = 0;

	FM_RDA5820_Write(RDA5820_tab[0][0], RDA5820_tab[0][1]);
	RDA5820_Delay_ms(100);
	FM_RDA5820_Read(0x00, &DeviceID);
	RDA5820_Delay_ms(50);

	FM_RDA5820_Write(RDA5820_tab[1][0], RDA5820_tab[1][1]);
	RDA5820_Delay_ms(600);

	if (DeviceID == 0x5805){
		for(cnt = 0;cnt< ((sizeof(RDA5820_Intialization_tab))/(sizeof(RDA5820_Intialization_tab[0])));cnt++)
			FM_RDA5820_Write(RDA5820_Intialization_tab[cnt][0], RDA5820_Intialization_tab[cnt][1]);
		
		for(cnt=0;cnt<3;cnt++)
			RDA5820_tab[cnt+3][1] = RDA5820_Intialization_tab[cnt][1];
	}else if(DeviceID == 0x5820){
		for(cnt = 0;cnt< ((sizeof(RDA5820NS_TX_Intialization_tab))/(sizeof(RDA5820NS_TX_Intialization_tab[0])));cnt++)
			FM_RDA5820_Write(RDA5820NS_TX_Intialization_tab[cnt][0], RDA5820NS_TX_Intialization_tab[cnt][1]);

		for(cnt=0;cnt<3;cnt++)
			RDA5820_tab[cnt+3][1] = RDA5820NS_TX_Intialization_tab[cnt][1];
	}
}

void RDA5820_TXI2S_Intialization(void)
{
	uint16_t DeviceID = 0x0000;
	uint16_t cnt = 0;

	FM_RDA5820_Write(RDA5820_tab[0][0], RDA5820_tab[0][1]);
	RDA5820_Delay_ms(100);
	FM_RDA5820_Read(0x00, &DeviceID);
	RDA5820_Delay_ms(50);

	FM_RDA5820_Write(RDA5820_tab[1][0], RDA5820_tab[1][1]);
	RDA5820_Delay_ms(600);

	if (DeviceID == 0x5805){
		for(cnt = 0;cnt< ((sizeof(RDA5820_Intialization_tab))/(sizeof(RDA5820_Intialization_tab[0])));cnt++)
			FM_RDA5820_Write(RDA5820_Intialization_tab[cnt][0], RDA5820_Intialization_tab[cnt][1]);
		
		for(cnt=0;cnt<3;cnt++)
			RDA5820_tab[cnt+3][1] = RDA5820_Intialization_tab[cnt][1];
	}else if(DeviceID == 0x5820){
		for(cnt = 0;cnt< ((sizeof(RDA5820NS_TXI2S_Intialization_tab))/(sizeof(RDA5820NS_TXI2S_Intialization_tab[0])));cnt++)
			FM_RDA5820_Write(RDA5820NS_TXI2S_Intialization_tab[cnt][0], RDA5820NS_TXI2S_Intialization_tab[cnt][1]);

		for(cnt=0;cnt<3;cnt++)
			RDA5820_tab[cnt+3][1] = RDA5820NS_TXI2S_Intialization_tab[cnt][1];
	}
}

void RDA5820_DAC_Intialization(void)
{
	uint16_t DeviceID = 0x0000;
	uint16_t cnt = 0;

	FM_RDA5820_Write(RDA5820_tab[0][0], RDA5820_tab[0][1]);
	RDA5820_Delay_ms(100);
	FM_RDA5820_Read(0x00, &DeviceID);
	RDA5820_Delay_ms(50);

	FM_RDA5820_Write(RDA5820_tab[1][0], RDA5820_tab[1][1]);
	RDA5820_Delay_ms(600);

	if (DeviceID == 0x5805){
		for(cnt = 0;cnt< ((sizeof(RDA5820_Intialization_tab))/(sizeof(RDA5820_Intialization_tab[0])));cnt++)
			FM_RDA5820_Write(RDA5820_Intialization_tab[cnt][0], RDA5820_Intialization_tab[cnt][1]);
		
		for(cnt=0;cnt<3;cnt++)
			RDA5820_tab[cnt+3][1] = RDA5820_Intialization_tab[cnt][1];
	}else if(DeviceID == 0x5820){
		for(cnt = 0;cnt< ((sizeof(RDA5820NS_DAC_Intialization_tab))/(sizeof(RDA5820NS_DAC_Intialization_tab[0])));cnt++)
			FM_RDA5820_Write(RDA5820NS_DAC_Intialization_tab[cnt][0], RDA5820NS_DAC_Intialization_tab[cnt][1]);

		for(cnt=0;cnt<3;cnt++)
			RDA5820_tab[cnt+3][1] = RDA5820NS_DAC_Intialization_tab[cnt][1];
	}
}

/**
* @brief Deinitialize RDA5820 function
* @author RDA Ri'an Zeng
* @date 2010-09-01
* @param void
* @return void
* @retval
*/

void RDA5820_Deinit(void)
{
	uint16_t RDA5820_poweroff = 0x0000;

	FM_RDA5820_Write(0x02, RDA5820_poweroff);
	RDA5820_Delay_ms(100);
}

/**
* @brief Set RDA5820 into mute mode
* @author RDA Ri'an Zeng
* @date 2010-09-01
* @param bool mute: if mute is true,then set mute; if mute is false,then set no mute
* @return void
* @retval
*/

void RDA5820_Mute(unsigned char mute)
{
	if(mute) //mute
		FM_RDA5820_Write(0x02, RDA5820_tab[2][1]&0xbfff);
	else
		FM_RDA5820_Write(0x02, RDA5820_tab[2][1]|0x4000);
}

/**
* @brief Cover the frequency to channel value
* @author RDA Ri'an Zeng
* @date 2010-09-01
* @param uint32_t frequency:covered frequency
* @return uint32_t: channel value
* @retval
*/

uint32_t RDA5820_FreqToChan(uint32_t freq)
{
	uint32_t curChan = 0;
	uint32_t nBand,nSpace;

	if((RDA5820_tab[3][1] & 0x0c) == 0x00)
		nBand = 8700;
	else if((RDA5820_tab[3][1] & 0x0c) == 0x04)
		nBand = 7600;
	else if((RDA5820_tab[3][1] & 0x0c) == 0x08)
		nBand = 7600;

	if((RDA5820_tab[3][1] & 0x03) == 0x00)
		nSpace = 10;
	else if((RDA5820_tab[3][1] & 0x03) == 0x01)
		nSpace = 20;
	else
		nSpace = 5;

	curChan	= (freq - nBand)/nSpace;

	return curChan;
}

/**
* @brief Cover the channel value to frequency
* @author RDA Ri'an Zeng
* @date 2010-09-01
* @param uint32_t chan: channel value
* @return uint32_t: frequency value
* @retval
*/

uint32_t RDA5820_ChanToFreq(uint32_t chan)
{
	uint32_t curChan = 0;
	uint32_t nBand,nSpace;
	uint32_t freq = 0;

	if((RDA5820_tab[3][1] & 0x0c) == 0x00)
		nBand = 8700;
	else if((RDA5820_tab[3][1] & 0x0c) == 0x04)
		nBand = 7600;
	else if((RDA5820_tab[3][1] & 0x0c) == 0x08)
		nBand = 7600;
	
	if((RDA5820_tab[3][1] & 0x03) == 0x00)
		nSpace = 10;
	else if((RDA5820_tab[3][1] & 0x03) == 0x01)
		nSpace = 20;
	else
		nSpace = 5;

	freq = curChan*nSpace + nBand;
	return freq;
}

/**
* @brief Set frequency function
* @author RDA Ri'an Zeng
* @date 2010-09-01
* @param uint32_t curFreq: frequency value
* @return void
* @retval
*/

void RDA5820_SetFreq(uint32_t curFreq)
{
	uint32_t RDA5820_channel_tune = 0;
	uint32_t Chanel_H= 0;
	uint32_t curChan = 0;

	curChan = RDA5820_FreqToChan(curFreq);
	Chanel_H = (curChan>>2);
	RDA5820_channel_tune=(Chanel_H<<8) |(((curChan&0x0003)<<6)|(RDA5820_tab[3][1]&0x000f)|0x0010) ;	//set tune bit
	FM_RDA5820_Write( 0x03, RDA5820_channel_tune);
	RDA5820_Delay_ms(500);//kal_sleep_task(5);

}

/**
* @brief Station judge for auto search
* @In auto search mode,uses this function to judge the frequency if a station
* @author RDA Ri'an Zeng
* @date 2010-09-01
* @param uint32_t freq:frequency value
* @return bool: if return true,the frequency has a true station;otherwise doesn't have a station
* @retval
*/

bool RDA5820_ValidStop(uint32_t freq)
{
	uint16_t RDA5820_data[2]={0};
	uint16_t RDA5820_channel_tune = 0;
	uint8_t falseStation = 0;
	uint16_t Chanel_H= 0;
	uint32_t curChan;

	curChan = RDA5820_FreqToChan(freq);
	Chanel_H = (curChan>>2);
	RDA5820_channel_tune=(Chanel_H<<8) |(((curChan&0x0003)<<6)|(RDA5820_tab[3][1]&0x000f)|0x0010) ;	//set tune bit
	FM_RDA5820_Write( 0x03, RDA5820_channel_tune);
	RDA5820_Delay_ms(300);//kal_sleep_task(10);
	//read REG0A&0B
	FM_RDA5820_Read(0x0a, &RDA5820_data[0]);
	RDA5820_Delay_ms(100);//kal_sleep_task(10);
	FM_RDA5820_Read(0x0b, &RDA5820_data[1]);
	//check whether STC=1
	if((RDA5820_data[0]&0x4000)==0) falseStation=1;
	//check FM_TURE
	if((RDA5820_data[1] &0x0100)==0) falseStation=1;
	if (falseStation==1)
		return false;
	else
		return true;
}

/**
* @brief Get the signal level(RSSI) of the current frequency
* @author RDA Ri'an Zeng
* @date 2010-09-01
* @param uint32_t curf:frequency value
* @return uint8_t: the signal level(RSSI)
* @retval
*/

uint8_t RDA5820_GetSigLvl(uint32_t curf)
{
	return 0;
}

/**
* @brief Set FM volume
* @It has better use the system volume operation to replace this function
* @author RDA Ri'an Zeng
* @date 2010-09-01
* @param uint8_t level: volume value
* @return void
* @retval
*/

void RDA5820_SetVolumeLevel(unsigned char level)
{
	RDA5820_tab[5][1] &= 0xFFF0;
	RDA5820_tab[5][1] |= (level & 0x0f);

	FM_RDA5820_Write(0x05, RDA5820_tab[5][1]);
}

void i2c_config(void)
{
  rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_I2C0);
	
	gpio_af_set(RDA5820_SCLK_GPIO_PORT, GPIO_AF_4, RDA5820_SCLK_PIN);
	gpio_af_set(RDA5820_SDIO_GPIO_PORT, GPIO_AF_4, RDA5820_SDIO_PIN);

	gpio_mode_set(RDA5820_SDIO_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,RDA5820_SDIO_PIN);
	gpio_output_options_set(RDA5820_SDIO_GPIO_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,RDA5820_SDIO_PIN);

	gpio_mode_set(RDA5820_SCLK_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,RDA5820_SCLK_PIN);
	gpio_output_options_set(RDA5820_SCLK_GPIO_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,RDA5820_SCLK_PIN);

	/* I2C clock configure */
	i2c_clock_config(I2C0, 100000, I2C_DTCY_2);
	/* I2C address configure */
	i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, MCU_OWN_ADDRESS7);
	/* enable I2C */
	i2c_enable(I2C0);
	/* enable acknowledge */
	i2c_ack_config(I2C0, I2C_ACK_ENABLE);	
}

void RDA5820_CommInit(void)
{
	i2c_config();
}

void RDA5820_SetModu(uint8_t level)
{
	FM_RDA5820_Write(0x68, 0x0100|level);
}
