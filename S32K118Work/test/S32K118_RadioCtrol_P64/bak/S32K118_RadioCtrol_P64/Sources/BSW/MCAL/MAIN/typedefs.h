/******************************************************************************
*
* ALLGO Inc.
* (c) Copyright 2020 ALLGO Inc.
* ALL RIGHTS RESERVED.
***************************************************************************//*!
*
* @file		Platform_Typedefs.h
*
* @author	ALLGO
*
* @brief	
*
*******************************************************************************
* History:
* 
******************************************************************************/
#ifndef BSW_MCAL_MAIN_PLATFORM_TYPEDEFS_H_
	#define BSW_MCAL_MAIN_PLATFORM_TYPEDEFS_H_

    /*******************************************************************************
    **                      Global Type Definitions                               **
    *******************************************************************************/
    typedef unsigned char           uint8_t ;           /**< \brief  8 bit unsigned value */
    typedef unsigned short int      uint16_t ;          /**< \brief 16 bit unsigned value */
    typedef unsigned long int       uint32_t ;          /**< \brief 32 bit unsigned value */
    typedef unsigned long long      uint64_t ;          /**< \brief 64 bit unsigned value */

    typedef unsigned char           uint8 ;             /**< \brief  8 bit unsigned value */
    typedef unsigned short int      uint16 ;            /**< \brief 16 bit unsigned value */
    typedef unsigned long int       uint32 ;            /**< \brief 32 bit unsigned value */
    typedef unsigned long long      uint64 ;            /**< \brief 64 bit unsigned value */

    typedef signed char             int8_t ;            /**< \brief  8 bit signed value */
    typedef short int               int16_t ;           /**< \brief 16 bit signed value */
    typedef long int                int32_t ;           /**< \brief 32 bit signed value */
    typedef long long               int64_t ;           /**< \brief 64 bit signed value */

    typedef signed char             int8 ;              /**< \brief  8 bit signed value */
    typedef short int               int16 ;             /**< \brief 16 bit signed value */
    typedef long int                int32 ;             /**< \brief 32 bit signed value */
    typedef long long               int64 ;             /**< \brief 64 bit signed value */

    typedef unsigned char           boolean ;           /**< \brief for use with TRUE/FALSE */

    typedef unsigned char *         puint8_t ;          /**< \brief  8 bit unsigned point */
    typedef unsigned short int *    puint16_t ;         /**< \brief 16 bit unsigned point */
    typedef unsigned long int *     puint32_t ;         /**< \brief 32 bit unsigned point */
    typedef unsigned long long *    puint64_t ;         /**< \brief 64 bit unsigned point */

    typedef unsigned char *         puint8 ;            /**< \brief  8 bit unsigned point */
    typedef unsigned short int *    puint16 ;           /**< \brief 16 bit unsigned point */
    typedef unsigned long int *     puint32 ;           /**< \brief 32 bit unsigned point */
    typedef unsigned long long *    puint64 ;           /**< \brief 64 bit unsigned point */

    typedef signed char *           pint8_t ;           /**< \brief  8 bit signed point */
    typedef short int *             pint16_t ;          /**< \brief 16 bit signed point */
    typedef long int *              pint32_t ;          /**< \brief 32 bit signed point */
    typedef long long *             pint64_t ;          /**< \brief 64 bit signed point */

    typedef signed char *           pint8 ;             /**< \brief  8 bit signed point */
    typedef short int *             pint16 ;            /**< \brief 16 bit signed point */
    typedef long int *              pint32 ;            /**< \brief 32 bit signed point */
    typedef long long *             pint64 ;            /**< \brief 64 bit signed point */
    /********************************************************************************
    **                      Global  Definitions                                    **
    ********************************************************************************/
    #define NULL_PTR ((void *)0)

    typedef enum {
        FAIL            = 0U
        , SUCCESS       = 1U
        , FALSE         = 0U
        , TRUE          = 1U
        , ABSENT        = 0U
        , PRESENT       = 1U
    } BOOL ;

    typedef union {
        uint64_t        udata64 ;
        long long       data64 ;
        uint32_t        udata32 [2] ;
        uint16_t        udata16 [4] ;
        uint8_t         udata8 [8] ;
        double          double_data ;
    } XDATA64 , * pXDATA64 ;

    typedef union {
        uint32_t        udata32 ;
        long int        data32 ;
        uint16_t        udata16 [2] ;
        uint32          val ;
       struct {
            uint16_t    word_lsb ;
            uint16_t    word_msb ;
        };
        struct {
            uint8_t     lword_lsb ;
            uint8_t     lword_msb ;
            uint8_t     hword_lsb ;
            uint8_t     hword_msb ;
        };
        struct {
            uint8       LOLSB ;
            uint8       LOMSB ;
            uint8       HILSB ;
            uint8       HIMSB ;
        } byte;
        struct {
            uint16        LSW ;
            uint16      MSW ;
        } word ;
        uint8           v [4] ;
        uint8_t         udata8 [4] ;
        float           float_data ;
    } DWORD_VAL, XDATA32, * pXDATA32 ;
    #define LOWER_LSB(a)    ((a).v[0])
    #define LOWER_MSB(a)    ((a).v[1])
    #define UPPER_LSB(a)    ((a).v[2])
    #define UPPER_MSB(a)    ((a).v[3])

    typedef union {
        uint16_t        udata16 ;
        short int       data16 ;
        uint16          val ;
        uint8_t         udata8 [2] ;
        struct {
            uint8_t     lobyte ;
            uint8_t     hibyte ;
        };
        struct {
            uint8       LSB ;
            uint8       MSB ;
        } byte;
        uint8           v[2] ;
    } WORD_VAL, XDATA16 , * pXDATA16 ;

    #define LSB(a)          ((a).v[0])
    #define MSB(a)          ((a).v[1])

    typedef union {
        uint8_t         udata8 ;
        char            data8 ;
        struct {
            uint8_t     bit0    : 1 ;
            uint8_t     bit1    : 1 ;
            uint8_t     bit2    : 1 ;
            uint8_t     bit3    : 1 ;
            uint8_t     bit4    : 1 ;
            uint8_t     bit5    : 1 ;
            uint8_t     bit6    : 1 ;
            uint8_t     bit7    : 1 ;
        };
        struct {
            uint8_t     rbit7   : 1 ;
            uint8_t     rbit6   : 1 ;
            uint8_t     rbit5   : 1 ;
            uint8_t     rbit4   : 1 ;
            uint8_t     rbit3   : 1 ;
            uint8_t     rbit2   : 1 ;
            uint8_t     rbit1   : 1 ;
            uint8_t     rbit0   : 1 ;
        };
        struct {
            uint8_t     b0:1;
            uint8_t     b1:1;
            uint8_t     b2:1;
            uint8_t     b3:1;
            uint8_t     b4:1;
            uint8_t     b5:1;
            uint8_t     b6:1;
            uint8_t     b7:1;
        } bits;
        uint8   Val ;
    } BYTE_VAL, XDATA8 , * pXDATA8 ;

    /********************************************************************************
    **                      Global  Definitions                                    **
    ********************************************************************************/
    #define NULL_PTR ((void *)0)

    #define BIT_0                   (0x01U)
    #define BIT_1                   (0x02U)
    #define BIT_2                   (0x04U)
    #define BIT_3                   (0x08U)
    #define BIT_4                   (0x10U)
    #define BIT_5                   (0x20U)
    #define BIT_6                   (0x40U)
    #define BIT_7                   (0x80U)
    #define BIT_8                   (0x0100U)
    #define BIT_9                   (0x0200U)
    #define BIT_10                  (0x0400U)
    #define BIT_11                  (0x0800U)
    #define BIT_12                  (0x1000U)
    #define BIT_13                  (0x2000U)
    #define BIT_14                  (0x4000U)
    #define BIT_15                  (0x8000U)
    #define BIT_16                  (0x010000U)
    #define BIT_17                  (0x020000U)
    #define BIT_18                  (0x040000U)
    #define BIT_19                  (0x080000U)
    #define BIT_20                  (0x100000U)
    #define BIT_21                  (0x200000U)
    #define BIT_22                  (0x400000U)
    #define BIT_23                  (0x800000U)
    #define BIT_24                  (0x01000000U)
    #define BIT_25                  (0x02000000U)
    #define BIT_26                  (0x04000000U)
    #define BIT_27                  (0x08000000U)
    #define BIT_28                  (0x10000000U)
    #define BIT_29                  (0x20000000U)
    #define BIT_30                  (0x40000000U)
    #define BIT_31                  (0x80000000U)

    typedef status_t (* psFUNCv)(void) ;
    typedef void (* pvFUNCv)(void) ;
    typedef void (* pvFUNCs)(status_t) ;
    typedef void (* pvFUNCu8)(uint8) ;
    typedef void (* pvFUNCpu8)(uint8 *) ;
    typedef void (* pvFUNCsu8)(status_t, uint8) ;
#endif /* BSW_MCAL_MAIN_PLATFORM_TYPEDEFS_H_ */
/******************************************************************************
* end of file (Platform_Typedefs.h)
******************************************************************************/
