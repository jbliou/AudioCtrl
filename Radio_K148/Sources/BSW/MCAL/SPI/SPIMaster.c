/*
 * SPIMaster.c
 *
 *  Created on: 2020年1月21日
 *      Author: ALLGO
 */

#include "board.h"

#define __SPIMASTER_INS__
#include "SPIMaster.h"

void Handle_SPI(void) {
    if (spi_state < SPI_STATE_MAX) {
        (Handle_SPI_FUNC[spi_state]) () ;
    }
    else {
        spi_state = SPI_STATE_INIT ;
    }
}   // void Handle_SPI(void)

void Handle_SPI_INIT(void) {
    SPI_OP_STATE.isPcsContinuous = true ;
    LPSPI_DRV_MasterInit (spi_op_ins, &SPI_OP_STATE, &SPI_OP_CONFIG) ;
    TimerStop(&timer_spi) ;
    spi_state = SPI_STATE_IDLE ;
}   // void Handle_SPI_INIT(void)

void Handle_SPI_IDLE(void) {
#if (0)
    if (TimerHasExpired(&timer_spi) == TRUE || TimerHasStopped(timer_spi) == TRUE) {
        spi_buf [0] = 0xc2 ;
        spi_buf [1] = 0 ;
        TimerSet(&timer_spi, 50) ;
        LPSPI_DRV_MasterTransfer(spi_op_ins, spi_buf, spi_buf+ 10, 2);
        spi_state = SPI_STATE_BUSY ;
    }
#endif  // #if (0)
}   // void Handle_SPI_IDLE(void)

void Handle_SPI_BUSY(void) {
    status_t    read_state ;
    uint32_t    bytesRemaining ;

    read_state = LPSPI_DRV_MasterGetTransferStatus(spi_op_ins, &bytesRemaining) ;
    if (read_state == STATUS_SUCCESS) {
        spi_state = SPI_STATE_WAIT ;
        TimerSet(&timer_spi, 50) ;
    }
    else if (read_state == STATUS_ERROR) {
        LPSPI_DRV_MasterAbortTransfer(spi_op_ins) ;
        TimerSet(&timer_spi, 50) ;
        spi_state = SPI_STATE_ABORT ;
    }
}   // void Handle_SPI_BUSY(void)

void Handle_SPI_WAIT(void) {
    if (TimerHasExpired(&timer_spi) == TRUE) {
        spi_state = SPI_STATE_IDLE ;
    }   // if (TimerHasExpired(&timer_spi) == TRUE)
}   // void Handle_SPI_WAIT(void)

status_t GetSPIState(void) {
    if (spi_state == SPI_STATE_IDLE || spi_state == SPI_STATE_WAIT) {
        return (STATUS_SUCCESS) ;
    }   // if (spi_state == SPI_STATE_IDLE)
    else if (spi_state == SPI_STATE_ABORT) {
        return (STATUS_ERROR) ;
    }

    return (STATUS_BUSY) ;
}

status_t SetSPIFree(void) {
    if (spi_state == SPI_STATE_IDLE || spi_state == SPI_STATE_WAIT || spi_state == SPI_STATE_ABORT) {
        spi_state = SPI_STATE_IDLE ;
        TimerStop(&timer_spi) ;

        return (STATUS_SUCCESS) ;
    }   // if (spi_state == SPI_STATE_WAIT || spi_state == SPI_STATE_IDLE || spi_state == SPI_STATE_ABORT)

    return (STATUS_ERROR) ;
}   // void SetSPIFree(void)

status_t SPIAccess(const uint8_t * sendBuffer, uint8_t * receiveBuffer, uint16_t transferByteCount) {
    if (spi_state == SPI_STATE_IDLE) {
        LPSPI_DRV_MasterTransfer(spi_op_ins, sendBuffer, receiveBuffer, transferByteCount);
        spi_state = SPI_STATE_BUSY ;
        TimerSet(&timer_spi, 50) ;

        return (STATUS_SUCCESS) ;
    }   // if (spi_state == SPI_STATE_IDLE)

    return (STATUS_ERROR) ;
}

void Set_SPI_Attribute(bool flag) {
    if (spi_state != SPI_STATE_BUSY) {
        SPI_OP_STATE.isPcsContinuous = flag ;
        LPSPI_DRV_MasterInit (spi_op_ins, &SPI_OP_STATE, &SPI_OP_CONFIG) ;
        spi_state = SPI_STATE_IDLE ;
    }   // if (spi_state != SPI_STATE_BUSY)
}   // void Handle_SPI_INIT(void)
