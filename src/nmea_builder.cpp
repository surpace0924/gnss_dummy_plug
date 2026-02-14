#include "nmea_builder.h"

// --- Setters ---

void NmeaBuilder::set_utc(int hour, int min, float sec)
{
    utc_ = {hour, min, sec};
}

void NmeaBuilder::set_position(double lat_deg, double lon_deg)
{
    position_ = {lat_deg, lon_deg};
}

void NmeaBuilder::set_quality(int quality)
{
    quality_ = quality;
}

void NmeaBuilder::set_num_satellites(int num_satellites)
{
    num_satellites_ = num_satellites;
}

void NmeaBuilder::set_hdop(float hdop)
{
    hdop_ = hdop;
}

void NmeaBuilder::set_altitude(float altitude)
{
    altitude_ = altitude;
}

void NmeaBuilder::set_geoid_height(float geoid_height)
{
    geoid_height_ = geoid_height;
}

void NmeaBuilder::set_dgps(float age, int station_id)
{
    dgps_ = {age, station_id};
}

void NmeaBuilder::set_heading(float heading)
{
    heading_ = heading;
}

// --- Getters ---

std::string NmeaBuilder::get_gpgga() const
{
    if (!utc_)
        throw std::runtime_error("GPGGA: utc has not been set");
    if (!position_)
        throw std::runtime_error("GPGGA: position has not been set");
    if (!quality_)
        throw std::runtime_error("GPGGA: quality has not been set");
    if (!num_satellites_)
        throw std::runtime_error("GPGGA: num_satellites has not been set");
    if (!hdop_)
        throw std::runtime_error("GPGGA: hdop has not been set");
    if (!altitude_)
        throw std::runtime_error("GPGGA: altitude has not been set");
    if (!geoid_height_)
        throw std::runtime_error("GPGGA: geoid_height has not been set");
    if (!dgps_)
        throw std::runtime_error("GPGGA: dgps has not been set");

    char ns, ew;
    std::string utc = format_utc(utc_->hour, utc_->min, utc_->sec);
    std::string lat = deg_to_nmea_lat(position_->lat_deg, ns);
    std::string lon = deg_to_nmea_lon(position_->lon_deg, ew);

    char body[128];
    snprintf(body, sizeof(body),
             "GPGGA,%s,%s,%c,%s,%c,%d,%02d,%.1f,%.3f,M,%.3f,M,%.1f,%04d",
             utc.c_str(), lat.c_str(), ns, lon.c_str(), ew,
             *quality_, *num_satellites_, *hdop_,
             *altitude_, *geoid_height_,
             dgps_->age, dgps_->station_id);

    char sentence[140];
    snprintf(sentence, sizeof(sentence), "$%s*%02X", body, nmea_checksum(body));
    return std::string(sentence);
}

std::string NmeaBuilder::get_gphdt() const
{
    if (!heading_)
        throw std::runtime_error("GPHDT: heading has not been set");

    char body[32];
    snprintf(body, sizeof(body), "GPHDT,%.3f,T", *heading_);

    char sentence[48];
    snprintf(sentence, sizeof(sentence), "$%s*%02X", body, nmea_checksum(body));
    return std::string(sentence);
}

// --- Private helpers ---

uint8_t NmeaBuilder::nmea_checksum(const std::string &sentence)
{
    uint8_t checksum = 0;
    for (char c : sentence)
    {
        checksum ^= static_cast<uint8_t>(c);
    }
    return checksum;
}

std::string NmeaBuilder::deg_to_nmea_lat(double deg, char &ns)
{
    ns = (deg >= 0.0) ? 'N' : 'S';
    double abs_deg = std::fabs(deg);
    int d = static_cast<int>(abs_deg);
    double min = (abs_deg - d) * 60.0;
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d%08.5f", d, min);
    return std::string(buf);
}

std::string NmeaBuilder::deg_to_nmea_lon(double deg, char &ew)
{
    ew = (deg >= 0.0) ? 'E' : 'W';
    double abs_deg = std::fabs(deg);
    int d = static_cast<int>(abs_deg);
    double min = (abs_deg - d) * 60.0;
    char buf[16];
    snprintf(buf, sizeof(buf), "%03d%08.5f", d, min);
    return std::string(buf);
}

std::string NmeaBuilder::format_utc(int hour, int min, float sec)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d%02d%05.2f", hour, min, sec);
    return std::string(buf);
}
