/***************************************************************************************************
** Copyright (c) 2018 Embitel
**
** This software is the property of Embitel.
** It can not be used or duplicated without Embitel authorization.
**
** -------------------------------------------------------------------------------------------------
** File Name     : WDT.Cfg.h
** Module Name   : WDT
** -------------------------------------------------------------------------------------------------
**
** Description : Watchdog Target
**
** -------------------------------------------------------------------------------------------------
**
** Documentation reference :
**
****************************************************************************************************
** R E V I S I O N H I S T O R Y
****************************************************************************************************
** V01.00 10/08/2018
** - Baseline Created
** -------------------------------------------------------------------------------------------------
***************************************************************************************************/
/* To avoid multi-inclusions */
#ifndef WDT_CFG_H_
#define WDT_CFG_H_
/********************************** Declaration of global macros **********************************/
#define WDT_TIMEOUT_VAL_MS				500U

#define WDT_CLK_FROM_LPO_FREQ			128U

#define WDT_SCALING_HZ					1U

/*Formula to for WDT timeout*/
#define WDT_CFG_TIMEOUT					((WDT_CLK_FROM_LPO_FREQ * WDT_TIMEOUT_VAL_MS)/ WDT_SCALING_HZ)

/* @brief The value used to set WDOG source clock from LPO. */
#define FEATURE_WDOG_CLK_FROM_LPO                       (0x1UL)

#define WDOG_PRES_EN					(0x1UL)

/* @brief The 32-bit value used for resetting the WDOG counter. */
#define FEATURE_WDOG_TRIGGER_VALUE                      (0xB480A602U)

/* @brief The 32-bit value used for unlocking the WDOG. */
#define FEATURE_WDOG_UNLOCK_VALUE                       (0xD928C520U)

#endif /* WDT_CFG_H_ */
