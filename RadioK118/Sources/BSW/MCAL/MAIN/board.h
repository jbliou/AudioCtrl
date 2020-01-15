/*
 * board.h
 *
 *  Created on: 2020�~1��13��
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
		#define LED_GREEN       15U   /* pin PTD15 - RGB LED on DEV-KIT */
		#define LED_GREEN_PORT	PTD   /* GPIO PORT D */

		#define LED_RED       	16U   /* pin PTD15 - RGB LED on DEV-KIT */
		#define LED_RED_PORT   	PTD   /* GPIO PORT D */

		#define LED_BLUE      	8U   /* pin PTD15 - RGB LED on DEV-KIT */
		#define LED_BLUE_PORT   PTE   /* GPIO PORT D */

		#define BTN_SW2			3U   /* pin PTD3 - SW2 on DEV-KIT */
		#define BTN_SW2_PORT   	PTD   /* GPIO PORT D */

		#define BTN_SW3			5U   /* pin PTD5 - SW3 on DEV-KIT */
		#define BTN_SW3_PORT   	PTD   /* GPIO PORT D */
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
