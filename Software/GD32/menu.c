/*
  ******************************************************************************
  * @file    menu.h
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
	
#include "menu.h"
#include "segmentdisplay.h"
#include "keyscan.h"
#include "radio.h"
#include "rda5820.h"

#define Null 0

MENU_ITEM m0_main[4];
MENU_ITEM m1_launch1[1];
MENU_ITEM m4_launch2[1];
MENU_ITEM m2_receive[3];
MENU_ITEM m3_dac[1];
MENU_ITEM receive0_setfreq[1];
MENU_ITEM receive1_setvol[1];

void launch1SetFreq(uint8_t para)
{
	static uint16_t freq=10000;
		
	if(KEY_ENTER==para){
		radio_tx_init(freq);
	}else if(KEY_INPUP==para){
		if(freq<10800)freq+=10;
		displayNum(freq/10,10);
	}else if(KEY_INPDN==para){
		if(freq>8700)freq-=10;
		displayNum(freq/10,10);
	}
}

void launch2SetFreq(uint8_t para)
{
	static uint16_t freq=10000;
		
	if(KEY_ENTER==para){
		radio_txi2s_init(freq);
	}else if(KEY_INPUP==para){
		if(freq<10800)freq+=10;
		displayNum(freq/10,10);
	}else if(KEY_INPDN==para){
		if(freq>8700)freq-=10;
		displayNum(freq/10,10);
	}
}

uint8_t volume=4;
uint16_t rx_freq=8700;
void receiveSetFreq(uint8_t para)
{
	if(KEY_ENTER==para){
		radio_rx_init(rx_freq,volume); 
	}else if(KEY_INPUP==para){
		if(rx_freq<10800)rx_freq+=10;
		displayNum(rx_freq/10,10);
	}else if(KEY_INPDN==para){
		if(rx_freq>8700)rx_freq-=10;
		displayNum(rx_freq/10,10);
	}
}

void receiveSetVolu(uint8_t para)
{
	if(KEY_ENTER==para){
		radio_setVolume(volume);
	}else if(KEY_INPUP==para){
		if(volume<15)volume++;
		displayNum(volume,10);	
	}else if(KEY_INPDN==para){
		if(volume>0)volume--;
		displayNum(volume,10);
	}
}

void dacSet(uint8_t para)
{
	if(KEY_ENTER==para){
		radio_dac_init(volume); 
	}else if(KEY_INPUP==para){
		if(volume<15)volume++;
		displayNum(volume,10);	
	}else if(KEY_INPDN==para){
		if(volume>0)volume--;
		displayNum(volume,10);
	}
}

void nullfunc(uint8_t para)
{

}

MENU_ITEM m0_main[4]= 
{ 
	{4,{C_A,C_U,C_H,C_OFF},nullfunc,m1_launch1,Null}, //Launch,FM Transmit
	{4,{C_B,C_T,C_OFF,C_OFF},nullfunc,m4_launch2,Null}, //Launch,FM Transmit
	{4,{C_A,C_E,C_C,C_OFF},nullfunc,m2_receive,Null},//Receive,FM Receive
	{4,{C_D,C_A,C_C,C_OFF},nullfunc,m3_dac,Null},//DAC
};

MENU_ITEM m1_launch1[1]= 
{ 
	{1,{C_F,C_DOT,C_OFF,C_OFF},launch1SetFreq,Null,m0_main}, //Frequency
};

MENU_ITEM m2_receive[3]= 
{ 
	{2,{C_F|C_DOT,C_OFF,C_OFF,C_OFF},receiveSetFreq,receive0_setfreq,m0_main},//Set channel
	{2,{C_U|C_DOT,C_OFF,C_OFF,C_OFF},receiveSetVolu,receive1_setvol,m0_main},//Volume
};

MENU_ITEM m3_dac[1]= 
{ 
	{1,{C_U|C_DOT,C_OFF,C_OFF,C_OFF},dacSet,Null,m0_main},//Volume
};

MENU_ITEM m4_launch2[1]= 
{ 
	{1,{C_F|C_DOT,C_OFF,C_OFF,C_OFF},launch2SetFreq,Null,m0_main}, //Frequency
};

MENU_ITEM receive0_setfreq[1]= 
{ 
	{1,{C_F|C_DOT,C_OFF,C_OFF,C_OFF},receiveSetFreq,Null,m2_receive},//Set channel
};

MENU_ITEM receive1_setvol[1]= 
{ 
	{1,{C_U|C_DOT,C_OFF,C_OFF,C_OFF},receiveSetVolu,Null,m2_receive},//Volume
};

MENU_ITEM receive1_seek[1]= 
{ 
	{1,{C_5,C_E,C_E,C_OFF},receiveSetVolu,Null,m2_receive},//Seek
};

void title_show(MENU_ITEM *mitem)
{
	segrefresh(mitem->DisplayString,4);
}

void menu_execution(MENU_ITEM *mitem,uint8_t para)
{
	(*(mitem->Subs))(para);
}

MENU_ITEM *mitem_ptr;
uint8_t menuid[3]={0,0,0},menuidx=0;

void menu_init(void)
{
	mitem_ptr = &m0_main[0];
	title_show(mitem_ptr); 
}

void menu_show(uint8_t keyval) 
{ 
	switch (keyval)
	{ 
		case KEY_INPDN:
			{ 
				if(mitem_ptr->MenuCount==1){
					menu_execution(mitem_ptr+menuid[menuidx],KEY_INPDN);
				}else{				
					menuid[menuidx]++; 
					if (menuid[menuidx]>mitem_ptr->MenuCount-1) menuid[menuidx]=0;
					title_show(mitem_ptr+menuid[menuidx]); 
				}
				break; 
			} 
		case KEY_INPUP:
			{ 
				if(mitem_ptr->MenuCount==1){
					menu_execution(mitem_ptr+menuid[menuidx],KEY_INPUP);
				}else{
					if (menuid[menuidx]==0) menuid[menuidx]=mitem_ptr->MenuCount-1;
					else menuid[menuidx]--; 
					title_show(mitem_ptr+menuid[menuidx]); 
				}
				break; 
			} 
		case KEY_ENTER:
			{ 
				if ((mitem_ptr+menuid[menuidx])->Childrenms !=Null) 
				{ 
					mitem_ptr=(mitem_ptr+menuid[menuidx])->Childrenms; 
					menuidx++; 
					menuid[menuidx]=0; 
					title_show(mitem_ptr+menuid[menuidx]); 
				} 
				else 
				{ 
					menu_execution(mitem_ptr+menuid[menuidx],KEY_ENTER); 
				} 
				break; 
			} 
		case KEY_CANCL:
			{ 
				if ((mitem_ptr+menuid[menuidx])->Parentms !=Null) 
				{ 
					mitem_ptr=(mitem_ptr+menuid[menuidx])->Parentms; 
					menuidx--; 
					title_show(mitem_ptr+menuid[menuidx]); 
				} 
				else 
				{ 
					//printf("You are at the top of menu"); 
				} 
				break; 
			} 
		default:  break; 
	} 

}
