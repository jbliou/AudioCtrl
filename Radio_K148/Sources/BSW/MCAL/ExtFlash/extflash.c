/*
 * extflash.c
 *
 *  Created on: 2020/01/30/
 *      Author: ALLGO
 */
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "SPIMaster.h"

#define __EXTFLASH_INS__
#include "extflash.h"

uint8_t test_buf [512] ;
uint8_t test_flag = 0 ;

void Handle_ExtFlash (void) {
    if (drv_extflash_main_state < ExtFlash_STATE_MAX) {
        (Handle_Drv_ExtFlash_FUNC[drv_extflash_main_state]) () ;
    }
    else {
        SetSPIFlashDriver_state (ExtFlash_STATE_IDLE) ;
    }

    switch (extflash_main_state) {
        case ExtFlash_STATE_INIT :
            if (drv_extflash_main_state == ExtFlash_STATE_IDLE) {
                SetSPIFree() ;
                TimerStop(&timer_extflash) ;
#if (0)
                // Test code
                if (preg_status->WEL == 0) {
                    SetSPIFlashDriver_state (Handle_ExtFlash_WEL) ;
                    wel_func = Handle_ExtFlash_WEL_SET ;
                }
                else {
                    TimerSet(&timer_extflash, 50) ;
                }
#endif  //#if (0)
                SetSPIFlashAPP_state (ExtFlash_STATE_IDLE) ;
            }   // if (drv_extflash_state == ExtFlash_STATE_IDLE)
            break ;
        case ExtFlash_STATE_IDLE :
#if (0)
            // Test code
            if (TimerHasExpired(&timer_extflash) == TRUE) {
                TimerStop(&timer_extflash) ;
                SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                TimerSet(&timer_extflash, 50) ;
            }   // if (TimerHasExpired(&timer_extflash) == TRUE)
#else
            if (drv_extflash_main_state == ExtFlash_STATE_IDLE) {
#if (WRITE_TABLE == 0 || WRITE_TABLE == 1)
                int32_t len ;
#endif  // #if (WRITE_TABLE == 0 || WRITE_TABLE == 1)

                SetSPIFree() ;
                switch (test_flag) {
#if (WRITE_TABLE == 0)
                    case 0 :
                        extflash_ready = 0U ;
                        ptr_table = (uint8_t *)(pconfig_table) ;
                        table_length = 0 ;
                        test_flag = 1 ;
                        break ;
                    case 1 :
                        len = sizeof(TABLE_CONFIG)/sizeof(uint8_t) - table_length  ;
                        if (len > 0) {
                            if (len > MAX_SPI_BUF) {
                                len = MAX_SPI_BUF ;
                            }
                            test_buf [511] = 0x55 ;
                            SPIFlash_Read_Req (CONFIG_BASE_ADDR + table_length, test_buf + table_length, len, test_buf+511, (PFUNC)NULL) ;
                            table_length += len ;
                        }   // if (len > 0)
                        else {
                            pconfig_table = (PTABLE_CONFIG)test_buf ;

                            config_table.table_ver_m            = pconfig_table->table_ver_m ;
                            config_table.table_ver_s            = pconfig_table->table_ver_s ;
                            config_table.root_base_addr         = pconfig_table->root_base_addr ;
                            config_table.geohash_prefix_first   = pconfig_table->geohash_prefix_first ;
                            config_table.geohash_prefix_middle  = pconfig_table->geohash_prefix_middle ;
                            config_table.geohash_prefix_last    = pconfig_table->geohash_prefix_last ;
                            config_table.root_base_addr         = pconfig_table->root_base_addr ;
                            config_table.root_records           = pconfig_table->root_records ;
                            config_table.zip_table_base         = pconfig_table->zip_table_base ;
                            config_table.region_zip_table_base  = pconfig_table->region_zip_table_base ;
                            config_table.FM_subscription_base   = pconfig_table->FM_subscription_base ;
                            config_table.FM_chain_root_base     = pconfig_table->FM_chain_root_base ;
                            config_table.FM_region_table_base   = pconfig_table->FM_region_table_base ;
                            config_table.AM_subscription_base   = pconfig_table->AM_subscription_base ;
                            config_table.AM_chain_root_base     = pconfig_table->AM_chain_root_base ;
                            config_table.AM_region_table_base   = pconfig_table->AM_region_table_base ;

                            pconfig_table = &(config_table) ;
                            extflash_ready = 0x0a ;
                            test_flag = 2 ;
                        }   // if (len)
                        break ;
                    case 2 :
                        break ;
#elif (WRITE_TABLE == 1)
                    case 0 :
                        extflash_ready = 0U ;
                        pconfig_table->table_ver_m          = DEF_TABLE_VER_M ;
                        pconfig_table->table_ver_s          = DEF_TABLE_VER_S ;
                        pconfig_table->root_base_addr       = ROOT_TABLE_BASE_ADDR ;
                        pconfig_table->root_records         = MAX_ROOT_TABLE ;
                        pconfig_table->geohash_prefix_first = RootTable [0] & (uint32_t) (-256) ;
                        pconfig_table->geohash_prefix_middle= RootTable [MAX_ROOT_TABLE/2] & (uint32_t) (-256) ;
                        pconfig_table->geohash_prefix_last  = RootTable [MAX_ROOT_TABLE-1] & (uint32_t) (-256) ;
                        pconfig_table->zip_table_base       = ZIP_TABLE_BASE_ADDR ;
                        pconfig_table->region_zip_table_base= REGION_ZIP_BASE_ADDR ;
                        pconfig_table->FM_subscription_base = FM_SUBSCRIPTION_BASE_ADDR ;
                        pconfig_table->FM_chain_root_base   = FM_CHAIN_ROOT_TABLE_BASE_ADDR ;
                        pconfig_table->FM_region_table_base = FM_CHAIN_REGION_TABLE_BASE_ADDR ;
                        pconfig_table->AM_subscription_base = AM_SUBSCRIPTION_BASE_ADDR ;
                        pconfig_table->AM_chain_root_base   = AM_CHAIN_ROOT_TABLE_BASE_ADDR ;
                        pconfig_table->AM_region_table_base = AM_CHAIN_REGION_TABLE_BASE_ADDR ;

                        // needs 5.8 s
                        SPIFlash_Erase_Req (FLAHMEM_OP_ERASE_CHIP, 0, NULL, (PFUNC)NULL) ;

                        ptr_table = (uint8_t *)(pconfig_table) ;
                        table_length = 0 ;
                        test_flag = 1 ;
                        break ;
                    case 1 :
                        len = sizeof(TABLE_CONFIG)/sizeof(uint8_t) - table_length  ;
                        if (len > 0) {
                            if (len > MAX_SPI_BUF) {
                                len = MAX_SPI_BUF ;
                            }
                            SPIFlash_Write_Req (CONFIG_BASE_ADDR + table_length, ptr_table + table_length, len, 0, (PFUNC)NULL) ;
                            table_length += len ;
                        }   // if (len > 0)
                        else {
                            ptr_table = (uint8_t *)RootTable ;
                            table_length = 0 ;
                            test_flag = 2 ;
                        }   // if (len)
                        break ;
                    case 2 :
                        len = sizeof(RootTable)/sizeof(uint8_t) - table_length  ;

                        if (len > 0) {
                            if (len >= MAX_SPI_BUF) {
                                len = MAX_SPI_BUF ;
                            }
                            SPIFlash_Write_Req (ROOT_TABLE_BASE_ADDR + table_length, ptr_table + table_length, len, NULL, (PFUNC)NULL) ;
                            table_length += len ;
                        }   // if (len > 0)
                        else {
                            ptr_table = (uint8_t *)zip_list ;
                            table_length = 0 ;
                            test_flag = 3 ;
                        }
                        break ;
                    case 3 :
                        len = sizeof(zip_list)/sizeof(uint8_t) - table_length  ;

                        if (len > 0) {
                            if (len >= MAX_SPI_BUF) {
                                len = MAX_SPI_BUF ;
                            }
                            SPIFlash_Write_Req (ZIP_TABLE_BASE_ADDR + table_length, ptr_table + table_length, len, NULL, (PFUNC)NULL) ;
                            table_length += len ;
                        }   // if (len > 0)
                        else {
                            ptr_table = (uint8_t *)radio_region_table ;
                            table_length = 0 ;
                            test_flag = 4 ;
                        }
                        break ;
                    case 4 :
                        len = sizeof(radio_region_table)/sizeof(uint8_t) - table_length  ;

                        if (len > 0) {
                            if (len >= MAX_SPI_BUF) {
                                len = MAX_SPI_BUF ;
                            }
                            SPIFlash_Write_Req (REGION_ZIP_BASE_ADDR + table_length, ptr_table + table_length, len, NULL, (PFUNC)NULL) ;
                            table_length += len ;
                        }   // if (len > 0)
                        else {
                            ptr_table = (uint8_t *)radio_root_table ;
                            table_length = 0 ;
                            test_flag = 5 ;
                        }
                        break ;
                    case 5 :
                        len = sizeof(radio_root_table)/sizeof(uint8_t) - table_length  ;

                        if (len > 0) {
                            if (len >= MAX_SPI_BUF) {
                                len = MAX_SPI_BUF ;
                            }
                            SPIFlash_Write_Req (FM_CHAIN_ROOT_TABLE_BASE_ADDR + table_length, ptr_table + table_length, len, NULL, (PFUNC)NULL) ;
                            table_length += len ;
                        }   // if (len > 0)
                        else {
                            ptr_table = (uint8_t *)radio_table ;
                            table_length = 0 ;
                            test_flag = 6 ;
                        }
                        break ;
                    case 6 :
                        len = sizeof(radio_table)/sizeof(uint8_t) - table_length  ;

                        if (len > 0) {
                            if (len >= MAX_SPI_BUF) {
                                len = MAX_SPI_BUF ;
                            }
                            SPIFlash_Write_Req (FM_CHAIN_REGION_TABLE_BASE_ADDR + table_length, ptr_table + table_length, len, NULL, (PFUNC)NULL) ;
                            table_length += len ;
                        }   // if (len > 0)
                        else {
                            ptr_table = (uint8_t *)NULL ;
                            table_length = 0 ;
                            test_flag = 10 ;
                        }
                        break ;
                    case 10 :
                        extflash_ready = 0x0aU ;
                        test_flag = 5 ;
                        break ;
#else
                    case 0 :
                        break ;
                    case 1 :
                        memset(test_buf, 0, sizeof(test_buf)) ;
                        SPIFlash_Read_Req (0x123456, test_buf, 257, 0, (PFUNC)NULL) ;
                        test_flag = 10 ;
                        break ;
                    case 2 :
                        // needs 42 ms
                        SPIFlash_Erase_Req (FLAHMEM_OP_ERASE_SECTOR, 0x123456, NULL, (PFUNC)NULL) ;
                        test_flag = 10 ;
                        break ;
                    case 3 :
                        // needs 102 ms
                        SPIFlash_Erase_Req (FLAHMEM_OP_ERASE_BLOCK_32K, 0x123456, NULL, (PFUNC)NULL) ;
                        test_flag = 10 ;
                        break ;
                    case 4 :
                        // needs 144 ms
                        SPIFlash_Erase_Req (FLAHMEM_OP_ERASE_BLOCK_64K, 0x123456, NULL, (PFUNC)NULL) ;
                        test_flag = 10 ;
                        break ;
                    case 5 :
                        // needs 5.8 s
                        SPIFlash_Erase_Req (FLAHMEM_OP_ERASE_CHIP, 0, NULL, (PFUNC)NULL) ;
                        test_flag = 10 ;
                        break ;
                    case 6 :
                        SPIFlash_Write_Req (0x123456, test_buf, 259, NULL, (PFUNC)NULL) ;
                        test_flag = 10 ;
                        break ;
#endif  // #if defined(WRITE_TABLE)
                    default :
                        break ;
                } // switch (test_flag)
            }   // if (drv_extflash_main_state == ExtFlash_STATE_IDLE)
            switch (pSPIMemory->mode) {
                case FLAHMEM_NOP :
                    break ;
                case FLAHMEM_OP_READ :
                    if (pSPIMemory->len > 0) {
                        SetSPIFlashAPP_state (ExtFlash_STATE_READ) ;
                    }
                    else {
                        // illegal length
                        SPIMemory_Request_Reset() ;
                    }
                    break ;
                case FLAHMEM_OP_WRITE :
                    if (pSPIMemory->len > 0) {
                        SetSPIFlashAPP_state (ExtFlash_STATE_WRITE) ;
                    }
                    else {
                        // illegal length
                        SPIMemory_Request_Reset() ;
                    }
                    break ;
                case FLAHMEM_OP_ERASE_SECTOR :
                    SetSPIFlashAPP_state (ExtFlash_STATE_ERASE) ;
                    op_func = Handle_ExtFlash_ERASE_SECTOR ;
                    break ;
                case FLAHMEM_OP_ERASE_BLOCK_32K :
                    SetSPIFlashAPP_state (ExtFlash_STATE_ERASE) ;
                    op_func = Handle_ExtFlash_ERASE_BLOCK_32K ;
                    break ;
                case FLAHMEM_OP_ERASE_BLOCK_64K :
                    SetSPIFlashAPP_state (ExtFlash_STATE_ERASE) ;
                    op_func = Handle_ExtFlash_ERASE_BLOCK_64K ;
                    break ;
                case FLAHMEM_OP_ERASE_CHIP :
                    SetSPIFlashAPP_state (ExtFlash_STATE_ERASE) ;
                    op_func = Handle_ExtFlash_ERASE_CHIP ;
                    break ;
                default :
                    // illegal mode
                    SPIMemory_Request_Reset() ;
                    break ;
            }   // switch (pSPIMemory->mode)
#endif  //#if (0)
            break ;
        case ExtFlash_STATE_READ :
            Handle_ExtFlashApp_READ() ;
            break ;
        case ExtFlash_STATE_WRITE :
            Handle_ExtFlashApp_WRITE() ;
            break ;
        case ExtFlash_STATE_ERASE :
            Handle_ExtFlashApp_ERASE() ;
            break ;
        default :
            SPIMemory_Request_Reset() ;
            extflash_main_state = ExtFlash_STATE_IDLE ;
            break ;
    }   // switch (extflash_state)
}   // void Handle_ExtFlash (void)

