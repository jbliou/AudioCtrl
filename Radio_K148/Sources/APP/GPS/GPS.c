/*
 * GPS.c
 *
 *  Created on: 2020�~1��6��
 *      Author: ALLGO
 */

#include "board.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "Uart.h"
#include "extflash.h"

#define __GPS_INS__
#include "Gps.h"

#include "Geohash.h"

const GPS_Process pHandleGPS[] = {
    Handle_GPS_INIT ,
    Handle_GPS_GETDATA ,
    Handle_GPS_ANALYSIS ,
    Handle_GPS_FIND_LATITUDE ,
    Handle_GPS_FIND_LONGITUDE ,
    Handle_GPS_FIND_TIME ,
    Handle_GPS_FIND_DATE ,
    Handle_GPS_RESET
};

void Handle_GPS(void) {
    if (gps_handle_state < GPS_HANDLE_STATE_MAX) {
        (pHandleGPS[gps_handle_state])() ;
    }   // if (gps_handle_state < GPS_HANDLE_STATE_MAX)
    else {
        gps_handle_state = GPS_HANDLE_STATE_INIT ;
    }
    Handle_find_zip() ;
}   // void Handle_GPS(void)

void UartDataFilterByte (uint8_t rx_data) {
    uint8_t getch ;

    getch = rx_data ;
    //*rx_data = 0 ;

    if (bufferIdx >= BUFFER_SIZE_GPS) {
        bufferIdx = 0 ;
        pdata_gps = data_gps [gps_rx_active] ;
        FiltState = STATE_FILT_STAGE1 ;
    }
    switch (FiltState) {
        case STATE_FILT_STAGE1 : // $
            if (getch == gps_header[0]) {
                nmea_comma_posn_idx = 0 ;
                *pdata_gps++ = getch ;
                bufferIdx ++ ;
                FiltState ++ ;
            }   // if (get_ch == '$')
            else {
                bufferIdx = 0 ;
                pdata_gps = data_gps [gps_rx_active] ;
                FiltState = STATE_FILT_STAGE1 ;
            }
            break ;
        case STATE_FILT_STAGE3 : // N
            *pdata_gps++ = getch ;
            bufferIdx ++ ;
            FiltState ++ ;
            break ;
        case STATE_FILT_STAGE2 : // G
        case STATE_FILT_STAGE4 : // G
        case STATE_FILT_STAGE5 : // G
        case STATE_FILT_STAGE6 : // A
            if (getch == gps_header[FiltState]) {
                *pdata_gps++ = getch ;
                bufferIdx ++ ;
                FiltState ++ ;
            }   // if (getch == gps_header[FiltState])
            else {
                bufferIdx = 0 ;
                pdata_gps = data_gps [gps_rx_active] ;
                FiltState = STATE_FILT_STAGE1 ;
            }
            break ;
        case STATE_GET_GPSDATA : //
            if (getch == 10 || getch == 13) {
                *pdata_gps++ = 0 ;
                bufferIdx ++ ;
                FiltState ++ ;
            }   // if (get_ch == 10 || get_ch == 13)
            else {
                if (getch == ',') {
                    nmea_comma_posn[gps_rx_active][nmea_comma_posn_idx++] = bufferIdx ;
                }
                *pdata_gps++ = getch ;
                bufferIdx ++ ;
            }
            break ;
        case STATE_GET_TAIL :    //
            *pdata_gps++ = 0 ;
            bufferIdx ++ ;
            gps_rx_ready = (gps_rx_active++) & 1 ;
            gps_rx_active &= 1 ;
            FiltState = STATE_FILT_STAGE1 ;
            return ;
        default :
            FiltState = STATE_FILT_STAGE1 ;
            break ;
    }   // switch (FiltState)
}   // void UartDataFilterByte (char * rx_data)

void Handle_GPS_INIT(void) {
    gps_handle_state    = GPS_HANDLE_STATE_GETDATA ;
    //GPS_nRESET_PT->PSOR |= (1 << GPS_nRESET_POS) ;
}   // void Handle_GPS_INIT(void)

