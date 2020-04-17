/*
 * w25q32jv.h
 *
 *  Created on: 2020年1月22日
 *      Author: ALLGO
 */

#ifndef BSW_MCAL_EXTFLASH_W25Q32JV_H_
    #define BSW_MCAL_EXTFLASH_W25Q32JV_H_

    #define MAX_BYTES_PER_PAGE          (256U)
    #define MAX_DUMMY_BYTES             (8U)
    #define MAX_RECEIVING_BYTES         (MAX_DUMMY_BYTES + MAX_BYTES_PER_PAGE)
    #define MAX_TRANSMITING_BYTES       (16)

    enum {
        IsBusy              = 1
        , Write_Enable      = 1
    };

    typedef struct {
        union {
            struct {
                uint8_t     BUSY    : 1 ;       // Erase/Write In Progress
                uint8_t     WEL     : 1 ;       // Write Enable Latch
                uint8_t     BP      : 3 ;       // Block Protect Bits
                uint8_t     TB      : 1 ;       // Top/Bottom Block Protect
                uint8_t     SEC     : 1 ;       // Sector/Block Protect Bit
                uint8_t     dummy0  : 1 ;       // Reserved
            };
            uint8_t Reg_Status_L ;
        };

        union {
            struct {
                uint8_t     SRL     : 1 ;       // Status Register Lock bit
                uint8_t     QE      : 1 ;       // Quad Enable
                uint8_t     dummy1  : 1 ;       // Reserved
                uint8_t     LB      : 3 ;       // Security Register Lock Bits
                uint8_t     CMP     : 1 ;       // Complement Protect
                uint8_t     SUS     : 1 ;       // Erase/Program Suspend Status
            };
            uint8_t Reg_Status_M ;
        };

        union {
            struct {
                uint8_t     dummy2_0: 2 ;       // Reserved
                uint8_t     WPS     : 1 ;       // Write Protect Selection
                uint8_t     dummy2_1: 2 ;       // Reserved
                uint8_t     DRV     : 2 ;       // Erase/Program Suspend Status
                uint8_t     dummy2_2: 1 ;       // Reserved
            };
            uint8_t Reg_Status_H ;
        };
    } W25Q32JV_STSTUS_REG, * PW25Q32JV_STSTUS_REG ;

    typedef enum {
        W25Q32JV_CMD_ENABLE_RESET                   = (0x66U)
        , W25Q32JV_CMD_RESET_DEVICE                 = (0x99U)
        , W25Q32JV_CMD_Write_Enable                 = (0x06U)
        , W25Q32JV_CMD_VOLATILE_SR_WRITE_ENABLE     = (0x50U)
        , W25Q32JV_CMD_Write_Disable                = (0x04U)
        , W25Q32JV_CMD_RELEASE_POWER_DOWN           = (0xabU)
        , W25Q32JV_CMD_Manufacturer_ID              = (0x90U)
        , W25Q32JV_CMD_JEDEC_ID                     = (0x9fU)
        , W25Q32JV_CMD_Read_Unique_ID               = (0x4bU)
        , W25Q32JV_CMD_Read_Data                    = (0x03U)
        , W25Q32JV_CMD_Fast_Read                    = (0x0bU)
        , W25Q32JV_CMD_Page_Program                 = (0x02U)
        , W25Q32JV_CMD_Sector_Erase_4KB             = (0x20U)
        , W25Q32JV_CMD_Block_Erase_32KB             = (0x52U)
        , W25Q32JV_CMD_Block_Erase_64KB             = (0xd8U)
        , W25Q32JV_CMD_Chip_Erase                   = (0xc7U)
        , W25Q32JV_CMD_Read_Status_Register_1       = (0x05U)
        , W25Q32JV_CMD_Read_Status_Register_2       = (0x35U)
        , W25Q32JV_CMD_Read_Status_Register_3       = (0x15U)
        , W25Q32JV_CMD_Read_SFDP_Register           = (0x5aU)
        , W25Q32JV_CMD_Erase_Security_Register      = (0x44U)
        , W25Q32JV_CMD_Program_Security_Register    = (0x42U)
        , W25Q32JV_CMD_Read_Security_Register       = (0x48U)
        , W25Q32JV_CMD_Global_Block_Lock            = (0x7eU)
        , W25Q32JV_CMD_Global_Block_Unlock          = (0x98U)
        , W25Q32JV_CMD_Read_Block_Lock              = (0x3dU)
        , W25Q32JV_CMD_Individual_Block_Lock        = (0x36U)
        , W25Q32JV_CMD_Individual_Block_Unlock      = (0x39U)
        , W25Q32JV_CMD_Erase_Program_Suspend        = (0x75U)
        , W25Q32JV_CMD_Erase_Program_Resume         = (0x7aU)
        , W25Q32JV_CMD_Power_Down                   = (0xb9U)
        , W25Q32JV_CMD_Enable_Reset                 = (0x66U)
        , W25Q32JV_CMD_Reset_Device                 = (0x99U)
    } W25Q32JV_COMMAND ;

#endif /* BSW_MCAL_EXTFLASH_W25Q32JV_H_ */
