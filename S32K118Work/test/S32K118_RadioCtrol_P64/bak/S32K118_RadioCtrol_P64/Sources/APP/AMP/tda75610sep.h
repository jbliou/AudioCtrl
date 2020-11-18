/******************************************************************************
*
* ALLGO Inc.
* (c) Copyright 2020 ALLGO Inc.
* ALL RIGHTS RESERVED.
***************************************************************************//*!
*
* @file		tda75610sep.h
*
* @author	ALLGO
*
* @brief	
*
*******************************************************************************
* History:
* 
******************************************************************************/

#ifndef BSW_MCAL_I2C_TDA75610SEP_H_
	#define BSW_MCAL_I2C_TDA75610SEP_H_

    typedef enum {
        Handle_PwrAmpCtrl_STATE_INIT
        , Handle_PwrAmpCtrl_STATE_WRITE
        , Handle_PwrAmpCtrl_STATE_WRITE_WAIT
        , Handle_PwrAmpCtrl_STATE_READ
        , Handle_PwrAmpCtrl_STATE_READ_WAIT
        , Handle_PwrAmpCtrl_STATE_IDLE
        , Handle_PwrAmpCtrl_STATE_MAX
    } HANDLE_PWRAMPCTRL_STATE;

    typedef struct {
        struct {
            uint8_t     D0_CD_LEVEL                         : 1 ;       // D0
            uint8_t     D1_MuteRearChannels                 : 1 ;       // D1
            uint8_t     D2_MuteFrontChannels                : 1 ;       // D2
            uint8_t     D3_RearChanGain                     : 1 ;       // D3 : Channel 2 and 4
            uint8_t     D4_FrontChanGain                    : 1 ;       // D4 : Channel 1 and 3
            uint8_t     D5_OffsetDetection                  : 1 ;       // D5
            uint8_t     D6_Diagnostic                       : 1 ;       // D6
            uint8_t     D7_SupplyTransitionMuteThreshold    : 1 ;       // D7
        } IB1 ;
        struct {
            uint8_t     D0_PwrAmpWorkingMode_LChan          : 1 ;       // D0
            uint8_t     D1_PwrAmpWorkingMode_RChan          : 1 ;       // D1
            uint8_t     D2_CurrentDetectionDiagnostic       : 1 ;       // D2
            uint8_t     D3_DiagnosticMode                   : 1 ;       // D3
            uint8_t     D4_AmplifierWorkingMode             : 1 ;       // D4
            uint8_t     D5_MutingTimeMode                   : 1 ;       // D5
            uint8_t     dummy                               : 1 ;       // D6
            uint8_t     D7_CurrentDetectionThreshold        : 1 ;       // D7
        } IB2 ;
    } STDA75610_WREG, * PTDA75610_WREG ;


    typedef struct {
        struct {
            uint8_t     D0_ChanLFShortToGndState            : 1 ;       // D0
            uint8_t     D1_ChanLFShortToVccState            : 1 ;       // D1
            uint8_t     D2_ChanLFDiagnosticState            : 1 ;       // D2
            uint8_t     D3_ChanLFLoadState                  : 1 ;       // D3
            uint8_t     D4_ChanLFDiagnosticType             : 1 ;       // D4
            uint8_t     D5_ChanLFOutputPeakCurrentState     : 1 ;       // D5
            uint8_t     D6_DiagnosticCycle                  : 1 ;       // D6
            uint8_t     D7_ThermalWarning1                  : 1 ;       // D7
        } DB1 ;
        struct {
            uint8_t     D0_ChanLRShortToGndState            : 1 ;       // D0
            uint8_t     D1_ChanLRShortToVccState            : 1 ;       // D1
            uint8_t     D2_ChanLRDiagnosticState            : 1 ;       // D2
            uint8_t     D3_ChanLRLoadState                  : 1 ;       // D3
            uint8_t     D4_ChanLRDiagnosticType             : 1 ;       // D4
            uint8_t     D5_ChanLROutputPeakCurrentState     : 1 ;       // D5
            uint8_t     dummy                               : 1 ;       // D6
            uint8_t     D7_OffsetDetectionState             : 1 ;       // D7
        } DB2 ;
        struct {
            uint8_t     D0_ChanRFShortToGndState            : 1 ;       // D0
            uint8_t     D1_ChanRFShortToVccState            : 1 ;       // D1
            uint8_t     D2_ChanRFDiagnosticState            : 1 ;       // D2
            uint8_t     D3_ChanRFLoadState                  : 1 ;       // D3
            uint8_t     D4_ChanRFDiagnosticType             : 1 ;       // D4
            uint8_t     D5_ChanRFOutputPeakCurrentState     : 1 ;       // D5
            uint8_t     D6_DiagnosticStatus                 : 1 ;       // D6 : = IB1 - D6
            uint8_t     D7_StandbyStatus                    : 1 ;       // D7 : = IB2 - D4
        } DB3 ;
        struct {
            uint8_t     D0_ChanRRShortToGndState            : 1 ;       // D0
            uint8_t     D1_ChanRRShortToVccState            : 1 ;       // D1
            uint8_t     D2_ChanRRDiagnosticState            : 1 ;       // D2
            uint8_t     D3_ChanRRLoadState                  : 1 ;       // D3
            uint8_t     D4_ChanRRDiagnosticType             : 1 ;       // D4
            uint8_t     D5_ChanRROutputPeakCurrentState     : 1 ;       // D5
            uint8_t     D6_ThermalWarning3                  : 1 ;       // D6
            uint8_t     D7_ThermalWarning2                  : 1 ;       // D7
        } DB4 ;
    } STDA75610_RREG, * PTDA75610_RREG ;

    #define I2C_SALVE_ADDR_TDA75610         (0xd8/2)

    #define MAX_TDA75610_WBuf               (2U)
    #define MAX_TDA75610_RBuf               (4U)

    typedef enum {
        Low_Threshold                       = 0U
        , High_Threshold                    = 1U
        , Function_Disable                  = 0U
        , Function_Enable                   = 1U
        , Gain_26DB                         = 0U
        , Gain_16DB                         = 1U
        , CD_2Percentage                    = 0U
        , CD_10Percentage                   = 1U
        , Normal_MutingTime                 = 0U
        , Fast_MutingTime                   = 1U
        , Standby_On                        = 0U
        , Standby_Off                       = 1U
        , Power_Amplifier                   = 0U
        , Line_Drive                        = 1U
        , Standard_Mode                     = 0U
        , High_Efficiency_Mode              = 1U
    } TDA75610_SETTING ;

    #define IB1_OP_SupplyTransitionMuteThreshold_High           (BIT_7)
    #define IB1_OP_SupplyTransitionMuteThreshold_Low            (0U)

    #define IB1_OP_Diagnostic_Enable                            (BIT_6)
    #define IB1_OP_Diagnostic_Defeat                            (0U)

    #define IB1_OP_OffsetDetection_Enable                       (BIT_5)
    #define IB1_OP_OffsetDetection_Defeat                       (0U)

    #define IB1_OP_FrontChannelGain_16DB                        (BIT_4)
    #define IB1_OP_FrontChannelGain_26DB                        (0U)

    #define IB1_OP_RearChannelGain_16DB                         (BIT_3)
    #define IB1_OP_RearChannelGain_26DB                         (0U)

    #define IB1_OP_FrontChannels_Unmute                         (BIT_2)
    #define IB1_OP_FrontChannels_Mute                           (0)

    #define IB1_OP_RearChannels_Unmute                          (BIT_1)
    #define IB1_OP_RearChannels_Mute                            (0)

    #define IB1_OP_CD_10Percen                                  (BIT_0)
    #define IB1_OP_CD_2Percen                                   (0U)

    #define IB2_OP_CurrentDetection_LowThreshold                (BIT_7)
    #define IB2_OP_CurrentDetection_HighThreshold               (0U)

    #define IB2_OP_MutingTime_Fast                              (BIT_5)
    #define IB2_OP_MutingTime_Normal                            (0U)

    #define IB2_OP_Amplifier_Working                            (BIT_4)
    #define IB2_OP_Amplifier_NotWorking                         (0U)

    #define IB2_OP_DiagnosticMode_LineDriver                    (BIT_3)
    #define IB2_OP_DiagnosticMode_PowerAmplifier                (0U)

    #define IB2_OP_CurrentDetectionDiagnostic_Enable            (BIT_2)
    #define IB2_OP_CurrentDetectionDiagnostic_Defeat            (0U)

    #define IB2_OP_RChannelPowerAmplifie_HighEfficiencyMode     (BIT_1)
    #define IB2_OP_RChannelPowerAmplifie_StandardMode           (0U)

    #define IB2_OP_LChannelPowerAmplifie_HighEfficiencyMode     (BIT_0)
    #define IB2_OP_LChannelPowerAmplifie_StandardMode           (0U)

    #define WREG_IB1                                            (0U)
    #define WREG_IB2                                            (1U)
