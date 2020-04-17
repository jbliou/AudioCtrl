/*
 * board.c
 *
 *  Created on: 2020年1月14日
 *      Author: ALLGO
 */

#define __BOARD_INS__
#include "board.h"

#define pulse_per_tick          (4)

#define WEELTICK_1KMH           (28257/pulse_per_tick)      // 100us

void board_init(void) {
    /* Initialize pins
     *  -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

#ifdef EVB
    PINS_DRV_SetPins(nRESET_CODEC_PORT      , (1 << nRESET_CODEC));
    PINS_DRV_ClearPins(LED_RED_PORT         , (1 << LED_RED));
    PINS_DRV_ClearPins(LED_GREEN_PORT       , (1 << LED_GREEN));
    PINS_DRV_ClearPins(LED_BLUE_PORT        , (1 << LED_BLUE));
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
    //Handle_WHEELTICK () ;
}

void Handle_WHEELTICK(void) {
    if (vehicle_speed != vehicle_speed_old) {
        if (vehicle_speed == 0) {
            wheeltick_timeInterval = 0 ;
            PINS_DRV_ClearPins(LED_RED_PORT ,   (1 << LED_RED));
        }   // if (vehicle_speed == 0)
        else {
            wheeltick_timeInterval = WEELTICK_1KMH / vehicle_speed ;
            if (wheeltick_timeInterval < 21) {
                // 21 -> 2.1 ms, the vehicle speed is faster than 340 KmH
                wheeltick_timeInterval = 0 ;
            }   // if (wheeltick_timeInterval < 15)
        }

        if (wheeltick_timeInterval == 0) {
            PINS_DRV_ClearPins(LED_RED_PORT ,   (1 << LED_RED));
        }   // if (wheeltick_timeInterval == 0)
        vehicle_speed_old = vehicle_speed ;
    }   // if (vehicle_speed != vehicle_speed_old)

    if (wheeltick_timeInterval > 20) {
        if (TimerHasExpired(&tmr_wheeltick) == TRUE) {
            PINS_DRV_TogglePins(LED_RED_PORT , (1 << LED_RED));
            TimerSet100us (&tmr_wheeltick, wheeltick_timeInterval) ;
        }   // if (TimerHasExpired(&tmr_wheeltick) == TRUE)
    }   // if (wheeltick_timeInterval > 0)
}   // void Handle_WHEELTICK(void)
