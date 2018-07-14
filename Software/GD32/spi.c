/**
  ******************************************************************************
  * @file    spi.c
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
#include "spi.h"

/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_config(void)
{
    spi_parameter_struct spi_init_struct;
	
	  rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_SPI0);
	
	  /* SPI0 GPIO config: SCK/PA5, MISO/PA6, MOSI/PA7 */
    gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_7);
	
		//CS
		gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
		gpio_bit_set(GPIOA,GPIO_PIN_4);
	
    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8;
    spi_init_struct.endian               = SPI_ENDIAN_LSB;
    spi_init(SPI0, &spi_init_struct);
}

void spi_open(void)
{
	gpio_bit_set(GPIOA,GPIO_PIN_4);
	spi_enable(SPI0);
}

void spi_sendbyte(uint8_t data)
{
	while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE));
	spi_i2s_data_transmit(SPI0, data);
	while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE));
	spi_i2s_data_receive(SPI0);
}

void spi_cs_enable(void)
{
		gpio_bit_reset(GPIOA,GPIO_PIN_4);
}

void spi_cs_disable(void)
{
		gpio_bit_set(GPIOA,GPIO_PIN_4);
}
