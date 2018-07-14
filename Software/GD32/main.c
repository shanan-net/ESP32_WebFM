/*
  ******************************************************************************
  * @file    main.c
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
/*!
    \file  main.c
    \brief 
*/

#include "gd32f1x0.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "radio.h"
#include "interrupt_priority.h"
#include "keyscan.h"
#include "segmentdisplay.h"
#include "menu.h"


/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
	uint16_t timecnt=0;
	bool flag_second=false;
	bool flag_toggle=false;
  
	nvic_config();
	
  SystemCoreClockUpdate();

  systick_config();
 
	keyInit();
	
	radio_preinit();
	
	segDispInit();
	
	menu_init();
	
	timer_config();
	
  while(1){
				if(flag_10ms){
			flag_10ms=false;
			timecnt++;
			if(timecnt>=100){
				timecnt=0;
				flag_second=true;
			}
			
			keyScanFSM();
			
			if(testKeyPress()){
				clearKeyPress();
				keyPressProcess();
			}
			
			if(testKeyRelease()){
				clearKeyRelease();
			}
			       
		}//if(flag_10ms)
		
		if(flag_second){
			flag_second=false;
			flag_toggle=!flag_toggle;		
		}//if(flag_second)

  }//while(1)
}

