/*
 * TEF668x.c
 *
 *  Created on: 2019撟�12���2�
 *      Author: ALLGO
 */


#include <stdio.h>
#include <string.h>

#include "board.h"
#include "I2CMaster.h"

#define _RADIO_TEF668X_INS_
#include "TEF668x.h"

status_t GetState_TEF668x (void) {
    if (state_TEF668x == TEF668x_STATE_STANDBY) {
        return (STATUS_SUCCESS) ;
    }   //if (state_TEF668x == TEF668x_STATE_STANDBY)
    return (STATUS_BUSY) ;
}   // status_t GetState_TEF668x (void)

status_t SetState_TEF668x (TEF668x_STATE new_state) {
    if (state_TEF668x == TEF668x_STATE_STANDBY) {
        state_TEF668x = new_state ;

        return (STATUS_SUCCESS) ;
    }   //if (state_TEF668x == TEF668x_STATE_STANDBY)
    return (STATUS_BUSY) ;
}   // status_t SetState_TEF668x (TEF668x_STATE new_state)

status_t TEF668x_api (uint8_t cmd, PFUNCvS cb, uint8_t argc, ...) {
    status_t        ret_state = STATUS_BUSY ;

#if (TEF668x_API_DEBUG == 0)
    if (GetState_TEF668x() == STATUS_SUCCESS && GetI2CExecuteState() == (status_t)I2C_SUCCESS) {
#endif  // #if (TEF668x_API_DEBUG == 0)
        va_list va;
        XDATA16 tmp ;

        va_start(va, argc);
        memset(TEF668x_Buf, '\0', MAX_TEF668x_BUF) ;
        ret_state = STATUS_UNSUPPORTED ;
        tuner_api_callback = cb ;

        switch (cmd) {
            case TEF668X_API_FM_Tune_To :
            case TEF668X_API_AM_Set_Tune_To :
                {
                    PSetTune_To     pTune_To ;

                    pTune_To = (PSetTune_To)TEF668x_Buf ;

                    if (cmd == TEF668X_API_FM_Tune_To) {
                        pTune_To->module= TEF668x_Module_Radio_FM ;
                    }
                    else {
                        pTune_To->module= TEF668x_Module_Radio_AM ;
                    }
                    pTune_To->cmd   = TEF668x_cmd_Tune_To ;
                    pTune_To->index = 1 ;

                    tmp.udata16 = va_arg(va, int) ;
                    pTune_To->tuning_actions_hi         = tmp.hibyte ;
                    pTune_To->tuning_actions_lo         = tmp.lobyte ;
                    if (tmp.udata16 == Tuning_Actions_Mode_End) {
#if (TEF668x_API_DEBUG == 0)
                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                        , TEF668x_Buf
                                        , 5
                                        , Handle_TEF668x_Service
                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                        ret_state = STATUS_SUCCESS ;
                    }   // if (tmp == Tuning_Actions_Mode_End)
                    else  {
                        if (cmd == TEF668X_API_FM_Tune_To) {
                            if (tmp.udata16 > Tuning_Actions_Mode_End
                             || tmp.udata16 == Tuning_Actions_Mode_No_Action
                            ) {
                                break ;
                            }
                        }
                        else if (tmp.udata16 > Tuning_Actions_Mode_Search
                              || tmp.udata16 == Tuning_Actions_Mode_No_Action
                             ) {
                            break ;
                        }
                        tmp.udata16 = va_arg(va, int) ;
                        pTune_To->tuning_frequency_hi   = tmp.hibyte ;
                        pTune_To->tuning_frequency_lo   = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                        , TEF668x_Buf
                                        , 7
                                        , Handle_TEF668x_Service
                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                        ret_state = STATUS_SUCCESS ;
                    }   // if (tmp.udata16 < Tuning_Actions_Mode_End && tmp.udata16 > Tuning_Actions_Mode_No_Action)
                }
                break ;
            case TEF668X_API_FM_Set_Tune_Options :
                {
                    PSet_Tune_Options   pSet_Tune_Options ;

                    pSet_Tune_Options = (PSet_Tune_Options)TEF668x_Buf ;

                    pSet_Tune_Options->module= TEF668x_Module_Radio_FM ;
                    pSet_Tune_Options->cmd   = TEF668x_cmd_Set_Tune_Options ;
                    pSet_Tune_Options->index = 1 ;

                    tmp.udata16 = va_arg(va, int) ;
                    if (tmp.udata16 == AFU_BW_MODE_FIXED
                     || tmp.udata16 == AFU_BW_MODE_AUTOMATIC_BANDWIDTH
                    ) {
                        // IF bandwidth control mode during AF_Update
                        pSet_Tune_Options->afu_bw_mode_hi = tmp.hibyte ;
                        pSet_Tune_Options->afu_bw_mode_lo = tmp.lobyte ;

                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= MIN_AFU_FIXED_IF_BANDWIDTH
                         && tmp.udata16 <= MAX_AFU_FIXED_IF_BANDWIDTH
                        ) {
                            // fixed IF bandwidth during AF_Update
                            pSet_Tune_Options->afu_bandwidth_hi = tmp.hibyte ;
                            pSet_Tune_Options->afu_bandwidth_lo = tmp.lobyte ;

                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= MIN_AFU_MUTE_TIME
                             && tmp.udata16 <= MAX_AFU_MUTE_TIME
                            ) {
                                // AF_update inaudible mute slope time
                                pSet_Tune_Options->afu_mute_time_hi = tmp.hibyte ;
                                pSet_Tune_Options->afu_mute_time_lo = tmp.lobyte ;

                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= MIN_AFU_SAMPLE_TIME
                                 && tmp.udata16 <= MAX_AFU_SAMPLE_TIME
                                ) {
                                    pSet_Tune_Options->afu_sample_time_hi = tmp.hibyte ;
                                    pSet_Tune_Options->afu_sample_time_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                    SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                    I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                    , TEF668x_Buf
                                                    , 11
                                                    , Handle_TEF668x_Service
                                                    ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                    ret_state = STATUS_SUCCESS ;
                                }   // if (tmp.udata16 >= MIN_AFU_SAMPLE_TIME && tmp.udata16 <= MAX_AFU_SAMPLE_TIME)
                            }   // if (tmp.udata16 >= MIN_AFU_MUTE_TIME && tmp.udata16 <= MAX_AFU_MUTE_TIME)
                        }   // if (tmp.udata16 >= MIN_AFU_FIXED_IF_BANDWIDTH && tmp.udata16 <= MAX_AFU_FIXED_IF_BANDWIDTH)
                    }   // if (tmp.udata16 == AFU_BW_MODE_FIXED || tmp.udata16 == AFU_BW_MODE_AUTOMATIC_BANDWIDTH)
                }
                break ;
            case TEF668X_API_FM_Set_Bandwidth :
            case TEF668X_API_AM_Set_Bandwidth :
                {
                    PSet_Bandwidth_FM   pSet_Bandwidth_FM ;
#if (TEF668x_API_DEBUG == 0)
                    uint8_t             write_len = 0 ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                    pSet_Bandwidth_FM = (PSet_Bandwidth_FM)TEF668x_Buf ;

                    if (cmd == TEF668X_API_FM_Set_Bandwidth) {
                        pSet_Bandwidth_FM->module= TEF668x_Module_Radio_FM ;
                    }
                    else {
                        pSet_Bandwidth_FM->module= TEF668x_Module_Radio_AM ;
                    }
                    pSet_Bandwidth_FM->cmd   = TEF668x_cmd_Set_Bandwidth ;
                    pSet_Bandwidth_FM->index = 1 ;

                    // IF bandwidth control mode
                    tmp.udata16 = va_arg(va, int) ;
                    if (cmd == TEF668X_API_FM_Set_Bandwidth) {
                        if (tmp.udata16 == IF_BandWidth_Control_Mode_Fixed
                         || tmp.udata16 == IF_BandWidth_Control_Mode_Automatic
                        ) {
                            pSet_Bandwidth_FM->IF_bandwidth_control_mode_hi = tmp.hibyte ;
                            pSet_Bandwidth_FM->IF_bandwidth_control_mode_lo = tmp.lobyte ;

                            // fixed IF bandwidth
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= MIN_AFU_FIXED_IF_BANDWIDTH
                             && tmp.udata16 <= MAX_AFU_FIXED_IF_BANDWIDTH
                            ) {
                                pSet_Bandwidth_FM->bandwidth_hi = tmp.hibyte ;
                                pSet_Bandwidth_FM->bandwidth_lo = tmp.lobyte ;

                                // FM automatic IF bandwidth control sensitivity
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 500
                                 && tmp.udata16 <= 1500
                                ) {
                                    pSet_Bandwidth_FM->control_sensitivity_hi = tmp.hibyte ;
                                    pSet_Bandwidth_FM->control_sensitivity_lo = tmp.lobyte ;

                                    // FM automatic IF bandwidth control sensitivity for low level conditions
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 500
                                     && tmp.udata16 <= 1500
                                    ) {
                                        pSet_Bandwidth_FM->low_level_sensitivity_hi = tmp.hibyte ;
                                        pSet_Bandwidth_FM->low_level_sensitivity_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                        write_len = 11 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                                        if (argc > 4) {
                                            // extended API: FM minimum IF bandwidth
                                            tmp.udata16 = va_arg(va, int) ;
                                            if (tmp.udata16 >= 560
                                             && tmp.udata16 <= 1140
                                            ) {
                                                pSet_Bandwidth_FM->min_bandwidth_hi = tmp.hibyte ;
                                                pSet_Bandwidth_FM->min_bandwidth_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                                write_len = 13 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                                                if (argc > 5) {
                                                    // extended API: FM automatic IF bandwidth control nominal bandwidth
                                                    tmp.udata16 = va_arg(va, int) ;
                                                    if (tmp.udata16 >= 1510
                                                     && tmp.udata16 <= 2360
                                                    ) {
                                                        pSet_Bandwidth_FM->nominal_bandwidth_hi = tmp.hibyte ;
                                                        pSet_Bandwidth_FM->nominal_bandwidth_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                                        write_len = 15 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                                                        if (argc > 6) {
                                                            // extended API: FM automatic IF bandwidth control attack timing
                                                            tmp.udata16 = va_arg(va, int) ;
                                                            if (tmp.udata16 >= 150
                                                             && tmp.udata16 <= 450
                                                            ) {
                                                                pSet_Bandwidth_FM->control_attack_hi = tmp.hibyte ;
                                                                pSet_Bandwidth_FM->control_attack_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                                                write_len = 17 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                                                            }   // if (tmp.udata16 >= 150 && tmp.udata16 <= 450)
                                                        }   // if (argc > 6)
                                                    }   // if (tmp.udata16 >= 1510 && tmp.udata16 <= 2360)
                                                }   // if (argc > 5)
                                            }   // if (tmp.udata16 >= 560 && tmp.udata16 <= 1140)
                                        }   // if (argc > 4)

#if (TEF668x_API_DEBUG == 0)
                                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                        , TEF668x_Buf
                                                        , write_len
                                                        , Handle_TEF668x_Service
                                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                        ret_state = STATUS_SUCCESS ;
                                    }   // if (tmp.udata16 >= 500 && tmp.udata16 <= 1500)
                                }   // if (tmp.udata16 >= 500 && tmp.udata16 <= 1500)
                            }   // if (tmp.udata16 >= MIN_AFU_FIXED_IF_BANDWIDTH && tmp.udata16 <= MAX_AFU_FIXED_IF_BANDWIDTH)
                        }   // if (tmp.udata16 == IF_BandWidth_Control_Mode_Fixed || tmp.udata16 == IF_BandWidth_Control_Mode_Automatic)
                    }   // if (cmd == TEF668X_API_FM_Set_Bandwidth)
                    else if (tmp.udata16 == IF_BandWidth_Control_Mode_Fixed) {
                        // cmd == TEF668X_API_AM_Set_Bandwidth
                        pSet_Bandwidth_FM->IF_bandwidth_control_mode_hi = tmp.hibyte ;
                        pSet_Bandwidth_FM->IF_bandwidth_control_mode_lo = tmp.lobyte ;

                        // fixed IF bandwidth
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 30
                         && tmp.udata16 <= 80
                        ) {
                            pSet_Bandwidth_FM->bandwidth_hi = tmp.hibyte ;
                            pSet_Bandwidth_FM->bandwidth_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 7
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.udata16 >= 30 && tmp.udata16 <= 80)
                    }   // if (tmp.udata16 == IF_BandWidth_Control_Mode_Fixed)
                }
                break ;
            case TEF668X_API_FM_Set_RFAGC :
            case TEF668X_API_AM_Set_RFAGC :
                {
                    PSet_RFAGC  pSet_RFAGC ;

                    pSet_RFAGC = (PSet_RFAGC)TEF668x_Buf ;

                    if (cmd == TEF668X_API_FM_Set_RFAGC) {
                        pSet_RFAGC->module= TEF668x_Module_Radio_FM ;
                    }   // if (cmd == TEF668X_API_FM_Set_RFAGC)
                    else {
                        pSet_RFAGC->module= TEF668x_Module_Radio_AM ;
                    }   // if (cmd == TEF668X_API_AM_Set_RFAGC)
                    pSet_RFAGC->cmd   = TEF668x_cmd_Set_RFAGC ;
                    pSet_RFAGC->index = 1 ;

                    // RF AGC start
                    tmp.udata16 = va_arg(va, int) ;
                    if (cmd == TEF668X_API_FM_Set_RFAGC) {
                        if (840 <= tmp.udata16
                         && tmp.udata16 <= 920
                        ) {
                            pSet_RFAGC->start_hi = tmp.hibyte ;
                            pSet_RFAGC->start_lo = tmp.lobyte ;

                            // RF AGC step extension
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 == 0
                             || tmp.udata16 == 1
                            ) {
                                pSet_RFAGC->extension_hi = tmp.hibyte ;
                                pSet_RFAGC->extension_lo = tmp.lobyte ;
                            }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                            else {
                                break ;
                            }
                        }   // if (840 <= tmp.udata16 && tmp.udata16 <= 920)
                        else {
                            break ;
                        }
                    }   // if (cmd == TEF668X_API_FM_Set_RFAGC)
                    else {
                        if (940 <= tmp.udata16
                         && tmp.udata16 <= 1020
                        ) {
                            pSet_RFAGC->start_hi = tmp.hibyte ;
                            pSet_RFAGC->start_lo = tmp.lobyte ;
                        }   // if (940 <= tmp.udata16 && tmp.udata16 <= 1020)
                        else {
                            break ;
                        }
                    }   // if (cmd == TEF668X_API_FM_Set_RFAGC)

