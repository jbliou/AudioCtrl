/*
 * GPS.h
 *
 *  Created on: 2020¦~1¤ë6¤é
 *      Author: ALLGO
 */

#ifndef BSW_OBU_DEVICES_GPS_GPS_H_
#define BSW_OBU_DEVICES_GPS_GPS_H_

    #define GPS_GPS_TEST_PATTERN    0
    //#define GPS_GPS_TEST_PATTERN    1

    #define NMEA_FORMAT_xxGGA       1
    #define NMEA_FORMAT_xxGGA       1
    #define NMEA_FORMAT_xxRMC       2
    //#define NMEA_FILTER             NMEA_FORMAT_xxGGA
    #define NMEA_FILTER             NMEA_FORMAT_xxRMC

    typedef struct {
    #if (NMEA_FILTER == NMEA_FORMAT_xxRMC)
        uint16_t    year ;
        uint8_t     month ;
        uint8_t     day ;
        uint8_t     week ;
    #endif  // #if (NMEA_FILTER == NMEA_FORMAT_xxRMC)
        uint8_t     hour ;
        uint8_t     minute ;
        uint8_t     second ;
    } SGPS_TIME , * PGPSTIME ;

    typedef enum {
        STATE_FILT_STAGE1   = 0 ,   // $
        STATE_FILT_STAGE2   ,       // G
        STATE_FILT_STAGE3   ,       // N
        STATE_FILT_STAGE4   ,       // G
        STATE_FILT_STAGE5   ,       // G
        STATE_FILT_STAGE6   ,       // A
        STATE_GET_GPSDATA   ,       //
        STATE_GET_TAIL      ,       //
    } UART_STATE ;

    typedef enum {
        GPS_HANDLE_STATE_INIT       = 0 ,
        GPS_HANDLE_STATE_GETDATA        ,
        GPS_HANDLE_STATE_ANALYSIS       ,
        GPS_HANDLE_STATE_FIND_LATITUDE  ,
        GPS_HANDLE_STATE_FIND_LONGITUDE ,
        GPS_HANDLE_STATE_FIND_TIME      ,
        GPS_HANDLE_STATE_FIND_DATE      ,
        GPS_HANDLE_STATE_RESET      ,
        GPS_HANDLE_STATE_MAX
    } GPS_HANDLE_STATE ;

    typedef void (* GPS_Process)(void) ;

    #if defined(__GPS_INS__)
        uint8_t                 Tmr_GPS = 0 ;
        SGPS_TIME               system_time ;
        UART_STATE              FiltState = STATE_FILT_STAGE1 ;
        GPS_HANDLE_STATE        gps_handle_state = GPS_HANDLE_STATE_INIT ;

        #if (NMEA_FILTER == NMEA_FORMAT_xxGGA)
            const uint8_t       gps_header [] = "$GNGGA" ;
        #elif (NMEA_FILTER == NMEA_FORMAT_xxRMC)
            const uint8_t       gps_header [] = "$GNRMC" ;
            const uint8_t       month_offset [] = {6, 2, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4} ;
            const uint8_t       century_offset [] = {0, 5, 3, 1, 0} ;
        #endif

        #if (GPS_GPS_TEST_PATTERN == 1)
            #if (NMEA_FILTER == NMEA_FORMAT_xxGGA)
                const uint8_t   test_patterm1 [] = "$GNGGA,104230.00,2456.3284832,N,12107.8042908,E,1,12,99.99,221.231,M,-33.698,M,,*4A\n\r" ;
                const uint8_t   test_patterm2 [] = "$GNGGA,104230.00,2449.9843639,N,12100.8600513,E,1,12,99.99,221.231,M,-33.698,M,,*4A\n\r" ;
            #elif (NMEA_FILTER == NMEA_FORMAT_xxRMC)
                const uint8_t   test_patterm1 [] = "$GPRMC,031939.997,A,2456.3284832,N,12107.8042908,E,000.0,000.0,021219,003.1,W*69\n\r" ;
                const uint8_t   test_patterm2 [] = "$GPRMC,031939.997,A,2449.9843639,N,12100.8600513,E,000.0,000.0,021219,003.1,W*69\n\r" ;
            #endif
            char                activeTestPattern = 1 ;
            static int          TestPatternIdx = 0 ;
        #endif  // #if (GPS_GPS_TEST_PATTERN == 1)
        char                    data_gps [2][BUFFER_SIZE_GPS] = {{0},};
        char *                  pdata_gps = data_gps [0] ;
        unsigned char           gps_rx_ready = 0xff ;
        unsigned char           gps_rx_active = 0 ;
        unsigned char           bufferIdx;

        int                     nmea_comma_posn [2][20] = {0,} ;
        int                     nmea_comma_posn_idx ;
        double                  GPS_Lat_point, GPS_Lon_point ;
        int                     geohash_code ;
        int                     geohash_code_old = 0 ;
        char                    hash[6] ;

        void UartDataFilterByte (uint8_t rx_data) ;

        void Handle_GPS_INIT(void) ;
        void Handle_GPS_GETDATA(void) ;
        void Handle_GPS_ANALYSIS (void) ;
        void Handle_GPS_FIND_LATITUDE(void) ;
        void Handle_GPS_FIND_LONGITUDE(void) ;
        void Handle_GPS_FIND_DATE(void) ;
        void Handle_GPS_FIND_TIME (void) ;
        void Handle_GPS_RESET (void) ;

        #undef __GPS_INS__
    #else
        extern void Handle_GPS(void) ;
    #endif  //#if defined(__GPS_INS__)

#endif /* BSW_OBU_DEVICES_GPS_GPS_H_ */
