/*
 * TEF668x.c
 *
 *  Created on: 2019撟�12���2�
 *      Author: ALLGO
 */


#include "board.h"
#include <stdio.h>
#include <string.h>

#define _RADIO_TEF668X_INS_
#include "TEF668x.h"

void TEF668x_Handle (void) {
    Tune_To(TEF668x_Buf, TEF668x_Module_Radio_FM, Tuning_Actions_Mode_Preset, 8930) ;
}   // void TEF668x_Handle (void)

/*FUNCTION**********************************************************************
 *
 * Function Name : Tune_To
 * Description   : Tuning within the active radio band or tuning to a different
 *                 radio band with selection of FM / AM operation.
 * Parameters    :
 *END**************************************************************************/
uint8_t Tune_To (uint8_t * buf, TEF668x_Module module, Tuning_Actions tuning_action_mode, uint16_t frequency) {
    uint8_t     ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        if (tuning_action_mode == Tuning_Actions_Mode_End) {
            PSetTune_To_End pTune_To ;
            XDATA16         tmp ;

            pTune_To = (PSetTune_To_End)buf ;
            memset (buf, '\0', sizeof (SSetTune_To_End)) ;

            pTune_To->module                    = module ;
            pTune_To->cmd                       = TEF668x_cmd_Tune_To ;
            pTune_To->index                     = 1 ;

            tmp.udata16                         = Tuning_Actions_Mode_End ;
            pTune_To->tuning_actions_hi         = tmp.hibyte ;
            pTune_To->tuning_actions_lo         = tmp.lobyte ;

            ret_val                             = sizeof (SSetTune_To_End) ;
        } else {
            PSetTune_To pTune_To ;
            XDATA16     tmp ;

            pTune_To = (PSetTune_To)buf ;
            memset (buf, '\0', sizeof (SSetTune_To)) ;

            pTune_To->module                    = module ;
            pTune_To->cmd                       = TEF668x_cmd_Tune_To ;
            pTune_To->index                     = 1 ;

            tmp.udata16                         = tuning_action_mode ;
            pTune_To->tuning_actions_hi         = tmp.hibyte ;
            pTune_To->tuning_actions_lo         = tmp.lobyte ;

            tmp.udata16                         = frequency ;
            pTune_To->tuning_frequency_hi       = tmp.hibyte ;
            pTune_To->tuning_frequency_lo       = tmp.lobyte ;

            ret_val                             = sizeof (SSetTune_To) ;
        }
    }

    return (ret_val) ;
}   // void Tune_To (uint8_t * buf, TEF668x_Module module, uint16_t action, uint16_t frequency)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Tune_Options
 * Description   : Settings used during a tuning action (FM AF_Update).
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_Tune_Options (uint8_t * buf, TEF668x_Module module, uint16_t bw_mode, uint16_t bandwidth, uint16_t mute_time, uint16_t sample_time) {
    uint8_t             ret_val = 0 ;

    if (module          == TEF668x_Module_Radio_FM
     && (bw_mode        == AFU_BW_MODE_FIXED            || bw_mode      == AFU_BW_MODE_AUTOMATIC_BANDWIDTH)
     && (bandwidth      >= MIN_AFU_FIXED_IF_BANDWIDTH   && bandwidth    <= MAX_AFU_FIXED_IF_BANDWIDTH)
     && (mute_time      >= MIN_AFU_MUTE_TIME            && mute_time    <= MAX_AFU_MUTE_TIME)
     && (sample_time    >= MIN_AFU_SAMPLE_TIME          && sample_time  <= MAX_AFU_SAMPLE_TIME)
    ) {
        PSet_Tune_Options   pTune_Options ;
        XDATA16             tmp ;

        pTune_Options = (PSet_Tune_Options)buf ;
        memset (buf, '\0', sizeof (SSet_Tune_Options)) ;

        pTune_Options->module               = module ;
        pTune_Options->cmd                  = TEF668x_cmd_Set_Tune_Options ;
        pTune_Options->index                = 1 ;

        tmp.udata16                         = bw_mode ;
        pTune_Options->afu_bw_mode_hi       = tmp.hibyte ;
        pTune_Options->afu_bw_mode_lo       = tmp.lobyte ;

        tmp.udata16                         = bandwidth ;
        pTune_Options->afu_bandwidth_hi     = tmp.hibyte ;
        pTune_Options->afu_bandwidth_lo     = tmp.lobyte ;

        tmp.udata16                         = mute_time ;
        pTune_Options->afu_mute_time_hi     = tmp.hibyte ;
        pTune_Options->afu_mute_time_lo     = tmp.lobyte ;

        tmp.udata16                         = sample_time ;
        pTune_Options->afu_sample_time_hi   = tmp.hibyte ;
        pTune_Options->afu_sample_time_lo   = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Tune_Options) ;
    }

    return (ret_val) ;
}   // uint8_t Set_Tune_Options (uint8_t * buf, TEF668x_Module module, uint16_t bw_mode, uint16_t bandwidth, uint16_t mute_time, uint16_t sample_time)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Bandwidth
 * Description   : Fixed bandwidth selection of the radio selectivity filter.
 *                 For FM automatic bandwidth control can be selected with
 *                 control sensitivity options.
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_Bandwidth(uint8_t * buf, TEF668x_Module module, IF_BandWidth_Control_Mode mode, uint16_t bandwidth, uint16_t control_sensitivity, uint16_t low_level_sensitivity, uint16_t min_bandwidth, uint16_t nominal_bandwidth, uint16_t control_attack) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_Bandwidth_FM   pSet_Bandwidth ;
        XDATA16             tmp ;

        pSet_Bandwidth = (PSet_Bandwidth_FM)buf ;
        memset (buf, '\0', sizeof (SSet_Bandwidth_FM)) ;

        pSet_Bandwidth->module              = TEF668x_Module_Radio_FM ;
        pSet_Bandwidth->cmd                 = TEF668x_cmd_Set_Bandwidth ;
        pSet_Bandwidth->index               = 1 ;

        tmp.udata16                         = mode ;
        pSet_Bandwidth->IF_bandwidth_control_mode_hi = tmp.hibyte ;
        pSet_Bandwidth->IF_bandwidth_control_mode_lo = tmp.lobyte ;

        tmp.udata16                         = bandwidth ;
        pSet_Bandwidth->bandwidth_hi        = tmp.hibyte ;
        pSet_Bandwidth->bandwidth_lo        = tmp.lobyte ;

        tmp.udata16                         = control_sensitivity ;
        pSet_Bandwidth->control_sensitivity_hi = tmp.hibyte ;
        pSet_Bandwidth->control_sensitivity_lo = tmp.lobyte ;

        tmp.udata16                         = low_level_sensitivity ;
        pSet_Bandwidth->low_level_sensitivity_hi = tmp.hibyte ;
        pSet_Bandwidth->low_level_sensitivity_lo = tmp.lobyte ;

        tmp.udata16                         = min_bandwidth ;
        pSet_Bandwidth->min_bandwidth_hi    = tmp.hibyte ;
        pSet_Bandwidth->min_bandwidth_lo    = tmp.lobyte ;

        tmp.udata16                         = nominal_bandwidth ;
        pSet_Bandwidth->min_bandwidth_hi    = tmp.hibyte ;
        pSet_Bandwidth->min_bandwidth_lo    = tmp.lobyte ;

        tmp.udata16                         = nominal_bandwidth ;
        pSet_Bandwidth->nominal_bandwidth_hi= tmp.hibyte ;
        pSet_Bandwidth->nominal_bandwidth_lo= tmp.lobyte ;

        tmp.udata16                         = control_attack ;
        pSet_Bandwidth->control_attack_hi   = tmp.hibyte ;
        pSet_Bandwidth->control_attack_lo   = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Bandwidth_FM) ;
    }   // if (module == TEF668x_Module_Radio_FM)
    else if (module == TEF668x_Module_Radio_AM) {
        PSet_Bandwidth_AM      pSet_Bandwidth ;
        XDATA16             tmp ;

        pSet_Bandwidth = (PSet_Bandwidth_AM)buf ;
        memset (buf, '\0', sizeof (SSet_Bandwidth_AM)) ;

        pSet_Bandwidth->module              = TEF668x_Module_Radio_AM ;
        pSet_Bandwidth->cmd                 = TEF668x_cmd_Set_Bandwidth ;
        pSet_Bandwidth->index               = 1 ;

        tmp.udata16                         = mode ;
        pSet_Bandwidth->IF_bandwidth_control_mode_hi = tmp.hibyte ;
        pSet_Bandwidth->IF_bandwidth_control_mode_lo = tmp.lobyte ;

        tmp.udata16                         = bandwidth ;
        pSet_Bandwidth->bandwidth_hi        = tmp.hibyte ;
        pSet_Bandwidth->bandwidth_lo        = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Bandwidth_AM) ;
    }   // if (module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_Bandwidth(uint8_t * buf, TEF668x_Module module, IF_BandWidth_Control_Mode mode, uint16_t bandwidth, uint16_t control_sensitivity, uint16_t low_level_sensitivity, uint16_t min_bandwidth, uint16_t nominal_bandwidth, uint16_t control_attack)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_RFAGC
 * Description   : Start level of the tuner front-end AGC.
 *                 Performance balancing of desensitization (high start level)
 *                 against inter-modulation (low start level). ptional extension
 *                 of the FM RF AGC range with one step by external application
 *                 use.
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_RFAGC (uint8_t * buf, TEF668x_Module module, uint16_t start, uint16_t extension) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_RFAGC          pSet_RFAGC ;
        XDATA16             tmp ;

        pSet_RFAGC = (PSet_RFAGC)buf ;
        memset (buf, '\0', sizeof (SSet_RFAGC)) ;

        pSet_RFAGC->module                  = module ;
        pSet_RFAGC->cmd                     = TEF668x_cmd_Set_RFAGC ;
        pSet_RFAGC->index                   = 1 ;

        tmp.udata16                         = start ;
        pSet_RFAGC->start_hi                = tmp.hibyte ;
        pSet_RFAGC->start_lo                = tmp.lobyte ;

        tmp.udata16                         = extension ;
        pSet_RFAGC->extension_hi            = tmp.hibyte ;
        pSet_RFAGC->extension_lo            = tmp.lobyte ;

        ret_val                             = sizeof (SSet_RFAGC) ;
    }   // if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_RFAGC (uint8_t * buf, TEF668x_Module module, uint16_t start, uint16_t extension)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Antenna
 * Description   : AM antenna attenuation control (RF AGC attenuation limit).
 *                 In case of an AM active antenna application part of the
 *                 required level correction can be located in the front-end.
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_Antenna (uint8_t * buf, TEF668x_Module module, LNA_Gain_Reduction_LEVEL attenuation) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_AM) {
        PSet_Antenna        pSet_Antenna ;
        XDATA16             tmp ;

        pSet_Antenna = (PSet_Antenna)buf ;
        memset (buf, '\0', sizeof (SSet_Antenna)) ;

        pSet_Antenna->module                = TEF668x_Module_Radio_AM ;
        pSet_Antenna->cmd                   = TEF668x_cmd_Set_Antenna ;
        pSet_Antenna->index                 = 1 ;

        tmp.udata16                         = attenuation ;
        pSet_Antenna->attenuation_hi        = tmp.hibyte ;
        pSet_Antenna->attenuation_lo        = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Antenna) ;
    }   // if (module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_Antenna (uint8_t * buf, TEF668x_Module module, LNA_Gain_Reduction_LEVEL attenuation)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_CoChannelDet
 * Description   : Control of the AM co-channel detector.
 *                 The AM co-channel detector searches for sub-sonic audio content
 *                 as may be found during conditions where the AM channel is
 *                 disturbed by the signal from a different station transmitting
 *                 on the same channel but with a slightly different carrier
 *                 frequency.
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_CoChannelDet (uint8_t * buf, TEF668x_Module module, uint16_t restart, uint16_t sensitivity, uint16_t count) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_AM) {
        PCoChannelDet       pCoChannelDet ;
        XDATA16             tmp ;

        pCoChannelDet = (PCoChannelDet)buf ;
        memset (buf, '\0', sizeof (SCoChannelDet)) ;

        pCoChannelDet->module               = TEF668x_Module_Radio_AM ;
        pCoChannelDet->cmd                  = TEF668x_cmd_Set_CoChannelDet ;
        pCoChannelDet->index                = 1 ;

        tmp.udata16                         = 1 ;
        pCoChannelDet->mode_hi              = tmp.hibyte ;
        pCoChannelDet->mode_lo              = tmp.lobyte ;

        tmp.udata16                         = restart ;
        pCoChannelDet->restart_hi           = tmp.hibyte ;
        pCoChannelDet->restart_lo           = tmp.lobyte ;

        tmp.udata16                         = sensitivity ;
        pCoChannelDet->sensitivity_hi       = tmp.hibyte ;
        pCoChannelDet->sensitivity_lo       = tmp.lobyte ;

        tmp.udata16                         = count ;
        pCoChannelDet->count_hi             = tmp.hibyte ;
        pCoChannelDet->count_lo             = tmp.lobyte ;

        ret_val                             = sizeof (SCoChannelDet) ;
    }   // if (module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_CoChannelDet (uint8_t * buf, TEF668x_Module module, uint16_t restart, uint16_t sensitivity, uint16_t count)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_MphSuppression
 * Description   : Optional use of the �MS�� FM multiple path suppression system.
 *
 * Parameters    : Enable/Disable the Multiple path suppression
 *END**************************************************************************/
uint8_t Set_MphSuppression (uint8_t * buf, TEF668x_Module module, OP_MODE mode) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_MphSuppression pSet_MphSuppression ;
        XDATA16             tmp ;

        pSet_MphSuppression = (PSet_MphSuppression)buf ;
        memset (buf, '\0', sizeof (SSet_MphSuppression)) ;

        pSet_MphSuppression->module         = TEF668x_Module_Radio_FM ;
        pSet_MphSuppression->cmd            = TEF668x_cmd_Set_MphSuppression ;
        pSet_MphSuppression->index          = 1 ;

        tmp.udata16                         = mode ;
        pSet_MphSuppression->mode_hi        = tmp.hibyte ;
        pSet_MphSuppression->mode_lo        = tmp.lobyte ;

        ret_val                             = sizeof (SSet_MphSuppression) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_MphSuppression (uint8_t * buf, TEF668x_Module module, OP_MODE mode)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_ChannelEqualizer
 * Description   : Optional use of the FM channel equalizer.
 *
 * Parameters    : Enable/Disable the channel equalizer
 *END**************************************************************************/
