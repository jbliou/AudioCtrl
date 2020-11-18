/******************************************************************************
*
* ALLGO Inc.
* (c) Copyright 2020 ALLGO Inc.
* ALL RIGHTS RESERVED.
***************************************************************************//*!
*
* @file		board.c
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
#define _BOARD_INS_
#include "board.h"


void board_init(void) {
    /* Initialize and configure clocks
     *     -    see clock manager component for details
     */
    CLOCK_SYS_Init(g_clockManConfigsArr
                  , CLOCK_MANAGER_CONFIG_CNT
                  , g_clockManCallbacksArr
                  , CLOCK_MANAGER_CALLBACK_CNT);

    CLOCK_SYS_UpdateConfiguration(0U
                                 , CLOCK_MANAGER_POLICY_AGREEMENT
                                 );

    /* Initialize pins
     *    -    See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

}   // void board_init(void)

#undef	_BOARD_INS_
/******************************************************************************
* end of file (board.c)
******************************************************************************/
