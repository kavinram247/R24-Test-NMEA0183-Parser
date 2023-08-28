#include "gps.h"
#include <string.h>
#include "crc.h"
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

enum {
  SENTENCE_GGA = 0,
  SENTENCE_GLL = 1,
  SENTENCE_RMC = 2,
  SENTENCE_UNKNOWN,
};

struct gps_gga_t {
  float lat;
  float lon;
  char time[8];
  int satc;
  float hdop;
  float altitude;
  float geoid_sep_metres;
} gga;

struct gps_gll_t {
  float lat;
  float lon;
  float time;
  int flags;
} gll;

const char* sentence_prefix[] = { "GGA", "GLL", "RMC" };

gps_error_code_t (* const sentence_parsers[])(gps_t, const char*, int) = {
  parse_gga,
  parse_gll,
  parse_rmc
};

int next_field(const char* sentence, int len, int offset) {
  for (int i = offset; i < len; i++) {
    if (sentence[i] == ',') return i;
  }
  return -1;
}

gps_t gps_init() {
  struct gps_instance_t* var = calloc(1, sizeof(struct gps_instance_t));
  return var;
}

gps_error_code_t gps_destroy(gps_t gps_instance) {
  free(gps_instance);
  return GPS_NO_ERROR;
}

gps_error_code_t gps_update(gps_t gps_instance, const char* sentence, int len) {
  if (verify_checksum(sentence, len) == 0) return GPS_INVALID_CHECKSUM;
  int current_sentence = SENTENCE_UNKNOWN;
  for (int i = 0; i < SENTENCE_UNKNOWN; i++) {
    if (strncmp(sentence+3, sentence_prefix[i], 3) == 0) {
      current_sentence = i;
      break;
    }
  }
  if (current_sentence == SENTENCE_UNKNOWN) {
    return GPS_UNKNOWN_PREFIX;
  }

  if (current_sentence == SENTENCE_GGA || current_sentence == SENTENCE_GLL || current_sentence == SENTENCE_RMC) {
    return sentence_parsers[current_sentence](gps_instance, sentence, len);
  } else {
    return GPS_UNIMPLEMENTED;
  }
}

gps_error_code_t gps_get_lat_lon(gps_t gps_instance, int* degmin, int* minfrac) {
  if (gps_instance->last_msg_type == SENTENCE_GGA) return gga_get_lat_lon(degmin, minfrac);
  if (gps_instance->last_msg_type == SENTENCE_GLL) return gll_get_lat_lon(degmin, minfrac);
  return GPS_UNIMPLEMENTED;
}

gps_error_code_t gps_get_time(gps_t gps_instance, struct tm* time) {
  return GPS_UNIMPLEMENTED;
}

gps_error_code_t gps_get_altitude(gps_t gps_instance, float* msl_metres) {
  return GPS_UNIMPLEMENTED;
}

gps_error_code_t gga_get_lat_lon(int* degmin, int* minfrac) {
  return GPS_NO_ERROR;
}

gps_error_code_t gps_get_geoid_sep(gps_t gps_instance, float* geoid_sep_metres) {
  if (gps_instance->last_msg_type == SENTENCE_GGA) {
    *geoid_sep_metres = gga.geoid_sep_metres;
    return GPS_NO_ERROR;
  } else {
    return GPS_UNIMPLEMENTED;
  }
}

gps_error_code_t parse_gga(gps_t gps_instance, const char* sentence, int len) {
  gps_instance->last_msg_type = SENTENCE_GGA;
  return GPS_NO_ERROR;
}

gps_error_code_t parse_gll(gps_t gps_instance, const char* sentence, int len) {
  gps_instance->last_msg_type = SENTENCE_GLL;
  return GPS_NO_ERROR;
}

gps_error_code_t parse_rmc(gps_t gps_instance, const char* sentence, int len) {
  gps_instance->last_msg_type = SENTENCE_RMC;
  return GPS_NO_ERROR;
}

int main() {
  sentence_type current_sentence = SENTENCE_RMC;

  gps_t gps_instance = gps_init();

  gps_error_code_t result = gps_update(gps_instance, sentence, len);

  if (result == GPS_SUCCESS) {
    int lat_degmin, lat_minfrac, lon_degmin, lon_minfrac;
    result = gps_get_lat_lon(gps_instance, &lat_degmin, &lat_minfrac, &lon_degmin, &lon_minfrac);
    if (result == GPS_SUCCESS) {
      // Process the data...
    }
  }

  return 0;
}

