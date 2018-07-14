/**
  ******************************************************************************
  * @file    timer.c
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
/*!
    \file  timer.c
    \brief 
*/

#include "timer.h"

__IO bool flag_10ms=false;

/**
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
  */
void timer_config(void)
{
	timer_parameter_struct timer_initpara;

	rcu_periph_clock_enable(RCU_TIMER1);

	timer_deinit(TIMER1);

	/* TIMER1 configuration */
	timer_initpara.prescaler         = 3999;
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	timer_initpara.period            = 19;
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER1,&timer_initpara);

	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(TIMER1);
	
	NVIC_SetPriority(TIMER1_IRQn, 0x01);
	
	timer_interrupt_enable(TIMER1, TIMER_INT_UP);
	 
	timer_enable(TIMER1);
}
