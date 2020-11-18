/******************************************************************************
*
* ALLGO Inc.
* (c) Copyright 2020 ALLGO Inc.
* ALL RIGHTS RESERVED.
***************************************************************************//*!
*
* @file		tda75610sep.c
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
#define _TDA75610SEP_INS_
#include "tda75610sep.h"

status_t Handle_PwrAmpCtrl (void) {
    if (handle_pwrampctrl_state < Handle_PwrAmpCtrl_STATE_MAX) {
        (pwr_amp_process[handle_pwrampctrl_state])() ;
    }   // if (handle_pwrampctrl_state < Handle_PwrAmpCtrl_STATE_MAX)

    return (STATUS_SUCCESS) ;
}   // status_t Handle_PwrAmpCtrl (void)

void pwr_amp_wait_i2c (status_t state) {
    /* Unused parameters */
    (void)state;

    NOP() ;
    pwr_amp_wait_state = false ;
}

void pwr_amp_process_init(void) {
    TimerStop(&tmr_TDA75610) ;
    memset(TDA75610_WBuf, 0, sizeof (TDA75610_WBuf)) ;
    memset(TDA75610_RBuf, 0, sizeof (TDA75610_RBuf)) ;
    ptda75610_wreg = (PTDA75610_WREG)TDA75610_WBuf ;
    ptda75610_rreg = (PTDA75610_RREG)TDA75610_RBuf ;
    pwr_amp_wait_state = false ;
    //handle_pwrampctrl_state = Handle_PwrAmpCtrl_STATE_IDLE ;
    handle_pwrampctrl_state = Handle_PwrAmpCtrl_STATE_WRITE ;
    //handle_pwrampctrl_state = Handle_PwrAmpCtrl_STATE_READ ;

    TDA75610_WBuf[WREG_IB1] = (IB1_OP_SupplyTransitionMuteThreshold_High
                              | IB1_OP_Diagnostic_Defeat
                              | IB1_OP_OffsetDetection_Defeat
                              | IB1_OP_FrontChannelGain_26DB
                              | IB1_OP_RearChannelGain_26DB
                              //| IB1_OP_FrontChannels_Mute
                              | IB1_OP_FrontChannels_Unmute
                              //| IB1_OP_RearChannels_Mute
                              | IB1_OP_RearChannels_Unmute
                              | IB1_OP_CD_2Percen
                              ) ;

    TDA75610_WBuf[WREG_IB2] = (IB2_OP_CurrentDetection_LowThreshold
                              | IB2_OP_MutingTime_Fast
                              | IB2_OP_Amplifier_Working
                              | IB2_OP_DiagnosticMode_PowerAmplifier
                              | IB2_OP_CurrentDetectionDiagnostic_Enable
                              | IB2_OP_RChannelPowerAmplifie_HighEfficiencyMode
                              | IB2_OP_LChannelPowerAmplifie_HighEfficiencyMode
                              ) ;

    TimerSet(&tmr_TDA75610, 200) ;
}   // void pwr_amp_process_init(void)

void pwr_amp_process_write(void) {
    if (TimerHasExpired(&tmr_TDA75610) == true) {
        status_t    get_state ;

        TimerStop(&tmr_TDA75610) ;
        NOP() ;
        pwr_amp_wait_state = true ;
        TimerSet(&tmr_TDA75610, 100) ;
        get_state = i2c_masterReqTransceiving ( I2C_SALVE_ADDR_TDA75610
                                              , TDA75610_WBuf
                                              , (uint32_t)MAX_TDA75610_WBuf
                                              , (uint8_t *)NULL
                                              , (uint32_t)NULL
                                              , pwr_amp_wait_i2c
                                              ) ;
        if (get_state == STATUS_SUCCESS) {
            NOP() ;
            handle_pwrampctrl_state = Handle_PwrAmpCtrl_STATE_WRITE_WAIT ;
        }   // if (get_state == STATUS_SUCCESS)
        else {
            NOP() ;
            pwr_amp_wait_state = false ;
            NOP() ;
            TimerSet(&tmr_TDA75610, 300) ;
        }
    }   // if (TimerHasExpired(&tmr_TDA75610) == true)
}   // void pwr_amp_process_write(void)

void pwr_amp_process_write_wait(void) {
    if (pwr_amp_wait_state == false) {
        NOP() ;
        TimerSet(&tmr_TDA75610, 100) ;
        //handle_pwrampctrl_state = Handle_PwrAmpCtrl_STATE_READ ;
        handle_pwrampctrl_state = Handle_PwrAmpCtrl_STATE_IDLE ;
    }   // if (pwr_amp_wait_state == false)
    else if (TimerHasExpired(&tmr_TDA75610) == true) {
        pwr_amp_wait_state = false ;
        TimerSet(&tmr_TDA75610, 100) ;
        handle_pwrampctrl_state = Handle_PwrAmpCtrl_STATE_WRITE ;
    }   // if (TimerHasExpired(&tmr_TDA75610) == true)
}   // void pwr_amp_process_write_wait(void)

