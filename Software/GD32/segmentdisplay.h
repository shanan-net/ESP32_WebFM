/**
  ******************************************************************************
  * @file    segmentdisplay.h
  * @author  shanan.net@qq.com
  * @version
  * @date    2017-01-21
  * @brief   ¶ÎÂëÏÔÊ¾Çý¶¯
  ******************************************************************************
  */
#ifndef _SEGMENTDISPLAY_H_
#define _SEGMENTDISPLAY_H_

#include "gd32f1x0.h"
#include <stdbool.h>

// disp state define
#define ST_POWER_UP   		 0x00
#define	ST_CLOCK		   		 0x01
#define ST_SETTINGS   		 0x02

#define CHAR_MINUS 16  //At dstable[16]

//#define BIT(A) (1<<A)

/*
    ___ a  
f  |___| b ___g
e  |___| c  .dp
      d
*/

#define DS8A      0
#define DS8B      1
#define DS8C      2
#define DS8D      3
#define DS8E      4
#define DS8F      5
#define DS8G      6
#define DS8DP     7

#define C_0   (BIT(DS8A)|BIT(DS8B)|BIT(DS8C)|BIT(DS8D)|BIT(DS8E)|BIT(DS8F))  // '0'
#define C_1   (BIT(DS8B)|BIT(DS8C))  // '1'
#define C_2   (BIT(DS8A)|BIT(DS8B)|BIT(DS8D)|BIT(DS8E)|BIT(DS8G)) // '2'
#define C_3   (BIT(DS8A)|BIT(DS8B)|BIT(DS8C)|BIT(DS8D)|BIT(DS8G)) // '3'
#define C_4   (BIT(DS8F)|BIT(DS8G)|BIT(DS8B)|BIT(DS8C)) // '4'
#define C_5   (BIT(DS8A)|BIT(DS8F)|BIT(DS8G)|BIT(DS8C)|BIT(DS8D)) // '5'
#define C_6   (BIT(DS8A)|BIT(DS8F)|BIT(DS8G)|BIT(DS8C)|BIT(DS8D)|BIT(DS8E)) // '6'
#define C_7   (BIT(DS8A)|BIT(DS8B)|BIT(DS8C)) // '7'
#define C_8   (BIT(DS8A)|BIT(DS8B)|BIT(DS8C)|BIT(DS8D)|BIT(DS8E)|BIT(DS8F)|BIT(DS8G)) // '8'
#define C_9   (BIT(DS8A)|BIT(DS8B)|BIT(DS8C)|BIT(DS8D)|BIT(DS8F)|BIT(DS8G)) // '9'
#define C_A   (BIT(DS8A)|BIT(DS8B)|BIT(DS8C)|BIT(DS8F)|BIT(DS8E)|BIT(DS8G)) // 'A'
#define C_B   (BIT(DS8F)|BIT(DS8E)|BIT(DS8D)|BIT(DS8G)|BIT(DS8C)) // 'b'
#define C_C   (BIT(DS8A)|BIT(DS8F)|BIT(DS8E)|BIT(DS8D)) // 'C'
#define C_D   (BIT(DS8B)|BIT(DS8C)|BIT(DS8D)|BIT(DS8E)|BIT(DS8G)) // 'd'
#define C_E   (BIT(DS8A)|BIT(DS8G)|BIT(DS8D)|BIT(DS8F)|BIT(DS8E)) // 'E'
#define C_F   (BIT(DS8A)|BIT(DS8G)|BIT(DS8F)|BIT(DS8E)) // 'F'
#define C_N   (BIT(DS8A)|BIT(DS8B)|BIT(DS8C)|BIT(DS8E)|BIT(DS8F)) // 'N'
#define C_NL  (BIT(DS8E)|BIT(DS8C)|BIT(DS8G)) // 'n'
#define C_CL  (BIT(DS8E)|BIT(DS8D)|BIT(DS8G)) // 'c'
#define C_P   (BIT(DS8A)|BIT(DS8B)|BIT(DS8E)|BIT(DS8F)|BIT(DS8G)) // 'P'
#define C_MLINE   (BIT(DS8G)) // '-'
#define C_TLINE   (BIT(DS8A)) // '-'
#define C_BLINE   (BIT(DS8D)) // '_'
#define C_DOT   (BIT(DS8DP)) // '.'
#define C_H   (BIT(DS8B)|BIT(DS8C)|BIT(DS8E)|BIT(DS8F)|BIT(DS8G)) // 'H'
#define C_HL   (BIT(DS8C)|BIT(DS8E)|BIT(DS8F)|BIT(DS8G)) // 'h'
#define C_U  (BIT(DS8B)|BIT(DS8C)|BIT(DS8D)|BIT(DS8E)|BIT(DS8F)) // 'U'
#define C_UL   (BIT(DS8C)|BIT(DS8D)|BIT(DS8E)) // 'u'
#define C_L   (BIT(DS8D)|BIT(DS8E)|BIT(DS8F)) // 'L'
#define C_T   (BIT(DS8D)|BIT(DS8E)|BIT(DS8F)|BIT(DS8G)) // 't'
#define C_OL   (BIT(DS8D)|BIT(DS8E)|BIT(DS8C)|BIT(DS8G)) // 'o'
#define C_RL   (BIT(DS8B)|BIT(DS8C)|BIT(DS8G)) // 'r'
#define C_V   (BIT(DS8B)|BIT(DS8G)|BIT(DS8E)|BIT(DS8F)) // 'V'
#define C_ALL   (BIT(DS8A)|BIT(DS8B)|BIT(DS8C)|BIT(DS8D)|BIT(DS8E)|BIT(DS8F)|BIT(DS8G)|BIT(DS8DP)) // '8.'
#define C_OFF   (uint8_t)~(BIT(DS8A)|BIT(DS8B)|BIT(DS8C)|BIT(DS8D)|BIT(DS8E)|BIT(DS8F)|BIT(DS8G)|BIT(DS8DP)) // OFF

typedef struct{
   uint8_t year;
   uint8_t leapyear;
   uint8_t month;
   uint8_t day;
   uint8_t week;
   uint8_t hour;
   uint8_t minute;
   uint8_t second;
}CLOCK_VAR;

void segDispInit(void);
void segrefresh( uint8_t *dispbuf, uint8_t len );
void itodigit( int16_t n, uint8_t s[], uint8_t base);
unsigned char animateDisplay( unsigned char *buf, unsigned char len );
unsigned char rotate180( unsigned char dssegdata );
void display2Num( unsigned char num1, unsigned char num2, unsigned char base );
void bitFlash( unsigned char combit );
void displayNum(unsigned short num, unsigned char base );

#endif
