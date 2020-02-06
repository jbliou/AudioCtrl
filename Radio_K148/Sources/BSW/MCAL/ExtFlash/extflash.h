/*
 * extflash.h
 *
 *  Created on: 2020年1月30日
 *      Author: ALLGO
 */

#ifndef BSW_MCAL_EXTFLASH_EXTFLASH_H_
    #define BSW_MCAL_EXTFLASH_EXTFLASH_H_

    typedef enum {
        ExtFlash_STATE_NULL = 0
        , ExtFlash_STATE_INIT
        , ExtFlash_STATE_STABLE
        , ExtFlash_STATE_RESET
        , ExtFlash_STATE_GET_ID
        , ExtFlash_STATE_GET_STATUS
        , ExtFlash_STATE_SET_WEL
        , ExtFlash_STATE_CLR_WEL
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
    } SFLASH_MEM_ACCESS , * PFLASH_MEM_ACCESS ;

    #if defined(__EXTFLASH_INS__)
        ExtFlash_STATE          drv_extflash_main_state = ExtFlash_STATE_INIT ;
        ExtFlash_STATE          extflash_main_state     = ExtFlash_STATE_INIT ;

        ExtFlash_STATE          drv_extflash_sub_state  = ExtFlash_STATE_NULL ;
        ExtFlash_STATE          extflash_sub_state      = ExtFlash_STATE_NULL ;

        SFLASH_MEM_ACCESS       SPIMemory ;
        PFLASH_MEM_ACCESS       pSPIMemory = &(SPIMemory) ;

        uint32_t                drv_timer_extflash ;
        uint32_t                timer_extflash ;
        uint32_t                timer_extflash_status ;
        uint8_t                 Manufacturer_ID ;
        uint8_t                 Device_ID ;
        uint8_t                 extflash_cmd_buf [16] ;
        uint8_t                 extflash_buf [MAX_RECEIVING_BYTES] ;
        #if (DIE_ACTIVE == DIE_W25Q32JV)
            W25Q32JV_STSTUS_REG         reg_status ;
            PW25Q32JV_STSTUS_REG const  preg_status = &(reg_status) ;
        #elif (DIE_ACTIVE == DIE_W25N01GV)
            uint8_t     reg_status ;
        #endif

        void Handle_ExtFlashApp_READ(void) ;
        void Handle_ExtFlashApp_WRITE(void) ;
        void Handle_ExtFlashApp_ERASE(void) ;

        void Handle_ExtFlash_INIT(void) ;
        void Handle_ExtFlash_STABLE(void) ;
        void Handle_ExtFlash_RESET(void) ;
        void Handle_ExtFlash_GET_ID(void) ;
        void Handle_ExtFlash_GET_STATUS(void) ;
        void Handle_ExtFlash_SET_WEL(void) ;
        void Handle_ExtFlash_CLR_WEL(void) ;
        void Handle_ExtFlash_NOP(void) ;
        void Handle_ExtFlash_READ(void) ;
        void Handle_ExtFlash_WRITE(void) ;
        void Handle_ExtFlash_ERASE (void) ;

        void Handle_ExtFlash_ERASE_SECTOR(void) ;
        void Handle_ExtFlash_ERASE_BLOCK_32K(void) ;
        void Handle_ExtFlash_ERASE_BLOCK_64K(void) ;
        void Handle_ExtFlash_ERASE_CHIP(void) ;

        void SPIMemory_Request_Reset (void) ;
        void SetSPIFlashDriver_state(ExtFlash_STATE new_state) ;
        void SetSPIFlashAPP_state(ExtFlash_STATE new_state) ;
        void SPIFlash_Read_Req (uint32_t src_address, uint8_t * target, uint32_t length) ;
        void SPIFlash_Write_Req (uint32_t target_address, uint8_t * source, uint32_t length) ;
        void SPIFlash_Erase_Req (FLAHMEM_OP_MODE cmd, uint32_t src_address) ;

        const PFUNC Handle_Drv_ExtFlash_FUNC[] = {
            Handle_ExtFlash_NOP                 /* ExtFlash_STATE_NULL          */
            , Handle_ExtFlash_INIT              /* ExtFlash_STATE_INIT          */
            , Handle_ExtFlash_STABLE            /* ExtFlash_STATE_STABLE        */
            , Handle_ExtFlash_RESET             /* ExtFlash_STATE_RESET         */
            , Handle_ExtFlash_GET_ID            /* ExtFlash_STATE_GET_ID        */
            , Handle_ExtFlash_GET_STATUS        /* ExtFlash_STATE_GET_STATUS    */
            , Handle_ExtFlash_SET_WEL           /* ExtFlash_STATE_SET_WEL       */
            , Handle_ExtFlash_CLR_WEL           /* ExtFlash_STATE_CLR_WEL       */
            , Handle_ExtFlash_NOP               /* ExtFlash_STATE_IDLE          */
            , Handle_ExtFlash_READ              /* ExtFlash_STATE_READ          */
            , Handle_ExtFlash_WRITE             /* ExtFlash_STATE_WRITE         */
            , Handle_ExtFlash_ERASE             /* ExtFlash_STATE_ERASE         */
        } ;

        PFUNC   op_func = (PFUNC)NULL ;

        #undef  __EXTFLASH_INS__
    #else
        extern void Handle_ExtFlash (void) ;
        extern void SPIFlash_Read_Req (uint32_t src_address, uint8_t * target, uint32_t length) ;
        extern void SPIFlash_Write_Req (uint32_t target_address, uint8_t * source, uint32_t length) ;
        extern void SPIFlash_Erase_Req (FLAHMEM_OP_MODE cmd, uint32_t src_address) ;

    #endif  // #if defined(__EXTFLASH_INS__)
#endif /* BSW_MCAL_EXTFLASH_EXTFLASH_H_ */
