/*
 * I2CMaster.c
 *
 *  Created on: 2020�~1��8��
 *      Author: ALLGO
 */

#include <string.h>
#include "board.h"

#define __I2CMASTER_INS__
#include "I2CMaster.h"

/* Allocate memory for the LPI2C driver state structure */
lpi2c_master_state_t lpi2c1MasterState ;

void Handle_I2C(void) {
    if (i2c_master_state < I2C_STATE_MAX) {
        (pHandleI2C[i2c_master_state])() ;
    }   // if (gps_handle_state < GPS_HANDLE_STATE_MAX)
    else {
        i2c_master_state = I2C_STATE_WAIT ;
    }
}   // void Handle_i2c(void)

status_t GetI2CState(void) {
    if (i2c_master_state != I2C_STATE_IDLE || req_i2c_state != I2C_STATE_IDLE) {
        return (STATUS_BUSY) ;
    }   // if (i2c_master_state != I2C_STATE_IDLE || req_i2c_state != I2C_STATE_IDLE)

    return (STATUS_SUCCESS) ;
}   // status_t GetI2CState(void)

uint8_t GetI2CExecuteState(void) {
    return (i2c_result) ;
}   // uint8_t GetI2CExecuteState(void)

int16_t I2C_WriteData (uint8_t req_slave_address, uint8_t * buf, uint16_t len) {
    req_i2c_slave_address = req_slave_address ;

    req_i2c_state = I2C_STATE_WRITE ;

    if (len <= MAX_I2C_BUF) {
        i2c_buf_len = len ;
    }
    else {
        i2c_buf_len = MAX_I2C_BUF ;
    }

    memcpy (i2c_buf, buf, i2c_buf_len) ;

    return (i2c_buf_len) ;
}   // int16_t I2C_WriteData (uint8_t req_slave_address, uint8_t * buf, uint16_t len)

void I2C_ReadData (uint8_t req_slave_address, uint8_t * buf, uint16_t len) {
    req_i2c_slave_address = req_slave_address ;
    req_i2c_state = I2C_STATE_READ ;
    i2c_tar_buf = buf ;
    i2c_tar_buf_len = len ;
}   // void I2C_ReadData (uint8_t req_slave_address, uint8_t * buf, uint16_t len)

int16_t I2C_WriteReadData (uint8_t req_slave_address, uint8_t * wbuf, uint16_t wlen, uint8_t * rbuf, uint16_t rlen) {
    req_i2c_slave_address = req_slave_address ;
    req_i2c_state = I2C_STATE_WRITE_THEN_READ ;
    i2c_tar_buf = rbuf ;
    i2c_tar_buf_len = rlen ;
    if (wlen <= MAX_I2C_BUF) {
        i2c_buf_len = wlen ;
    }
    else {
        i2c_buf_len = MAX_I2C_BUF ;
    }   // if (wlen <= MAX_I2C_BUF)

    memcpy (i2c_buf, wbuf, i2c_buf_len) ;

    return (i2c_buf_len) ;
}   // int16_t I2C_WriteReadData (uint8_t req_slave_address, uint8_t * wbuf, uint16_t wlen, , uint8_t * rbuf, uint16_t rlen)

void Handle_I2C_STATE_INIT (void) {

    /* Initialize LPI2C Master configuration
     *  - Slave address 0x01
     *  - Fast operating mode, 400 KHz SCL frequency
     *  -   See LPI2C components for configuration details
     */
    LPI2C_DRV_MasterInit(INST_LPI2C1, &lpi2c1_MasterConfig0, &lpi2c1MasterState);

    i2c_slave_address = (uint16_t)NULL ;
    req_i2c_slave_address = (uint16_t)NULL ;
    TimerStop(&tmr_i2c) ;
    i2c_tar_buf = (uint8_t *)NULL ;
    i2c_tar_buf_len = 0 ;

    i2c_master_state = I2C_STATE_IDLE ;
    req_i2c_state = I2C_STATE_IDLE ;
    i2c_result = I2C_IDLE ;
}   // void Handle_I2C_STATE_INIT (void)

