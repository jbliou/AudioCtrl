/**************************************************************************************************
** Copyright (c) 2018 Embitel
**
** This software is the property of Embitel.
** It can not be used or duplicated without Embitel authorization.
**
** ------------------------------------------------------------------------------------------------
** File Name     : WDT.c
** Module Name   : WDT
** ------------------------------------------------------------------------------------------------
**
** Description : Watchdog Target
**
** ------------------------------------------------------------------------------------------------
**
** Documentation reference :
**
***************************************************************************************************
** R E V I S I O N H I S T O R Y
***************************************************************************************************
** V01.00 10/10/2018
** - Baseline Created
**
***************************************************************************************************/
/************************************** Inclusion files *******************************************/
#include "WDT.h"

/********************************* Component configuration ****************************************/

/*********************** Declaration of local symbol and constants ********************************/

/****************************** Declaration of local macros ***************************************/

/****************************** Declaration of local types ****************************************/

/**************************** Declaration of local variables **************************************/


/**************************** Declaration of local constants **************************************/

/************************** Declaration of exported variables *************************************/

/*************************** Declaration of exported constant *************************************/

/*************************** Internal functions declarations **************************************/

/***************************************************************************************************
** FUNCTIONS **
***************************************************************************************************/
/***************************************************************************************************
** Function name    : WDT_Init
** Description      : This function Configures all WDOG registers.
** Parameter 		: None
** Return value     : None
** Remarks          : None
***************************************************************************************************/
void WDT_Init(void)
{
	if ((WDOG->CS & WDOG_CS_CMD32EN_MASK) != 0U)
	{
		WDOG->CNT = FEATURE_WDOG_UNLOCK_VALUE;
		(void)WDOG->CNT;
	}

	/* Configure timeout */
	WDOG->TOVAL = (uint32_t)WDT_CFG_TIMEOUT;

	WDOG->CS  = (uint32_t ) ((1UL << WDOG_CS_CMD32EN_SHIFT) | (0UL << 12 ) |
							(FEATURE_WDOG_CLK_FROM_LPO << WDOG_CS_CLK_SHIFT) |
									(1U << WDOG_CS_EN_SHIFT) |
								(1U << WDOG_CS_UPDATE_SHIFT));

	/* wait until new configuration takes effect */
	while(((WDOG->CS & WDOG_CS_RCS_MASK) >> WDOG_CS_RCS_SHIFT) == 0UL);
}

/***************************************************************************************************
** Function name    : WDT_Refresh
** Description      : This function Refreshes the WDOG counter.
** Parameter 		: None
** Return value     : None
** Remarks          : None
***************************************************************************************************/
void WDT_Refresh(void)
{
	/* For 32-bit write refresh write sequence, WDOG_CS[CMD32EN] should be 1*/
	if(((WDOG->CS & WDOG_CS_CMD32EN_MASK) >> WDOG_CS_CMD32EN_SHIFT) == 1UL)
	{
	/* Write of the WDOG unlock key to CNT register, must be done in order to allow any modifications*/
	WDOG->CNT = (uint32_t) FEATURE_WDOG_TRIGGER_VALUE;
	}
	else
	{
	/* Do Nothing */
	}
}

/***************************************************************************************************
** Function name    : WDT_ResetImmediate
** Description      : This function resets the WDOG.
** Parameter 		: None
** Return value     : None
** Remarks          : None
***************************************************************************************************/
void WDT_ResetImmediate(void)
{
	WDOG->CNT = 0x80;
}

/***************************************************************************************************
** Function name    : WDT_Disable
** Description      : This function disables the WDOG.
** Parameter 		: None
** Return value     : None
** Remarks          : None
***************************************************************************************************/
void WDT_Disable(void)
{
	/*unlock WatchDog*/
	WDOG->CNT = (uint32_t)FEATURE_WDOG_UNLOCK_VALUE;
	/* disable WatchDog */
	WDOG->CS &= ~WDOG_CS_EN_MASK;
	
}
