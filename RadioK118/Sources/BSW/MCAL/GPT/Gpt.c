/** Copyright (c) 2018 EMBITEL
**
** This software is the property of Embitel.
** It can not be used or duplicated without Embitel authorization.
**
** -------------------------------------------------------------------------------------------------
** File Name    : Gpt.c
** Module name  : GENERAL PURPOSE TIMER
** -------------------------------------------------------------------------------------------------
** Description : Include file of component Gpt.c
**
** -------------------------------------------------------------------------------------------------
**
****************************************************************************************************
** R E V I S I O N  H I S T O R Y
****************************************************************************************************
** V01.00
** - First release
*/

/**************************************** Inclusion files *****************************************/
#include "Gpt.h"

/**************************************Component configuration ************************************/

/* 1ms timer for normal operations */
static volatile uint32_t  GPT_Cntr1ms;
/* Timer trigger Flag */
static boolean GPT_CH0_TrigFlag;

/**************************************Function definitions ***************************************/
/***************************************************************************************************
 *
** Function name: GPT_Init
** Description: Timer Initialization function
** Parameter index : None
** Return value: None
** Remarks: global variables used, side effects
***************************************************************************************************/
void GPT_init(void)
{
	#ifdef FIRC_CLOCKSELECTION
	/* Clock source = 6 (FIRCDIV2_CLK) */
	PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(FIRCDIV2_CLK);
	#else
	/* Clock source = 3 (FIRCDIV2_CLK) */
	PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(SOSCDIV2_CLK);
    #endif
	
	/* Enable clock to GPT registers */
	PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK;

	/* DBG_EN-0: Timer channel stop in Debug mode */
	/* DOZE_EN=0: Timer channel are stopped in DOZE mode */
	/* SW_RST=0: SW reset does not reset timer channel, registers */
	/* M_CEN=1: enable module clock (allows writing other GPT registers)*/
	GPT->MCR = MCR_MASK;

	/* Enable interrupt for timer Channel 0 */
	GPT->MIER = TIE0_MASK;

	/* IRQ20-GPT ch0: clear any pending IRQ*/
	Clr_Pending_IRQ(LPIT0_IRQn);

	/* IRQ20-GPT ch0: enable IRQ */
	Enable_IRQ(LPIT0_IRQn);

	/* IRQ20-GPT ch0: set priority 10 */
	Set_IRQ_Priority(LPIT0_IRQn,IRQ_PRIORITY);

	/* TVAL 0 Timeout period */
	GPT->TMR[0].TVAL = TVAL_MASK;

	/* TCTRL=1: Timer channel is enabled */
	GPT->TMR[0].TCTRL = TCTRL_MASK;
}

/***************************************************************************************************
** Function name: GPT_Sleep
** Description: GPT_Sleep
** Parameter index : None
** Return value: None
** Remarks: global variables used, side effects
***************************************************************************************************/
void GPT_Sleep(void)
{
	/* Disable timer channel 0 interrupt */
	GPT->MIER &= ~(LPIT_MIER_TIE0_MASK);

	/* Clear channel 0 timer interrupt flag */
	GPT->MSR &= ~(LPIT_MSR_TIF0_MASK);

	/* Disable timer channel 0 */
	GPT->CLRTEN &= ~(LPIT_CLRTEN_CLR_T_EN_0_MASK);
}

/***************************************************************************************************
** Function               : GPT_GetElapsedTime
** Description            : Calculates the time period difference in 1ms,
                            between provided input and the free running timer
** Parameter  timetMarkVal  : Mark timer value to know the time difference.
** Return value           : uint32 - Time difference in 1ms/bit resolution
** Remarks                : None
***************************************************************************************************/
uint32_t GPT_GetElapsedTime(uint32_t timetMarkVal)
{
    uint32_t retDiff;             			/* local to calculate difference time in 1ms */
    uint32_t timetCt1msCpy;        			/* local to copy running counter value */

	/* make a copy of the free running 1ms counter */
    timetCt1msCpy = GPT_Cntr1ms;				

    /* has the running counter crossed the 32-bit limit at least once */
    if(timetCt1msCpy < timetMarkVal)
    {
        /* YES: then its an overflow, so take care with difference     */
        /* however multiple such overflows will be treated just as one */
        retDiff = timetCt1msCpy + ((uint32_t)0xFFFFFFFFU - timetMarkVal);
    }
    else
    {
        /* NO: no overflow, normal difference gives time elapsed */
        retDiff = timetCt1msCpy - timetMarkVal;
    }

    /* return time elapsed in 1ms resolution */
    return (retDiff);
}

/***************************************************************************************************
** Function                 : LPIT0_IRQHandler
** Description              : Scheduler timer tick ISR
** Parameter                : None
** Return value             : None
** Remarks                  : None
***************************************************************************************************/
void LPIT0_IRQHandler(void) {
    if (GPT_Cntr1ms == 0xFFFFFFFFU) {
        /* Reset the counter to avoid overflow */
        GPT_Cntr1ms = 0U;
    } else {
        /* Increment global 1ms counter */
        GPT_Cntr1ms++;
    }
    /* Clear GPT timer flag 0 */
    GPT->MSR |= LPIT_MSR_TIF0_MASK;

    if (FALSE == GPT_CH0_TrigFlag) {
        /* Set the Trigger Flag */
        GPT_CH0_TrigFlag = TRUE;
    } else {
        /* Do nothing */
    }
}

