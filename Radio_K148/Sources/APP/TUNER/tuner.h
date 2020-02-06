/*
 * tuner.h
 *
 *  Created on: 2020年1月10日
 *      Author: ALLGO
 */

#ifndef BSW_OBU_DEVICES_TUNER_TUNER_H_
    #define BSW_OBU_DEVICES_TUNER_TUNER_H_

    enum {
        TUNER_STATE_INIT            = 0 ,
        TUNER_STATE_GET_POWERON_STATE   ,
        TUNER_STATE_CHECK_POWERON_STATE ,
        TUNER_STATE_BOOT                ,
    };
    #if defined (__TUNER_INS__)
        uint32_t            tmr_tuner ;
        uint8_t             state_tuner = TUNER_STATE_INIT ;
        uint8_t             i2c_slave_tuner_address ;

        #undef  __TUNER_INS__
    #else
        extern void Handle_TUNER(void) ;
    #endif  // #if defined (__TUNER_INS__)
#endif /* BSW_OBU_DEVICES_TUNER_TUNER_H_ */
