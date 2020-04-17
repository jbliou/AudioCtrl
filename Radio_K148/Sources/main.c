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
#include "board.h"
#include "UART.h"
#include "I2CMaster.h"
#include "tuner.h"
#include "SPIMaster.h"
#include "ExtFlash.h"
#include "codec.h"

volatile int exit_code = 0;

/* User includes (#include below this line is not maintained by Processor Expert) */
typedef enum {
    HANDLE_PHASE_START
    , HANDLE_PHASE_I2C = HANDLE_PHASE_START
    , HANDLE_PHASE_SPI
    , HANDLE_PHASE_UART
    , HANDLE_PHASE_TUNER
    , HANDLE_PHASE_EXTFLASH
    , HANDLE_PHASE_CODEC
    , HANDLE_PHASE_LED
    , HANDLE_PHASE_MAX
} HANDLE_PHASE;

void Handle_Nop(void) {
}

const PFUNC Handle_FUNC[] = {
    Handle_I2C
    //, Handle_Nop
    , Handle_SPI
    //, Handle_Nop
    , Handle_UART
    //, Handle_Nop
    , Handle_TUNER
    //, Handle_Nop
    , Handle_ExtFlash
    //, Handle_Nop
    , Handle_CODEC
    //, Handle_Nop
    , Handle_LED
    //, Handle_Nop
};

/*! 
 \brief The main function for the project.
 \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
 */
int main(void) {
    /* Write your local variable definition here */
    HANDLE_PHASE    handle_idx ;

    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
    /*** End of Processor Expert internal initialization.                    ***/

    /* Write your code here */
    WDT_Disable();
    Mcu_Init();
    board_init();
    GPT_init();
    handle_idx = HANDLE_PHASE_START ;
    /* For example: for(;;) { } */
    while (true) {
        if (handle_idx < HANDLE_PHASE_MAX) {
            (Handle_FUNC[handle_idx++])();
            if (handle_idx >= HANDLE_PHASE_MAX) {
                handle_idx = HANDLE_PHASE_START;
            }
            Handle_WHEELTICK() ;
        } else {
            handle_idx = HANDLE_PHASE_START;
        }
    }

    /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

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
