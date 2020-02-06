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
#include "w25m321av.h"

#if (DIE_ACTIVE == DIE_W25Q32JV)
    #include "w25q32jv.h"
#elif (DIE_ACTIVE == DIE_W25N01GV)
    #include "w25n01gv.h"
#endif

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
                    SetSPIFlashDriver_state (ExtFlash_STATE_SET_WEL) ;
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
#if (1)
            switch (test_flag) {
                case 0 :
                    break ;
                case 1 :
                    memset(test_buf, 0, sizeof(test_buf)) ;
                    SPIFlash_Read_Req (0x123456, test_buf, 257) ;
                    test_flag = 10 ;
                    break ;
                case 2 :
                    // needs 42 ms
                    SPIFlash_Erase_Req (FLAHMEM_OP_ERASE_SECTOR, 0x123456) ;
                    test_flag = 10 ;
                    break ;
                case 3 :
                    // needs 102 ms
                    SPIFlash_Erase_Req (FLAHMEM_OP_ERASE_BLOCK_32K, 0x123456) ;
                    test_flag = 10 ;
                    break ;
                case 4 :
                    // needs 144 ms
                    SPIFlash_Erase_Req (FLAHMEM_OP_ERASE_BLOCK_64K, 0x123456) ;
                    test_flag = 10 ;
                    break ;
                case 5 :
                    // needs 5.8 s
                    SPIFlash_Erase_Req (FLAHMEM_OP_ERASE_CHIP, 0) ;
                    test_flag = 10 ;
                    break ;
                case 6 :
                    SPIFlash_Write_Req (0x123456, test_buf, 259) ;
                    test_flag = 10 ;
                    break ;
            }   // switch (test_flag)
