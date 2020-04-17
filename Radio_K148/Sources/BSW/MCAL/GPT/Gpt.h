
/** Copyright (c) 2018 EMBITEL
**
** This software is the property of Embitel.
** It can not be used or duplicated without Embitel authorization.
**
** -------------------------------------------------------------------------------------------------
** File Name    : Gpt.h
** Module name  : GENERAL PURPOSE TIMER
** -------------------------------------------------------------------------------------------------
** Description : Include file of component Gpt.h
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
/* To avoid multi-inclusions */
#ifndef GPT_H
#define GPT_H
#include "Platform_Types.h"
/************************************Function definitions ****************************************/
/***************************************************************************************************
** Function name: GPT_init
** Description: GPT_init
** Parameter index : Description
** Return value: None
** Remarks: global variables used, side effects
***************************************************************************************************/
extern void GPT_init(void);
/***************************************************************************************************
** Function                 : GPT_GetElapsedTime
** Description              : Time period difference in 1ms
** Parameter                : None
** Return value             : None
** Remarks                  : None
***************************************************************************************************/
extern uint32_t GPT_GetElapsedTime(uint32_t timetMarkVal);
/***************************************************************************************************
** Function name: GPT_GetSystemTime
** Description:  Returns free running timer value
** Parameter index : Description
** Return value: GPT_Cntr1ms
** Remarks: global variables used, side effects
***************************************************************************************************/
extern uint32_t GPT_GetSystemTime(void);
/***************************************************************************************************
** Function name: GPT_Get1msTrig
** Description:  Returns timer flag
** Parameter index : Description
** Return value: GPT_Ch0_TrigFlag
***************************************************************************************************/
extern boolean GPT_Get1msTrig(void);
/***************************************************************************************************
** Function name: GPT_Reset1msTrig
** Description:  Reset Timer Flag
** Parameter index : Description
** Return value: None
***************************************************************************************************/
extern void GPT_Reset1msTrig(void);
/*******************************************************************************
** Function               : Timer_Get1ms

** Description            : Returns Timer_Get1ms status

** Parameter              : None

** Return value           : boolean - 1msTrig status

** Remarks                : None
*******************************************************************************/
extern uint32_t GPT_Get1ms(void);

extern void GPT_Reset1msVal(void);  //Pinchi 20190115 add

/*****************************************************************************
*  Name        : TimerSet
*  Description : Sets a timer. timer must not equ 0 because 0 means timer is stop
*  Params      : STimer pointer to timer value
*                TimeLength - timer period
*  Returns     : none
*****************************************************************************/
extern void TimerSet(uint32_t *STimer, uint32_t TimeLength) ;
extern void TimerSet100us(uint32_t *STimer, uint32_t TimeLength) ;
/*****************************************************************************
*  Name        : TimerStop
*  Description : stop timer
*  Params      : STimer pointer to timer value
*  Returns     :
*****************************************************************************/
extern void TimerStop(uint32_t *STimer);
/*****************************************************************************
*  Name        : TimerHasStopped
*  Description : Checks whether given timer has stopped
*  Params      : STimer is timer value
*  Returns     : TRUE if timer is stopped
*****************************************************************************/
extern uint8_t TimerHasStopped(uint32_t STimer) ;
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
extern uint8_t TimerHasExpired (uint32_t *STimer) ;
#endif /*end of GPT.H*/

