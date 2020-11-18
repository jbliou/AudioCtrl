/******************************************************************************
*
* ALLGO Inc.
* (c) Copyright 2020 ALLGO Inc.
* ALL RIGHTS RESERVED.
***************************************************************************//*!
*
* @file		i2c_master.h
*
* @author	ALLGO
*
* @brief	
*
*******************************************************************************
* History:
* 
******************************************************************************/

#ifndef BSW_MCAL_I2C_I2C_MASTER_H_
	#define BSW_MCAL_I2C_I2C_MASTER_H_

    typedef enum {
        I2C_MASTER_STATE_INIT
        , I2C_MASTER_STATE_WRITE
        , I2C_MASTER_STATE_READ
        , I2C_MASTER_STATE_WriteRead_SUBSTATE_WRITE
        , I2C_MASTER_STATE_WRITEREAD_SUBSTATE_READ
        , I2C_MASTER_STATE_IDLE
        , I2C_MASTER_STATE_PASS
        , I2C_MASTER_STATE_FAIL
        , I2C_MASTER_STATE_MAX
    } I2C_MASTER_STATE ;
    #define MAX_I2C_WRITING_TIME        (200U)
    #define MAX_I2C_READING_TIME        (200U)
#endif	/* BSW_MCAL_I2C_I2C_MASTER_H_ */

#if defined(_I2C_MASTER_INS_)
    /* Allocate memory for the LPI2C driver state structure */
    lpi2c_master_state_t        lpi2c1MasterState ;

    volatile I2C_MASTER_STATE   i2c_master_state = I2C_MASTER_STATE_INIT ;
    volatile pvFUNCs            i2c_master_callback ;
    volatile uint8_t *          i2c_master_rxBuff ;
    volatile uint32_t           i2c_master_rxSize ;
    uint32_t                    tmr_i2c_master ;

    void i2c_master_process_init(void) ;
    void i2c_master_opmode (void) ;
    void i2c_master_process_nop(void) ;
    void i2c_master_substate_read(void) ;
    void i2c_master_process_idle(void) ;
    void i2c_master_process_pass(void) ;
    void i2c_master_process_fail(void) ;

    const pvFUNCv i2c_master_process [] = {
        i2c_master_process_init             /* I2C_MASTER_STATE_INIT                        */
        , i2c_master_opmode                 /* I2C_MASTER_STATE_WRITE                       */
        , i2c_master_opmode                 /* I2C_MASTER_STATE_READ                        */
        , i2c_master_opmode                 /* I2C_MASTER_STATE_WriteRead_SUBSTATE_WRITE    */
        , i2c_master_substate_read          /* I2C_MASTER_STATE_WRITEREAD_SUBSTATE_READ     */
        , i2c_master_process_idle           /* I2C_MASTER_STATE_IDLE                        */
        , i2c_master_process_pass           /* I2C_MASTER_STATE_PASS                        */
        , i2c_master_process_fail           /* I2C_MASTER_STATE_FAIL                        */
        , i2c_master_process_nop            /* I2C_MASTER_STATE_MAX                         */
    };
#else
    extern status_t Handle_i2c_master (void) ;
    extern status_t i2c_masterReqTransceiving (uint16_t slave_addr, uint8_t * w_buf, uint32_t w_len, uint8_t * r_buf, uint32_t r_len, pvFUNCs i2c_master_cb) ;
#endif	// #if defined(_I2C_MASTER_INS_)
/******************************************************************************
* end of file (i2c_master.h)
******************************************************************************/
