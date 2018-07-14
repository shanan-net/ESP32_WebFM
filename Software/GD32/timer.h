/**
  ******************************************************************************
  * @file    timer.h
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
/*!
    \file  timer.h
    \brief 
*/

#ifndef __TIMER_H
#define __TIMER_H

#include "gd32f1x0.h"
#include <stdbool.h>

extern __IO bool flag_10ms;

void timer_config(void);

#endif
