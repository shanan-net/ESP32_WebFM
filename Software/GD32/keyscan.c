/**
  ******************************************************************************
  * @file    keyscan.c
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
	
/*!
    \file  keyscan.c
    \brief 
*/

#include "keyscan.h"
#include "systick.h"
#include "menu.h"

#define KEY_DEBOUNCE_TIME    1 // N*flag_10ms
#define KEY_LONG_TIME        100  // N*flag_10ms
#define KEY_REPEAT_TIME      12   // N*flag_10ms

typedef union{
	uint8_t state;
	struct{
		uint8_t state_wait	:1;
		uint8_t state_debounce :1;
		uint8_t state_process	:1;
		uint8_t state_longpress	:1;
		uint8_t volu_notify	:1;
		uint8_t chan_notify	:1;
		uint8_t flag_keypress	:1;
		uint8_t flag_keyrelease	:1;
	}mb;
}keystate_typedef_bit;

__IO uint16_t keytime;
__IO uint8_t keytemp;
__IO uint8_t keyhit;
__IO uint8_t keyval,prekeyval;

keystate_typedef_bit keystate;

__I uint8_t keytab[32]={
//single and multipressed key:short time 
		0,            	//0000b : S1 S2 S3 S4 Bit Value
		KEY_ENTER,      //0001b	//TK3
		KEY_CANCL,      //0010b	//TK4
		0,              //0011b
	  KEY_INPDN,			//0100b	//TK1
	  0,							//0101b
		0,							//0110b
		0,							//0111b
		KEY_INPUP,			//1000b	//TK2
		0,							//1001b
		0,							//1010b
		0,							//1011b
		0,	        		//1100b
		0,							//1101b
		0,							//1110b
		0, 					  	//1111b
//single and multipressed key:long time		
		0,            	// 0000b : S1 S2 S3 S4 Bit Value
		0, 					    // 0001b
		0,   				    // 0010b
		0,    				  // 0011b
	  KEY_INPDN,			//0100b
	  0,							//0101b
		0,							//0110b
		0,							//0111b
		KEY_INPUP,			//1000b
		0,							//1001b
		0,							//1010b
		0,							//1011b
		0,	  					//1100b
		0,							//1101b
		0,							//1110b
		0,							//1111b
};

void nullFunc(uint8_t nul)
{
	
}

void(*keyfunclist[6])(uint8_t)={
	menu_show,
	menu_show,
	menu_show,
	menu_show,
};

#define KEY_VALID_NUM 4
__I uint8_t keyfuncmap[KEY_VALID_NUM][2]={
	{KEY_ENTER,0},
	{KEY_CANCL,1},
	{KEY_INPDN,2},
	{KEY_INPUP,3},
};

void keyInit(void)
{
	/* enable the led clock */
	rcu_periph_clock_enable(RCU_GPIOA);
	/* configure led GPIO port */ 
	gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,GPIO_PIN_0);
	gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP,GPIO_PIN_1);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
	gpio_bit_set(GPIOA,GPIO_PIN_1);
	
	keystate.state = 0;
	keystate.mb.state_wait = 1;
	keytime = 0;
	keyval=prekeyval=0;
}

//8MHz,delay_us(20)=42us
void delay_us(uint8_t t)
{
	uint8_t i,j;
	for(i=0;i<t;i++)
		for(j=0;j<1;j++)
			__nop();
}


uint8_t keyHitRead(void)
{
	uint8_t keytmp=0,i=0,j=0;
	
	for(i=0;i<8;i++){
		gpio_bit_reset(GPIOA,GPIO_PIN_1);
		delay_us(20);
		gpio_bit_set(GPIOA,GPIO_PIN_1);
		delay_us(20);
		keytmp |= gpio_input_bit_get(GPIOA,GPIO_PIN_0)<<i;	
	}

	if(keytmp&0x80){
		for(i=0;i<4;i++){
			j+=((keytmp>>i)&0x01?0:1);
		}
		if(j==((keytmp>>4)&0x07)){
			return ((~keytmp)&0x0F);
		}else{
			return 0;
		}
	}
	else{
		return 0;
	}
}

uint8_t keyvalRead(void)
{
	uint8_t i;

	if ( keytime >= KEY_LONG_TIME ){
		i = 16 + keytemp;
	}else{
		i = keytemp;
	}
	
	if(i>31)i=0;
	
	return keytab[i];
}

void keyPressProcess(void)
{
	uint8_t i=0;
	for(i=0;i<KEY_VALID_NUM;i++){
		if(keyval==keyfuncmap[i][0]){
			keyfunclist[keyfuncmap[i][1]](keyval);
			break;
		}
	}
}

void keyScanFSM(void)
{
	/*******key scan program************/
	keyhit = keyHitRead();
	if ( keystate.mb.state_wait ){
		keytemp=keyhit;             //read the key to temp
		if ( KEY_NULL != keyhit ){
			 keytime = 0;              //start count
			 keystate.state =0;
			 keystate.mb.state_debounce = 1;
			 keyval = 0;
			 prekeyval = 0;
			 return;
		}
	}
	
	if ( keystate.mb.state_debounce ){
		if ( keyhit == keytemp ){
			 keytime++;
			 if ( keytime >= KEY_DEBOUNCE_TIME ){
					keystate.state = 0;
					keystate.mb.state_process = 1;
			 }
		}
		else{
			 keystate.state = 0;
			 keystate.mb.state_wait = 1;
		}
	}
	
	if ( keystate.mb.state_process ){
		if ( keyhit == keytemp){
			if ( keytime < 255 )keytime++;

			keyval = keyvalRead();
			 
			if ( prekeyval != keyval ){
				prekeyval = keyval;
				keystate.mb.flag_keypress = 1;
			}

			if ( keytime >= KEY_LONG_TIME ){
				keytime = 0;
				keystate.state = 0;
				keystate.mb.state_longpress = 1;
			}
		}
		else{
			 keyval = keyvalRead();
			 keystate.mb.flag_keyrelease = 1; 
			 keystate.state = 0;
			 keystate.mb.state_wait =1;               
		}
	}
	
	if ( keystate.mb.state_longpress ){
		if ( keyhit == keytemp){
			 keytime++;
			 if ( keytime >= KEY_REPEAT_TIME ){
					keytime = 0;
					keystate.mb.flag_keypress = 1;
			 }
		}
		else{
			 keyval = keyvalRead();
			 keystate.mb.flag_keyrelease = 1; 
			 keystate.state = 0;
			 keystate.mb.state_wait =1;               
		}            
	}
	/*******key scan program************/	 
}

bool testKeyPress(void)
{
	return (keystate.mb.flag_keypress==1?true:false);
}

bool testKeyRelease(void)
{
	return (keystate.mb.flag_keyrelease==1?true:false);
}

void clearKeyPress(void)
{
	keystate.mb.flag_keypress=0;
}

void clearKeyRelease(void)
{
	keystate.mb.flag_keyrelease=0;
}