void Handle_I2C_STATE_IDLE (void) {
    if (req_i2c_state != I2C_STATE_IDLE) {
        switch (req_i2c_state) {
            case I2C_STATE_WRITE :
                TimerStop(&tmr_i2c) ;
                //LPI2C_DRV_MasterInit(INST_LPI2C1, &lpi2c1_MasterConfig0, &lpi2c1MasterState);

                i2c_slave_address = req_i2c_slave_address ;
                i2c_master_state = req_i2c_state ;
                req_i2c_state = I2C_STATE_IDLE ;
                i2c_result = I2C_RUNNING ;
                break ;
            case I2C_STATE_READ :
                TimerStop(&tmr_i2c) ;
                //LPI2C_DRV_MasterInit(INST_LPI2C1, &lpi2c1_MasterConfig0, &lpi2c1MasterState);

                i2c_slave_address = req_i2c_slave_address ;
                i2c_master_state = req_i2c_state ;
                req_i2c_state = I2C_STATE_IDLE ;
                i2c_result = I2C_RUNNING ;
                break ;
            case I2C_STATE_WRITE_THEN_READ :
                TimerStop(&tmr_i2c) ;
                //LPI2C_DRV_MasterInit(INST_LPI2C1, &lpi2c1_MasterConfig0, &lpi2c1MasterState);

                i2c_slave_address = req_i2c_slave_address ;
                i2c_master_state = req_i2c_state ;
                req_i2c_state = I2C_STATE_IDLE ;
                i2c_result = I2C_RUNNING ;
                break ;
            default :
                TimerStop(&tmr_i2c) ;
                req_i2c_state = I2C_STATE_WAIT ;
                i2c_result = I2C_IDLE ;
                break ;
        }   // switch (req_i2c_state)
    }   // if (req_i2c_state != I2C_STATE_IDLE)
}   // void Handle_I2C_STATE_IDLE (void)

void Handle_I2C_STATE_WAIT (void) {
    i2c_slave_address = (uint16_t)NULL ;
    req_i2c_slave_address = (uint16_t)NULL ;

    i2c_tar_buf = (uint8_t *)NULL ;
    i2c_tar_buf_len = 0 ;
    i2c_buf_len = 0 ;
    req_i2c_state = I2C_STATE_IDLE ;

    i2c_master_state = I2C_STATE_IDLE ;
}   // void Handle_I2C_STATE_WAIT (void)

void Handle_I2C_STATE_WRITE (void) {
    i2c_master_state = I2C_STATE_WRITE_START ;
    LPI2C_DRV_MasterSetSlaveAddr(INST_LPI2C1 , i2c_slave_address , FALSE) ;
}   // void Handle_I2C_STATE_WRITE (void)

void Handle_I2C_STATE_WRITE_START (void) {
    TimerSet(&tmr_i2c, MAX_I2C_ACCESS_TIME) ;
    i2c_master_state = I2C_STATE_WRITE_FINISHED ;
    //LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C1, i2c_buf, i2c_buf_len, true, OSIF_WAIT_FOREVER);
    LPI2C_DRV_MasterSendData(INST_LPI2C1, i2c_buf, i2c_buf_len, true);
}   // void Handle_I2C_STATE_WRITE_START (void)

