/** Copyright (c) 2018 EMBITEL
**
** This software is the property of Embitel.
** It can not be used or duplicated without Embitel authorization.
**
** -------------------------------------------------------------------------------------------------
** File Name    : GPT_cfg.h
** Module name  : GENERAL PURPOSE TIMER
** -------------------------------------------------------------------------------------------------
** Description : Include file of component GPT_cfg.h
**
** -------------------------------------------------------------------------------------------------
**
****************************************************************************************************
** R E V I S I O N  H I S T O R Y
****************************************************************************************************
** V01.00
** - First release
*/

/**************************************** Inclusion files *****************************************/
/* To avoid multi-inclusions */
#ifndef GPT_CFG_H
#define GPT_CFG_H

#include "Mcu.h"
/****************************************Inclusion files ******************************************/
/**************************************component configuration ************************************/
/* Configurable Value But Keep it untouched .This is the base of Scheduler. */

#define GPT_TIMEOUT_VAL_MS	    1U /*Now it is 1 ms value */

#define GPT_CLK_FREQ			MCU_SYSTEM_CLK

#define GPT_SCALING_HZ		    1000U

/*Formula for GPT timeout*/
#define TVAL_MASK				((GPT_CLK_FREQ/GPT_SCALING_HZ) * GPT_TIMEOUT_VAL_MS)

#endif /* GPT_CFG_H */
