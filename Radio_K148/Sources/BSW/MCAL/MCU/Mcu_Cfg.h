/*******************************************************************************
** Copyright (c) 2018 Embitel
**
** This software is the property of Embitel.
** It can not be used or duplicated without Embitel authorization.
**
** -----------------------------------------------------------------------------
** File Name : Mcu_Cfg.h
** Module Name : MCU
** -----------------------------------------------------------------------------
**
** Description : Micro-controller Clock Unit Module
**
** -----------------------------------------------------------------------------
**
** Documentation reference : LLD_MCU
**
********************************************************************************
** R E V I S I O N H I S T O R Y
********************************************************************************
** V01.00 06/08/2018
** - Baseline Created
*******************************************************************************/
/*************************** Inclusion files **********************************/
#ifndef MCU_CFG_H_
#define MCU_CFG_H_
/*************************** Configuration Parameters  ************************/
/* Clock Selection */
/* #define FIRC_CLOCKSELECTION */

/* Mention the External Oscillator Clock Range*/
#define MCU_OSC_CLK  		 8000000U

#define FIRC_CLOCKSELECTION  //Pinchi 20181214 add

#ifdef FIRC_CLOCKSELECTION
#define MCU_SYSTEM_CLK 		48000000U
#else
/* Required System clock */
#define MCU_SYSTEM_CLK 		40000000U
#endif 

/*  MCU BUS Peripheral clock division value ranges from 2-16 divider
 * EXAMPLE: IF SYSTEM CLOCK IS 80Mhz THEN (BUS_CLK=SYSTEM_CLK/BUS_DIVI)
 * BUS_CLK=80MHz/2=40MHz
 * BUS_CLK=40MHz*/
/* NOTE:- Bus Clock should be less then SYSTEM Clock & core clock */
#define MCU_BUS_CLOCK_DIVI 		1U


#endif /* MCU_CFG_H_ */
