/******************************************************************************
*
* ALLGO Inc.
* (c) Copyright 2020 ALLGO Inc.
* ALL RIGHTS RESERVED.
***************************************************************************//*!
*
* @file		i2c_master.c
*
* @author	ALLGO
*
* @brief	
*
*******************************************************************************
* History:
* 
******************************************************************************/
#include "platform.h"
#define _I2C_MASTER_INS_
#include "i2c_master.h"

status_t Handle_i2c_master (void) {
    if (i2c_master_state < I2C_MASTER_STATE_MAX) {
        (i2c_master_process[i2c_master_state])() ;
        NOP() ;
    }   // if (i2c_master_state < I2C_MASTER_STATE_MAX)
#if (0)
    if (i2c_master_state == I2C_MASTER_STATE_IDLE) {
        return (STATUS_SUCCESS) ;
    }   // if (i2c_master_state == I2C_MASTER_STATE_IDLE)

    return (STATUS_BUSY) ;
#else
    return (STATUS_SUCCESS) ;
#endif  // #if (0)
}   // status_t Handle_i2c_master (void)

void i2c_masterCallback(i2c_master_event_t event, void *userData) {
    /* Unused parameters */
    (void)userData;

    /* Check the event type */
    if (event == I2C_MASTER_EVENT_END_TRANSFER) {
        NOP() ;
        if (lpi2c1MasterState.status == STATUS_SUCCESS) {
            if (i2c_master_state == I2C_MASTER_STATE_WriteRead_SUBSTATE_WRITE) {
                i2c_master_state = I2C_MASTER_STATE_WRITEREAD_SUBSTATE_READ ;
            }   // if (i2c_master_state == I2C_MASTER_STATE_WriteRead_SUBSTATE_WRITE)
            else {
                i2c_master_state = I2C_MASTER_STATE_PASS ;
            }
        }   // if (lpi2c1MasterState.status == STATUS_SUCCESS)
        else {
            i2c_master_state = I2C_MASTER_STATE_FAIL ;
        }
    }   // if (event == I2C_MASTER_EVENT_END_TRANSFER)
}   // void i2c_masterCallback(void *driverState, uart_event_t event, void *userData)

void i2c_master_process_init(void) {
    /* Initialize LPI2C Master configuration
     *  - Slave address ??
     *  - Fast operating mode, 400 KHz SCL frequency
     *  -   See LPI2C components for configuration details
     */
    NOP() ;
    memset ((uint8_t *)&lpi2c1MasterState, 0, sizeof(lpi2c_master_state_t)/sizeof(uint8_t)) ;

    LPI2C_DRV_MasterInit(INST_LPI2C1, &lpi2c1_MasterConfig0, &lpi2c1MasterState);
    lpi2c1MasterState.status = STATUS_SUCCESS ;
    lpi2c1MasterState.i2cIdle = true ;
    i2c_master_callback = (pvFUNCs)NULL ;
    i2c_master_rxBuff = (uint8_t *)NULL ;
    i2c_master_rxSize = (uint32_t)NULL ;
    TimerStop(&tmr_i2c_master) ;
    i2c_master_state = I2C_MASTER_STATE_IDLE ;
}   // void i2c_master_process_init(void)

void i2c_master_opmode(void) {
    if (TimerHasExpired(&tmr_i2c_master) == true) {
        NOP() ;
        i2c_master_state = I2C_MASTER_STATE_FAIL ;
    }   // if (TimerHasExpired(&tmr_i2c_master) == true)
}   // void i2c_master_opmode(void)

void i2c_master_substate_read(void) {
    if (i2c_master_rxBuff == (uint8_t *)NULL || i2c_master_rxSize == (uint32_t)NULL) {
        i2c_master_state = I2C_MASTER_STATE_FAIL ;
    }   // if (i2c_master_rxBuff == (uint8_t *)NULL || i2c_master_rxSize = NULL)
    else {
        TimerSet(&tmr_i2c_master, MAX_I2C_READING_TIME) ;
        /* Request data from the bus slave */
        LPI2C_DRV_MasterReceiveData(INST_LPI2C1
                                   , (uint8_t *)i2c_master_rxBuff
                                   , (uint32_t)i2c_master_rxSize
                                   , true
                                   );
        i2c_master_state = I2C_MASTER_STATE_READ ;
    }
}   // void i2c_master_substate_read(void)

void i2c_master_process_nop(void) {
}   // void i2c_master_process_nop(void)

