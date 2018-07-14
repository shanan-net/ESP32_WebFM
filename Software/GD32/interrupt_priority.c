/**
  ******************************************************************************
  * @file    interrupt_priority.c
  * @author  shanan.net@qq.com
  * @version
  * @date    2018-07-14
  * @brief   
  ******************************************************************************
  */
/*!
    \file  interrupt_priority.c
    \brief 
*/

#include "interrupt_priority.h"

/**
    \brief      configure the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
  */
void nvic_config(void)
{
	nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
	nvic_irq_enable(TIMER1_IRQn, 1, 6);
}
