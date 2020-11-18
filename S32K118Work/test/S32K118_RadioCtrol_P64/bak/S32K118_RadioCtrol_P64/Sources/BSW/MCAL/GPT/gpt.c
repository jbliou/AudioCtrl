/******************************************************************************
*
* ALLGO Inc.
* (c) Copyright 2020 ALLGO Inc.
* ALL RIGHTS RESERVED.
***************************************************************************//*!
*
* @file		gpt.c
*
* @author	JBLIOU
*
* @brief	
*
*******************************************************************************
* History:
* 
******************************************************************************/
#include "platform.h"
#define	_GPT_INS_
#include "gpt.h"

void gpt_init(void) {
    /* Init LPTMR as timer
     *  -   interrupt after 1 second
     *  -   SIRC as clock source
     *  -   counter disabled
     */
    LPTMR_DRV_Init(INST_LPTMR1, &lpTmr1_config0, false);

    /* Install IRQ handler for LPTMR interrupt */
    INT_SYS_InstallHandler(LPTMR0_IRQn, &LPTMR_ISR, (isr_t *)0);

    /* Enable IRQ for LPTMR */
    INT_SYS_EnableIRQ(LPTMR0_IRQn);

    /* Start LPTMR counter */
    LPTMR_DRV_StartCounter(INST_LPTMR1);

    system_1ms = 0 ;
}   // void gpt_init(void)

/*!
 * @brief: LPMR interrupt handler.
 *         When an interrupt occurs clear channel flag.
 */
void LPTMR_ISR(void) {
    system_1ms++ ;
    /* Clear compare flag */
    LPTMR_DRV_ClearCompareFlag(INST_LPTMR1);
}   // void LPTMR_ISR(void)


/*****************************************************************************
*  Name        : TimerSet
*  Description : Sets a timer. timer must not equ 0 because 0 means timer is stop
*  Params      : STimer pointer to timer value
*                TimeLength - timer period
*  Returns     : none
*****************************************************************************/
void TimerSet(uint32_t *STimer, uint32_t TimeLength) {
    //*STimer = GPT_Cntr1ms + TimeLength;
    *STimer = system_1ms + TimeLength ;

    if(*STimer == 0U) {
        *STimer = 1U; //not set timer to 0 for timer is running
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
bool TimerHasExpired (uint32_t *STimer) {
    int32_t     diff = (int32_t)(system_1ms) ;

    diff = (int32_t)(diff - ((int32_t)(*STimer))) ;
#if (0)
    if (*STimer == 0 || diff >= 0) {
        //set timer to stop
        *STimer = 0;

        return (true) ;
    }   // if (*STimer == 0 || diff >= 0)
#else
    if (*STimer == 0) {
        return (false) ;
    }   // if (*STimer == 0)
    else if (diff >= 0) {
        //set timer to stop
        *STimer = 0;

        return (true) ;
    }   // if (diff >= 0)
#endif  // #if (0)

    return (false) ;
}   // bool TimerHasExpired (uint32_t *STimer)
#undef	_GPT_INS_
/******************************************************************************
* end of file (gpt.c)
******************************************************************************/
