/*
 * tuner.c
 *
 *  Created on: 2020撟�1���10�
 *      Author: ALLGO
 */

#include "board.h"

#include "TEF668x.h"
#include "I2CMaster.h"

#define __TUNER_INS__
#include "tuner.h"

void Handle_TUNER(void) {
    uint8_t     tmp_state ;

    if (GetI2CState() != STATUS_BUSY) {
        switch (state_tuner) {
            case TUNER_STATE_INIT :
                //TimerStop(&tmr_tuner) ;
                state_tuner = TUNER_STATE_GET_POWERON_STATE ;
                i2c_slave_tuner_address = I2C_ADDRESS_TEF668x ;
                break ;
            case TUNER_STATE_GET_POWERON_STATE :
                TEF668x_Buf[0] = TEF668x_Module_APPL ;
                TEF668x_Buf[1] = TEF668x_cmd_Get_Operation_Status ;
                TEF668x_Buf[2] = 1 ;
                I2C_WriteReadData(i2c_slave_tuner_address, TEF668x_Buf, 3U, TEF668x_Buf+3, 2U) ;
                state_tuner = TUNER_STATE_CHECK_POWERON_STATE ;
                TimerSet(&tmr_tuner, 50) ;
                break ;
            case TUNER_STATE_CHECK_POWERON_STATE :
                tmp_state = GetI2CExecuteState() ;
                if (tmp_state == I2C_SUCCESS) {
                    TimerStop(&tmr_tuner) ;
                    state_tuner = TUNER_STATE_BOOT ;
                }
                if (TimerHasExpired(&tmr_tuner) == TRUE) {
                    state_tuner = TUNER_STATE_GET_POWERON_STATE ;
                }
                break ;
            case TUNER_STATE_BOOT :
                break ;
        }   // switch (state_tuner)
    }
}