void pwr_amp_process_idle(void) {
    NOP() ;
    if (tmr_TDA75610 == 0) {
        TimerSet(&tmr_TDA75610, 20) ;
    }   // if (tmr_TDA75610 == 0)
    else if (TimerHasExpired(&tmr_TDA75610) == true) {
        status_t    ret_state ;

        TimerStop(&tmr_TDA75610) ;

        ret_state = i2c_masterReqTransceiving ( I2C_SALVE_ADDR_TDA75610
                                              , (uint8_t *)NULL
                                              , (uint32_t)NULL
                                              , TDA75610_RBuf
                                              , (uint32_t)MAX_TDA75610_RBuf
                                              , (pvFUNCs)NULL
                                              ) ;
        if (ret_state == STATUS_SUCCESS) {
            TimerSet(&tmr_TDA75610, 100) ;
        }   // if (ret_state == STATUS_SUCCESS)
        else {
            TimerSet(&tmr_TDA75610, 43) ;
        }
    }   // if (TimerHasExpired(&tmr_TDA75610) == true)
}   // void pwr_amp_process_idle(void)

void pwr_amp_process_nop(void) {
}   // void pwr_amp_process_nop(void)

void pwr_amp_SetIB1 (uint8_t set_val) {
    TDA75610_WBuf[WREG_IB1] = set_val ;
}   // void pwr_amp_SetIB1 (uint8_t set_val)

void pwr_amp_SetIB2 (uint8_t set_val) {
    set_val &= ~((uint8_t)BIT_6) ;
    TDA75610_WBuf[WREG_IB2] = set_val ;
}   // void pwr_amp_SetIB2 (uint8_t set_val)

void pwr_amp_SupplyTransitionMuteThreshold (uint8_t set_val) {
    if (set_val == Low_Threshold) {
        ptda75610_wreg->IB1.D7_SupplyTransitionMuteThreshold = Low_Threshold ;
    }   // if (set_val == Low_Threshold)
    else if (set_val == High_Threshold) {
        ptda75610_wreg->IB1.D7_SupplyTransitionMuteThreshold = High_Threshold ;
    }   // if (set_val == High_Threshold)
}   // void pwr_amp_SupplyTransitionMuteThreshold (uint8_t set_val)

void pwr_amp_Diagnostic (uint8_t set_val) {
    if (set_val == Function_Disable) {
        ptda75610_wreg->IB1.D6_Diagnostic = Function_Disable ;
    }   // if (set_val == Function_Disable)
    else if (set_val == Function_Enable) {
        ptda75610_wreg->IB1.D6_Diagnostic = Function_Enable ;
    }   // if (set_val == Function_Enable)
}   // void pwr_amp_Diagnostic (uint8_t set_val)

void pwr_amp_OffsetDetection (uint8_t set_val) {
    if (set_val == Function_Enable) {
        ptda75610_wreg->IB1.D5_OffsetDetection = Function_Enable ;
    }   // if (set_val == Function_Enable)
    else if (set_val == Function_Disable) {
        ptda75610_wreg->IB1.D5_OffsetDetection = Function_Disable ;
    }   // if (set_val == Function_Disable)
}   // void pwr_amp_OffsetDetection (uint8_t set_val)

void pwr_amp_FrontChannelGain (uint8_t set_val) {
    if (set_val == Gain_26DB) {
        ptda75610_wreg->IB1.D4_FrontChanGain = Gain_26DB ;
    }   // if (set_val == Gain_26DB)
    else if (set_val == Gain_16DB) {
        ptda75610_wreg->IB1.D4_FrontChanGain = Gain_16DB ;
    }   // if (set_val == Gain_16DB)
}   // void pwr_amp_FrontChannelGain (uint8_t set_val)

void pwr_amp_RearChannelGain (uint8_t set_val) {
    if (set_val == Gain_26DB) {
        ptda75610_wreg->IB1.D3_RearChanGain = Gain_26DB ;
    }   // if (set_val == Gain_26DB)
    else if (set_val == Gain_16DB) {
        ptda75610_wreg->IB1.D3_RearChanGain = Gain_16DB ;
    }   // if (set_val == Gain_16DB)
}   // void pwr_amp_RearChannelGain (uint8_t set_val)

void pwr_amp_MuteFrontChannels (uint8_t set_val) {
    if (set_val == Function_Enable) {
        ptda75610_wreg->IB1.D2_MuteFrontChannels = Function_Enable ;
    }   // if (set_val == Function_Enable)
    else if (set_val == Function_Disable) {
        ptda75610_wreg->IB1.D2_MuteFrontChannels = Function_Disable ;
    }   // if (set_val == Function_Disable)
}   // void pwr_amp_MuteFrontChannels (uint8_t set_val)