void Handle_GPS_GETDATA(void) {
    status_t    ret_state ;
    uint8_t     udata ;

    ret_state = LPUART_DRV_GetReceiveQueueStatus(INST_LPUART2, &udata) ;
    if (ret_state == STATUS_SUCCESS) {
#if (GPS_GPS_TEST_PATTERN == 1)
        if (FiltState == STATE_FILT_STAGE6) {
            TestPatternIdx = FiltState + 1 ;
        }
        if (FiltState > STATE_FILT_STAGE6) {
            if (activeTestPattern == 1) {
                udata = test_patterm1[TestPatternIdx] ;
            }
            else if (activeTestPattern == 2) {
                udata = test_patterm2[TestPatternIdx] ;
            }
            if (udata == 0x0A || udata == 0x0D) {
                TestPatternIdx = 0 ;
            } else {
                TestPatternIdx++ ;
            }
        }   // if (FiltState == STATE_FILT_STAGE6)
#endif  // #if (GPS_GPS_TEST_PATTERN == 1)
        UartDataFilterByte(udata) ;
        if (gps_rx_ready != 0xff) {
            gps_handle_state    = GPS_HANDLE_STATE_ANALYSIS ;
#ifdef EVB
            //PINS_DRV_TogglePins(LED_RED_PORT, (1 << LED_RED));
#endif  // #ifdef EVB
        }   // if (gps_rx_ready != 0xff)
    }   // if (ret_state == STATUS_SUCCESS)
}   // void Handle_GPS_GETDATA(void)

void Handle_GPS_ANALYSIS(void) {
    int latitude_field_len, longitude_field_len ;

#if (NMEA_FILTER == NMEA_FORMAT_xxGGA)
    latitude_field_len = nmea_comma_posn[gps_rx_ready][2] - nmea_comma_posn[gps_rx_ready][1] ;
    longitude_field_len = nmea_comma_posn[gps_rx_ready][4] - nmea_comma_posn[gps_rx_ready][3] ;
#elif (NMEA_FILTER == NMEA_FORMAT_xxRMC)
    latitude_field_len = nmea_comma_posn[gps_rx_ready][3] - nmea_comma_posn[gps_rx_ready][2] ;
    longitude_field_len = nmea_comma_posn[gps_rx_ready][5] - nmea_comma_posn[gps_rx_ready][4] ;
#endif
    if (latitude_field_len > 4 && longitude_field_len > 4) {
        gps_handle_state = GPS_HANDLE_STATE_FIND_LATITUDE ;
    }   // if (latitude_field_len > 4 && longitude_field_len > 4)
    else if ((nmea_comma_posn[gps_rx_ready][1] - nmea_comma_posn[gps_rx_ready][0]) > 6) {
        gps_handle_state = GPS_HANDLE_STATE_FIND_TIME ;
    }
    else {
        gps_handle_state = GPS_HANDLE_STATE_RESET ;
    }
}   // Handle_GPS_ANALYSIS

void Handle_GPS_FIND_LATITUDE(void) {
    int     GPS_Lat_int ;
    double  latitude ;

#if (NMEA_FILTER == NMEA_FORMAT_xxGGA)
    latitude = strtod (data_gps[gps_rx_ready]+nmea_comma_posn[gps_rx_ready][1]+1, NULL) ;
#elif (NMEA_FILTER == NMEA_FORMAT_xxRMC)
    latitude = strtod (data_gps[gps_rx_ready]+nmea_comma_posn[gps_rx_ready][2]+1, NULL) ;
#endif
    GPS_Lat_int = (int)(latitude / 100.0) ;
    GPS_Lat_point = (((latitude / 100.0 - (double)GPS_Lat_int) / 60.0) * 100.0);

    GPS_Lat_point = (double)GPS_Lat_int + GPS_Lat_point;

    gps_handle_state = GPS_HANDLE_STATE_FIND_LONGITUDE ;
}   // void Handle_GPS_FIND_LATITUDE(void)

void Handle_GPS_FIND_LONGITUDE(void) {
    int     GPS_Lon_int ;
    double  longitude ;

#if (NMEA_FILTER == NMEA_FORMAT_xxGGA)
    longitude = strtod (data_gps[gps_rx_ready]+nmea_comma_posn[gps_rx_ready][3]+1, NULL) ;
#elif (NMEA_FILTER == NMEA_FORMAT_xxRMC)
    longitude = strtod (data_gps[gps_rx_ready]+nmea_comma_posn[gps_rx_ready][4]+1, NULL) ;
#endif
    GPS_Lon_int = (int)(longitude / 100.0) ;
    GPS_Lon_point = (((longitude / 100.0 - (double)GPS_Lon_int) / 60.0) * 100.0);

    GPS_Lon_point = (double)GPS_Lon_int + GPS_Lon_point;

    geohash_code = geohash_encode(hash ,GPS_Lat_point, GPS_Lon_point, sizeof(hash));
    if ((nmea_comma_posn[gps_rx_ready][1] - nmea_comma_posn[gps_rx_ready][0]) > 6) {
        gps_handle_state = GPS_HANDLE_STATE_FIND_TIME ;
    }   //if ((nmea_comma_posn[gps_rx_ready][1] - nmea_comma_posn[gps_rx_ready][0]) > 6)
    else {
        gps_handle_state = GPS_HANDLE_STATE_RESET ;
    }
}   // void Handle_GPS_FIND_LONGITUDE(void)