#endif	/* BSW_MCAL_I2C_TDA75610SEP_H_ */

#if defined(_TDA75610SEP_INS_)
    HANDLE_PWRAMPCTRL_STATE handle_pwrampctrl_state = Handle_PwrAmpCtrl_STATE_INIT ;
    uint8_t                 TDA75610_WBuf [MAX_TDA75610_WBuf] ;
    uint8_t                 TDA75610_RBuf [MAX_TDA75610_RBuf] ;
    PTDA75610_WREG          ptda75610_wreg ;
    PTDA75610_RREG          ptda75610_rreg ;
    bool                    pwr_amp_wait_state ;
    uint32_t                tmr_TDA75610 ;

    void pwr_amp_process_init(void) ;
    void pwr_amp_process_write(void) ;
    void pwr_amp_process_write_wait(void) ;
    void pwr_amp_process_idle(void) ;
    void pwr_amp_process_nop(void) ;

    const pvFUNCv pwr_amp_process [] = {
        pwr_amp_process_init                        /* Handle_PwrAmpCtrl_STATE_INIT         */
        , pwr_amp_process_write                     /* Handle_PwrAmpCtrl_STATE_WRITE        */
        , pwr_amp_process_write_wait                /* Handle_PwrAmpCtrl_STATE_WRITE_WAIT   */
        , pwr_amp_process_nop                       /* Handle_PwrAmpCtrl_STATE_READ         */
        , pwr_amp_process_nop                       /* Handle_PwrAmpCtrl_STATE_READ_WAIT    */
        , pwr_amp_process_idle                      /* Handle_PwrAmpCtrl_STATE_IDLE         */
    };

    void pwr_amp_SetIB1 (uint8_t set_val) ;
    void pwr_amp_SupplyTransitionMuteThreshold (uint8_t set_val) ;
    void pwr_amp_Diagnostic (uint8_t set_val) ;
    void pwr_amp_OffsetDetection (uint8_t set_val) ;
    void pwr_amp_FrontChannelGain (uint8_t set_val) ;
    void pwr_amp_RearChannelGain (uint8_t set_val) ;
    void pwr_amp_MuteFrontChannels (uint8_t set_val) ;
    void pwr_amp_MuteRearChannels (uint8_t set_val) ;
    void pwr_amp_CD (uint8_t set_val) ;

    void pwr_amp_SetIB2 (uint8_t set_val) ;
    void pwr_amp_CurrentDetectionThreshold (uint8_t set_val) ;
    void pwr_amp_MutingTime (uint8_t set_val) ;
    void pwr_amp_AmplifierWorkingMode (uint8_t set_val) ;
    void pwr_amp_DiagnosticMode (uint8_t set_val) ;
    void pwr_amp_CurrentDetectionDiagnostic (uint8_t set_val) ;
    void pwr_amp_RightChannelPowerAmplifierWorkingMode (uint8_t set_val) ;
    void pwr_amp_LeftChannelPowerAmplifierWorkingMode (uint8_t set_val) ;
