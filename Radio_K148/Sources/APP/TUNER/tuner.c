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
#if (TEF668x_API_DEBUG == 0)
    Handle_TEF668x() ;
    switch (state_tuner) {
        case TUNER_STATE_INIT :
            TimerStop(&tmr_tuner) ;
            state_tuner = TUNER_STATE_STANDBY ;
            break ;
        case TUNER_STATE_STANDBY :
            if (GetState_TEF668x() == STATUS_SUCCESS) {
                state_tuner = TUNER_STATE_IDLE ;
            }   // if (GetState_TEF668x() == STATUS_SUCCESS)
            break ;
        case TUNER_STATE_IDLE :
            break ;
    }   // switch (state_tuner)
#else
    //TEF668x_api(TEF668X_API_FM_Tune_To, (PFUNCvS)NULL, 2, Tuning_Actions_Mode_Preset, 10790) ;
    //TEF668x_api(TEF668X_API_FM_Set_MphSuppression, (PFUNCvS)NULL, 1, 0) ;
    //TEF668x_api(TEF668X_API_FM_Set_NoiseBlanker, (PFUNCvS)NULL, 4, 1, 1000, 1000, 1250) ;
    //TEF668x_api(TEF668X_API_FM_Set_NoiseBlanker, (PFUNCvS)NULL, 3, 1, 1000, 1000) ;
    //TEF668x_api(TEF668X_API_APPL_Set_ReferenceClock, (PFUNCvS)NULL, 2, 12000000, 1) ;
#endif  // #if (TEF668x_API_DEBUG == 0)
}
