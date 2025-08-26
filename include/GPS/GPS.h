#ifndef _GPS__H_
#define _GPS__H_


// NMEA string struct parse the GLL string
typedef struct GPS_INFO
{
    int hasFix;
    float lat;
    float lon;
    int latDir; // 0=N , 1=S
    int lonDir; // 0=E , 1=W
}GPS_INFO_t;

// // main struct used for interface
 GPS_INFO_t info; // read data directly from here

// void init_gps();
// void sendConfig();
// int hasFix();
// void readNMEA();
// void parseNMEA(char* NMEA_string, int len);
// float parse_nmea_coord(const char *coord, const char *direction);

// int gpsHasFix() {return info.hasFix;}
// float getLat() {return info.lat;}
// float getLon() {return info.lon;}
// int getLatDir() {return info.latDir;}
// int getLonDir() {return info.lonDir;}

#endif