void SPIMemory_Request_Reset (void) {
    pSPIMemory->mode    = FLAHMEM_NOP ;
    pSPIMemory->len     = (uint32_t)(0U) ;
    pSPIMemory->buf     = (uint8_t *)(0U) ;
    pSPIMemory->index   = (uint32_t)(0U) ;
    pSPIMemory->address = (uint32_t)(0U) ;
    pSPIMemory->semaphore = (uint8_t *)(0U) ;
    pSPIMemory->callback = (PFUNC)NULL ;
    op_func             = (PFUNC)NULL ;
}   // void SPIMemory_Request_Reset (void)

void SetSPIFlashAPP_state(ExtFlash_STATE new_state) {
    // Set the main state of App layer, and clean the sub state of App layer
    extflash_main_state     = new_state ;
    extflash_sub_state      = ExtFlash_STATE_NULL ;
}   // void SetSPIFlashAPP_state(ExtFlash_STATE new_state)

void SetSPIFlashDriver_state(ExtFlash_STATE new_state) {
    // Set the main state of Driver layer, and clean the sub state of driver layer
    drv_extflash_main_state = new_state ;
    drv_extflash_sub_state  = ExtFlash_STATE_NULL ;
}   // void SetSPIFlash_state(ExtFlash_STATE new_state)

