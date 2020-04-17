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
#include "clockMan1.h"
#include "board.h"

ftm_state_t ftmStateStruct ;

/**************************************Component configuration ************************************/

/* 1ms timer for normal operations */
//static volatile uint32_t  GPT_Cntr1ms = 0 ;
volatile uint32_t  GPT_Cntr1ms = 0 ;

/**************************************Function definitions ***************************************/
void ftmTimerISR(void) {
#if(0)
    if (GPT_Cntr1ms == 0xFFFFFFFFU) {
        /* Reset the counter to avoid overflow */
        GPT_Cntr1ms = 0U;
    } else {
        /* Increment global 1ms counter */
        GPT_Cntr1ms++;
    }
#else
    /* Increment global 1ms counter */
    GPT_Cntr1ms++;
#endif  // #if(0)
    /* Clear FTM Timer Overflow flag */
    FTM_DRV_ClearStatusFlags(INST_FLEXTIMER_MC1, (uint32_t)FTM_TIME_OVER_FLOW_FLAG);
}   // void ftmTimerISR(void)
/***************************************************************************************************
 *
** Function name: GPT_Init
** Description: Timer Initialization function
** Parameter index : None
** Return value: None
** Remarks: global variables used, side effects
***************************************************************************************************/
void GPT_init(void) {
    /* Initialize Flex Timer instance as simple timer */
    FTM_DRV_Init(INST_FLEXTIMER_MC1, &flexTimer_mc1_InitConfig, &ftmStateStruct);

    /* Install handler for the Timer overflow interrupt and enable it */
    INT_SYS_InstallHandler(FTM0_Ovf_Reload_IRQn, &ftmTimerISR, (isr_t*) 0U);
    INT_SYS_EnableIRQ(FTM0_Ovf_Reload_IRQn);

    /* Setup the counter to trigger an interrupt every 100 us */
    FTM_DRV_InitCounter(INST_FLEXTIMER_MC1, &flexTimer_mc1_TimerConfig);
    /* Start the counter */
    FTM_DRV_CounterStart(INST_FLEXTIMER_MC1);
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
** Function name: GPT_GetSystemTime
** Description:  Returns free running timer value
** Parameter index : Description
** Return value: GPT_Cntr1ms
***************************************************************************************************/
uint32_t GPT_GetSystemTime(void)
{
	return(GPT_Cntr1ms);
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
    //*STimer = GPT_Cntr1ms + TimeLength;
    *STimer = GPT_Cntr1ms + TimeLength * 10 ;

    if(*STimer == 0U) {
        //*STimer = 1U; //not set timer to 0 for timer is running
        *STimer = 10U; //not set timer to 0 for timer is running
    }
}

void TimerSet100us(uint32_t *STimer, uint32_t TimeLength) {
    *STimer = GPT_Cntr1ms + TimeLength/10 ;

    if(*STimer == 0U) {
        //*STimer = 1U; //not set timer to 0 for timer is running
        *STimer = 10U; //not set timer to 0 for timer is running
    }
}

/*****************************************************************************
*  Name        : TimerStop
*  Description : stop timer
*  Params      : STimer pointer to timer value
*  Returns     :
*****************************************************************************/
void TimerStop(uint32_t *STimer) {
    *STimer = 0U;
}

/*****************************************************************************
*  Name        : TimerHasStopped
*  Description : Checks whether given timer has stopped
*  Params      : STimer is timer value
*  Returns     : TRUE if timer is stopped
*****************************************************************************/
uint8_t TimerHasStopped(uint32_t STimer) {
    return (STimer == 0U);
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
inline uint8_t TimerHasExpired (uint32_t *STimer) {
#if (0)
    if(*STimer == 0) {
        return (TRUE) ;
    }
    else if((GPT_Cntr1ms - *STimer) <= 0x7fffffffU) {
        *STimer = 0;    //set timer to stop
        return (TRUE) ;
    }
#else
    int32_t     diff = (int32_t)(GPT_Cntr1ms) ;

    diff = (int32_t)(diff - ((int32_t)(*STimer))) ;
    if (*STimer == 0 || diff >= 0) {
        //set timer to stop
        *STimer = 0;

        return (TRUE) ;
    }   // if (*STimer == 0 || diff >= 0)
#endif  // #if (0)

    return (FALSE) ;
}
