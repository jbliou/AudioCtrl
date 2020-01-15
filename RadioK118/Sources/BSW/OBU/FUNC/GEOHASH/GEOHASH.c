/*
 * GEOHASH.c
 *
 *  Created on: 2020¦~1¤ë7¤é
 *      Author: ALLGO
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define __GEOHASH_INS__
#include "GEOHASH.h"

unsigned int index_for_char(char c, char *string) {
    int index = -1;
    int string_amount = strlen(string);
    int i;

    for(i = 0; i < string_amount; i++) {
        if(c == string[i]) {
            index = i;
            break;
        }
    }

    return index;
}

char* get_neighbor(char *hash, int direction) {

    int hash_length = strlen(hash);

    char last_char = hash[hash_length - 1];

    int is_odd = hash_length % 2;
#if (0)
    char **border = is_odd ? odd_borders : even_borders;
    char **neighbor = is_odd ? odd_neighbors : even_neighbors;
#else
    char **border ;
    char **neighbor ;

    if (is_odd) {
        border = odd_borders ;
        neighbor = odd_neighbors ;
    }
    else {
        border = even_borders ;
        neighbor = even_neighbors ;
    }
#endif  // #if (0)


    char *base = malloc(sizeof(char) * 1);
    base[0] = '\0';
    strncat(base, hash, hash_length - 1);

    if(index_for_char(last_char, border[direction]) != -1) {
        base = get_neighbor(base, direction);
    }

    int neighbor_index = index_for_char(last_char, neighbor[direction]);
    last_char = char_map[neighbor_index];

    char *last_hash = malloc(sizeof(char) * 2);
    last_hash[0] = last_char;
    last_hash[1] = '\0';
    strcat(base, last_hash);
    free(last_hash);

    return base;
}

//char* geohash_encode(char *hash, double lat, double lng, int precision) {
int geohash_encode(char *hash, double lat, double lng, int precision) {
    int ret_val = 0 ;

    if(precision < 1 || precision > 12)
        precision = 6;

    //char* hash = NULL;

    if(lat <= 90.0 && lat >= -90.0 && lng <= 180.0 && lng >= -180.0) {

        //hash = (char*)malloc(sizeof(char) * (precision + 1));
        hash[precision] = '\0';

        precision *= 5.0;

        Interval lat_interval = {MAX_LAT, MIN_LAT};
        Interval lng_interval = {MAX_LONG, MIN_LONG};

        Interval *interval;
        double coord, mid;
        int is_even = 1;
        unsigned int hashChar = 0;
        int i;

        for (i = 1; i <= precision; i++) {
            if(is_even) {
                interval = &lng_interval;
                coord = lng;
            }
            else {
                interval = &lat_interval;
                coord = lat;
            }

            mid = (interval->low + interval->high) / 2.0;
            hashChar = hashChar << 1;

            if(coord > mid) {
                interval->low = mid;
                hashChar |= 0x01;
            }
            else {
                interval->high = mid;
            }

            if(!(i % 5)) {
                hash[(i - 1) / 5] = char_map[hashChar];
                ret_val <<= 5 ;
                ret_val += hashChar ;
                hashChar = 0;
            }
            is_even = !is_even;
        }
    }

    //return hash;
    return (ret_val) ;
}

GeoCoord geohash_decode(char *hash) {
    GeoCoord coordinate = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    if(hash) {
        int char_amount = strlen(hash);

        if(char_amount) {
            unsigned int char_mapIndex;
            Interval lat_interval = {MAX_LAT, MIN_LAT};
            Interval lng_interval = {MAX_LONG, MIN_LONG};
            Interval *interval;

            int is_even = 1;
            double delta;
            int i, j;
            for(i = 0; i < char_amount; i++) {

                char_mapIndex = index_for_char(hash[i], (char*)char_map);

                if(char_mapIndex < 0) {
                    break;
                }

                // Interpret the last 5 bits of the integer
                for(j = 0; j < 5; j++) {
                    interval = is_even ? &lng_interval : &lat_interval;

                    delta = (interval->high - interval->low) / 2.0;

                    if((char_mapIndex << j) & 0x0010) {
                        interval->low += delta;
                    }
                    else {
                        interval->high -= delta;
                    }

                    is_even = !is_even;
                }
            }

            coordinate.latitude = lat_interval.high - ((lat_interval.high - lat_interval.low) / 2.0);
            coordinate.longitude = lng_interval.high - ((lng_interval.high - lng_interval.low) / 2.0);

            coordinate.north = lat_interval.high;
            coordinate.east = lng_interval.high;
            coordinate.south = lat_interval.low;
            coordinate.west = lng_interval.low;
        }
    }

    return coordinate;
}


char** geohash_neighbors(char *hash) {
    char** neighbors = NULL;

    if(hash) {
        // N, NE, E, SE, S, SW, W, NW
        neighbors = (char**)malloc(sizeof(char*) * 8);

        neighbors[0] = get_neighbor(hash, NORTH);
        neighbors[1] = get_neighbor(neighbors[0], EAST);
        neighbors[2] = get_neighbor(hash, EAST);
        neighbors[3] = get_neighbor(neighbors[2], SOUTH);
        neighbors[4] = get_neighbor(hash, SOUTH);
        neighbors[5] = get_neighbor(neighbors[4], WEST);
        neighbors[6] = get_neighbor(hash, WEST);
        neighbors[7] = get_neighbor(neighbors[6], NORTH);
    }

    return neighbors;
}

GeoBoxDimension geohash_dimensions_for_precision(int precision) {
    GeoBoxDimension dimensions = {0.0, 0.0};

    if(precision > 0) {
        int lat_times_to_cut = precision * 5 / 2;
        int lng_times_to_cut = precision * 5 / 2 + (precision % 2 ? 1 : 0);

        double width = 360.0;
        double height = 180.0;

        int i;
        for(i = 0; i < lat_times_to_cut; i++) {
            height /= 2.0;
        }

        for(i = 0; i < lng_times_to_cut; i++) {
            width /= 2.0;
        }

        dimensions.width = width;
        dimensions.height = height;
    }

    return dimensions;
}
