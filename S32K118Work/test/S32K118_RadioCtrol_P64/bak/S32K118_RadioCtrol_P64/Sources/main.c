/*
 * Copyright (c) 2015 - 2016 , Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
/* ###################################################################
**     Filename    : main.c
**     Project     : lpuart_echo_s32k118
**     Processor   : S32K118_64
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-08-04, 12:27, # CodeGen: 1
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

/* Including needed modules to compile this module/procedure */
#include "platform.h"

status_t ProcessHandle_NOP (void) ;

const psFUNCv DriverHandle [] = {
    ProcessHandle_NOP
    //, Handle_spi_master
    //, Handle_i2c_master
    , Handle_UART
    , ProcessHandle_NOP
} ;

const psFUNCv MiddleWareHandle [] = {
    ProcessHandle_NOP
    //, Handle_PwrAmpCtrl
    , ProcessHandle_NOP
} ;

#define MAX_DRIVER_HANDLER          (sizeof(DriverHandle)/sizeof(psFUNCv))
#define MAX_MIDDLEWARE_HANDLER      (sizeof(MiddleWareHandle)/sizeof(psFUNCv))


volatile int exit_code = 0;

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
status_t ProcessHandle_NOP (void) {
    return (STATUS_SUCCESS) ;
}   // status_t ProcessHandle_NOP (void)

/*!
 \brief The main function for the project.
 \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
 */
int main(void) {
    /* Write your local variable definition here */
    uint8_t     DriverHandle_Idx ;
    uint8_t     MiddlewareHandle_Idx ;

    DISABLE_INTERRUPTS();

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
	PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */
	board_init() ;

	//gpt_init() ;
	/* Infinite loop:
	 *     - Receive data from user
	 *     - Echo the received data back
	 */
    DriverHandle_Idx = 0 ;
    MiddlewareHandle_Idx = 0 ;
	while (1) {
        NOP() ;

        if (DriverHandle_Idx < MAX_DRIVER_HANDLER) {
            if (DriverHandle[DriverHandle_Idx]() == STATUS_SUCCESS) {
                DriverHandle_Idx++ ;
            }   // if (DriverHandle_Idx < MAX_DRIVER_HANDLER)
        }	// if (DriverHandle_Idx < MAX_DRIVER_HANDLER)
        else {
            DriverHandle_Idx = 0 ;
        }

        MiddleWareHandle[MiddlewareHandle_Idx]() ;
        if (MiddlewareHandle_Idx < MAX_MIDDLEWARE_HANDLER) {
            MiddlewareHandle_Idx++ ;
        }   // if (MiddlewareHandle_Idx < MAX_MIDDLEWARE_HANDLER)
        else {
            MiddlewareHandle_Idx = 0 ;
        }
	}	// while (1)

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
 **     for the Freescale S32K series of microcontrollers.
 **
 ** ###################################################################
 */