void i2c_master_process_idle(void) {
}   // void i2c_master_process_idle(void)

void i2c_master_process_pass(void) {
    TimerStop(&tmr_i2c_master) ;
    if (i2c_master_callback != (pvFUNCs)NULL) {
        i2c_master_callback(STATUS_SUCCESS) ;
    }   // if (i2c_master_callback != (pvFUNCs)NULL)

    i2c_master_callback = (pvFUNCs)NULL ;
    //i2c_master_state = I2C_MASTER_STATE_INIT ;
    i2c_master_state = I2C_MASTER_STATE_IDLE ;
}   // void i2c_master_process_pass(void)

void i2c_master_process_fail(void) {
    TimerStop(&tmr_i2c_master) ;
    if (i2c_master_callback != (pvFUNCs)NULL) {
        i2c_master_callback(STATUS_ERROR) ;
    }   // if (i2c_master_callback != (pvFUNCs)NULL)

    i2c_master_callback = (pvFUNCs)NULL ;

    LPI2C_DRV_MasterDeinit(INST_LPI2C1) ;

    i2c_master_state = I2C_MASTER_STATE_INIT ;
}   // void i2c_master_process_fail(void)

status_t i2c_masterReqTransceiving (uint16_t slave_addr, uint8_t * w_buf, uint32_t w_len, uint8_t * r_buf, uint32_t r_len, pvFUNCs i2c_master_cb) {
    if (i2c_master_state == I2C_MASTER_STATE_IDLE) {
        status_t    ret_state ;

        if (slave_addr != lpi2c1MasterState.slaveAddress) {
            LPI2C_DRV_MasterSetSlaveAddr (INST_LPI2C1
                                         , (const uint16_t)slave_addr
                                         , (const bool)false
                                         ) ;
        }   // if (slave_addr != lpi2c1MasterState.slaveAddress)

        TimerSet(&tmr_i2c_master, MAX_I2C_WRITING_TIME) ;

        if (i2c_master_cb != (pvFUNCs)NULL) {
            i2c_master_callback = i2c_master_cb ;
        }   // if (i2c_master_cb != (pvFUNCs)NULL)

        if ((w_buf != (uint8_t *)NULL && w_len != (uint32_t)NULL)
          &&(r_buf != (uint8_t *)NULL && r_len != (uint32_t)NULL)) {
            i2c_master_rxBuff = r_buf ;
            i2c_master_rxSize = r_len ;
            i2c_master_state = I2C_MASTER_STATE_WriteRead_SUBSTATE_WRITE ;
            ret_state = LPI2C_DRV_MasterSendData ( INST_LPI2C1
                                                 , (const uint8_t *)w_buf
                                                 , w_len
                                                 , false
                                                 ) ;
        }
        else if (w_buf != (uint8_t *)NULL && w_len != (uint32_t)NULL) {
            i2c_master_state = I2C_MASTER_STATE_WRITE ;
            ret_state = LPI2C_DRV_MasterSendData ( INST_LPI2C1
                                                 , (const uint8_t *)w_buf
                                                 , w_len
                                                 , true
                                                 ) ;
        }   // if (w_buf != (uint8_t *)NULL && w_len != (uint32_t)NULL)
        else if (r_buf != (uint8_t *)NULL && r_len != (uint32_t)NULL) {
            i2c_master_state = I2C_MASTER_STATE_READ ;
            ret_state = LPI2C_DRV_MasterReceiveData ( INST_LPI2C1
                                                    , r_buf
                                                    , r_len
                                                    , true
                                                    ) ;

        }   // if (r_buf != (uint8_t *)NULL && r_len != (uint32_t)NULL)
        else {
            ret_state = STATUS_ERROR ;
        }

        if (ret_state != STATUS_SUCCESS) {
            TimerStop(&tmr_i2c_master) ;
            i2c_master_state = I2C_MASTER_STATE_IDLE ;
            i2c_master_callback = (pvFUNCs)NULL ;
        }   // if (ret_state != STATUS_SUCCESS) {

        return (ret_state) ;
    }   // if (i2c_master_state == I2C_MASTER_STATE_IDLE)

    return (STATUS_BUSY) ;
}   // status_t i2c_masterReqTransceiving (uint16_t slave_addr, uint8_t * w_buf, uint32_t w_len, uint8_t * r_buf, uint32_t r_len, pvFUNCs i2c_master_cb)

#undef	_I2C_MASTER_INS_
/******************************************************************************
* end of file (i2c_master.c)
******************************************************************************/
