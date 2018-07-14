/**
  ******************************************************************************
  * @file    spi.h
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
#ifndef _SPI_H_
#define _SPI_H_

#include "gd32f1x0.h"
#include "gd32f1x0_libopt.h"

void spi_config(void);
void spi_open(void);
void spi_sendbyte(uint8_t data);
void spi_cs_enable(void);
void spi_cs_disable(void);
#endif
