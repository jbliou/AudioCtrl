/*******************************************************************************
** Copyright (c) 2018 Embitel
**
** This software is the property of Embitel.
** It can not be used or duplicated without Embitel authorization.
**
** -----------------------------------------------------------------------------
** File Name : Mcu.c
** Module Name : MCU
** -----------------------------------------------------------------------------
**
** Description : Micro-controller Clock Unit Module
**
** -----------------------------------------------------------------------------
**
** Documentation reference : LLD_MCU
**
********************************************************************************
** R E V I S I O N H I S T O R Y
********************************************************************************
** V01.00 06/08/2018
** - Baseline Created
*******************************************************************************/
/*************************** Inclusion files **********************************/

#include "board.h"

/*******************************************************************************
** FUNCTIONS **
*******************************************************************************/

/******************** Internal functions declarations *************************/

/************************** Function definitions ******************************/
/*******************************************************************************
** Function name: Mcu_Init.
** Description: Initialization of MCU Module.
** Parameter index : Initialization of Clock Registers.
** Return value: None
** Remarks: None
*******************************************************************************/
void Mcu_Init(void)
{
#if (0)
	/* SOSCDIV1 & SOSCDIV2 =1: divide by 1 */
	 SCG->SOSCDIV = SCG_SOSCDIV_SOSCDIV1(1U) | SCG_SOSCDIV_SOSCDIV2(1U);

	 /* FIRCDIV1 & FIRCDIV2 =1: divide by 1 */
	 SCG->FIRCDIV = SCG_FIRCDIV_FIRCDIV1(1U) | SCG_FIRCDIV_FIRCDIV2(1U);
	 
	 /* configuring the external reference clock and and 1=LOW FREQ 2=MEDIUM
	  *  1-8MHz 3=8-40MHz*/
	 SCG->SOSCCFG =  SCG_SOSCCFG_RANGE(MCU_RANGE_CLK) | SCG_SOSCCFG_EREFS(1U);

	 /* Ensure SOSCCSR unlocked */
	 while(SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK);

	 /*  Enable oscillator */
	 SCG->SOSCCSR=SCG_SOSCCSR_SOSCEN(1U);

	 /* Wait for system  OSC clock valid */
	 while(!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK));
#else
	    CLOCK_SYS_Init ( g_clockManConfigsArr,
	                     CLOCK_MANAGER_CONFIG_CNT,
	                     g_clockManCallbacksArr,
	                     CLOCK_MANAGER_CALLBACK_CNT
	                   ) ;
	    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT) ;
#endif  // #if (0)
}
