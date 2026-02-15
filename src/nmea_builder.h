#pragma once

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <optional>
#include <stdexcept>
#include <string>

/**
 * @brief NMEA sentence builder for GPGGA, GPHDT, and GPRMC
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

    /**
     * @brief Set receiver status
     * @param status 'A' (active/valid) or 'V' (void/invalid)
     */
    void set_status(char status);

    /**
     * @brief Set speed over ground
     * @param speed Speed over ground [knots]
     */
    void set_speed_knots(float speed);

    /**
     * @brief Set course over ground
     * @param course Course over ground [degrees] (0.0-359.9)
     */
    void set_course(float course);

    /**
     * @brief Set date
     * @param day Day (1-31)
     * @param month Month (1-12)
     * @param year Year (e.g. 2026)
     */
    void set_date(int day, int month, int year);

    /**
     * @brief Set magnetic variation
     * @param magvar Magnetic variation [degrees]. Positive is east; negative is west.
     */
    void set_magnetic_variation(float magvar);

    /**
     * @brief Set positioning mode indicator
     * @param mode 'A' (autonomous), 'D' (DGPS), 'E' (estimated), 'N' (not valid)
     */
    void set_mode_indicator(char mode);

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

    /**
     * @brief Get GPRMC sentence with checksum
     * @return GPRMC sentence string
     * @throws std::runtime_error if any required field has not been set
     */
    std::string get_gprmc() const;

private:
    static uint8_t nmea_checksum(const std::string &sentence);
    static std::string deg_to_nmea_lat(double deg, char &ns);
    static std::string deg_to_nmea_lon(double deg, char &ew);
    static std::string format_utc(int hour, int min, float sec);
    static std::string format_date(int day, int month, int year);

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

    struct Date
    {
        int day;
        int month;
        int year;
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
    std::optional<char> status_;
    std::optional<float> speed_knots_;
    std::optional<float> course_degrees_;
    std::optional<Date> date_;
    std::optional<float> magnetic_variation_;
    std::optional<char> mode_indicator_;
};
