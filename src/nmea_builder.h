#pragma once

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <optional>
#include <stdexcept>
#include <string>

/**
 * @brief NMEA sentence builder for GPGGA and GPHDT
 *
 * Set each field value via setters, then retrieve the formatted
 * NMEA sentence via getters. Throws std::runtime_error if any
 * required field has not been set.
 */
class NmeaBuilder
{
public:
    // --- Setters ---

    /**
     * @brief Set UTC time
     * @param hour Hour (0-23)
     * @param min Minute (0-59)
     * @param sec Second (0.00-59.99)
     */
    void set_utc(int hour, int min, float sec);

    /**
     * @brief Set position in decimal degrees
     * @param lat_deg Latitude [degrees]. Positive is north; negative is south.
     * @param lon_deg Longitude [degrees]. Positive is east; negative is west.
     */
    void set_position(double lat_deg, double lon_deg);

    /**
     * @brief Set fix quality
     * @param quality Fix quality (0:Invalid, 1:SPS, 2:DGPS, 4:RTK Fixed, 5:RTK Float)
     */
    void set_quality(int quality);

    /**
     * @brief Set number of satellites in use
     * @param num_satellites Number of satellites
     */
    void set_num_satellites(int num_satellites);

    /**
     * @brief Set horizontal dilution of precision
     * @param hdop HDOP value
     */
    void set_hdop(float hdop);

    /**
     * @brief Set antenna altitude above MSL
     * @param altitude Altitude [m]
     */
    void set_altitude(float altitude);

    /**
     * @brief Set geoidal separation
     * @param geoid_height Geoidal separation [m]
     */
    void set_geoid_height(float geoid_height);

    /**
     * @brief Set differential GPS data
     * @param age Age of differential GPS data [s]
     * @param station_id Differential reference station ID
     */
    void set_dgps(float age, int station_id);

    /**
     * @brief Set true heading
     * @param heading True heading [degrees] (0.000-359.999)
     */
    void set_heading(float heading);

    // --- Getters ---

    /**
     * @brief Get GPGGA sentence with checksum
     * @return GPGGA sentence string
     * @throws std::runtime_error if any required field has not been set
     */
    std::string get_gpgga() const;

    /**
     * @brief Get GPHDT sentence with checksum
     * @return GPHDT sentence string
     * @throws std::runtime_error if heading has not been set
     */
    std::string get_gphdt() const;

private:
    static uint8_t nmea_checksum(const std::string &sentence);
    static std::string deg_to_nmea_lat(double deg, char &ns);
    static std::string deg_to_nmea_lon(double deg, char &ew);
    static std::string format_utc(int hour, int min, float sec);

    struct Utc
    {
        int hour;
        int min;
        float sec;
    };

    struct Position
    {
        double lat_deg;
        double lon_deg;
    };

    struct Dgps
    {
        float age;
        int station_id;
    };

    std::optional<Utc> utc_;
    std::optional<Position> position_;
    std::optional<int> quality_;
    std::optional<int> num_satellites_;
    std::optional<float> hdop_;
    std::optional<float> altitude_;
    std::optional<float> geoid_height_;
    std::optional<Dgps> dgps_;
    std::optional<float> heading_;
};