void Handle_I2C_STATE_WRITE_FINISHED (void) {
    status_t    read_state ;
    uint32_t    bytesRemaining ;

    read_state = LPI2C_DRV_MasterGetTransferStatus(INST_LPI2C1, &bytesRemaining) ;
    if (read_state == STATUS_SUCCESS) {
        i2c_master_state = I2C_STATE_WAIT ;
        i2c_result = I2C_SUCCESS ;
        TimerStop(&tmr_i2c) ;
    }   // if (read_state == STATUS_SUCCESS)
    else if (read_state == STATUS_ERROR || read_state == STATUS_I2C_RECEIVED_NACK) {
        i2c_result = I2C_FAIL ;
        i2c_master_state = I2C_STATE_WAIT ;
        LPI2C_DRV_MasterInit(INST_LPI2C1, &lpi2c1_MasterConfig0, &lpi2c1MasterState);
        //LPI2C_DRV_MasterAbortTransferData(INST_LPI2C1);
        TimerStop(&tmr_i2c) ;
    }
    else if (TimerHasExpired(&tmr_i2c) == TRUE) {
        LPI2C_DRV_MasterAbortTransferData(INST_LPI2C1) ;
        i2c_result = I2C_FAIL ;
        i2c_master_state = I2C_STATE_WAIT ;
        TimerStop(&tmr_i2c) ;
    }   // if (TimerHasExpired(&tmr_i2c) == TRUE)
}   // void Handle_I2C_STATE_WRITE_FINISHED (void)

void Handle_I2C_STATE_READ (void) {
    i2c_master_state = I2C_STATE_READ_START ;
    LPI2C_DRV_MasterSetSlaveAddr(INST_LPI2C1 , i2c_slave_address , FALSE) ;
}   // void Handle_I2C_STATE_READ (void)

void Handle_I2C_STATE_READ_START (void) {
    TimerSet(&tmr_i2c, MAX_I2C_ACCESS_TIME) ;
    //LPI2C_DRV_MasterReceiveDataBlocking(INST_LPI2C1, i2c_tar_buf, i2c_tar_buf_len, true, OSIF_WAIT_FOREVER);
    LPI2C_DRV_MasterReceiveData(INST_LPI2C1, i2c_tar_buf, i2c_tar_buf_len, true);
    i2c_master_state = I2C_STATE_READ_FINISHED ;
}   // void Handle_I2C_STATE_READ_START (void)

void Handle_I2C_STATE_READ_FINISHED (void) {
    status_t    read_state ;
    uint32_t    bytesRemaining ;

    read_state = LPI2C_DRV_MasterGetTransferStatus(INST_LPI2C1, &bytesRemaining) ;
    if (read_state == STATUS_SUCCESS) {
        i2c_master_state = I2C_STATE_WAIT ;
        i2c_result = I2C_SUCCESS ;
        TimerStop(&tmr_i2c) ;
    }   // if (read_state == STATUS_SUCCESS)
    else if (read_state == STATUS_ERROR || read_state == STATUS_I2C_RECEIVED_NACK) {
        i2c_result = I2C_FAIL ;
        i2c_master_state = I2C_STATE_WAIT ;
        LPI2C_DRV_MasterInit(INST_LPI2C1, &lpi2c1_MasterConfig0, &lpi2c1MasterState);
        //LPI2C_DRV_MasterAbortTransferData(INST_LPI2C1);
        TimerStop(&tmr_i2c) ;
    }
    else if (TimerHasExpired(&tmr_i2c) == TRUE) {
        LPI2C_DRV_MasterAbortTransferData(INST_LPI2C1) ;
        i2c_result = I2C_FAIL ;
        i2c_master_state = I2C_STATE_WAIT ;
        TimerStop(&tmr_i2c) ;
    }   // if (TimerHasExpired(&tmr_i2c) == TRUE)
}   // void Handle_I2C_STATE_READ_FINISHED (void)

void Handle_I2C_STATE_WRITE_THEN_READ (void) {
    i2c_master_state = I2C_STATE_WRITE_THEN_READ_START_WRITE ;
    LPI2C_DRV_MasterSetSlaveAddr(INST_LPI2C1 , i2c_slave_address , FALSE) ;
}   //void Handle_I2C_STATE_WRITE_THEN_READ (void)

void Handle_I2C_STATE_WRITE_THEN_READ_START_WRITE (void) {
    TimerSet(&tmr_i2c, MAX_I2C_ACCESS_TIME) ;
    i2c_master_state = I2C_STATE_WRITE_THEN_READ_FINISHED_WRITE ;
    //LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C1, i2c_buf, i2c_buf_len, true, OSIF_WAIT_FOREVER);
    LPI2C_DRV_MasterSendData(INST_LPI2C1, i2c_buf, i2c_buf_len, true);
}   // void Handle_I2C_STATE_WRITE_THEN_READ_START_WRITE (void)