uint8_t Set_ChannelEqualizer (uint8_t * buf, TEF668x_Module module, OP_MODE mode) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_ChannelEqualizer   pSet_ChannelEqualizer ;
        XDATA16                 tmp ;

        pSet_ChannelEqualizer = (PSet_ChannelEqualizer)buf ;
        memset (buf, '\0', sizeof (SSet_ChannelEqualizer)) ;

        pSet_ChannelEqualizer->module       = TEF668x_Module_Radio_FM ;
        pSet_ChannelEqualizer->cmd          = TEF668x_cmd_Set_ChannelEqualizer ;
        pSet_ChannelEqualizer->index        = 1 ;

        tmp.udata16                         = mode ;
        pSet_ChannelEqualizer->mode_hi      = tmp.hibyte ;
        pSet_ChannelEqualizer->mode_lo      = tmp.lobyte ;

        ret_val                             = sizeof (SSet_ChannelEqualizer) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_ChannelEqualizer (uint8_t * buf, TEF668x_Module module, OP_MODE mode)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_NoiseBlanker
 * Description   : Noise blanker options and sensitivity setting.
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_NoiseBlanker (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t sensitivity, uint16_t modulation_gain, uint16_t offset_blank_time, uint16_t attack, uint16_t decay) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_NoiseBlanker_FM    pSet_NoiseBlanker_FM ;
        XDATA16             tmp ;

        pSet_NoiseBlanker_FM = (PSet_NoiseBlanker_FM)buf ;
        memset (buf, '\0', sizeof (SSet_NoiseBlanker_FM)) ;

        pSet_NoiseBlanker_FM->module        = TEF668x_Module_Radio_FM ;
        pSet_NoiseBlanker_FM->cmd           = TEF668x_cmd_Set_NoiseBlanker ;
        pSet_NoiseBlanker_FM->index         = 1 ;

        tmp.udata16                         = mode ;
        pSet_NoiseBlanker_FM->mode_hi       = tmp.hibyte ;
        pSet_NoiseBlanker_FM->mode_lo       = tmp.lobyte ;

        tmp.udata16                         = sensitivity ;
        pSet_NoiseBlanker_FM->sensitivity_hi= tmp.hibyte ;
        pSet_NoiseBlanker_FM->sensitivity_lo= tmp.lobyte ;

        tmp.udata16                         = modulation_gain ;
        pSet_NoiseBlanker_FM->modulation_hi = tmp.hibyte ;
        pSet_NoiseBlanker_FM->modulation_lo = tmp.lobyte ;

        tmp.udata16                         = offset_blank_time ;
        pSet_NoiseBlanker_FM->offset_hi     = tmp.hibyte ;
        pSet_NoiseBlanker_FM->offset_lo     = tmp.lobyte ;

        tmp.udata16                         = attack ;
        pSet_NoiseBlanker_FM->attack_hi     = tmp.hibyte ;
        pSet_NoiseBlanker_FM->attack_lo     = tmp.lobyte ;

        tmp.udata16                         = decay ;
        pSet_NoiseBlanker_FM->decay_hi      = tmp.hibyte ;
        pSet_NoiseBlanker_FM->decay_lo      = tmp.lobyte ;

        ret_val                             = sizeof (SSet_NoiseBlanker_FM) ;
    }   // if (module == TEF668x_Module_Radio_FM)
    else if (module == TEF668x_Module_Radio_AM) {
        PSet_NoiseBlanker_AM    pSet_NoiseBlanker_AM ;
        XDATA16             tmp ;

        pSet_NoiseBlanker_AM = (PSet_NoiseBlanker_AM)buf ;
        memset (buf, '\0', sizeof (SSet_NoiseBlanker_AM)) ;

        pSet_NoiseBlanker_AM->module        = TEF668x_Module_Radio_AM ;
        pSet_NoiseBlanker_AM->cmd           = TEF668x_cmd_Set_NoiseBlanker ;
        pSet_NoiseBlanker_AM->index         = 1 ;

        tmp.udata16                         = mode ;
        pSet_NoiseBlanker_AM->mode_hi       = tmp.hibyte ;
        pSet_NoiseBlanker_AM->mode_lo       = tmp.lobyte ;

        tmp.udata16                         = sensitivity ;
        pSet_NoiseBlanker_AM->sensitivity_hi= tmp.hibyte ;
        pSet_NoiseBlanker_AM->sensitivity_lo= tmp.lobyte ;

        tmp.udata16                         = modulation_gain ;
        pSet_NoiseBlanker_AM->gain_hi       = tmp.hibyte ;
        pSet_NoiseBlanker_AM->gain_lo       = tmp.lobyte ;

        tmp.udata16                         = offset_blank_time ;
        pSet_NoiseBlanker_AM->blank_time_hi = tmp.hibyte ;
        pSet_NoiseBlanker_AM->blank_time_lo = tmp.lobyte ;

        ret_val                             = sizeof (SSet_NoiseBlanker_AM) ;
    }

    return (ret_val) ;
}   // uint8_t Set_NoiseBlanker (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t sensitivity, uint16_t modulation_gain, uint16_t offset_blank_time, uint16_t attack, uint16_t decay)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_NoiseBlanker_Options
 * Description   : Extended API control options of the FM noise blanker.
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_NoiseBlanker_Options (uint8_t * buf, TEF668x_Module module, uint16_t blank_time, uint16_t blank_time2, uint16_t blank_modulation) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_NoiseBlanker_Options   pSet_NoiseBlanker_Options ;
        XDATA16                     tmp ;

        pSet_NoiseBlanker_Options = (PSet_NoiseBlanker_Options)buf ;
        memset (buf, '\0', sizeof (SSet_NoiseBlanker_Options)) ;

        pSet_NoiseBlanker_Options->module   = TEF668x_Module_Radio_FM ;
        pSet_NoiseBlanker_Options->cmd      = TEF668x_cmd_Set_NoiseBlanker_Options ;
        pSet_NoiseBlanker_Options->index    = 1 ;

        tmp.udata16                         = blank_time ;
        pSet_NoiseBlanker_Options->blank_time_hi    = tmp.hibyte ;
        pSet_NoiseBlanker_Options->blank_time_lo    = tmp.lobyte ;

        tmp.udata16                         = blank_time2 ;
        pSet_NoiseBlanker_Options->blank_time2_hi   = tmp.hibyte ;
        pSet_NoiseBlanker_Options->blank_time2_lo   = tmp.lobyte ;

        tmp.udata16                         = blank_modulation ;
        pSet_NoiseBlanker_Options->blank_modulation_hi  = tmp.hibyte ;
        pSet_NoiseBlanker_Options->blank_modulation_lo  = tmp.lobyte ;

        ret_val                             = sizeof (SSet_NoiseBlanker_Options) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_NoiseBlanker_Options (uint8_t * buf, TEF668x_Module module, uint16_t blank_time, uint16_t blank_time2, uint16_t blank_modulation)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_NoiseBlanker_Audio
 * Description   : AM Audio noise blanker options and sensitivity setting.
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_NoiseBlanker_Audio (uint8_t * buf, TEF668x_Module module, OP_MODE mode,
                                uint16_t sensitivity, uint16_t blank_time) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_AM) {
        PSet_NoiseBlanker_Audio     pSet_NoiseBlanker_Audio ;
        XDATA16                     tmp ;

        pSet_NoiseBlanker_Audio = (PSet_NoiseBlanker_Audio)buf ;
        memset (buf, '\0', sizeof (SSet_NoiseBlanker_Audio)) ;

        pSet_NoiseBlanker_Audio->module     = TEF668x_Module_Radio_AM ;
        pSet_NoiseBlanker_Audio->cmd        = TEF668x_cmd_Set_NoiseBlanker_Audio ;
        pSet_NoiseBlanker_Audio->index      = 1 ;

        tmp.udata16                         = mode ;
        pSet_NoiseBlanker_Audio->mode_hi    = tmp.hibyte ;
        pSet_NoiseBlanker_Audio->mode_lo    = tmp.lobyte ;

        tmp.udata16                         = sensitivity ;
        pSet_NoiseBlanker_Audio->sensitivity_hi = tmp.hibyte ;
        pSet_NoiseBlanker_Audio->sensitivity_lo = tmp.lobyte ;

        tmp.udata16                         = blank_time ;
        pSet_NoiseBlanker_Audio->blank_time_hi  = tmp.hibyte ;
        pSet_NoiseBlanker_Audio->blank_time_lo  = tmp.lobyte ;

        ret_val                             = sizeof (SSet_NoiseBlanker_Audio) ;
    }   // if (module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_NoiseBlanker_Audio (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t sensitivity, uint16_t blank_time)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_DigitalRadio
 * Description   : Available for TEF6688 and TEF6689 only.
 *                 Enabling of I/O signal lines for external digital radio processor;
 *                 DR I簡S output and DR Blend input (enabling DR audio input from
 *                 IIS_SD_0).
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_DigitalRadio (uint8_t * buf, TEF668x_Module module, OP_MODE mode) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_AM || module == TEF668x_Module_Radio_FM) {
        PSet_DigitalRadio   pSet_DigitalRadio ;
        XDATA16             tmp ;

        pSet_DigitalRadio = (PSet_DigitalRadio)buf ;
        memset (buf, '\0', sizeof (SSet_DigitalRadio)) ;

        pSet_DigitalRadio->module           = module ;
        pSet_DigitalRadio->cmd              = TEF668x_cmd_Set_DigitalRadio ;
        pSet_DigitalRadio->index            = 1 ;

        tmp.udata16                         = mode ;
        pSet_DigitalRadio->mode_hi          = tmp.hibyte ;
        pSet_DigitalRadio->mode_lo          = tmp.lobyte ;

        ret_val                             = sizeof (SSet_DigitalRadio) ;
    }   // if (module == TEF668x_Module_Radio_AM || module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_DigitalRadio (uint8_t * buf, TEF668x_Module module, OP_MODE mode)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Deemphasis
 * Description   : Selection of FM deemphasis time constant.
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_Deemphasis (uint8_t * buf, TEF668x_Module module, DeemphasisTimeConstant_Mode mode) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_Deemphasis     pSet_Deemphasis ;
        XDATA16             tmp ;

        pSet_Deemphasis = (PSet_Deemphasis)buf ;
        memset (buf, '\0', sizeof (SSet_Deemphasis)) ;

        pSet_Deemphasis->module             = TEF668x_Module_Radio_FM ;
        pSet_Deemphasis->cmd                = TEF668x_cmd_Set_Deemphasis ;
        pSet_Deemphasis->index              = 1 ;

        tmp.udata16                         = mode ;
        pSet_Deemphasis->timeconstant_hi    = tmp.hibyte ;
        pSet_Deemphasis->timeconstant_lo    = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Deemphasis) ;
    }   // if (module == TEF668x_Module_Radio_AM || module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Deemphasis (uint8_t * buf, TEF668x_Module module, DeemphasisTimeConstant_Mode mode)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_StereoImprovement
 * Description   : Available for TEF6687 and TEF6689 only.
 *                 Selection of extended stereo weak signal handling; stereo high
 *                 blend (default operation) or special FMSI (FM stereo improvement)
 *                 blend system for improved sound quality.
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_StereoImprovement (uint8_t * buf, TEF668x_Module module, StereoImprovement_Mode mode) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_StereoImprovement  pSet_StereoImprovement ;
        XDATA16                 tmp ;

        pSet_StereoImprovement = (PSet_StereoImprovement)buf ;
        memset (buf, '\0', sizeof (SSet_StereoImprovement)) ;

        pSet_StereoImprovement->module      = TEF668x_Module_Radio_FM ;
        pSet_StereoImprovement->cmd         = TEF668x_cmd_Set_StereoImprovement ;
        pSet_StereoImprovement->index       = 1 ;

        tmp.udata16                         = mode ;
        pSet_StereoImprovement->mode_hi     = tmp.hibyte ;
        pSet_StereoImprovement->mode_lo     = tmp.lobyte ;

        ret_val                             = sizeof (SSet_StereoImprovement) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_StereoImprovement (uint8_t * buf, TEF668x_Module module, StereoImprovement_Mode mode)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_LevelStep
 * Description   : Selection of level correction as a function of the tuner
 *                 front-end AGC.
 *                 The level step offset is included in the weak signal handling
 *                 and the level read value of Get_Quality
 *                 (4.1 FM / AM cmd 128 / 129   Get_Quality). A setting of 0 dB
 *                 will show no level change by full compensation of the actual
 *                 -6 dB AGC attenuation step. Instead a setting of -6 dB will
 *                 show the actual AGC attenuation step.
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_LevelStep (uint8_t * buf, TEF668x_Module module, int16_t step1, int16_t step2, int16_t step3, int16_t step4, int16_t step5, int16_t step6, int16_t step7) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_LevelStep          pSet_LevelStep ;
        XDATA16                 tmp ;

        pSet_LevelStep = (PSet_LevelStep)buf ;
        memset (buf, '\0', sizeof (SSet_LevelStep)) ;

        pSet_LevelStep->module              = module ;
        pSet_LevelStep->cmd                 = TEF668x_cmd_Set_LevelStep ;
        pSet_LevelStep->index               = 1 ;

        tmp.data16                          = step1 ;
        pSet_LevelStep->step1_hi            = tmp.hibyte ;
        pSet_LevelStep->step1_lo            = tmp.lobyte ;

        tmp.data16                          = step2 ;
        pSet_LevelStep->step2_hi            = tmp.hibyte ;
        pSet_LevelStep->step2_lo            = tmp.lobyte ;

        tmp.data16                          = step3 ;
        pSet_LevelStep->step3_hi            = tmp.hibyte ;
        pSet_LevelStep->step3_lo            = tmp.lobyte ;

        tmp.data16                          = step4 ;
        pSet_LevelStep->step4_hi            = tmp.hibyte ;
        pSet_LevelStep->step4_lo            = tmp.lobyte ;

        tmp.data16                          = step5 ;
        pSet_LevelStep->step5_hi            = tmp.hibyte ;
        pSet_LevelStep->step5_lo            = tmp.lobyte ;

        tmp.data16                          = step6 ;
        pSet_LevelStep->step6_hi            = tmp.hibyte ;
        pSet_LevelStep->step6_lo            = tmp.lobyte ;

        tmp.data16                          = step7 ;
        pSet_LevelStep->step7_hi            = tmp.hibyte ;
        pSet_LevelStep->step7_lo            = tmp.lobyte ;

        ret_val                             = sizeof (SSet_LevelStep) ;
    }   // if (module == TEF668x_Module_Radio_AM || module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_LevelStep (uint8_t * buf, TEF668x_Module module, int16_t step1, int16_t step2, int16_t step3, int16_t step4, int16_t step5, int16_t step6, int16_t step7)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_LevelOffset
 * Description   : Selection of level correction.
 *                 The level offset can be used as an overall correction for
 *                 antenna noise level and is included in the weak signal handling
 *                 and the level read value of Get_Quality (4.1 FM / AM cmd 128
 *                 / 129   Get_Quality). A standard use case is the compensation
 *                 for AM active antenna circuits (typical offset setting = -30 dB).
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_LevelOffset (uint8_t * buf, TEF668x_Module module, int16_t offset) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_LevelOffset        pSet_LevelOffset ;
        XDATA16                 tmp ;

        pSet_LevelOffset = (PSet_LevelOffset)buf ;
        memset (buf, '\0', sizeof (SSet_LevelOffset)) ;

        pSet_LevelOffset->module            = module ;
        pSet_LevelOffset->cmd               = TEF668x_cmd_Set_LevelOffset ;
        pSet_LevelOffset->index             = 1 ;

        tmp.data16                          = offset ;
        pSet_LevelOffset->offset_hi         = tmp.hibyte ;
        pSet_LevelOffset->offset_lo         = tmp.lobyte ;

        ret_val                             = sizeof (SSet_LevelOffset) ;
    }   // if (module == TEF668x_Module_Radio_AM || module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_LevelOffset (uint8_t * buf, TEF668x_Module module, int16_t offset)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Softmute
 * Description   : Timing and quality sensitivity settings for the Softmute weak
 *                 signal handling.
 *                 �et_Softmute_Time�� defines the weak signal handling response
 *                 times active for the level detector and FM noise and multipath
 *                 detectors. Fast and slow response times are available for dual
 *                 timer functionality, with enable options at the level, noise
 *                 and mph commands.
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_Softmute (uint8_t * buf, TEF668x_Module module, uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_Softmute   pSet_Softmute ;
        XDATA16         tmp ;

        pSet_Softmute = (PSet_Softmute)buf ;
        memset (buf, '\0', sizeof (SSet_Softmute)) ;

        pSet_Softmute->module               = module ;
        pSet_Softmute->cmd                  = TEF668x_cmd_Set_Softmute_Time ;
        pSet_Softmute->index                = 1 ;

        tmp.udata16                         = slow_attack ;
        pSet_Softmute->slow_attack_hi       = tmp.hibyte ;
        pSet_Softmute->slow_attack_lo       = tmp.lobyte ;

        tmp.udata16                         = slow_decay ;
        pSet_Softmute->slow_decay_hi        = tmp.hibyte ;
        pSet_Softmute->slow_decay_lo        = tmp.lobyte ;

        tmp.udata16                         = fast_attack ;
        pSet_Softmute->fast_attack_hi       = tmp.hibyte ;
        pSet_Softmute->fast_attack_lo       = tmp.lobyte ;

        tmp.udata16                         = fast_decay ;
        pSet_Softmute->fast_decay_hi        = tmp.hibyte ;
        pSet_Softmute->fast_decay_lo        = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Softmute) ;
    }   // if (module == TEF668x_Module_Radio_AM || module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Softmute (uint8_t * buf, TEF668x_Module module, uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Softmute_Mod
 * Description   : enables modulation dependency and sets sensitivity (AM only).
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_Softmute_Mod (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_AM) {
        PSet_Softmute_Mod   pSet_Softmute_Mod ;
        XDATA16             tmp ;

        pSet_Softmute_Mod = (PSet_Softmute_Mod)buf ;
        memset (buf, '\0', sizeof (SSet_Softmute_Mod)) ;

        pSet_Softmute_Mod->module           = TEF668x_Module_Radio_AM ;
        pSet_Softmute_Mod->cmd              = TEF668x_cmd_Set_Softmute_Mod ;
        pSet_Softmute_Mod->index            = 1 ;

        tmp.udata16                         = mode ;
        pSet_Softmute_Mod->mode_hi          = tmp.hibyte ;
        pSet_Softmute_Mod->mode_lo          = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_Softmute_Mod->start_hi         = tmp.hibyte ;
        pSet_Softmute_Mod->start_lo         = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_Softmute_Mod->slope_hi         = tmp.hibyte ;
        pSet_Softmute_Mod->slope_lo         = tmp.lobyte ;

        tmp.udata16                         = shift ;
        pSet_Softmute_Mod->shift_hi         = tmp.hibyte ;
        pSet_Softmute_Mod->shift_lo         = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Softmute_Mod) ;
    }   // if (module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_Softmute_Mod (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Softmute_Level
 * Description   : sets the level sensitivity and enables slow and fast timing.
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_Softmute_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_AM || module == TEF668x_Module_Radio_FM) {
        PSet_Softmute_Level pSet_Softmute_Level ;
        XDATA16             tmp ;

        pSet_Softmute_Level = (PSet_Softmute_Level)buf ;
        memset (buf, '\0', sizeof (SSet_Softmute_Level)) ;

        pSet_Softmute_Level->module         = module ;
        pSet_Softmute_Level->cmd            = TEF668x_cmd_Set_Softmute_Level ;
        pSet_Softmute_Level->index            = 1 ;

        tmp.udata16                         = mode ;
        pSet_Softmute_Level->mode_hi        = tmp.hibyte ;
        pSet_Softmute_Level->mode_lo        = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_Softmute_Level->start_hi       = tmp.hibyte ;
        pSet_Softmute_Level->start_lo       = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_Softmute_Level->slope_hi       = tmp.hibyte ;
        pSet_Softmute_Level->slope_lo       = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Softmute_Level) ;
    }   // if (module == TEF668x_Module_Radio_AM || module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Softmute_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Softmute_Noise
 * Description   : set the noise and multiple path sensitivity and enables slow
 *                 and fast timing (FM only).
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_Softmute_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_Softmute_Noise pSet_Softmute_Noise ;
        XDATA16             tmp ;

        pSet_Softmute_Noise = (PSet_Softmute_Noise)buf ;
        memset (buf, '\0', sizeof (SSet_Softmute_Noise)) ;

        pSet_Softmute_Noise->module         = TEF668x_Module_Radio_FM ;
        pSet_Softmute_Noise->cmd            = TEF668x_cmd_Set_Softmute_Noise ;
        pSet_Softmute_Noise->index            = 1 ;

        tmp.udata16                         = mode ;
        pSet_Softmute_Noise->mode_hi        = tmp.hibyte ;
        pSet_Softmute_Noise->mode_lo        = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_Softmute_Noise->start_hi       = tmp.hibyte ;
        pSet_Softmute_Noise->start_lo       = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_Softmute_Noise->slope_hi       = tmp.hibyte ;
        pSet_Softmute_Noise->slope_lo       = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Softmute_Noise) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Softmute_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Softmute_Mph
 * Description   : set the noise and multiple path sensitivity and enables slow
 *                 and fast timing (FM only).
 * Parameters    :
 *END**************************************************************************/
