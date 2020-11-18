//-----------------------------------------------------------------------------
//
// si47xxAMRX.h
//
// Contains the function prototypes for the functions contained in si47xxAMRX.c
//
//-----------------------------------------------------------------------------
#ifndef _SI47XXAMRX_H_
#define _SI47XXAMRX_H_

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
extern bit WaitSTCInterrupt;
extern bit PoweredUp;

//-----------------------------------------------------------------------------
// Function prototypes for si47xxAMRX.c
//-----------------------------------------------------------------------------
void si47xxAMRX_powerup(void);
void si47xxAMRX_powerdown(void);
void si47xxAMRX_initialize(void);
void si47xxAMSWLWRX_initialize(void);
void si47xxAMRX_configure(void);
void si47xxAMRX_set_volume(u8 volume);
void si47xxAMRX_mute(u8 mute);
u8 si47xxAMRX_tune(u16 frequency);
u8 si47xxAMRX_seek(u8 seekup, u8 seekmode);
u16 si47xxAMRX_get_frequency(void);
u8 si47xxAMRX_get_rssi(void);

//-----------------------------------------------------------------------------
// Function prototypes for AMRXautoseek.c
//-----------------------------------------------------------------------------
u8 si47xxAMRX_autoseek(void);

//-----------------------------------------------------------------------------
// Function prototypes for si47xx_low.c
//-----------------------------------------------------------------------------
void si47xx_set_property(u16 propNumber, u16 propValue);
void si47xx_getPartInformation(void);

#endif
