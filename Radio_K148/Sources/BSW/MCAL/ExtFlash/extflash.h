/*
 * extflash.h
 *
 *  Created on: 2020年1月30日
 *      Author: ALLGO
 */

#ifndef BSW_MCAL_EXTFLASH_EXTFLASH_H_
    #define BSW_MCAL_EXTFLASH_EXTFLASH_H_

    #define WINBOND_W25             (1U)
    #define ISSI_IS25               (2U)

    //#define ExtFlashType            (WINBOND_W25)
    #define ExtFlashType            (ISSI_IS25)

    #if (ExtFlashType == WINBOND_W25)
        #include "w25m321av.h"

        #if (DIE_ACTIVE == DIE_W25Q32JV)
            #include "w25q32jv.h"
        #elif (DIE_ACTIVE == DIE_W25N01GV)
            #include "w25n01gv.h"
        #endif
    #elif (ExtFlashType == ISSI_IS25)
        #include "is25lp128.h"
    #endif

    #define WRITE_TABLE                     (0U)
    //#define WRITE_TABLE                     (1U)

    #define CONFIG_BASE_ADDR                (0U)
    #define ROOT_TABLE_BASE_ADDR            (0x1000U)
    #define ZIP_TABLE_BASE_ADDR             (0x10000U)
    #define ZIP_BLOCK_SIZE                  (256U * 4U)         // 256 entry * 4 bytes/entry

    #define REGION_ZIP_BASE_ADDR            (0x8000U)
    #define FM_SUBSCRIPTION_BASE_ADDR       (0xC000U)
    #define AM_SUBSCRIPTION_BASE_ADDR       (0xD000U)
    #define FM_CHAIN_ROOT_TABLE_BASE_ADDR   (0x64000U)
    #define FM_CHAIN_REGION_TABLE_BASE_ADDR (0x6C000U)
    #define AM_CHAIN_ROOT_TABLE_BASE_ADDR   (0x8C000U)
    #define AM_CHAIN_REGION_TABLE_BASE_ADDR (0x94000U)

    #define DEF_TABLE_VER_M                 (0U)
    #define DEF_TABLE_VER_S                 (1U)

    typedef enum {
        ExtFlash_STATE_NULL = 0
        , ExtFlash_STATE_INIT
        , ExtFlash_STATE_STABLE
        , ExtFlash_STATE_RESET
        , ExtFlash_STATE_GET_ID
        , ExtFlash_STATE_GET_STATUS
        , ExtFlash_STATE_WEL
        , ExtFlash_STATE_IDLE
        , ExtFlash_STATE_READ
        , ExtFlash_STATE_WRITE
        , ExtFlash_STATE_ERASE
        , ExtFlash_STATE_MAX
    } ExtFlash_STATE ;

    typedef enum {
        FLAHMEM_NOP         = 0
        , FLAHMEM_OP_READ
        , FLAHMEM_OP_WRITE
        , FLAHMEM_OP_ERASE_SECTOR
        , FLAHMEM_OP_ERASE_BLOCK_32K
        , FLAHMEM_OP_ERASE_BLOCK_64K
        , FLAHMEM_OP_ERASE_CHIP
        , FLAHMEM_MAX_OP
    } FLAHMEM_OP_MODE ;

    typedef union {
        struct {
            uint8_t     address_L ;
            uint8_t     address_M ;
            uint8_t     address_H ;
        };
        uint32_t        rw_address ;
    } SRW_ADDR , * PRW_ADDR ;

    typedef struct {
        FLAHMEM_OP_MODE mode ;
        uint32_t        len ;
        uint32_t        index ;
        uint32_t        address ;
        uint8_t *       buf ;
        uint8_t *       semaphore ;
        PFUNC           callback ;
    } SFLASH_MEM_ACCESS , * PFLASH_MEM_ACCESS ;

    typedef struct {
        uint8_t         table_ver_m ;
        uint8_t         table_ver_s ;
        uint32_t        root_base_addr ;
        uint32_t        root_records ;
        uint32_t        geohash_prefix_first ;
        uint32_t        geohash_prefix_middle ;
        uint32_t        geohash_prefix_last ;
        uint32_t        zip_table_base ;
        uint32_t        region_zip_table_base ;
        uint32_t        FM_subscription_base ;
        uint32_t        FM_chain_root_base ;
        uint32_t        FM_region_table_base ;
        uint32_t        AM_subscription_base ;
        uint32_t        AM_chain_root_base ;
        uint32_t        AM_region_table_base ;
    } TABLE_CONFIG , *  PTABLE_CONFIG ;

    #if defined(__EXTFLASH_INS__)
        #if (WRITE_TABLE == 1)
        #include "RootTable.h"
        #include "zip_list.h"
        #include "radio_region_table.h"
        #include "radio_root_table.h"
        #include "radio_table.h"
        #endif  // #if (WRITE_TABLE == 1)
        uint8_t *               ptr_table ;
        uint32_t                table_length ;

        ExtFlash_STATE          drv_extflash_main_state = ExtFlash_STATE_INIT ;
        ExtFlash_STATE          extflash_main_state     = ExtFlash_STATE_INIT ;

        ExtFlash_STATE          drv_extflash_sub_state  = ExtFlash_STATE_NULL ;
        ExtFlash_STATE          extflash_sub_state      = ExtFlash_STATE_NULL ;

        SFLASH_MEM_ACCESS       SPIMemory ;
        PFLASH_MEM_ACCESS       pSPIMemory = &(SPIMemory) ;
        TABLE_CONFIG            config_table ;
        PTABLE_CONFIG           pconfig_table = &config_table ;

        uint32_t                drv_timer_extflash ;
        uint32_t                timer_extflash ;
        uint32_t                timer_extflash_status ;
        uint8_t                 extflash_cmd_buf [16] ;
        uint8_t                 extflash_buf [MAX_RECEIVING_BYTES] ;
        uint8_t                 extflash_ready = 0 ;
        uint16_t                JEDEC_ID ;
        #if (ExtFlashType == WINBOND_W25)
            #if (DIE_ACTIVE == DIE_W25Q32JV)
                W25Q32JV_STSTUS_REG         reg_status ;
                PW25Q32JV_STSTUS_REG const  preg_status = &(reg_status) ;
            #elif (DIE_ACTIVE == DIE_W25N01GV)
                uint8_t     reg_status ;
            #endif
            uint8_t                 Manufacturer_ID ;
            uint8_t                 Device_ID ;
        #elif (ExtFlashType == ISSI_IS25)
            IS25LP128_STSTUS_REG            reg_status ;
            PIS25LP128_STSTUS_REG const     preg_status = &(reg_status) ;
        #endif

        void Handle_ExtFlashApp_READ(void) ;
        void Handle_ExtFlashApp_WRITE(void) ;
        void Handle_ExtFlashApp_ERASE(void) ;

        void Handle_ExtFlash_INIT(void) ;
        void Handle_ExtFlash_STABLE(void) ;
        void Handle_ExtFlash_RESET(void) ;
        void Handle_ExtFlash_GET_ID(void) ;
        void Handle_ExtFlash_GET_STATUS(void) ;
        void Handle_ExtFlash_WEL(void) ;
        void Handle_ExtFlash_NOP(void) ;
        void Handle_ExtFlash_READ(void) ;
        void Handle_ExtFlash_WRITE(void) ;
        void Handle_ExtFlash_ERASE (void) ;

        void Handle_ExtFlash_ERASE_SECTOR(void) ;
        void Handle_ExtFlash_ERASE_BLOCK_32K(void) ;
        void Handle_ExtFlash_ERASE_BLOCK_64K(void) ;
        void Handle_ExtFlash_ERASE_CHIP(void) ;
        void Handle_ExtFlash_WEL_CLR(void) ;
        void Handle_ExtFlash_WEL_SET(void) ;

        void SPIMemory_Request_Reset (void) ;
        void SetSPIFlashDriver_state(ExtFlash_STATE new_state) ;
        void SetSPIFlashAPP_state(ExtFlash_STATE new_state) ;
        void SPIFlash_Read_Req (uint32_t src_address, uint8_t * target, uint32_t length, uint8_t * semaphore, PFUNC callback) ;
        void SPIFlash_Write_Req (uint32_t target_address, uint8_t * source, uint32_t length, uint8_t * semaphore, PFUNC callback) ;
        void SPIFlash_Erase_Req (FLAHMEM_OP_MODE cmd, uint32_t src_address, uint8_t * semaphore, PFUNC callback) ;

        const PFUNC Handle_Drv_ExtFlash_FUNC[] = {
            Handle_ExtFlash_NOP                 /* ExtFlash_STATE_NULL          */
            , Handle_ExtFlash_INIT              /* ExtFlash_STATE_INIT          */
            , Handle_ExtFlash_STABLE            /* ExtFlash_STATE_STABLE        */
            , Handle_ExtFlash_RESET             /* ExtFlash_STATE_RESET         */
            , Handle_ExtFlash_GET_ID            /* ExtFlash_STATE_GET_ID        */
            , Handle_ExtFlash_GET_STATUS        /* ExtFlash_STATE_GET_STATUS    */
            , Handle_ExtFlash_WEL               /* ExtFlash_STATE_WEL           */
            , Handle_ExtFlash_NOP               /* ExtFlash_STATE_IDLE          */
            , Handle_ExtFlash_READ              /* ExtFlash_STATE_READ          */
            , Handle_ExtFlash_WRITE             /* ExtFlash_STATE_WRITE         */
            , Handle_ExtFlash_ERASE             /* ExtFlash_STATE_ERASE         */
        } ;

        PFUNC   op_func = (PFUNC)NULL ;
        PFUNC   wel_func = (PFUNC)NULL ;

        #undef  __EXTFLASH_INS__
    #else
        extern PTABLE_CONFIG           pconfig_table ;

        extern void Handle_ExtFlash (void) ;
        extern void SPIFlash_Read_Req (uint32_t src_address, uint8_t * target, uint32_t length, uint8_t * semaphore, PFUNC callback) ;
        extern void SPIFlash_Write_Req (uint32_t target_address, uint8_t * source, uint32_t length, uint8_t * semaphore, PFUNC callback) ;
        extern void SPIFlash_Erase_Req (FLAHMEM_OP_MODE cmd, uint32_t src_address, uint8_t * semaphore, PFUNC callback) ;
        extern status_t get_extFlashConfig (void);
    #endif  // #if defined(__EXTFLASH_INS__)
#endif /* BSW_MCAL_EXTFLASH_EXTFLASH_H_ */