uint8_t Set_Softmute_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_Softmute_Mph   pSet_Softmute_Mph ;
        XDATA16             tmp ;

        pSet_Softmute_Mph = (PSet_Softmute_Mph)buf ;
        memset (buf, '\0', sizeof (SSet_Softmute_Mph)) ;

        pSet_Softmute_Mph->module           = TEF668x_Module_Radio_FM ;
        pSet_Softmute_Mph->cmd              = TEF668x_cmd_Set_Softmute_Mph ;
        pSet_Softmute_Mph->index            = 1 ;

        tmp.udata16                         = mode ;
        pSet_Softmute_Mph->mode_hi          = tmp.hibyte ;
        pSet_Softmute_Mph->mode_lo          = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_Softmute_Mph->start_hi         = tmp.hibyte ;
        pSet_Softmute_Mph->start_lo         = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_Softmute_Mph->slope_hi         = tmp.hibyte ;
        pSet_Softmute_Mph->slope_lo         = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Softmute_Mph) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Softmute_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Softmute_Max
 * Description   : enables and defines the maximum amount of softmute attenuation
 *                 (as realized for poor signal conditions)..
 * Parameters    :
 *                 mode : weak signal handling (dynamic control)
 *                        0 : off (for evaluation only)
 *                        1 : on; maximum dynamic control defined by limit parameter (default)
 *                 limit : softmute dynamic attenuation limit
 *                        0 �� 400 [*0.1 dB] = 0 �� 40 dB  softmute maximum attenuation
 *                        200 = 20 dB (FM default) / 250 = 25 dB (AM default)
 *END**************************************************************************/
uint8_t Set_Softmute_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_Softmute_Max   pSet_Softmute_Max ;
        XDATA16             tmp ;

        pSet_Softmute_Max = (PSet_Softmute_Max)buf ;
        memset (buf, '\0', sizeof (SSet_Softmute_Max)) ;

        pSet_Softmute_Max->module           = module ;
        pSet_Softmute_Max->cmd              = TEF668x_cmd_Set_Softmute_Max ;
        pSet_Softmute_Max->index            = 1 ;

        tmp.udata16                         = mode ;
        pSet_Softmute_Max->mode_hi          = tmp.hibyte ;
        pSet_Softmute_Max->mode_lo          = tmp.lobyte ;

        tmp.udata16                         = limit ;
        pSet_Softmute_Max->limit_hi         = tmp.hibyte ;
        pSet_Softmute_Max->limit_lo         = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Softmute_Max) ;
    }   // if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_Softmute_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Highcut_Time
 * Description   : defines the weak signal handling response times active for the
 *                 level detector and FM noise and multiple path detectors.
 *                 Fast and slow response times are available for dual timer
 *                 functionality, with enable options at the level, noise and mph
 *                 commands.��.
 * Parameters    :
 *                 slow_attack : slow attack time of weak signal handling
 *                        60 �� 2000 (ms) = 60 ms �� 2 s slow attack time
 *                        500 = 500 ms (default)
 *                 slow_decay : slow decay time of weak signal handling
 *                        120 �� 12500 (ms) = 120 ms �� 12.5 s slow attack time
 *                        2000 = 2 s (default)
 *                 fast_attack : fast attack time of weak signal handling
 *                        10 �� 1200 (*0.1 ms) = 1 ms �� 120 ms fast attack time
 *                        20 = 2 ms (FM default) / 120 = 12 ms (AM default)
 *                 fast_decay : fast decay time of weak signal handling
 *                        20 �� 5000 (*0.1 ms) = 2 ms �� 500 ms fast attack time
 *                        20 = 2 ms (FM default) / 500 = 50 ms (AM default)
 *END**************************************************************************/
uint8_t Set_Highcut_Time (uint8_t * buf, TEF668x_Module module, uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_Highcut_Time   pSet_Highcut_Time ;
        XDATA16             tmp ;

        pSet_Highcut_Time = (PSet_Highcut_Time)buf ;
        memset (buf, '\0', sizeof (SSet_Highcut_Time)) ;

        pSet_Highcut_Time->module           = module ;
        pSet_Highcut_Time->cmd              = TEF668x_cmd_Set_Highcut_Time ;
        pSet_Highcut_Time->index            = 1 ;

        tmp.udata16                         = slow_attack ;
        pSet_Highcut_Time->slow_attack_hi   = tmp.hibyte ;
        pSet_Highcut_Time->slow_attack_lo   = tmp.lobyte ;

        tmp.udata16                         = slow_decay ;
        pSet_Highcut_Time->slow_decay_hi    = tmp.hibyte ;
        pSet_Highcut_Time->slow_decay_lo    = tmp.lobyte ;

        tmp.udata16                         = fast_attack ;
        pSet_Highcut_Time->fast_attack_hi   = tmp.hibyte ;
        pSet_Highcut_Time->fast_attack_lo   = tmp.lobyte ;

        tmp.udata16                         = fast_decay ;
        pSet_Highcut_Time->fast_decay_hi    = tmp.hibyte ;
        pSet_Highcut_Time->fast_decay_lo    = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Highcut_Time) ;
    }   // if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_Highcut_Time (uint8_t * buf, TEF668x_Module module, uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Highcut_Mod
 * Description   : enables modulation dependency and sets sensitivity.
 * Parameters    :
 *                 mode : slow attack time of weak signal handling
 *                        0 = off (default)
 *                        1 = on (independent modulation timer)
 *                 start : slow decay time of weak signal handling
 *                        100 �� 1000 [*0.1 %] = control when modulation falls below 10% �� 100%
 *                        250 = 25% (default)
 *                        (note: for FM band 100% modulation equals 75 kHz deviation)
 *                 slope : weak signal handling modulation range
 *                        30 �� 1000 (*0.1 %) = control over modulation range of 3% �� 100%
 *                        130 = 13% (default)
 *                 shift : weak signal handling control shift
 *                        50 �� 1000 (*0.1 %) = maximum weak signal control shift of 5% �� 100%
 *                        500 = 50% (default)
 *                        (percentage of the linear control range from _Min limit to _Max limit)
 *END**************************************************************************/
uint8_t Set_Highcut_Mod (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_Highcut_Mod    pSet_Highcut_Mod ;
        XDATA16             tmp ;

        pSet_Highcut_Mod = (PSet_Highcut_Mod)buf ;
        memset (buf, '\0', sizeof (SSet_Highcut_Mod)) ;

        pSet_Highcut_Mod->module            = module ;
        pSet_Highcut_Mod->cmd               = TEF668x_cmd_Set_Highcut_Mod ;
        pSet_Highcut_Mod->index             = 1 ;

        tmp.udata16                         = mode ;
        pSet_Highcut_Mod->mode_hi           = tmp.hibyte ;
        pSet_Highcut_Mod->mode_lo           = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_Highcut_Mod->start_hi          = tmp.hibyte ;
        pSet_Highcut_Mod->start_lo          = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_Highcut_Mod->slope_hi          = tmp.hibyte ;
        pSet_Highcut_Mod->slope_lo          = tmp.lobyte ;

        tmp.udata16                         = shift ;
        pSet_Highcut_Mod->shift_hi          = tmp.hibyte ;
        pSet_Highcut_Mod->shift_lo          = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Highcut_Mod) ;
    }   // if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_Highcut_Mod (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Highcut_Level
 * Description   : enables modulation dependency and sets sensitivity.
 * Parameters    :
 *                 mode : timer selection
 *                        0 = off (only for evaluation)
 *                        1 = fast timer control
 *                        2 = slow timer control (AM default)
 *                        3 = dual timer control; combined fast and slow timer control (FM default)
 *                 start : weak signal handling level start
 *                        200 �� 600 [*0.1 dB�V] = control when level is below 20 dBuV �� 60 dBuV
 *                        360 = 36 dB�V (FM default) / 400 = 40 dB�V (AM default)
 *                 slope : weak signal handling level range
 *                        60 �� 300 [*0.1 dB] = control over level range of 6 dB �� 30 dB
 *                        300 = 30 dB (FM default) / 200 = 20 dB (AM default)
 *END**************************************************************************/
uint8_t Set_Highcut_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_Highcut_Level  pSet_Highcut_Level ;
        XDATA16             tmp ;

        pSet_Highcut_Level = (PSet_Highcut_Level)buf ;
        memset (buf, '\0', sizeof (SSet_Highcut_Level)) ;

        pSet_Highcut_Level->module          = module ;
        pSet_Highcut_Level->cmd             = TEF668x_cmd_Set_Highcut_Level ;
        pSet_Highcut_Level->index           = 1 ;

        tmp.udata16                         = mode ;
        pSet_Highcut_Level->mode_hi         = tmp.hibyte ;
        pSet_Highcut_Level->mode_lo         = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_Highcut_Level->start_hi        = tmp.hibyte ;
        pSet_Highcut_Level->start_lo        = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_Highcut_Level->slope_hi        = tmp.hibyte ;
        pSet_Highcut_Level->slope_lo        = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Highcut_Level) ;
    }   // if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_Highcut_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Highcut_Noise
 * Description   : set the noise and multiple path sensitivity and enables slow
 *                 and fast timing (FM only)
 * Parameters    :
 *                 mode : timer selection
 *                        0 = off (only for evaluation)
 *                        1 = fast timer control
 *                        2 = slow timer control (AM default)
 *                        3 = dual timer control; combined fast and slow timer control (FM default)
 *                 start : FM weak signal handling noise start
 *                        0 �� 800 [*0.1 %] = control when noise above 0�� 80% of USN detector
 *                        360 = 36% (default)
 *                 slope : FM weak signal handling noise range
 *                        100 �� 1000 [*0.1 %] = control over range of 10�� 100% of USN detector
 *                        300 = 30% (default)
 *END**************************************************************************/
uint8_t Set_Highcut_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_Highcut_Level  pSet_Highcut_Level ;
        XDATA16             tmp ;

        pSet_Highcut_Level = (PSet_Highcut_Level)buf ;
        memset (buf, '\0', sizeof (SSet_Highcut_Level)) ;

        pSet_Highcut_Level->module          = module ;
        pSet_Highcut_Level->cmd             = TEF668x_cmd_Set_Highcut_Level ;
        pSet_Highcut_Level->index           = 1 ;

        tmp.udata16                         = mode ;
        pSet_Highcut_Level->mode_hi         = tmp.hibyte ;
        pSet_Highcut_Level->mode_lo         = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_Highcut_Level->start_hi        = tmp.hibyte ;
        pSet_Highcut_Level->start_lo        = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_Highcut_Level->slope_hi        = tmp.hibyte ;
        pSet_Highcut_Level->slope_lo        = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Highcut_Level) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Highcut_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Highcut_Mph
 * Description   : set the noise and multiple path sensitivity and enables slow
 *                 and fast timing (FM only)
 * Parameters    :
 *                 mode : timer selection
 *                        0 = off (only for evaluation)
 *                        1 = fast timer control
 *                        2 = slow timer control (AM default)
 *                        3 = dual timer control; combined fast and slow timer control (FM default)
 *                 start : FM weak signal handling multiple path start
 *                        0 �� 800 [*0.1 %] = control when mph above 0�� 80% of WAM detector
 *                        360 = 36% (default)
 *                 slope : FM weak signal handling multiple path range
 *                        100 �� 1000 [*0.1 %] = control over range of 10�� 100% of WAM detector
 *                        300 = 30% (default)
 *END**************************************************************************/