void Handle_ExtFlashApp_READ(void) {
    if (drv_extflash_main_state == ExtFlash_STATE_IDLE) {
        if (GetSPIState() != STATUS_BUSY) {
            SetSPIFree() ;
            switch(extflash_sub_state) {
                case 0 :
                    pSPIMemory->index   = (uint32_t)(0U) ;
                    SetSPIFlashDriver_state (ExtFlash_STATE_READ) ;
                    extflash_sub_state = 1 ;
                    break ;
                case 1 :
                    if (pSPIMemory->index >= pSPIMemory->len) {
                        extflash_sub_state = 2 ;
                    }   // if (pSPIMemory->index >= pSPIMemory->len)
                    break ;
                default:
                    if (pSPIMemory->semaphore != (uint8_t *)NULL) {
                        *(pSPIMemory->semaphore) = 0xaa ;
                        if (pSPIMemory->callback != (PFUNC)NULL) {
                            (pSPIMemory->callback)() ;
                        }   // if (pSPIMemory->callback != (PFUNC)NULL)
                    }   // if (pSPIMemory->semaphore != (uint8_t *)NULL)
                    SPIMemory_Request_Reset() ;
                    SetSPIFlashAPP_state (ExtFlash_STATE_IDLE) ;
                    break ;
            }   // switch(extflash_sub_state)
        }   // if (GetSPIState() != STATUS_BUSY)
    }   // if (drv_extflash_main_state == ExtFlash_STATE_IDLE)
}   // void Handle_ExtFlashApp_READ(void)

