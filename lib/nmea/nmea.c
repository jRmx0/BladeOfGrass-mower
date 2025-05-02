#include "nmea.h"

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

static bool nmea_is_gngga_message(const char *message);
static bool nmea_gga_has_location_data(const char *gga_message);

bool nmea_is_gga_location_report(const char *message)
{
    if (!nmea_is_gngga_message(message))
    {
        return 0;
    }

    if (nmea_gga_has_location_data(message))
    {
        return 1;
    }

    return 0;
}

location_data_t nmea_extract_location_data(const char *gga_message)
{
    location_data_t location_data;
    location_data.latitude = 0;
    location_data.longitude = 0;

    double lat = 0;
    char lat_dir = '\0';
    double lon = 0;
    char lon_dir = '\0';

    if (gga_message == NULL)
    {
        return location_data;
    }

    if (!nmea_is_gga_location_report(gga_message))
    {
        return location_data;
    }

    char message_copy[256];
    strncpy(message_copy, gga_message, sizeof(message_copy) - 1);
    message_copy[sizeof(message_copy) - 1] = '\0';

    char *p = strtok(message_copy, ",");
    int field = 1;

    while (p != NULL && field <= GGA_FIELD_LON_DIR)
    {
        switch (field)
        {
        case GGA_FIELD_LAT:
            lat = atof(p);
            int lat_deg = (int)(lat / 100);
            double lat_min = lat - (lat_deg * 100);
            lat = lat_deg + (lat_min / 60.0);
            break;
        
        case GGA_FIELD_LAT_DIR:
            lat_dir = p[0];
            break;
        
        case GGA_FIELD_LON:
            lon = atof(p);
            int lon_deg = (int)(lon / 100);
            double lon_min = lon - (lon_deg * 100);
            lon = lon_deg + (lon_min / 60.0);
            break;
    
        case GGA_FIELD_LON_DIR:
            lon_dir = p[0];
            break;
        
        default:
            break;
        }

        p = strtok(NULL, ",");
        field++;
    }

    if (lat_dir == 'S')
    {
        lat = -lat;
    }

    if(lon_dir == 'W')
    {
        lon = -lon;
    }

    location_data.latitude = lat;
    location_data.longitude = lon;

    return location_data;
}

int nmea_get_gga_fix_quality(const char *gga_message)
{
    if (gga_message == NULL)
    {
        return -1;
    }

    char message_copy[256];
    strncpy(message_copy, gga_message, sizeof(message_copy) - 1);
    message_copy[sizeof(message_copy) - 1] = '\0';

    char *p = strtok(message_copy, ",");
    int field = 1;

    while (p != NULL && field <= GGA_FIELD_QUALITY)
    {
        if (field == GGA_FIELD_QUALITY)
        {
            return atoi(p);
        }
        p = strtok(NULL, ",");
        field++;
    }

    return -1;
}

/**
 * @brief Check if a message is a valid GNGGA message
 *
 * @param message The NMEA message to check
 * @return true if the message starts with $GNGGA
 * @return false otherwise
 */
static bool nmea_is_gngga_message(const char *message)
{
    if (message == NULL)
    {
        return false;
    }

    if (strncmp(message, "$GNGGA", 6) == 0)
    {
        return true;
    }

    return false;
}

/**
 * @brief Check if a GGA message has valid location data
 *
 * Message is considered valid if:
 * 1. It has latitude and longitude data
 * 2. Quality indicator is between 1 and 6 inclusive
 *    (GPS_QUALITY_SPS, GPS_QUALITY_DIFFERENTIAL, GPS_QUALITY_PPS,
 *     GPS_QUALITY_RTK_INT, GPS_QUALITY_RTK_FLOAT, GPS_QUALITY_DEAD_RECK)
 *
 * @param gga_message The GGA NMEA message to check
 * @return true if the message contains valid location data
 * @return false if the message is NULL or doesn't contain valid location data
 */
static bool nmea_gga_has_location_data(const char *gga_message)
{
    if (gga_message == NULL)
    {
        return false;
    }

    char message_copy[256];
    strncpy(message_copy, gga_message, sizeof(message_copy) - 1);
    message_copy[sizeof(message_copy) - 1] = '\0';

    char *p = strtok(message_copy, ",");
    int field = 1;

    bool has_lat = false;
    bool has_lon = false;
    int quality = -1;

    while (p != NULL)
    {
        switch (field)
        {
        case GGA_FIELD_LAT:
            if (strlen(p) > 0)
            {
                has_lat = true;
            }
            break;

        case GGA_FIELD_LON:
            if (strlen(p) > 0)
            {
                has_lon = true;
            }
            break;

        case GGA_FIELD_QUALITY:
            quality = atoi(p);
            break;
        }

        p = strtok(NULL, ",");
        field++;
    }

    return has_lat && has_lon && quality >= GPS_QUALITY_SPS && quality <= GPS_QUALITY_DEAD_RECK;
}