uint8_t Set_Highcut_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_Highcut_Mph    pSet_Highcut_Mph ;
        XDATA16             tmp ;

        pSet_Highcut_Mph = (PSet_Highcut_Mph)buf ;
        memset (buf, '\0', sizeof (SSet_Highcut_Mph)) ;

        pSet_Highcut_Mph->module            = TEF668x_Module_Radio_FM ;
        pSet_Highcut_Mph->cmd               = TEF668x_cmd_Set_Highcut_Mph ;
        pSet_Highcut_Mph->index             = 1 ;

        tmp.udata16                         = mode ;
        pSet_Highcut_Mph->mode_hi           = tmp.hibyte ;
        pSet_Highcut_Mph->mode_lo           = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_Highcut_Mph->start_hi          = tmp.hibyte ;
        pSet_Highcut_Mph->start_lo          = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_Highcut_Mph->slope_hi          = tmp.hibyte ;
        pSet_Highcut_Mph->slope_lo          = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Highcut_Mph) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Highcut_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Highcut_Max
 * Description   : enables and defines the maximum amount of Highcut attenuation
 *                 (as realized for poor signal conditions)
 * Parameters    :
 *                 mode : weak signal handling (dynamic control)
 *                        0 = off; for evaluation only
 *                        1 = on; maximum dynamic control set by limit parameter (default)
 *                 limit : Highcut attenuation limit
 *                        FM    Highcut corner frequency for maximum -3 dB attenuation
 *                              4000 = 4 kHz (default)
 *                              1500 �� 7000 [*1 Hz] = 1.5 �� 7 kHz �IR�� filter (Options ��1�� (default))
 *                              700 �� 3000 [*1 Hz] = 0.7 �� 3 kHz �eemphasis�� mode (Options ��2��)
 *                              2700 �� 7000 [*1 Hz] = 2.7 �� 7 kHz �IR�� highcut filter (Options ��3��)
 *                        AM    1350 �� 7000 [*1 Hz] = 1.35 �� 7 kHz  Highcut maximum -3 dB att.
 *                              1800 = 1.8 kHz (default)
 *END**************************************************************************/
uint8_t Set_Highcut_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_Highcut_Max    pSet_Highcut_Max ;
        XDATA16             tmp ;

        pSet_Highcut_Max = (PSet_Highcut_Max)buf ;
        memset (buf, '\0', sizeof (SSet_Highcut_Max)) ;

        pSet_Highcut_Max->module            = module ;
        pSet_Highcut_Max->cmd               = TEF668x_cmd_Set_Highcut_Max ;
        pSet_Highcut_Max->index             = 1 ;

        tmp.udata16                         = mode ;
        pSet_Highcut_Max->mode_hi           = tmp.hibyte ;
        pSet_Highcut_Max->mode_lo           = tmp.lobyte ;

        tmp.udata16                         = limit ;
        pSet_Highcut_Max->limit_hi          = tmp.hibyte ;
        pSet_Highcut_Max->limit_lo          = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Highcut_Max) ;
    }   // if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_Highcut_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Highcut_Min
 * Description   : optionally defines a minimum amount of Highcut attenuation (as
 *                 realized for good signal conditions).
 * Parameters    :
 *                 mode : strong signal handling
 *                        0 = off; high audio frequency bandwidth is not limited (FM default)
 *                        1 = on; minimum control limit set by limit parameter (AM default)
 *                 limit : Highcut fixed attenuation limit
 *                        FM    Highcut corner frequency for minimum -3 dB attenuation
 *                              10000 = 10 kHz (default)
 *                              2700 �� 15000 [*1 Hz] = 2.7 �� 15 kHz �IR�� filter (Options ��1�� (default))
 *                              1500 �� 3183 [*1 Hz] = 1.5 �� 3.18 kHz �eemphasis�� (Options ��2��)
 *                              2122 = 75 us deemphasis / 3183 = 50 us deemphasis
 *                              2700 �� 15000 [*1 Hz] = 2.7 �� 15 kHz �IR�� highcut filter (Options ��3��)
 *                        AM    2700 �� 15000 [*1 Hz] = 2.7 �� 15 kHz  -3 dB att. for min. Highcut
 *                              6000 = 6 kHz (default)
 *END**************************************************************************/
uint8_t Set_Highcut_Min (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_Highcut_Min    pSet_Highcut_Min ;
        XDATA16             tmp ;

        pSet_Highcut_Min = (PSet_Highcut_Min)buf ;
        memset (buf, '\0', sizeof (SSet_Highcut_Min)) ;

        pSet_Highcut_Min->module            = module ;
        pSet_Highcut_Min->cmd               = TEF668x_cmd_Set_Highcut_Min ;
        pSet_Highcut_Min->index             = 1 ;

        tmp.udata16                         = mode ;
        pSet_Highcut_Min->mode_hi           = tmp.hibyte ;
        pSet_Highcut_Min->mode_lo           = tmp.lobyte ;

        tmp.udata16                         = limit ;
        pSet_Highcut_Min->limit_hi          = tmp.hibyte ;
        pSet_Highcut_Min->limit_lo          = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Highcut_Min) ;
    }   // if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_Highcut_Min (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Lowcut_Max
 * Description   : enables and defines the maximum dynamic Lowcut attenuation.
 * Parameters    :
 *                 mode : weak signal handling (dynamic control)
 *                        0 = off
 *                        1 =  on; maximum dynamic control defined by limit parameter (default)
 *                 limit : Lowcut dynamic attenuation limit
 *                        30 �� 500 [Hz] = 30 �� 500 Hz  -3 dB attenuation for maximum Lowcut
 *                        120 = 120 Hz (default)
 *END**************************************************************************/
uint8_t Set_Lowcut_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_Lowcut_Max     pSet_Lowcut_Max ;
        XDATA16             tmp ;

        pSet_Lowcut_Max = (PSet_Lowcut_Max)buf ;
        memset (buf, '\0', sizeof (SSet_Lowcut_Max)) ;

        pSet_Lowcut_Max->module            = module ;
        pSet_Lowcut_Max->cmd               = TEF668x_cmd_Set_Lowcut_Max ;
        pSet_Lowcut_Max->index             = 1 ;

        tmp.udata16                         = mode ;
        pSet_Lowcut_Max->mode_hi            = tmp.hibyte ;
        pSet_Lowcut_Max->mode_lo            = tmp.lobyte ;

        tmp.udata16                         = limit ;
        pSet_Lowcut_Max->limit_hi           = tmp.hibyte ;
        pSet_Lowcut_Max->limit_lo           = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Lowcut_Max) ;
    }   // if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_Lowcut_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Lowcut_Min
 * Description   : optionally defines a minimum attenuation for low signal frequencies.
 * Parameters    :
 *                 mode : strong signal handling
 *                        0 = off; low audio frequency bandwidth is not limited (FM default)
 *                        1 = on; minimum control limit set by limit parameter (AM default)
 *                 limit : Lowcut fixed attenuation limit
 *                        10 �� 200 [Hz] = 10 �� 200 Hz  Lowcut minimum -3 dB attenuation
 *                        20 = 20 Hz (default)
 *END**************************************************************************/
uint8_t Set_Lowcut_Min (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_Lowcut_Min     pSet_Lowcut_Min ;
        XDATA16             tmp ;

        pSet_Lowcut_Min = (PSet_Lowcut_Min)buf ;
        memset (buf, '\0', sizeof (SSet_Lowcut_Min)) ;

        pSet_Lowcut_Min->module            = module ;
        pSet_Lowcut_Min->cmd               = TEF668x_cmd_Set_Lowcut_Min ;
        pSet_Lowcut_Min->index             = 1 ;

        tmp.udata16                         = mode ;
        pSet_Lowcut_Min->mode_hi            = tmp.hibyte ;
        pSet_Lowcut_Min->mode_lo            = tmp.lobyte ;

        tmp.udata16                         = limit ;
        pSet_Lowcut_Min->limit_hi           = tmp.hibyte ;
        pSet_Lowcut_Min->limit_lo           = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Lowcut_Min) ;
    }   // if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_Lowcut_Min (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Highcut_Options
 * Description   : optionally defines a minimum attenuation for low signal frequencies.
 * Parameters    :
 *                 mode : FM Highcut control characteristics
 *                        1 = IIR; �nalog�� first order lowpass filter with controlled frequency (default)
 *                        2 = deemphasis; controlled frequency of the 50 / 75 us deemphasis filter
 *                        3 = FIR; �igital�� high order lowpass filter with controlled frequency
 *END**************************************************************************/
uint8_t Set_Highcut_Options (uint8_t * buf, TEF668x_Module module, Highcut_Options_Mode mode) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_Highcut_Options    pSet_Highcut_Options ;
        XDATA16                 tmp ;

        pSet_Highcut_Options = (PSet_Highcut_Options)buf ;
        memset (buf, '\0', sizeof (SSet_Highcut_Options)) ;

        pSet_Highcut_Options->module        = TEF668x_Module_Radio_FM ;
        pSet_Highcut_Options->cmd           = TEF668x_cmd_Set_Highcut_Options ;
        pSet_Highcut_Options->index         = 1 ;

        tmp.udata16                         = mode ;
        pSet_Highcut_Options->mode_hi       = tmp.hibyte ;
        pSet_Highcut_Options->mode_lo       = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Highcut_Options) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Highcut_Options (uint8_t * buf, TEF668x_Module module, Highcut_Options_Mode mode)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Stereo_Time
 * Description   :  defines the weak signal handling response times active for the level
 *                  detector and noise and multipath detectors.
 * Parameters    :
 *                 slow_attack : slow attack time of weak signal handling
 *                        60 �� 2000 (ms) = 60 ms �� 2 s slow attack time
 *                        1000 = 1 s (default)
 *                 slow_decay : slow decay time of weak signal handling
 *                        120 �� 12500 (ms) = 120 ms �� 12.5 s slow attack time
 *                        4000 = 4 s (default)
 *                 fast_attack : fast attack time of weak signal handling
 *                        10 �� 1200 (*0.1 ms) = 1 ms �� 120 ms fast attack time
 *                        80 = 8 ms
 *                 fast_decay : fast decay time of weak signal handling
 *                        20 �� 5000 ( *0.1 ms) = 2 ms �� 500 ms fast attack time
 *                        80 = 8 ms
 *END**************************************************************************/
uint8_t Set_Stereo_Time (uint8_t * buf, TEF668x_Module module, uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_Stereo_Time    pSet_Stereo_Time ;
        XDATA16             tmp ;

        pSet_Stereo_Time = (PSet_Stereo_Time)buf ;
        memset (buf, '\0', sizeof (SSet_Stereo_Time)) ;

        pSet_Stereo_Time->module        = TEF668x_Module_Radio_FM ;
        pSet_Stereo_Time->cmd           = TEF668x_cmd_Set_Stereo_Time ;
        pSet_Stereo_Time->index         = 1 ;

        tmp.udata16                         = slow_attack ;
        pSet_Stereo_Time->slow_attack_hi    = tmp.hibyte ;
        pSet_Stereo_Time->slow_attack_lo    = tmp.lobyte ;

        tmp.udata16                         = slow_decay ;
        pSet_Stereo_Time->slow_decay_hi     = tmp.hibyte ;
        pSet_Stereo_Time->slow_decay_lo     = tmp.lobyte ;

        tmp.udata16                         = fast_attack ;
        pSet_Stereo_Time->fast_attack_hi    = tmp.hibyte ;
        pSet_Stereo_Time->fast_attack_lo    = tmp.lobyte ;

        tmp.udata16                         = fast_decay ;
        pSet_Stereo_Time->fast_decay_hi     = tmp.hibyte ;
        pSet_Stereo_Time->fast_decay_lo     = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Stereo_Time) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Stereo_Time (uint8_t * buf, TEF668x_Module module, uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Stereo_Mod
 * Description   : enables modulation dependency and sets sensitivity.
 * Parameters    :
 *                 mode : modulation dependent weak signal handling
 *                        0 = off (default)
 *                        1 = on (independent modulation timer)
 *                 start : weak signal handling modulation start
 *                        100 �� 1000 [*0.1 %] = control when modulation falls below 10% �� 100%
 *                        210 = 21% (default)
 *                        ( note: for FM band 100% modulation equals 75 kHz deviation )
 *                 slope : weak signal handling modulation range
 *                        30 �� 1000 (*0.1 %) = control over modulation range of 3% �� 100%
 *                        90 = 9% (default)
 *                 shift : weak signal handling control shift
 *                        50 �� 1000 (*0.1 %) = maximum weak signal control shift of 5% �� 100%
 *                        500 = 50% (default)
 *                        (percentage of the linear control range from _Min limit to �ono��)
 *END**************************************************************************/
uint8_t Set_Stereo_Mod (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_Stereo_Mod     pSet_Stereo_Mod ;
        XDATA16             tmp ;

        pSet_Stereo_Mod = (PSet_Stereo_Mod)buf ;
        memset (buf, '\0', sizeof (SSet_Stereo_Mod)) ;

        pSet_Stereo_Mod->module             = TEF668x_Module_Radio_FM ;
        pSet_Stereo_Mod->cmd                = TEF668x_cmd_Set_Stereo_Mod ;
        pSet_Stereo_Mod->index              = 1 ;

        tmp.udata16                         = mode ;
        pSet_Stereo_Mod->mode_hi            = tmp.hibyte ;
        pSet_Stereo_Mod->mode_lo            = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_Stereo_Mod->start_hi           = tmp.hibyte ;
        pSet_Stereo_Mod->start_lo           = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_Stereo_Mod->slope_hi           = tmp.hibyte ;
        pSet_Stereo_Mod->slope_lo           = tmp.lobyte ;

        tmp.udata16                         = shift ;
        pSet_Stereo_Mod->shift_hi           = tmp.hibyte ;
        pSet_Stereo_Mod->shift_lo           = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Stereo_Mod) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Stereo_Mod (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Stereo_Level
 * Description   : sets the level sensitivity and enables slow and fast timing.
 * Parameters    :
 *                 mode : timer selection
 *                        0 = off (only for evaluation)
 *                        1 = fast timer control
 *                        2 = slow timer control
 *                        3 =  dual timer control; combined fast and slow timer control (default)
 *                 start : weak signal handling level start
 *                        300 �� 600 [*0.1 dBuV] = control when level below 30 dB�V �� 60 dBuV
 *                        460 = 46 dB�V (default)
 *                 slope : weak signal handling level range
 *                        60 �� 300 [*0.1 dB] = control over level range of 6 dB �� 30 dB
 *                        240 = 24 dB (default)
 *END**************************************************************************/
uint8_t Set_Stereo_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_Stereo_Level   pSet_Stereo_Level ;
        XDATA16             tmp ;

        pSet_Stereo_Level = (PSet_Stereo_Level)buf ;
        memset (buf, '\0', sizeof (SSet_Stereo_Level)) ;

        pSet_Stereo_Level->module           = TEF668x_Module_Radio_FM ;
        pSet_Stereo_Level->cmd              = TEF668x_cmd_Set_Stereo_Level ;
        pSet_Stereo_Level->index            = 1 ;

        tmp.udata16                         = mode ;
        pSet_Stereo_Level->mode_hi          = tmp.hibyte ;
        pSet_Stereo_Level->mode_lo          = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_Stereo_Level->start_hi         = tmp.hibyte ;
        pSet_Stereo_Level->start_lo         = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_Stereo_Level->slope_hi         = tmp.hibyte ;
        pSet_Stereo_Level->slope_lo         = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Stereo_Level) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Stereo_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Stereo_Noise
 * Description   : set the noise and multiple path sensitivity and enables slow
 *                 and fast timing.
 * Parameters    :
 *                 mode : timer selection
 *                        0 = off (only for evaluation)
 *                        1 = fast timer control
 *                        2 = slow timer control
 *                        3 =  dual timer control; combined fast and slow timer control (default)
 *                 start : FM weak signal handling noise start
 *                        0 �� 800 [*0.1 %] = control when noise above 0�� 80% of USN detector
 *                        240 = 24% (default)
 *                 slope : FM weak signal handling noise range
 *                        100 �� 1000 [*0.1 %] = control over range of 10�� 100% of USN detector
 *                        200 = 20% (default)
 *END**************************************************************************/
uint8_t Set_Stereo_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_Stereo_Noise   pSet_Stereo_Noise ;
        XDATA16             tmp ;

        pSet_Stereo_Noise = (PSet_Stereo_Noise)buf ;
        memset (buf, '\0', sizeof (SSet_Stereo_Noise)) ;

        pSet_Stereo_Noise->module           = TEF668x_Module_Radio_FM ;
        pSet_Stereo_Noise->cmd              = TEF668x_cmd_Set_Stereo_Noise ;
        pSet_Stereo_Noise->index            = 1 ;

        tmp.udata16                         = mode ;
        pSet_Stereo_Noise->mode_hi          = tmp.hibyte ;
        pSet_Stereo_Noise->mode_lo          = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_Stereo_Noise->start_hi         = tmp.hibyte ;
        pSet_Stereo_Noise->start_lo         = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_Stereo_Noise->slope_hi         = tmp.hibyte ;
        pSet_Stereo_Noise->slope_lo         = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Stereo_Noise) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Stereo_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Stereo_Mph
 * Description   : set the noise and multiple path sensitivity and enables slow
 *                 and fast timing.
 * Parameters    :
 *                 mode : timer selection
 *                        0 = off
 *                        1 = fast timer control
 *                        2 = slow timer control
 *                        3 = dual timer control; combined fast and slow timer control (default)
 *                 start : FM weak signal handling multiple path start
 *                        0 �� 800 [*0.1 %] = control when mph above 0�� 80% of WAM detector
 *                        240 = 24% (default)
 *                 slope : FM weak signal handling multiple path range
 *                        100 �� 1000 [*0.1 %] = control over range of 10�� 100% of WAM detector
 *                        200 = 20% (default)
 *END**************************************************************************/
