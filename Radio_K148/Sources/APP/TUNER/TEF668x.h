/*
 * TEF668x.h
 *
 *  Created on: 2019年12月2日
 *      Author: ALLGO
 */

#ifndef RADIO_TEF668X_H_
    #define RADIO_TEF668X_H_

    #define MAX_TEF668x_BUF                     (20U)

    #define GPIO_PULL_DOWN                      (0U)
    #define GPIO_PULL_UP                        (1U)

    #define TEF668x_GPIO_2_TYPE                 (GPIO_PULL_DOWN)
    //#define TEF668x_GPIO_2_TYPE                 (GPIO_PULL_UP)

    #if (TEF668x_GPIO_2_TYPE == GPIO_PULL_DOWN)
        //#define I2C_ADDRESS_TEF668x             (0x64U)
        #define I2C_ADDRESS_TEF668x             (0x64U)
    #elif (TEF668x_GPIO_2_TYPE == GPIO_PULL_UP)
        //#define I2C_ADDRESS_TEF668x             (0x65U)
        #define I2C_ADDRESS_TEF668x             (0x65U)
    #endif

    typedef enum {
        TEF668x_Module_Radio_FM                 = (32U) ,
        TEF668x_Module_Radio_AM                 = (33U) ,
        TEF668x_Module_Audio                    = (48U) ,
        TEF668x_Module_APPL                     = (64U)
    } TEF668x_Module ;

    typedef enum {
        TEF668x_cmd_Tune_To                     = (1U) ,
        TEF668x_cmd_Set_Tune_Options            = (2U) ,

        TEF668x_cmd_Set_Bandwidth               = (10U) ,
        TEF668x_cmd_Set_RFAGC                   = (11U) ,
        TEF668x_cmd_Set_Antenna                 = (12U) ,
        TEF668x_cmd_Set_CoChannelDet            = (14U) ,

        TEF668x_cmd_Set_MphSuppression          = (20U) ,
        TEF668x_cmd_Set_ChannelEqualizer        = (22U) ,
        TEF668x_cmd_Set_NoiseBlanker            = (23U) ,
        TEF668x_cmd_Set_NoiseBlanker_Options    = (24U) ,
        //TEF668x_cmd_Set_NoiseBlanker            = (23U) ,
        TEF668x_cmd_Set_NoiseBlanker_Audio      = (24U) ,

        TEF668x_cmd_Set_DigitalRadio            = (30U) ,
        TEF668x_cmd_Set_Deemphasis              = (31U) ,
        TEF668x_cmd_Set_StereoImprovement       = (32U) ,
        TEF668x_cmd_Set_LevelStep               = (38U) ,
        TEF668x_cmd_Set_LevelOffset             = (39U) ,

        TEF668x_cmd_Set_Softmute_Time           = (40U) ,
        TEF668x_cmd_Set_Softmute_Mod            = (41U) ,
        TEF668x_cmd_Set_Softmute_Level          = (42U) ,
        TEF668x_cmd_Set_Softmute_Noise          = (43U) ,
        TEF668x_cmd_Set_Softmute_Mph            = (44U) ,
        TEF668x_cmd_Set_Softmute_Max            = (45U) ,

        TEF668x_cmd_Set_Highcut_Time            = (50U) ,
        TEF668x_cmd_Set_Highcut_Mod             = (51U) ,
        TEF668x_cmd_Set_Highcut_Level           = (52U) ,
        TEF668x_cmd_Set_Highcut_Noise           = (53U) ,
        TEF668x_cmd_Set_Highcut_Mph             = (54U) ,
        TEF668x_cmd_Set_Highcut_Max             = (55U) ,
        TEF668x_cmd_Set_Highcut_Min             = (56U) ,
        TEF668x_cmd_Set_Lowcut_Max              = (57U) ,
        TEF668x_cmd_Set_Lowcut_Min              = (58U) ,
        TEF668x_cmd_Set_Highcut_Options         = (59U) ,

        TEF668x_cmd_Set_Stereo_Time             = (60U) ,
        TEF668x_cmd_Set_Stereo_Mod              = (61U) ,
        TEF668x_cmd_Set_Stereo_Level            = (62U) ,
        TEF668x_cmd_Set_Stereo_Noise            = (63U) ,
        TEF668x_cmd_Set_Stereo_Mph              = (64U) ,
        TEF668x_cmd_Set_Stereo_Max              = (65U) ,
        TEF668x_cmd_Set_Stereo_Min              = (66U) ,

        TEF668x_cmd_Set_StHiBlend_Time          = (70U) ,
        TEF668x_cmd_Set_StHiBlend_Mod           = (71U) ,
        TEF668x_cmd_Set_StHiBlend_Level         = (72U) ,
        TEF668x_cmd_Set_StHiBlend_Noise         = (73U) ,
        TEF668x_cmd_Set_StHiBlend_Mph           = (74U) ,
        TEF668x_cmd_Set_StHiBlend_Max           = (75U) ,
        TEF668x_cmd_Set_StHiBlend_Min           = (76U) ,

        TEF668x_cmd_Set_Scaler                  = (80U) ,
        TEF668x_cmd_Set_RDS                     = (81U) ,
        TEF668x_cmd_Set_QualityStatus           = (82U) ,
        TEF668x_cmd_Set_DR_Blend                = (83U) ,
        TEF668x_cmd_Set_DR_Options              = (84U) ,
        TEF668x_cmd_Set_Specials                = (85U) ,
        TEF668x_cmd_Set_Bandwidth_Options       = (86U) ,

        TEF668x_cmd_Set_StBandBlend_Time        = (90U) ,
        TEF668x_cmd_Set_StBandBlend_Gain        = (91U) ,
        TEF668x_cmd_Set_StBandBlend_Bias        = (92U) ,

        TEF668x_cmd_Set_Volume                  = (10U) ,
        TEF668x_cmd_Set_Mute                    = (11U) ,
        TEF668x_cmd_Set_Input                   = (12U) ,
        TEF668x_cmd_Set_Output_Source           = (13U) ,

        TEF668x_cmd_Set_Ana_Out                 = (21U) ,
        TEF668x_cmd_Set_Dig_IO                  = (22U) ,
        TEF668x_cmd_Set_Input_Scaler            = (23U) ,
        TEF668x_cmd_Set_WaveGen                 = (24U) ,

        TEF668x_cmd_Set_OperationMode           = (1U) ,
        TEF668x_cmd_Set_GPIO                    = (3U) ,
        TEF668x_cmd_Set_ReferenceClock          = (4U) ,
        TEF668x_cmd_Activate                    = (5U) ,

        TEF668x_cmd_Get_Quality_Status          = (128U) ,
        TEF668x_cmd_Get_Quality_Data            = (129U) ,

        TEF668x_cmd_Get_RDS_Status              = (130U) ,
        TEF668x_cmd_Get_RDS_Data                = (131U) ,
        TEF668x_cmd_Get_AGC                     = (132U) ,
        TEF668x_cmd_Get_Signal_Status           = (133U) ,
        TEF668x_cmd_Get_Processing_Status       = (134U) ,
        TEF668x_cmd_Get_Interface_Status        = (135U) ,

        TEF668x_cmd_Get_Operation_Status        = (128U) ,
        TEF668x_cmd_Get_GPIO_Status             = (129U) ,

        TEF668x_cmd_Get_Identification          = (130U) ,
        TEF668x_cmd_Get_LastWrite               = (131U) ,
    } TEF668x_CMD ;

    typedef enum {
        Tuning_Actions_Mode_No_Action           = (0U) ,
        Tuning_Actions_Mode_Preset              = (1U) ,
        Tuning_Actions_Mode_Search              = (2U) ,
        Tuning_Actions_Mode_FM_AF_Update        = (3U) ,
        Tuning_Actions_Mode_FM_Jump             = (4U) ,
        Tuning_Actions_Mode_FM_Check            = (5U) ,
        Tuning_Actions_Mode_End                 = (7U) ,
    } Tuning_Actions;

    typedef enum {
        AFU_BW_MODE_FIXED                       = (0U) ,
        AFU_BW_MODE_AUTOMATIC_BANDWIDTH         = (1U) ,
        MAX_AFU_FIXED_IF_BANDWIDTH              = (3110U),
        MIN_AFU_FIXED_IF_BANDWIDTH              = (560U),
        MAX_AFU_MUTE_TIME                       = (1000U),
        MIN_AFU_MUTE_TIME                       = (250U),
        MAX_AFU_SAMPLE_TIME                     = (20000U),
        MIN_AFU_SAMPLE_TIME                     = (1000U),
    } AFU_OPTIONS ;

    typedef enum {
        IF_BandWidth_Control_Mode_Fixed         = 0 ,
        IF_BandWidth_Control_Mode_Automatic     = 1 ,
    } IF_BandWidth_Control_Mode ;

    typedef enum {
        LNA_Gain_Reduction_LEVEL_0dB            = 0 ,
        LNA_Gain_Reduction_LEVEL_6dB            = 60 ,
        LNA_Gain_Reduction_LEVEL_12dB           = 120 ,
        LNA_Gain_Reduction_LEVEL_18dB           = 180 ,
        LNA_Gain_Reduction_LEVEL_24dB           = 240 ,
        LNA_Gain_Reduction_LEVEL_30dB           = 300 ,
        LNA_Gain_Reduction_LEVEL_36dB           = 360 ,
    } LNA_Gain_Reduction_LEVEL  ;

    typedef enum {
        DeemphasisTimeConstant_50uS             = 500 ,
        DeemphasisTimeConstant_75uS             = 750 ,
    } DeemphasisTimeConstant_Mode  ;

    typedef enum {
        Mode_Stereo_High_Blend                  = 0 ,
        Mode_FMSI_Stereo_Band_Blend_System      = 1 ,
    } StereoImprovement_Mode  ;

    typedef enum {
        Mode_OFF                                = 0 ,
        Mode_ON                                 = 1 ,
    } OP_MODE ;

    typedef enum {
        Stereo_Min_Mode_OFF                     = 0 ,
        Stereo_Min_Mode_ON                      = 1 ,
        Stereo_Min_Mode_Forced_MONO             = 2 ,
    } Stereo_Min_MODE ;

    typedef enum {
        Highcut_Options_Mode_IIR                = 1 ,
        Highcut_Options_Mode_deemphasis         = 2 ,
        Highcut_Options_Mode_FIR                = 3 ,
    } Highcut_Options_Mode ;

    typedef enum {
        Mode_Timer_Control_OFF                  = 0 ,
        Mode_Fast_Timer_Control                 = 1 ,
        Mode_Slow_Timer_Control                 = 2 ,
        Mode_Dual_Timer_Control                 = 3 ,
    } Timer_Control_Mode ;

    typedef enum {
        RDS_Control_Mode_OFF                    = 0 ,
        RDS_Control_Mode_DECODER                = 1 ,
        RDS_Control_Mode_DEMODULATOR            = 2 ,
    } RDS_Control_Mode ;

    typedef enum {
        RDS_RESTART_Mode_NO_CONTROL             = 0 ,
        RDS_RESTART_Mode_MANUAL                 = 1 ,
        RDS_RESTART_Mode_AUTOMATIC              = 2 ,
    } RDS_RESTART_Mode ;

    typedef enum {
        RDS_PIN_SIGNAL_NO_PIN_INTERFACE         = 0 ,
        RDS_PIN_SIGNAL_DATA_STATUS              = 2 ,
        RDS_PIN_SIGNAL_2_Wire_Demodulator       = 4 ,
    } RDS_PIN_SIGNAL_Type ;

    typedef enum {
        QualityStatus_SIGNAL_NO_PIN_INTERFACE   = 0 ,
        RDS_PIN_SIGNAL_QSI                      = 2 ,
    } QualityStatus_PIN_SIGNAL_Type ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 tuning_actions_hi ;
        uint8_t                 tuning_actions_lo ;
    } SSetTune_To_End, * PSetTune_To_End ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 tuning_actions_hi ;
        uint8_t                 tuning_actions_lo ;
        uint8_t                 tuning_frequency_hi ;
        uint8_t                 tuning_frequency_lo ;
    } SSetTune_To, * PSetTune_To ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 afu_bw_mode_hi ;
        uint8_t                 afu_bw_mode_lo ;
        uint8_t                 afu_bandwidth_hi ;
        uint8_t                 afu_bandwidth_lo ;
        uint8_t                 afu_mute_time_hi ;
        uint8_t                 afu_mute_time_lo ;
        uint8_t                 afu_sample_time_hi ;
        uint8_t                 afu_sample_time_lo ;
    } SSet_Tune_Options, * PSet_Tune_Options ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 IF_bandwidth_control_mode_hi ;
        uint8_t                 IF_bandwidth_control_mode_lo ;
        uint8_t                 bandwidth_hi ;
        uint8_t                 bandwidth_lo ;
        uint8_t                 control_sensitivity_hi ;
        uint8_t                 control_sensitivity_lo ;
        uint8_t                 low_level_sensitivity_hi ;
        uint8_t                 low_level_sensitivity_lo ;
        uint8_t                 min_bandwidth_hi ;
        uint8_t                 min_bandwidth_lo ;
        uint8_t                 nominal_bandwidth_hi ;
        uint8_t                 nominal_bandwidth_lo ;
        uint8_t                 control_attack_hi ;
        uint8_t                 control_attack_lo ;
    } SSet_Bandwidth_FM, * PSet_Bandwidth_FM ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 IF_bandwidth_control_mode_hi ;
        uint8_t                 IF_bandwidth_control_mode_lo ;
        uint8_t                 bandwidth_hi ;
        uint8_t                 bandwidth_lo ;
    } SSet_Bandwidth_AM, * PSet_Bandwidth_AM ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 start_hi ;
        uint8_t                 start_lo ;
        uint8_t                 extension_hi ;
        uint8_t                 extension_lo ;
    } SSet_RFAGC, * PSet_RFAGC ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 attenuation_hi ;
        uint8_t                 attenuation_lo ;
    } SSet_Antenna, * PSet_Antenna ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 mode_hi ;
        uint8_t                 mode_lo ;
        uint8_t                 restart_hi ;
        uint8_t                 restart_lo ;
        uint8_t                 sensitivity_hi ;
        uint8_t                 sensitivity_lo ;
        uint8_t                 count_hi ;
        uint8_t                 count_lo ;
    } SCoChannelDet, * PCoChannelDet ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 mode_hi ;
        uint8_t                 mode_lo ;
    } SSet_MphSuppression,      * PSet_MphSuppression,
      SSet_ChannelEqualizer,    * PSet_ChannelEqualizer,
      SSet_DigitalRadio,        * PSet_DigitalRadio,
      SSet_StereoImprovement,   * PSet_StereoImprovement,
      SSet_Highcut_Options,     * PSet_Highcut_Options,
      SSet_Stereo_Max,          * PSet_Stereo_Max,
      SSet_Mute,                * PSet_Mute,
      SSet_OperationMode,       * PSet_OperationMode,
      SSet_APPL_Activate,       * PSet_APPL_Activate
      ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 mode_hi ;
        uint8_t                 mode_lo ;
        uint8_t                 sensitivity_hi ;
        uint8_t                 sensitivity_lo ;
        uint8_t                 reserved_hi ;
        uint8_t                 reserved_lo ;
        uint8_t                 modulation_hi ;
        uint8_t                 modulation_lo ;
        uint8_t                 offset_hi ;
        uint8_t                 offset_lo ;
        uint8_t                 attack_hi ;
        uint8_t                 attack_lo ;
        uint8_t                 decay_hi ;
        uint8_t                 decay_lo ;
    } SSet_NoiseBlanker_FM, * PSet_NoiseBlanker_FM ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 mode_hi ;
        uint8_t                 mode_lo ;
        uint8_t                 sensitivity_hi ;
        uint8_t                 sensitivity_lo ;
        uint8_t                 gain_hi ;
        uint8_t                 gain_lo ;
        uint8_t                 blank_time_hi ;
        uint8_t                 blank_time_lo ;
    } SSet_NoiseBlanker_AM, * PSet_NoiseBlanker_AM ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 blank_time_hi ;
        uint8_t                 blank_time_lo ;
        uint8_t                 blank_time2_hi ;
        uint8_t                 blank_time2_lo ;
        uint8_t                 blank_modulation_hi ;
        uint8_t                 blank_modulation_lo ;
    } SSet_NoiseBlanker_Options, * PSet_NoiseBlanker_Options ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 mode_hi ;
        uint8_t                 mode_lo ;
        uint8_t                 sensitivity_hi ;
        uint8_t                 sensitivity_lo ;
        uint8_t                 reserved_hi ;
        uint8_t                 reserved_lo ;
        uint8_t                 blank_time_hi ;
        uint8_t                 blank_time_lo ;
    } SSet_NoiseBlanker_Audio, * PSet_NoiseBlanker_Audio ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 timeconstant_hi ;
        uint8_t                 timeconstant_lo ;
    } SSet_Deemphasis, * PSet_Deemphasis ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 step1_hi ;
        uint8_t                 step1_lo ;
        uint8_t                 step2_hi ;
        uint8_t                 step2_lo ;
        uint8_t                 step3_hi ;
        uint8_t                 step3_lo ;
        uint8_t                 step4_hi ;
        uint8_t                 step4_lo ;
        uint8_t                 step5_hi ;
        uint8_t                 step5_lo ;
        uint8_t                 step6_hi ;
        uint8_t                 step6_lo ;
        uint8_t                 step7_hi ;
        uint8_t                 step7_lo ;
    } SSet_LevelStep, * PSet_LevelStep ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 offset_hi ;
        uint8_t                 offset_lo ;
    } SSet_LevelOffset, * PSet_LevelOffset ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 slow_attack_hi ;
        uint8_t                 slow_attack_lo ;
        uint8_t                 slow_decay_hi ;
        uint8_t                 slow_decay_lo ;
        uint8_t                 fast_attack_hi ;
        uint8_t                 fast_attack_lo ;
        uint8_t                 fast_decay_hi ;
        uint8_t                 fast_decay_lo ;
    } SSet_Softmute, * PSet_Softmute ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 mode_hi ;
        uint8_t                 mode_lo ;
        uint8_t                 start_hi ;
        uint8_t                 start_lo ;
        uint8_t                 slope_hi ;
        uint8_t                 slope_lo ;
        uint8_t                 shift_hi ;
        uint8_t                 shift_lo ;
    } SSet_Softmute_Mod,    * PSet_Softmute_Mod,
      SSet_Highcut_Mod,     * PSet_Highcut_Mod,
      SSet_Stereo_Mod,      * PSet_Stereo_Mod,
      SSet_StHiBlend_Mod,   * PSet_StHiBlend_Mod
      ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 mode_hi ;
        uint8_t                 mode_lo ;
        uint8_t                 start_hi ;
        uint8_t                 start_lo ;
        uint8_t                 slope_hi ;
        uint8_t                 slope_lo ;
    } SSet_Softmute_Level,  * PSet_Softmute_Level,
      SSet_Softmute_Noise,  * PSet_Softmute_Noise,
      SSet_Softmute_Mph,    * PSet_Softmute_Mph,
      SSet_Highcut_Level,   * PSet_Highcut_Level,
      SSet_Highcut_Mph,     * PSet_Highcut_Mph,
      SSet_Stereo_Level,    * PSet_Stereo_Level,
      SSet_Stereo_Noise,    * PSet_Stereo_Noise,
      SSet_Stereo_Mph,      * PSet_Stereo_Mph,
      SSet_StHiBlend_Level, * PSet_StHiBlend_Level,
      SSet_StHiBlend_Noise, * PSet_StHiBlend_Noise,
      SSet_StHiBlend_Mph,   * PSet_StHiBlend_Mph
      ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 mode_hi ;
        uint8_t                 mode_lo ;
        uint8_t                 limit_hi ;
        uint8_t                 limit_lo ;
    } SSet_Softmute_Max,    * PSet_Softmute_Max,
      SSet_Highcut_Max,     * PSet_Highcut_Max,
      SSet_Highcut_Min,     * PSet_Highcut_Min,
      SSet_Lowcut_Max,      * PSet_Lowcut_Max,
      SSet_Lowcut_Min,      * PSet_Lowcut_Min,
      SSet_Stereo_Min,      * PSet_Stereo_Min,
      SSet_StHiBlend_Max,   * PSet_StHiBlend_Max,
      SSet_StHiBlend_Min,   * PSet_StHiBlend_Min
      ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 slow_attack_hi ;
        uint8_t                 slow_attack_lo ;
        uint8_t                 slow_decay_hi ;
        uint8_t                 slow_decay_lo ;
        uint8_t                 fast_attack_hi ;
        uint8_t                 fast_attack_lo ;
        uint8_t                 fast_decay_hi ;
        uint8_t                 fast_decay_lo ;
    } SSet_Highcut_Time,    * PSet_Highcut_Time,
      SSet_Stereo_Time,     * PSet_Stereo_Time,
      SSet_StHiBlend_Time,  * PSet_StHiBlend_Time
      ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 gain_hi ;
        uint8_t                 gain_lo ;
    } SSet_Scaler,          * PSet_Scaler ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 mode_hi ;
        uint8_t                 mode_lo ;
        uint8_t                 restart_hi ;
        uint8_t                 restart_lo ;
        uint8_t                 interface_hi ;
        uint8_t                 interface_lo ;
    } SSet_RDS,             * PSet_RDS ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 mode_hi ;
        uint8_t                 mode_lo ;
        uint8_t                 interface_hi ;
        uint8_t                 interface_lo ;
    } SSet_QualityStatus,   * PSet_QualityStatus ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 mode_hi ;
        uint8_t                 mode_lo ;
        uint8_t                 in_time_hi ;
        uint8_t                 in_time_lo ;
        uint8_t                 out_time_hi ;
        uint8_t                 out_time_lo ;
        uint8_t                 gain_hi ;
        uint8_t                 gain_lo ;
    } SSet_DR_Blend,        * PSet_DR_Blend ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 samplerate_hi ;
        uint8_t                 samplerate_lo ;
        uint8_t                 mode_hi ;
        uint8_t                 mode_lo ;
        uint8_t                 format_hi ;
        uint8_t                 format_lo ;
    } SSet_DR_Options,      * PSet_DR_Options ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 ana_out_hi ;
        uint8_t                 ana_out_lo ;
    } SSet_Specials,        * PSet_Specials ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 modulation_hi ;
        uint8_t                 modulation_lo ;
    } SSet_Bandwidth_Options,   * PSet_Bandwidth_Options ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 attack_hi ;
        uint8_t                 attack_lo ;
        uint8_t                 decay_hi ;
        uint8_t                 decay_lo ;
    } SSet_StBandBlend_Time,    * PSet_StBandBlend_Time ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 band1_hi ;
        uint8_t                 band1_lo ;
        uint8_t                 band2_hi ;
        uint8_t                 band2_lo ;
        uint8_t                 band3_hi ;
        uint8_t                 band3_lo ;
        uint8_t                 band4_hi ;
        uint8_t                 band4_lo ;
    } SSet_StBandBlend_Gain,    * PSet_StBandBlend_Gain,
      SSet_StBandBlend_Bias,    * PSet_StBandBlend_Bias
      ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 volume_hi ;
        uint8_t                 volume_lo ;
    } SSet_Volume,          * PSet_Volume ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 source_hi ;
        uint8_t                 source_lo ;
    } SSet_Input,           * PSet_Input ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 signal_hi ;
        uint8_t                 signal_lo ;
        uint8_t                 source_hi ;
        uint8_t                 source_lo ;
    } SSet_Output_Source,   * PSet_Output_Source ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 signal_hi ;
        uint8_t                 signal_lo ;
        uint8_t                 mode_hi ;
        uint8_t                 mode_lo ;
    } SSet_Ana_Out,         * PSet_Ana_Out ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 signal_hi ;
        uint8_t                 signal_lo ;
        uint8_t                 mode_hi ;
        uint8_t                 mode_lo ;
        uint8_t                 format_hi ;
        uint8_t                 format_lo ;
        uint8_t                 operation_hi ;
        uint8_t                 operation_lo ;
        uint8_t                 samplerate_hi ;
        uint8_t                 samplerate_lo ;
    } SSet_Dig_IO,          * PSet_Dig_IO ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 source_hi ;
        uint8_t                 source_lo ;
        uint8_t                 gain_hi ;
        uint8_t                 gain_lo ;
    } SSet_Input_Scaler,    * PSet_Input_Scaler ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 mode_hi ;
        uint8_t                 mode_lo ;
        uint8_t                 offset_hi ;
        uint8_t                 offset_lo ;
        uint8_t                 amplitude1_hi ;
        uint8_t                 amplitude1_lo ;
        uint8_t                 frequency1_hi ;
        uint8_t                 frequency1_lo ;
        uint8_t                 amplitude2_hi ;
        uint8_t                 amplitude2_lo ;
        uint8_t                 frequency2_hi ;
        uint8_t                 frequency2_lo ;
    } SSet_WaveGen,         * PSet_WaveGen ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 pin_hi ;
        uint8_t                 pin_lo ;
        uint8_t                 module_hi ;
        uint8_t                 module_lo ;
        uint8_t                 feature_hi ;
        uint8_t                 feature_lo ;
    } SSet_GPIO,            * PSet_GPIO ;

    typedef struct {
        uint8_t                 module ;
        uint8_t                 cmd ;
        uint8_t                 index ;
        uint8_t                 frequency_msb_hi ;
        uint8_t                 frequency_msb_lo ;
        uint8_t                 frequency_lsb_hi ;
        uint8_t                 frequency_lsb_lo ;
        uint8_t                 type_hi ;
        uint8_t                 type_lo ;
    } SSet_ReferenceClock,  * PSet_ReferenceClock ;

    #if defined(_RADIO_TEF668X_INS_)
		#include "Tuner_Patch_Lithio_V205_p512.h"
		
        uint8_t                 TEF668x_Buf [MAX_TEF668x_BUF] ;

        uint8_t Tune_To (uint8_t * buf, TEF668x_Module module, Tuning_Actions action, uint16_t frequency) ;
        uint8_t Set_Tune_Options (uint8_t * buf, TEF668x_Module module, uint16_t bw_mode, uint16_t bandwidth, uint16_t mute_time, uint16_t sample_time) ;
        uint8_t Set_Bandwidth(uint8_t * buf, TEF668x_Module module, IF_BandWidth_Control_Mode mode, uint16_t bandwidth, uint16_t control_sensitivity, uint16_t low_level_sensitivity, uint16_t min_bandwidth, uint16_t nominal_bandwidth, uint16_t control_attack) ;
        uint8_t Set_RFAGC (uint8_t * buf, TEF668x_Module module, uint16_t start, uint16_t extension) ;
        uint8_t Set_Antenna (uint8_t * buf, TEF668x_Module module, LNA_Gain_Reduction_LEVEL attenuation) ;
        uint8_t Set_CoChannelDet (uint8_t * buf, TEF668x_Module module, uint16_t restart, uint16_t sensitivity, uint16_t count) ;
        uint8_t Set_MphSuppression (uint8_t * buf, TEF668x_Module module, OP_MODE mode) ;
        uint8_t Set_ChannelEqualizer (uint8_t * buf, TEF668x_Module module, OP_MODE mode) ;
        uint8_t Set_NoiseBlanker (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t sensitivity, uint16_t modulation, uint16_t offset, uint16_t attack, uint16_t decay) ;
        uint8_t Set_NoiseBlanker_Options (uint8_t * buf, TEF668x_Module module, uint16_t blank_time, uint16_t blank_time2, uint16_t blank_modulation) ;
        uint8_t Set_NoiseBlanker_Audio (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t sensitivity, uint16_t blank_time) ;
        uint8_t Set_DigitalRadio (uint8_t * buf, TEF668x_Module module, OP_MODE mode) ;
        uint8_t Set_Deemphasis (uint8_t * buf, TEF668x_Module module, DeemphasisTimeConstant_Mode mode) ;
        uint8_t Set_StereoImprovement (uint8_t * buf, TEF668x_Module module, StereoImprovement_Mode mode) ;
        uint8_t Set_LevelStep (uint8_t * buf, TEF668x_Module module, int16_t step1, int16_t step2, int16_t step3, int16_t step4, int16_t step5, int16_t step6, int16_t step7) ;
        uint8_t Set_LevelOffset (uint8_t * buf, TEF668x_Module module, int16_t offset) ;
        uint8_t Set_Softmute (uint8_t * buf, TEF668x_Module module, uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay) ;
        uint8_t Set_Softmute_Mod (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift) ;
        uint8_t Set_Softmute_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        uint8_t Set_Softmute_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        uint8_t Set_Softmute_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        uint8_t Set_Softmute_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) ;
        uint8_t Set_Highcut_Time (uint8_t * buf, TEF668x_Module module, uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay) ;
        uint8_t Set_Highcut_Mod (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift) ;
        uint8_t Set_Highcut_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        uint8_t Set_Highcut_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        uint8_t Set_Highcut_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        uint8_t Set_Highcut_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) ;
        uint8_t Set_Highcut_Min (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) ;
        uint8_t Set_Lowcut_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) ;
        uint8_t Set_Lowcut_Min (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) ;
        uint8_t Set_Highcut_Options (uint8_t * buf, TEF668x_Module module, Highcut_Options_Mode mode) ;
        uint8_t Set_Stereo_Time (uint8_t * buf, TEF668x_Module module, uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay) ;
        uint8_t Set_Stereo_Mod (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift) ;
        uint8_t Set_Stereo_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        uint8_t Set_Stereo_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        uint8_t Set_Stereo_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        uint8_t Set_Stereo_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode) ;
        uint8_t Set_Stereo_Min (uint8_t * buf, TEF668x_Module module, Stereo_Min_MODE mode, uint16_t limit) ;
        uint8_t Set_StHiBlend_Time (uint8_t * buf, TEF668x_Module module, uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay) ;
        uint8_t Set_StHiBlend_Mod (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift) ;
        uint8_t Set_StHiBlend_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        uint8_t Set_StHiBlend_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        uint8_t Set_StHiBlend_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        uint8_t Set_StHiBlend_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) ;
        uint8_t Set_StHiBlend_Min (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) ;
        uint8_t Set_Scaler (uint8_t * buf, TEF668x_Module module, int16_t gain) ;
        uint8_t Set_RDS (uint8_t * buf, TEF668x_Module module, RDS_Control_Mode mode, RDS_RESTART_Mode restart, RDS_PIN_SIGNAL_Type interface) ;
        uint8_t Set_QualityStatus (uint8_t * buf, TEF668x_Module module, uint16_t mode, QualityStatus_PIN_SIGNAL_Type interface) ;
        uint8_t Set_DR_Blend (uint8_t * buf, TEF668x_Module module, uint16_t mode, uint16_t in_time, uint16_t out_time, int16_t gain) ;
        uint8_t Set_DR_Options (uint8_t * buf, TEF668x_Module module, uint16_t samplerate, uint16_t mode, uint16_t format) ;
        uint8_t Set_Specials (uint8_t * buf, TEF668x_Module module, uint16_t ana_out) ;
        uint8_t Set_Bandwidth_Options (uint8_t * buf, TEF668x_Module module, uint16_t modulation) ;
        uint8_t Set_StBandBlend_Time (uint8_t * buf, TEF668x_Module module, uint16_t attack, uint16_t decay) ;
        uint8_t Set_StBandBlend_Gain (uint8_t * buf, TEF668x_Module module, uint16_t band1, uint16_t band2, uint16_t band3, uint16_t band4) ;
        uint8_t Set_StBandBlend_Bias (uint8_t * buf, TEF668x_Module module, int16_t band1, int16_t band2, int16_t band3, int16_t band4) ;
        uint8_t Set_Volume (uint8_t * buf, TEF668x_Module module, int16_t volume) ;
        uint8_t Set_Input (uint8_t * buf, TEF668x_Module module, uint16_t source) ;
        uint8_t Set_Output_Source (uint8_t * buf, TEF668x_Module module, uint16_t signal, uint16_t source) ;
        uint8_t Set_Ana_Out (uint8_t * buf, TEF668x_Module module, uint16_t signal, uint16_t mode) ;
        uint8_t Set_Dig_IO (uint8_t * buf, TEF668x_Module module, uint16_t signal, uint16_t mode, uint16_t format, uint16_t operation, uint16_t samplerate) ;
        uint8_t Set_Input_Scaler (uint8_t * buf, TEF668x_Module module, uint16_t source, int16_t gain) ;
        uint8_t Set_WaveGen (uint8_t * buf, TEF668x_Module module, uint16_t mode, int16_t offset, int16_t amplitude1, uint16_t frequency1, int16_t amplitude2, uint16_t frequency2) ;
        uint8_t Set_OperationMode (uint8_t * buf, TEF668x_Module module, uint16_t mode) ;
        uint8_t Set_GPIO (uint8_t * buf, TEF668x_Module module, uint16_t pin, TEF668x_Module op_module, uint16_t feature) ;
        uint8_t Set_ReferenceClock (uint8_t * buf, TEF668x_Module module, uint16_t frequency_msb, uint16_t frequency_lsb, uint16_t clock_type) ;

        #undef                  _RADIO_TEF668X_INS_
    #else
        extern uint8_t          TEF668x_Buf [MAX_TEF668x_BUF] ;

        extern void TEF668x_Handle (void) ;
        extern uint8_t Tune_To (uint8_t * buf, TEF668x_Module module, Tuning_Actions action, uint16_t frequency) ;
        extern uint8_t Set_Tune_Options (uint8_t * buf, TEF668x_Module module, uint16_t bw_mode, uint16_t bandwidth, uint16_t mute_time, uint16_t sample_time) ;
        extern uint8_t Set_Bandwidth(uint8_t * buf, TEF668x_Module module, IF_BandWidth_Control_Mode mode, uint16_t bandwidth, uint16_t control_sensitivity, uint16_t low_level_sensitivity, uint16_t min_bandwidth, uint16_t nominal_bandwidth, uint16_t control_attack) ;
        extern uint8_t Set_RFAGC (uint8_t * buf, TEF668x_Module module, uint16_t start, uint16_t extension) ;
        extern uint8_t Set_Antenna (uint8_t * buf, TEF668x_Module module, LNA_Gain_Reduction_LEVEL attenuation) ;
        extern uint8_t Set_CoChannelDet (uint8_t * buf, TEF668x_Module module, uint16_t restart, uint16_t sensitivity, uint16_t count) ;
        extern uint8_t Set_MphSuppression (uint8_t * buf, TEF668x_Module module, OP_MODE mode) ;
        extern uint8_t Set_ChannelEqualizer (uint8_t * buf, TEF668x_Module module, OP_MODE mode) ;
        extern uint8_t Set_NoiseBlanker (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t sensitivity, uint16_t modulation, uint16_t offset, uint16_t attack, uint16_t decay) ;
        extern uint8_t Set_NoiseBlanker_Options (uint8_t * buf, TEF668x_Module module, uint16_t blank_time, uint16_t blank_time2, uint16_t blank_modulation) ;
        extern uint8_t Set_NoiseBlanker_Audio (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t sensitivity, uint16_t blank_time) ;
        extern uint8_t Set_DigitalRadio (uint8_t * buf, TEF668x_Module module, OP_MODE mode) ;
        extern uint8_t Set_Deemphasis (uint8_t * buf, TEF668x_Module module, DeemphasisTimeConstant_Mode mode) ;
        extern uint8_t Set_StereoImprovement (uint8_t * buf, TEF668x_Module module, StereoImprovement_Mode mode) ;
        extern uint8_t Set_LevelStep (uint8_t * buf, TEF668x_Module module, int16_t step1, int16_t step2, int16_t step3, int16_t step4, int16_t step5, int16_t step6, int16_t step7) ;
        extern uint8_t Set_LevelOffset (uint8_t * buf, TEF668x_Module module, int16_t offset) ;
        extern uint8_t Set_Softmute (uint8_t * buf, TEF668x_Module module, uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay) ;
        extern uint8_t Set_Softmute_Mod (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift) ;
        extern uint8_t Set_Softmute_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        extern uint8_t Set_Softmute_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        extern uint8_t Set_Softmute_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;

        extern uint8_t Set_Softmute_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) ;
        extern uint8_t Set_Highcut_Time (uint8_t * buf, TEF668x_Module module, uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay) ;
        extern uint8_t Set_Highcut_Mod (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift) ;
        extern uint8_t Set_Highcut_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        extern uint8_t Set_Highcut_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        extern uint8_t Set_Highcut_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        extern uint8_t Set_Highcut_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) ;
        extern uint8_t Set_Highcut_Min (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) ;
        extern uint8_t Set_Lowcut_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) ;
        extern uint8_t Set_Lowcut_Min (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) ;
        extern uint8_t Set_Highcut_Options (uint8_t * buf, TEF668x_Module module, Highcut_Options_Mode mode) ;
        extern uint8_t Set_Stereo_Time (uint8_t * buf, TEF668x_Module module, uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay) ;
        extern uint8_t Set_Stereo_Mod (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift) ;
        extern uint8_t Set_Stereo_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        extern uint8_t Set_Stereo_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        extern uint8_t Set_Stereo_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        extern uint8_t Set_Stereo_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode) ;
        extern uint8_t Set_Stereo_Min (uint8_t * buf, TEF668x_Module module, Stereo_Min_MODE mode, uint16_t limit) ;
        extern uint8_t Set_StHiBlend_Time (uint8_t * buf, TEF668x_Module module,  uint16_t slow_attack, uint16_t slow_decay, uint16_t fast_attack, uint16_t fast_decay) ;
        extern uint8_t Set_StHiBlend_Mod (uint8_t * buf, TEF668x_Module module,  OP_MODE mode, uint16_t start, uint16_t slope, uint16_t shift) ;
        extern uint8_t Set_StHiBlend_Level (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        extern uint8_t Set_StHiBlend_Noise (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        extern uint8_t Set_StHiBlend_Mph (uint8_t * buf, TEF668x_Module module, Timer_Control_Mode mode, uint16_t start, uint16_t slope) ;
        extern uint8_t Set_StHiBlend_Max (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) ;
        extern uint8_t Set_StHiBlend_Min (uint8_t * buf, TEF668x_Module module, OP_MODE mode, uint16_t limit) ;
        extern uint8_t Set_Scaler (uint8_t * buf, TEF668x_Module module, int16_t gain) ;
        extern uint8_t Set_RDS (uint8_t * buf, TEF668x_Module module, RDS_Control_Mode mode, RDS_RESTART_Mode restart, RDS_PIN_SIGNAL_Type interface) ;
        extern uint8_t Set_QualityStatus (uint8_t * buf, TEF668x_Module module, uint16_t mode, QualityStatus_PIN_SIGNAL_Type interface) ;
        extern uint8_t Set_DR_Blend (uint8_t * buf, TEF668x_Module module, uint16_t mode, uint16_t in_time, uint16_t out_time, int16_t gain) ;
        extern uint8_t Set_DR_Options (uint8_t * buf, TEF668x_Module module, uint16_t samplerate, uint16_t mode, uint16_t format) ;
        extern uint8_t Set_Specials (uint8_t * buf, TEF668x_Module module, uint16_t ana_out) ;
        extern uint8_t Set_Bandwidth_Options (uint8_t * buf, TEF668x_Module module, uint16_t modulation) ;
        extern uint8_t Set_StBandBlend_Time (uint8_t * buf, TEF668x_Module module, uint16_t attack, uint16_t decay) ;
        extern uint8_t Set_StBandBlend_Gain (uint8_t * buf, TEF668x_Module module, uint16_t band1, uint16_t band2, uint16_t band3, uint16_t band4) ;
        extern uint8_t Set_StBandBlend_Bias (uint8_t * buf, TEF668x_Module module, int16_t band1, int16_t band2, int16_t band3, int16_t band4) ;
        extern uint8_t Set_Volume (uint8_t * buf, TEF668x_Module module, int16_t volume) ;
        extern uint8_t Set_Input (uint8_t * buf, TEF668x_Module module, uint16_t source) ;
        extern uint8_t Set_Output_Source (uint8_t * buf, TEF668x_Module module, uint16_t signal, uint16_t source) ;
        extern uint8_t Set_Ana_Out (uint8_t * buf, TEF668x_Module module, uint16_t signal, uint16_t mode) ;
        extern uint8_t Set_Dig_IO (uint8_t * buf, TEF668x_Module module, uint16_t signal, uint16_t mode, uint16_t format, uint16_t operation, uint16_t samplerate) ;
        extern uint8_t Set_Input_Scaler (uint8_t * buf, TEF668x_Module module, uint16_t source, int16_t gain) ;
        extern uint8_t Set_WaveGen (uint8_t * buf, TEF668x_Module module, uint16_t mode, int16_t offset, int16_t amplitude1, uint16_t frequency1, int16_t amplitude2, uint16_t frequency2) ;
        extern uint8_t Set_OperationMode (uint8_t * buf, TEF668x_Module module, uint16_t mode) ;
        extern uint8_t Set_GPIO (uint8_t * buf, TEF668x_Module module, uint16_t pin, TEF668x_Module op_module, uint16_t feature) ;
        extern uint8_t Set_ReferenceClock (uint8_t * buf, TEF668x_Module module, uint16_t frequency_msb, uint16_t frequency_lsb, uint16_t clock_type) ;
    #endif  // #if defined(_RADIO_TEF668X_INS_)
#endif /* RADIO_TEF668X_H_ */
