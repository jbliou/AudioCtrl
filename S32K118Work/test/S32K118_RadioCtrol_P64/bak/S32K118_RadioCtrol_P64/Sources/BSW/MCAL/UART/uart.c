/******************************************************************************
*
* ALLGO Inc.
* (c) Copyright 2020 ALLGO Inc.
* ALL RIGHTS RESERVED.
***************************************************************************//*!
*
* @file		uart.c
*
* @author	JBLIOU
*
* @brief	
*
*******************************************************************************
* History:
* 
******************************************************************************/
#include "platform.h"
#define _UART_INS_
#include "uart.h"

status_t Handle_UART (void) {
	if (handle_uart_state < UART_MAX) {
		(UART_process[handle_uart_state])() ;
	}	// if (handle_uart_state < UART_MAX)
	else {
		handle_uart_state = UART_ERROR ;
	}
	if (handle_uart_state == UART_NORMAL
	 || handle_uart_state == UART_ERROR) {
		return (STATUS_SUCCESS) ;
	}
	return (STATUS_BUSY) ;
}	// status_t Handle_UART (void)

void UART_process_IDLE(void) {
    memset (uart_queue, 0, sizeof(uart_queue)/sizeof(uint8_t)) ;
    uart_queue_rx_idx  = 0 ;
    uart_queue_tx_idx  = 0 ;
    // nmea_parse_init() ;
    handle_uart_state   = UART_INIT_PARAMETER ;
}	// void UART_process_IDLE(void)

void UART_process_INIT_PARAMETER(void) {
	/* Initialize LPUART instance */
	LPUART_DRV_Init(INST_LPUART1
			       , &lpuart1_State
				   , &lpuart1_InitConfig0
				   );

	/* Install the callback for rx events */
	LPUART_DRV_InstallRxCallback(INST_LPUART1
								, rxCallback
								, NULL
								);

    LPUART_DRV_ReceiveData(INST_LPUART1
                          , (uint8_t *)(uart_queue+uart_queue_rx_idx)
                          , 1U
                          );
    handle_uart_state   = UART_NORMAL ;
}	// void UART_process_INIT_PARAMETER(void)

void UART_process_UART_NORMAL(void) {
    uint32_t    bytesRemaining ;
    status_t    ret_state = STATUS_SUCCESS ;

    /* Check the status */
    ret_state = LPUART_DRV_GetReceiveStatus(INST_LPUART1, &bytesRemaining)  ;
    NOP() ;
    if (ret_state == STATUS_BUSY) {
        uint16_t    tmp_idx = uart_queue_tx_idx ;

        if (tmp_idx != uart_queue_rx_idx) {
            // uart queue is not empty
            uart_queue_tx_idx++ ;
            if (uart_queue_tx_idx >= MAX_UART_BUFFER) {
                uart_queue_tx_idx = 0 ;
            }   // if (uart_buffer_tx_idx >= MAX_UART_BUFFER)

            //NMEA_PARSE_HANDLER((uint8_t *)(uart_queue + tmp_idx)) ;
        }	// if (tmp_idx != uart_queue_rx_idx)
    }	// if (ret_state == STATUS_BUSY)
    else {
        LPUART_DRV_ReceiveData(INST_LPUART1
                              , (uint8_t *)(uart_queue+uart_queue_rx_idx)
                              , 1U
                              );
    }
}	// void UART_process_UART_NORMAL(void)

void UART_process_UART_ERROR(void) {
    LPUART_DRV_Deinit(INST_LPUART1) ;

    handle_uart_state   = UART_IDLE ;
}	// void UART_process_UART_ERROR(void)

void UART_process_UART_nop(void) {
}	// void UART_process_UART_nop(void)

/* UART rx callback for continuous reception, byte by byte */
void rxCallback(void *driverState, uart_event_t event, void *userData) {
    /* Unused parameters */
    (void)driverState;
    (void)userData;

    /* Check the event type */
    if (event == UART_EVENT_RX_FULL) {
        if (handle_uart_state == UART_NORMAL) {
            uint16_t    tmp_idx = uart_queue_rx_idx ;

            /* Update the buffer index and the rx buffer */
            tmp_idx++;
            if (tmp_idx >= MAX_UART_BUFFER) {
                tmp_idx = 0 ;
            }   // if (tmp_idx >= MAX_UART_BUFFER)
            if (tmp_idx != uart_queue_tx_idx) {
                uart_queue_rx_idx = tmp_idx ;
            }   // if (tmp_idx == uart_buffer_tx_idx)
            else {
                // buffer overflow
            }
        }   // if (handle_uart_state != UART_NORMAL)
        LPUART_DRV_SetRxBuffer(INST_LPUART1
                              , (uint8_t *)(uart_queue + uart_queue_rx_idx)
                              , 1U
                              );
    }	// if (event == UART_EVENT_RX_FULL)
    else {
    	/* Process the following event here :
			//!< Tx buffer is empty
			UART_EVENT_TX_EMPTY     = 0x01U,
			//!< The current transfer is ending
			UART_EVENT_END_TRANSFER = 0x02U,
			//!< An error occured during transfer
			UART_EVENT_ERROR        = 0x03U,
    	 */
    }
}	// void rxCallback(void *driverState, uart_event_t event, void *userData)
#undef	_UART_INS_
/******************************************************************************
* end of file (uart.c)
******************************************************************************/