uint8_t Set_Stereo_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_Stereo_Mph     pSet_Stereo_Mph ;
        XDATA16             tmp ;

        pSet_Stereo_Mph = (PSet_Stereo_Mph)buf ;
        memset (buf, '\0', sizeof (SSet_Stereo_Mph)) ;

        pSet_Stereo_Mph->module             = TEF668x_Module_Radio_FM ;
        pSet_Stereo_Mph->cmd                = TEF668x_cmd_Set_Stereo_Mph ;
        pSet_Stereo_Mph->index              = 1 ;

        tmp.udata16                         = mode ;
        pSet_Stereo_Mph->mode_hi            = tmp.hibyte ;
        pSet_Stereo_Mph->mode_lo            = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_Stereo_Mph->start_hi           = tmp.hibyte ;
        pSet_Stereo_Mph->start_lo           = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_Stereo_Mph->slope_hi           = tmp.hibyte ;
        pSet_Stereo_Mph->slope_lo           = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Stereo_Mph) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Stereo_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Stereo_Max
 * Description   : allows disabling the dynamic stereo control for evaluation purposes.
 * Parameters    :
 *                 mode : weak signal handling (dynamic control)
 *                        0 = off (for evaluation only)
 *                        1 = on; maximum dynamic control is 0 dB channel sep, i.e. mono (default)
 *END**************************************************************************/
uint8_t Set_Stereo_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_Stereo_Max     pSet_Stereo_Max ;
        XDATA16             tmp ;

        pSet_Stereo_Max = (PSet_Stereo_Max)buf ;
        memset (buf, '\0', sizeof (SSet_Stereo_Max)) ;

        pSet_Stereo_Max->module             = TEF668x_Module_Radio_FM ;
        pSet_Stereo_Max->cmd                = TEF668x_cmd_Set_Stereo_Max ;
        pSet_Stereo_Max->index              = 1 ;

        tmp.udata16                         = mode ;
        pSet_Stereo_Max->mode_hi            = tmp.hibyte ;
        pSet_Stereo_Max->mode_lo            = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Stereo_Max) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Stereo_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Stereo_Min
 * Description   : optionally defines a minimum amount of Stereo attenuation (as
 *                 realized for good signal conditions).
 * Parameters    :
 *                 mode : strong signal handling
 *                        0 = off; channel separation is not limited (default)
 *                        1 = on; minimum control limit set by limit parameter
 *                        2 = forced mono
 *                 limit : Stereo fixed attenuation limit
 *                        60 ��  400 [0.1* dB] = 6 �� 40 dB  Stereo minimum channel separation
 *                        400 = 40 dB (default)
 *END**************************************************************************/
uint8_t Set_Stereo_Min (uint8_t * buf, TEF668x_Module module, Stereo_Min_MODE mode, uint16_t limit) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_Stereo_Min     pSet_Stereo_Min ;
        XDATA16             tmp ;

        pSet_Stereo_Min = (PSet_Stereo_Min)buf ;
        memset (buf, '\0', sizeof (SSet_Stereo_Min)) ;

        pSet_Stereo_Min->module             = TEF668x_Module_Radio_FM ;
        pSet_Stereo_Min->cmd                = TEF668x_cmd_Set_Stereo_Min ;
        pSet_Stereo_Min->index              = 1 ;

        tmp.udata16                         = mode ;
        pSet_Stereo_Min->mode_hi            = tmp.hibyte ;
        pSet_Stereo_Min->mode_lo            = tmp.lobyte ;

        tmp.udata16                         = limit ;
        pSet_Stereo_Min->limit_hi           = tmp.hibyte ;
        pSet_Stereo_Min->limit_lo           = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Stereo_Min) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Stereo_Min (uint8_t * buf, TEF668x_Module module, Stereo_Min_MODE mode, uint16_t limit)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_StHiBlend_Time
 * Description   : defines the weak signal handling response times active for the
 *                 level detector and noise and multiple path detectors. Fast and
 *                 slow response times are available for dual timer functionality,
 *                 with enable options at the level, noise and mph commands.
 * Parameters    :
 *                 slow_attack : slow attack time of weak signal handling
 *                        60 �� 2000 (ms) = 60 ms �� 2 s slow attack time
 *                        500 = 500 ms (default)
 *                 slow_decay : slow decay time of weak signal handling
 *                        120 �� 12500 (ms) = 120 ms �� 12.5 s slow attack time
 *                        2000 = 2 s (default)
 *                 fast_attack : fast attack time of weak signal handling
 *                        10 �� 1200 (*0.1 ms) = 1 ms �� 120 ms fast attack time
 *                        20 = 2 ms (default)
 *                 fast_decay : fast decay time of weak signal handling
 *                        20 �� 5000 ( *0.1 ms) = 2 ms �� 500 ms fast attack time
 *                        20 = 2 ms (default)
 *END**************************************************************************/
