/*
 * TEF668x.h
 *
 *  Created on: 2019年12月2日
 *      Author: ALLGO
 */

#ifndef RADIO_TEF668X_H_
    #define RADIO_TEF668X_H_

    //#define TEF668x_API_DEBUG                 0
    #define TEF668x_API_DEBUG                   1

    #define MAX_TEF668x_BUF                     (0x20U)

    #define GPIO_PULL_DOWN                      (0U)
    #define GPIO_PULL_UP                        (1U)

    #define TEF668x_GPIO_2_TYPE                 (GPIO_PULL_DOWN)
    //#define TEF668x_GPIO_2_TYPE                 (GPIO_PULL_UP)

    #if (TEF668x_GPIO_2_TYPE == GPIO_PULL_DOWN)
        #define I2C_ADDRESS_TEF668x             (0x64U)
    #elif (TEF668x_GPIO_2_TYPE == GPIO_PULL_UP)
        #define I2C_ADDRESS_TEF668x             (0x65U)
    #endif

    //#define PATCH_VERSION                       (116U)
    //#define PATCH_VERSION                       (119U)
    //#define PATCH_VERSION                       (209U)
    //#define PATCH_VERSION                       (212U)
    //#define PATCH_VERSION                       (213U)
    //#define PATCH_VERSION                       (215U)
    //#define PATCH_VERSION                       (217U)
    //#define PATCH_VERSION                       (224U)
    #define PATCH_VERSION                       (512U)

    //#define REF_CLOCK                           4000000
    //#define REF_CLOCK                           9216000
    #define REF_CLOCK                           12000000

    #define MAX_TYNER_QUEUE_BUF                 16

    //------------please refer to GUI--------------------------
    #define TEF668X_SPLIT_SIZE                  (24U)

    #define INIT_FLAG_TIMER                     (0xffU)
    #define INIT_FLAG_PATCH1                    (0xfeU)
    #define INIT_FLAG_PATCH2                    (0xfdU)

    typedef enum {
        TEF668x_BOOT_STATE_START_STAGE
        , TEF668x_BOOT_STATE_INIT_STAGE
        , TEF668x_BOOT_STATE_WAITFOR_TIMEOUT_STAGE
        , TEF668x_BOOT_STATE_LOAD_PATCH_DATA_STAGE
    } TEF668x_BOOT_STATE ;

    typedef enum {
        TEF668x_STATE_WAIT_POWERON
        , TEF668x_STATE_CHECK_POWERON
        , TEF668x_STATE_CHECK_POWERON_BEGIN
        , TEF668x_STATE_CHECK_POWERON_END
        , TEF668x_STATE_BOOT
        , TEF668x_STATE_STANDBY
        , TEF668x_STATE_ACCESS_SERVICE
    } TEF668x_STATE ;

    typedef enum {
        TEF668x_Module_Radio_FM                 = (32U)
        , TEF668x_Module_Radio_AM               = (33U)
        , TEF668x_Module_Audio                  = (48U)
        , TEF668x_Module_APPL                   = (64U)
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
        TEF668x_cmd_Set_Highcut_Fix             = (33U) ,
        TEF668x_cmd_Set_Lowcut_Fix              = (34U) ,
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

    typedef enum {
        TEF668X_API_NULL
        , TEF668X_API_FM_Tune_To
        , TEF668X_API_FM_Set_Tune_Options
        , TEF668X_API_FM_Set_Bandwidth
        , TEF668X_API_FM_Set_RFAGC
        , TEF668X_API_FM_Set_Antenna
        , TEF668X_API_FM_Set_MphSuppression
        , TEF668X_API_FM_Set_ChannelEqualizer
        , TEF668X_API_FM_Set_NoiseBlanker
        , TEF668X_API_FM_Set_NoiseBlanker_Options
        , TEF668X_API_FM_Set_DigitalRadio
        , TEF668X_API_FM_Set_Deemphasis
        , TEF668X_API_FM_Set_StereoImprovement
        , TEF668X_API_FM_Set_Highcut_Fix
        , TEF668X_API_FM_Set_Lowcut_Fix
        , TEF668X_API_FM_Set_LevelStep
        , TEF668X_API_FM_Set_LevelOffset
        , TEF668X_API_FM_Set_Softmute_Time
        , TEF668X_API_FM_Set_Softmute_Level
        , TEF668X_API_FM_Set_Softmute_Noise
        , TEF668X_API_FM_Set_Softmute_Mph
        , TEF668X_API_FM_Set_Softmute_Max
        , TEF668X_API_FM_Set_Highcut_Time
        , TEF668X_API_FM_Set_Highcut_Mod
        , TEF668X_API_FM_Set_Highcut_Level
        , TEF668X_API_FM_Set_Highcut_Noise
        , TEF668X_API_FM_Set_Highcut_Mph
        , TEF668X_API_FM_Set_Highcut_Max
        , TEF668X_API_FM_Set_Highcut_Min
        , TEF668X_API_FM_Set_Lowcut_Max
        , TEF668X_API_FM_Set_Lowcut_Min
        , TEF668X_API_FM_Set_Highcut_Options
        , TEF668X_API_FM_Set_Stereo_Time
        , TEF668X_API_FM_Set_Stereo_Mod
        , TEF668X_API_FM_Set_Stereo_Level
        , TEF668X_API_FM_Set_Stereo_Noise
        , TEF668X_API_FM_Set_Stereo_Mph
        , TEF668X_API_FM_Set_Stereo_Max
        , TEF668X_API_FM_Set_Stereo_Min
        , TEF668X_API_FM_Set_StHiBlend_Time
        , TEF668X_API_FM_Set_StHiBlend_Mod
        , TEF668X_API_FM_Set_StHiBlend_Level
        , TEF668X_API_FM_Set_StHiBlend_Noise
        , TEF668X_API_FM_Set_StHiBlend_Mph
        , TEF668X_API_FM_Set_StHiBlend_Max
        , TEF668X_API_FM_Set_StHiBlend_Min
        , TEF668X_API_FM_Set_Scaler
        , TEF668X_API_FM_Set_RDS
        , TEF668X_API_FM_Set_QualityStatus
        , TEF668X_API_FM_Set_DR_Blend
        , TEF668X_API_FM_Set_DR_Options
        , TEF668X_API_FM_Set_Specials
        , TEF668X_API_FM_Set_Bandwidth_Options
        , TEF668X_API_FM_Set_StBandBlend_Time
        , TEF668X_API_FM_Set_StBandBlend_Gain
        , TEF668X_API_FM_Set_StBandBlend_Bias
        , TEF668X_API_AM_Set_Tune_To
        , TEF668X_API_AM_Set_Bandwidth
        , TEF668X_API_AM_Set_RFAGC
        , TEF668X_API_AM_Set_Antenna
        , TEF668X_API_AM_Set_CoChannelDet
        , TEF668X_API_AM_Set_NoiseBlanker
        , TEF668X_API_AM_Set_NoiseBlanker_Audio
        , TEF668X_API_AM_Set_DigitalRadio
        , TEF668X_API_AM_Set_Highcut_Fix
        , TEF668X_API_AM_Set_Lowcut_Fix
        , TEF668X_API_AM_Set_LevelStep
        , TEF668X_API_AM_Set_LevelOffset
        , TEF668X_API_AM_Set_Softmute_Time
        , TEF668X_API_AM_Set_Softmute_Mod
        , TEF668X_API_AM_Set_Softmute_Level
        , TEF668X_API_AM_Set_Softmute_Max
        , TEF668X_API_AM_Set_Highcut_Time
        , TEF668X_API_AM_Set_Highcut_Mod
        , TEF668X_API_AM_Set_Highcut_Level
        , TEF668X_API_AM_Set_Highcut_Noise
        , TEF668X_API_AM_Set_Highcut_Max
        , TEF668X_API_AM_Set_Highcut_Min
        , TEF668X_API_AM_Set_Lowcut_Max
        , TEF668X_API_AM_Set_Lowcut_Min
        , TEF668X_API_AM_Set_Scaler
        , TEF668X_API_AM_Set_QualityStatus
        , TEF668X_API_AM_Set_DR_Blend
        , TEF668X_API_AM_Set_DR_Options
        , TEF668X_API_AM_Set_Specials
        , TEF668X_API_AUDIO_Set_Volume
        , TEF668X_API_AUDIO_Set_Mute
        , TEF668X_API_AUDIO_Set_Input
        , TEF668X_API_AUDIO_Set_Output_Source
        , TEF668X_API_AUDIO_Set_Ana_Out
        , TEF668X_API_AUDIO_Set_Dig_IO
        , TEF668X_API_AUDIO_Set_Input_Scaler
        , TEF668X_API_AUDIO_Set_WaveGen
        , TEF668X_API_APPL_Set_OperationMode
        , TEF668X_API_APPL_Set_GPIO
        , TEF668X_API_APPL_Set_ReferenceClock
        , TEF668X_API_APPL_Activate
        , TEF668X_API_FM_Get_Quality_Status
        , TEF668X_API_FM_Get_Quality_Data
        , TEF668X_API_FM_Get_RDS_Status
        , TEF668X_API_FM_Get_RDS_Data
        , TEF668X_API_FM_Get_AGC
        , TEF668X_API_FM_Get_Signal_Status
        , TEF668X_API_FM_Get_Processing_Status
        , TEF668X_API_FM_Get_Interface_Status
        , TEF668X_API_AM_Get_Quality_Status
        , TEF668X_API_AM_Get_Quality_Data
        , TEF668X_API_AM_Get_AGC
        , TEF668X_API_AM_Get_Signal_Status
        , TEF668X_API_AM_Get_Processing_Status
        , TEF668X_API_AM_Get_Interface_Status
        , TEF668X_API_APPL_Get_Operation_Status
        , TEF668X_API_APPL_Get_GPIO_Status
        , TEF668X_API_APPL_Get_Identification
        , TEF668X_API_APPL_Get_LastWrite
        , TEF668X_API_APPL_Get_Interface_Status
    } eTEF668X_API ;

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
        uint8_t                 off_mode_hi ;
        uint8_t                 off_mode_lo ;
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
        union {
            struct {
                uint8_t         level_sensitivity_hi ;
                uint8_t         level_sensitivity_lo ;
                uint8_t         modulation_hi ;
                uint8_t         modulation_lo ;
                uint8_t         offset_hi ;
                uint8_t         offset_lo ;
                uint8_t         attack_hi ;
                uint8_t         attack_lo ;
                uint8_t         decay_hi ;
                uint8_t         decay_lo ;
            } ;
            struct {
                uint8_t         gain_hi ;
                uint8_t         gain_lo ;
                uint8_t         blank_time_hi ;
                uint8_t         blank_time_lo ;
            } ;
        };
    } SSet_NoiseBlanker,    * PSet_NoiseBlanker ;

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
        uint8_t                 blank_time_level_hi ;
        uint8_t                 blank_time_level_lo ;
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
        uint8_t                 limit_mode_hi ;
        uint8_t                 limit_mode_lo ;
        uint8_t                 limit_hi ;
        uint8_t                 limit_lo ;
    } SSet_Softmute_Noise,  * PSet_Softmute_Noise ,
      SSet_Softmute_Mph,    * PSet_Softmute_Mph ;

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
      SSet_Highcut_Level,   * PSet_Highcut_Level,
      SSet_Highcut_Noise,   * PSet_Highcut_Noise,
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
      SSet_Highcut_Fix,     * PSet_Highcut_Fix,
      SSet_Lowcut_Max,      * PSet_Lowcut_Max,
      SSet_Lowcut_Min,      * PSet_Lowcut_Min,
      SSet_Lowcut_Fix,      * PSet_Lowcut_Fix,
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
        #if (PATCH_VERSION == 116U)
            #include "Tuner_Patch_Lithio_V101_p116.h"
        #elif (PATCH_VERSION == 119U)
            #include "Tuner_Patch_Lithio_V101_p119.h"
        #elif (PATCH_VERSION == 209U)
            #include "Tuner_Patch_Lithio_V102_p209.h"
        #elif (PATCH_VERSION == 212U)
            #include "Tuner_Patch_Lithio_V102_p212.h"
        #elif (PATCH_VERSION == 213U)
            #include "Tuner_Patch_Lithio_V102_p213.h"
        #elif (PATCH_VERSION == 215U)
            #include "Tuner_Patch_Lithio_V102_p215.h"
        #elif (PATCH_VERSION == 217U)
            #include "Tuner_Patch_Lithio_V102_p217.h"
        #elif (PATCH_VERSION == 224U)
            #include "Tuner_Patch_Lithio_V102_p224.h"
        #elif (PATCH_VERSION == 512U)
            #include "Tuner_Patch_Lithio_V205_p512.h"
        #endif  // #if (PATCH_VERSION == 512U)

        TEF668x_STATE           state_TEF668x = TEF668x_STATE_WAIT_POWERON ;
        uint8_t                 TEF668x_Buf [MAX_TEF668x_BUF] ;
        uint32_t                tmr_TEF668x ;
        uint8_t                 state_boot ;
        uint8_t *               ptr_boot ;
        uint8_t *               ptr_patch ;
        uint32_t                patch_size ;
        PFUNCvS                 tuner_api_callback ;

        void Handle_TEF668x_Service (status_t exe_state) ;
        status_t TEF668x_api (uint8_t cmd, PFUNCvS cb, uint8_t argc, ...) ;

        status_t TEF668x_Boot (void) ;
        /*
        Note:
        This table was pasted from GUI, so customer need to change this table for their own request
              此表從GUI 完全拷貝, 加上最後兩行測試代碼,會有FM 102M 聲音輸出
        */
        const uint8_t tuner_init_para_tab[] = {
        /*
        Length, Command and Data,                                   Remark
        ------  --------------------------------------------------- -----------------------------------------------
        */
            5,  0x1e, 0x5a, 0x01, 0x5a, 0x5a,                       //reset device
            2,  INIT_FLAG_TIMER,  100,                              //wait 100ms

            3,  0x1c, 0x00, 0x00,                                   //Clear Required Initialization Control
            3,  0x1C, 0x00, 0x74,                                   //Set Required Initialization Control(1)
            1,  INIT_FLAG_PATCH1,                                   //Load Required Initialization(s)....

            3,  0x1c, 0x00, 0x00,                                   //Clear Required Initialization Control
            3,  0x1C, 0x00, 0x75,                                   //Set Required Initialization Control(2)
            1,  INIT_FLAG_PATCH2,                                   //Load Required Initialization(s)....

            3,  0x1c, 0x00, 0x00,                                   //Clear Required Initialization Control
            3,  0x14, 0x00, 0x01,                                   //Start Firmware....

            2,  INIT_FLAG_TIMER,  50,                               //(Boot state to Idle State)
                                                                    //wait 50ms

            #if (REF_CLOCK == 12000000)
            9,  0x40, 0x04, 0x01, 0x00, 0xb7, 0x1b,                 //APPL_Set_ReferenceClock 12 MHz
                0x00, 0x00, 0x00,
            #elif (REF_CLOCK == 9216000)
            9,  0x40, 0x04, 0x01, 0x00, 0x8C, 0xA0,                 //APPL_Set_ReferenceClock 9.216 MHz
                0x00, 0x00, 0x00,
            #elif (REF_CLOCK == 4000000)
            9,  0x40, 0x04, 0x01, 0x00, 0x3D, 0x09,                 //APPL_Set_ReferenceClock 4 MHz
                0x00, 0x00, 0x00,
            #endif

            //Activate Device...(Idle state to Active state)
            5,  0x40, 0x05, 0x01, 0x00, 0x01,                       // APPL_Activate
            2,  INIT_FLAG_TIMER,  100,                              //(Idle state to Active state)
                                                                    //wait 100ms
            #if (PATCH_VERSION >= 217)
            17, 0x20, 0x0A, 0x01, 0x00, 0x01, 0x09, 0x38, 0x05,     // FM_Set_Bandwidth
                0x46, 0x03, 0xE8, 0x02, 0x30, 0x09, 0x38, 0x01,
                0x2C,
            //17, 0x20, 0x0A, 0x01, 0x00, 0x01, 0x09, 0x38, 0x05,       // FM_Set_Bandwidth
            //    0x46, 0x03, 0xE8, 0x03, 0xE8, 0x09, 0x38, 0x01,
            //    0x2C,
            5,  0x20, 0x16, 0x01, 0x00, 0x01,                       // FM_Set_ChannelEqualizer
            5,  0x20, 0x14, 0x01, 0x00, 0x01,                       // FM_Set_MphSuppression
            9,  0x20, 0x2A, 0x01, 0x00, 0x03, 0x00, 0x96, 0x00,     // FM_Set_SoftMute_Level
                0xDC,
            11, 0x20, 0x28, 0x01, 0x01, 0xF4, 0x00, 0x78, 0x00,     // FM_Set_SoftMute_Time
                0x0A, 0x00, 0x14,
            9,  0x20, 0x2C, 0x01, 0x00, 0x03, 0x01, 0x04, 0x03,     // FM_Set_SoftMute_Mph
                0x34,
            9,  0x20, 0x2B, 0x01, 0x00, 0x03, 0x01, 0x36, 0x03,     // FM_Set_SoftMute_Noise
                0x34,
            9,  0x20, 0x34, 0x01, 0x00, 0x03, 0x01, 0xC2, 0x00,     // FM_Set_HighCut_Level
                0xFA,
            11, 0x20, 0x32, 0x01, 0x03, 0xE8, 0x07, 0xD0, 0x00,     // FM_Set_HighCut_Time
                0x0A, 0x00, 0x14,
            9,  0x20, 0x36, 0x01, 0x00, 0x03, 0x00, 0xD2, 0x01,     // FM_Set_HighCut_Mph
                0x18,
            9,  0x20, 0x35, 0x01, 0x00, 0x03, 0x00, 0xB4, 0x01,     // FM_Set_HighCut_Noise
                0x18,
            7,  0x20, 0x3A, 0x01, 0x00, 0x01, 0x00, 0x14,           // FM_Set_LowCut_Min
            7,  0x20, 0x37, 0x01, 0x00, 0x01, 0x09, 0x60,           // FM_Set_HighCut_Max
            7,  0x20, 0x39, 0x01, 0x00, 0x00, 0x00, 0x64,           // FM_Set_LowCut_Max
            9,  0x20, 0x3E, 0x01, 0x00, 0x03, 0x02, 0x58, 0x00,     // FM_Set_Stereo_Level
                0xFA,
            11, 0x20, 0x3C, 0x01, 0x03, 0xE8, 0x0F, 0xA0, 0x00,     // FM_Set_Stereo_Time
                0x0A, 0x00, 0x14,
            9,  0x20, 0x40, 0x01, 0x00, 0x03, 0x00, 0xD2, 0x01,     // FM_Set_Stereo_Mph
                0x18,
            9,  0x20, 0x3F, 0x01, 0x00, 0x03, 0x00, 0xB4, 0x01,     // FM_Set_Stereo_Noise
                0x18,
            9,  0x20, 0x48, 0x01, 0x00, 0x03, 0x02, 0x58, 0x00,     // FM_Set_StHiBlend_Level
                0xFA,
            11, 0x20, 0x46, 0x01, 0x01, 0xF4, 0x07, 0xD0, 0x00,     // FM_Set_StHiBlend_Time
                0x0A, 0x00, 0x14,
            9,  0x20, 0x4A, 0x01, 0x00, 0x03, 0x00, 0xD2, 0x01,     // FM_Set_StHiBlend_Mph
                0x18,
            9,  0x20, 0x49, 0x01, 0x00, 0x03, 0x00, 0xB4, 0x01,     // FM_Set_StHiBlend_Noise
                0x18,
            9,  0x40, 0x03, 0x01, 0x00, 0x00, 0x00, 0x21, 0x00,     // APPL_Set_GPIO
                0x03,
            9,  0x40, 0x03, 0x01, 0x00, 0x01, 0x00, 0x21, 0x00,     // APPL_Set_GPIO
                0x00,
            9,  0x40, 0x03, 0x01, 0x00, 0x02, 0x00, 0x21, 0x00,     // APPL_Set_GPIO
                0x00,
            9,  0x40, 0x03, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00,     // APPL_Set_GPIO
                0x03,
            9,  0x40, 0x03, 0x01, 0x00, 0x01, 0x00, 0x20, 0x00,     // APPL_Set_GPIO
                0x00,
            9,  0x40, 0x03, 0x01, 0x00, 0x02, 0x00, 0x20, 0x00,     // APPL_Set_GPIO
                0x00,

            13, 0x30, 0x16, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00,     // AUDIO_Set_Dig_IO
                0x20, 0x00, 0x00, 0x11, 0x3A,
            13, 0x30, 0x16, 0x01, 0x00, 0x21, 0x00, 0x00, 0x00,     // AUDIO_Set_Dig_IO
                0x20, 0x00, 0x00, 0x11, 0x3A,

            5,  0x30, 0x0B, 0x01, 0x00, 0x00,                       // AUDIO_Set_Mute
            5,  0x30, 0x0A, 0x01, 0x00, 0x35,                       // AUDIO_Set_Volume
            7,  0x30, 0x0D, 0x01, 0x00, 0x80, 0x00, 0xE0,           // AUDIO_Set_Output_Source
#endif
            //Device Fully Initialised: LithioDR (TEF6688), Waiting for User Event........

            //from here for testing -------------------------------------

            7,  0x20, 0x01, 0x01, 0x00, 0x01, 0x28, 0xFA,           // FM_Tune_To (1, 1, 10490)
            5,  0x30, 0x0A, 0x01, 0x00, 0x14,                       // AUDIO_Set_Volume 20dB

            //-------after loading the table, FM104.9M will output
            0                                                       // END
        } ;  // static const uint8_t tuner_init_para_tab[]

        // This table is executed after command FM_Tune_To
        const uint8_t tuner_FM_para_tab[] = {
            /*===== =================================================== ===============================================
            Length, Command and Data,                                   Remark
            ======= =================================================== =============================================== */
            0x05,   0x20, 0x16, 0x01, 0x00, 0x01,                       // FM_Set_ChannelEqualizer
            0x05,   0x20, 0x14, 0x01, 0x00, 0x01,                       // FM_Set_MphSuppression
            0x0b,   0x20, 0x28, 0x01, 0x00, 0x78, 0x01, 0xf4, 0x00,     // FM_Set_SoftMute_Time
                    0x0a, 0x00, 0x14,
            0x09,   0x20, 0x2c, 0x01, 0x00, 0x00, 0x00, 0xc8, 0x03,     // FM_Set_SoftMute_Time_Mph
                    0xe8,
            0x09,   0x20, 0x2c, 0x01, 0x00, 0x00, 0x00, 0xc8, 0x03,     // FM_Set_SoftMute_Time_Mph
                    0xe8,
            0x09,   0x20, 0x2b, 0x01, 0x00, 0x00, 0x00, 0xc8, 0x03,     // FM_Set_SoftMute_Time_Noise
                    0xe8,
            0x0b,   0x20, 0x32, 0x01, 0x00, 0xc8, 0x07, 0xd0, 0x00,     // FM_Set_HighCut_Time
                    0x0a, 0x00, 0x50,
            0x09,   0x20, 0x36, 0x01, 0x00, 0x03, 0x00, 0x78, 0x00,     // FM_Set_HighCut_Mph
                    0xa0,
            0x09,   0x20, 0x35, 0x01, 0x00, 0x03, 0x00, 0x96, 0x00,     // FM_Set_HighCut_Noise
                    0xc8,
            0x07,   0x20, 0x37, 0x01, 0x00, 0x01, 0x09, 0x60,           // FM_Set_HighCut_Max
            0x07,   0x20, 0x39, 0x01, 0x00, 0x01, 0x00, 0x64,           // FM_Set_LowCut_Min
            0x0b,   0x20, 0x3C, 0x01, 0x00, 0xc8, 0x0f, 0xa0, 0x00,     // FM_Set_Stereo_Time
                    0x14, 0x00, 0x60,
            0x09,   0x20, 0x40, 0x01, 0x00, 0x03, 0x00, 0x64, 0x00,     // FM_Set_Stereo_Mph
                    0x96,
            0x09,   0x20, 0x3f, 0x01, 0x00, 0x03, 0x00, 0x78, 0x00,     // FM_Set_Stereo_Noise
                    0xa0,
            0x09,   0x20, 0x4a, 0x01, 0x00, 0x03, 0x00, 0x50, 0x00,     // FM_Set_StHiBlend_Mph
                    0x8c,
            0x09,   0x20, 0x49, 0x01, 0x00, 0x03, 0x00, 0x50, 0x00,     // FM_Set_StHiBlend_Noise
                    0x8c,
            0x09,   0x40, 0x03, 0x01, 0x00, 0x00, 0x00, 0x21, 0x00,     // APPL_Set_GPIO
                    0x03,
            0x09,   0x40, 0x03, 0x01, 0x00, 0x01, 0x00, 0x21, 0x00,     // APPL_Set_GPIO
                    0x00,
            0x09,   0x40, 0x03, 0x01, 0x00, 0x02, 0x00, 0x21, 0x00,     // APPL_Set_GPIO
                    0x00,
            0x09,   0x40, 0x03, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00,     // APPL_Set_GPIO
                    0x03,
            0x09,   0x40, 0x03, 0x01, 0x00, 0x01, 0x00, 0x20, 0x00,     // APPL_Set_GPIO
                    0x00,
            0x09,   0x40, 0x03, 0x01, 0x00, 0x02, 0x00, 0x20, 0x00,     // APPL_Set_GPIO
                    0x00,
            0x0d,   0x30, 0x16, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00,     // AUDIO_Set_Dig_IO
                    0x20, 0x00, 0x00, 0x11, 0x3a,
            0x0d,   0x30, 0x16, 0x01, 0x00, 0x21, 0x00, 0x00, 0x00,     // AUDIO_Set_Dig_IO
                    0x20, 0x00, 0x00, 0x11, 0x3a,
            0x05,   0x30, 0x0b, 0x01, 0x00, 0x00,                       // AUDIO_Set_Mute
            0x05,   0x30, 0x0a, 0x01, 0x00, 0x32,                       // AUDIO_Set_Volume
            0x07,   0x30, 0x0d, 0x01, 0x00, 0x80, 0x00, 0xe0,           // AUDIO_Set_Output_Source
            0
        } ; // const uint8_t tuner_FM_para_tab[]

        // This table is executed after command AM_Tune_To
        const uint8_t tuner_AM_para_tab[] = {
            /*===== =================================================== ===============================================
            Length, Command and Data,                                   Remark
            ======= =================================================== =============================================== */
            0x07,   0x21, 0x52, 0x01, 0x00, 0xc8, 0x00, 0xe0,           // AM_Set_QualityStatus
            0x07,   0x21, 0x52, 0x01, 0x00, 0x00, 0x00, 0xe0,           // AM_Set_QualityStatus
            0x05,   0x30, 0x0a, 0x01, 0x00, 0x32,                       // AUDIO_Set_Volume
            0
        } ; // const uint8_t tuner_AM_para_tab[]

        #undef                  _RADIO_TEF668X_INS_
    #else
        extern uint8_t          TEF668x_Buf [MAX_TEF668x_BUF] ;

        extern void Handle_TEF668x (void) ;
        extern status_t TEF668x_api (uint8_t cmd, PFUNCvS cb, uint8_t argc, ...) ;

        extern status_t GetState_TEF668x (void) ;
        extern status_t SetState_TEF668x (TEF668x_STATE new_state) ;
    #endif  // #if defined(_RADIO_TEF668X_INS_)
#endif /* RADIO_TEF668X_H_ */