void Handle_I2C_STATE_WRITE_THEN_READ_FINISHED_WRITE (void) {
    status_t    read_state ;
    uint32_t    bytesRemaining ;

    read_state = LPI2C_DRV_MasterGetTransferStatus(INST_LPI2C1, &bytesRemaining) ;
    if (read_state == STATUS_SUCCESS) {
        i2c_master_state = I2C_STATE_WRITE_THEN_READ_START_READ ;
        TimerStop(&tmr_i2c) ;
    }   // if (read_state == STATUS_SUCCESS)
    else if (read_state == STATUS_ERROR || read_state == STATUS_I2C_RECEIVED_NACK) {
        i2c_result = I2C_FAIL ;
        i2c_master_state = I2C_STATE_WAIT ;
        LPI2C_DRV_MasterAbortTransferData(INST_LPI2C1);
        LPI2C_DRV_MasterInit(INST_LPI2C1, &lpi2c1_MasterConfig0, &lpi2c1MasterState);
        TimerStop(&tmr_i2c) ;
    }
    else if (TimerHasExpired(&tmr_i2c) == TRUE) {
        LPI2C_DRV_MasterAbortTransferData(INST_LPI2C1) ;
        i2c_result = I2C_FAIL ;
        i2c_master_state = I2C_STATE_WAIT ;
        TimerStop(&tmr_i2c) ;
    }   // if (TimerHasExpired(&tmr_i2c) == TRUE)
}   // void Handle_I2C_STATE_WRITE_THEN_READ_FINISHED_WRITE (void)

void Handle_I2C_STATE_WRITE_THEN_READ_START_READ (void) {
    TimerSet(&tmr_i2c, MAX_I2C_ACCESS_TIME) ;
    //LPI2C_DRV_MasterReceiveDataBlocking(INST_LPI2C1, i2c_tar_buf, i2c_tar_buf_len, true, OSIF_WAIT_FOREVER);
    LPI2C_DRV_MasterReceiveData(INST_LPI2C1, i2c_tar_buf, i2c_tar_buf_len, true);
    i2c_master_state = I2C_STATE_WRITE_THEN_READ_FINISHED_READ ;
}   // void Handle_I2C_STATE_WRITE_THEN_READ_START_READ (void)

void Handle_I2C_STATE_WRITE_THEN_READ_FINISHED_READ (void) {
    status_t    read_state ;
    uint32_t    bytesRemaining ;

    read_state = LPI2C_DRV_MasterGetTransferStatus(INST_LPI2C1, &bytesRemaining) ;
    if (read_state == STATUS_SUCCESS) {
        i2c_master_state = I2C_STATE_WAIT ;
        i2c_result = I2C_SUCCESS ;
        TimerStop(&tmr_i2c) ;
    }   // if (read_state == STATUS_SUCCESS)
    else if (read_state == STATUS_ERROR || read_state == STATUS_I2C_RECEIVED_NACK) {
        i2c_result = I2C_FAIL ;
        i2c_master_state = I2C_STATE_WAIT ;
        LPI2C_DRV_MasterAbortTransferData(INST_LPI2C1);
        LPI2C_DRV_MasterInit(INST_LPI2C1, &lpi2c1_MasterConfig0, &lpi2c1MasterState);
        TimerStop(&tmr_i2c) ;
    }
    else if (TimerHasExpired(&tmr_i2c) == TRUE) {
        LPI2C_DRV_MasterAbortTransferData(INST_LPI2C1) ;
        i2c_result = I2C_FAIL ;
        i2c_master_state = I2C_STATE_WAIT ;
        TimerStop(&tmr_i2c) ;
    }   // if (TimerHasExpired(&tmr_i2c) == TRUE)
}   // void Handle_I2C_STATE_WRITE_THEN_READ_FINISHED_READ (void)