uint8_t Set_StHiBlend_Time (uint8_t * buf, TEF668x_Module module,  uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_StHiBlend_Time pSet_StHiBlend_Time ;
        XDATA16             tmp ;

        pSet_StHiBlend_Time = (PSet_StHiBlend_Time)buf ;
        memset (buf, '\0', sizeof (SSet_StHiBlend_Time)) ;

        pSet_StHiBlend_Time->module             = TEF668x_Module_Radio_FM ;
        pSet_StHiBlend_Time->cmd                = TEF668x_cmd_Set_StHiBlend_Time ;
        pSet_StHiBlend_Time->index              = 1 ;

        tmp.udata16                         = slow_attack ;
        pSet_StHiBlend_Time->slow_attack_hi = tmp.hibyte ;
        pSet_StHiBlend_Time->slow_attack_lo = tmp.lobyte ;

        tmp.udata16                         = slow_decay ;
        pSet_StHiBlend_Time->slow_decay_hi  = tmp.hibyte ;
        pSet_StHiBlend_Time->slow_decay_lo  = tmp.lobyte ;

        tmp.udata16                         = fast_attack ;
        pSet_StHiBlend_Time->fast_attack_hi = tmp.hibyte ;
        pSet_StHiBlend_Time->fast_attack_lo = tmp.lobyte ;

        tmp.udata16                         = fast_decay ;
        pSet_StHiBlend_Time->fast_decay_hi  = tmp.hibyte ;
        pSet_StHiBlend_Time->fast_decay_lo  = tmp.lobyte ;

        ret_val                             = sizeof (SSet_StHiBlend_Time) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_StHiBlend_Time (uint8_t * buf, TEF668x_Module module,  uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_StHiBlend_Mod
 * Description   : enables modulation dependency and sets sensitivity.
 * Parameters    :
 *                 mode : modulation dependent weak signal handling
 *                        0 = off (default)
 *                        1 = on (independent modulation timer)
 *                 start : weak signal handling modulation start
 *                        100 �� 1000 [*0.1 %] = control when modulation falls below 10% �� 100%
 *                        240 = 24% (default)
 *                        ( note: for FM band 100% modulation equals 75 kHz deviation )
 *                 slope : weak signal handling modulation range
 *                        30 �� 1000 (*0.1 %) = control over modulation range of 3% �� 100%
 *                        120 = 12% (default)
 *                 shift : weak signal handling control shift
 *                        50 �� 1000 (*0.1 %) = maximum weak signal control shift of 5% �� 100%
 *                        670 = 67% (default)
 *                        (percentage of the control range from _Min limit to _Max limit)
 *END**************************************************************************/
uint8_t Set_StHiBlend_Mod (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_StHiBlend_Mod  pSet_StHiBlend_Mod ;
        XDATA16             tmp ;

        pSet_StHiBlend_Mod = (PSet_StHiBlend_Mod)buf ;
        memset (buf, '\0', sizeof (SSet_StHiBlend_Mod)) ;

        pSet_StHiBlend_Mod->module          = TEF668x_Module_Radio_FM ;
        pSet_StHiBlend_Mod->cmd             = TEF668x_cmd_Set_StHiBlend_Mod ;
        pSet_StHiBlend_Mod->index           = 1 ;

        tmp.udata16                         = mode ;
        pSet_StHiBlend_Mod->mode_hi         = tmp.hibyte ;
        pSet_StHiBlend_Mod->mode_lo         = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_StHiBlend_Mod->start_hi        = tmp.hibyte ;
        pSet_StHiBlend_Mod->start_lo        = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_StHiBlend_Mod->slope_hi        = tmp.hibyte ;
        pSet_StHiBlend_Mod->slope_lo        = tmp.lobyte ;

        tmp.udata16                         = shift ;
        pSet_StHiBlend_Mod->shift_hi        = tmp.hibyte ;
        pSet_StHiBlend_Mod->shift_lo        = tmp.lobyte ;

        ret_val                             = sizeof (SSet_StHiBlend_Mod) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_StHiBlend_Mod (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_StHiBlend_Level
 * Description   : sets the level sensitivity and enables slow and fast timing.
 * Parameters    :
 *                 mode : timer selection
 *                        0 = off
 *                        1 = fast timer control
 *                        2 = slow timer control
 *                        3 = dual timer control; combined fast and slow timer control (default)
 *                 start : weak signal handling level start
 *                        300 �� 660 [*0.1 dBuV] = control when level below 30 dB�V �� 66 dBuV
 *                        600 = 60 dBuV (default)
 *                 slope : weak signal handling level range
 *                        60 �� 300 [*0.1 dB] = control over level range of 6 dB �� 30 dB
 *                        240 = 24 dB (default)
 *END**************************************************************************/
uint8_t Set_StHiBlend_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_StHiBlend_Level    pSet_StHiBlend_Level ;
        XDATA16                 tmp ;

        pSet_StHiBlend_Level = (PSet_StHiBlend_Level)buf ;
        memset (buf, '\0', sizeof (SSet_StHiBlend_Level)) ;

        pSet_StHiBlend_Level->module        = TEF668x_Module_Radio_FM ;
        pSet_StHiBlend_Level->cmd           = TEF668x_cmd_Set_StHiBlend_Level ;
        pSet_StHiBlend_Level->index         = 1 ;

        tmp.udata16                         = mode ;
        pSet_StHiBlend_Level->mode_hi       = tmp.hibyte ;
        pSet_StHiBlend_Level->mode_lo       = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_StHiBlend_Level->start_hi      = tmp.hibyte ;
        pSet_StHiBlend_Level->start_lo      = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_StHiBlend_Level->slope_hi      = tmp.hibyte ;
        pSet_StHiBlend_Level->slope_lo      = tmp.lobyte ;

        ret_val                             = sizeof (SSet_StHiBlend_Level) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_StHiBlend_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_StHiBlend_Noise
 * Description   : set the noise and multiple path sensitivity and enables slow
 *                 and fast timing.
 * Parameters    :
 *                 mode : timer selection
 *                        0 = off
 *                        1 = fast timer control
 *                        2 = slow timer control
 *                        3 = dual timer control; combined fast and slow timer control (default)
 *                 start : FM weak signal handling noise start
 *                        0 �� 800 [*0.1 %] = control when noise above 0�� 80% of USN detector
 *                        160 = 16% (default)
 *                 slope : FM weak signal handling noise range
 *                        100 �� 1000 [*0.1 %] = control over range of 10�� 100% of USN detector
 *                        140 = 14% (default)
 *END**************************************************************************/
uint8_t Set_StHiBlend_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_StHiBlend_Noise    pSet_StHiBlend_Noise ;
        XDATA16                 tmp ;

        pSet_StHiBlend_Noise = (PSet_StHiBlend_Noise)buf ;
        memset (buf, '\0', sizeof (SSet_StHiBlend_Noise)) ;

        pSet_StHiBlend_Noise->module        = TEF668x_Module_Radio_FM ;
        pSet_StHiBlend_Noise->cmd           = TEF668x_cmd_Set_StHiBlend_Noise ;
        pSet_StHiBlend_Noise->index         = 1 ;

        tmp.udata16                         = mode ;
        pSet_StHiBlend_Noise->mode_hi       = tmp.hibyte ;
        pSet_StHiBlend_Noise->mode_lo       = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_StHiBlend_Noise->start_hi      = tmp.hibyte ;
        pSet_StHiBlend_Noise->start_lo      = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_StHiBlend_Noise->slope_hi      = tmp.hibyte ;
        pSet_StHiBlend_Noise->slope_lo      = tmp.lobyte ;

        ret_val                             = sizeof (SSet_StHiBlend_Noise) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_StHiBlend_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_StHiBlend_Mph
 * Description   : set the noise and multiple path sensitivity and enables slow and
 *                 fast timing.
 * Parameters    :
 *                 mode : timer selection
 *                        0 = off
 *                        1 = fast timer control
 *                        2 = slow timer control
 *                        3 = dual timer control; combined fast and slow timer control (default)
 *                 start : FM weak signal handling multiple path start
 *                        0 �� 800 [*0.1 %] = control when mph above 0�� 80% of WAM detector
 *                        160 = 16% (default)
 *                 slope : FM weak signal handling multiple path range
 *                        100 �� 1000 [*0.1 %] = control over range of 10�� 100% of WAM detector
 *                        140 = 14% (default)
 *END**************************************************************************/
uint8_t Set_StHiBlend_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_StHiBlend_Mph  pSet_StHiBlend_Mph ;
        XDATA16             tmp ;

        pSet_StHiBlend_Mph = (PSet_StHiBlend_Mph)buf ;
        memset (buf, '\0', sizeof (SSet_StHiBlend_Mph)) ;

        pSet_StHiBlend_Mph->module          = TEF668x_Module_Radio_FM ;
        pSet_StHiBlend_Mph->cmd             = TEF668x_cmd_Set_StHiBlend_Mph ;
        pSet_StHiBlend_Mph->index           = 1 ;

        tmp.udata16                         = mode ;
        pSet_StHiBlend_Mph->mode_hi         = tmp.hibyte ;
        pSet_StHiBlend_Mph->mode_lo         = tmp.lobyte ;

        tmp.udata16                         = start ;
        pSet_StHiBlend_Mph->start_hi        = tmp.hibyte ;
        pSet_StHiBlend_Mph->start_lo        = tmp.lobyte ;

        tmp.udata16                         = slope ;
        pSet_StHiBlend_Mph->slope_hi        = tmp.hibyte ;
        pSet_StHiBlend_Mph->slope_lo        = tmp.lobyte ;

        ret_val                             = sizeof (SSet_StHiBlend_Mph) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_StHiBlend_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_StHiBlend_Max
 * Description   : enables and defines the maximum amount of StHiBlend attenuation
 *                 (as realized for very poor signal conditions).
 * Parameters    :
 *                 mode : weak signal handling (dynamic control)
 *                        0 = off
 *                        1 = on; maximum dynamic control defined by limit parameter (default)
 *                 limit : StHiBlend dynamic attenuation limit
 *                       2700 ��  7000 [Hz] = 2.7 kHz �� 7 kHz StHiBlend max.
 *                       reduction of channel separation bandwidth
 *                       4000 = 4 kHz (default)
 *END**************************************************************************/
uint8_t Set_StHiBlend_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_StHiBlend_Max  pSet_StHiBlend_Max ;
        XDATA16             tmp ;

        pSet_StHiBlend_Max = (PSet_StHiBlend_Max)buf ;
        memset (buf, '\0', sizeof (SSet_StHiBlend_Max)) ;

        pSet_StHiBlend_Max->module          = TEF668x_Module_Radio_FM ;
        pSet_StHiBlend_Max->cmd             = TEF668x_cmd_Set_StHiBlend_Max ;
        pSet_StHiBlend_Max->index           = 1 ;

        tmp.udata16                         = mode ;
        pSet_StHiBlend_Max->mode_hi         = tmp.hibyte ;
        pSet_StHiBlend_Max->mode_lo         = tmp.lobyte ;

        tmp.udata16                         = limit ;
        pSet_StHiBlend_Max->limit_hi        = tmp.hibyte ;
        pSet_StHiBlend_Max->limit_lo        = tmp.lobyte ;

        ret_val                             = sizeof (SSet_StHiBlend_Max) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_StHiBlend_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_StHiBlend_Min
 * Description   : optionally defines a minimum amount of StHiBlend attenuation
 *                 (as realized for good signal conditions).
 * Parameters    :
 *                 mode : strong signal handling
 *                        0 = off; channel separation bw is not limited (default)
 *                        1 = on; minimum control limit set by limit parameter
 *                 limit : StHiBlend fixed attenuation limit
 *                       3000 ��  15000 [Hz] = 3 kHz �� 15 kHz StHiBlend min.
 *                       reduction of channel separation bandwidth
 *                       7000 = 7 kHz (default)
 *END**************************************************************************/
uint8_t Set_StHiBlend_Min (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_StHiBlend_Min  pSet_StHiBlend_Min ;
        XDATA16             tmp ;

        pSet_StHiBlend_Min = (PSet_StHiBlend_Min)buf ;
        memset (buf, '\0', sizeof (SSet_StHiBlend_Min)) ;

        pSet_StHiBlend_Min->module          = TEF668x_Module_Radio_FM ;
        pSet_StHiBlend_Min->cmd             = TEF668x_cmd_Set_StHiBlend_Min ;
        pSet_StHiBlend_Min->index           = 1 ;

        tmp.udata16                         = mode ;
        pSet_StHiBlend_Min->mode_hi         = tmp.hibyte ;
        pSet_StHiBlend_Min->mode_lo         = tmp.lobyte ;

        tmp.udata16                         = limit ;
        pSet_StHiBlend_Min->limit_hi        = tmp.hibyte ;
        pSet_StHiBlend_Min->limit_lo        = tmp.lobyte ;

        ret_val                             = sizeof (SSet_StHiBlend_Min) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_StHiBlend_Min (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Scaler
 * Description   : Fine tuning of sound amplitude between FM and AM analog radio
 *                 sound.
 * Parameters    :
 *                 gain : channel gain
 *                        -120 �� +60 [*0.1 dB] = -12 �� +6 dB analog radio signal gain
 *                        0 = 0 dB (default)
 *END**************************************************************************/
uint8_t Set_Scaler (uint8_t * buf, TEF668x_Module module, int16_t gain) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_Scaler pSet_Scaler ;
        XDATA16     tmp ;

        pSet_Scaler = (PSet_Scaler)buf ;
        memset (buf, '\0', sizeof (SSet_Scaler)) ;

        pSet_Scaler->module                 = module ;
        pSet_Scaler->cmd                    = TEF668x_cmd_Set_Scaler ;
        pSet_Scaler->index                  = 1 ;

        tmp.data16                          = gain ;
        pSet_Scaler->gain_hi                = tmp.hibyte ;
        pSet_Scaler->gain_lo                = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Scaler) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_Scaler (uint8_t * buf, TEF668x_Module module, int16_t gain)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_RDS
 * Description   : Fine tuning of sound amplitude between FM and AM analog radio
 *                 sound.
 * Parameters    :
 *                 mode : RDS operation control
 *                        0 = off (RDS function disabled)
 *                        1 = decoder mode (default); output of RDS group data (block A, B, C, D)
 *                            from Get_RDS_Status/Get_RDS_Data; FM cmd = 130/131
 *                        2 = demodulator mode; output of raw demodulator data from
 *                            Get_RDS_Status/Get_RDS_Data; FM cmd = 130/131
 *                 restart : RDS decoder restart
 *                        0 = no control
 *                        1 = manual restart; start looking for new RDS signal immediately
 *                        2 = automatic restart after tuning (default); start looking for new RDS signal
 *                            after Preset, Search, Jump or Check tuning action (see FM cmd = 1)
 *                 interface : RDS pin signal functionality
 *                        0 = no pin interface (default)
 *                        2 = data-available status output; active low  (GPIO feature �AVN��)
 *                        4 = legacy 2-wire demodulator data and clock output (�DDA�� and �DCL��)
 *END**************************************************************************/
uint8_t Set_RDS (uint8_t * buf, TEF668x_Module module, RDS_Control_Mode mode, RDS_RESTART_Mode restart, RDS_PIN_SIGNAL_Type interface) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_RDS    pSet_RDS ;
        XDATA16     tmp ;

        pSet_RDS = (PSet_RDS)buf ;
        memset (buf, '\0', sizeof (SSet_RDS)) ;

        pSet_RDS->module                    = module ;
        pSet_RDS->cmd                       = TEF668x_cmd_Set_RDS ;
        pSet_RDS->index                     = 1 ;

        tmp.udata16                         = mode ;
        pSet_RDS->mode_hi                   = tmp.hibyte ;
        pSet_RDS->mode_lo                   = tmp.lobyte ;

        tmp.udata16                         = restart ;
        pSet_RDS->restart_hi                = tmp.hibyte ;
        pSet_RDS->restart_lo                = tmp.lobyte ;

        tmp.udata16                         = interface ;
        pSet_RDS->interface_hi              = tmp.hibyte ;
        pSet_RDS->interface_lo              = tmp.lobyte ;

        ret_val                             = sizeof (SSet_RDS) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_RDS (uint8_t * buf, TEF668x_Module module, RDS_Control_Mode mode, RDS_RESTART_Mode restart, RDS_PIN_SIGNAL_Type interface)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_QualityStatus
 * Description   : Enable and define interrupt use or status pin output for
 *                 quality detector status flag.
 * Parameters    :
 *                 mode : quality status flag after tuning ready
 *                        0 = no flag set after tuning (default)
 *                        [ 8:0 ] : 10 �� 320 (* 0.1 ms) = set flag at 1 �� 32 ms after tuning ready
 *                        [15] :    1 = set flag when FM AF_Update quality result is available
 *                 interface : quality status pin signal functionality
 *                        0 = no pin interface (default)
 *                        2 = quality status output; active low  (�SI��)
 *END**************************************************************************/
uint8_t Set_QualityStatus (uint8_t * buf, TEF668x_Module module, uint16_t mode, QualityStatus_PIN_SIGNAL_Type interface) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_QualityStatus  pSet_QualityStatus ;
        XDATA16             tmp ;

        pSet_QualityStatus = (PSet_QualityStatus)buf ;
        memset (buf, '\0', sizeof (SSet_QualityStatus)) ;

        pSet_QualityStatus->module          = module ;
        pSet_QualityStatus->cmd             = TEF668x_cmd_Set_QualityStatus ;
        pSet_QualityStatus->index           = 1 ;

        tmp.udata16                         = mode ;
        pSet_QualityStatus->mode_hi         = tmp.hibyte ;
        pSet_QualityStatus->mode_lo         = tmp.lobyte ;

        tmp.udata16                         = interface ;
        pSet_QualityStatus->interface_hi    = tmp.hibyte ;
        pSet_QualityStatus->interface_lo    = tmp.lobyte ;

        ret_val                             = sizeof (SSet_QualityStatus) ;
    }   // if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_QualityStatus (uint8_t * buf, TEF668x_Module module, uint16_t mode, QualityStatus_PIN_SIGNAL_Type interface)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_DR_Blend
 * Description   : Available for TEF6688 and TEF6689 only.
 *                 Control of digital radio blend functionality and digital radio
 *                 scaler.
 * Parameters    :
 *                 mode : blend pin use (DR_BL input)
 *                        0 = Standard pin use : DR Blend pin High = digital radio (default)
 *                        1 = Inverted pin use : DR Blend pin Low = digital radio
 *                        2 = No pin use; Force blend to digital radio
 *                        3 = No pin use; Force blend to analog radio
 *                 in_time : blend time from analog radio to digital radio
 *                        10 �� 5000 [*0.1 ms] = 1 �� 500 ms
 *                        50 = 5 ms (default)
 *                 out_time : blend time from digital radio to analog radio
 *                        10 �� 5000 [*0.1 ms] = 1 �� 500 ms
 *                        50 = 5 ms (default)
 *                 gain : digital radio channel gain
 *                        -180 �� +60 [*0.1 dB] = -18 �� +6 dB  digital radio signal gain
 *                        0 = 0 dB (default)
 *END**************************************************************************/
uint8_t Set_DR_Blend (uint8_t * buf, TEF668x_Module module, uint16_t mode, uint16_t in_time, uint16_t out_time, int16_t gain) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_DR_Blend   pSet_DR_Blend ;
        XDATA16         tmp ;

        pSet_DR_Blend = (PSet_DR_Blend)buf ;
        memset (buf, '\0', sizeof (SSet_DR_Blend)) ;

        pSet_DR_Blend->module               = module ;
        pSet_DR_Blend->cmd                  = TEF668x_cmd_Set_DR_Blend ;
        pSet_DR_Blend->index                = 1 ;

        tmp.udata16                         = mode ;
        pSet_DR_Blend->mode_hi              = tmp.hibyte ;
        pSet_DR_Blend->mode_lo              = tmp.lobyte ;

        tmp.udata16                         = in_time ;
        pSet_DR_Blend->in_time_hi           = tmp.hibyte ;
        pSet_DR_Blend->in_time_lo           = tmp.lobyte ;

        tmp.udata16                         = out_time ;
        pSet_DR_Blend->out_time_hi          = tmp.hibyte ;
        pSet_DR_Blend->out_time_lo          = tmp.lobyte ;

        tmp.data16                          = gain ;
        pSet_DR_Blend->gain_hi              = tmp.hibyte ;
        pSet_DR_Blend->gain_lo              = tmp.lobyte ;

        ret_val                             = sizeof (SSet_DR_Blend) ;
    }   // if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_DR_Blend (uint8_t * buf, TEF668x_Module module, uint16_t mode, uint16_t in_time, uint16_t out_time, int16_t gain)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_DR_Options
 * Description   : For TEF6688 and TEF6689 only.
 *                 Control of digital radio I/O functionality. Note: DR output is
 *                 functional only when digital radio is enabled
 *                 (see 3.13 FM / AM cmd 30   Set_DigitalRadio).
 * Parameters    :
 *                 samplerate : baseband digital radio sample rate (DR_I2S output)
 *                        0 = automatic frequency selection based on tuning frequency (default)
 *                        6500 = 650 kHz (not for normal application use)
 *                        6750 = 675 kHz (not for normal application use)
 *                 mode : baseband digital radio pin mode
 *                        [ 15:8 ] =   BCK and WS pin mode
 *                          34 = standard operation, voltage output (default)
 *                        [ 7:0 ] =  Data pin(s) mode
 *                          2 = voltage output
 *                          4 = open drain (�ull down��) (default)
 *                 format : baseband digital radio format select
 *                        16 = I2S 16 bit, ��3 wire�� interface with single I/Q signal line (DR_I_DATA)
 *                        (f DR_BCK  = 32 * sample rate)
 *                        4112 = I2S 16 bit, ��4 wire�� interface with independent I and Q signal lines
 *                        (f DR_BCK  = 16 * sample rate) (default)
 *END**************************************************************************/
uint8_t Set_DR_Options (uint8_t * buf, TEF668x_Module module, uint16_t samplerate, uint16_t mode, uint16_t format) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_DR_Options pSet_DR_Options ;
        XDATA16         tmp ;

        pSet_DR_Options = (PSet_DR_Options)buf ;
        memset (buf, '\0', sizeof (SSet_DR_Options)) ;

        pSet_DR_Options->module             = module ;
        pSet_DR_Options->cmd                = TEF668x_cmd_Set_DR_Options ;
        pSet_DR_Options->index              = 1 ;

        tmp.udata16                         = samplerate ;
        pSet_DR_Options->samplerate_hi      = tmp.hibyte ;
        pSet_DR_Options->samplerate_lo      = tmp.lobyte ;

        tmp.udata16                         = mode ;
        pSet_DR_Options->mode_hi            = tmp.hibyte ;
        pSet_DR_Options->mode_lo            = tmp.lobyte ;

        tmp.udata16                         = format ;
        pSet_DR_Options->format_hi          = tmp.hibyte ;
        pSet_DR_Options->format_lo          = tmp.lobyte ;

        ret_val                             = sizeof (SSet_DR_Options) ;
    }   // if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_DR_Options (uint8_t * buf, TEF668x_Module module, uint16_t samplerate, uint16_t mode, uint16_t format)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Specials
 * Description   : Special radio options for evaluation and extended application use.
 * Parameters    :
 *                 ana_out : audio output use
 *                        0 = normal operation (default)
 *                        1 = DAC_L : FM MPX wideband (DARC) signal / DAC_R : FM mono audio
 *                        2 = L : digital radio left channel / R : analog radio left channel
 *END**************************************************************************/
uint8_t Set_Specials (uint8_t * buf, TEF668x_Module module, uint16_t ana_out) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_Specials   pSet_Specials ;
        XDATA16         tmp ;

        pSet_Specials = (PSet_Specials)buf ;
        memset (buf, '\0', sizeof (SSet_Specials)) ;

        pSet_Specials->module               = module ;
        pSet_Specials->cmd                  = TEF668x_cmd_Set_Specials ;
        pSet_Specials->index                = 1 ;

        tmp.udata16                         = ana_out ;
        pSet_Specials->ana_out_hi           = tmp.hibyte ;
        pSet_Specials->ana_out_lo           = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Specials) ;
    }   // if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_Specials (uint8_t * buf, TEF668x_Module module, uint16_t ana_out)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Bandwidth_Options
 * Description   : Additional control option of the FM automatic bandwidth radio selectivity filter.
 * Parameters    :
 *                 modulation : extended API: FM automatic bandwidth boost on modulation
 *                        660 �� 1330 (*0.1 %) = 66 �� 133 % modulation (= 50 �� 100 kHz FM dev.)
 *                        Modulation index where bandwidth is boosted to maximum bandwidth.
 *                        950 = 95 % (default)
 *END**************************************************************************/