void Handle_ExtFlashApp_WRITE(void) {
    if (drv_extflash_main_state == ExtFlash_STATE_IDLE) {
        if (GetSPIState() != STATUS_BUSY) {
            SetSPIFree() ;
            switch(extflash_sub_state) {
                case 0 :
                    pSPIMemory->index   = (uint32_t)(0U) ;
                    extflash_sub_state = 4 ;
                    break ;
                case 1 :
                    SetSPIFlashDriver_state (ExtFlash_STATE_WEL) ;
                    wel_func = Handle_ExtFlash_WEL_SET ;
                    extflash_sub_state = 2 ;
                    TimerSet(&timer_extflash_status, 10) ;
                    break ;
                case 2 :
                    if (TimerHasExpired(&timer_extflash) == TRUE) {
                        TimerStop(&timer_extflash) ;
                        if (pSPIMemory->semaphore != (uint8_t *)NULL) {
                            *(pSPIMemory->semaphore) = 0xf0 ;
                            if (pSPIMemory->callback != (PFUNC)NULL) {
                                (pSPIMemory->callback)() ;
                            }   // if (pSPIMemory->callback != (PFUNC)NULL)
                        }   // if (pSPIMemory->semaphore != (uint8_t *)NULL)
                        extflash_sub_state = 6 ;
                    }   // if (TimerHasExpired(&timer_extflash) == TRUE)
#if (ExtFlashType == WINBOND_W25)
                    else if (preg_status->BUSY == 1)
#elif (ExtFlashType == ISSI_IS25)
                    else if (preg_status->WIP == 1)
#endif
                    {
                        if (TimerHasExpired(&timer_extflash_status) == TRUE) {
                            SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                            TimerSet(&timer_extflash_status, 10) ;
                        }   // if (TimerHasExpired(&timer_extflash_status) == TRUE)
                    }
                    else if (preg_status->WEL == 0) {
                        if (pSPIMemory->index >= pSPIMemory->len) {
                            if (pSPIMemory->semaphore != (uint8_t *)NULL) {
                                *(pSPIMemory->semaphore) = 0xaa ;
                                if (pSPIMemory->callback != (PFUNC)NULL) {
                                    (pSPIMemory->callback)() ;
                                }   // if (pSPIMemory->callback != (PFUNC)NULL)
                            }   // if (pSPIMemory->semaphore != (uint8_t *)NULL)
                            extflash_sub_state = 6 ;
                        }   // if (pSPIMemory->index >= pSPIMemory->len)
                        else {
                            extflash_sub_state = 1 ;
                        }
                    }
                    else {
                        extflash_sub_state = 3 ;
                    }
                    break ;
                case 3 :
                    if (pSPIMemory->index >= pSPIMemory->len) {
                        if (preg_status->WEL == 0) {
                            if (pSPIMemory->semaphore != (uint8_t *)NULL) {
                                *(pSPIMemory->semaphore) = 0xaa ;
                                if (pSPIMemory->callback != (PFUNC)NULL) {
                                    (pSPIMemory->callback)() ;
                                }   // if (pSPIMemory->callback != (PFUNC)NULL)
                            }   // if (pSPIMemory->semaphore != (uint8_t *)NULL)
                            extflash_sub_state = 6 ;
                        }
                        else {
                            extflash_sub_state = 5 ;
                        }
                    }
                    else {
                        SetSPIFlashDriver_state (ExtFlash_STATE_WRITE) ;
                        extflash_sub_state = 2 ;
                    }
                    break ;
                case 4 :
                    if (pSPIMemory->index >= pSPIMemory->len) {
                        if (preg_status->WEL == 0) {
                            if (pSPIMemory->semaphore != (uint8_t *)NULL) {
                                *(pSPIMemory->semaphore) = 0xaa ;
                                if (pSPIMemory->callback != (PFUNC)NULL) {
                                    (pSPIMemory->callback)() ;
                                }   // if (pSPIMemory->callback != (PFUNC)NULL)
                            }   // if (pSPIMemory->semaphore != (uint8_t *)NULL)
                            extflash_sub_state = 6 ;
                        }
                        else {
                            extflash_sub_state = 5 ;
                        }
                    }
                    else {
                        TimerStop(&timer_extflash) ;
                        TimerStop(&timer_extflash_status) ;
                        TimerSet(&timer_extflash, 30) ;
                        TimerSet(&timer_extflash_status, 10) ;
                        SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                        extflash_sub_state = 2 ;
                    }
                    break ;
                case 5 :
                    SetSPIFlashDriver_state (ExtFlash_STATE_WEL) ;
                    wel_func = Handle_ExtFlash_WEL_CLR ;
                    if (pSPIMemory->semaphore != (uint8_t *)NULL) {
                        *(pSPIMemory->semaphore) = 0xaa ;
                        if (pSPIMemory->callback != (PFUNC)NULL) {
                            (pSPIMemory->callback)() ;
                        }   // if (pSPIMemory->callback != (PFUNC)NULL)
                    }   // if (pSPIMemory->semaphore != (uint8_t *)NULL)
                    extflash_sub_state = 6 ;
                    break ;
                default:
                    TimerStop(&timer_extflash) ;
                    SPIMemory_Request_Reset() ;
                    SetSPIFlashAPP_state (ExtFlash_STATE_IDLE) ;
                    break ;
            }   // switch(extflash_sub_state)
        }   // if (GetSPIState() != STATUS_BUSY)
    }   // if (drv_extflash_main_state == ExtFlash_STATE_IDLE)
}   // void Handle_ExtFlashApp_WRITE(void)

