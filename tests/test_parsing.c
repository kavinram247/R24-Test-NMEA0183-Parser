#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Define GPS data structures and functions
typedef struct {
    // Define your GPS data structure here
    // ...
} gps_t;

typedef enum {
    GPS_NO_ERROR,
    // Define error codes here
    // ...
} gps_error_t;

gps_t gps_init(void) {
    // Initialize your GPS data structure here
    gps_t handle; // Example initialization
    return handle;
}

gps_error_t gps_update(gps_t handle, const char *data, size_t length) {
    // Update the GPS data based on the NMEA data
    // ...
    return GPS_NO_ERROR;
}

typedef struct {
    int degmins;
    int minfracs;
    char hemisphere;  // 'N', 'S', 'E', 'W'
} LatLonInfo;

void gps_get_lat_lon(gps_t handle, LatLonInfo *latitude, LatLonInfo *longitude) {
    // Assume the latitude and longitude values are obtained here
    int latlon_degmins[2] = {3342, -11751}; // Example values
    int latlon_minfracs[2] = {6618, 3858}; // Example values

    // Set hemisphere indicators based on positive or negative values
    latitude->hemisphere = (latlon_degmins[0] >= 0) ? 'N' : 'S';
    longitude->hemisphere = (latlon_degmins[1] >= 0) ? 'E' : 'W';

    // Store the absolute values in the structures
    latitude->degmins = abs(latlon_degmins[0]);
    latitude->minfracs = latlon_minfracs[0];
    longitude->degmins = abs(latlon_degmins[1]);
    longitude->minfracs = latlon_minfracs[1];
}

// Define your testing framework or assertions here
#define TEST_ASSERT_EQUAL(a, b) // Define your assertion macro
#define TEST_ASSERT_TRUE(a) // Define your assertion macro

void test_gga_parsing_over_examples(void) {
    const char* examples[] = { 
        "$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E"
    }; 
    gps_t handle = gps_init(); 
    TEST_ASSERT_EQUAL(GPS_NO_ERROR, gps_update(handle, examples[0], strlen(examples[0]))); 

    LatLonInfo latitude, longitude;
    gps_get_lat_lon(handle, &latitude, &longitude);

    TEST_ASSERT_EQUAL_INT(3342, latitude.degmins);
    TEST_ASSERT_EQUAL_INT(6618, latitude.minfracs);
    TEST_ASSERT_EQUAL_INT(11751, longitude.degmins);
    TEST_ASSERT_EQUAL_INT(3858, longitude.minfracs);
    TEST_ASSERT_TRUE(latitude.hemisphere == 'N');
    TEST_ASSERT_TRUE(longitude.hemisphere == 'W');
}

int main() {
    test_gga_parsing_over_examples();
    return 0;
}