uint8_t Set_Bandwidth_Options (uint8_t * buf, TEF668x_Module module, uint16_t modulation) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM) {
        PSet_Bandwidth_Options  pSet_Bandwidth_Options ;
        XDATA16                 tmp ;

        pSet_Bandwidth_Options = (PSet_Bandwidth_Options)buf ;
        memset (buf, '\0', sizeof (SSet_Bandwidth_Options)) ;

        pSet_Bandwidth_Options->module      = module ;
        pSet_Bandwidth_Options->cmd         = TEF668x_cmd_Set_Bandwidth_Options ;
        pSet_Bandwidth_Options->index       = 1 ;

        tmp.udata16                         = modulation ;
        pSet_Bandwidth_Options->modulation_hi   = tmp.hibyte ;
        pSet_Bandwidth_Options->modulation_lo   = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Bandwidth_Options) ;
    }   // if (module == TEF668x_Module_Radio_FM || module == TEF668x_Module_Radio_AM)

    return (ret_val) ;
}   // uint8_t Set_Bandwidth_Options (uint8_t * buf, TEF668x_Module module, uint16_t modulation)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_StBandBlend_Time
 * Description   : defines the weak signal handling response times.
 * Parameters    :
 *                 attack : attack time of FMSI weak signal handling
 *                        10 �� 1000 (ms) = 10 ms �� 1 s attack time (control time towards mono)
 *                        50 = 50 ms (default)
 *                 decay : decay time of FMSI weak signal handling
 *                        10 �� 1000 (ms) = 10 ms �� 1 s attack time (control time towards stereo)
 *                        50 = 50 ms (default)
 *END**************************************************************************/
uint8_t Set_StBandBlend_Time (uint8_t * buf, TEF668x_Module module, uint16_t attack, uint16_t decay) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_StBandBlend_Time   pSet_StBandBlend_Time ;
        XDATA16                 tmp ;

        pSet_StBandBlend_Time = (PSet_StBandBlend_Time)buf ;
        memset (buf, '\0', sizeof (SSet_StBandBlend_Time)) ;

        pSet_StBandBlend_Time->module      = TEF668x_Module_Radio_FM ;
        pSet_StBandBlend_Time->cmd         = TEF668x_cmd_Set_StBandBlend_Time ;
        pSet_StBandBlend_Time->index       = 1 ;

        tmp.udata16                         = attack ;
        pSet_StBandBlend_Time->attack_hi    = tmp.hibyte ;
        pSet_StBandBlend_Time->attack_lo    = tmp.lobyte ;

        tmp.udata16                         = decay ;
        pSet_StBandBlend_Time->decay_hi     = tmp.hibyte ;
        pSet_StBandBlend_Time->decay_lo     = tmp.lobyte ;

        ret_val                             = sizeof (SSet_StBandBlend_Time) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_StBandBlend_Time (uint8_t * buf, TEF668x_Module module, uint16_t attack, uint16_t decay)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_StBandBlend_Gain
 * Description   : defines the weak signal handling sensitivity for the four
 *                 available audio bands.
 * Parameters    :
 *                 band1 : control sensitivity for low frequency audio band
 *                        500 �� 1500 [*0.1 %] = 50% �� 150%  weak �� strong control to mono
 *                        1000 = 100% (default)
 *                 band2 : control sensitivity for audio band around 2 kHz
 *                        500 �� 1500 [*0.1 %] = 50% �� 150%  weak �� strong control to mono
 *                        1000 = 100% (default)
 *                 band3 : control sensitivity for audio band around 5 kHz
 *                        500 �� 1500 [*0.1 %] = 50% �� 150%  weak �� strong control to mono
 *                        1000 = 100% (default)
 *                 band4 : control sensitivity for high frequency audio band
 *                        500 �� 1500 [*0.1 %] = 50% �� 150%  weak �� strong control to mono
 *                        1000 = 100% (default)
 *END**************************************************************************/
uint8_t Set_StBandBlend_Gain (uint8_t * buf, TEF668x_Module module, uint16_t band1, uint16_t band2, uint16_t band3, uint16_t band4) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_StBandBlend_Gain   pSet_StBandBlend_Gain ;
        XDATA16                 tmp ;

        pSet_StBandBlend_Gain = (PSet_StBandBlend_Gain)buf ;
        memset (buf, '\0', sizeof (SSet_StBandBlend_Gain)) ;

        pSet_StBandBlend_Gain->module      = TEF668x_Module_Radio_FM ;
        pSet_StBandBlend_Gain->cmd         = TEF668x_cmd_Set_StBandBlend_Gain ;
        pSet_StBandBlend_Gain->index       = 1 ;

        tmp.udata16                         = band1 ;
        pSet_StBandBlend_Gain->band1_hi     = tmp.hibyte ;
        pSet_StBandBlend_Gain->band1_lo     = tmp.lobyte ;

        tmp.udata16                         = band2 ;
        pSet_StBandBlend_Gain->band2_hi     = tmp.hibyte ;
        pSet_StBandBlend_Gain->band2_lo     = tmp.lobyte ;

        tmp.udata16                         = band3 ;
        pSet_StBandBlend_Gain->band3_hi     = tmp.hibyte ;
        pSet_StBandBlend_Gain->band3_lo     = tmp.lobyte ;

        tmp.udata16                         = band4 ;
        pSet_StBandBlend_Gain->band4_hi     = tmp.hibyte ;
        pSet_StBandBlend_Gain->band4_lo     = tmp.lobyte ;

        ret_val                             = sizeof (SSet_StBandBlend_Gain) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_StBandBlend_Gain (uint8_t * buf, TEF668x_Module module, uint16_t band1, uint16_t band2, uint16_t band3, uint16_t band4)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_StBandBlend_Bias
 * Description   : defines the system operation on the condition of good quality
 *                 reception with low modulation.
 * Parameters    :
 *                 band1 : control bias for low frequency audio band
 *                        -250 �� +250 [*0.1 %] = -25% �� +25%  stereo �� mono bias
 *                        -75 = -7.5% (default)
 *                 band2 : control bias for audio band around 2 kHz
 *                        -250 �� +250 [*0.1 %] = -25% �� +25%  stereo �� mono bias
 *                        -35 = -3.5% (default)
 *                 band3 : control bias for audio band around 5 kHz
 *                        -250 �� +250 [*0.1 %] = -25% �� +25%  stereo �� mono bias
 *                        -25 = -2.5% (default)
 *                 band4 : control bias for high frequency audio band
 *                        -250 �� +250 [*0.1 %] = -25% �� +25%  stereo �� mono bias
 *                        -25 = -2.5% (default)
 *END**************************************************************************/
uint8_t Set_StBandBlend_Bias (uint8_t * buf, TEF668x_Module module, int16_t band1, int16_t band2, int16_t band3, int16_t band4) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Radio_FM) {
        PSet_StBandBlend_Bias   pSet_StBandBlend_Bias ;
        XDATA16                 tmp ;

        pSet_StBandBlend_Bias = (PSet_StBandBlend_Bias)buf ;
        memset (buf, '\0', sizeof (SSet_StBandBlend_Bias)) ;

        pSet_StBandBlend_Bias->module       = TEF668x_Module_Radio_FM ;
        pSet_StBandBlend_Bias->cmd          = TEF668x_cmd_Set_StBandBlend_Bias ;
        pSet_StBandBlend_Bias->index        = 1 ;

        tmp.data16                          = band1 ;
        pSet_StBandBlend_Bias->band1_hi     = tmp.hibyte ;
        pSet_StBandBlend_Bias->band1_lo     = tmp.lobyte ;

        tmp.data16                          = band2 ;
        pSet_StBandBlend_Bias->band2_hi     = tmp.hibyte ;
        pSet_StBandBlend_Bias->band2_lo     = tmp.lobyte ;

        tmp.data16                          = band3 ;
        pSet_StBandBlend_Bias->band3_hi     = tmp.hibyte ;
        pSet_StBandBlend_Bias->band3_lo     = tmp.lobyte ;

        tmp.data16                          = band4 ;
        pSet_StBandBlend_Bias->band4_hi     = tmp.hibyte ;
        pSet_StBandBlend_Bias->band4_lo     = tmp.lobyte ;

        ret_val                             = sizeof (SSet_StBandBlend_Bias) ;
    }   // if (module == TEF668x_Module_Radio_FM)

    return (ret_val) ;
}   // uint8_t Set_StBandBlend_Bias (uint8_t * buf, TEF668x_Module module, int16_t band1, int16_t band2, int16_t band3, int16_t band4)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Volume
 * Description   : Setting of audio volume.
 * Parameters    :
 *                 volume : audio volume
 *                        -599 �� +240 = -60 �� +24 dB volume
 *                        0 = 0 dB (default)
 *END**************************************************************************/
uint8_t Set_Volume (uint8_t * buf, TEF668x_Module module, int16_t volume) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Audio) {
        PSet_Volume     pSet_Volume ;
        XDATA16         tmp ;

        pSet_Volume = (PSet_Volume)buf ;
        memset (buf, '\0', sizeof (SSet_Volume)) ;

        pSet_Volume->module                 = TEF668x_Module_Audio ;
        pSet_Volume->cmd                    = TEF668x_cmd_Set_Volume ;
        pSet_Volume->index                  = 1 ;

        tmp.data16                          = volume ;
        pSet_Volume->volume_hi              = tmp.hibyte ;
        pSet_Volume->volume_lo              = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Volume) ;
    }   // if (module == TEF668x_Module_Audio)

    return (ret_val) ;
}   // uint8_t Set_Volume (uint8_t * buf, TEF668x_Module module, int16_t volume)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Input
 * Description   : Enable and disable of the audio mute.
 * Parameters    :
 *                 source : audio source select
 *                        0 = radio (default)
 *                            (analog radio or digital radio when enabled and available)
 *                        32 = I2S digital audio input IIS_SD_0
 *                        240 = sine wave generator
 *END**************************************************************************/
uint8_t Set_Input (uint8_t * buf, TEF668x_Module module, uint16_t source) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Audio) {
        PSet_Input  pSet_Input ;
        XDATA16     tmp ;

        pSet_Input = (PSet_Input)buf ;
        memset (buf, '\0', sizeof (SSet_Input)) ;

        pSet_Input->module                  = TEF668x_Module_Audio ;
        pSet_Input->cmd                     = TEF668x_cmd_Set_Input ;
        pSet_Input->index                   = 1 ;

        tmp.udata16                         = source ;
        pSet_Input->source_hi               = tmp.hibyte ;
        pSet_Input->source_lo               = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Input) ;
    }   // if (module == TEF668x_Module_Audio)

    return (ret_val) ;
}   // uint8_t Set_Input (uint8_t * buf, TEF668x_Module module, uint16_t source)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Output_Source
 * Description   : Output select; selection of source signal for audio output.
 * Parameters    :
 *                 signal : audio output
 *                        33 = I2S digital audio output IIS_SD_1
 *                        128  = DAC L/R output
 *                 source : source
 *                        4 = analog radio
 *                        32 = I2S digital audio input IIS_SD_0
 *                        224 = audio processor (default)
 *                        240 = sine wave generator
 *END**************************************************************************/
uint8_t Set_Output_Source (uint8_t * buf, TEF668x_Module module, uint16_t signal, uint16_t source) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Audio) {
        PSet_Output_Source  pSet_Output_Source ;
        XDATA16             tmp ;

        pSet_Output_Source = (PSet_Output_Source)buf ;
        memset (buf, '\0', sizeof (SSet_Output_Source)) ;

        pSet_Output_Source->module          = TEF668x_Module_Audio ;
        pSet_Output_Source->cmd             = TEF668x_cmd_Set_Output_Source ;
        pSet_Output_Source->index           = 1 ;

        tmp.udata16                         = signal ;
        pSet_Output_Source->signal_hi       = tmp.hibyte ;
        pSet_Output_Source->signal_lo       = tmp.lobyte ;

        tmp.udata16                         = source ;
        pSet_Output_Source->source_hi       = tmp.hibyte ;
        pSet_Output_Source->source_lo       = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Output_Source) ;
    }   // if (module == TEF668x_Module_Audio)

    return (ret_val) ;
}   // uint8_t Set_Output_Source (uint8_t * buf, TEF668x_Module module, uint16_t signal, uint16_t source)


/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Ana_Out
 * Description   : Definition of analog output signals.
 * Parameters    :
 *                 signal : analog audio output
 *                        128 = DAC L/R output
 *                 mode : output mode
 *                        0 = off (power down)
 *                        1 = output enabled (default)
 *END**************************************************************************/
uint8_t Set_Ana_Out (uint8_t * buf, TEF668x_Module module, uint16_t signal, uint16_t mode) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Audio) {
        PSet_Ana_Out    pSet_Ana_Out ;
        XDATA16         tmp ;

        pSet_Ana_Out = (PSet_Ana_Out)buf ;
        memset (buf, '\0', sizeof (SSet_Ana_Out)) ;

        pSet_Ana_Out->module                = TEF668x_Module_Audio ;
        pSet_Ana_Out->cmd                   = TEF668x_cmd_Set_Ana_Out ;
        pSet_Ana_Out->index                 = 1 ;

        tmp.udata16                         = signal ;
        pSet_Ana_Out->signal_hi             = tmp.hibyte ;
        pSet_Ana_Out->signal_lo             = tmp.lobyte ;

        tmp.udata16                         = mode ;
        pSet_Ana_Out->mode_hi               = tmp.hibyte ;
        pSet_Ana_Out->mode_lo               = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Ana_Out) ;
    }   // if (module == TEF668x_Module_Audio)

    return (ret_val) ;
}   // uint8_t Set_Ana_Out (uint8_t * buf, TEF668x_Module module, uint16_t signal, uint16_t mode)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Dig_IO
 * Description   : Definition of digital input and output audio signals.
 * Parameters    :
 *                 signal : digital audio input / output
 *                        32 = I簡S digital audio IIS_SD_0 (input)
 *                        33 = I簡S digital audio IIS_SD_1 (output)
 *                 mode : I/O mode
 *                        0 = off (default)
 *                        1 = input (only available for signal = 32)
 *                        2 = output (only available for signal = 33)
 *                 format : digital audio format select
 *                        16 = I簡S 16 bits (f IIS_BCK  = 32 * samplerate)
 *                        32 = I簡S 32 bits (f IIS_BCK  = 64 * samplerate) (default)
 *                        272 = lsb aligned 16 bit (f IIS_BCK  = 64 * samplerate)
 *                        274 = lsb aligned 18 bit (f IIS_BCK  = 64 * samplerate)
 *                        276 = lsb aligned 20 bit (f IIS_BCK  = 64 * samplerate)
 *                        280 = lsb aligned 24 bit (f IIS_BCK  = 64 * samplerate)
 *                 operation : operation mode
 *                        0 = slave mode; IIS_BCK and IIS_WS input defined by source (default)
 *                        256 = master mode; IIS_BCK and IIS_WS output defined by device
 *                 samplerate : audio sample rate select
 *                        3200 = 32.0 kHz
 *                        4410 = 44.1 kHz (default)
 *                        4800 = 48.0 kHz
 *END**************************************************************************/