void pwr_amp_MuteRearChannels (uint8_t set_val) {
    if (set_val == Function_Enable) {
        ptda75610_wreg->IB1.D1_MuteRearChannels = Function_Enable ;
    }   // if (set_val == Function_Enable)
    else if (set_val == Function_Disable) {
        ptda75610_wreg->IB1.D1_MuteRearChannels = Function_Disable ;
    }   // if (set_val == Function_Disable)
}   // void pwr_amp_MuteRearChannels (uint8_t set_val)

void pwr_amp_CD (uint8_t set_val) {
    if (set_val == CD_2Percentage) {
        ptda75610_wreg->IB1.D0_CD_LEVEL = CD_2Percentage ;
    }   // if (set_val == CD_2Percentage)
    else if (set_val == CD_10Percentage) {
        ptda75610_wreg->IB1.D0_CD_LEVEL = CD_10Percentage ;
    }   // if (set_val == CD_10Percentage)
}   // void pwr_amp_CD (uint8_t set_val)

void pwr_amp_CurrentDetectionThreshold (uint8_t set_val) {
    if (set_val == Low_Threshold) {
        ptda75610_wreg->IB2.D7_CurrentDetectionThreshold = Low_Threshold ;
    }   // if (set_val == Low_Threshold)
    else if (set_val == High_Threshold) {
        ptda75610_wreg->IB2.D7_CurrentDetectionThreshold = High_Threshold ;
    }   // if (set_val == High_Threshold)
}   // void pwr_amp_CurrentDetectionThreshold (uint8_t set_val)

void pwr_amp_MutingTime (uint8_t set_val) {
    if (set_val == Normal_MutingTime) {
        ptda75610_wreg->IB2.D5_MutingTimeMode = Normal_MutingTime ;
    }   // if (set_val == Normal_MutingTime)
    else if (set_val == Fast_MutingTime) {
        ptda75610_wreg->IB2.D5_MutingTimeMode = Fast_MutingTime ;
    }   // if (set_val == Fast_MutingTime)
}   // void pwr_amp_MutingTime (uint8_t set_val)

void pwr_amp_AmplifierWorkingMode (uint8_t set_val) {
    if (set_val == Standby_On) {
        ptda75610_wreg->IB2.D4_AmplifierWorkingMode = Standby_On ;
    }   // if (set_val == Standby_On)
    else if (set_val == Standby_Off) {
        ptda75610_wreg->IB2.D4_AmplifierWorkingMode = Standby_Off ;
    }   // if (set_val == Standby_Off)
}   // void pwr_amp_AmplifierWorkingMode (uint8_t set_val)

void pwr_amp_DiagnosticMode (uint8_t set_val) {
    if (set_val == Power_Amplifier) {
        ptda75610_wreg->IB2.D3_DiagnosticMode = Power_Amplifier ;
    }   // if (set_val == Power_Amplifier)
    else if (set_val == Line_Drive) {
        ptda75610_wreg->IB2.D3_DiagnosticMode = Line_Drive ;
    }   // if (set_val == Line_Drive)
}   // void pwr_amp_DiagnosticMode (uint8_t set_val)

void pwr_amp_CurrentDetectionDiagnostic (uint8_t set_val) {
    if (set_val == Function_Disable) {
        ptda75610_wreg->IB2.D2_CurrentDetectionDiagnostic = Function_Disable ;
    }   // if (set_val == Function_Disable)
    else if (set_val == Function_Enable) {
        ptda75610_wreg->IB2.D2_CurrentDetectionDiagnostic = Function_Enable ;
    }   // if (set_val == Function_Enable)
}   // void pwr_amp_CurrentDetectionDiagnostic (uint8_t set_val)

void pwr_amp_RightChannelPowerAmplifierWorkingMode (uint8_t set_val) {
    if (set_val == Standard_Mode) {
        ptda75610_wreg->IB2.D1_PwrAmpWorkingMode_RChan = Standard_Mode ;
    }   // if (set_val == Standard_Mode)
    else if (set_val == High_Efficiency_Mode) {
        ptda75610_wreg->IB2.D1_PwrAmpWorkingMode_RChan = High_Efficiency_Mode ;
    }   // if (set_val == High_Efficiency_Mode)
}   // void pwr_amp_RightChannelPowerAmplifierWorkingMode (uint8_t set_val)

void pwr_amp_LeftChannelPowerAmplifierWorkingMode (uint8_t set_val) {
    if (set_val == Standard_Mode) {
        ptda75610_wreg->IB2.D0_PwrAmpWorkingMode_LChan = Standard_Mode ;
    }   // if (set_val == Standard_Mode)
    else if (set_val == High_Efficiency_Mode) {
        ptda75610_wreg->IB2.D0_PwrAmpWorkingMode_LChan = High_Efficiency_Mode ;
    }   // if (set_val == High_Efficiency_Mode)
}   // void pwr_amp_LeftChannelPowerAmplifierWorkingMode (uint8_t set_val)
#undef	_TDA75610SEP_INS_
/******************************************************************************
* end of file (tda75610sep.c)
******************************************************************************/
