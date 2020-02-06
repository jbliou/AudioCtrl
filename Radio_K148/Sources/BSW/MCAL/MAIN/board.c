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
     *  -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

#ifdef EVB
    PINS_DRV_ClearPins(LED_RED_PORT ,   (1 << LED_RED));
    PINS_DRV_ClearPins(LED_GREEN_PORT , (1 << LED_GREEN));
    PINS_DRV_ClearPins(LED_BLUE_PORT ,  (1 << LED_BLUE));
#endif  // #ifdef EVB

    TimerSet (&sys_tmr_led, 100) ;
}   // void board_init(void)

void Handle_LED(void) {
    if (TimerHasExpired(&sys_tmr_led) == TRUE) {
#ifdef EVB
        PINS_DRV_TogglePins(LED_BLUE_PORT , (1 << LED_BLUE));
#endif  // #ifdef EVB
        TimerSet (&sys_tmr_led, 200) ;
    }   // if (TimerHasExpired(&sys_tmr_led) == TRUE)
}
