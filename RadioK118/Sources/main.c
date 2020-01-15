/* ###################################################################
 **     Filename    : main.c
 **     Processor   : S32K1xx
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.00
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */

/* Including necessary module. Cpu.h contains other modules needed for compiling.*/
volatile int exit_code = 0;

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "board.h"
#include "Uart.h"

void Handle_NOP (void) ;

const PHANDLE pHandle[] = {
    Handle_UART ,
    Handle_LED ,
    Handle_NOP
} ;

typedef enum {
    HANDLE_PHASE_START ,
    HANDLE_PHASE_UART = HANDLE_PHASE_START ,
    HANDLE_PHASE_LED ,
    HANDLE_PHASE_NOP ,
    HANDLE_PHASE_MAX
} MAIN_HANDLE_PHASE ;
/*! 
 \brief The main function for the project.
 \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
 */
int main(void) {
    /* Write your local variable definition here */
    uint32_t            sys_tmr_1ms;
    MAIN_HANDLE_PHASE   mhandle_phase = HANDLE_PHASE_START ;

    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
    /*** End of Processor Expert internal initialization.                    ***/

    /* Write your code here */
    Mcu_Init();
    WDT_Init();
    WDT_Disable();
    GPT_init();
    board_init();
    /* For example: for(;;) { } */
    TimerSet(&sys_tmr_1ms, 1);
    while (true) {
        if (TimerHasExpired(&sys_tmr_1ms) == TRUE) {
            TimerSet(&sys_tmr_1ms, 1);
        } else {
            if (mhandle_phase < HANDLE_PHASE_MAX) {
                (pHandle[mhandle_phase++])() ;
                if (mhandle_phase >= HANDLE_PHASE_MAX) {
                    mhandle_phase = HANDLE_PHASE_START ;
                }   // if (mhandle_phase >= HANDLE_PHASE_MAX)
            } // if (mhandle_phase < HANDLE_PHASE_MAX)
            else {
                mhandle_phase = HANDLE_PHASE_START ;
            }
        }
    }   // while(true)

    /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
    /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
#ifdef PEX_RTOS_START
    PEX_RTOS_START(); /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
#endif
    /*** End of RTOS startup code.  ***/
    /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
    for (;;) {
        if (exit_code != 0) {
            break;
        }
    }
    return exit_code;
    /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

void Handle_NOP (void) {
}   // void Handle_NOP (void)
/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.1 [05.21]
 **     for the NXP S32K series of microcontrollers.
 **
 ** ###################################################################
 */
