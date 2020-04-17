/*
 * I2CMaster.h
 *
 *  Created on: 2020�~1��8��
 *      Author: ALLGO
 */

#ifndef BSW_MCAL_I2C_I2CMASTER_H_
    #define BSW_MCAL_I2C_I2CMASTER_H_

    #define MAX_I2C_BUF             (256U)

    /*
     *  256*8/400K = 5.12 mSec., so, assume the max duration is 10 mSec.
     */
    #define MAX_I2C_ACCESS_TIME     (10U)

    enum {
        I2C_IDLE            ,
        I2C_RUNNING         ,
        I2C_FAIL            ,
        I2C_SUCCESS
    };

    typedef enum {
        I2C_STATE_INIT                          = 0
        , I2C_STATE_IDLE
        , I2C_STATE_WAIT
        , I2C_STATE_WRITE
        , I2C_STATE_WRITE_START
        , I2C_STATE_WRITE_FINISHED
        , I2C_STATE_READ
        , I2C_STATE_READ_START
        , I2C_STATE_READ_FINISHED
        , I2C_STATE_WRITE_THEN_READ
        , I2C_STATE_WRITE_THEN_READ_START_WRITE
        , I2C_STATE_WRITE_THEN_READ_FINISHED_WRITE
        , I2C_STATE_WRITE_THEN_READ_START_READ
        , I2C_STATE_WRITE_THEN_READ_FINISHED_READ
        , I2C_STATE_MAX
        , I2C_STATE_BUSY
    } I2C_MASTER_STATE ;

    typedef void (* I2C_State_Process)(void) ;

    #if defined (__I2CMASTER_INS__)
        I2C_MASTER_STATE    i2c_master_state = I2C_STATE_INIT ;
        I2C_MASTER_STATE    req_i2c_state = I2C_STATE_IDLE ;
        uint16_t            i2c_slave_address = NULL ;
        uint16_t            req_i2c_slave_address = NULL ;
        uint32_t            tmr_i2c ;
        uint8_t             i2c_buf [MAX_I2C_BUF] ;
        uint16_t            i2c_buf_len ;
        uint8_t *           i2c_tar_buf ;
        uint16_t            i2c_tar_buf_len ;
        uint8_t             i2c_result ;
        PFUNCvS             i2c_ReportCallBack = (PFUNCvS)NULL ;

        void Handle_I2C_STATE_INIT (void) ;
        void Handle_I2C_STATE_IDLE (void) ;
        void Handle_I2C_STATE_WAIT (void) ;
        void Handle_I2C_STATE_WRITE (void) ;
        void Handle_I2C_STATE_WRITE_START (void) ;
        void Handle_I2C_STATE_WRITE_FINISHED (void) ;
        void Handle_I2C_STATE_READ (void) ;
        void Handle_I2C_STATE_READ_START (void) ;
        void Handle_I2C_STATE_READ_FINISHED (void) ;
        void Handle_I2C_STATE_WRITE_THEN_READ (void) ;
        void Handle_I2C_STATE_WRITE_THEN_READ_START_WRITE (void) ;
        void Handle_I2C_STATE_WRITE_THEN_READ_FINISHED_WRITE (void) ;
        void Handle_I2C_STATE_WRITE_THEN_READ_START_READ (void) ;
        void Handle_I2C_STATE_WRITE_THEN_READ_FINISHED_READ (void) ;

        const I2C_State_Process pHandleI2C[] = {
            Handle_I2C_STATE_INIT ,                             /* I2C_STATE_INIT                            */
            Handle_I2C_STATE_IDLE ,                             /* I2C_STATE_IDLE                            */
            Handle_I2C_STATE_WAIT ,                             /* I2C_STATE_WAIT                            */
            Handle_I2C_STATE_WRITE ,                            /* I2C_STATE_WRITE                           */
            Handle_I2C_STATE_WRITE_START ,                      /* I2C_STATE_WRITE_START                     */
            Handle_I2C_STATE_WRITE_FINISHED ,                   /* I2C_STATE_WRITE_FINISHED                  */
            Handle_I2C_STATE_READ ,                             /* I2C_STATE_READ                            */
            Handle_I2C_STATE_READ_START ,                       /* I2C_STATE_READ_START                      */
            Handle_I2C_STATE_READ_FINISHED ,                    /* I2C_STATE_READ_FINISHED                   */
            Handle_I2C_STATE_WRITE_THEN_READ ,                  /* I2C_STATE_WRITE_THEN_READ                 */
            Handle_I2C_STATE_WRITE_THEN_READ_START_WRITE ,      /* I2C_STATE_WRITE_THEN_READ_START_WRITE     */
            Handle_I2C_STATE_WRITE_THEN_READ_FINISHED_WRITE ,   /* I2C_STATE_WRITE_THEN_READ_FINISHED_WRITE  */
            Handle_I2C_STATE_WRITE_THEN_READ_START_READ ,       /* I2C_STATE_WRITE_THEN_READ_START_READ      */
            Handle_I2C_STATE_WRITE_THEN_READ_FINISHED_READ ,    /* I2C_STATE_WRITE_THEN_READ_FINISHED_READ   */
        };

        #undef __I2CMASTER_INS__
    #else
        extern void Handle_I2C(void) ;
        extern status_t GetI2CState(void) ;
        extern uint8_t GetI2CExecuteState(void) ;
        extern void I2C_ReadData (uint8_t req_slave_address, uint8_t * buf, uint16_t len) ;
        extern int16_t I2C_WriteData (uint8_t req_slave_address, uint8_t * buf, uint16_t len) ;
        extern int16_t I2C_WriteReadData (uint8_t req_slave_address, uint8_t * wbuf, uint16_t wlen, uint8_t * rbuf, uint16_t rlen) ;
        extern int16_t I2C_WriteDataCB (uint8_t req_slave_address, uint8_t * buf, uint16_t len, PFUNCvS cb) ;
        extern void I2C_ReadDataCB (uint8_t req_slave_address, uint8_t * buf, uint16_t len, PFUNCvS cb) ;
        extern int16_t I2C_WriteReadDataCB (uint8_t req_slave_address, uint8_t * wbuf, uint16_t wlen, uint8_t * rbuf, uint16_t rlen, PFUNCvS cb) ;
    #endif  // #if defined (__I2CMASTER_INS__)
#endif /* BSW_MCAL_I2C_I2CMASTER_H_ */
