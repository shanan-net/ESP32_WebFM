/**
  ******************************************************************************
  * @file    menu.h
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
#ifndef _MENU_H_
#define _MENU_H_

#include "gd32f1x0.h"
#include <stdbool.h>
 
typedef struct MenuItem     
{  
	uint8_t MenuCount;
	uint8_t DisplayString[4];
	void (*Subs)(uint8_t);
	struct MenuItem *Childrenms;
	struct MenuItem *Parentms; 
}MENU_ITEM;

void menu_init(void);
void menu_show(uint8_t keyval);

#endif