#endif  //#if (1)
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
                    if (TimerHasExpired(&timer_extflash_status) == TRUE) {
                        SetSPIFlashDriver_state (ExtFlash_STATE_SET_WEL) ;
                        extflash_sub_state = 2 ;
                    }   // if (TimerHasExpired(&timer_extflash_status) == TRUE)
                    break ;
                case 2 :
                    if (TimerHasExpired(&timer_extflash) == TRUE) {
                        TimerStop(&timer_extflash) ;
                        extflash_sub_state = 6 ;
                    }   // if (TimerHasExpired(&timer_extflash) == TRUE)
                    else if (preg_status->BUSY == 1) {
                        if (TimerHasExpired(&timer_extflash_status) == TRUE) {
                            SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                            TimerSet(&timer_extflash_status, 10) ;
                        }   // if (TimerHasExpired(&timer_extflash_status) == TRUE)
                    }
                    else if (preg_status->WEL == 0) {
                        extflash_sub_state = 1 ;
                        TimerSet(&timer_extflash_status, 10) ;
                    }
                    else {
                        extflash_sub_state = 3 ;
                    }
                    break ;
                case 3 :
                    if (pSPIMemory->index >= pSPIMemory->len) {
                        extflash_sub_state = 5 ;
                    }
                    else {
                        SetSPIFlashDriver_state (ExtFlash_STATE_WRITE) ;
                        extflash_sub_state = 4 ;
                    }
                    break ;
                case 4 :
                    if (pSPIMemory->index >= pSPIMemory->len) {
                        extflash_sub_state = 5 ;
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
                    SetSPIFlashDriver_state (ExtFlash_STATE_CLR_WEL) ;
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
                    TimerSet(&timer_extflash, 8000) ;
                    SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                    extflash_sub_state = 1 ;
                    break ;
                case 1 :
                    if (TimerHasExpired(&timer_extflash) == TRUE) {
                        TimerStop(&timer_extflash) ;
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
                    SetSPIFlashDriver_state (ExtFlash_STATE_SET_WEL) ;
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
                        extflash_sub_state = 5 ;
                    }   // if (TimerHasExpired(&timer_extflash) == TRUE)
                    else if (preg_status->BUSY == 1) {
                        if (TimerHasExpired(&timer_extflash_status) == TRUE) {
                            SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                            TimerSet(&timer_extflash_status, 20) ;
                        }
                    } else {
                        SetSPIFlashDriver_state (ExtFlash_STATE_CLR_WEL) ;
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
                extflash_cmd_buf [0] = SOFTWARE_DIE_SELECT ;
                extflash_cmd_buf [1] = DIE_ACTIVE ;
                SPIAccess(extflash_cmd_buf, extflash_cmd_buf, 2) ;
                TimerSet(&drv_timer_extflash, 5) ;     //
                drv_extflash_sub_state = 1 ;
                break ;
            case 1 :    // wait for timeout
                if (TimerHasExpired(&drv_timer_extflash) == TRUE) {
                    TimerStop(&drv_timer_extflash) ;
                    drv_extflash_sub_state = 2 ;
                }   // if (TimerHasExpired(&drv_timer_extflash) == TRUE)
                break ;
            case 2 :
                extflash_cmd_buf [0] = W25Q32JV_CMD_Enable_Reset ;
                SPIAccess(extflash_cmd_buf, extflash_cmd_buf, 1) ;
                drv_extflash_sub_state = 3 ;
                break ;
            case 3 :
                TimerStop(&drv_timer_extflash) ;     //

                extflash_cmd_buf [0] = W25Q32JV_CMD_Reset_Device ;
                SPIAccess(extflash_cmd_buf, extflash_cmd_buf, 1) ;
                drv_extflash_sub_state = 4 ;

                TimerSet(&drv_timer_extflash, 15) ;     //
                break ;
            case 4 :    // wait for timeout (15 ms), than transfer to Get ID state
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
                extflash_cmd_buf [0] = W25Q32JV_CMD_MANUFACTURER_ID ;
                SPIAccess(extflash_cmd_buf, extflash_cmd_buf, 6) ;
                drv_extflash_sub_state = 1 ;
                break ;
            case 1 :    // Store Manufacturer and Device ID, than transfer to Get Status state
                Manufacturer_ID = extflash_cmd_buf[4] ;
                Device_ID       = extflash_cmd_buf[5] ;
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
                extflash_cmd_buf [0] = W25Q32JV_CMD_Read_Status_Register_1 ;
                SPIAccess(extflash_cmd_buf, extflash_cmd_buf, 2) ;
                drv_extflash_sub_state = 2 ;
                break ;
            case 2 :    // Send Read Status_Reg2, , store Status_Reg1
                #if (DIE_ACTIVE == DIE_W25Q32JV)
                    preg_status->Reg_Status_L = extflash_cmd_buf[1] ;
                #endif

                memset(extflash_cmd_buf, 0, 2) ;
                extflash_cmd_buf [0] = W25Q32JV_CMD_Read_Status_Register_2 ;
                SPIAccess(extflash_cmd_buf, extflash_cmd_buf, 2) ;
                drv_extflash_sub_state = 3 ;
                break ;
            case 3 :    // Send Read Status_Reg3, store Status_Reg2
                #if (DIE_ACTIVE == DIE_W25Q32JV)
                    preg_status->Reg_Status_M = extflash_cmd_buf[1] ;
                #endif

                memset(extflash_cmd_buf, 0, 2) ;
                extflash_cmd_buf [0] = W25Q32JV_CMD_Read_Status_Register_3 ;
                SPIAccess(extflash_cmd_buf, extflash_cmd_buf, 2) ;
                drv_extflash_sub_state = 4 ;
                break ;
            case 4 :    // Store Status_Reg3
                #if (DIE_ACTIVE == DIE_W25Q32JV)
                    preg_status->Reg_Status_H = extflash_cmd_buf[1] ;
                #endif
                drv_extflash_sub_state = 5 ;
                break ;
            default :
                SetSPIFlashDriver_state (ExtFlash_STATE_IDLE) ;
                break ;
        }   // switch (drv_extflash_sub_state)
    }   // if (GetSPIState() != STATUS_BUSY)
}   // void Handle_ExtFlash_GET_STATUS(void)

void Handle_ExtFlash_CLR_WEL(void) {
    if (GetSPIState() != STATUS_BUSY) {
        SetSPIFree() ;
        switch (drv_extflash_sub_state) {
            case 0 :
                drv_extflash_sub_state = 1 ;
                break ;
            case 1 :
                extflash_cmd_buf [0] = W25Q32JV_CMD_WRITE_DISABLE ;
                SPIAccess(extflash_cmd_buf
                        , extflash_cmd_buf
                        , 1
                        ) ;
                drv_extflash_sub_state = 2 ;
                break ;
            default :
                SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                break ;
        }   // switch (drv_extflash_sub_state)
    }   // if (GetSPIState() != STATUS_BUSY)
}   // void Handle_ExtFlash_CLR_WEL(void)

void Handle_ExtFlash_SET_WEL(void) {
    if (GetSPIState() != STATUS_BUSY) {
        SetSPIFree() ;
        switch (drv_extflash_sub_state) {
            case 0 :
                drv_extflash_sub_state = 1 ;
                break ;
            case 1 :
                extflash_cmd_buf [0] = W25Q32JV_CMD_WRITE_ENABLE ;
                SPIAccess(extflash_cmd_buf
                        , extflash_cmd_buf
                        , 1
                        ) ;
                drv_extflash_sub_state = 2 ;
                break ;
            default :
                SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                break ;
        }   // switch (drv_extflash_sub_state)
    }   // if (GetSPIState() != STATUS_BUSY)
}   // void Handle_ExtFlash_SET_WEL(void)