void Handle_ExtFlashApp_ERASE(void) {
    if (drv_extflash_main_state == ExtFlash_STATE_IDLE) {
        if (GetSPIState() != STATUS_BUSY) {
            SetSPIFree() ;
            switch(extflash_sub_state) {
                case 0 :
                    TimerStop(&timer_extflash) ;
#if (ExtFlashType == WINBOND_W25)
                    TimerSet(&timer_extflash, 8000) ;
#elif (ExtFlashType == ISSI_IS25)
                    TimerSet(&timer_extflash, 95000) ;
#endif
                    SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                    extflash_sub_state = 1 ;
                    break ;
                case 1 :
                    if (TimerHasExpired(&timer_extflash) == TRUE) {
                        TimerStop(&timer_extflash) ;
                        if (pSPIMemory->semaphore != (uint8_t *)NULL) {
                            *(pSPIMemory->semaphore) = 0xf0 ;
                            if (pSPIMemory->callback != (PFUNC)NULL) {
                                (pSPIMemory->callback)() ;
                            }   // if (pSPIMemory->callback != (PFUNC)NULL)
                        }   // if (pSPIMemory->semaphore != (uint8_t *)NULL)
                        extflash_sub_state = 5 ;
                    }   // if (TimerHasExpired(&timer_extflash) == TRUE)
                    else if (preg_status->WEL == 0) {
                        extflash_sub_state = 2 ;
                    }   // if (preg_status->WEL == 0)
                    else {
                        extflash_sub_state = 3 ;
                    }   // if (preg_status->WEL == 1)
                    break ;
                case 2 :
                    SetSPIFlashDriver_state (ExtFlash_STATE_WEL) ;
                    wel_func = Handle_ExtFlash_WEL_SET ;
                    extflash_sub_state = 1 ;
                    break ;
                case 3 :
                    SetSPIFlashDriver_state (ExtFlash_STATE_ERASE) ;
                    TimerStop(&timer_extflash_status) ;
                    TimerSet(&timer_extflash_status, 20) ;
                    extflash_sub_state = 4 ;
                    break ;
                case 4 :
                    if (TimerHasExpired(&timer_extflash) == TRUE) {
                        TimerStop(&timer_extflash) ;
                        if (pSPIMemory->semaphore != (uint8_t *)NULL) {
                            *(pSPIMemory->semaphore) = 0xf0 ;
                            if (pSPIMemory->callback != (PFUNC)NULL) {
                                (pSPIMemory->callback)() ;
                            }   // if (pSPIMemory->callback != (PFUNC)NULL)
                        }   // if (pSPIMemory->semaphore != (uint8_t *)NULL)
                        extflash_sub_state = 5 ;
                    }   // if (TimerHasExpired(&timer_extflash) == TRUE)
#if (ExtFlashType == WINBOND_W25)
                    else if (preg_status->BUSY == 1) {
#elif (ExtFlashType == ISSI_IS25)
                    else if (preg_status->WIP == 1) {
#endif
                        if (TimerHasExpired(&timer_extflash_status) == TRUE) {
                            SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                            TimerSet(&timer_extflash_status, 20) ;
                        }
                    }
                    else if (preg_status->WEL == 1) {
                        SetSPIFlashDriver_state (ExtFlash_STATE_WEL) ;
                        wel_func = Handle_ExtFlash_WEL_CLR ;
                    }
                    else {
                        if (pSPIMemory->semaphore != (uint8_t *)NULL) {
                            *(pSPIMemory->semaphore) = 0xaa ;
                            if (pSPIMemory->callback != (PFUNC)NULL) {
                                (pSPIMemory->callback)() ;
                            }   // if (pSPIMemory->callback != (PFUNC)NULL)
                        }   // if (pSPIMemory->semaphore != (uint8_t *)NULL)
                        extflash_sub_state = 5 ;
                    }
                    break ;
                default:
                    TimerStop(&timer_extflash) ;
                    TimerStop(&timer_extflash_status) ;
                    SPIMemory_Request_Reset() ;
                    SetSPIFlashAPP_state (ExtFlash_STATE_IDLE) ;
                    break ;
            }   // switch(extflash_sub_state)
        }   // if (GetSPIState() != STATUS_BUSY)
    }   // if (drv_extflash_main_state == ExtFlash_STATE_IDLE)
}   // void Handle_ExtFlashApp_ERASE(void)

void Handle_ExtFlash_INIT(void) {
    if (GetSPIState() != STATUS_BUSY) {
        SetSPIFree() ;
        SetSPIFlashDriver_state (ExtFlash_STATE_STABLE) ;

        TimerSet(&drv_timer_extflash, 5) ;     // wait for external flash stable ,
    }   // if (GetSPIState() != STATUS_BUSY)
}   // void Handle_ExtFlash_INIT(void)

void Handle_ExtFlash_STABLE(void) {
    if (TimerHasExpired(&drv_timer_extflash) == TRUE) {
        TimerStop(&drv_timer_extflash) ;

        SetSPIFlashDriver_state (ExtFlash_STATE_RESET) ;
    }   // if (TimerHasExpired(&drv_timer_extflash) == TRUE)
}   // void Handle_ExtFlash_INIT(void)

void Handle_ExtFlash_RESET(void) {
    if (GetSPIState() != STATUS_BUSY) {
        SetSPIFree() ;
        switch (drv_extflash_sub_state) {
            case 0 :    // selected command
#if (ExtFlashType == WINBOND_W25)
                extflash_cmd_buf [0] = SOFTWARE_DIE_SELECT ;
                extflash_cmd_buf [1] = DIE_ACTIVE ;
                SPIAccess(extflash_cmd_buf, extflash_cmd_buf, 2) ;
#elif (ExtFlashType == ISSI_IS25)
#endif
                TimerSet(&drv_timer_extflash, 5) ;     //
                drv_extflash_sub_state = 1 ;
                break ;
            case 1 :    // wait for timeout
                if (TimerHasExpired(&drv_timer_extflash) == TRUE) {
                    TimerStop(&drv_timer_extflash) ;
#if (ExtFlashType == WINBOND_W25)
                    extflash_cmd_buf [0] = W25Q32JV_CMD_Enable_Reset ;
#elif (ExtFlashType == ISSI_IS25)
                    extflash_cmd_buf [0] = IS25LP128_CMD_Enable_Reset ;
#endif
                    SPIAccess(extflash_cmd_buf
                            , extflash_cmd_buf
                            , 1
                            ) ;
                    drv_extflash_sub_state = 2 ;
                }   // if (TimerHasExpired(&drv_timer_extflash) == TRUE)
                break ;
            case 2 :
#if (ExtFlashType == WINBOND_W25)
                extflash_cmd_buf [0] = W25Q32JV_CMD_Reset_Device ;
#elif (ExtFlashType == ISSI_IS25)
                extflash_cmd_buf [0] = IS25LP128_CMD_Reset_Device ;
#endif
                SPIAccess(extflash_cmd_buf
                        , extflash_cmd_buf
                        , 1
                        ) ;
                TimerSet(&drv_timer_extflash, 15) ;     //
                drv_extflash_sub_state = 3 ;
                break ;
            case 3 :    // wait for timeout (15 ms), than transfer to Get ID state
                if (TimerHasExpired(&drv_timer_extflash) == TRUE) {
                    TimerStop(&drv_timer_extflash) ;
                    SetSPIFlashDriver_state (ExtFlash_STATE_GET_ID) ;
                }   // if (TimerHasExpired(&drv_timer_extflash) == TRUE)
                break ;
            default :
                drv_extflash_sub_state = 0 ;
                break ;
        }   // switch (drv_extflash_sub_state)
    }   // if (GetSPIState() != STATUS_BUSY)
}   // void Handle_ExtFlash_RESET(void) {

void Handle_ExtFlash_GET_ID(void) {
    if (GetSPIState() != STATUS_BUSY) {
        SetSPIFree() ;
        switch (drv_extflash_sub_state) {
            case 0 :    // Send Read ID command
                memset(extflash_cmd_buf, 0, 6) ;
#if (ExtFlashType == WINBOND_W25)
                //extflash_cmd_buf [0] = W25Q32JV_CMD_Manufacturer_ID ;
                //SPIAccess(extflash_cmd_buf
                //        , extflash_cmd_buf
                //        , 6) ;
                extflash_cmd_buf [0] = W25Q32JV_CMD_JEDEC_ID ;
                SPIAccess(extflash_cmd_buf
                        , extflash_cmd_buf
                        , 4) ;
#elif (ExtFlashType == ISSI_IS25)
                //extflash_cmd_buf [0] = IS25LP128_CMD_Read_Manufacturer_ID ;
                extflash_cmd_buf [0] = IS25LP128_CMD_Read_JEDEC_ID ;
                //extflash_cmd_buf [0] = IS25LP128_CMD_Read_JEDEC_ID ;
                //extflash_cmd_buf [0] = IS25LP128_CMD_Release_Power_Down ;
                SPIAccess(extflash_cmd_buf
                        , extflash_cmd_buf
                        , 4) ;
#endif
                drv_extflash_sub_state = 1 ;
                break ;
            case 1 :    // Store Manufacturer and Device ID, than transfer to Get Status state
#if (ExtFlashType == WINBOND_W25)
                //Manufacturer_ID = extflash_cmd_buf[4] ;
                //Device_ID       = extflash_cmd_buf[5] ;
                JEDEC_ID        = extflash_cmd_buf[2] ;
                JEDEC_ID        <<= 8 ;
                JEDEC_ID        += extflash_cmd_buf[3] ;
                // JEDEC_ID should be 0x4016
#elif (ExtFlashType == ISSI_IS25)
                JEDEC_ID        = extflash_cmd_buf[2] ;
                JEDEC_ID        <<= 8 ;
                JEDEC_ID        += extflash_cmd_buf[3] ;
                // JEDEC_ID should be 0x6018
#endif
                SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                break ;
            default :
                drv_extflash_sub_state = 0 ;
                break ;
        }   // switch (drv_extflash_sub_state)
    }   // if (GetSPIState() != STATUS_BUSY)
}   // void Handle_ExtFlash_GET_ID(void)

void Handle_ExtFlash_GET_STATUS(void) {
    if (GetSPIState() != STATUS_BUSY) {
        SetSPIFree() ;
        switch (drv_extflash_sub_state) {
            case 0 :    // Send Read Status_Reg1
                drv_extflash_sub_state = 1 ;
                break ;
            case 1 :    // Send Read Status_Reg1
                memset(extflash_cmd_buf, 0, 2) ;
#if (ExtFlashType == WINBOND_W25)
                extflash_cmd_buf [0] = W25Q32JV_CMD_Read_Status_Register_1 ;
#elif (ExtFlashType == ISSI_IS25)
                extflash_cmd_buf [0] = IS25LP128_CMD_Read_Status_Register ;
#endif
                SPIAccess(extflash_cmd_buf
                        , extflash_cmd_buf
                        , 2
                        ) ;
                drv_extflash_sub_state = 2 ;
                break ;
            case 2 :    // Send Read Status_Reg2, , store Status_Reg1
#if (ExtFlashType == WINBOND_W25)
                #if (DIE_ACTIVE == DIE_W25Q32JV)
                    preg_status->Reg_Status_L = extflash_cmd_buf[1] ;
                #endif

                memset(extflash_cmd_buf, 0, 2) ;
                extflash_cmd_buf [0] = W25Q32JV_CMD_Read_Status_Register_2 ;
                SPIAccess(extflash_cmd_buf
                        , extflash_cmd_buf
                        , 2
                        ) ;
                drv_extflash_sub_state = 3 ;
#elif (ExtFlashType == ISSI_IS25)
                preg_status->Reg_Status = extflash_cmd_buf[1] ;
                drv_extflash_sub_state = 5 ;
#endif
                break ;
            case 3 :    // Send Read Status_Reg3, store Status_Reg2
#if (ExtFlashType == WINBOND_W25)
                #if (DIE_ACTIVE == DIE_W25Q32JV)
                    preg_status->Reg_Status_M = extflash_cmd_buf[1] ;
                #endif

                memset(extflash_cmd_buf, 0, 2) ;
                extflash_cmd_buf [0] = W25Q32JV_CMD_Read_Status_Register_3 ;
                SPIAccess(extflash_cmd_buf
                        , extflash_cmd_buf
                        , 2
                        ) ;
#endif
                drv_extflash_sub_state = 4 ;
                break ;
            case 4 :    // Store Status_Reg3
#if (ExtFlashType == WINBOND_W25)
                #if (DIE_ACTIVE == DIE_W25Q32JV)
                    preg_status->Reg_Status_H = extflash_cmd_buf[1] ;
                #endif
#endif
                drv_extflash_sub_state = 5 ;
                break ;
            default :
                SetSPIFlashDriver_state (ExtFlash_STATE_IDLE) ;
                break ;
        }   // switch (drv_extflash_sub_state)
    }   // if (GetSPIState() != STATUS_BUSY)
}   // void Handle_ExtFlash_GET_STATUS(void)

void Handle_ExtFlash_WEL(void) {
    if (wel_func == (PFUNC)NULL) {
        SetSPIFlashDriver_state (ExtFlash_STATE_IDLE) ;
    }   // if (op_func == (PFUNC)NULL)
    else if (GetSPIState() != STATUS_BUSY) {
        SetSPIFree() ;
        switch (drv_extflash_sub_state) {
            case 0 :
                drv_extflash_sub_state = 1 ;
                break ;
            case 1 :
                wel_func () ;
                SPIAccess(extflash_cmd_buf
                        , extflash_cmd_buf
                        , 1
                        ) ;
                drv_extflash_sub_state = 2 ;
                break ;
            default :
                wel_func = (PFUNC)NULL ;
                SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                break ;
        }   // switch (drv_extflash_sub_state)
    }   // if (GetSPIState() != STATUS_BUSY)
}   // void Handle_ExtFlash_WEL(void)

void Handle_ExtFlash_WEL_CLR(void) {
#if (ExtFlashType == WINBOND_W25)
    extflash_cmd_buf [0] = W25Q32JV_CMD_Write_Disable ;
#elif (ExtFlashType == ISSI_IS25)
    extflash_cmd_buf [0] = IS25LP128_CMD_Write_Disable ;
#endif
}   // void Handle_ExtFlash_WEL_CLR(void)

void Handle_ExtFlash_WEL_SET(void) {
#if (ExtFlashType == WINBOND_W25)
    extflash_cmd_buf [0] = W25Q32JV_CMD_Write_Enable ;
#elif (ExtFlashType == ISSI_IS25)
    extflash_cmd_buf [0] = IS25LP128_CMD_Write_Enable ;
#endif
}   // void Handle_ExtFlash_WEL_SET(void)

void Handle_ExtFlash_NOP(void) {
#if (ExtFlashType == WINBOND_W25)
#elif (ExtFlashType == ISSI_IS25)
#endif
}   //void Handle_ExtFlash_NOP(void)

void Handle_ExtFlash_READ(void) {
    if (GetSPIState() != STATUS_BUSY) {
        SRW_ADDR    tmp ;
        uint32_t    tar, src ;
        uint16_t    idx ;
        uint8_t *   ptar ;

        SetSPIFree() ;
        switch (drv_extflash_sub_state) {
            case 0 :
                tmp.rw_address = pSPIMemory->address + pSPIMemory->index ;
                memset(extflash_buf, 0, MAX_RECEIVING_BYTES) ;
#if (ExtFlashType == WINBOND_W25)
                extflash_buf [0] = W25Q32JV_CMD_Fast_Read ;
#elif (ExtFlashType == ISSI_IS25)
                extflash_buf [0] = IS25LP128_CMD_Fast_Read ;
#endif
                extflash_buf [1] = tmp.address_H ;
                extflash_buf [2] = tmp.address_M ;
                extflash_buf [3] = tmp.address_L ;
                extflash_buf [4] = 0 ;              // dummy
                if ((pSPIMemory->len - pSPIMemory->index) > MAX_BYTES_PER_PAGE) {
                    SPIAccess(extflash_buf
                            , extflash_buf
                            , MAX_BYTES_PER_PAGE + 5
                            ) ;
                }   // if ((pSPIMemory->address + pSPIMemory->len) > tmp)
                else {
                    SPIAccess(extflash_buf
                            , extflash_buf
                            , pSPIMemory->len - pSPIMemory->index + 5
                            ) ;
                }
                drv_extflash_sub_state = 1 ;
                break ;
            case 1 :
                src = pSPIMemory->len ;
                tar = pSPIMemory->index ;
                ptar = pSPIMemory->buf + pSPIMemory->index ;
                for (idx = 5; tar < src && idx < (MAX_BYTES_PER_PAGE + 5); idx++, tar++) {
                    *ptar++ = extflash_buf[idx] ;
                }   // for (idx = 5; tar < src && idx < (MAX_BYTES_PER_PAGE + 5); idx++, tar++)
                pSPIMemory->index = tar ;
                if (tar < src) {
                    drv_extflash_sub_state = 0 ;
                }
                else {
                    drv_extflash_sub_state = 2 ;
                }
                break ;
            default :
                //SPIMemory_Request_Reset() ;
                SetSPIFlashDriver_state (ExtFlash_STATE_IDLE) ;
                //SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                break ;
        }   // switch (drv_extflash_sub_state)
    }   // if (GetSPIState() != STATUS_BUSY)
}   // void Handle_ExtFlash_READ(void)

void Handle_ExtFlash_WRITE(void) {
    if (GetSPIState() != STATUS_BUSY) {
        SRW_ADDR    tmp ;
        uint32_t    tar, src ;
        uint16_t    idx, idx1 ;
        uint8_t *   ptar ;

        SetSPIFree() ;
        switch (drv_extflash_sub_state) {
            case 0 :
                tmp.rw_address = pSPIMemory->address + pSPIMemory->index ;
                memset(extflash_buf, 0, MAX_RECEIVING_BYTES) ;
#if (ExtFlashType == WINBOND_W25)
                extflash_buf [0] = W25Q32JV_CMD_Page_Program ;
#elif (ExtFlashType == ISSI_IS25)
                extflash_buf [0] = IS25LP128_CMD_Input_Page_Program ;
#endif
                extflash_buf [1] = tmp.address_H ;
                extflash_buf [2] = tmp.address_M ;
                extflash_buf [3] = tmp.address_L ;

                src = pSPIMemory->len ;
                tar = pSPIMemory->index ;
                ptar = pSPIMemory->buf + pSPIMemory->index ;

                idx = tmp.rw_address & (uint32_t)(0xffU) ;
                for (idx1 = 4; tar < src && idx < MAX_BYTES_PER_PAGE; idx++, idx1++, tar++) {
                    extflash_buf[idx1] = *ptar++ ;
                }   // for (idx1 = 4; tar < src && idx < MAX_BYTES_PER_PAGE; idx++, idx1++, tar++)

                pSPIMemory->index = tar ;
                SPIAccess(extflash_buf
                        , extflash_buf
                        , idx1
                        ) ;
                drv_extflash_sub_state = 1 ;
                break ;
            case 1 :
                TimerSet(&drv_timer_extflash, 2) ;     //
                drv_extflash_sub_state = 2 ;
                break ;
            case 2 :
                if (TimerHasExpired(&drv_timer_extflash) == TRUE) {
                    TimerStop(&drv_timer_extflash) ;     //
                    drv_extflash_sub_state = 3 ;
                }   // if (TimerHasExpired(&drv_timer_extflash) == TRUE)
                break ;
            default :
                SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                break ;
        }   // switch (drv_extflash_sub_state)
    }   // if (GetSPIState() != STATUS_BUSY)
}   // void Handle_ExtFlash_WRITE(void)

void Handle_ExtFlash_ERASE (void) {
    if (op_func == (PFUNC)NULL) {
        //SPIMemory_Request_Reset() ;
        SetSPIFlashDriver_state (ExtFlash_STATE_IDLE) ;
    }   // if (op_func == (PFUNC)NULL)
    else {
        if (GetSPIState() != STATUS_BUSY) {
            SetSPIFree() ;
            switch (drv_extflash_sub_state) {
                case 0 :
                    op_func() ;
                    drv_extflash_sub_state = 1 ;
                    break ;
                case 1 :
                    TimerSet(&drv_timer_extflash, 5) ;      //
                    drv_extflash_sub_state = 2 ;
                    break ;
                case 2 :
                    if (TimerHasExpired(&drv_timer_extflash) == TRUE) {
                        TimerStop(&drv_timer_extflash) ;    //
                        drv_extflash_sub_state = 3 ;
                    }   // if (TimerHasExpired(&drv_timer_extflash) == TRUE)
                    break ;
                default :
                    TimerStop(&drv_timer_extflash) ;        //
                    //SPIMemory_Request_Reset() ;
                    SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                    break ;
            }   // switch (drv_extflash_sub_state)
        }   // if (GetSPIState() != STATUS_BUSY)
    }
}   // void Handle_ExtFlash_ERASE (void)

void Handle_ExtFlash_ERASE_SECTOR(void) {
    SRW_ADDR    tmp ;

    tmp.rw_address = pSPIMemory->address & (uint32_t)(-4096) ;

#if (ExtFlashType == WINBOND_W25)
    extflash_cmd_buf [0] = W25Q32JV_CMD_Sector_Erase_4KB ;
#elif (ExtFlashType == ISSI_IS25)
    extflash_cmd_buf [0] = IS25LP128_CMD_Sector_Erase_4KB ;
#endif
    extflash_cmd_buf [1] = tmp.address_H ;
    extflash_cmd_buf [2] = tmp.address_M ;
    extflash_cmd_buf [3] = tmp.address_L ;

    SPIAccess(extflash_cmd_buf
            , extflash_cmd_buf
            , 4
            ) ;
}   // void Handle_ExtFlash_ERASE_SECTOR(void)

void Handle_ExtFlash_ERASE_BLOCK_32K(void) {
    SRW_ADDR    tmp ;

    tmp.rw_address = pSPIMemory->address & (uint32_t)(-32768) ;

#if (ExtFlashType == WINBOND_W25)
    extflash_cmd_buf [0] = W25Q32JV_CMD_Block_Erase_32KB ;
#elif (ExtFlashType == ISSI_IS25)
    extflash_cmd_buf [0] = IS25LP128_CMD_Block_Erase_32KB ;
#endif
    extflash_cmd_buf [1] = tmp.address_H ;
    extflash_cmd_buf [2] = tmp.address_M ;
    extflash_cmd_buf [3] = tmp.address_L ;

    SPIAccess(extflash_cmd_buf
            , extflash_cmd_buf
            , 4
            ) ;
}   // void Handle_ExtFlash_ERASE_SECTOR(void)

void Handle_ExtFlash_ERASE_BLOCK_64K(void) {
    SRW_ADDR    tmp ;

    tmp.rw_address = pSPIMemory->address & (uint32_t)(-65536) ;

#if (ExtFlashType == WINBOND_W25)
    extflash_cmd_buf [0] = W25Q32JV_CMD_Block_Erase_64KB ;
#elif (ExtFlashType == ISSI_IS25)
    extflash_cmd_buf [0] = IS25LP128_CMD_Block_Erase_64KB ;
#endif
    extflash_cmd_buf [1] = tmp.address_H ;
    extflash_cmd_buf [2] = tmp.address_M ;
    extflash_cmd_buf [3] = tmp.address_L ;

    SPIAccess(extflash_cmd_buf
            , extflash_cmd_buf
            , 4
            ) ;
}   // void Handle_ExtFlash_ERASE_SECTOR(void)

void Handle_ExtFlash_ERASE_CHIP(void) {
#if (ExtFlashType == WINBOND_W25)
    extflash_cmd_buf [0] = W25Q32JV_CMD_Chip_Erase ;
#elif (ExtFlashType == ISSI_IS25)
    extflash_cmd_buf [0] = IS25LP128_CMD_Chip_Erase ;
#endif

    SPIAccess(extflash_cmd_buf
            , extflash_cmd_buf
            , 1
            ) ;
}   // void Handle_ExtFlash_ERASE_SECTOR(void)

void SPIFlash_Read_Req (uint32_t src_address, uint8_t * target, uint32_t length, uint8_t * semaphore, PFUNC callback) {
    pSPIMemory->mode = FLAHMEM_OP_READ ;
    pSPIMemory->address = src_address ;
    pSPIMemory->buf = target ;
    pSPIMemory->len = length ;
    pSPIMemory->index = 0 ;
    pSPIMemory->semaphore = semaphore ;
    pSPIMemory->callback = callback ;
}   // void SPIFlash_Read_Req (uint32_t src, uint8_t * tar, uint32_t length, uint8_t * semaphore, PFUNC callback)

void SPIFlash_Write_Req (uint32_t target_address, uint8_t * source, uint32_t length, uint8_t * semaphore, PFUNC callback) {
#if (0)
    uint16_t idx ;
    uint8_t * src ;

    src = source ;
    for (idx=0; idx < 259; idx++) {
        *src++ = (idx & 0x0f) | 0xa0 ;
    }

#endif  // #if (1)
    pSPIMemory->mode = FLAHMEM_OP_WRITE ;
    pSPIMemory->address = target_address ;
    pSPIMemory->buf = source ;
    pSPIMemory->len = length ;
    pSPIMemory->index = 0 ;
    pSPIMemory->semaphore = semaphore ;
    pSPIMemory->callback = callback ;
}   // void SPIFlash_Write_Req (uint32_t target_address, uint8_t * source, uint32_t length, uint8_t * semaphore, PFUNC callback)

void SPIFlash_Erase_Req (FLAHMEM_OP_MODE cmd, uint32_t src_address, uint8_t * semaphore, PFUNC callback) {
    if (cmd >= FLAHMEM_OP_ERASE_SECTOR && cmd <= FLAHMEM_OP_ERASE_CHIP) {
        pSPIMemory->mode = cmd ;
        pSPIMemory->address = src_address ;
        pSPIMemory->buf = (uint8_t *)NULL ;
        pSPIMemory->len = 0 ;
        pSPIMemory->index = 0 ;
        pSPIMemory->semaphore = semaphore ;
        pSPIMemory->callback = callback ;
    }   // if (cmd >= FLAHMEM_OP_ERASE_SECTOR && cmd <= FLAHMEM_OP_ERASE_CHIP)
}   // void SPIFlash_Erase_Req (FLAHMEM_OP_MODE cmd, uint32_t src_address, uint8_t * semaphore, PFUNC callback)

status_t get_extFlashConfig (void) {
#if (WRITE_TABLE == 0U)
#endif  // #if (WRITE_TABLE == 0U)
    if (extflash_main_state == ExtFlash_STATE_IDLE) {
        if (extflash_ready == 0x0aU) {
            return (STATUS_SUCCESS) ;
        }   // if (extflash_ready == 0x0aU)
    }   // if (extflash_main_state == ExtFlash_STATE_IDLE)

    return (STATUS_BUSY) ;
}   // status_t get_extFlashConfig (void)

uint32_t get_GeohashPrefixTotalRecord (void) {
    return (pconfig_table->root_records) ;
}   // uint32_t get_GeohashPrefixTotalRecord (void)
