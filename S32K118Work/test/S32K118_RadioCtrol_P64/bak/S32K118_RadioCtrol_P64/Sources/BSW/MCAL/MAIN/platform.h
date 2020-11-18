/******************************************************************************
*
* ALLGO Inc.
* (c) Copyright 2020 ALLGO Inc.
* ALL RIGHTS RESERVED.
***************************************************************************//*!
*
* @file		platform.h
*
* @author	ALLGO
*
* @brief	
*
*******************************************************************************
* History:
* 
******************************************************************************/

#ifndef BSW_MCAL_MAIN_PLATFORM_H_
	#define BSW_MCAL_MAIN_PLATFORM_H_

    #ifdef __cplusplus
        extern "C" {
    #endif

    /* Including necessary module. Cpu.h contains other modules needed for compiling.*/
	#include "Cpu.h"

	#include "pin_mux.h"
	#include "dmaController1.h"
	#include "clockMan1.h"
	#include "lpuart1.h"
    #include "lpTmr1.h"

    /* User includes (#include below this line is not maintained by Processor Expert) */
    #include <string.h>
    #include <stdint.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include <stdio.h>

    #include "typedefs.h"
    #include "board.h"
    #include "gpt.h"
    #include "uart.h"
    //#include "i2c_master.h"

    #include "tda75610sep.h"

    #ifdef __cplusplus
        }
    #endif
#endif	/* BSW_MCAL_MAIN_PLATFORM_H_ */
/******************************************************************************
* end of file (platform.h)
******************************************************************************/
