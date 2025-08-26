#include "common.h"
void init_gps()
{
    gpio_init(GPS_EN);
    gpio_init(GPS_RESET);
    gpio_set_dir(GPS_EN,GPIO_OUT);
    gpio_set_dir(GPS_RESET,GPIO_OUT);
    gpio_put(GPS_EN,0);
    sleep_ms(200);
    gpio_put(GPS_RESET,0);
    sleep_ms(800);
    gpio_put(GPS_RESET,1);
    sleep_ms(200);
    sendConfig();
}

void sendConfig()
{
    // UBX-CFG-GNSS: Enable only GPS
    const uint8_t UBX_CFG_GNSS_GPS_ONLY[] = {
        0xB5, 0x62, 0x06, 0x3E, 0x24, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x01, 0x00, 0x01, 0x01, 0x01,
        0x00, 0x00, 0x10, 0x00,
        0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xB0, 0x1E
    };

    // UBX-CFG-PRT: Enable both NMEA and UBX output on UART1
    const uint8_t UBX_CFG_PRT_UART1[] = {
        0xB5, 0x62, 0x06, 0x00, 0x14, 0x00,
        0x01, 0x00,
        0x00, 0x00,
        0xD0, 0x08, 0x00, 0x00,
        0x80, 0x25, 0x00, 0x00,
        0x03, 0x00,
        0x00, 0x00,
        0x00, 0x00,
        0x9A, 0x79
    };

    uart_write_blocking(GPS_UART,UBX_CFG_GNSS_GPS_ONLY,sizeof(UBX_CFG_GNSS_GPS_ONLY));
    sleep_ms(200);
    uart_write_blocking(GPS_UART,UBX_CFG_PRT_UART1,sizeof(UBX_CFG_PRT_UART1));
    sleep_ms(200);
}

int hasFix()
{
    
}

void readNMEA()
{
    // read the NMEA GLL string
    char buff[256];
    int index=0;
    char c;
    do
    {
        c=uart_getc(GPS_UART);
        if(c==10)
        {
            index=0;
            break;
        }
            
        buff[index++]=c;
    }while(uart_is_readable(GPS_UART));
    const char* tmp;
    printf("%s\n",buff);
    strncpy(tmp,buff,buff+5);
    if(strcmp(tmp, "$GNGLL"))
        parseNMEA(buff,256);
}
float parse_nmea_coord(const char *coord, const char *direction) {
    float deg = 0.0, min = 0.0;

    if (strlen(coord) < 4) return 0;

    if (direction[0] == 'N' || direction[0] == 'S') {
        sscanf(coord, "%2lf%lf", &deg, &min);
    } else {
        sscanf(coord, "%3lf%lf", &deg, &min);
    }

    float result = deg + (min / 60.0);
    if (direction[0] == 'S' || direction[0] == 'W') {
        result = -result;
    }

    return result;
}
void parseNMEA(char* NMEA_string, int len)
{
    char copy[256];
    strncpy(copy, NMEA_string, sizeof(copy));
    copy[sizeof(copy) - 1] = '\0';

    char *tokens[10];
    int i = 0;
    char *token = strtok(copy, ",");
    while (token != NULL && i < 10) {
        tokens[i++] = token;
        token = strtok(NULL, ",");
    }

    if (i < 7) {
        printf("Incomplete GPGLL sentence\n");
        return;
    }

    const char *lat = tokens[1];
    const char *lat_dir = tokens[2];
    const char *lon = tokens[3];
    const char *lon_dir = tokens[4];
    const char *utc_time = tokens[5];
    const char *status = tokens[6];

    double latitude = parse_nmea_coord(lat, lat_dir);
    double longitude = parse_nmea_coord(lon, lon_dir);

    printf("Latitude: %.6f\n", latitude);
    printf("Longitude: %.6f\n", longitude);
    printf("UTC Time: %s\n", utc_time);
    printf("Status: %s (%s)\n", status, (status[0] == 'A') ? "Data Valid" : "Data Invalid");
    
    if(status[0]=='A')
        info.hasFix=1;
    else
        info.hasFix=0;

    info.lat=latitude;
    info.lon=longitude;
    // check if on nord and sud 
    if (lon_dir[0] == 'N')
        info.lonDir=0;
    else
        info.lonDir=1;
    // check if on west and east
    if (lat_dir[0] == 'E')
        info.latDir=1;
    else
        info.latDir=0;
}
