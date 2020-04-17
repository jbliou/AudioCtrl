/*
 * SPIMaster.h
 *
 *  Created on: 2020年1月21日
 *      Author: ALLGO
 */

#ifndef BSW_MCAL_SPI_SPIMASTER_H_
    #define BSW_MCAL_SPI_SPIMASTER_H_

    #define SPI_OP_PORT     (LPSPICOM1)
    #define SPI_OP_STATE    (lpspiCom1State)
    #define SPI_OP_CONFIG   (lpspiCom1_MasterConfig0)

    #define MAX_SPI_BUF     (256U)

    typedef enum {
        SPI_STATE_INIT = 0
        , SPI_STATE_IDLE
        , SPI_STATE_BUSY
        , SPI_STATE_WAIT
        , SPI_STATE_ABORT
        , SPI_STATE_MAX
    } SPI_STATE ;

    #if defined (__SPIMASTER_INS__)
        uint32_t            timer_spi ;
        SPI_STATE           spi_state = SPI_STATE_INIT ;
        uint8_t             spi_buf [MAX_SPI_BUF] ;
        const uint32_t      spi_op_ins = SPI_OP_PORT ;

        void Handle_SPI_INIT(void) ;
        void Handle_SPI_IDLE(void) ;
        void Handle_SPI_BUSY(void) ;
        void Handle_SPI_WAIT(void) ;

        const PFUNC Handle_SPI_FUNC[] = {
            Handle_SPI_INIT
            , Handle_SPI_IDLE
            , Handle_SPI_BUSY
            , Handle_SPI_WAIT
            , Handle_SPI_WAIT
        };

        #undef __SPIMASTER_INS__
    #else
        extern void Handle_SPI(void) ;
        extern status_t GetSPIState(void) ;
        extern void Set_SPI_Attribute(bool flag) ;
        extern status_t SetSPIFree(void) ;
        extern status_t SPIAccess(const uint8_t * sendBuffer, uint8_t * receiveBuffer, uint16_t transferByteCount) ;
    #endif  // #if defined (__SPIMASTER_INS__)

#endif /* BSW_MCAL_SPI_SPIMASTER_H_ */
