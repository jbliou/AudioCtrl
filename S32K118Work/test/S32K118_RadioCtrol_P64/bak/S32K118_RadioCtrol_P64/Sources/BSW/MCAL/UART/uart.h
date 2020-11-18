/******************************************************************************
*
* ALLGO Inc.
* (c) Copyright 2020 ALLGO Inc.
* ALL RIGHTS RESERVED.
***************************************************************************//*!
*
* @file		uart.h
*
* @author	JBLIOU
*
* @brief	
*
*******************************************************************************
* History:
* 
******************************************************************************/

#ifndef BSW_MCAL_UART_UART_H_
	#define BSW_MCAL_UART_UART_H_

    typedef enum {
        UART_IDLE
        , UART_INIT_PARAMETER
        , UART_NORMAL
        , UART_ERROR
        , UART_MAX
    } HANDLE_UART_STATE ;

    /* Receive buffer size */
    #define MAX_UART_BUFFER         (200U)
#endif	/* BSW_MCAL_UART_UART_H_ */

#if defined(_UART_INS_)
    volatile HANDLE_UART_STATE  handle_uart_state = UART_IDLE ;
    volatile uint16_t           uart_queue_rx_idx = 0 ;
    volatile uint16_t           uart_queue_tx_idx = 0 ;
    char                        uart_queue[MAX_UART_BUFFER+2] ;

    void rxCallback(void *driverState, uart_event_t event, void *userData) ;

    void UART_process_IDLE(void) ;
    void UART_process_INIT_PARAMETER(void) ;
    void UART_process_UART_NORMAL(void) ;
    void UART_process_UART_ERROR(void) ;
    void UART_process_UART_nop(void) ;

    const pvFUNCv UART_process [] = {
		/* UART_IDLE                */ UART_process_IDLE ,
        /* UART_INIT_PARAMETER      */ UART_process_INIT_PARAMETER ,
        /* UART_NORMAL              */ UART_process_UART_NORMAL ,
        /* UART_ERROR               */ UART_process_UART_ERROR ,
        /* UART_MAX					*/ UART_process_UART_nop
    };
#else
    extern status_t Handle_UART (void) ;
    extern void clear_uart_buf (void) ;
#endif	// #if defined(_UART_INS_)
/******************************************************************************
* end of file (uart.h)
******************************************************************************/
