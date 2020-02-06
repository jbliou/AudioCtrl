/*
 * UART.c
 *
 *  Created on: 2020�~1��6��
 *      Author: ALLGO
 */

#include "board.h"

#define __UART_INS__
#include "Uart.h"

#include "GPS.h"

void rxCallback(void *driverState, uart_event_t event, void *userData) {
    /* Unused parameters */
    (void)driverState;
    (void)userData;

    /* Check the event type */
    if (event == UART_EVENT_RX_FULL) {
    }
    else if (event == UART_EVENT_ERROR) {
        //uart_state = UART_STATE_REINIT ;
        LPUART_DRV_ReceiveData_Queue(INST_LPUART2, (uint8_t *)uart_buf , sizeof(uart_buf));
    }
}   // void rxCallback(void *driverState, uart_event_t event, void *userData)

void Handle_UART(void)  {
    uint8_t tmp ;

    switch (uart_state) {
        case UART_STATE_INIT :
            /* Initialize LPUART instance */
            LPUART_DRV_Init(INST_LPUART2, &lpuart2_State, &lpuart2_InitConfig0);
            /* Install the callback for rx events */
            LPUART_DRV_InstallRxCallback(INST_LPUART2, rxCallback, NULL);

            LPUART_DRV_SetRxQueue (INST_LPUART2, (uint8_t *)uart_queue , BUFFER_SIZE_RX);
            LPUART_DRV_ReceiveData_Queue(INST_LPUART2, (uint8_t *)uart_buf , sizeof(uart_buf));

            uart_state = UART_STATE_ACTIVE ;
            break ;
        case UART_STATE_REINIT :
            LPUART_DRV_Deinit(INST_LPUART2) ;
            uart_state = UART_STATE_INIT ;
            break ;
        case UART_STATE_ACTIVE :
        	Handle_GPS() ;
            break ;
        case UART_STATE_DEACTIVE :
            LPUART_DRV_GetReceiveQueueStatus(INST_LPUART2, &tmp) ;
            break ;
    }
}   // void Handle_UART(void)
