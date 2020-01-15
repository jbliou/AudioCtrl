/*
 * board.c
 *
 *  Created on: 2020年1月14日
 *      Author: ALLGO
 */

#define __BOARD_INS__
#include "board.h"

void board_init(void) {
    /* Initialize pins
     *    -    See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    PINS_DRV_ClearPins(LED_RED_PORT ,   (1 << LED_RED));
    PINS_DRV_ClearPins(LED_GREEN_PORT , (1 << LED_GREEN));
    PINS_DRV_ClearPins(LED_BLUE_PORT ,  (1 << LED_BLUE));

    TimerSet (&sys_tmr_led, 100) ;
}   // void board_init(void)

void Handle_LED(void) {
    if (TimerHasExpired(&sys_tmr_led) == TRUE) {
        PINS_DRV_TogglePins(LED_BLUE_PORT , (1 << LED_BLUE));
        TimerSet (&sys_tmr_led, 100) ;
    }   // if (TimerHasExpired(&sys_tmr_led) == TRUE)
}
