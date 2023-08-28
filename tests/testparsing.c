#include <math.h> // Include math library for floating-point comparisons

void test_geoid_separation(void) {
    const char* examples[] = { 
        "$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E", 
    }; 
    gps_t handle = gps_init(); 
    TEST_ASSERT_EQUAL(GPS_NO_ERROR, gps_update(handle, examples[0], strlen(examples[0]))); 
    float geoid_sep; 
    gps_get_geoid_sep(handle, &geoid_sep); 

    // Use a tolerance value for floating-point comparison
    float expected_geoid_sep = -34.2;
    float tolerance = 0.001; // Adjust tolerance based on precision needed

    // Check if the difference between the actual and expected values is within the tolerance
    TEST_ASSERT_TRUE(fabs(geoid_sep - expected_geoid_sep) < tolerance);
}

