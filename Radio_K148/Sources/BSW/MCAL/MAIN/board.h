/*
 * board.h
 *
 *  Created on: 2020/1/13
 *      Author: ALLGO
 */

/* Including necessary module. Cpu.h contains other modules needed for compiling.*/
#include "Cpu.h"
#include "Platform_Types.h"
#include "Mcu.h"
#include "Gpt.h"
#include "Wdt.h"

#ifndef MCAL_MAIN_BOARD_H_
	#define MCAL_MAIN_BOARD_H_

	#define EVB

	#ifdef EVB
		#define LED_GREEN       22U   /* pin PTE22 - Green LED on DEV-KIT */
		#define LED_GREEN_PORT	PTE   /* GPIO PORT E */

		#define LED_RED       	21U   /* pin PTE21 - Red LED on DEV-KIT */
		#define LED_RED_PORT   	PTE   /* GPIO PORT E */

		#define LED_BLUE      	23U   /* pin PTE23 - Blue LED on DEV-KIT */
		#define LED_BLUE_PORT   PTE   /* GPIO PORT E */

		#define BTN_SW2			12U   /* pin PTC12 - SW2 on DEV-KIT */
		#define BTN_SW2_PORT   	PTC   /* GPIO PORT C */

		#define BTN_SW3			13U   /* pin PTC13 - SW3 on DEV-KIT */
		#define BTN_SW3_PORT   	PTC   /* GPIO PORT C */
	#else
		#define LED_GREEN       1U    /* pin PTC1 - LED1 on Motherboard */
		#define LED_GPIO_PORT   PTC   /* GPIO PORT C */
	#endif

    typedef void (* PHANDLE)(void) ;

    #if defined (__BOARD_INS__)
        uint32_t    sys_tmr_led ;

        #undef  __BOARD_INC__
    #else
        extern void board_init(void) ;
        extern void Handle_LED(void) ;
    #endif  // #if defined (__BOARD_INS__)
#endif /* MCAL_MAIN_BOARD_H_ */
