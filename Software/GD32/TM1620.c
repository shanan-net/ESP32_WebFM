/**
  ******************************************************************************
  * @file    TM1620.c
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
#include "TM1620.h"
#include "spi.h"
#include "systick.h"

#define CMD_MODE	0x00
#define CMD_DATA	0x40
#define CMD_CTRL	0x80
#define CMD_ADDR	0xC0

void tm1620_Init(void)
{
	spi_config();
	spi_open();
	//��ʾģʽ��������:6 λ 8 ��
	spi_cs_enable();
	spi_sendbyte(0x02);
	spi_cs_disable();
	delay_1ms(2);
  //�����������ã��Զ���ַ����
	spi_cs_enable();
	spi_sendbyte(0x40);
	spi_cs_disable();
	delay_1ms(2);
}

void tm1620_DispOn(void)
{
	//��ʾ���أ���ʾ���ȵ��ڣ�2/16����
	spi_cs_enable();
	spi_sendbyte(0x89);
	spi_cs_disable();
	delay_1ms(2);
}

void tm1620_DispOff(void)
{
	//��ʾ���أ���ʾ���ȵ��ڣ�1/16����
	spi_cs_enable();
	spi_sendbyte(0x80);
	spi_cs_disable();
	delay_1ms(2);
}

void tm1620_DispRefresh(uint8_t *dispbuf)
{
	uint8_t i=0;

	spi_cs_enable();
	//������ʼ��ַ
	spi_sendbyte(0xC0);
	for(i=0;i<6;i++){
		spi_sendbyte(*(dispbuf+i));
		spi_sendbyte(0);
	}
	spi_cs_disable();
}