uint8_t Set_Dig_IO (uint8_t * buf, TEF668x_Module module, uint16_t signal, uint16_t mode, uint16_t format, uint16_t operation, uint16_t samplerate) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Audio) {
        PSet_Dig_IO     pSet_Dig_IO ;
        XDATA16         tmp ;

        pSet_Dig_IO = (PSet_Dig_IO)buf ;
        memset (buf, '\0', sizeof (SSet_Dig_IO)) ;

        pSet_Dig_IO->module                 = TEF668x_Module_Audio ;
        pSet_Dig_IO->cmd                    = TEF668x_cmd_Set_Dig_IO ;
        pSet_Dig_IO->index                  = 1 ;

        tmp.udata16                         = signal ;
        pSet_Dig_IO->signal_hi              = tmp.hibyte ;
        pSet_Dig_IO->signal_lo              = tmp.lobyte ;

        tmp.udata16                         = mode ;
        pSet_Dig_IO->mode_hi                = tmp.hibyte ;
        pSet_Dig_IO->mode_lo                = tmp.lobyte ;

        tmp.udata16                         = format ;
        pSet_Dig_IO->format_hi              = tmp.hibyte ;
        pSet_Dig_IO->format_lo              = tmp.lobyte ;

        tmp.udata16                         = operation ;
        pSet_Dig_IO->operation_hi           = tmp.hibyte ;
        pSet_Dig_IO->operation_lo           = tmp.lobyte ;

        tmp.udata16                         = samplerate ;
        pSet_Dig_IO->samplerate_hi          = tmp.hibyte ;
        pSet_Dig_IO->samplerate_lo          = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Dig_IO) ;
    }   // if (module == TEF668x_Module_Audio)

    return (ret_val) ;
}   // uint8_t Set_Dig_IO (uint8_t * buf, TEF668x_Module module, uint16_t signal, uint16_t mode, uint16_t format, uint16_t operation, uint16_t samplerate)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_Input_Scaler
 * Description   : Fine tuning of sound amplitude of external sources. For each
 *                 of the available external sources a separate sound amplitude
 *                 correction can be programmed for use when the audio input is
 *                 selected (see 3.32 AUDIO cmd 12 Set_Input).
 * Parameters    :
 *                 source : audio source
 *                        32 = I簡S digital audio input : IIS_SD_0
 *                 gain : external source channel gain
 *                        -120 �� +60 [*0.1 dB] = -12 �� +6 dB external source signal gain
 *                        0 = 0 dB (default)
 *END**************************************************************************/
uint8_t Set_Input_Scaler (uint8_t * buf, TEF668x_Module module, uint16_t source, int16_t gain) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Audio) {
        PSet_Input_Scaler   pSet_Input_Scaler ;
        XDATA16             tmp ;

        pSet_Input_Scaler = (PSet_Input_Scaler)buf ;
        memset (buf, '\0', sizeof (SSet_Input_Scaler)) ;

        pSet_Input_Scaler->module           = TEF668x_Module_Audio ;
        pSet_Input_Scaler->cmd              = TEF668x_cmd_Set_Input_Scaler ;
        pSet_Input_Scaler->index            = 1 ;

        tmp.udata16                         = source ;
        pSet_Input_Scaler->source_hi        = tmp.hibyte ;
        pSet_Input_Scaler->source_lo        = tmp.lobyte ;

        tmp.data16                          = gain ;
        pSet_Input_Scaler->gain_hi          = tmp.hibyte ;
        pSet_Input_Scaler->gain_lo          = tmp.lobyte ;

        ret_val                             = sizeof (SSet_Input_Scaler) ;
    }   // if (module == TEF668x_Module_Audio)

    return (ret_val) ;
}   // uint8_t Set_Input_Scaler (uint8_t * buf, TEF668x_Module module, uint16_t source, int16_t gain)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_WaveGen
 * Description   : Definition of the internal sine wave and offset generator signal.
 *                 The wave generator can be selected as an audio source
 *                 (see 3.32 AUDIO cmd 12 Set_Input) and is intended for test
 *                 purposes only.
 * Parameters    :
 *                 mode : audio source
 *                        0 = wave signal off (default)
 *                        1 = wave 1 signal on Left channel
 *                        2 = wave 2 signal on Right channel
 *                        3 = wave 1 signal on Left channel and wave 2 signal on Right channel
 *                        5 = wave 1 signal on Left and Right channel
 *                        6 = wave 2 signal on Left and Right channel
 *                        7 = wave 1 + wave 2 signal on Left and Right channel
 *                 offset : DC offset
 *                        -32768 �� + 32767 (* 1 LSB of 16 bit) = max negative �� max positive.
 *                        0 = no offset (default)
 *                 amplitude1 : wave 1 amplitude
 *                        -300 �� 0 (*0.1 dB) = -30 �� 0 dB
 *                        -200 = -20 dB (default)
 *                 frequency1 : wave 1 frequency
 *                        10 �� 20000 (*1 Hz) = 10 Hz �� 20 kHz
 *                        400 = 400 Hz (default)
 *                 amplitude2 : wave 2 amplitude
 *                        -300 �� 0 (*0.1 dB) = -30 �� 0 dB
 *                        -200 = -20 dB (default)
 *                 frequency2 : wave 2 frequency
 *                        10 �� 20000 (*1 Hz) = 10 Hz �� 20 kHz
 *                        1000 = 1 kHz (default)
 *END**************************************************************************/
uint8_t Set_WaveGen (uint8_t * buf, TEF668x_Module module, uint16_t mode, int16_t offset, int16_t amplitude1, uint16_t frequency1, int16_t amplitude2, uint16_t frequency2) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_Audio) {
        PSet_WaveGen    pSet_WaveGen ;
        XDATA16         tmp ;

        pSet_WaveGen = (PSet_WaveGen)buf ;
        memset (buf, '\0', sizeof (SSet_WaveGen)) ;

        pSet_WaveGen->module                = TEF668x_Module_Audio ;
        pSet_WaveGen->cmd                   = TEF668x_cmd_Set_WaveGen ;
        pSet_WaveGen->index                 = 1 ;

        tmp.udata16                         = mode ;
        pSet_WaveGen->mode_hi               = tmp.hibyte ;
        pSet_WaveGen->mode_lo               = tmp.lobyte ;

        tmp.data16                          = offset ;
        pSet_WaveGen->offset_hi             = tmp.hibyte ;
        pSet_WaveGen->offset_lo             = tmp.lobyte ;

        tmp.data16                          = amplitude1 ;
        pSet_WaveGen->amplitude1_hi         = tmp.hibyte ;
        pSet_WaveGen->amplitude1_lo         = tmp.lobyte ;

        tmp.udata16                         = frequency1 ;
        pSet_WaveGen->frequency1_hi         = tmp.hibyte ;
        pSet_WaveGen->frequency1_lo         = tmp.lobyte ;

        tmp.data16                          = amplitude2 ;
        pSet_WaveGen->amplitude2_hi         = tmp.hibyte ;
        pSet_WaveGen->amplitude2_lo         = tmp.lobyte ;

        tmp.udata16                         = frequency2 ;
        pSet_WaveGen->frequency2_hi         = tmp.hibyte ;
        pSet_WaveGen->frequency2_lo         = tmp.lobyte ;

        ret_val                             = sizeof (SSet_WaveGen) ;
    }   // if (module == TEF668x_Module_Audio)

    return (ret_val) ;
}   // uint8_t Set_WaveGen (uint8_t * buf, TEF668x_Module module, uint16_t mode, int16_t offset, int16_t amplitude1, uint16_t frequency1, int16_t amplitude2, uint16_t frequency2)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_OperationMode
 * Description   : Device power control.
 * Parameters    :
 *                 mode : device operation mode
 *                        0 = normal operation
 *                        1 = radio standby mode (low-power mode without radio functionality)
 *                        (default)
 *END**************************************************************************/
uint8_t Set_OperationMode (uint8_t * buf, TEF668x_Module module, uint16_t mode) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_APPL) {
        PSet_OperationMode  pSet_OperationMode ;
        XDATA16             tmp ;

        pSet_OperationMode = (PSet_OperationMode)buf ;
        memset (buf, '\0', sizeof (SSet_OperationMode)) ;

        pSet_OperationMode->module          = TEF668x_Module_APPL ;
        pSet_OperationMode->cmd             = TEF668x_cmd_Set_OperationMode ;
        pSet_OperationMode->index           = 1 ;

        tmp.udata16                         = mode ;
        pSet_OperationMode->mode_hi         = tmp.hibyte ;
        pSet_OperationMode->mode_lo         = tmp.lobyte ;

        ret_val                             = sizeof (SSet_OperationMode) ;
    }   // if (module == TEF668x_Module_APPL)

    return (ret_val) ;
}   // uint8_t Set_OperationMode (uint8_t * buf, TEF668x_Module module, uint16_t mode)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_GPIO
 * Description   : Define general purpose and application pin use.
 * Parameters    :
 *                 pin : GPIO number
 *                        0 �� 2 = GPIO number
 *                 module : module
 *                        32 = FM
 *                        33 = AM
 *                 feature : feature
 *                        0 = no use (default) (FM / AM)
 *                        1 = general purpose input (FM / AM)
 *                        2 = general purpose output ��0�� (FM / AM)
 *                        3 = general purpose output ��1�� (FM / AM)
 *                        257 = output RDS (FM : see cmd 81 �AVN��)
 *                        258 = output QSI (FM / AM : see cmd 82 �imer and AF_Update flag��)
 *                        259 = output QSI + RDS (active �ow�� if �AVN�� is active or �SI�� is active)
 *                        260 = output RDDA (FM : see cmd 81 �DDA, RDCL legacy option��)
 *                        261 = output RDCL (FM : see cmd 81 �DDA, RDCL legacy option��)
 *                        262 = output AGC (FM : see cmd 11 �GC step extension��)
 *END**************************************************************************/
uint8_t Set_GPIO (uint8_t * buf, TEF668x_Module module, uint16_t pin, TEF668x_Module op_module, uint16_t feature) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_APPL && (op_module == TEF668x_Module_Radio_FM || op_module == TEF668x_Module_Radio_AM)) {
        PSet_GPIO   pSet_GPIO ;
        XDATA16     tmp ;

        pSet_GPIO = (PSet_GPIO)buf ;
        memset (buf, '\0', sizeof (SSet_GPIO)) ;

        pSet_GPIO->module                   = TEF668x_Module_APPL ;
        pSet_GPIO->cmd                      = TEF668x_cmd_Set_GPIO ;
        pSet_GPIO->index                    = 1 ;

        tmp.udata16                         = pin ;
        pSet_GPIO->pin_hi                   = tmp.hibyte ;
        pSet_GPIO->pin_lo                   = tmp.lobyte ;

        tmp.udata16                         = op_module ;
        pSet_GPIO->module_hi                = tmp.hibyte ;
        pSet_GPIO->module_lo                = tmp.lobyte ;

        tmp.udata16                         = feature ;
        pSet_GPIO->feature_hi               = tmp.hibyte ;
        pSet_GPIO->feature_lo               = tmp.lobyte ;

        ret_val                             = sizeof (SSet_GPIO) ;
    }   // if (module == TEF668x_Module_APPL && (op_module == TEF668x_Module_Radio_FM || op_module == TEF668x_Module_Radio_AM))

    return (ret_val) ;
}   // uint8_t Set_GPIO (uint8_t * buf, TEF668x_Module module, uint16_t pin, TEF668x_Module op_module, uint16_t feature)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_ReferenceClock
 * Description   : This command is only available during �dle state��.
 *                 Several different frequencies can be used for the crystal
 *                 oscillator or the external reference clock. For proper
 *                 functioning the reference frequency must be entered before
 *                 activation of the device, the command is therefore available
 *                 during �dle�� state only.
 * Parameters    :
 *                 frequency_msb : MSB part of the reference clock frequency
 *                        [ 31:16 ]
 *                 frequency_lsb : LSB part of the reference clock frequency
 *                        [ 15:0 ]
 *                        frequency [*1 Hz]
 *                        (default = 9216000 )
 *                 clock_type : clock type
 *                        0 = crystal oscillator operation (default)
 *                        1 = external clock input operation
 *END**************************************************************************/
uint8_t Set_ReferenceClock (uint8_t * buf, TEF668x_Module module, uint16_t frequency_msb, uint16_t frequency_lsb, uint16_t clock_type) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_APPL) {
        PSet_ReferenceClock pSet_ReferenceClock ;
        XDATA16             tmp ;

        pSet_ReferenceClock = (PSet_ReferenceClock)buf ;
        memset (buf, '\0', sizeof (SSet_ReferenceClock)) ;

        pSet_ReferenceClock->module         = TEF668x_Module_APPL ;
        pSet_ReferenceClock->cmd            = TEF668x_cmd_Set_ReferenceClock ;
        pSet_ReferenceClock->index          = 1 ;

        tmp.udata16                         = frequency_msb ;
        pSet_ReferenceClock->frequency_msb_hi = tmp.hibyte ;
        pSet_ReferenceClock->frequency_msb_lo = tmp.lobyte ;

        tmp.udata16                         = frequency_lsb ;
        pSet_ReferenceClock->frequency_lsb_hi = tmp.hibyte ;
        pSet_ReferenceClock->frequency_lsb_lo = tmp.lobyte ;

        tmp.udata16                         = clock_type ;
        pSet_ReferenceClock->type_hi        = tmp.hibyte ;
        pSet_ReferenceClock->type_lo        = tmp.lobyte ;

        ret_val                             = sizeof (SSet_ReferenceClock) ;
    }   // if (module == TEF668x_Module_APPL)

    return (ret_val) ;
}   // uint8_t Set_ReferenceClock (uint8_t * buf, TEF668x_Module module, uint16_t frequency_msb, uint16_t frequency_lsb, uint16_t clock_type)

/*FUNCTION**********************************************************************
 *
 * Function Name : Set_APPL_Activate
 * Description   : This command is only available during �dle state��.
 *                 After the reference clock frequency has been defined the device
 *                 must be put in active state to allow for further initialization
 *                 and control. After activation the device will be in operational
 *                 mode �adio standby��.
 * Parameters    :
 *                 mode :
 *                        1 = goto �ctive�� state with operation mode of �adio standby��
 *END**************************************************************************/
uint8_t Set_APPL_Activate (uint8_t * buf, TEF668x_Module module, uint16_t mode) {
    uint8_t             ret_val = 0 ;

    if (module == TEF668x_Module_APPL) {
        PSet_APPL_Activate  pSet_APPL_Activate ;
        XDATA16             tmp ;

        pSet_APPL_Activate = (PSet_APPL_Activate)buf ;
        memset (buf, '\0', sizeof (SSet_APPL_Activate)) ;

        pSet_APPL_Activate->module          = TEF668x_Module_APPL ;
        pSet_APPL_Activate->cmd             = TEF668x_cmd_Activate ;
        pSet_APPL_Activate->index           = 1 ;

        tmp.udata16                         = mode ;
        pSet_APPL_Activate->mode_hi         = tmp.hibyte ;
        pSet_APPL_Activate->mode_lo         = tmp.lobyte ;

        ret_val                             = sizeof (SSet_APPL_Activate) ;
    }   // if (module == TEF668x_Module_APPL)

    return (ret_val) ;
}   // uint8_t Set_ReferenceClock (uint8_t * buf, TEF668x_Module module, uint16_t frequency_msb, uint16_t frequency_lsb, uint16_t clock_type)

/*******************************************************************************
 * End of File (TEF668x.c)
 ******************************************************************************/