void Handle_ExtFlash_NOP(void) {
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
                extflash_buf [0] = W25Q32JV_CMD_Fast_Read ;
                extflash_buf [1] = tmp.address_H ;
                extflash_buf [2] = tmp.address_M ;
                extflash_buf [3] = tmp.address_L ;
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
                SPIMemory_Request_Reset() ;
                //SetSPIFlashDriver_state (ExtFlash_STATE_IDLE) ;
                SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
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
                extflash_buf [0] = W25Q32JV_CMD_Page_Program ;
                extflash_buf [1] = tmp.address_H ;
                extflash_buf [2] = tmp.address_M ;
                extflash_buf [3] = tmp.address_L ;

                src = pSPIMemory->len ;
                tar = pSPIMemory->index ;
                ptar = pSPIMemory->buf + pSPIMemory->index ;

                idx = tmp.rw_address & (uint32_t)(0xffU) ;
                for (idx1 = 4; tar < src && idx < MAX_BYTES_PER_PAGE; idx++, idx1++, tar++) {
                    extflash_buf[idx1] = *ptar++ ;
                }   // for (; tar < src && idx < (MAX_BYTES_PER_PAGE + 4); idx++, tar++)

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
        SPIMemory_Request_Reset() ;
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
                    SPIMemory_Request_Reset() ;
                    SetSPIFlashDriver_state (ExtFlash_STATE_GET_STATUS) ;
                    break ;
            }   // switch (drv_extflash_sub_state)
        }   // if (GetSPIState() != STATUS_BUSY)
    }
}   // void Handle_ExtFlash_ERASE (void)

void Handle_ExtFlash_ERASE_SECTOR(void) {
    SRW_ADDR    tmp ;

    tmp.rw_address = pSPIMemory->address & (uint32_t)(-4096) ;

    extflash_cmd_buf [0] = W25Q32JV_CMD_Sector_Erase_4KB ;
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

    extflash_cmd_buf [0] = W25Q32JV_CMD_Block_Erase_32KB ;
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

    extflash_cmd_buf [0] = W25Q32JV_CMD_Block_Erase_64KB ;
    extflash_cmd_buf [1] = tmp.address_H ;
    extflash_cmd_buf [2] = tmp.address_M ;
    extflash_cmd_buf [3] = tmp.address_L ;

    SPIAccess(extflash_cmd_buf
            , extflash_cmd_buf
            , 4
            ) ;
}   // void Handle_ExtFlash_ERASE_SECTOR(void)

void Handle_ExtFlash_ERASE_CHIP(void) {
    extflash_cmd_buf [0] = W25Q32JV_CMD_Chip_Erase ;

    SPIAccess(extflash_cmd_buf
            , extflash_cmd_buf
            , 1
            ) ;
}   // void Handle_ExtFlash_ERASE_SECTOR(void)

void SPIFlash_Read_Req (uint32_t src_address, uint8_t * target, uint32_t length) {
    pSPIMemory->mode = FLAHMEM_OP_READ ;
    pSPIMemory->address = src_address ;
    pSPIMemory->buf = target ;
    pSPIMemory->len = length ;
    pSPIMemory->index = 0 ;
}   // void SPIFlash_Read_Req (uint32_t src, uint8_t * tar, uint32_t length)

void SPIFlash_Write_Req (uint32_t target_address, uint8_t * source, uint32_t length) {
#if (1)
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

}   // void SPIFlash_Write_Req (uint32_t target_address, uint8_t * source, uint32_t length)

void SPIFlash_Erase_Req (FLAHMEM_OP_MODE cmd, uint32_t src_address) {
    if (cmd >= FLAHMEM_OP_ERASE_SECTOR && cmd <= FLAHMEM_OP_ERASE_CHIP) {
        pSPIMemory->mode = cmd ;
        pSPIMemory->address = src_address ;
        pSPIMemory->buf = (uint8_t *)NULL ;
        pSPIMemory->len = 0 ;
        pSPIMemory->index = 0 ;
    }   // if (cmd >= FLAHMEM_OP_ERASE_SECTOR && cmd <= FLAHMEM_OP_ERASE_CHIP)
}   // void SPIFlash_Erase_Req (FLAHMEM_OP_MODE cmd, uint32_t src_address)
