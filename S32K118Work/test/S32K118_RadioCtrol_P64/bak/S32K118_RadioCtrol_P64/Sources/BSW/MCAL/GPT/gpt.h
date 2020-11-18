/******************************************************************************
*
* ALLGO Inc.
* (c) Copyright 2020 ALLGO Inc.
* ALL RIGHTS RESERVED.
***************************************************************************//*!
*
* @file		gpt.h
*
* @author	JBLIOU
*
* @brief	
*
*******************************************************************************
* History:
* 
******************************************************************************/

#ifndef BSW_MCAL_GPT_GPT_H_
	#define BSW_MCAL_GPT_GPT_H_

#endif	/* BSW_MCAL_GPT_GPT_H_ */

#if defined(_GPT_INS_)
    uint32_t    system_1ms ;

    void LPTMR_ISR(void) ;
#else
    extern void gpt_init(void) ;

    extern void TimerSet(uint32_t *STimer, uint32_t TimeLength) ;
    extern void TimerStop(uint32_t *STimer) ;
    extern uint8_t TimerHasStopped(uint32_t STimer) ;
    extern bool TimerHasExpired (uint32_t *STimer) ;
#endif	// #if defined(_GPT_INS_)
/******************************************************************************
* end of file (gpt.h)
******************************************************************************/
