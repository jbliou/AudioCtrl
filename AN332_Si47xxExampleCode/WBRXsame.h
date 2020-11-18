//-----------------------------------------------------------------------------
//
// WBRXsame.h
//
// Contains the function prototypes for the functions contained in WBRXsame.c
//
//-----------------------------------------------------------------------------
#ifndef _WBRXSAME_H_
#define _WBRXSAME_H_
#include "typedefs.h"
#include "commanddefs.h"
//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
extern u8 xdata SameDataAvailable;

//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
extern void updateSame(void);
extern void initSameVars(void);
#endif