void Handle_GPS_FIND_TIME (void) {
    uint8_t idx ;

    idx = nmea_comma_posn[gps_rx_ready][0] ;
    // get time from GNGGA format
    system_time.hour    =  (data_gps[gps_rx_ready][idx + 1] - '0') * 10 ;
    system_time.hour    += (data_gps[gps_rx_ready][idx + 2] - '0') ;
    system_time.hour    +=  8 ;
    system_time.hour    %= 24 ;
    system_time.minute  =  (data_gps[gps_rx_ready][idx + 3] - '0') * 10 ;
    system_time.minute  += (data_gps[gps_rx_ready][idx + 4] - '0') ;
    system_time.second  =  (data_gps[gps_rx_ready][idx + 5] - '0') * 10 ;
    system_time.second  += (data_gps[gps_rx_ready][idx + 6] - '0') ;
#if (NMEA_FILTER == NMEA_FORMAT_xxRMC)
    if ((nmea_comma_posn[gps_rx_ready][9] - nmea_comma_posn[gps_rx_ready][8]) > 5) {
        gps_handle_state = GPS_HANDLE_STATE_FIND_DATE ;
    }
    else
#endif
    {
        gps_handle_state = GPS_HANDLE_STATE_RESET ;
    }
}   // void Handle_GPS_FIND_TIME (void)

void Handle_GPS_FIND_DATE(void) {
#if (NMEA_FILTER == NMEA_FORMAT_xxRMC)
    uint8_t idx ;

    idx = nmea_comma_posn[gps_rx_ready][8] ;

    system_time.day     =  (data_gps[gps_rx_ready][idx + 1] - '0') * 10 ;
    system_time.day     += (data_gps[gps_rx_ready][idx + 2] - '0') ;
    system_time.month   =  (data_gps[gps_rx_ready][idx + 3] - '0') * 10 ;
    system_time.month   += (data_gps[gps_rx_ready][idx + 4] - '0') ;
    system_time.year    =  (data_gps[gps_rx_ready][idx + 5] - '0') * 10 ;
    system_time.year    += (data_gps[gps_rx_ready][idx + 6] - '0') ;

    /*
    idx  = system_time.year + system_time.month + system_time.day + system_time.year / 4  - 1 ;
    if (system_time.month <= 1) {
        idx  += 12 ;
    }
    system_time.week = idx % 7 ;
    */
    idx = system_time.day + month_offset[system_time.month - 1] + system_time.year + system_time.year / 4 + century_offset [(20-1)%5] - 1 ;
    system_time.week = idx % 7 ;
    system_time.year = system_time.year + 2000 ;
#endif

    gps_handle_state = GPS_HANDLE_STATE_RESET ;
}   // void Handle_GPS_FIND_DATE(void)

void Handle_GPS_RESET (void) {
    memset (data_gps[gps_rx_ready]          , 0, BUFFER_SIZE_GPS) ;
    memset (nmea_comma_posn[gps_rx_ready]   , 0, 20 * sizeof (int)) ;
    gps_rx_ready = 0xff ;
    gps_handle_state = GPS_HANDLE_STATE_GETDATA ;
#ifdef EVB
    //PINS_DRV_TogglePins(LED_RED_PORT, (1 << LED_RED));
#endif  // #ifdef EVB
}   // void Handle_GPS_FIND_TIME (void)

void boundry_ready(void) {
    if (flag_zip == 0xaa) {
        geohash_boundry_m &= (uint32_t)(-256) ;
        find_zip_state = ZIP_HANDLE_STATE_FIND_NEW_BOUNDRY ;
    }   // if (flag_zip == 0xaa)
    else {
        find_zip_state = ZIP_HANDLE_STATE_LOAD_BOUNDRY ;
    }   // if (flag_zip != 0xaa)
}   // void boundry_ready(void)

