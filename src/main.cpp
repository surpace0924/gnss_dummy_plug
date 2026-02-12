#include <M5Core2.h>
#include <string>
#include <cstdio>
#include <cmath>

/**
 * @brief Calculate NMEA checksum
 * @param sentence String between '$' and '*'
 * @return XOR of all characters
 */
uint8_t nmea_checksum(const std::string &sentence)
{
    uint8_t checksum = 0;
    for (char c : sentence)
    {
        checksum ^= static_cast<uint8_t>(c);
    }
    return checksum;
}

/**
 * @brief Convert decimal degrees to NMEA latitude format (ddmm.mmmmm)
 * @param deg Latitude [degrees]. Positive is north; negative is south.
 * @param[out] ns 'N' or 'S'
 * @return String in ddmm.mmmmm format
 */
std::string deg_to_nmea_lat(double deg, char &ns)
{
    ns = (deg >= 0.0) ? 'N' : 'S';
    double abs_deg = std::fabs(deg);
    int d = static_cast<int>(abs_deg);
    double min = (abs_deg - d) * 60.0;
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d%08.5f", d, min);
    return std::string(buf);
}

/**
 * @brief Convert decimal degrees to NMEA longitude format (dddmm.mmmmm)
 * @param deg Longitude [degrees]. Positive is east; negative is west.
 * @param[out] ew 'E' or 'W'
 * @return String in dddmm.mmmmm format
 */
std::string deg_to_nmea_lon(double deg, char &ew)
{
    ew = (deg >= 0.0) ? 'E' : 'W';
    double abs_deg = std::fabs(deg);
    int d = static_cast<int>(abs_deg);
    double min = (abs_deg - d) * 60.0;
    char buf[16];
    snprintf(buf, sizeof(buf), "%03d%08.5f", d, min);
    return std::string(buf);
}

/**
 * @brief Format UTC time to NMEA format (hhmmss.ss)
 * @param hour Hour (0-23)
 * @param min Minute (0-59)
 * @param sec Second (0.00-59.99)
 * @return String in hhmmss.ss format
 */
std::string format_utc(int hour, int min, float sec)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d%02d%05.2f", hour, min, sec);
    return std::string(buf);
}

/**
 * @brief Build a GPGGA sentence
 * @param utc_hour UTC hour (0-23)
 * @param utc_min UTC minute (0-59)
 * @param utc_sec UTC second (0.00-59.99)
 * @param lat_deg Latitude [degrees]. Positive is north; negative is south.
 * @param lon_deg Longitude [degrees]. Positive is east; negative is west.
 * @param quality Fix quality (0:Invalid, 1:SPS, 2:DGPS, 4:RTK Fixed, 5:RTK Float)
 * @param num_satellites Number of satellites in use
 * @param hdop Horizontal dilution of precision
 * @param altitude Antenna altitude above MSL [m]
 * @param geoid_height Geoidal separation [m]
 * @param dgps_age Age of differential GPS data [s]
 * @param dgps_station_id Differential reference station ID
 * @return GPGGA sentence with checksum
 */
std::string build_gpgga(int utc_hour, int utc_min, float utc_sec,
                        double lat_deg, double lon_deg,
                        int quality, int num_satellites,
                        float hdop, float altitude,
                        float geoid_height, float dgps_age,
                        int dgps_station_id)
{
    char ns, ew;
    std::string utc = format_utc(utc_hour, utc_min, utc_sec);
    std::string lat = deg_to_nmea_lat(lat_deg, ns);
    std::string lon = deg_to_nmea_lon(lon_deg, ew);

    char body[128];
    snprintf(body, sizeof(body),
             "GPGGA,%s,%s,%c,%s,%c,%d,%02d,%.1f,%.3f,M,%.3f,M,%.1f,%04d",
             utc.c_str(), lat.c_str(), ns, lon.c_str(), ew,
             quality, num_satellites, hdop,
             altitude, geoid_height,
             dgps_age, dgps_station_id);

    char sentence[140];
    snprintf(sentence, sizeof(sentence), "$%s*%02X", body, nmea_checksum(body));
    return std::string(sentence);
}

/**
 * @brief Build a GPHDT sentence
 * @param heading True heading [degrees] (0.000-359.999)
 * @return GPHDT sentence with checksum
 */
std::string build_gphdt(float heading)
{
    char body[32];
    snprintf(body, sizeof(body), "GPHDT,%.3f,T", heading);

    char sentence[48];
    snprintf(sentence, sizeof(sentence), "$%s*%02X", body, nmea_checksum(body));
    return std::string(sentence);
}

void setup()
{
    M5.begin();
    Serial.begin(115200);
}

void loop()
{
    std::string gpgga = build_gpgga(
        0, 34, 14.30,
        35.68114, 139.767061,
        4, 14, 0.7,
        12.635, 34.091,
        1.3, 1686);
    Serial.println(gpgga.c_str());

    std::string gphdt = build_gphdt(62.528);
    Serial.println(gphdt.c_str());

    delay(1000);
}
