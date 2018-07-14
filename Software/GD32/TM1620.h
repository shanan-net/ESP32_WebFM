/**
  ******************************************************************************
  * @file    TM1620.h
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
#ifndef _TM1620_H_
#define _TM1620_H_

#include "gd32f1x0.h"


void tm1620_Init(void);
void tm1620_DispOn(void);
void tm1620_DispOff(void);
void tm1620_DispRefresh(uint8_t *dispbuf);
#endif
