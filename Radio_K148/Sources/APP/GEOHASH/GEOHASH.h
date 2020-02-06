/*
 * GEOHASH.h
 *
 *  Created on: 2020¦~1¤ë7¤é
 *      Author: ALLGO
 */

#ifndef BSW_OBU_FUNC_GEOHASH_GEOHASH_H_
    #define BSW_OBU_FUNC_GEOHASH_GEOHASH_H_

    #define MAX_LAT             90.0
    #define MIN_LAT             -90.0

    #define MAX_LONG            180.0
    #define MIN_LONG            -180.0

    #define NORTH               0
    #define EAST                1
    #define SOUTH               2
    #define WEST                3

    #define LENGTH_OF_DEGREE    111100              // meters

    // Metric in meters
    typedef struct GeoBoxDimensionStruct {
        double          height ;
        double          width ;
    } GeoBoxDimension ;

    typedef struct GeoCoordStruct {
        double          latitude ;
        double          longitude ;

        double          north ;
        double          east ;
        double          south ;
        double          west ;

        GeoBoxDimension dimension ;
    } GeoCoord;


    typedef struct IntervalStruct {
        double high;
        double low;

    } Interval;

    #if defined (__GEOHASH_INS__)
        uint8_t const base32_table[] = {
        //   0     1     2     3     4     5     6     7     8     9     10    11    12    13    14    15
            '0' , '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , 'b' , 'c' , 'd' , 'e' , 'f' , 'g' ,
        //   16    17    18    19    20    21    22    23    24    25    26    27    28    29    30    31
            'h' , 'j' , 'k' , 'm' , 'n' , 'p' , 'q' , 'r' , 's' , 't' , 'u' , 'v' , 'w' , 'x' , 'y' , 'z'
        } ;

        uint8_t const base32_table_reverse[] = {
            // offset = '0',
               0 ,    1 ,    2 ,    3 ,    4 ,    5 ,    6 ,    7 ,    8 ,    9 ,
            0xff , 0xff , 0xff , 0xff , 0xff , 0xff , 0xff , 0xff ,   10 ,   11 ,
              12 ,   13 ,   14 ,   15 ,   16 , 0xff ,   17 ,   18 , 0xff ,   19 ,
              20 , 0xff ,   21 ,   22 ,   23 ,   24 ,   25 ,   26 ,   27 ,   28 ,
              29 ,   30 ,   31 , 0xff , 0xff , 0xff , 0xff , 0xff , 0xff , 0xff ,
              10 ,   11 ,   12 ,   13 ,   14 ,   15 ,   16 , 0xff ,   17 ,   18 ,
            0xff ,   19 ,   20 , 0xff ,   21 ,   22 ,   23 ,   24 ,   25 ,   26 ,
              27 ,   28 ,   29 ,   30 ,   31
        } ;

        uint8_t const neighborhood_v [] = {
            // prxz
            'p' , 'r' , 'x' , 'z' ,
            // nqwy
            'n' , 'q' , 'w' , 'y' ,
            // jmtv
            'j' , 'm' , 't' , 'v' ,
            // hksu
            'h' , 'k' , 's' , 'u' ,
            // 57eg
            '5' , '7' , 'e' , 'g' ,
            // 46df
            '4' , '6' , 'd' , 'f' ,
            // 139c
            '1' , '3' , '9' , 'c' ,
            // 028b
            '0' , '2' , '8' , 'b'
        };

        unsigned char const neighborhood_h [] = {
            // 0145hjnp
            '0' , '1' , '4' , '5' , 'h' , 'j' , 'n' , 'p' ,
            // 2367kmqr
            '2' , '3' , '6' , '7' , 'k' , 'm' , 'q' , 'r' ,
            // 89destwx
            '8' , '9' , 'd' , 'e' , 's' , 't' , 'w' , 'x' ,
            // bcfguvyz
            'b' , 'c' , 'f' , 'g' , 'u' , 'v' , 'y' , 'z'
        };
        /* Normal 32 characer map used for geohashing */
        char const char_map[32] =  "0123456789bcdefghjkmnpqrstuvwxyz";

        /*
         *  The follow character maps were created by Dave Troy and used in his Javascript Geohashing
         *  library. http://github.com/davetroy/geohash-js
         */
        char * even_neighbors[] = { "p0r21436x8zb9dcf5h7kjnmqesgutwvy",
                                    "bc01fg45238967deuvhjyznpkmstqrwx",
                                    "14365h7k9dcfesgujnmqp0r2twvyx8zb",
                                    "238967debc01fg45kmstqrwxuvhjyznp"
                                   };

        char * odd_neighbors[] = { "bc01fg45238967deuvhjyznpkmstqrwx",
                                   "p0r21436x8zb9dcf5h7kjnmqesgutwvy",
                                   "238967debc01fg45kmstqrwxuvhjyznp",
                                   "14365h7k9dcfesgujnmqp0r2twvyx8zb"
                                 };

        char * even_borders[] = {"prxz", "bcfguvyz", "028b", "0145hjnp"};
        char * odd_borders[] = {"bcfguvyz", "prxz", "0145hjnp", "028b"};

        #undef  __GEOHASH_INS__
    #else
        extern uint8_t const base32_table[] ;
        extern uint8_t const base32_table_reverse[] ;
        extern uint8_t const neighborhood_v [] ;
        extern uint8_t const neighborhood_h [] ;

        /*
         * Creates a the hash at the specified precision. If precision is set to 0.
         * or less than it defaults to 12.
         */
        //extern char* geohash_encode(char *ptr, double lat, double lng, int precision);
        extern int geohash_encode(char *ptr, double lat, double lng, int precision);

        /*
         * Returns the latitude and longitude used to create the hash along with
         * the bounding box for the encoded coordinate.
         */
        extern GeoCoord geohash_decode(char* hash);

        /*
         * Return an array of geohashes that represent the neighbors of the passed
         * in value. The neighbors are indexed as followed:
         *
         *                  N, NE, E, SE, S, SW, W, NW
         *                  0, 1,  2,  3, 4,  5, 6, 7
         */
        extern char** geohash_neighbors(char* hash);

        /*
         * Returns the width and height of a precision value.
         */
        extern GeoBoxDimension geohash_dimensions_for_precision(int precision);
    #endif  // #if defined (__GEOHASH_INS__)
#endif /* BSW_OBU_FUNC_GEOHASH_GEOHASH_H_ */