void zip_ready(void) {
    if (flag_zip == 0xaa) {
        find_zip_state = ZIP_HANDLE_STATE_GET_ZIP_DATA ;
    }   // if (flag_zip == 0xaa)
    else {
        find_zip_state = ZIP_HANDLE_STATE_LOAD_ZIP_DATA ;
    }   // if (flag_zip != 0xaa)
}   // void zip_ready(void)

void Handle_find_zip(void) {
    uint32_t    tmp ;

    switch (find_zip_state) {
        case ZIP_HANDLE_STATE_WAIT :
            if (get_extFlashConfig() == STATUS_SUCCESS) {
                find_zip_state = ZIP_HANDLE_STATE_CHECK_NEW_GEOHASH ;
            }   // if (get_extFlashConfig() == STATUS_SUCCESS)
            break ;
        case ZIP_HANDLE_STATE_CHECK_NEW_GEOHASH :
            if (geohash_code != geohash_code_old) {
                tmp = geohash_code & (uint32_t)(-256) ;
                geohash_code_old = tmp ;
                geohash_boundry_l = 0 ;
                geohash_boundry_h = pconfig_table->root_records - 1 ;
                geohash_boundry_m = pconfig_table->geohash_prefix_middle ;
                find_zip_state = ZIP_HANDLE_STATE_FIND_NEW_BOUNDRY ;
#ifdef EVB
                //PINS_DRV_TogglePins(LED_RED_PORT, (1 << LED_RED));
#endif  // #ifdef EVB
            }   // if (geohash_code != geohash_code_old)
            break ;
        case ZIP_HANDLE_STATE_LOAD_BOUNDRY :
            tmp = (geohash_boundry_h + geohash_boundry_l) / 2 ;
            tmp <<= 2 ;
            flag_zip = 0x01 ;
            SPIFlash_Read_Req (pconfig_table->root_base_addr + tmp
                              , (uint8_t *)&geohash_boundry_m
                              , sizeof(uint32_t)
                              , &flag_zip
                              , boundry_ready
                              ) ;
            find_zip_state = ZIP_HANDLE_STATE_WAIT_LOADING_PROCESS ;
            break ;
        case ZIP_HANDLE_STATE_FIND_NEW_BOUNDRY :
            tmp = (geohash_boundry_h + geohash_boundry_l) / 2 ;
            if (geohash_code_old == geohash_boundry_m) {
                geohash_code_old = geohash_code ;
                find_zip_state = ZIP_HANDLE_STATE_LOAD_ZIP_DATA ;    // get
            }
            else {
                if (geohash_boundry_h == geohash_boundry_l) {
                    geohash_code_old = geohash_code ;
                    find_zip_state = ZIP_HANDLE_STATE_CHECK_NEW_GEOHASH ;
                }   // if (geohash_boundry_h == geohash_boundry_l)
                else if (geohash_code_old > geohash_boundry_m) {
                    geohash_boundry_l = tmp + 1 ;
                }
                else {
                    geohash_boundry_h = tmp - 1 ;
                }
                find_zip_state = ZIP_HANDLE_STATE_LOAD_BOUNDRY ;
            }
            break ;
        case ZIP_HANDLE_STATE_LOAD_ZIP_DATA :
            tmp = (geohash_boundry_h + geohash_boundry_l) / 2 ;
            tmp *= ZIP_BLOCK_SIZE ;
            tmp += (geohash_code & 0xff) * 4 ;
            flag_zip = 0x01 ;

            SPIFlash_Read_Req (pconfig_table->zip_table_base + tmp
                              , (uint8_t *)&zip_code
                              , sizeof(uint32_t)
                              , &flag_zip
                              , zip_ready
                              ) ;
            find_zip_state = ZIP_HANDLE_STATE_WAIT_LOADING_PROCESS ;
            break ;
        case ZIP_HANDLE_STATE_GET_ZIP_DATA :
            find_zip_state = ZIP_HANDLE_STATE_CHECK_NEW_GEOHASH ;
#ifdef EVB
            //PINS_DRV_TogglePins(LED_RED_PORT, (1 << LED_RED));
#endif  // #ifdef EVB
            break ;
        case ZIP_HANDLE_STATE_WAIT_LOADING_PROCESS :
            break ;
        default :
            break ;
    }   // switch (find_zip_state)
}
