/**
  ******************************************************************************
  * @file    keyscan.h
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
/*!
    \file  keyscan.h
    \brief 
*/

#ifndef __KEYSCAN_H
#define __KEYSCAN_H

#include "gd32f1x0.h"
#include <stdbool.h>

#define KEY1_PIN                         GPIO_PIN_1
#define KEY1_GPIO_PORT                   GPIOA
#define KEY1_GPIO_CLK                    RCU_GPIOA

#define KEY2_PIN                         GPIO_PIN_3
#define KEY2_GPIO_PORT                   GPIOA
#define KEY2_GPIO_CLK                    RCU_GPIOA

#define KEY3_PIN                         GPIO_PIN_4
#define KEY3_GPIO_PORT                   GPIOA
#define KEY3_GPIO_CLK                    RCU_GPIOA

#define KEY4_PIN                         GPIO_PIN_5
#define KEY4_GPIO_PORT                   GPIOA
#define KEY4_GPIO_CLK                    RCU_GPIOA

//key value define
#define KEY_NULL           0x00
#define KEY_INPDN          0x01
#define KEY_INPUP          0x02
#define KEY_ENTER          0x04
#define KEY_CANCL          0x08
#define KEY_POWER          0x10
#define KEY_CMENU   		   0x20

extern __IO uint8_t keyval;

void keyInit(void);

uint8_t keyHitRead(void);

uint8_t keyValueRead(void);

void keyScanFSM(void);

bool testKeyPress(void);

bool testKeyRelease(void);

void clearKeyPress(void);

void clearKeyRelease(void);

void keyPressProcess(void);

#endif
