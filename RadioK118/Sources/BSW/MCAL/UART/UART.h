/*
 * UART.h
 *
 *  Created on: 2020�~1��3��
 *      Author: ALLGO
 */

#ifndef BSW_MCAL_UART_UART_H_
    #define BSW_MCAL_UART_UART_H_
    enum {
        UART_STATE_INIT         = 0 ,
        UART_STATE_ACTIVE           ,
        UART_STATE_DEACTIVE         ,
    };

    /* Receive buffer size */
    #define BUFFER_SIZE             (2U)
    #define BUFFER_SIZE_RX          (256U)
    #define BUFFER_SIZE_GPS         (128U)

    #if defined (__UART_INS__)
        uint8_t uart_state = UART_STATE_INIT ;
        uint8_t uart_queue [BUFFER_SIZE_RX] ;
        uint8_t uart_buf [BUFFER_SIZE] ;

        #undef  __UART_INS__
    #else
        extern void Handle_UART (void) ;
        extern status_t GetUARTData (uint8_t * ret_buf) ;
        extern uint8_t GetUARTStatus (void) ;
    #endif  // #if defined (__UART_INS__)

#endif /* BSW_MCAL_UART_UART_H_ */
