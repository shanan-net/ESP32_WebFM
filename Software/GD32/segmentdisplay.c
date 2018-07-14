/**
  ******************************************************************************
  * @file    segmentdisplay.c
  * @author  shanan.net@qq.com
  * @version
  * @date    2017-01-21
  * @brief   段码显示驱动
  ******************************************************************************
  */
#include "segmentdisplay.h"
#include "TM1620.h"

uint8_t g_dispbuf[6];
uint8_t bitstringbuf[10];
uint8_t dispstate,subdispstate;
CLOCK_VAR clock_time;
bool  flag_secflash,flag_dispflash=false;
uint8_t animatebuf[12];

const uint8_t builtdate[12]=__DATE__;
const uint8_t builttime[12]=__TIME__;

/*
    ___ a  
f  |___| b  ___g
e  |___| c  .dp
      d
*/
//共阴数码管
__I uint8_t dstable[17]={
   C_0,   // '0'
   C_1,   // '1'
   C_2,   // '2'
   C_3,   // '3'
   C_4,  // '4'
   C_5, // '5'
   C_6, // '6'
   C_7, // '7'
   C_8, // '8'
   C_9, // '9'
   C_A, // 'A'
   C_B, // 'b'
   C_C, // 'C'
   C_D, // 'd'
   C_E, // 'E'
   C_F, // 'F'
   C_MLINE // '-'
};
void displayAdapter( uint8_t *dispbuf );

void segDispInit(void)
{
	tm1620_Init();
}

void segrefresh( uint8_t *dispbuf, uint8_t len )
{
	uint8_t i;
	
	if(len>6)len=6;
	
	for(i=0;i<len;i++)
		g_dispbuf[i] = *(dispbuf+i);
	
	displayAdapter(g_dispbuf);
	
	tm1620_DispRefresh(g_dispbuf);
	
	tm1620_DispOn();
}

void itodigit( int16_t n, uint8_t s[], uint8_t base)
{
   uint8_t i=0, j=0;
   uint8_t sign=0;
   
   if( n<0 )//记录符号
   {
      sign = 1;
      n = -n;//使n成为正数
   }
   
   i = 0;
   do
   {
      s[i]=n%base;//取下一个数字
      i++;
   }while ((n/=base)>0);//删除该数字
   
   if( sign )
   {
      s[i]=CHAR_MINUS;
      for ( j=0; j<((i+1)/2); j++ )//生成的数字是逆序的，所以要逆序输出
      {
         sign = s[j];
         s[j] = s[i-j];
         s[i-j] = sign;
      }
   }
   else
   {
      for ( j=0; j<(i/2); j++ )//生成的数字是逆序的，所以要逆序输出
      {
         sign = s[j];
         s[j] = s[i-1-j];
         s[i-1-j] = sign;
      } 
      
      if ( 1==i )
      {
         s[1] = s[0];
         s[0] = 0;//补零
      }
   }
}

uint8_t animateDisplay( uint8_t *buf, uint8_t len )
{
   static uint8_t anicnt=0;
   uint8_t i;
   for ( i=0;i<4;i++ )
   {
      if ( (i+anicnt) >= len )
      {
				g_dispbuf[i]=*(buf+i+anicnt-len);
      }
      else
      {

				g_dispbuf[i]=*(buf+i+anicnt);
      }
   }
   anicnt++;
   if ( anicnt>=len )
   {
      anicnt = 0;
      return 0;
   }
   else
      return 1;
}

/*
dp.  ___ d  
  c |___| e  ___g
  b |___| f  
         a
*/
//旋转180度		
uint8_t rotate180( uint8_t dssegdata )
{
   uint8_t tp1, tp2;
   tp1=tp2= dssegdata; 
   tp1 >>= 3;
   tp2 <<= 3;
   dssegdata &= 0xC0;
   dssegdata |= (tp1&0x07)|(tp2&0x38);
   return dssegdata;
}

void display2Num( uint8_t num1, uint8_t num2, uint8_t base )
{
   itodigit( num1, bitstringbuf, base );
   if ( num1<base )
   {
      g_dispbuf[0] = C_OFF;
      g_dispbuf[1] = dstable[ bitstringbuf[1] ];               
   }
   else
   {
      g_dispbuf[0] = dstable[ bitstringbuf[0] ];
      g_dispbuf[1] = dstable[ bitstringbuf[1] ];               
   }
   
   itodigit( num2, bitstringbuf, base );
   if ( num2<base )
   {
      g_dispbuf[2] = C_0;
      g_dispbuf[3] = dstable[ bitstringbuf[1] ];                  
   }
   else
   {
      g_dispbuf[2] = rotate180( dstable[ bitstringbuf[0] ] );
      g_dispbuf[3] = dstable[ bitstringbuf[1] ];               
   }  
}

void bitFlash( unsigned char combit )
{
   if ( flag_dispflash )
   {
      flag_dispflash = false;
      if ( combit&0x08 )
      {
         g_dispbuf[0] = 0xFF;
      }
      if ( combit&0x04 )
      {
         g_dispbuf[1] = 0xFF;
      }
      if ( combit&0x02 )
      {
         g_dispbuf[2] = 0xFF;
      }
      if ( combit&0x01 )
      {
         g_dispbuf[3] = 0xFF;
      }
   }
   else
   {
      flag_dispflash = true;
   }
}

void displayNum(unsigned short num, unsigned char base )
{
   itodigit( num, bitstringbuf, base );     
   if ( num<base )
   {
      g_dispbuf[0] = C_OFF;
      g_dispbuf[1] = C_OFF;
      g_dispbuf[2] = C_OFF;
      g_dispbuf[3] = dstable[ bitstringbuf[1] ];               
   }
   else if ( num<(base*base) )
   {
      g_dispbuf[0] = C_OFF;
      g_dispbuf[1] = C_OFF;
      g_dispbuf[2] = dstable[ bitstringbuf[0] ];  
      g_dispbuf[3] = dstable[ bitstringbuf[1] ];              
   }
   else if ( num<(base*base*base))
   {
      g_dispbuf[0] = C_OFF;
      g_dispbuf[1] = dstable[ bitstringbuf[0] ];     
      g_dispbuf[2] = dstable[ bitstringbuf[1] ];   
      g_dispbuf[3] = dstable[ bitstringbuf[2] ];              
   }
   else
   {
      g_dispbuf[0] = dstable[ bitstringbuf[0] ]; 
      g_dispbuf[1] = dstable[ bitstringbuf[1] ];     
      g_dispbuf[2] = dstable[ bitstringbuf[2] ];   
      g_dispbuf[3] = dstable[ bitstringbuf[3] ];                
   }    

	displayAdapter(g_dispbuf);
	
	tm1620_DispRefresh(g_dispbuf);
	
	tm1620_DispOn();	 
}

void displayAdapter( uint8_t *dispbuf )
{
	uint8_t tempbuf[6],i;
	
	tempbuf[0]=*dispbuf;//DIG1
	tempbuf[1]=*(dispbuf+1);//DIG2
	tempbuf[5]=*(dispbuf+2);//DIG6
	tempbuf[2]=*(dispbuf+3);//DIG3
	tempbuf[4]=0x0F;//DIG5:ABCD:LED
	tempbuf[3]=0x00;//DIG4:NOT USED
	
	for(i=0;i<6;i++)*(dispbuf+i)=tempbuf[i];
}