#if (TEF668x_API_DEBUG == 0)
                    SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                    I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                    , TEF668x_Buf
                                    , 5
                                    , Handle_TEF668x_Service
                                    ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                    ret_state = STATUS_SUCCESS ;
                }
                break ;
            case TEF668X_API_FM_Set_Antenna :
            case TEF668X_API_AM_Set_Antenna :
                {
                    PSet_Antenna    pSet_Antenna ;
#if (TEF668x_API_DEBUG == 0)
                    uint8_t         write_len = 0 ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                    pSet_Antenna = (PSet_Antenna)TEF668x_Buf ;

                    if (cmd == TEF668X_API_FM_Set_Antenna) {
                        // LNA gain reduction, FM LNA attenuation setting used during FM off, callback
                        if (argc != 2) {
                            break ;
                        }
                        else {
                            pSet_Antenna->module= TEF668x_Module_Radio_FM ;
                        }
                    }   // if (cmd == TEF668X_API_FM_Set_Antenna)
                    else {
                        // LNA gain reduction, callback
                        if (argc != 1) {
                            break ;
                        }
                        else {
                            pSet_Antenna->module= TEF668x_Module_Radio_AM ;
                        }
                    }   // if (cmd == TEF668X_API_AM_Set_Antenna)
                    pSet_Antenna->cmd   = TEF668x_cmd_Set_Antenna ;
                    pSet_Antenna->index = 1 ;

                    // LNA gain reduction
                    tmp.udata16 = va_arg(va, int) ;
                    if (tmp.udata16 == 0                //  0 dB
                     || tmp.udata16 == 60               //  6 dB
                     || tmp.udata16 == 120              // 12 dB
                     || tmp.udata16 == 180              // 18 dB
                     || tmp.udata16 == 240              // 24 dB
                     || tmp.udata16 == 300              // 30 dB
                     || tmp.udata16 == 360              // 36 dB
                     ) {
                        pSet_Antenna->attenuation_hi = tmp.hibyte ;
                        pSet_Antenna->attenuation_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                        write_len = 5 ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                        if (cmd == TEF668X_API_FM_Set_Antenna) {
                            // FM LNA attenuation setting used during FM off
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 == 300      // 30 dB
                             || tmp.udata16 == 420      // 42 dB
                             ) {
                                pSet_Antenna->off_mode_hi = tmp.hibyte ;
                                pSet_Antenna->off_mode_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                write_len = 7 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                            }
                            else {
                                break ;
                            }   // if (tmp.udata16 == 300 || tmp.udata16 == 420 )
                        }   // if (cmd == TEF668X_API_FM_Set_Antenna)

#if (TEF668x_API_DEBUG == 0)
                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                        , TEF668x_Buf
                                        , write_len
                                        , Handle_TEF668x_Service
                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                        ret_state = STATUS_SUCCESS ;
                    }
                    else {
                        break ;
                    }
                }
                break ;
            case TEF668X_API_FM_Set_MphSuppression :
                {
                    if (argc == 1) {
                        PSet_MphSuppression pSet_MphSuppression ;

                        pSet_MphSuppression = (PSet_MphSuppression)TEF668x_Buf ;

                        pSet_MphSuppression->module = TEF668x_Module_Radio_FM ;
                        pSet_MphSuppression->cmd    = TEF668x_cmd_Set_MphSuppression ;
                        pSet_MphSuppression->index  = 1 ;

                        // FM multiple path suppression
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0        // off (default)
                         || tmp.udata16 == 1        // on
                         ) {
                            pSet_MphSuppression->mode_hi = tmp.hibyte ;
                            pSet_MphSuppression->mode_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 5
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Set_ChannelEqualizer :
                {
                    if (argc == 1) {
                        PSet_ChannelEqualizer   pSet_ChannelEqualizer ;

                        pSet_ChannelEqualizer = (PSet_ChannelEqualizer)TEF668x_Buf ;

                        pSet_ChannelEqualizer->module = TEF668x_Module_Radio_FM ;
                        pSet_ChannelEqualizer->cmd    = TEF668x_cmd_Set_ChannelEqualizer ;
                        pSet_ChannelEqualizer->index  = 1 ;

                        // FM channel equalizer
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0        // off (default)
                         || tmp.udata16 == 1        // on
                         ) {
                            pSet_ChannelEqualizer->mode_hi = tmp.hibyte ;
                            pSet_ChannelEqualizer->mode_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 5
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Set_NoiseBlanker :
            case TEF668X_API_AM_Set_NoiseBlanker :
                {
                    PSet_NoiseBlanker   pSet_NoiseBlanker ;
#if (TEF668x_API_DEBUG == 0)
                    uint8_t             write_len = 0 ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                    pSet_NoiseBlanker = (PSet_NoiseBlanker)TEF668x_Buf ;

                    if (cmd == TEF668X_API_FM_Set_NoiseBlanker) {
                        pSet_NoiseBlanker->module = TEF668x_Module_Radio_FM ;
                    }   // if (cmd == TEF668X_API_FM_Set_NoiseBlanker)
                    else {
                        pSet_NoiseBlanker->module = TEF668x_Module_Radio_AM ;
                    }
                    pSet_NoiseBlanker->cmd    = TEF668x_cmd_Set_NoiseBlanker ;
                    pSet_NoiseBlanker->index  = 1 ;

                    // noise blanker
                    tmp.udata16 = va_arg(va, int) ;
                    if (tmp.udata16 == 0        // off (default)
                     || tmp.udata16 == 1        // on
                     ) {
                        pSet_NoiseBlanker->mode_hi = tmp.hibyte ;
                        pSet_NoiseBlanker->mode_lo = tmp.lobyte ;

                        // trigger sensitivity
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 500  // 50%
                         && tmp.udata16 <= 1500 // 150%
                         ) {
                            pSet_NoiseBlanker->sensitivity_hi = tmp.hibyte ;
                            pSet_NoiseBlanker->sensitivity_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            write_len = 7 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                            if (cmd == TEF668X_API_FM_Set_NoiseBlanker) {
                                // trigger sensitivity on level detection
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 500  // 50%
                                 && tmp.udata16 <= 1500 // 150%
                                 ) {
                                    pSet_NoiseBlanker->level_sensitivity_hi = tmp.hibyte ;
                                    pSet_NoiseBlanker->level_sensitivity_lo = tmp.lobyte ;
#if (TEF668x_API_DEBUG == 0)
                                    write_len = 9 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                                    if (argc > 3) {
                                        // extended API: modulation dependency on trigger sensitivity
                                        tmp.udata16 = va_arg(va, int) ;
                                        if (tmp.udata16 >= 330  // 33%
                                         && tmp.udata16 <= 1660 // 166%
                                         ) {
                                            pSet_NoiseBlanker->modulation_hi = tmp.hibyte ;
                                            pSet_NoiseBlanker->modulation_lo = tmp.lobyte ;
#if (TEF668x_API_DEBUG == 0)
                                            write_len = 11 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                                            if (argc > 4) {
                                                // extended API: sensitivity offset
                                                tmp.udata16 = va_arg(va, int) ;
                                                if (tmp.udata16 >= 25   // 2.5%
                                                 && tmp.udata16 <= 200  // 20%
                                                 ) {
                                                    pSet_NoiseBlanker->offset_hi = tmp.hibyte ;
                                                    pSet_NoiseBlanker->offset_lo = tmp.lobyte ;
#if (TEF668x_API_DEBUG == 0)
                                                    write_len = 13 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                                                    if (argc > 5) {
                                                        // extended API: trigger reference attack time
                                                        tmp.udata16 = va_arg(va, int) ;
                                                        if (tmp.udata16 >= 15   // 15 us
                                                         && tmp.udata16 <= 1200 // 1200 us
                                                         ) {
                                                            pSet_NoiseBlanker->attack_hi = tmp.hibyte ;
                                                            pSet_NoiseBlanker->attack_lo = tmp.lobyte ;
#if (TEF668x_API_DEBUG == 0)
                                                            write_len = 15 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                                                            if (argc > 6) {
                                                                // extended API: trigger reference decay time
                                                                tmp.udata16 = va_arg(va, int) ;
                                                                if (tmp.udata16 >= 150  // 150 us
                                                                 && tmp.udata16 <= 6000 // 6 ms
                                                                 ) {
                                                                    pSet_NoiseBlanker->decay_hi = tmp.hibyte ;
                                                                    pSet_NoiseBlanker->decay_lo = tmp.lobyte ;
#if (TEF668x_API_DEBUG == 0)
                                                                    write_len = 17 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                                                                }   // if (tmp.udata16 >= 150 && tmp.udata16 <= 6000)
                                                                else {
                                                                    break ;
                                                                }
                                                            }   // if (argc > 6)
                                                        }   // if (tmp.udata16 >= 15 && tmp.udata16 <= 1200)
                                                        else {
                                                            break ;
                                                        }
                                                    }   // if (argc > 5)
                                                }   // if (tmp.udata16 >= 25 && tmp.udata16 <= 200)
                                                else {
                                                    break ;
                                                }
                                            }   // if (argc > 4)
                                        }   // if (tmp.udata16 >= 330 && tmp.udata16 <= 1660)
                                        else {
                                            break ;
                                        }
                                    }   // if (argc > 3)
                                }   // if (tmp.udata16 >= 500 && tmp.udata16 <= 1500)
                                else {
                                    break ;
                                }
                            }   // if (cmd == TEF668X_API_FM_Set_NoiseBlanker)
                            else {
                                if (argc > 2) {
                                    // extended API: AM trigger sensitivity for noise conditions
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 600  // 60 %
                                     && tmp.udata16 <= 1600 // 160 %
                                    ) {
                                        pSet_NoiseBlanker->gain_hi = tmp.hibyte ;
                                        pSet_NoiseBlanker->gain_lo = tmp.lobyte ;
#if (TEF668x_API_DEBUG == 0)
                                        write_len = 9 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                                        if (argc > 3) {
                                            // extended API: AM noise blanker blank time
                                            tmp.udata16 = va_arg(va, int) ;
                                            if (tmp.udata16 >= 25   // 25 us
                                             && tmp.udata16 <= 250  // 250 us
                                            ) {
                                                pSet_NoiseBlanker->blank_time_hi = tmp.hibyte ;
                                                pSet_NoiseBlanker->blank_time_lo = tmp.lobyte ;
#if (TEF668x_API_DEBUG == 0)
                                                write_len = 11 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                                            }   // if (tmp.udata16 >= 25 && tmp.udata16 <= 250)
                                            else {
                                                break ;
                                            }
                                        }   // if (argc > 3)
                                    }   // if (tmp.udata16 >= 600 && tmp.udata16 <= 1600)
                                    else {
                                        break ;
                                    }
                                }   // if (argc > 2)
                            }   // // if (cmd == TEF668X_API_AM_Set_NoiseBlanker)
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , write_len
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.udata16 >= 500 && tmp.udata16 <= 1500)
                    }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                }
                break ;
            case TEF668X_API_FM_Set_NoiseBlanker_Options :
                {
                    if (argc == 4) {
                        PSet_NoiseBlanker_Options   pSet_NoiseBlanker_Options ;

                        pSet_NoiseBlanker_Options = (PSet_NoiseBlanker_Options)TEF668x_Buf ;

                        pSet_NoiseBlanker_Options->module = TEF668x_Module_Radio_FM ;
                        pSet_NoiseBlanker_Options->cmd    = TEF668x_cmd_Set_NoiseBlanker_Options ;
                        pSet_NoiseBlanker_Options->index  = 1 ;

                        // noise blanker blank time on low modulation
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 75       // 75 µs pulse stretch time
                         && tmp.udata16 <= 300      // 300 µs pulse stretch time
                         ) {
                            pSet_NoiseBlanker_Options->blank_time_hi = tmp.hibyte ;
                            pSet_NoiseBlanker_Options->blank_time_lo = tmp.lobyte ;

                            // noise blanker blank time on high modulation
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 75       // 75 µs pulse stretch time
                             && tmp.udata16 <= 300      // 300 µs pulse stretch time
                             ) {
                                pSet_NoiseBlanker_Options->blank_time2_hi = tmp.hibyte ;
                                pSet_NoiseBlanker_Options->blank_time2_lo = tmp.lobyte ;

                                // modulation dependent blank time
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 160      // 16 % modulation (= 12 … 125 kHz FM dev.)
                                 && tmp.udata16 <= 1660     // 166 % modulation (= 12 … 125 kHz FM dev.)
                                 ) {
                                    pSet_NoiseBlanker_Options->blank_modulation_hi = tmp.hibyte ;
                                    pSet_NoiseBlanker_Options->blank_modulation_lo = tmp.lobyte ;

                                    // noise blanker blank time on level detection
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 100      // 100µs pulse stretch time
                                     && tmp.udata16 <= 400      // 400µs pulse stretch time
                                     ) {
                                        pSet_NoiseBlanker_Options->blank_time_level_hi = tmp.hibyte ;
                                        pSet_NoiseBlanker_Options->blank_time_level_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                        , TEF668x_Buf
                                                        , 11
                                                        , Handle_TEF668x_Service
                                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                        ret_state = STATUS_SUCCESS ;
                                    }   // if (tmp.udata16 >= 100 && tmp.udata16 <= 400)
                                }   // if (tmp.udata16 >= 160 && tmp.udata16 <= 1660)
                            }   // if (tmp.udata16 >= 75 && tmp.udata16 <= 300)
                        }   // if (tmp.udata16 >= 75 && tmp.udata16 <= 300)
                    }   // if (argc == 4)
                }
                break ;
            case TEF668X_API_FM_Set_DigitalRadio :
            case TEF668X_API_AM_Set_DigitalRadio :
                {
                    if (argc == 1) {
                        PSet_DigitalRadio   pSet_DigitalRadio ;

                        pSet_DigitalRadio = (PSet_DigitalRadio)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_DigitalRadio) {
                            pSet_DigitalRadio->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_DigitalRadio)
                        else {
                            pSet_DigitalRadio->module = TEF668x_Module_Radio_AM ;
                        }
                        pSet_DigitalRadio->cmd    = TEF668x_cmd_Set_DigitalRadio ;
                        pSet_DigitalRadio->index  = 1 ;

                        // digital radio
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0        // off (default)
                         || tmp.udata16 == 1        // on
                        ) {
                            pSet_DigitalRadio->mode_hi = tmp.hibyte ;
                            pSet_DigitalRadio->mode_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 5
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Set_Deemphasis :
                {
                    if (argc == 1) {
                        PSet_Deemphasis pSet_Deemphasis ;

                        pSet_Deemphasis = (PSet_Deemphasis)TEF668x_Buf ;

                        pSet_Deemphasis->module = TEF668x_Module_Radio_FM ;
                        pSet_Deemphasis->cmd    = TEF668x_cmd_Set_Deemphasis ;
                        pSet_Deemphasis->index  = 1 ;

                        // Selection of FM de-emphasis time constant
                        tmp.udata16 = va_arg(va, int) ;
                        pSet_Deemphasis->timeconstant_hi = tmp.hibyte ;
                        pSet_Deemphasis->timeconstant_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                        , TEF668x_Buf
                                        , 5
                                        , Handle_TEF668x_Service
                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                        ret_state = STATUS_SUCCESS ;
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Set_StereoImprovement :
                {
                    if (argc == 1) {
                        PSet_StereoImprovement  pSet_StereoImprovement ;

                        pSet_StereoImprovement = (PSet_StereoImprovement)TEF668x_Buf ;

                        pSet_StereoImprovement->module = TEF668x_Module_Radio_FM ;
                        pSet_StereoImprovement->cmd    = TEF668x_cmd_Set_StereoImprovement ;
                        pSet_StereoImprovement->index  = 1 ;

                        // FM stereo extended weak signal handling
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = stereo high blend (default)
                         || tmp.udata16 == 1    // 1 = FMSI stereo band blend system
                         ) {
                            pSet_StereoImprovement->mode_hi = tmp.hibyte ;
                            pSet_StereoImprovement->mode_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 5
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Set_Highcut_Fix :
            case TEF668X_API_AM_Set_Highcut_Fix :
                {
                    if (argc == 2) {
                        PSet_Highcut_Fix    pSet_Highcut_Fix ;

                        pSet_Highcut_Fix = (PSet_Highcut_Fix)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_Highcut_Fix) {
                            pSet_Highcut_Fix->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Highcut_Fix)
                        else {
                            pSet_Highcut_Fix->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_Highcut_Fix)

                        pSet_Highcut_Fix->cmd    = TEF668x_cmd_Set_Highcut_Fix ;
                        pSet_Highcut_Fix->index  = 1 ;

                        // independent audio low-pass filter
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off; high audio frequency bandwidth is not limited (default)
                         || tmp.udata16 == 1    // 1 = on; high audio frequencies are attenuated by 1 st  order ‘IIR’ low-pass
                         ) {
                            pSet_Highcut_Fix->mode_hi = tmp.hibyte ;
                            pSet_Highcut_Fix->mode_lo = tmp.lobyte ;

                            // fixed highcut attenuation
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 1000     // 1.0 … 15 kHz  -3 dB att. for fixed highcut filter
                             && tmp.udata16 <= 15000
                             ) {
                                pSet_Highcut_Fix->limit_hi = tmp.hibyte ;
                                pSet_Highcut_Fix->limit_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                , TEF668x_Buf
                                                , 7
                                                , Handle_TEF668x_Service
                                                ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                ret_state = STATUS_SUCCESS ;
                            }   // tmp.udata16 >= 1000 && tmp.udata16 <= 15000)
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_FM_Set_Lowcut_Fix :
            case TEF668X_API_AM_Set_Lowcut_Fix :
                {
                    if (argc == 2) {
                        PSet_Lowcut_Fix     pSet_Lowcut_Fix ;

                        pSet_Lowcut_Fix = (PSet_Lowcut_Fix)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_Lowcut_Fix) {
                            pSet_Lowcut_Fix->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Lowcut_Fix)
                        else {
                            pSet_Lowcut_Fix->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_Lowcut_Fix)

                        pSet_Lowcut_Fix->cmd    = TEF668x_cmd_Set_Lowcut_Fix ;
                        pSet_Lowcut_Fix->index  = 1 ;

                        // independent audio high-pass filter
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off; low audio frequency bandwidth is not limited (default)
                         || tmp.udata16 == 1    // 1 = on; low audio frequencies are attenuated by 1st order ‘IIR’ high-pass
                         ) {
                            pSet_Lowcut_Fix->mode_hi = tmp.hibyte ;
                            pSet_Lowcut_Fix->mode_lo = tmp.lobyte ;

                            // fixed lowcut attenuation
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 20     //  20 … 200 Hz  -3 dB attenuation for fixed lowcut filter
                             && tmp.udata16 <= 200
                             ) {
                                pSet_Lowcut_Fix->limit_hi = tmp.hibyte ;
                                pSet_Lowcut_Fix->limit_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                , TEF668x_Buf
                                                , 7
                                                , Handle_TEF668x_Service
                                                ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                ret_state = STATUS_SUCCESS ;
                            }   // tmp.udata16 >= 20 && tmp.udata16 <= 200)
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_FM_Set_LevelStep :
            case TEF668X_API_AM_Set_LevelStep :
                {
                    if (argc == 7) {
                        PSet_LevelStep     pSet_LevelStep ;

                        pSet_LevelStep = (PSet_LevelStep)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_LevelStep) {
                            pSet_LevelStep->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_LevelStep)
                        else {
                            pSet_LevelStep->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_LevelStep)

                        pSet_LevelStep->cmd    = TEF668x_cmd_Set_LevelStep ;
                        pSet_LevelStep->index  = 1 ;

                        // step1: level offset for an AGC step from 0 to 1
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.data16 >= -60   // -60 … 0 (*0.1 dB) = -6 … 0 dB
                         && tmp.data16 <= 0
                         ) {
                            pSet_LevelStep->step1_hi = tmp.hibyte ;
                            pSet_LevelStep->step1_lo = tmp.lobyte ;

                            // step2: level offset for an AGC step from 1 to 2
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.data16 >= -60   // -60 … 0 (*0.1 dB) = -6 … 0 dB
                             && tmp.data16 <= 0
                             ) {
                                pSet_LevelStep->step2_hi = tmp.hibyte ;
                                pSet_LevelStep->step2_lo = tmp.lobyte ;

                                // step3: level offset for an AGC step from 2 to 3
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.data16 >= -60   // -60 … 0 (*0.1 dB) = -6 … 0 dB
                                 && tmp.data16 <= 0
                                 ) {
                                    pSet_LevelStep->step3_hi = tmp.hibyte ;
                                    pSet_LevelStep->step3_lo = tmp.lobyte ;

                                    // step4: level offset for an AGC step from 3 to 4
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.data16 >= -60   // -60 … 0 (*0.1 dB) = -6 … 0 dB
                                     && tmp.data16 <= 0
                                     ) {
                                        pSet_LevelStep->step4_hi = tmp.hibyte ;
                                        pSet_LevelStep->step4_lo = tmp.lobyte ;

                                        // step5: level offset for an AGC step from 4 to 5
                                        tmp.udata16 = va_arg(va, int) ;
                                        if (tmp.data16 >= -60   // -60 … 0 (*0.1 dB) = -6 … 0 dB
                                         && tmp.data16 <= 0
                                         ) {
                                            pSet_LevelStep->step5_hi = tmp.hibyte ;
                                            pSet_LevelStep->step5_lo = tmp.lobyte ;

                                            // step6: level offset for an AGC step from 5 to 6
                                            tmp.udata16 = va_arg(va, int) ;
                                            if (tmp.data16 >= -60   // -60 … 0 (*0.1 dB) = -6 … 0 dB
                                             && tmp.data16 <= 0
                                             ) {
                                                pSet_LevelStep->step6_hi = tmp.hibyte ;
                                                pSet_LevelStep->step6_lo = tmp.lobyte ;

                                                // step7: level offset for an AGC step from 5 to 6
                                                tmp.udata16 = va_arg(va, int) ;
                                                if (tmp.data16 >= -60   // -60 … 0 (*0.1 dB) = -6 … 0 dB
                                                 && tmp.data16 <= 0
                                                 ) {
                                                    pSet_LevelStep->step7_hi = tmp.hibyte ;
                                                    pSet_LevelStep->step7_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                                    SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                                    I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                                    , TEF668x_Buf
                                                                    , 17
                                                                    , Handle_TEF668x_Service
                                                                    ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                                    ret_state = STATUS_SUCCESS ;
                                                }   // if (tmp.data16 >= -60 && tmp.data16 <= 0)
                                            }   // if (tmp.data16 >= -60 && tmp.data16 <= 0)
                                        }   // if (tmp.data16 >= -60 && tmp.data16 <= 0)
                                    }   // if (tmp.data16 >= -60 && tmp.data16 <= 0)
                                }   // if (tmp.data16 >= -60 && tmp.data16 <= 0)
                            }   // if (tmp.data16 >= -60 && tmp.data16 <= 0)
                        }   // if (tmp.data16 >= -60 && tmp.data16 <= 0)
                    }   // if (argc == 7)
                }
                break ;
            case TEF668X_API_FM_Set_LevelOffset :
            case TEF668X_API_AM_Set_LevelOffset :
                {
                    if (argc == 1) {
                        PSet_LevelOffset    pSet_LevelOffset ;

                        pSet_LevelOffset = (PSet_LevelOffset)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_LevelOffset) {
                            pSet_LevelOffset->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Highcut_Fix)
                        else {
                            pSet_LevelOffset->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_LevelOffset)

                        pSet_LevelOffset->cmd    = TEF668x_cmd_Set_LevelOffset ;
                        pSet_LevelOffset->index  = 1 ;

                        // level offset
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.data16 >= -480  //-480 … +150 (*0.1 dB) = -48 … +15 dB
                         && tmp.data16 <= 150
                         ) {
                            pSet_LevelOffset->offset_hi = tmp.hibyte ;
                            pSet_LevelOffset->offset_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 5
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // tmp.data16 >= -480 && tmp.data16 <= 150)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Set_Softmute_Time :
            case TEF668X_API_AM_Set_Softmute_Time :
                {
                    if (argc == 4) {
                        PSet_Softmute   pSet_Softmute ;

                        pSet_Softmute = (PSet_Softmute)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_Softmute_Time) {
                            pSet_Softmute->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Softmute_Time)
                        else {
                            pSet_Softmute->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_Softmute_Time)

                        pSet_Softmute->cmd    = TEF668x_cmd_Set_Softmute_Time ;
                        pSet_Softmute->index  = 1 ;

                        // slow_attack : slow attack time of weak signal handling
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 60   // 60 … 2000 (ms) = 60 ms … 2s slow attack time
                         && tmp.udata16 <= 2000
                         ) {
                            pSet_Softmute->slow_attack_hi = tmp.hibyte ;
                            pSet_Softmute->slow_attack_lo = tmp.lobyte ;

                            // slow_decay : slow decay time of weak signal handling
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 120  // 120 … 12500 (ms) = 120 ms … 12.5 s slow decay time
                             && tmp.udata16 <= 12500
                             ) {
                                pSet_Softmute->slow_decay_hi = tmp.hibyte ;
                                pSet_Softmute->slow_decay_lo = tmp.lobyte ;

                                // fast_attack : fast attack time of weak signal handling
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 10   // 10 … 1200 (*0.1 ms) = 1 ms … 120 ms fast attack time
                                 && tmp.udata16 <= 1200
                                 ) {
                                    pSet_Softmute->fast_attack_hi = tmp.hibyte ;
                                    pSet_Softmute->fast_attack_lo = tmp.lobyte ;

                                    // fast_decay : fast decay time of weak signal handling
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 20   // 20 … 5000 ( *0.1 ms) = 2 ms … 500 ms fast decay time
                                     && tmp.udata16 <= 5000
                                     ) {
                                        pSet_Softmute->fast_decay_hi = tmp.hibyte ;
                                        pSet_Softmute->fast_decay_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                        , TEF668x_Buf
                                                        , 11
                                                        , Handle_TEF668x_Service
                                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                        ret_state = STATUS_SUCCESS ;
                                    }   // if (tmp.udata16 >= 20 && tmp.udata16 <= 5000)
                                }   // if (tmp.udata16 >= 10 && tmp.udata16 <= 1200)
                            }   // if (tmp.udata16 >= 120 && tmp.udata16 <= 12500)
                        }   // if (tmp.udata16 >= 60 && tmp.udata16 <= 2000)
                    }   // if (argc == 4)
                }
                break ;
            case TEF668X_API_FM_Set_Softmute_Level :
            case TEF668X_API_AM_Set_Softmute_Level :
                {
                    if (argc == 3) {
                        PSet_Softmute_Level pSet_Softmute_Level ;

                        pSet_Softmute_Level = (PSet_Softmute_Level)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_Softmute_Level) {
                            pSet_Softmute_Level->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Softmute_Level)
                        else {
                            pSet_Softmute_Level->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_aM_Set_Softmute_Level)

                        pSet_Softmute_Level->cmd    = TEF668x_cmd_Set_Softmute_Level ;
                        pSet_Softmute_Level->index  = 1 ;

                        // mode : timer selection
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 0    // 0 = off (only for evaluation)
                         && tmp.udata16 <= 3    // 1 = fast timer control
                                                // 2 = slow timer control (default)
                                                // 3 = dual timer control; combined fast and slow timer control
                         ) {
                            pSet_Softmute_Level->mode_hi = tmp.hibyte ;
                            pSet_Softmute_Level->mode_lo = tmp.lobyte ;

                            // start : weak signal handling level start
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 0    // 0 … 500 [*0.1 dBuV] = control when level falls below 0 dBV … 50 dBuV
                             && tmp.udata16 <= 500
                             ) {
                                pSet_Softmute_Level->start_hi = tmp.hibyte ;
                                pSet_Softmute_Level->start_lo = tmp.lobyte ;

                                // slope : weak signal handling level range
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 60   // 60 … 300 [*0.1 dB] = control over level range of 6 dB … 30 dB
                                 && tmp.udata16 <= 300
                                 ) {
                                    pSet_Softmute_Level->slope_hi = tmp.hibyte ;
                                    pSet_Softmute_Level->slope_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                    SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                    I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                    , TEF668x_Buf
                                                    , 9
                                                    , Handle_TEF668x_Service
                                                    ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                    ret_state = STATUS_SUCCESS ;
                                }   // if (tmp.udata16 >= 60 && tmp.udata16 <= 300)
                            }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 500)
                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 3)
                    }   // if (argc == 3)
                }
                break ;
            case TEF668X_API_FM_Set_Softmute_Noise :
                {
                    if (argc == 5) {
                        PSet_Softmute_Noise pSet_Softmute_Noise ;

                        pSet_Softmute_Noise = (PSet_Softmute_Noise)TEF668x_Buf ;

                        pSet_Softmute_Noise->module = TEF668x_Module_Radio_FM ;
                        pSet_Softmute_Noise->cmd    = TEF668x_cmd_Set_Softmute_Noise ;
                        pSet_Softmute_Noise->index  = 1 ;

                        // mode : timer selection
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 0    // 0 = off (default)
                         && tmp.udata16 <= 3    // 1 = fast timer control
                                                // 2 = slow timer control
                                                // 3 = dual timer control; combined fast and slow timer control
                         ) {
                            pSet_Softmute_Noise->mode_hi = tmp.hibyte ;
                            pSet_Softmute_Noise->mode_lo = tmp.lobyte ;

                            // start : FM weak signal handling noise start
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 0    // 0 … 800 [*0.1 %] = control when noise above 0… 80% of USN detector
                             && tmp.udata16 <= 800
                             ) {
                                pSet_Softmute_Noise->start_hi = tmp.hibyte ;
                                pSet_Softmute_Noise->start_lo = tmp.lobyte ;

                                // slope : FM weak signal handling noise range
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 100  // 100 … 1000 [*0.1 %] = control over range of 10… 100% of USN detector
                                 && tmp.udata16 <= 1000
                                 ) {
                                    pSet_Softmute_Noise->slope_hi = tmp.hibyte ;
                                    pSet_Softmute_Noise->slope_lo = tmp.lobyte ;

                                    // limit_mode : FM softmute on noise dynamic attenuation limit mode
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 == 0    // 0 = generic limit set by FM cmd 45 Set_Softmute_Max (default)
                                     || tmp.udata16 == 1    // 1 = independent limit set by parameter 5
                                     ) {
                                        pSet_Softmute_Noise->limit_mode_hi = tmp.hibyte ;
                                        pSet_Softmute_Noise->limit_mode_lo = tmp.lobyte ;

                                        // limit : FM softmute on noise dynamic attenuation limi
                                        tmp.udata16 = va_arg(va, int) ;
                                        if (tmp.udata16 >= 0    // 0 …. 400 [*0.1 dB] = 0 … 40 dB softmute maximum attenuation
                                         && tmp.udata16 <= 400
                                         ) {
                                            pSet_Softmute_Noise->limit_hi = tmp.hibyte ;
                                            pSet_Softmute_Noise->limit_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                            , TEF668x_Buf
                                                            , 13
                                                            , Handle_TEF668x_Service
                                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                            ret_state = STATUS_SUCCESS ;
                                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 400)
                                    }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                                }   // if (tmp.udata16 >= 100 && tmp.udata16 <= 1000)
                            }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 800)
                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 3)
                    }   // if (argc == 5)
                }
                break ;
            case TEF668X_API_FM_Set_Softmute_Mph :
                {
                    if (argc == 5) {
                        PSet_Softmute_Mph   pSet_Softmute_Mph ;

                        pSet_Softmute_Mph = (PSet_Softmute_Mph)TEF668x_Buf ;

                        pSet_Softmute_Mph->module = TEF668x_Module_Radio_FM ;
                        pSet_Softmute_Mph->cmd    = TEF668x_cmd_Set_Softmute_Mph ;
                        pSet_Softmute_Mph->index  = 1 ;

                        // mode : timer selection
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 0    // 0 = off (default)
                         && tmp.udata16 <= 3    // 1 = fast timer control
                                                // 2 = slow timer control
                                                // 3 = dual timer control; combined fast and slow timer control
                         ) {
                            pSet_Softmute_Mph->mode_hi = tmp.hibyte ;
                            pSet_Softmute_Mph->mode_lo = tmp.lobyte ;

                            // start : FM weak signal handling multipath start
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 0    // 0 … 800 [*0.1 %] = control when mph above 0… 80% of WAM detector
                             && tmp.udata16 <= 800
                             ) {
                                pSet_Softmute_Mph->start_hi = tmp.hibyte ;
                                pSet_Softmute_Mph->start_lo = tmp.lobyte ;

                                // slope : FM weak signal handling multipath range
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 100  // 100 … 1000 [*0.1 %] = control over range of 10… 100% of WAM detector
                                 && tmp.udata16 <= 1000
                                 ) {
                                    pSet_Softmute_Mph->slope_hi = tmp.hibyte ;
                                    pSet_Softmute_Mph->slope_lo = tmp.lobyte ;

                                    // limit_mode : FM softmute on multipath dynamic attenuation limit mode
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 == 0    // 0 = generic limit set by FM cmd 45 Set_Softmute_Max (default)
                                     || tmp.udata16 == 1    // 1 = independent limit set by parameter 5
                                     ) {
                                        pSet_Softmute_Mph->limit_mode_hi = tmp.hibyte ;
                                        pSet_Softmute_Mph->limit_mode_lo = tmp.lobyte ;

                                        // limit : FM softmute on multipath dynamic attenuation limit
                                        tmp.udata16 = va_arg(va, int) ;
                                        if (tmp.udata16 >= 0    // 0 …. 400 [*0.1 dB] = 0 … 40 dB softmute maximum attenuation
                                         && tmp.udata16 <= 400
                                         ) {
                                            pSet_Softmute_Mph->limit_hi = tmp.hibyte ;
                                            pSet_Softmute_Mph->limit_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                            , TEF668x_Buf
                                                            , 13
                                                            , Handle_TEF668x_Service
                                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                            ret_state = STATUS_SUCCESS ;
                                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 400)
                                    }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                                }   // if (tmp.udata16 >= 100 && tmp.udata16 <= 1000)
                            }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 800)
                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 3)
                    }   // if (argc == 5)
                }
                break ;
            case TEF668X_API_FM_Set_Softmute_Max :
            case TEF668X_API_AM_Set_Softmute_Max :
                {
                    if (argc == 2) {
                        PSet_Softmute_Max   pSet_Softmute_Max ;

                        pSet_Softmute_Max = (PSet_Softmute_Max)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_Softmute_Max) {
                            pSet_Softmute_Max->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Softmute_Max)
                        else {
                            pSet_Softmute_Max->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_Softmute_Max)

                        pSet_Softmute_Max->cmd    = TEF668x_cmd_Set_Softmute_Max ;
                        pSet_Softmute_Max->index  = 1 ;

                        // mode : weak signal handling (dynamic control)
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off (for evaluation only)
                         || tmp.udata16 == 1    // 1 = on; maximum dynamic control defined by limit parameter (default)
                         ) {
                            pSet_Softmute_Max->mode_hi = tmp.hibyte ;
                            pSet_Softmute_Max->mode_lo = tmp.lobyte ;

                            // limit : softmute dynamic attenuation limit
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 0    // 0 … 400 [*0.1 dB] = 0 … 40 dB  softmute maximum attenuation
                             && tmp.udata16 <= 400
                             ) {
                                pSet_Softmute_Max->limit_hi = tmp.hibyte ;
                                pSet_Softmute_Max->limit_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                , TEF668x_Buf
                                                , 7
                                                , Handle_TEF668x_Service
                                                ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                ret_state = STATUS_SUCCESS ;
                            }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 400)
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_FM_Set_Highcut_Time :
            case TEF668X_API_AM_Set_Highcut_Time :
                {
                    if (argc == 4) {
                        PSet_Highcut_Time   pSet_Highcut_Time ;

                        pSet_Highcut_Time = (PSet_Highcut_Time)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_Highcut_Time) {
                            pSet_Highcut_Time->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Highcut_Time)
                        else {
                            pSet_Highcut_Time->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_Highcut_Time)
                        pSet_Highcut_Time->cmd   = TEF668x_cmd_Set_Highcut_Time ;
                        pSet_Highcut_Time->index = 1 ;

                        // slow_attack : slow attack time of weak signal handling
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 60    // 60 … 2000 (ms) = 60 ms … 2 s slow attack time
                         && tmp.udata16 <= 2000
                         ) {
                            pSet_Highcut_Time->slow_attack_hi = tmp.hibyte ;
                            pSet_Highcut_Time->slow_attack_lo = tmp.lobyte ;

                            // slow_decay : slow decay time of weak signal handling
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 120      // 120 … 12500 (ms) = 120 ms … 12.5 s slow decay time
                             && tmp.udata16 <= 12500
                             ) {
                                pSet_Highcut_Time->slow_decay_hi = tmp.hibyte ;
                                pSet_Highcut_Time->slow_decay_lo = tmp.lobyte ;

                                // fast_attack : fast attack time of weak signal handling
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 10       // 10 … 1200 (*0.1 ms) = 1 ms … 120 ms fast attack time
                                 && tmp.udata16 <= 1200
                                 ) {
                                    pSet_Highcut_Time->fast_attack_hi = tmp.hibyte ;
                                    pSet_Highcut_Time->fast_attack_lo = tmp.lobyte ;

                                    // fast_decay : fast decay time of weak signal handling
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 20       // 20 … 5000 ( *0.1 ms) = 2 ms … 500 ms fast decay time
                                     && tmp.udata16 <= 5000
                                     ) {
                                        pSet_Highcut_Time->fast_attack_hi = tmp.hibyte ;
                                        pSet_Highcut_Time->fast_attack_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                        , TEF668x_Buf
                                                        , 11
                                                        , Handle_TEF668x_Service
                                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                        ret_state = STATUS_SUCCESS ;
                                    }   // if (tmp.udata16 >= 20 && tmp.udata16 <= 5000)
                                }   // if (tmp.udata16 >= 10 && tmp.udata16 <= 1200)
                            }   // if (tmp.udata16 >= 120 && tmp.udata16 <= 12500)
                        }   // if (tmp.udata16 >= 60 && tmp.udata16 <= 2000)
                    }   // if (argc == 4)
                }
                break ;
            case TEF668X_API_FM_Set_Highcut_Mod :
            case TEF668X_API_AM_Set_Highcut_Mod :
                {
                    if (argc == 4) {
                        PSet_Highcut_Mod    pSet_Highcut_Mod ;

                        pSet_Highcut_Mod = (PSet_Highcut_Mod)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_Highcut_Mod) {
                            pSet_Highcut_Mod->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Highcut_Mod)
                        else {
                            pSet_Highcut_Mod->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_Highcut_Mod)
                        pSet_Highcut_Mod->cmd   = TEF668x_cmd_Set_Highcut_Mod ;
                        pSet_Highcut_Mod->index = 1 ;

                        // mode : modulation dependent weak signal handling
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off (default)
                         || tmp.udata16 == 1    // 1 = on (independent modulation timer)
                         ) {
                            pSet_Highcut_Mod->mode_hi = tmp.hibyte ;
                            pSet_Highcut_Mod->mode_lo = tmp.lobyte ;

                            // start : weak signal handling modulation start
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 100      // 100 … 1000 [*0.1 %] = control when modulation falls below 10% … 100%
                             && tmp.udata16 <= 1000
                             ) {
                                pSet_Highcut_Mod->start_hi = tmp.hibyte ;
                                pSet_Highcut_Mod->start_lo = tmp.lobyte ;

                                // slope : weak signal handling modulation range
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 30       // 30 … 1000 (*0.1 %) = control over modulation range of 3% … 100%
                                 && tmp.udata16 <= 1000
                                 ) {
                                    pSet_Highcut_Mod->slope_hi = tmp.hibyte ;
                                    pSet_Highcut_Mod->slope_lo = tmp.lobyte ;

                                    // shift : weak signal handling control shift
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 50       // 50 … 1000 (*0.1 %) = maximum weak signal control shift of 5% … 100%
                                     && tmp.udata16 <= 1000
                                     ) {
                                        pSet_Highcut_Mod->shift_hi = tmp.hibyte ;
                                        pSet_Highcut_Mod->shift_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                        , TEF668x_Buf
                                                        , 11
                                                        , Handle_TEF668x_Service
                                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                        ret_state = STATUS_SUCCESS ;
                                    }   // if (tmp.udata16 >= 50 && tmp.udata16 <= 1000)
                                }   // if (tmp.udata16 >= 30 && tmp.udata16 <= 1000)
                            }   // if (tmp.udata16 >= 100 && tmp.udata16 <= 1000)
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 4)
                }
                break ;
            case TEF668X_API_FM_Set_Highcut_Level :
            case TEF668X_API_AM_Set_Highcut_Level :
                {
                    if (argc == 3) {
                        PSet_Highcut_Level  pSet_Highcut_Level ;

                        pSet_Highcut_Level = (PSet_Highcut_Level)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_Highcut_Level) {
                            pSet_Highcut_Level->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Highcut_Level)
                        else {
                            pSet_Highcut_Level->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_Highcut_Level)
                        pSet_Highcut_Level->cmd   = TEF668x_cmd_Set_Highcut_Level ;
                        pSet_Highcut_Level->index = 1 ;

                        // mode : timer selection
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 0    // 0 = off (only for evaluation)
                         && tmp.udata16 <= 3    // 1 = fast timer control
                                                // 2 = slow timer control (AM default)
                                                // 3 = dual timer control; combined fast and slow timer control (FM default)
                         ) {
                            pSet_Highcut_Level->mode_hi = tmp.hibyte ;
                            pSet_Highcut_Level->mode_lo = tmp.lobyte ;

                            // start : weak signal handling level start
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 200      // 200 … 600 [*0.1 dBuV] = control when level is below 20 dBV … 60 dBuV
                             && tmp.udata16 <= 600
                             ) {
                                pSet_Highcut_Level->start_hi = tmp.hibyte ;
                                pSet_Highcut_Level->start_lo = tmp.lobyte ;

                                // slope : weak signal handling level range
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 60       // 60 … 300 [*0.1 dB] = control over level range of 6 dB … 30 dB
                                 && tmp.udata16 <= 300
                                 ) {
                                    pSet_Highcut_Level->slope_hi = tmp.hibyte ;
                                    pSet_Highcut_Level->slope_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                    SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                    I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                    , TEF668x_Buf
                                                    , 9
                                                    , Handle_TEF668x_Service
                                                    ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                    ret_state = STATUS_SUCCESS ;
                                 }   // if (tmp.udata16 >= 60 && tmp.udata16 <= 300)
                            }   // if (tmp.udata16 >= 200 && tmp.udata16 <= 600)
                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 3)
                    }   // if (argc == 3)
                }
                break ;
            case TEF668X_API_FM_Set_Highcut_Noise :
            case TEF668X_API_AM_Set_Highcut_Noise :
                {
                    if (argc == 3) {
                        PSet_Highcut_Noise  pSet_Highcut_Noise ;

                        pSet_Highcut_Noise = (PSet_Highcut_Noise)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_Highcut_Noise) {
                            pSet_Highcut_Noise->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Highcut_Noise)
                        else {
                            pSet_Highcut_Noise->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_Highcut_Noise)
                        pSet_Highcut_Noise->cmd   = TEF668x_cmd_Set_Highcut_Noise ;
                        pSet_Highcut_Noise->index = 1 ;

                        // mode : timer selection
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 0    // 0 = off (AM:default)
                         && tmp.udata16 <= 3    // 1 = fast timer control
                                                // 2 = slow timer control (FM:default)
                                                // 3 = dual timer control; combined fast and slow timer control
                         ) {
                            pSet_Highcut_Noise->mode_hi = tmp.hibyte ;
                            pSet_Highcut_Noise->mode_lo = tmp.lobyte ;

                            if (cmd == TEF668X_API_FM_Set_Highcut_Noise) {
                                // start : FM weak signal handling noise start
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 0        // 0 … 800 [*0.1 %] = control when noise above 0… 80% of USN detector
                                 && tmp.udata16 <= 800
                                 ) {
                                    pSet_Highcut_Noise->start_hi = tmp.hibyte ;
                                    pSet_Highcut_Noise->start_lo = tmp.lobyte ;

                                    // slope : FM weak signal handling noise range
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 100      // 100 … 1000 [*0.1 %] = control over range of 10… 100% of USN detector
                                     && tmp.udata16 <= 1000
                                     ) {
                                        pSet_Highcut_Noise->slope_hi = tmp.hibyte ;
                                        pSet_Highcut_Noise->slope_lo = tmp.lobyte ;
                                    }   // if (tmp.udata16 >= 100 && tmp.udata16 <= 1000)
                                    else {
                                        break ;
                                    }
                                }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 800)
                                else {
                                    break ;
                                }
                            }
                            else {
                                // start : weak signal handling noise start
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 30   // 30 … 300 [*0.1 %] = control when noise above 3… 30% of noise detector
                                 && tmp.udata16 <= 300
                                 ) {
                                    pSet_Highcut_Noise->start_hi = tmp.hibyte ;
                                    pSet_Highcut_Noise->start_lo = tmp.lobyte ;

                                    // slope : trigger sensitivity
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 20      // 20 … 1000 [*0.1 %] = control over range of 2… 100% of noise detector
                                     && tmp.udata16 <= 1000
                                     ) {
                                        pSet_Highcut_Noise->slope_hi = tmp.hibyte ;
                                        pSet_Highcut_Noise->slope_lo = tmp.lobyte ;
                                    }   // if (tmp.udata16 >= 20 && tmp.udata16 <= 1000)
                                    else {
                                        break ;
                                    }
                                }   // if (tmp.udata16 >= 30 && tmp.udata16 <= 300)
                                else {
                                    break ;
                                }
                            }
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 9
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 3)
                    }   // if (argc == 3)
                }
                break ;
            case TEF668X_API_FM_Set_Highcut_Mph :
                {
                    if (argc == 3) {
                        PSet_Highcut_Mph    pSet_Highcut_Mph ;

                        pSet_Highcut_Mph = (PSet_Highcut_Mph)TEF668x_Buf ;

                        pSet_Highcut_Mph->module = TEF668x_Module_Radio_FM ;
                        pSet_Highcut_Mph->cmd    = TEF668x_cmd_Set_Highcut_Mph ;
                        pSet_Highcut_Mph->index  = 1 ;

                        // mode : timer selection
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 0    // 0 = off (only for evaluation)
                         && tmp.udata16 <= 3    // 1 = fast timer control
                                                // 2 = slow timer control (default)
                                                // 3 = dual timer control; combined fast and slow timer control
                         ) {
                            pSet_Highcut_Mph->mode_hi = tmp.hibyte ;
                            pSet_Highcut_Mph->mode_lo = tmp.lobyte ;

                            // start : FM weak signal handling multipath start
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 0        // 0 … 800 [*0.1 %] = control when mph above 0… 80% of WAM detector
                             && tmp.udata16 <= 800
                             ) {
                                pSet_Highcut_Mph->start_hi = tmp.hibyte ;
                                pSet_Highcut_Mph->start_lo = tmp.lobyte ;

                                // slope : FM weak signal handling multipath range
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 100  // 100 … 1000 [*0.1 %] = control over range of 10… 100% of WAM detector
                                 && tmp.udata16 <= 1000
                                 ) {
                                    pSet_Highcut_Mph->slope_hi = tmp.hibyte ;
                                    pSet_Highcut_Mph->slope_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                    SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                    I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                    , TEF668x_Buf
                                                    , 9
                                                    , Handle_TEF668x_Service
                                                    ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                    ret_state = STATUS_SUCCESS ;
                                 }   // if (tmp.udata16 >= 100 && tmp.udata16 <= 1000)
                            }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 800)
                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 3)
                    }   // if (argc == 3)
                }
                break ;
            case TEF668X_API_FM_Set_Highcut_Max :
            case TEF668X_API_AM_Set_Highcut_Max :
                {
                    if (argc == 2) {
                        PSet_Highcut_Max    pSet_Highcut_Max ;

                        pSet_Highcut_Max = (PSet_Highcut_Max)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_Highcut_Max) {
                            pSet_Highcut_Max->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Highcut_Max)
                        else {
                            pSet_Highcut_Max->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_Highcut_Max)
                        pSet_Highcut_Max->cmd   = TEF668x_cmd_Set_Highcut_Max ;
                        pSet_Highcut_Max->index = 1 ;

                        // mode : weak signal handling (dynamic control)
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off; for evaluation only
                         || tmp.udata16 == 1    // 1 = on; maximum dynamic control set by limit parameter (default)
                         ) {
                            pSet_Highcut_Max->mode_hi = tmp.hibyte ;
                            pSet_Highcut_Max->mode_lo = tmp.lobyte ;

                            // limit : Highcut attenuation limit
                            tmp.udata16 = va_arg(va, int) ;
                            if (cmd == TEF668X_API_FM_Set_Highcut_Max) {
                                // Highcut corner frequency for maximum -3 dB attenuation
                                if ((tmp.udata16 >= 1500 && tmp.udata16 <= 7000)    // 1500 … 7000 [*1 Hz] = 1.5 … 7 kHz ‘IIR’ filter (Options ‘1’ (default))
                                 || (tmp.udata16 >= 700  && tmp.udata16 <= 3000)    //  700 … 3000 [*1 Hz] = 0.7 … 3 kHz ‘de-emphasis’ mode (Options ‘2’)
                                 || (tmp.udata16 >= 2700 && tmp.udata16 <= 7000)    // 2700 … 7000 [*1 Hz] = 2.7 … 7 kHz ‘FIR’ highcut filter (Options ‘3’)
                                 ) {
                                    pSet_Highcut_Max->limit_hi = tmp.hibyte ;
                                    pSet_Highcut_Max->limit_lo = tmp.lobyte ;
                                }
                                else {
                                    break ;
                                }
                            }   // if (cmd == TEF668X_API_FM_Set_Highcut_Max)
                            else {
                                if (tmp.udata16 >= 1350
                                 && tmp.udata16 <= 7000
                                 ) {
                                    pSet_Highcut_Max->limit_hi = tmp.hibyte ;
                                    pSet_Highcut_Max->limit_lo = tmp.lobyte ;
                                }   // if (tmp.udata16 >= 1350 && tmp.udata16 <= 7000)
                                else {
                                    break ;
                                }
                            }   // if (cmd == TEF668X_API_AM_Set_Highcut_Max)

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 7
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                     }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_FM_Set_Highcut_Min :
            case TEF668X_API_AM_Set_Highcut_Min :
                {
                    if (argc == 2) {
                        PSet_Highcut_Min    pSet_Highcut_Min ;

                        pSet_Highcut_Min = (PSet_Highcut_Min)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_Highcut_Min) {
                            pSet_Highcut_Min->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Highcut_Min)
                        else {
                            pSet_Highcut_Min->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_Highcut_Min)
                        pSet_Highcut_Min->cmd   = TEF668x_cmd_Set_Highcut_Min ;
                        pSet_Highcut_Min->index = 1 ;

                        // mode : strong signal handling
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off; high audio frequency bandwidth is not limited (FM default)
                         || tmp.udata16 == 1    // 1 =  on; minimum control limit set by limit parameter (AM default)
                         ) {
                            pSet_Highcut_Min->mode_hi = tmp.hibyte ;
                            pSet_Highcut_Min->mode_lo = tmp.lobyte ;

                            // limit : Highcut fixed attenuation limit
                            tmp.udata16 = va_arg(va, int) ;
                            if (cmd == TEF668X_API_FM_Set_Highcut_Min) {
                                // Highcut corner frequency for maximum -3 dB attenuation
                                if ((tmp.udata16 >= 2700 && tmp.udata16 <= 15000)   // 2700 … 15000 [*1 Hz] = 2.7 … 15 kHz ‘IIR’ filter (Options ‘1’ (default))
                                                                                    // 2700 … 15000 [*1 Hz] = 2.7 … 15 kHz ‘FIR’ highcut filter (Options ‘3’)
                                 || (tmp.udata16 >= 1500 && tmp.udata16 <= 3183)    // 1500 … 3183 [*1 Hz] = 1.5 … 3.18 kHz ‘de-emphasis’ (Options ‘2’)
                                                                                    // 2122 = 75 us de-emphasis / 3183 = 50 us de-emphasis
                                 ) {
                                    pSet_Highcut_Min->limit_hi = tmp.hibyte ;
                                    pSet_Highcut_Min->limit_lo = tmp.lobyte ;
                                }
                                else {
                                    break ;
                                }
                            }   // if (cmd == TEF668X_API_FM_Set_Highcut_Min)
                            else {
                                if (tmp.udata16 >= 2700     // 2700 … 10000 [*1 Hz] = 2.7 … 10 kHz  -3 dB att. for min. Highcut
                                 && tmp.udata16 <= 10000
                                 ) {
                                    pSet_Highcut_Min->limit_hi = tmp.hibyte ;
                                    pSet_Highcut_Min->limit_lo = tmp.lobyte ;
                                }   // if (tmp.udata16 >= 1350 && tmp.udata16 <= 7000)
                                else {
                                    break ;
                                }
                            }   // if (cmd == TEF668X_API_AM_Set_Highcut_Min)
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 7
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_FM_Set_Lowcut_Max :
            case TEF668X_API_AM_Set_Lowcut_Max :
                {
                    if (argc == 2) {
                        PSet_Lowcut_Max     pSet_Lowcut_Max ;

                        pSet_Lowcut_Max = (PSet_Lowcut_Max)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_Lowcut_Max) {
                            pSet_Lowcut_Max->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Lowcut_Max)
                        else {
                            pSet_Lowcut_Max->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_Lowcut_Max)
                        pSet_Lowcut_Max->cmd   = TEF668x_cmd_Set_Lowcut_Max ;
                        pSet_Lowcut_Max->index = 1 ;

                        // mode : weak signal handling (dynamic control)
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off
                         || tmp.udata16 == 1    // 1 = on; maximum dynamic control defined by limit parameter (default)
                         ) {
                            pSet_Lowcut_Max->mode_hi = tmp.hibyte ;
                            pSet_Lowcut_Max->mode_lo = tmp.lobyte ;

                            // limit : Lowcut dynamic attenuation limit
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 30   // 30 … 500 [Hz] = 30 … 500 Hz  -3 dB attenuation for maximum Lowcut
                             && tmp.udata16 <= 500
                             ) {
                                pSet_Lowcut_Max->limit_hi = tmp.hibyte ;
                                pSet_Lowcut_Max->limit_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                , TEF668x_Buf
                                                , 7
                                                , Handle_TEF668x_Service
                                                ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                ret_state = STATUS_SUCCESS ;
                            }   // if (tmp.udata16 >= 30 || tmp.udata16 <= 500)
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_FM_Set_Lowcut_Min :
            case TEF668X_API_AM_Set_Lowcut_Min :
                {
                    if (argc == 2) {
                        PSet_Lowcut_Min     pSet_Lowcut_Min ;

                        pSet_Lowcut_Min = (PSet_Lowcut_Min)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_Lowcut_Min) {
                            pSet_Lowcut_Min->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Lowcut_Min)
                        else {
                            pSet_Lowcut_Min->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_Lowcut_Min)
                        pSet_Lowcut_Min->cmd   = TEF668x_cmd_Set_Lowcut_Min ;
                        pSet_Lowcut_Min->index = 1 ;

                        // mode : strong signal handling
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off; low audio frequency bandwidth is not limited (FM default)
                         || tmp.udata16 == 1    // 1 = on; minimum control limit set by limit parameter
                         ) {
                            pSet_Lowcut_Min->mode_hi = tmp.hibyte ;
                            pSet_Lowcut_Min->mode_lo = tmp.lobyte ;

                            // limit : Lowcut fixed attenuation limit
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 == 10   // 10 … 200 [Hz] = 10 … 200 Hz  Lowcut minimum -3 dB attenuation
                             || tmp.udata16 == 200
                             ) {
                                pSet_Lowcut_Min->limit_hi = tmp.hibyte ;
                                pSet_Lowcut_Min->limit_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                , TEF668x_Buf
                                                , 7
                                                , Handle_TEF668x_Service
                                                ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                ret_state = STATUS_SUCCESS ;
                            }   // if (tmp.udata16 >= 10 && tmp.udata16 <= 200)
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_FM_Set_Highcut_Options :
                {
                    if (argc == 1) {
                        PSet_Highcut_Options    pSet_Highcut_Options ;

                        pSet_Highcut_Options = (PSet_Highcut_Options)TEF668x_Buf ;

                        pSet_Highcut_Options->module = TEF668x_Module_Radio_FM ;
                        pSet_Highcut_Options->cmd   = TEF668x_cmd_Set_Highcut_Options ;
                        pSet_Highcut_Options->index = 1 ;

                        // mode : strong signal handling
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 1    // 1 = IIR; ‘analog’ first order low-pass filter with controlled frequency (default)
                         || tmp.udata16 == 2    // 2 = de-emphasis; controlled frequency of the 50 / 75 us de-emphasis filter
                         || tmp.udata16 == 3    // 3 = FIR; ‘digital’ high order low-pass filter with controlled frequency
                         ) {
                            pSet_Highcut_Options->mode_hi = tmp.hibyte ;
                            pSet_Highcut_Options->mode_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 5
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.udata16 == 1 || tmp.udata16 == 2 || tmp.udata16 == 3)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Set_Stereo_Time :
                {
                    if (argc == 4) {
                        PSet_Stereo_Time    pSet_Stereo_Time ;

                        pSet_Stereo_Time = (PSet_Stereo_Time)TEF668x_Buf ;

                        pSet_Stereo_Time->module = TEF668x_Module_Radio_FM ;
                        pSet_Stereo_Time->cmd    = TEF668x_cmd_Set_Stereo_Time ;
                        pSet_Stereo_Time->index  = 1 ;

                        // slow_attack : slow attack time of weak signal handling
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 60   // 60 … 2000 (ms) = 60 ms … 2 s slow attack time
                         && tmp.udata16 <= 2000
                         ) {
                            pSet_Stereo_Time->slow_attack_hi = tmp.hibyte ;
                            pSet_Stereo_Time->slow_attack_lo = tmp.lobyte ;

                            // slow_decay : slow decay time of weak signal handling
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 120      // 120 … 20000 (ms) = 120 ms … 20 s slow decay time
                             && tmp.udata16 <= 20000
                             ) {
                                pSet_Stereo_Time->slow_decay_hi = tmp.hibyte ;
                                pSet_Stereo_Time->slow_decay_lo = tmp.lobyte ;

                                // fast_attack : fast attack time of weak signal handling
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 10   // 10 … 1200 (*0.1 ms) = 1 ms … 120 ms fast attack time
                                 && tmp.udata16 <= 1200
                                 ) {
                                    pSet_Stereo_Time->fast_attack_hi = tmp.hibyte ;
                                    pSet_Stereo_Time->fast_attack_lo = tmp.lobyte ;

                                    // fast_decay : fast decay time of weak signal handling
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 20       // 20 … 5000 ( *0.1 ms) = 2 ms … 500 ms fast decay time
                                     && tmp.udata16 <= 5000
                                     ) {
                                        pSet_Stereo_Time->fast_decay_hi = tmp.hibyte ;
                                        pSet_Stereo_Time->fast_decay_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                        , TEF668x_Buf
                                                        , 11
                                                        , Handle_TEF668x_Service
                                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                        ret_state = STATUS_SUCCESS ;
                                    }   // if (tmp.udata16 >= 20 && tmp.udata16 <= 5000 )
                                }   // if (tmp.udata16 >= 10 && tmp.udata16 <= 1200 )
                            }   // if (tmp.udata16 >= 120 && tmp.udata16 <= 20000 )
                        }   // if (tmp.udata16 >= 60 && tmp.udata16 <= 2000 )
                    }   // if (argc == 4)
                }
                break ;
            case TEF668X_API_FM_Set_Stereo_Mod :
                {
                    if (argc == 4) {
                        PSet_Stereo_Mod     pSet_Stereo_Mod ;


                        pSet_Stereo_Mod = (PSet_Stereo_Mod)TEF668x_Buf ;

                        pSet_Stereo_Mod->module = TEF668x_Module_Radio_FM ;
                        pSet_Stereo_Mod->cmd    = TEF668x_cmd_Set_Stereo_Mod ;
                        pSet_Stereo_Mod->index  = 1 ;

                        // mode : modulation dependent weak signal handling
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off (default)
                         || tmp.udata16 == 1    // 1 = on (independent modulation timer)
                         ) {
                            pSet_Stereo_Mod->mode_hi = tmp.hibyte ;
                            pSet_Stereo_Mod->mode_lo = tmp.lobyte ;

                            // start : weak signal handling modulation start
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 10       // 100 … 1000 [*0.1 %] = control when modulation falls below 10% … 100%
                             && tmp.udata16 <= 1000
                             ) {
                                pSet_Stereo_Mod->start_hi = tmp.hibyte ;
                                pSet_Stereo_Mod->start_lo = tmp.lobyte ;

                                // slope : weak signal handling modulation range
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 30   // 30 … 1000 (*0.1 %) = control over modulation range of 3% … 100%
                                 && tmp.udata16 <= 1000
                                 ) {
                                    pSet_Stereo_Mod->slope_hi = tmp.hibyte ;
                                    pSet_Stereo_Mod->slope_lo = tmp.lobyte ;

                                    // shift : weak signal handling control shift
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 50       // 50 … 1000 (*0.1 %) = maximum weak signal control shift of 5% … 100%
                                     && tmp.udata16 <= 1000
                                     ) {
                                        pSet_Stereo_Mod->shift_hi = tmp.hibyte ;
                                        pSet_Stereo_Mod->shift_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                        , TEF668x_Buf
                                                        , 11
                                                        , Handle_TEF668x_Service
                                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                        ret_state = STATUS_SUCCESS ;
                                    }   // if (tmp.udata16 >= 50 && tmp.udata16 <= 1000)
                                }   // if (tmp.udata16 >= 30 && tmp.udata16 <= 1000)
                            }   // if (tmp.udata16 >= 10 && tmp.udata16 <= 1000)
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 4)
                }
                break ;
            case TEF668X_API_FM_Set_Stereo_Level :
                {
                    if (argc == 3) {
                        PSet_Stereo_Level   pSet_Stereo_Level ;

                        pSet_Stereo_Level = (PSet_Stereo_Level)TEF668x_Buf ;

                        pSet_Stereo_Level->module   = TEF668x_Module_Radio_FM ;
                        pSet_Stereo_Level->cmd      = TEF668x_cmd_Set_Stereo_Level ;
                        pSet_Stereo_Level->index    = 1 ;

                        // mode : timer selection
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 0    // 0 = off (only for evaluation)
                         && tmp.udata16 <= 3    // 1 = fast timer control
                                                // 2 = slow timer control
                                                // 3 = dual timer control; combined fast and slow timer control (default)
                         ) {
                            pSet_Stereo_Level->mode_hi = tmp.hibyte ;
                            pSet_Stereo_Level->mode_lo = tmp.lobyte ;

                            // start : weak signal handling level start
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 300      // 300 … 600 [*0.1 dBuV] = control when level below 30 dBV … 60 dBuV
                             && tmp.udata16 <= 600
                             ) {
                                pSet_Stereo_Level->start_hi = tmp.hibyte ;
                                pSet_Stereo_Level->start_lo = tmp.lobyte ;

                                // slope : weak signal handling level range
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 60   // 60 … 300 [*0.1 dB] = control over level range of 6 dB … 30 dB
                                 && tmp.udata16 <= 300
                                 ) {
                                    pSet_Stereo_Level->slope_hi = tmp.hibyte ;
                                    pSet_Stereo_Level->slope_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                    SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                    I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                    , TEF668x_Buf
                                                    , 9
                                                    , Handle_TEF668x_Service
                                                    ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                    ret_state = STATUS_SUCCESS ;
                                }   // if (tmp.udata16 >= 60 && tmp.udata16 <= 300)
                            }   // if (tmp.udata16 >= 300 && tmp.udata16 <= 600)
                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 3)
                    }   // if (argc == 3)
                }
                break ;
            case TEF668X_API_FM_Set_Stereo_Noise :
                {
                    if (argc == 3) {
                        PSet_Stereo_Noise   pSet_Stereo_Noise ;

                        pSet_Stereo_Noise = (PSet_Stereo_Noise)TEF668x_Buf ;

                        pSet_Stereo_Noise->module   = TEF668x_Module_Radio_FM ;
                        pSet_Stereo_Noise->cmd      = TEF668x_cmd_Set_Stereo_Noise ;
                        pSet_Stereo_Noise->index    = 1 ;

                        // mode : timer selection
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 0    // 0 = off (only for evaluation)
                         && tmp.udata16 <= 3    // 1 = fast timer control
                                                // 2 = slow timer control
                                                // 3 = dual timer control; combined fast and slow timer control (default)
                         ) {
                            pSet_Stereo_Noise->mode_hi = tmp.hibyte ;
                            pSet_Stereo_Noise->mode_lo = tmp.lobyte ;

                            // start : FM weak signal handling noise start
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 0    // 0 … 800 [*0.1 %] = control when noise above 0… 80% of USN detector
                             && tmp.udata16 <= 800
                             ) {
                                pSet_Stereo_Noise->start_hi = tmp.hibyte ;
                                pSet_Stereo_Noise->start_lo = tmp.lobyte ;

                                // slope : FM weak signal handling noise range
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 100   // 100 … 1000 [*0.1 %] = control over range of 10… 100% of USN detector
                                 && tmp.udata16 <= 1000
                                 ) {
                                    pSet_Stereo_Noise->slope_hi = tmp.hibyte ;
                                    pSet_Stereo_Noise->slope_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                    SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                    I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                    , TEF668x_Buf
                                                    , 9
                                                    , Handle_TEF668x_Service
                                                    ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                    ret_state = STATUS_SUCCESS ;
                                }   // if (tmp.udata16 >= 100 && tmp.udata16 <= 1000)
                            }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 800)
                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 3)
                    }   // if (argc == 3)
                }
                break ;
            case TEF668X_API_FM_Set_Stereo_Mph :
                {
                    if (argc == 3) {
                        PSet_Stereo_Mph     pSet_Stereo_Mph ;

                        pSet_Stereo_Mph = (PSet_Stereo_Mph)TEF668x_Buf ;

                        pSet_Stereo_Mph->module   = TEF668x_Module_Radio_FM ;
                        pSet_Stereo_Mph->cmd      = TEF668x_cmd_Set_Stereo_Mph ;
                        pSet_Stereo_Mph->index    = 1 ;

                        // mode : timer selection
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 0    // 0 = off (only for evaluation)
                         && tmp.udata16 <= 3    // 1 = fast timer control
                                                // 2 = slow timer control
                                                // 3 = dual timer control; combined fast and slow timer control (default)
                         ) {
                            pSet_Stereo_Mph->mode_hi = tmp.hibyte ;
                            pSet_Stereo_Mph->mode_lo = tmp.lobyte ;

                            // start : FM weak signal handling multipath start
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 0    // 0 … 800 [*0.1 %] = control when mph above 0… 80% of WAM detector
                             && tmp.udata16 <= 800
                             ) {
                                pSet_Stereo_Mph->start_hi = tmp.hibyte ;
                                pSet_Stereo_Mph->start_lo = tmp.lobyte ;

                                // slope : FM weak signal handling multipath range
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 100   // 100 … 1000 [*0.1 %] = control over range of 10… 100% of WAM detector
                                 && tmp.udata16 <= 1000
                                 ) {
                                    pSet_Stereo_Mph->slope_hi = tmp.hibyte ;
                                    pSet_Stereo_Mph->slope_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                    SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                    I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                    , TEF668x_Buf
                                                    , 9
                                                    , Handle_TEF668x_Service
                                                    ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                    ret_state = STATUS_SUCCESS ;
                                }   // if (tmp.udata16 >= 100 && tmp.udata16 <= 1000)
                            }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 800)
                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 3)
                    }   // if (argc == 3)
                }
                break ;
            case TEF668X_API_FM_Set_Stereo_Max :
                {
                    if (argc == 1) {
                        PSet_Stereo_Max     pSet_Stereo_Max ;

                        pSet_Stereo_Max = (PSet_Stereo_Max)TEF668x_Buf ;

                        pSet_Stereo_Max->module = TEF668x_Module_Radio_FM ;
                        pSet_Stereo_Max->cmd   = TEF668x_cmd_Set_Stereo_Max ;
                        pSet_Stereo_Max->index = 1 ;

                        // mode : weak signal handling (dynamic control)
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off (for evaluation only)
                         || tmp.udata16 == 1    // 1 = on; maximum dynamic control is 0 dB channel sep, i.e. mono (default)
                         ) {
                            pSet_Stereo_Max->mode_hi = tmp.hibyte ;
                            pSet_Stereo_Max->mode_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 5
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Set_Stereo_Min :
                {
                    if (argc == 2) {
                        PSet_Stereo_Min     pSet_Stereo_Min ;

                        pSet_Stereo_Min = (PSet_Stereo_Min)TEF668x_Buf ;

                        pSet_Stereo_Min->module = TEF668x_Module_Radio_FM ;
                        pSet_Stereo_Min->cmd    = TEF668x_cmd_Set_Stereo_Min ;
                        pSet_Stereo_Min->index  = 1 ;

                        // mode : strong signal handling
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off; channel separation is not limited (default)
                         || tmp.udata16 == 1    // 1 = on; minimum control limit set by limit parameter
                         || tmp.udata16 == 2    // 2 = forced mono
                         ) {
                            pSet_Stereo_Min->mode_hi = tmp.hibyte ;
                            pSet_Stereo_Min->mode_lo = tmp.lobyte ;

                            // limit : Stereo fixed attenuation limit
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 == 60   // 60 …  400 [0.1* dB] = 6 … 40 dB  Stereo minimum channel separation
                             || tmp.udata16 == 400
                             ) {
                                pSet_Stereo_Min->limit_hi = tmp.hibyte ;
                                pSet_Stereo_Min->limit_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                , TEF668x_Buf
                                                , 7
                                                , Handle_TEF668x_Service
                                                ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                ret_state = STATUS_SUCCESS ;
                            }   // if (tmp.udata16 >= 60 && tmp.udata16 <= 400)
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1 || tmp.udata16 == 2)
                    }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_FM_Set_StHiBlend_Time :
                {
                    if (argc == 4) {
                        PSet_StHiBlend_Time pSet_StHiBlend_Time ;

                        pSet_StHiBlend_Time = (PSet_StHiBlend_Time)TEF668x_Buf ;

                        pSet_StHiBlend_Time->module = TEF668x_Module_Radio_FM ;
                        pSet_StHiBlend_Time->cmd    = TEF668x_cmd_Set_StHiBlend_Time ;
                        pSet_StHiBlend_Time->index  = 1 ;

                        // slow_attack : slow attack time of weak signal handling
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 60   // 60 … 2000 (ms) = 60 ms … 2 s slow attack time
                         && tmp.udata16 <= 2000
                         ) {
                            pSet_StHiBlend_Time->slow_attack_hi = tmp.hibyte ;
                            pSet_StHiBlend_Time->slow_attack_lo = tmp.lobyte ;

                            // slow_decay : slow decay time of weak signal handling
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 120  // 120 … 12500 (ms) = 120 ms … 12.5 s slow decay time
                             && tmp.udata16 <= 12500
                             ) {
                                pSet_StHiBlend_Time->slow_decay_hi = tmp.hibyte ;
                                pSet_StHiBlend_Time->slow_decay_lo = tmp.lobyte ;

                                // fast_attack : fast attack time of weak signal handling
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 10   // 10 … 1200 (*0.1 ms) = 1 ms … 120 ms fast attack time
                                 && tmp.udata16 <= 1200
                                 ) {
                                    pSet_StHiBlend_Time->fast_attack_hi = tmp.hibyte ;
                                    pSet_StHiBlend_Time->fast_attack_lo = tmp.lobyte ;

                                    // fast_decay : fast decay time of weak signal handling
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 20   // 20 … 5000 ( *0.1 ms) = 2 ms … 500 ms fast decay time
                                     && tmp.udata16 <= 5000
                                     ) {
                                        pSet_StHiBlend_Time->fast_decay_hi = tmp.hibyte ;
                                        pSet_StHiBlend_Time->fast_decay_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                        , TEF668x_Buf
                                                        , 11
                                                        , Handle_TEF668x_Service
                                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                        ret_state = STATUS_SUCCESS ;
                                    }   // if (tmp.udata16 >= 20 && tmp.udata16 <= 5000)
                                }   // if (tmp.udata16 >= 10 && tmp.udata16 <= 1200)
                            }   // if (tmp.udata16 >= 120 && tmp.udata16 <= 12500)
                        }   // if (tmp.udata16 >= 60 && tmp.udata16 <= 2000)
                    }   // if (argc == 4)
                }
                break ;
            case TEF668X_API_FM_Set_StHiBlend_Mod :
                {
                    if (argc == 4) {
                        PSet_StHiBlend_Mod  pSet_StHiBlend_Mod ;

                        pSet_StHiBlend_Mod = (PSet_StHiBlend_Mod)TEF668x_Buf ;

                        pSet_StHiBlend_Mod->module = TEF668x_Module_Radio_FM ;
                        pSet_StHiBlend_Mod->cmd    = TEF668x_cmd_Set_StHiBlend_Mod ;
                        pSet_StHiBlend_Mod->index  = 1 ;

                        // mode : modulation dependent weak signal handling
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off (default)
                         || tmp.udata16 == 1    // 1 = on (independent modulation timer)
                         ) {
                            pSet_StHiBlend_Mod->mode_hi = tmp.hibyte ;
                            pSet_StHiBlend_Mod->mode_lo = tmp.lobyte ;

                            // start : weak signal handling modulation start
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 10       // 100 … 1000 [*0.1 %] = control when modulation falls below 10% … 100%
                             && tmp.udata16 <= 1000
                             ) {
                                pSet_StHiBlend_Mod->start_hi = tmp.hibyte ;
                                pSet_StHiBlend_Mod->start_lo = tmp.lobyte ;

                                // slope : weak signal handling modulation range
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 30   // 30 … 1000 (*0.1 %) = control over modulation range of 3% … 100%
                                 && tmp.udata16 <= 1000
                                 ) {
                                    pSet_StHiBlend_Mod->slope_hi = tmp.hibyte ;
                                    pSet_StHiBlend_Mod->slope_lo = tmp.lobyte ;

                                    // shift : weak signal handling control shift
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 50       // 50 … 1000 (*0.1 %) = maximum weak signal control shift of 5% … 100%
                                     && tmp.udata16 <= 1000
                                     ) {
                                        pSet_StHiBlend_Mod->shift_hi = tmp.hibyte ;
                                        pSet_StHiBlend_Mod->shift_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                        , TEF668x_Buf
                                                        , 11
                                                        , Handle_TEF668x_Service
                                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                        ret_state = STATUS_SUCCESS ;
                                    }   // if (tmp.udata16 >= 50 && tmp.udata16 <= 1000)
                                }   // if (tmp.udata16 >= 30 && tmp.udata16 <= 1000)
                            }   // if (tmp.udata16 >= 10 && tmp.udata16 <= 1000)
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 4)
                }
                break ;
            case TEF668X_API_FM_Set_StHiBlend_Level :
                {
                    if (argc == 3) {
                        PSet_StHiBlend_Level    pSet_StHiBlend_Level ;

                        pSet_StHiBlend_Level = (PSet_StHiBlend_Level)TEF668x_Buf ;

                        pSet_StHiBlend_Level->module   = TEF668x_Module_Radio_FM ;
                        pSet_StHiBlend_Level->cmd      = TEF668x_cmd_Set_StHiBlend_Level ;
                        pSet_StHiBlend_Level->index    = 1 ;

                        // mode : timer selection
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 0    // 0 = off (only for evaluation)
                         && tmp.udata16 <= 3    // 1 = fast timer control
                                                // 2 = slow timer control
                                                // 3 = dual timer control; combined fast and slow timer control (default)
                         ) {
                            pSet_StHiBlend_Level->mode_hi = tmp.hibyte ;
                            pSet_StHiBlend_Level->mode_lo = tmp.lobyte ;

                            // start : weak signal handling level start
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 300  // 300 … 660 [*0.1 dBuV] = control when level below 30 dBV … 66 dBuV
                             && tmp.udata16 <= 660
                             ) {
                                pSet_StHiBlend_Level->start_hi = tmp.hibyte ;
                                pSet_StHiBlend_Level->start_lo = tmp.lobyte ;

                                // slope : weak signal handling level range
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 60   // 60 … 300 [*0.1 dB] = control over level range of 6 dB … 30 dB
                                 && tmp.udata16 <= 300
                                 ) {
                                    pSet_StHiBlend_Level->slope_hi = tmp.hibyte ;
                                    pSet_StHiBlend_Level->slope_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                    SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                    I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                    , TEF668x_Buf
                                                    , 9
                                                    , Handle_TEF668x_Service
                                                    ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                    ret_state = STATUS_SUCCESS ;
                                }   // if (tmp.udata16 >= 60 && tmp.udata16 <= 300)
                            }   // if (tmp.udata16 >= 300 && tmp.udata16 <= 660)
                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 3)
                    }   // if (argc == 3)
                }
                break ;
            case TEF668X_API_FM_Set_StHiBlend_Noise :
                {
                    if (argc == 3) {
                        PSet_StHiBlend_Noise    pSet_StHiBlend_Noise ;

                        pSet_StHiBlend_Noise = (PSet_StHiBlend_Noise)TEF668x_Buf ;

                        pSet_StHiBlend_Noise->module   = TEF668x_Module_Radio_FM ;
                        pSet_StHiBlend_Noise->cmd      = TEF668x_cmd_Set_StHiBlend_Noise ;
                        pSet_StHiBlend_Noise->index    = 1 ;

                        // mode : timer selection
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 0    // 0 = off (only for evaluation)
                         && tmp.udata16 <= 3    // 1 = fast timer control
                                                // 2 = slow timer control
                                                // 3 = dual timer control; combined fast and slow timer control (default)
                         ) {
                            pSet_StHiBlend_Noise->mode_hi = tmp.hibyte ;
                            pSet_StHiBlend_Noise->mode_lo = tmp.lobyte ;

                            // start : FM weak signal handling noise start
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 0    // 0 … 800 [*0.1 %] = control when noise above 0… 80% of USN detector
                             && tmp.udata16 <= 800
                             ) {
                                pSet_StHiBlend_Noise->start_hi = tmp.hibyte ;
                                pSet_StHiBlend_Noise->start_lo = tmp.lobyte ;

                                // slope : FM weak signal handling noise range
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 100  // 100 … 1000 [*0.1 %] = control over range of 10… 100% of USN detector
                                 && tmp.udata16 <= 1000
                                 ) {
                                    pSet_StHiBlend_Noise->slope_hi = tmp.hibyte ;
                                    pSet_StHiBlend_Noise->slope_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                    SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                    I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                    , TEF668x_Buf
                                                    , 9
                                                    , Handle_TEF668x_Service
                                                    ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                    ret_state = STATUS_SUCCESS ;
                                }   // if (tmp.udata16 >= 100 && tmp.udata16 <= 1000)
                            }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 800)
                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 3)
                    }   // if (argc == 3)
                }
                break ;
            case TEF668X_API_FM_Set_StHiBlend_Mph :
                {
                    if (argc == 3) {
                        PSet_StHiBlend_Mph  pSet_StHiBlend_Mph ;

                        pSet_StHiBlend_Mph = (PSet_StHiBlend_Mph)TEF668x_Buf ;

                        pSet_StHiBlend_Mph->module   = TEF668x_Module_Radio_FM ;
                        pSet_StHiBlend_Mph->cmd      = TEF668x_cmd_Set_StHiBlend_Mph ;
                        pSet_StHiBlend_Mph->index    = 1 ;

                        // mode : timer selection
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 0    // 0 = off (only for evaluation)
                         && tmp.udata16 <= 3    // 1 = fast timer control
                                                // 2 = slow timer control
                                                // 3 = dual timer control; combined fast and slow timer control (default)
                         ) {
                            pSet_StHiBlend_Mph->mode_hi = tmp.hibyte ;
                            pSet_StHiBlend_Mph->mode_lo = tmp.lobyte ;

                            // start : FM weak signal handling multipath start
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 0    // 0 … 800 [*0.1 %] = control when mph above 0… 80% of WAM detector
                             && tmp.udata16 <= 800
                             ) {
                                pSet_StHiBlend_Mph->start_hi = tmp.hibyte ;
                                pSet_StHiBlend_Mph->start_lo = tmp.lobyte ;

                                // slope : FM weak signal handling multipath range
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 100  // 100 … 1000 [*0.1 %] = control over range of 10… 100% of WAM detector
                                 && tmp.udata16 <= 1000
                                 ) {
                                    pSet_StHiBlend_Mph->slope_hi = tmp.hibyte ;
                                    pSet_StHiBlend_Mph->slope_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                    SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                    I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                    , TEF668x_Buf
                                                    , 9
                                                    , Handle_TEF668x_Service
                                                    ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                    ret_state = STATUS_SUCCESS ;
                                }   // if (tmp.udata16 >= 60 && tmp.udata16 <= 300)
                            }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 800)
                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 3)
                    }   // if (argc == 3)
                }
                break ;
            case TEF668X_API_FM_Set_StHiBlend_Max :
                {
                    if (argc == 2) {
                        PSet_StHiBlend_Max  pSet_StHiBlend_Max ;

                        pSet_StHiBlend_Max = (PSet_StHiBlend_Max)TEF668x_Buf ;

                        pSet_StHiBlend_Max->module = TEF668x_Module_Radio_FM ;
                        pSet_StHiBlend_Max->cmd   = TEF668x_cmd_Set_StHiBlend_Max ;
                        pSet_StHiBlend_Max->index = 1 ;

                        // mode : weak signal handling (dynamic control)
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off;
                         || tmp.udata16 == 1    // 1 = on; maximum dynamic control defined by limit parameter (default)
                         ) {
                            pSet_StHiBlend_Max->mode_hi = tmp.hibyte ;
                            pSet_StHiBlend_Max->mode_lo = tmp.lobyte ;

                            // limit : StHiBlend dynamic attenuation limit
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 2700     // 2700 …  7000 [Hz] = 2.7 kHz … 7 kHz StHiBlend max. reduction
                             && tmp.udata16 <= 7000     // of channel separation bandwidth
                             ) {
                                pSet_StHiBlend_Max->limit_hi = tmp.hibyte ;
                                pSet_StHiBlend_Max->limit_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                , TEF668x_Buf
                                                , 7
                                                , Handle_TEF668x_Service
                                                ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                ret_state = STATUS_SUCCESS ;
                            }   // if (tmp.udata16 >= 2700 && tmp.udata16 <= 7000)
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_FM_Set_StHiBlend_Min :
                {
                    if (argc == 2) {
                        PSet_StHiBlend_Min  pSet_StHiBlend_Min ;

                        pSet_StHiBlend_Min = (PSet_StHiBlend_Min)TEF668x_Buf ;

                        pSet_StHiBlend_Min->module = TEF668x_Module_Radio_FM ;
                        pSet_StHiBlend_Min->cmd   = TEF668x_cmd_Set_StHiBlend_Min ;
                        pSet_StHiBlend_Min->index = 1 ;

                        // mode : strong signal handling
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off; channel separation bw is not limited (default)
                         || tmp.udata16 == 1    // 1 = on; minimum control limit set by limit parameter
                         ) {
                            pSet_StHiBlend_Min->mode_hi = tmp.hibyte ;
                            pSet_StHiBlend_Min->mode_lo = tmp.lobyte ;

                            // limit : StHiBlend fixed attenuation limit
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 3000     // 3000 …  15000 [Hz] = 3 kHz … 15 kHz StHiBlend min. reduction of
                             && tmp.udata16 <= 15000    // channel separation bandwidth
                             ) {
                                pSet_StHiBlend_Min->limit_hi = tmp.hibyte ;
                                pSet_StHiBlend_Min->limit_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                , TEF668x_Buf
                                                , 7
                                                , Handle_TEF668x_Service
                                                ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                ret_state = STATUS_SUCCESS ;
                            }   // if (tmp.udata16 >= 3000 && tmp.udata16 <= 15000)
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_FM_Set_Scaler :
            case TEF668X_API_AM_Set_Scaler :
                {
                    if (argc == 1) {
                        PSet_Scaler pSet_Scaler ;

                        pSet_Scaler = (PSet_Scaler)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_Scaler) {
                            pSet_Scaler->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Scaler)
                        else {
                            pSet_Scaler->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_Scaler)
                        pSet_Scaler->cmd    = TEF668x_cmd_Set_Scaler ;
                        pSet_Scaler->index  = 1 ;

                        // FM multiple path suppression
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.data16 >= -120  // -120 … +60 [*0.1 dB] = -12 … +6 dB  analog radio signal gain
                         && tmp.data16 <= 60
                         ) {
                            pSet_Scaler->gain_hi = tmp.hibyte ;
                            pSet_Scaler->gain_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 5
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.data16 >= -120 && tmp.data16 <= 60)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Set_RDS :
                {
                    if (argc == 3) {
                        PSet_RDS    pSet_RDS ;

                        pSet_RDS = (PSet_RDS)TEF668x_Buf ;

                        pSet_RDS->module   = TEF668x_Module_Radio_FM ;
                        pSet_RDS->cmd      = TEF668x_cmd_Set_RDS ;
                        pSet_RDS->index    = 1 ;

                        // mode : RDS operation control
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 0    // 0 = off (RDS function disabled)
                         && tmp.udata16 <= 3    // 1 = decoder mode (default); output of RDS group data (block A, B, C, D)
                                                //     from Get_RDS_Status/Get_RDS_Data; FM cmd = 130/131
                                                // 2 = demodulator mode; output of raw demodulator data from
                                                //     Get_RDS_Status/Get_RDS_Data; FM cmd = 130/131
                                                // 3 = ‘FULL SEARCH’ decoder mode; extended sensitivity RDS reception
                                                //     (for TEF6687A and TEF6689A only)
                                                //     standard mode = 1 compatible output of RDS group data (block A, B, C, D)
                                                //     from Get_RDS_Status/Get_RDS_Data; FM cmd = 130/131
                         ) {
                            pSet_RDS->mode_hi = tmp.hibyte ;
                            pSet_RDS->mode_lo = tmp.lobyte ;

                            // restart : RDS decoder restart
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 0    // 0 = no control
                             && tmp.udata16 <= 3    // 1 = manual restart; start looking for new RDS signal immediately
                                                    // 2 = automatic restart after tuning (default); start looking for new RDS signal
                                                    //     after Preset, Search, Jump or Check tuning action (see FM cmd = 1)
                                                    // 3 = flush; empty RDS output buffer (but maintain RDS synchronization)
                             ) {
                                pSet_RDS->restart_hi = tmp.hibyte ;
                                pSet_RDS->restart_lo = tmp.lobyte ;

                                // interface : RDS pin signal functionality
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 == 0    // 0 = no pin interface (default)
                                 || tmp.udata16 == 2    // 2 = data-available status output; active low  (GPIO feature ‘DAVN’)
                                 || tmp.udata16 == 4    // 4 = legacy 2-wire demodulator data and clock output (‘RDDA’ and ‘RDCL’)
                                 ) {
                                    pSet_RDS->interface_hi = tmp.hibyte ;
                                    pSet_RDS->interface_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                    SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                    I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                    , TEF668x_Buf
                                                    , 9
                                                    , Handle_TEF668x_Service
                                                    ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                    ret_state = STATUS_SUCCESS ;
                                }   // if (tmp.udata16 == 0 || tmp.udata16 == 2 || tmp.udata16 == 4)
                            }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 3)
                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 3)
                    }   // if (argc == 3)
                }
                break ;
            case TEF668X_API_FM_Set_QualityStatus :
            case TEF668X_API_AM_Set_QualityStatus :
                {
                    if (argc == 2) {
                        PSet_QualityStatus  pSet_QualityStatus ;

                        pSet_QualityStatus = (PSet_QualityStatus)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_QualityStatus) {
                            pSet_QualityStatus->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_QualityStatus)
                        else {
                            pSet_QualityStatus->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_QualityStatus)

                        pSet_QualityStatus->cmd   = TEF668x_cmd_Set_QualityStatus ;
                        pSet_QualityStatus->index = 1 ;

                        // mode : quality status flag after tuning ready
                        tmp.udata16 = va_arg(va, int) ;
                        if (cmd == TEF668X_API_FM_Set_QualityStatus) {
                            uint16_t            tmp1 ;

                            //        0 = no flag set after tuning (default)
                            //        [ 8:0 ] : 10 … 320 (* 0.1 ms) = set flag at 1 … 32 ms after tuning ready
                            //        [ 15 ] : 1 = set flag when FM AF_Update quality result is available
                            tmp1 = tmp.udata16 & ~BIT_15 ;
                            if (tmp.udata16 & BIT_15) {
                                if (tmp1 < 10
                                 || tmp1 > 320
                                 ) {
                                    break ;
                                }   // if (tmp1 < 10 || tmp1 > 320)
                            }
                            else if (tmp1 > 0) {
                                break ;
                            }   // if (tmp1 > 0)
                        }   // if (cmd == TEF668X_API_FM_Set_QualityStatus)
                        else {
                            // 0 = no flag set after tuning (default)
                            // [ 8:0 ] : 10 … 320 (* 0.1 ms) = set flag at 1 … 32 ms after tuning ready
                            if (tmp.udata16 != 0 && (tmp.udata16 < 10 || tmp.udata16 > 320)) {
                                break ;
                            }   // if (tmp.udata16 != 0 && (tmp.udata16 < 10 || tmp.udata16 > 320))
                        }   // if (cmd == TEF668X_API_AM_Set_QualityStatus)
                        pSet_QualityStatus->mode_hi = tmp.hibyte ;
                        pSet_QualityStatus->mode_lo = tmp.lobyte ;

                        // interface : quality status pin signal functionality
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = no pin interface (default)
                         || tmp.udata16 == 2    // 2 = quality status output; active low  (‘QSI’)
                         ) {
                            pSet_QualityStatus->interface_hi = tmp.hibyte ;
                            pSet_QualityStatus->interface_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 7
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 2)
                    }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_FM_Set_DR_Blend :
            case TEF668X_API_AM_Set_DR_Blend :
                {
                    if (argc == 4) {
                        PSet_DR_Blend   pSet_DR_Blend ;

                        pSet_DR_Blend = (PSet_DR_Blend)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_DR_Blend) {
                            pSet_DR_Blend->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_DR_Blend)
                        else {
                            pSet_DR_Blend->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_DR_Blend)
                        pSet_DR_Blend->cmd   = TEF668x_cmd_Set_DR_Blend ;
                        pSet_DR_Blend->index = 1 ;

                        // mode : blend pin use (DR_BL input)
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 0    // 0 = Standard pin use : DR Blend pin High = digital radio (default)
                         && tmp.udata16 <= 3    // 1 = Inverted pin use : DR Blend pin Low = digital radio
                                                // 2 = No pin use; Force blend to digital radio
                                                // 3 = No pin use; Force blend to analog radio
                         ) {
                            pSet_DR_Blend->mode_hi = tmp.hibyte ;
                            pSet_DR_Blend->mode_lo = tmp.lobyte ;

                            // in_time : blend time from analog radio to digital radio
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 10   // 10 … 5000 [*0.1 ms] = 1 … 500 ms
                             && tmp.udata16 <= 5000
                             ) {
                                pSet_DR_Blend->in_time_hi = tmp.hibyte ;
                                pSet_DR_Blend->in_time_lo = tmp.lobyte ;

                                // out_time : blend time from digital radio to analog radio
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 10   // 10 … 5000 [*0.1 ms] = 1 … 500 ms
                                 && tmp.udata16 <= 5000
                                 ) {
                                    pSet_DR_Blend->out_time_hi = tmp.hibyte ;
                                    pSet_DR_Blend->out_time_lo = tmp.lobyte ;

                                    // gain : wdigital radio channel gain
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.data16 >= -180  // -180 … +60 [*0.1 dB] = -18 … +6 dB  digital radio signal gain
                                     && tmp.data16 <= 60
                                     ) {
                                        pSet_DR_Blend->gain_hi = tmp.hibyte ;
                                        pSet_DR_Blend->gain_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                        , TEF668x_Buf
                                                        , 11
                                                        , Handle_TEF668x_Service
                                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                        ret_state = STATUS_SUCCESS ;
                                    }   // if (tmp.data16 >= -180 && tmp.udata16 <= 60)
                                }   // if (tmp.udata16 >= 10 && tmp.udata16 <= 5000)
                            }   // if (tmp.udata16 >= 10 && tmp.udata16 <= 5000)
                        }   // if (tmp.udata16 >= 0 && tmp.udata16 <= 3)
                    }   // if (argc == 4)
                }
                break ;
            case TEF668X_API_FM_Set_DR_Options :
            case TEF668X_API_AM_Set_DR_Options :
                {
                    if (argc == 1 || argc == 3) {
                        PSet_DR_Options pSet_DR_Options ;
#if (TEF668x_API_DEBUG == 0)
                        uint8_t         write_len = 0 ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                        pSet_DR_Options = (PSet_DR_Options)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_DR_Options) {
                            pSet_DR_Options->module   = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_DR_Options)
                        else {
                            pSet_DR_Options->module   = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_DR_Options)
                        pSet_DR_Options->cmd      = TEF668x_cmd_Set_DR_Options ;
                        pSet_DR_Options->index    = 1 ;

                        // samplerate : baseband digital radio sample rate (DR I2S output)
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = automatic frequency selection based on tuning frequency (default)
                         || tmp.udata16 == 6500 // 6500 = 650 kHz (not for normal application use)
                         || tmp.udata16 == 6750 // 6750 = 675 kHz (not for normal application use)
                         ) {
                            pSet_DR_Options->samplerate_hi = tmp.hibyte ;
                            pSet_DR_Options->samplerate_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            write_len = 5 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                            if (argc == 3) {
                                // mode : baseband digital radio pin mode
                                tmp.udata16 = va_arg(va, int) ;
                                // [ 15:8 ] = BCK and WS pin mode
                                //            34 = voltage output (default)
                                //            51 = current output (‘virtual ground’)  20 * I0
                                //            187 … 255 = current output level selection:
                                //              [ 15:12 ] = 11 … 15 = BCK  6 … 31 * I0
                                //              [ 11:8 ]  = 11 … 15 = WS   6 … 31 * I0

                                if (tmp.hibyte != 34
                                 && tmp.hibyte != 51
                                 && tmp.hibyte < 187
                                 ) {
                                    break ;
                                }   // if (tmp.hibyte != 34 && tmp.hibyte != 51 && tmp.hibyte < 187)

                                // [ 7:0 ] = Data pin(s) mode
                                //            2 = voltage output
                                //            3 = current output (‘virtual ground’)  20 * I0
                                //            4 = open drain (‘pull down’) (default)
                                //            11 … 15 = current output level selection  6 … 31 * I0
                                if (tmp.lobyte != 2
                                 && tmp.lobyte != 3
                                 && tmp.lobyte != 4
                                 && (tmp.lobyte < 11 || tmp.lobyte > 15)
                                 ) {
                                    break ;
                                }   // if (tmp.lobyte != 2 && tmp.lobyte != 3 && tmp.lobyte != 4 && (tmp.lobyte < 11 || tmp.lobyte > 15))
                                pSet_DR_Options->mode_hi = tmp.hibyte ;
                                pSet_DR_Options->mode_lo = tmp.lobyte ;

                                // format : baseband digital radio format select
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 == 16   // 16 = I2S 16 bit, ‘3 wire’ interface with single I/Q signal line (DR_I_DATA)
                                                        //      (fDR_BCK = 32 * sample rate)
                                 || tmp.udata16 == 4112 // 4112 =  I2S 16 bit, ‘4 wire’ interface with independent I and Q signal lines
                                                        //      (fDR_BCK = 16 * sample rate) (default)
                                 ) {
                                    pSet_DR_Options->format_hi = tmp.hibyte ;
                                    pSet_DR_Options->format_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                    write_len = 9 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                                }   // if (tmp.udata16 == 16 || tmp.udata16 == 4112)
                                else {
                                    break ;
                                }
                            }   // if (argc == 3)
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , write_len
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 6500 || tmp.udata16 == 6750)
                    }   // if (argc == 3)
                }
                break ;
            case TEF668X_API_FM_Set_Specials :
            case TEF668X_API_AM_Set_Specials :
                {
                    if (argc == 1) {
                        PSet_Specials   pSet_Specials ;

                        pSet_Specials = (PSet_Specials)TEF668x_Buf ;

                        if (cmd == TEF668X_API_FM_Set_Specials) {
                            pSet_Specials->module = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Set_Specials)
                        else {
                            pSet_Specials->module = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Set_Specials)
                        pSet_Specials->cmd    = TEF668x_cmd_Set_Specials ;
                        pSet_Specials->index  = 1 ;

                        // ana_out : audio output use
                        tmp.udata16 = va_arg(va, int) ;
                        if (cmd == TEF668X_API_FM_Set_Specials) {
                            if (tmp.udata16 > 2     // 0 = normal operation (default)
                                                    // 1 = DAC_L : FM MPX wideband (DARC) signal / DAC_R : FM mono audio
                                                    // 2 = L : digital radio left channel / R : analog radio left channel
                             ) {
                                break ;
                            }
                        }   // if (cmd == TEF668X_API_FM_Set_Specials)
                        else {
                            if (tmp.udata16 == 1    // 0 = normal operation (default)
                             || tmp.udata16 >  2    // 2 = L : digital radio left channel / R : analog radio left channel
                             ) {
                                break ;
                            }
                        }   // if (cmd == TEF668X_API_AM_Set_Specials)
                        pSet_Specials->ana_out_hi = tmp.hibyte ;
                        pSet_Specials->ana_out_lo = tmp.lobyte ;
#if (TEF668x_API_DEBUG == 0)
                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                        , TEF668x_Buf
                                        , 5
                                        , Handle_TEF668x_Service
                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                        ret_state = STATUS_SUCCESS ;
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Set_Bandwidth_Options :
                {
                    if (argc == 1) {
                        PSet_Bandwidth_Options  pSet_Bandwidth_Options ;

                        pSet_Bandwidth_Options = (PSet_Bandwidth_Options)TEF668x_Buf ;

                        pSet_Bandwidth_Options->module = TEF668x_Module_Radio_FM ;
                        pSet_Bandwidth_Options->cmd    = TEF668x_cmd_Set_Bandwidth_Options ;
                        pSet_Bandwidth_Options->index  = 1 ;

                        // modulation : extended API: FM automatic bandwidth boost on modulation
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 660  // 660 … 1330 (*0.1 %) = 66 … 133 % modulation (= 50 … 100 kHz FM dev.)
                         && tmp.udata16 <= 1330 // Modulation index where bandwidth is boosted to maximum bandwidth.
                          ) {
                            pSet_Bandwidth_Options->modulation_hi = tmp.hibyte ;
                            pSet_Bandwidth_Options->modulation_lo = tmp.lobyte ;
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 5
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (cmd == TEF668X_API_AM_Set_Specials)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Set_StBandBlend_Time :
                {
                    if (argc == 2) {
                        PSet_StBandBlend_Time   pSet_StBandBlend_Time ;

                        pSet_StBandBlend_Time = (PSet_StBandBlend_Time)TEF668x_Buf ;

                        pSet_StBandBlend_Time->module = TEF668x_Module_Radio_FM ;
                        pSet_StBandBlend_Time->cmd    = TEF668x_cmd_Set_StBandBlend_Time ;
                        pSet_StBandBlend_Time->index  = 1 ;

                        // attack : attack time of FMSI weak signal handling
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 10   // 10 … 1000 (ms) = 10 ms … 1 s attack time (control time towards mono)
                         && tmp.udata16 <= 1000
                         ) {
                            pSet_StBandBlend_Time->attack_hi = tmp.hibyte ;
                            pSet_StBandBlend_Time->attack_lo = tmp.lobyte ;

                            // decay : decay time of FMSI weak signal handling
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 10   // 10 … 1000 (ms) = 10 ms … 1 s decay time (control time towards stereo)
                             && tmp.udata16 <= 1000
                             ) {
                                pSet_StBandBlend_Time->decay_hi = tmp.hibyte ;
                                pSet_StBandBlend_Time->decay_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                , TEF668x_Buf
                                                , 7
                                                , Handle_TEF668x_Service
                                                ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                ret_state = STATUS_SUCCESS ;
                            }   // if (tmp.udata16 >= 10 && tmp.udata16 <= 1000)
                        }   // if (tmp.udata16 >= 10 && tmp.udata16 <= 1000)
                    }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_FM_Set_StBandBlend_Gain :
                {
                    if (argc == 4) {
                        PSet_StBandBlend_Gain   pSet_StBandBlend_Gain ;

                        pSet_StBandBlend_Gain = (PSet_StBandBlend_Gain)TEF668x_Buf ;

                        pSet_StBandBlend_Gain->module = TEF668x_Module_Radio_FM ;
                        pSet_StBandBlend_Gain->cmd    = TEF668x_cmd_Set_StBandBlend_Gain ;
                        pSet_StBandBlend_Gain->index  = 1 ;

                        // band1 : control sensitivity for low frequency audio band
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 >= 500  // 500 … 1500 [*0.1 %] = 50% … 150%  weak … strong control to mono
                         && tmp.udata16 <= 1500
                         ) {
                            pSet_StBandBlend_Gain->band1_hi = tmp.hibyte ;
                            pSet_StBandBlend_Gain->band1_lo = tmp.lobyte ;

                            // band2 :control sensitivity for audio band around 2 kHz
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 500  // 500 … 1500 [*0.1 %] = 50% … 150%  weak … strong control to mono
                             && tmp.udata16 <= 1500
                             ) {
                                pSet_StBandBlend_Gain->band2_hi = tmp.hibyte ;
                                pSet_StBandBlend_Gain->band2_lo = tmp.lobyte ;

                                // band3 : control sensitivity for audio band around 5 kHz
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 500  // 500 … 1500 [*0.1 %] = 50% … 150%  weak … strong control to mono
                                 && tmp.udata16 <= 1500
                                 ) {
                                    pSet_StBandBlend_Gain->band3_hi = tmp.hibyte ;
                                    pSet_StBandBlend_Gain->band3_lo = tmp.lobyte ;

                                    // band4 : control sensitivity for high frequency audio band
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 500  // 500 … 1500 [*0.1 %] = 50% … 150%  weak … strong control to mono
                                     && tmp.udata16 <= 1500
                                     ) {
                                        pSet_StBandBlend_Gain->band4_hi = tmp.hibyte ;
                                        pSet_StBandBlend_Gain->band4_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                        , TEF668x_Buf
                                                        , 11
                                                        , Handle_TEF668x_Service
                                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                        ret_state = STATUS_SUCCESS ;
                                    }   // if (tmp.udata16 >= 500 && tmp.udata16 <= 1500)
                                }   // if (tmp.udata16 >= 500 && tmp.udata16 <= 1500)
                            }   // if (tmp.udata16 >= 500 && tmp.udata16 <= 1500)
                        }   // if (tmp.udata16 >= 500 && tmp.udata16 <= 1500)
                    }   // if (argc == 4)
                }
                break ;
            case TEF668X_API_FM_Set_StBandBlend_Bias :
                {
                    if (argc == 4) {
                        PSet_StBandBlend_Bias   pSet_StBandBlend_Bias ;

                        pSet_StBandBlend_Bias = (PSet_StBandBlend_Bias)TEF668x_Buf ;

                        pSet_StBandBlend_Bias->module = TEF668x_Module_Radio_FM ;
                        pSet_StBandBlend_Bias->cmd    = TEF668x_cmd_Set_StBandBlend_Bias ;
                        pSet_StBandBlend_Bias->index  = 1 ;

                        // band1 : control bias for low frequency audio band
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.data16 >= -250  // -250 … +250 [*0.1 %] = -25% … +25%  stereo … mono bias
                         && tmp.data16 <= 250
                         ) {
                            pSet_StBandBlend_Bias->band1_hi = tmp.hibyte ;
                            pSet_StBandBlend_Bias->band1_lo = tmp.lobyte ;

                            // band2 : control bias for audio band around 2 kHz
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.data16 >= -250  // -250 … +250 [*0.1 %] = -25% … +25%  stereo … mono bias
                             && tmp.data16 <= 250
                             ) {
                                pSet_StBandBlend_Bias->band2_hi = tmp.hibyte ;
                                pSet_StBandBlend_Bias->band2_lo = tmp.lobyte ;

                                // band3 : control bias for audio band around 5 kHz
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.data16 >= -250  // -250 … +250 [*0.1 %] = -25% … +25%  stereo … mono bias
                                 && tmp.data16 <= 250
                                 ) {
                                    pSet_StBandBlend_Bias->band3_hi = tmp.hibyte ;
                                    pSet_StBandBlend_Bias->band3_lo = tmp.lobyte ;

                                    // band4 : control bias for high frequency audio band
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.data16 >= -250  // -250 … +250 [*0.1 %] = -25% … +25%  stereo … mono bias
                                     && tmp.data16 <= 250
                                     ) {
                                        pSet_StBandBlend_Bias->band4_hi = tmp.hibyte ;
                                        pSet_StBandBlend_Bias->band4_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                        , TEF668x_Buf
                                                        , 11
                                                        , Handle_TEF668x_Service
                                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                        ret_state = STATUS_SUCCESS ;
                                    }   // if (tmp.data16 >= -250 && tmp.data16 <= 250)
                                }   // if (tmp.data16 >= -250 && tmp.data16 <= 250)
                            }   // if (tmp.data16 >= -250 && tmp.data16 <= 250)
                        }   // if (tmp.data16 >= -250 && tmp.data16 <= 250)
                    }   // if (argc == 4)
                }
                break ;
            case TEF668X_API_AM_Set_CoChannelDet :
                {
                    if (argc == 4) {
                        PCoChannelDet       pCoChannelDet ;

                        pCoChannelDet = (PCoChannelDet)TEF668x_Buf ;

                        pCoChannelDet->module = TEF668x_Module_Radio_AM ;
                        pCoChannelDet->cmd    = TEF668x_cmd_Set_CoChannelDet ;
                        pCoChannelDet->index  = 1 ;

                        // mode : co-channel detector operation
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 1    // 1 = on (default)
                         ) {
                            pCoChannelDet->mode_hi = tmp.hibyte ;
                            pCoChannelDet->mode_lo = tmp.lobyte ;

                            // restart : co-channel detector restart
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 == 1    // 1 = manual restart; reset detector result and start looking for co-channel
                             || tmp.udata16 == 2    // 2 = automatic restart after tuning (default); start looking for new co-channel
                                                    //     after Preset and Search tuning action (see AM cmd 1 TuneTo)
                             ) {
                                pCoChannelDet->restart_hi = tmp.hibyte ;
                                pCoChannelDet->restart_lo = tmp.lobyte ;

                                // sensitivity : co-channel detection sensitivity
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 500  // 500 … 1500 [*0.1 %] = 50 … 150 % relative detection sensitivity
                                 && tmp.udata16 <= 1500
                                 ) {
                                    pCoChannelDet->sensitivity_hi = tmp.hibyte ;
                                    pCoChannelDet->sensitivity_lo = tmp.lobyte ;

                                    // count : co-channel detection count threshold
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 1    // 1 … 15 = 1 … 15 detection counts until signaling of co-channel detected
                                     && tmp.udata16 <= 15
                                     ) {
                                        pCoChannelDet->count_hi = tmp.hibyte ;
                                        pCoChannelDet->count_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                        , TEF668x_Buf
                                                        , 11
                                                        , Handle_TEF668x_Service
                                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                        ret_state = STATUS_SUCCESS ;
                                    }   // if (tmp.udata16 >= 1 && tmp.udata16 <= 15)
                                }   // if (tmp.udata16 >= 500 && tmp.udata16 <= 1500)
                            }   // if (tmp.udata16 == 1 || tmp.udata16 == 2)
                        }   // if (tmp.udata16 == 1)
                    }   // if (argc == 4)
                }
                break ;
            case TEF668X_API_AM_Set_NoiseBlanker_Audio :
                {
                    if (argc == 2 || argc == 3) {
                        PSet_NoiseBlanker_Audio pSet_NoiseBlanker_Audio ;
#if (TEF668x_API_DEBUG == 0)
                        uint8_t                 write_len = 0 ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                        pSet_NoiseBlanker_Audio = (PSet_NoiseBlanker_Audio)TEF668x_Buf ;

                        pSet_NoiseBlanker_Audio->module = TEF668x_Module_Radio_AM ;
                        pSet_NoiseBlanker_Audio->cmd    = TEF668x_cmd_Set_NoiseBlanker_Audio ;
                        pSet_NoiseBlanker_Audio->index  = 1 ;

                        // mode : AM audio noise blanker (audio frequency detection)
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off
                         || tmp.udata16 == 1    // 1 = on (default)
                         ) {
                            pSet_NoiseBlanker_Audio->mode_hi = tmp.hibyte ;
                            pSet_NoiseBlanker_Audio->mode_lo = tmp.lobyte ;

                            // sensitivity : AM audio noise blanker trigger sensitivity
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 500      // 500 … 1500 [*0.1 %] = 50 … 150 % relative trigger sensitivity
                             && tmp.udata16 <= 1500
                             ) {
                                pSet_NoiseBlanker_Audio->sensitivity_hi = tmp.hibyte ;
                                pSet_NoiseBlanker_Audio->sensitivity_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                write_len = 7 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                                if (argc == 3) {
                                    // blank_time : extended API; AM noise blanker time
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 400  // 400 … 1200 [*1 µs] = 400 … 1200 µs pulse stretch time
                                     && tmp.udata16 <= 1200
                                     ) {
                                        pSet_NoiseBlanker_Audio->blank_time_hi = tmp.hibyte ;
                                        pSet_NoiseBlanker_Audio->blank_time_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                        write_len = 11 ;
#endif  // #if (TEF668x_API_DEBUG == 0)
                                    }   // if (tmp.udata16 >= 400 && tmp.udata16 <= 1200)
                                    else {
                                        break ;
                                    }
                                }   // if (argc == 3)
#if (TEF668x_API_DEBUG == 0)
                                SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                , TEF668x_Buf
                                                , write_len
                                                , Handle_TEF668x_Service
                                                ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                ret_state = STATUS_SUCCESS ;
                            }   // if (tmp.udata16 >= 500 && tmp.udata16 <= 1500)
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 2 || argc == 3)
                }
                break ;
            case TEF668X_API_AM_Set_Softmute_Mod :
                {
                    if (argc == 4) {
                        PSet_Softmute_Mod   pSet_Softmute_Mod ;

                        pSet_Softmute_Mod = (PSet_Softmute_Mod)TEF668x_Buf ;

                        pSet_Softmute_Mod->module = TEF668x_Module_Radio_AM ;
                        pSet_Softmute_Mod->cmd    = TEF668x_cmd_Set_Softmute_Mod ;
                        pSet_Softmute_Mod->index  = 1 ;

                        // mode : modulation dependent weak signal handling
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = off (default)
                         || tmp.udata16 == 1    // 1 = on
                         ) {
                            pSet_Softmute_Mod->mode_hi = tmp.hibyte ;
                            pSet_Softmute_Mod->mode_lo = tmp.lobyte ;

                            // start : weak signal handling modulation start
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 >= 100      // 100 … 1000 [*0.1 %] = control when modulation falls below 10% … 100%
                             && tmp.udata16 <= 1000
                             ) {
                                pSet_Softmute_Mod->start_hi = tmp.hibyte ;
                                pSet_Softmute_Mod->start_lo = tmp.lobyte ;

                                // slope : weak signal handling modulation range
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 >= 30      // 30 … 1000 (*0.1 %) = control over modulation range of 3% … 100%
                                 && tmp.udata16 <= 1000
                                 ) {
                                    pSet_Softmute_Mod->slope_hi = tmp.hibyte ;
                                    pSet_Softmute_Mod->slope_lo = tmp.lobyte ;

                                    // shift : weak signal handling control shift
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 50      // 50 … 1000 (*0.1 %) = maximum weak signal control shift of 5% … 100%
                                     && tmp.udata16 <= 1000
                                     ) {
                                        pSet_Softmute_Mod->slope_hi = tmp.hibyte ;
                                        pSet_Softmute_Mod->slope_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                        SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                        I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                        , TEF668x_Buf
                                                        , 11
                                                        , Handle_TEF668x_Service
                                                        ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                        ret_state = STATUS_SUCCESS ;
                                    }   // if (tmp.udata16 >= 50 && tmp.udata16 <= 1000)
                                }   // if (tmp.udata16 >= 30 && tmp.udata16 <= 1000)
                            }   // if (tmp.udata16 >= 100 && tmp.udata16 <= 1000)
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 4)
                }
                break ;
            case TEF668X_API_AUDIO_Set_Volume :
                {
                    if (argc == 1) {
                        PSet_Volume     pSet_Volume ;

                        pSet_Volume = (PSet_Volume)TEF668x_Buf ;

                        pSet_Volume->module = TEF668x_Module_Audio ;
                        pSet_Volume->cmd    = TEF668x_cmd_Set_Volume ;
                        pSet_Volume->index  = 1 ;

                        // volume : audio volume
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.data16 >= -599  // -599 … +240 = -60 … +24 dB volume
                         && tmp.data16 <= 240
                         ) {
                            pSet_Volume->volume_hi = tmp.hibyte ;
                            pSet_Volume->volume_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                    , TEF668x_Buf
                                            , 5
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.data16 >= -599 && tmp.data16 <= 240)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_AUDIO_Set_Mute :
                {
                    if (argc == 1) {
                        PSet_Mute   pPSet_Mute ;

                        pPSet_Mute = (PSet_Mute)TEF668x_Buf ;

                        pPSet_Mute->module = TEF668x_Module_Audio ;
                        pPSet_Mute->cmd    = TEF668x_cmd_Set_Mute ;
                        pPSet_Mute->index  = 1 ;

                        // mode : audio mute
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = mute disabled
                         || tmp.udata16 == 1    // 1 = mute active (default)
                         ) {
                            pPSet_Mute->mode_hi = tmp.hibyte ;
                            pPSet_Mute->mode_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                    , TEF668x_Buf
                                            , 5
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.data16 >= -599 && tmp.data16 <= 240)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_AUDIO_Set_Input :
                {
                    if (argc == 1) {
                        PSet_Input  pSet_Input ;

                        pSet_Input = (PSet_Input)TEF668x_Buf ;

                        pSet_Input->module = TEF668x_Module_Audio ;
                        pSet_Input->cmd    = TEF668x_cmd_Set_Input ;
                        pSet_Input->index  = 1 ;

                        // volume : audio volume
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0    // 0 = radio with DR blend (default)
                                                //     (analog radio or digital radio when enabled and DR blend active)
                         || tmp.udata16 == 32   // 32 = I2S digital audio input I2S_SD_0
                         || tmp.udata16 == 240  // 240 = sine wave generator
                         ) {
                            pSet_Input->source_hi = tmp.hibyte ;
                            pSet_Input->source_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                    , TEF668x_Buf
                                            , 5
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.data16 >= -599 && tmp.data16 <= 240)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_AUDIO_Set_Output_Source :
                {
                    if (argc == 2) {
                        PSet_Output_Source  pSet_Output_Source ;

                        pSet_Output_Source = (PSet_Output_Source)TEF668x_Buf ;

                        pSet_Output_Source->module = TEF668x_Module_Audio ;
                        pSet_Output_Source->cmd    = TEF668x_cmd_Set_Output_Source ;
                        pSet_Output_Source->index  = 1 ;

                        // signal : audio output
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 33   // 33 = I2S digital audio output I2S_SD_1
                         || tmp.udata16 == 128  // 128 = DAC L/R output
                         ) {
                            pSet_Output_Source->signal_hi = tmp.hibyte ;
                            pSet_Output_Source->signal_lo = tmp.lobyte ;

                            // source
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 == 4    // 4 = analog radio
                             || tmp.udata16 == 32   // 32 = I2S digital audio input I2S_SD_0
                             || tmp.udata16 == 224  // 224 = audio processor (default)
                             || tmp.udata16 == 240  // 240 = sine wave generator
                             ) {
                                pSet_Output_Source->source_hi = tmp.hibyte ;
                                pSet_Output_Source->source_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                , TEF668x_Buf
                                                , 7
                                                , Handle_TEF668x_Service
                                                ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                ret_state = STATUS_SUCCESS ;
                            }   // if (tmp.udata16 == 4 || tmp.udata16 == 32 || tmp.udata16 == 224 || tmp.udata16 == 240)
                        }   // if (tmp.udata16 == 33 || tmp.udata16 == 128)
                    }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_AUDIO_Set_Ana_Out :
                {
                    if (argc == 2) {
                        PSet_Ana_Out    pSet_Ana_Out ;

                        pSet_Ana_Out = (PSet_Ana_Out)TEF668x_Buf ;

                        pSet_Ana_Out->module = TEF668x_Module_Audio ;
                        pSet_Ana_Out->cmd    = TEF668x_cmd_Set_Ana_Out ;
                        pSet_Ana_Out->index  = 1 ;

                        // signal : analog audio output
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 128  // 128 = DAC L/R output
                         ) {
                            pSet_Ana_Out->signal_hi = tmp.hibyte ;
                            pSet_Ana_Out->signal_lo = tmp.lobyte ;

                            // mode : output mode
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 == 0    // 0 = off (power down)
                             || tmp.udata16 == 1    // 1 = output enabled (default)
                             ) {
                                pSet_Ana_Out->mode_hi = tmp.hibyte ;
                                pSet_Ana_Out->mode_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                , TEF668x_Buf
                                                , 7
                                                , Handle_TEF668x_Service
                                                ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                ret_state = STATUS_SUCCESS ;
                            }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                        }   // if (tmp.udata16 == 128)
                    }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_AUDIO_Set_Dig_IO :
                {
                    if (argc == 5) {
                        PSet_Dig_IO     pSet_Dig_IO ;

                        pSet_Dig_IO = (PSet_Dig_IO)TEF668x_Buf ;

                        pSet_Dig_IO->module = TEF668x_Module_Audio ;
                        pSet_Dig_IO->cmd    = TEF668x_cmd_Set_Dig_IO ;
                        pSet_Dig_IO->index  = 1 ;

                        // signal : digital audio input / output
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 32   // 32 = I²S digital audio I2S_SD_0 (input)
                         || tmp.udata16 == 33   // 33 = I²S digital audio I2S_SD_1 (output)
                         ) {
                            pSet_Dig_IO->signal_hi = tmp.hibyte ;
                            pSet_Dig_IO->signal_lo = tmp.lobyte ;

                            // mode : I/O mode
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 == 0    // 0 = off (default)
                             || tmp.udata16 == 1    // 1 = current input (only available for signal = 32)
                             || tmp.udata16 == 2    // 2 = voltage output (only available for signal = 33)
                             || tmp.udata16 == 3    // 3 = current output (only available for signal = 33)  )  6 * I0
                             || (tmp.udata16 >= 11 && tmp.udata16 <= 15)
                                                    // 11 … 15 = current output level selection  6 … 31 * I0
                             ) {
                                pSet_Dig_IO->mode_hi = tmp.hibyte ;
                                pSet_Dig_IO->mode_lo = tmp.lobyte ;

                                // format : digital audio format select
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 == 16   // 16 = I2S 16 bits (fI2S_BCK = 32 * samplerate)
                                 || tmp.udata16 == 32   // 32 = I2S 32 bits (fI2S_BCK = 64 * samplerate) (default)
                                 || tmp.udata16 == 272  // 272 = lsb aligned 16 bit (fI2S_BCK = 64 * samplerate)
                                 || tmp.udata16 == 274  // 274 = lsb aligned 18 bit (fI2S_BCK = 64 * samplerate)
                                 || tmp.udata16 == 276  // 276 = lsb aligned 20 bit (fI2S_BCK = 64 * samplerate)
                                 || tmp.udata16 == 280  // 280 = lsb aligned 24 bit (fI2S_BCK = 64 * samplerate)
                                 ) {
                                    pSet_Dig_IO->format_hi = tmp.hibyte ;
                                    pSet_Dig_IO->format_lo = tmp.lobyte ;

                                    // operation : operation mode
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 == 0    // 0 = audio slave mode; I2S_BCK and I2S_WS input signals defined by
                                                            //     external signal source (default)
                                     || tmp.udata16 == 256  // 256 = audio master mode; I2S_BCK and I2S_WS voltage output
                                     || tmp.udata16 == 273  // 273 = audio master mode; I2S_BCK and I2S_WS current output  6 * I 0
                                     || tmp.udata16 >= 443  // 443 … 511 = audio master mode; current output level selection:
                                                            //     [ 15:8 ] = 1, [ 7:4 ] = 11 … 15 = I2S_BCK  6 … 31 * I0
                                                            //     [ 15:8 ] = 1, [ 3:0 ] = 11 … 15 = I2S_WS   6 … 31 * I0
                                     ) {
                                        pSet_Dig_IO->operation_hi = tmp.hibyte ;
                                        pSet_Dig_IO->operation_lo = tmp.lobyte ;

                                        // samplerate : audio sample rate select
                                        tmp.udata16 = va_arg(va, int) ;
                                        if (tmp.udata16 == 3200 // 3200 = 32.0 kHz
                                         || tmp.udata16 == 4410 // 4410 = 44.1 kHz (default)
                                         || tmp.udata16 == 4800 // 4800 = 48.0 kHz
                                         ) {
                                            pSet_Dig_IO->samplerate_hi = tmp.hibyte ;
                                            pSet_Dig_IO->samplerate_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                            , TEF668x_Buf
                                                            , 13
                                                            , Handle_TEF668x_Service
                                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                            ret_state = STATUS_SUCCESS ;
                                        }   // if (tmp.udata16 == 3200 || tmp.udata16 == 4410 || tmp.udata16 == 4800)
                                    }   // if (tmp.udata16 == 0 || tmp.udata16 == 256 || tmp.udata16 == 273 || tmp.udata16 >= 443)
                                }   // if (tmp.udata16 == 16 || tmp.udata16 == 32 || tmp.udata16 == 272 || tmp.udata16 == 274 || tmp.udata16 == 276 || tmp.udata16 == 280)
                            }   // if (tmp.udata16 == 0 || tmp.udata16 == 1 || tmp.udata16 == 2 || tmp.udata16 == 3 || (tmp.udata16 >= 11 && tmp.udata16 <= 15))
                        }   // if (tmp.udata16 == 32 || tmp.udata16 == 33)
                    }   // if (argc == 5)
                }
                break ;
            case TEF668X_API_AUDIO_Set_Input_Scaler :
                {
                    if (argc == 2) {
                        PSet_Input_Scaler   pSet_Input_Scaler ;

                        pSet_Input_Scaler = (PSet_Input_Scaler)TEF668x_Buf ;

                        pSet_Input_Scaler->module = TEF668x_Module_Audio ;
                        pSet_Input_Scaler->cmd    = TEF668x_cmd_Set_Input_Scaler ;
                        pSet_Input_Scaler->index  = 1 ;

                        // source : audio source
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 32  // 32 = I2S digital audio input : I2S_SD_0
                         ) {
                            pSet_Input_Scaler->source_hi = tmp.hibyte ;
                            pSet_Input_Scaler->source_lo = tmp.lobyte ;

                            // gain : external source channel gain
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.data16 >= -120  // -120 … +60 [*0.1 dB] = -12 … +6 dB external source signal gain
                             && tmp.data16 <= 60
                             ) {
                                pSet_Input_Scaler->gain_hi = tmp.hibyte ;
                                pSet_Input_Scaler->gain_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                , TEF668x_Buf
                                                , 7
                                                , Handle_TEF668x_Service
                                                ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                ret_state = STATUS_SUCCESS ;
                            }   // if (tmp.udata16 >= -120 || tmp.udata16 <= 60)
                        }   // if (tmp.udata16 == 32)
                    }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_AUDIO_Set_WaveGen :
                {
                    if (argc == 6) {
                        PSet_WaveGen    pSet_WaveGen ;

                        pSet_WaveGen = (PSet_WaveGen)TEF668x_Buf ;

                        pSet_WaveGen->module = TEF668x_Module_Audio ;
                        pSet_WaveGen->cmd    = TEF668x_cmd_Set_WaveGen ;
                        pSet_WaveGen->index  = 1 ;

                        // mode :
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 != 4// 0 = wave signal off (default)
                         && tmp.udata16 < 8 // 1 = wave 1 signal on Left channel
                                            // 2 = wave 2 signal on Right channel
                                            // 3 = wave 1 signal on Left channel and wave 2 signal on Right channel
                                            // 5 = wave 1 signal on Left and Right channel
                                            // 6 = wave 2 signal on Left and Right channel
                                            // 7 = wave 1 + wave 2 signal on Left and Right channel
                         ) {
                            pSet_WaveGen->mode_hi = tmp.hibyte ;
                            pSet_WaveGen->mode_lo = tmp.lobyte ;

                            // offset : DC offset
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.data16 >= -32768    // -32768 … + 32767 (* 1 LSB of 16 bit) = max negative … max positive.
                             && tmp.data16 <= 32768
                             ) {
                                pSet_WaveGen->offset_hi = tmp.hibyte ;
                                pSet_WaveGen->offset_lo = tmp.lobyte ;

                                // amplitude1 : wave 1 amplitude
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.data16 >= -300  // -300 … 0 (*0.1 dB) = -30 … 0 dB
                                 && tmp.data16 <= 0
                                 ) {
                                    pSet_WaveGen->amplitude1_hi = tmp.hibyte ;
                                    pSet_WaveGen->amplitude1_lo = tmp.lobyte ;

                                    // frequency1 : wave 1 frequency
                                    tmp.udata16 = va_arg(va, int) ;
                                    if (tmp.udata16 >= 10       // 10 … 20000 (*1 Hz) = 10 Hz … 20 kHz
                                     && tmp.udata16 <= 20000
                                     ) {
                                        pSet_WaveGen->frequency1_hi = tmp.hibyte ;
                                        pSet_WaveGen->frequency1_lo = tmp.lobyte ;

                                        // amplitude2 : wave 2 amplitude
                                        tmp.udata16 = va_arg(va, int) ;
                                        if (tmp.data16 >= -300  // -300 … 0 (*0.1 dB) = -30 … 0 dB
                                         && tmp.data16 <= 0
                                         ) {
                                            pSet_WaveGen->amplitude2_hi = tmp.hibyte ;
                                            pSet_WaveGen->amplitude2_lo = tmp.lobyte ;

                                            // frequency2 : wave 2 frequency
                                            tmp.udata16 = va_arg(va, int) ;
                                            if (tmp.udata16 >= 10   // 10 … 20000 (*1 Hz) = 10 Hz … 20 kHz
                                             && tmp.udata16 <= 20000
                                             ) {
                                                pSet_WaveGen->frequency2_hi = tmp.hibyte ;
                                                pSet_WaveGen->frequency2_lo = tmp.lobyte ;

    #if (TEF668x_API_DEBUG == 0)
                                                SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                                I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                                , TEF668x_Buf
                                                                , 13
                                                                , Handle_TEF668x_Service
                                                                ) ;
    #endif  // #if (TEF668x_API_DEBUG == 0)

                                                ret_state = STATUS_SUCCESS ;
                                            }   // if (tmp.udata16 >= 10 && tmp.udata16 <= 20000)
                                        }   // if (tmp.data16 >= -300 && tmp.data16 <= 0)
                                    }   // if (tmp.udata16 >= 10 && tmp.udata16 <= 20000)
                                }   // if (tmp.data16 >= -300 && tmp.data16 <= 0)
                            }   // if (tmp.data16 >= -32768 && tmp.udata16 <= 32768)
                        }   // if (tmp.udata16 != 4 && tmp.udata16 < 8)
                    }   // if (argc == 6)
                }
                break ;
            case TEF668X_API_APPL_Set_OperationMode :
                {
                    if (argc == 1) {
                        PSet_OperationMode  pSet_OperationMode ;

                        pSet_OperationMode = (PSet_OperationMode)TEF668x_Buf ;

                        pSet_OperationMode->module = TEF668x_Module_APPL ;
                        pSet_OperationMode->cmd    = TEF668x_cmd_Set_OperationMode ;
                        pSet_OperationMode->index  = 1 ;

                        // mode : device operation mode
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 0        // 0 = normal operation
                         || tmp.udata16 == 1        // 1 = radio standby mode (low-power mode without radio function) (default)
                         ) {
                            pSet_OperationMode->mode_hi = tmp.hibyte ;
                            pSet_OperationMode->mode_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 5
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.udata16 == 0 || tmp.udata16 == 1)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_APPL_Set_GPIO :
                {
                    if (argc == 3) {
                        PSet_GPIO   pSet_GPIO ;

                        pSet_GPIO = (PSet_GPIO)TEF668x_Buf ;

                        pSet_GPIO->module = TEF668x_Module_APPL ;
                        pSet_GPIO->cmd    = TEF668x_cmd_Set_GPIO ;
                        pSet_GPIO->index  = 1 ;

                        // pin : GPIO number
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 <= 3    // 0 … 3 = GPIO number
                         ) {
                            pSet_GPIO->pin_hi = tmp.hibyte ;
                            pSet_GPIO->pin_lo = tmp.lobyte ;

                            // module : AM/FM
                            tmp.udata16 = va_arg(va, int) ;
                            if (tmp.udata16 == TEF668x_Module_Radio_FM   // 32 = FM
                             || tmp.udata16 == TEF668x_Module_Radio_AM   // 33 = AM
                             ) {
                                pSet_GPIO->module_hi = tmp.hibyte ;
                                pSet_GPIO->module_lo = tmp.lobyte ;

                                // feature
                                tmp.udata16 = va_arg(va, int) ;
                                if (tmp.udata16 <= 3    // 0 = no use (default) (FM / AM)
                                                        // 1 = general purpose input (FM / AM)
                                                        // 2 = general purpose output ‘0’ (FM / AM)
                                                        // 3 = general purpose output ‘1’ (FM / AM)
                                 || (tmp.udata16 >= 257 // 257 = output RDS (FM : see cmd 81 ‘DAVN’)
                                  && tmp.udata16 <= 262)// 258 = output QSI (FM / AM : see cmd 82 ‘timer and AF_Update flag’)
                                                        // 259 = output QSI + RDS (active ‘low’ if ‘DAVN’ is active or ‘QSI’ is active)
                                                        // 260 = output RDDA (FM : see cmd 81 ‘RDDA, RDCL legacy option’)
                                                        // 261 = output RDCL (FM : see cmd 81 ‘RDDA, RDCL legacy option’)
                                                        // 262 = output AGC (FM : see cmd 11 ‘AGC step extension’)
                                 ) {
                                    pSet_GPIO->feature_hi = tmp.hibyte ;
                                    pSet_GPIO->feature_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                                    SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                                    I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                                    , TEF668x_Buf
                                                    , 9
                                                    , Handle_TEF668x_Service
                                                    ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                                    ret_state = STATUS_SUCCESS ;
                                 }   // if (tmp.udata16 <= 3 || (tmp.udata16 >= 257 && tmp.udata16 <= 262))
                            }   // if (tmp.udata16 == TEF668x_Module_Radio_FM || tmp.udata16 == TEF668x_Module_Radio_AM)
                        }   // if (tmp.udata16 <= 3)
                    }   // if (argc == 3)
                }
                break ;
            case TEF668X_API_APPL_Set_ReferenceClock :
                {
                    if (argc == 2) {
                        PSet_ReferenceClock pSet_ReferenceClock ;
                        XDATA32             tmp32 ;

                        pSet_ReferenceClock = (PSet_ReferenceClock)TEF668x_Buf ;

                        pSet_ReferenceClock->module = TEF668x_Module_APPL ;
                        pSet_ReferenceClock->cmd    = TEF668x_cmd_Set_ReferenceClock ;
                        pSet_ReferenceClock->index  = 1 ;

                        // the reference clock frequency
                        tmp32.udata32 = va_arg(va, uint32_t) ;
                        pSet_ReferenceClock->frequency_msb_hi = tmp32.hword_msb ;
                        pSet_ReferenceClock->frequency_msb_lo = tmp32.hword_lsb ;
                        pSet_ReferenceClock->frequency_lsb_hi = tmp32.lword_msb ;
                        pSet_ReferenceClock->frequency_lsb_lo = tmp32.lword_lsb ;

                        // clock type
                        tmp32.word_lsb = va_arg(va, int) ;
                        if (tmp32.word_lsb == 0     // crystal oscillator operation (default)
                         || tmp32.word_lsb == 1     // external clock input operation
                        ) {
                            pSet_ReferenceClock->type_hi = tmp32.lword_msb ;
                            pSet_ReferenceClock->type_lo = tmp32.lword_lsb ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 9
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp32.word_lsb == 0 || tmp32.word_lsb == 1)
                    }   // if (argc == 2)
                }
                break ;
            case TEF668X_API_APPL_Activate :
                {
                    if (argc == 1) {
                        PSet_APPL_Activate  pSet_APPL_Activate ;

                        pSet_APPL_Activate = (PSet_APPL_Activate)TEF668x_Buf ;

                        pSet_APPL_Activate->module = TEF668x_Module_APPL ;
                        pSet_APPL_Activate->cmd    = TEF668x_cmd_Activate ;
                        pSet_APPL_Activate->index  = 1 ;

                        // mode
                        tmp.udata16 = va_arg(va, int) ;
                        if (tmp.udata16 == 1    // 1 = goto ‘active’ state with operation mode of ‘radio standby’
                         ) {
                            pSet_APPL_Activate->mode_hi = tmp.hibyte ;
                            pSet_APPL_Activate->mode_lo = tmp.lobyte ;

#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;
                            I2C_WriteDataCB(I2C_ADDRESS_TEF668x
                                            , TEF668x_Buf
                                            , 5
                                            , Handle_TEF668x_Service
                                            ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (tmp.udata16 == 1)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Get_Quality_Status :
            case TEF668X_API_AM_Get_Quality_Status :
                {
                    if (argc == 1) {
                        uint8_t *   rbuf = (uint8_t *)NULL ;

                        if (cmd == TEF668X_API_FM_Get_Quality_Status) {
                            TEF668x_Buf[0] = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Get_Quality_Status)
                        else {
                            TEF668x_Buf[0] = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Get_Quality_Status)
                        TEF668x_Buf[1] = TEF668x_cmd_Get_Quality_Status ;
                        TEF668x_Buf[2] = 1 ;

                        rbuf = va_arg(va, uint8_t *) ;
                        if (rbuf != (uint8_t *)NULL) {
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;

                            I2C_WriteReadDataCB(I2C_ADDRESS_TEF668x
                                               , TEF668x_Buf
                                               , 3
                                               , rbuf
                                               , 1
                                               , Handle_TEF668x_Service
                                               ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (rbuf != (uint8_t *)NULL)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Get_Quality_Data :
            case TEF668X_API_AM_Get_Quality_Data :
                {
                    if (argc == 1) {
                        uint8_t *   rbuf = (uint8_t *)NULL ;

                        if (cmd == TEF668X_API_FM_Get_Quality_Data) {
                            TEF668x_Buf[0] = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Get_Quality_Data)
                        else {
                            TEF668x_Buf[0] = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Get_Quality_Data)
                        TEF668x_Buf[1] = TEF668x_cmd_Get_Quality_Data ;
                        TEF668x_Buf[2] = 1 ;

                        rbuf = va_arg(va, uint8_t *) ;
                        if (rbuf != (uint8_t *)NULL) {
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;

                            I2C_WriteReadDataCB(I2C_ADDRESS_TEF668x
                                               , TEF668x_Buf
                                               , 3
                                               , rbuf
                                               , 14
                                               , Handle_TEF668x_Service
                                               ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (rbuf != (uint8_t *)NULL)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Get_RDS_Status :
                {
                    if (argc == 1) {
                        uint8_t *   rbuf = (uint8_t *)NULL ;

                        TEF668x_Buf[0] = TEF668x_Module_Radio_FM ;
                        TEF668x_Buf[1] = TEF668x_cmd_Get_RDS_Status ;
                        TEF668x_Buf[2] = 1 ;

                        rbuf = va_arg(va, uint8_t *) ;
                        if (rbuf != (uint8_t *)NULL) {
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;

                            I2C_WriteReadDataCB(I2C_ADDRESS_TEF668x
                                               , TEF668x_Buf
                                               , 3
                                               , rbuf
                                               , 2
                                               , Handle_TEF668x_Service
                                               ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (rbuf != (uint8_t *)NULL)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Get_RDS_Data :
                {
                    if (argc == 1) {
                        uint8_t *   rbuf = (uint8_t *)NULL ;

                        TEF668x_Buf[0] = TEF668x_Module_Radio_FM ;
                        TEF668x_Buf[1] = TEF668x_cmd_Get_RDS_Data ;
                        TEF668x_Buf[2] = 1 ;

                        rbuf = va_arg(va, uint8_t *) ;
                        if (rbuf != (uint8_t *)NULL) {
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;

                            I2C_WriteReadDataCB(I2C_ADDRESS_TEF668x
                                               , TEF668x_Buf
                                               , 3
                                               , rbuf
                                               , 12
                                               , Handle_TEF668x_Service
                                               ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (rbuf != (uint8_t *)NULL)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Get_AGC :
            case TEF668X_API_AM_Get_AGC :
                {
                    if (argc == 1) {
                        uint8_t *   rbuf = (uint8_t *)NULL ;

                        if (cmd == TEF668X_API_FM_Get_AGC) {
                            TEF668x_Buf[0] = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Get_AGC)
                        else {
                            TEF668x_Buf[0] = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Get_AGC)
                        TEF668x_Buf[1] = TEF668x_cmd_Get_AGC ;
                        TEF668x_Buf[2] = 1 ;

                        rbuf = va_arg(va, uint8_t *) ;
                        if (rbuf != (uint8_t *)NULL) {
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;

                            I2C_WriteReadDataCB(I2C_ADDRESS_TEF668x
                                               , TEF668x_Buf
                                               , 3
                                               , rbuf
                                               , 4
                                               , Handle_TEF668x_Service
                                               ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (rbuf != (uint8_t *)NULL)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Get_Signal_Status :
            case TEF668X_API_AM_Get_Signal_Status :
                {
                    if (argc == 1) {
                        uint8_t *   rbuf = (uint8_t *)NULL ;

                        if (cmd == TEF668X_API_FM_Get_Signal_Status) {
                            TEF668x_Buf[0] = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Get_Signal_Status)
                        else {
                            TEF668x_Buf[0] = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Get_Signal_Status)
                        TEF668x_Buf[1] = TEF668x_cmd_Get_Signal_Status ;
                        TEF668x_Buf[2] = 1 ;

                        rbuf = va_arg(va, uint8_t *) ;
                        if (rbuf != (uint8_t *)NULL) {
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;

                            I2C_WriteReadDataCB(I2C_ADDRESS_TEF668x
                                               , TEF668x_Buf
                                               , 3
                                               , rbuf
                                               , 2
                                               , Handle_TEF668x_Service
                                               ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (rbuf != (uint8_t *)NULL)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Get_Processing_Status :
            case TEF668X_API_AM_Get_Processing_Status :
                {
                    if (argc == 1) {
                        uint8_t *   rbuf = (uint8_t *)NULL ;

                        if (cmd == TEF668X_API_FM_Get_Processing_Status) {
                            TEF668x_Buf[0] = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Get_Processing_Status)
                        else {
                            TEF668x_Buf[0] = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Get_Processing_Status)
                        TEF668x_Buf[1] = TEF668x_cmd_Get_Processing_Status ;
                        TEF668x_Buf[2] = 1 ;

                        rbuf = va_arg(va, uint8_t *) ;
                        if (rbuf != (uint8_t *)NULL) {
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;

                            if (cmd == TEF668X_API_FM_Get_Processing_Status) {
                                I2C_WriteReadDataCB(I2C_ADDRESS_TEF668x
                                                   , TEF668x_Buf
                                                   , 3
                                                   , rbuf
                                                   , 12
                                                   , Handle_TEF668x_Service
                                                   ) ;
                            }   // if (cmd == TEF668X_API_FM_Get_Processing_Status)
                            else {
                                I2C_WriteReadDataCB(I2C_ADDRESS_TEF668x
                                                   , TEF668x_Buf
                                                   , 3
                                                   , rbuf
                                                   , 4
                                                   , Handle_TEF668x_Service
                                                   ) ;
                            }   // if (cmd == TEF668X_API_AM_Get_Processing_Status)
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (rbuf != (uint8_t *)NULL)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_FM_Get_Interface_Status :
            case TEF668X_API_AM_Get_Interface_Status :
                {
                    if (argc == 1) {
                        uint8_t *   rbuf = (uint8_t *)NULL ;

                        if (cmd == TEF668X_API_FM_Get_Interface_Status) {
                            TEF668x_Buf[0] = TEF668x_Module_Radio_FM ;
                        }   // if (cmd == TEF668X_API_FM_Get_Interface_Status)
                        else {
                            TEF668x_Buf[0] = TEF668x_Module_Radio_AM ;
                        }   // if (cmd == TEF668X_API_AM_Get_Interface_Status)
                        TEF668x_Buf[1] = TEF668x_cmd_Get_Interface_Status ;
                        TEF668x_Buf[2] = 1 ;

                        rbuf = va_arg(va, uint8_t *) ;
                        if (rbuf != (uint8_t *)NULL) {
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;

                            I2C_WriteReadDataCB(I2C_ADDRESS_TEF668x
                                               , TEF668x_Buf
                                               , 3
                                               , rbuf
                                               , 2
                                               , Handle_TEF668x_Service
                                               ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (rbuf != (uint8_t *)NULL)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_APPL_Get_Operation_Status :
                {
                    if (argc == 1) {
                        uint8_t *   rbuf = (uint8_t *)NULL ;

                        TEF668x_Buf[0] = TEF668x_Module_APPL ;
                        TEF668x_Buf[1] = TEF668x_cmd_Get_Operation_Status ;
                        TEF668x_Buf[2] = 1 ;

                        rbuf = va_arg(va, uint8_t *) ;
                        if (rbuf != (uint8_t *)NULL) {
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;

                            I2C_WriteReadDataCB(I2C_ADDRESS_TEF668x
                                               , TEF668x_Buf
                                               , 3
                                               , rbuf
                                               , 2
                                               , Handle_TEF668x_Service
                                               ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (rbuf != (uint8_t *)NULL)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_APPL_Get_GPIO_Status :
                {
                    if (argc == 1) {
                        uint8_t *   rbuf = (uint8_t *)NULL ;

                        TEF668x_Buf[0] = TEF668x_Module_APPL ;
                        TEF668x_Buf[1] = TEF668x_cmd_Get_GPIO_Status ;
                        TEF668x_Buf[2] = 1 ;

                        rbuf = va_arg(va, uint8_t *) ;
                        if (rbuf != (uint8_t *)NULL) {
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;

                            I2C_WriteReadDataCB(I2C_ADDRESS_TEF668x
                                               , TEF668x_Buf
                                               , 3
                                               , rbuf
                                               , 2
                                               , Handle_TEF668x_Service
                                               ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (rbuf != (uint8_t *)NULL)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_APPL_Get_Identification :
                {
                    if (argc == 1) {
                        uint8_t *   rbuf = (uint8_t *)NULL ;

                        TEF668x_Buf[0] = TEF668x_Module_APPL ;
                        TEF668x_Buf[1] = TEF668x_cmd_Get_Identification ;
                        TEF668x_Buf[2] = 1 ;

                        rbuf = va_arg(va, uint8_t *) ;
                        if (rbuf != (uint8_t *)NULL) {
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;

                            I2C_WriteReadDataCB(I2C_ADDRESS_TEF668x
                                               , TEF668x_Buf
                                               , 3
                                               , rbuf
                                               , 6
                                               , Handle_TEF668x_Service
                                               ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (rbuf != (uint8_t *)NULL)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_APPL_Get_LastWrite :
                {
                    if (argc == 1) {
                        uint8_t *   rbuf = (uint8_t *)NULL ;

                        TEF668x_Buf[0] = TEF668x_Module_APPL ;
                        TEF668x_Buf[1] = TEF668x_cmd_Get_LastWrite ;
                        TEF668x_Buf[2] = 1 ;

                        rbuf = va_arg(va, uint8_t *) ;
                        if (rbuf != (uint8_t *)NULL) {
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;

                            I2C_WriteReadDataCB(I2C_ADDRESS_TEF668x
                                               , TEF668x_Buf
                                               , 3
                                               , rbuf
                                               , 14
                                               , Handle_TEF668x_Service
                                               ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (rbuf != (uint8_t *)NULL)
                    }   // if (argc == 1)
                }
                break ;
            case TEF668X_API_APPL_Get_Interface_Status :
                {
                    if (argc == 1) {
                        uint8_t *   rbuf = (uint8_t *)NULL ;

                        TEF668x_Buf[0] = TEF668x_Module_APPL ;
                        TEF668x_Buf[1] = TEF668x_cmd_Get_Interface_Status ;
                        TEF668x_Buf[2] = 1 ;

                        rbuf = va_arg(va, uint8_t *) ;
                        if (rbuf != (uint8_t *)NULL) {
#if (TEF668x_API_DEBUG == 0)
                            SetState_TEF668x (TEF668x_STATE_ACCESS_SERVICE) ;

                            I2C_WriteReadDataCB(I2C_ADDRESS_TEF668x
                                               , TEF668x_Buf
                                               , 3
                                               , rbuf
                                               , 2
                                               , Handle_TEF668x_Service
                                               ) ;
#endif  // #if (TEF668x_API_DEBUG == 0)

                            ret_state = STATUS_SUCCESS ;
                        }   // if (rbuf != (uint8_t *)NULL)
                    }   // if (argc == 1)
                }
                break ;
        }   // switch (cmd)
        va_end (va) ;
#if (TEF668x_API_DEBUG == 0)
    }   // if (GetState_TEF668x() == STATUS_SUCCESS && GetI2CExecuteState() == (status_t)I2C_SUCCESS)
#endif  // #if (TEF668x_API_DEBUG == 0)

    return (ret_state) ;
}   // status_t TEF668x_api (uint8_t cmd, PFUNCvS cb, uint8_t argc, ...)

void Handle_TEF668x_Service (status_t exe_state) {
    if (tuner_api_callback != (PFUNCvS)NULL) {
        tuner_api_callback(exe_state) ;
        tuner_api_callback = (PFUNCvS)NULL ;
    }   // if (tuner_api_callback != (PFUNCvS)NULL)
    SetState_TEF668x (TEF668x_STATE_STANDBY) ;
}   // void Handle_TEF668x_Service (status_t exe_state)

void Handle_TEF668x (void) {
    status_t tmp_state ;

    if (GetI2CState() != STATUS_BUSY) {
        switch (state_TEF668x) {
            case TEF668x_STATE_WAIT_POWERON :
                TimerStop(&tmr_TEF668x) ;
                TimerSet(&tmr_TEF668x, 10) ;
                state_TEF668x = TEF668x_STATE_CHECK_POWERON ;
                break ;
            case TEF668x_STATE_CHECK_POWERON :
                if (TimerHasExpired(&tmr_TEF668x) == TRUE) {
                    TimerStop(&tmr_TEF668x) ;
                    state_TEF668x = TEF668x_STATE_CHECK_POWERON_BEGIN ;
                }   // if (TimerHasExpired(&tmr_TEF668x) == TRUE)
                break ;
            case TEF668x_STATE_CHECK_POWERON_BEGIN :
                TEF668x_Buf[0] = TEF668x_Module_APPL ;
                TEF668x_Buf[1] = TEF668x_cmd_Get_Operation_Status ;
                TEF668x_Buf[2] = 1 ;
                I2C_WriteReadData(I2C_ADDRESS_TEF668x, TEF668x_Buf, 3U, TEF668x_Buf+3, 2U) ;
                state_TEF668x = TEF668x_STATE_CHECK_POWERON_END ;
                TimerSet(&tmr_TEF668x, 50) ;
                break ;
            case TEF668x_STATE_CHECK_POWERON_END :
                tmp_state = GetI2CExecuteState() ;
                if (tmp_state == (status_t)I2C_SUCCESS) {
                    TimerStop(&tmr_TEF668x) ;
                    state_boot = TEF668x_BOOT_STATE_START_STAGE ;
                    state_TEF668x = TEF668x_STATE_BOOT ;
                }   // if (tmp_state == I2C_SUCCESS)
                else if (TimerHasExpired(&tmr_TEF668x) == TRUE) {
                    state_TEF668x = TEF668x_STATE_CHECK_POWERON_BEGIN ;
                }   // if (TimerHasExpired(&tmr_TEF668x) == TRUE)
                break ;
            case TEF668x_STATE_BOOT :
                if (TEF668x_Boot() == STATUS_SUCCESS) {
                    state_TEF668x = TEF668x_STATE_STANDBY ;
                }   // if (TEF668x_Boot() == STATUS_SUCCESS)
                break ;
            case TEF668x_STATE_STANDBY :
                break ;
            case TEF668x_STATE_ACCESS_SERVICE :
                break ;
        }   // switch (state_TEF668x)
    }   // if (GetI2CState() != STATUS_BUSY)
    // Tune_To(TEF668x_Buf, TEF668x_Module_Radio_FM, Tuning_Actions_Mode_Preset, 8930) ;
}   // void Handle_TEF668x (void)

status_t TEF668x_Boot (void) {
    // this procedure takes 350 ms more or less.
    switch(state_boot) {
        case TEF668x_BOOT_STATE_START_STAGE :
            ptr_boot =  (uint8_t *)tuner_init_para_tab ;
            state_boot = TEF668x_BOOT_STATE_INIT_STAGE ;
            break ;
        case TEF668x_BOOT_STATE_INIT_STAGE :
            if (*ptr_boot == 0) {
                return (STATUS_SUCCESS) ;
            }   // if (*ptr_boot == 0)
            else {
                switch (*(ptr_boot+1)) {
                    case INIT_FLAG_TIMER :
                        TimerStop(&tmr_TEF668x) ;
                        TimerSet(&tmr_TEF668x, *(ptr_boot+2)) ;
                        state_boot = TEF668x_BOOT_STATE_WAITFOR_TIMEOUT_STAGE ;
                        break ;
                    case INIT_FLAG_PATCH1 :
                        ptr_patch = (uint8_t *)PatchByteValues ;
                        patch_size = (uint32_t)(sizeof(PatchByteValues)/sizeof (uint8_t)) ;
                        state_boot = TEF668x_BOOT_STATE_LOAD_PATCH_DATA_STAGE ;
                        break ;
                    case INIT_FLAG_PATCH2 :
                        ptr_patch = (uint8_t *)LutByteValues ;
                        patch_size = (uint32_t)(sizeof(LutByteValues)/sizeof (uint8_t)) ;
                        state_boot = TEF668x_BOOT_STATE_LOAD_PATCH_DATA_STAGE ;
                        break ;
                    default :
                        I2C_WriteData(I2C_ADDRESS_TEF668x, ptr_boot+1, *ptr_boot) ;
                        break ;
                }   // switch ()*(ptr_boot+1))
                ptr_boot = ptr_boot + *ptr_boot + 1 ;
            }   // lenth != 0
            break ;
        case TEF668x_BOOT_STATE_WAITFOR_TIMEOUT_STAGE :    // INIT_FLAG_TIMER
            if (TimerHasExpired(&tmr_TEF668x) == TRUE) {
                state_boot = TEF668x_BOOT_STATE_INIT_STAGE ;
            }   // if (TimerHasExpired(&tmr_TEF668x) == TRUE)
            break ;
        case TEF668x_BOOT_STATE_LOAD_PATCH_DATA_STAGE :    // upload patch data
            if (patch_size > 0) {
                uint8_t len ;

                TEF668x_Buf[0] = (0x1BU) ;
                if (patch_size > TEF668X_SPLIT_SIZE) {
                    len = TEF668X_SPLIT_SIZE ;
                }   // if (patch_size > TEF668X_SPLIT_SIZE)
                else {
                    len = patch_size ;
                }   // if (patch_size <= TEF668X_SPLIT_SIZE)
                memcpy (TEF668x_Buf+1, ptr_patch, len) ;

                I2C_WriteData(I2C_ADDRESS_TEF668x, TEF668x_Buf, len+1) ;
                patch_size = patch_size - (uint32_t)len ;
                ptr_patch  = ptr_patch + len ;
            }   //if (patch_size > 0)
            else {
                state_boot = TEF668x_BOOT_STATE_INIT_STAGE ;
            }
            break ;
        default :
            return (STATUS_SUCCESS) ;
            break ;
    }   // switch(state_boot)
    return (STATUS_BUSY) ;
}   // status_t TEF668x_Boot (void)

/*******************************************************************************
 * End of File (TEF668x.c)
 ******************************************************************************/