/***************************************************************************************************
** Function name: GPT_GetSystemTime
** Description:  Returns free running timer value
** Parameter index : Description
** Return value: GPT_Cntr1ms
***************************************************************************************************/
uint32_t GPT_GetSystemTime(void)
{
	return(GPT_Cntr1ms);
}

/***************************************************************************************************
** Function name: GPT_Get1msTrig
** Description:  Returns timer flag
** Parameter index : Description
** Return value: GPT_CH0_TrigFlag
***************************************************************************************************/
boolean GPT_Get1msTrig(void)
{
    /* Return  GPT_CH0_TrigFlag status */
    return (GPT_CH0_TrigFlag);
}

/***************************************************************************************************
** Function name: GPT_Reset1msTrig
** Description:  Reset Timer Flag
** Parameter index : Description
** Return value: None
***************************************************************************************************/
void GPT_Reset1msTrig(void)
{
	/* Reset GPT_Ch0_TrigFlag */
	GPT_CH0_TrigFlag = FALSE;
}

/***************************************************************************************************
** Function name: Clr_Pending_IRQ
** Description: Clear pending interrupt
** Parameter index: IRQn_Type irqNumber - Vector number
** Return value: None
***************************************************************************************************/
void Clr_Pending_IRQ(IRQn_Type irqNumber)
{
	 S32_NVIC->ICPR[(uint32_t)(irqNumber) >> 5U] = (uint32_t)(1UL << ((uint32_t)(irqNumber) &
			 	 	 	 	 	 	 	 	 	   (uint32_t)0x1FU));
}

/***************************************************************************************************
** Function name: Enable_IRQ
** Description: Enable interrupt
** Parameter index : IRQn_Type irqNumber - Vector number
** Return value: None
***************************************************************************************************/
void Enable_IRQ(IRQn_Type irqNumber)
{
	S32_NVIC->ISER[(uint32_t)(irqNumber) >> 5U] = (uint32_t)(1UL << ((uint32_t)(irqNumber) &
												  (uint32_t)0x1FU));
}

/***************************************************************************************************
** Function name: Set_IRQ_Priority
** Description: Set interrupt priority
** Parameter index : IRQn_Type irqNumber - Vector number
** 				   : uint8_t priority - Interrupt priority
** Return value: None
***************************************************************************************************/
void Set_IRQ_Priority(IRQn_Type irqNumber, uint8_t priority)
{
	uint8_t shift = (uint8_t) (8U - FEATURE_NVIC_PRIO_BITS);

    uint32_t iprVectorId = (uint32_t)(irqNumber) >> 2U;
    uint8_t priByteShift = (uint8_t)((((uint8_t)irqNumber) & 0x3U) << 3U);

    /* Clear the old value from the register */
    S32_NVIC->IPR[iprVectorId] &= ~(0xFFUL << priByteShift);

    S32_NVIC->IPR[iprVectorId] |= ((uint32_t)(((((uint32_t)priority) << shift)) & 0xFFUL))
								  << priByteShift;
}
/*******************************************************************************
** Function               : GPT_Get1ms

** Description            : Returns GPT_Get1ms status

** Parameter              : None

** Return value           : boolean - 1msTrig status

** Remarks                : None
*******************************************************************************/
uint32_t GPT_Get1ms(void) {
    return (GPT_Cntr1ms);
}

void GPT_Reset1msVal(void) {
    //Pinchi 20190115 add
	/* Reset GPT_Cntr1ms */
	GPT_Cntr1ms = 0U;
}

/*****************************************************************************
*  Name        : TimerSet
*  Description : Sets a timer. timer must not equ 0 because 0 means timer is stop
*  Params      : STimer pointer to timer value
*                TimeLength - timer period
*  Returns     : none
*****************************************************************************/
void TimerSet(uint32_t *STimer, uint32_t TimeLength) {
    *STimer = GPT_Cntr1ms + TimeLength;

    if(*STimer == 0) {
        *STimer = 1; //not set timer to 0 for timer is running
    }
}

/*****************************************************************************
*  Name        : TimerStop
*  Description : stop timer
*  Params      : STimer pointer to timer value
*  Returns     :
*****************************************************************************/
void TimerStop(uint32_t *STimer) {
    *STimer = 0;
}

/*****************************************************************************
*  Name        : TimerHasStopped
*  Description : Checks whether given timer has stopped
*  Params      : STimer is timer value
*  Returns     : TRUE if timer is stopped
*****************************************************************************/
uint8_t TimerHasStopped(uint32_t STimer) {
    return (STimer == 0);
}

/*****************************************************************************
*  Name        : TimerHasExpired
*  Description : Checks whether given timer has expired
*        With timer tick at 1ms maximum timer period is 10000000 ticks
*        When *STimer is set (SoftTimer-*STimer) has a min value of 0xFFF0BDBF
*            and will be more than this while the timer hasn't expired
*        When the timer expires
*                (SoftTimer-*STimer)==0
*            and (SoftTimer-*STimer)<=7FFF FFFF for the next 60 hours
*  Params      : STimer pointer to timer value
*  Returns     :  TRUE if timer has expired or timer is stopped, otherwise FALSE
*****************************************************************************/
uint8_t TimerHasExpired (uint32_t *STimer) {
    if(*STimer == 0) {
        return (TRUE) ;
    }
    else if((GPT_Cntr1ms - *STimer) <= 0x7fffffff) {
        *STimer = 0;    //set timer to stop
        return (TRUE) ;
    }

    return (FALSE) ;
}