#else
    extern status_t Handle_PwrAmpCtrl (void) ;

    extern void pwr_amp_SetIB1 (uint8_t set_val) ;
    extern void pwr_amp_SupplyTransitionMuteThreshold (uint8_t set_val) ;
    extern void pwr_amp_Diagnostic (uint8_t set_val) ;
    extern void pwr_amp_OffsetDetection (uint8_t set_val) ;
    extern void pwr_amp_FrontChannelGain (uint8_t set_val) ;
    extern void pwr_amp_RearChannelGain (uint8_t set_val) ;
    extern void pwr_amp_MuteFrontChannels (uint8_t set_val) ;
    extern void pwr_amp_MuteRearChannels (uint8_t set_val) ;
    extern void pwr_amp_CD (uint8_t set_val) ;

    extern void pwr_amp_SetIB2 (uint8_t set_val) ;
    extern void pwr_amp_CurrentDetectionThreshold (uint8_t set_val) ;
    extern void pwr_amp_MutingTime (uint8_t set_val) ;
    extern void pwr_amp_AmplifierWorkingMode (uint8_t set_val) ;
    extern void pwr_amp_DiagnosticMode (uint8_t set_val) ;
    extern void pwr_amp_CurrentDetectionDiagnostic (uint8_t set_val) ;
    extern void pwr_amp_RightChannelPowerAmplifierWorkingMode (uint8_t set_val) ;
    extern void pwr_amp_LeftChannelPowerAmplifierWorkingMode (uint8_t set_val) ;
#endif	// #if defined(_TDA75610SEP_INS_)
/******************************************************************************
* end of file (tda75610sep.h)
******************************************************************************/
