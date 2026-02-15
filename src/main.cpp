#include <M5Unified.h>
#include "nmea_builder.h"

NmeaBuilder nmea;

void update_display(const NmeaBuilder &nmea)
{
    M5.Display.setCursor(0, 0);

    M5.Display.setTextColor(GREEN, BLACK);
    M5.Display.println("GNSS Dummy Plug");
    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.println("----------------");

    auto &utc = nmea.utc();
    auto &date = nmea.date();
    auto &pos = nmea.position();

    if (utc)
        M5.Display.printf("UTC  %02d:%02d:%05.2f\n", utc->hour, utc->min, utc->sec);
    if (date)
        M5.Display.printf("Date %04d/%02d/%02d\n", date->year, date->month, date->day);
    if (pos)
    {
        char ns = (pos->lat_deg >= 0) ? 'N' : 'S';
        char ew = (pos->lon_deg >= 0) ? 'E' : 'W';
        M5.Display.printf("Lat %10.6f %c\n", std::fabs(pos->lat_deg), ns);
        M5.Display.printf("Lon %10.6f %c\n", std::fabs(pos->lon_deg), ew);
    }
    if (nmea.altitude())
        M5.Display.printf("Alt  %.3f m\n", *nmea.altitude());
    if (nmea.heading())
        M5.Display.printf("Hdg  %.3f deg\n", *nmea.heading());
    if (nmea.speed_knots())
        M5.Display.printf("Spd  %.1f kn\n", *nmea.speed_knots());
    if (nmea.quality() && nmea.num_satellites())
        M5.Display.printf("Qual %d  Sat %d\n", *nmea.quality(), *nmea.num_satellites());
    if (nmea.hdop())
        M5.Display.printf("HDOP %.1f\n", *nmea.hdop());
}

void setup()
{
    M5.begin();
    Serial.begin(115200);

    M5.Display.setTextSize(2);
    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.fillScreen(BLACK);

    nmea.set_utc(0, 34, 14.30);
    nmea.set_position(35.68114, 139.767061);
    nmea.set_quality(4);
    nmea.set_num_satellites(14);
    nmea.set_hdop(0.7);
    nmea.set_altitude(12.635);
    nmea.set_geoid_height(34.091);
    nmea.set_dgps(1.3, 1686);
    nmea.set_heading(62.528);
    nmea.set_status('A');
    nmea.set_speed_knots(0.0);
    nmea.set_course(62.5);
    nmea.set_date(14, 2, 2026);
    nmea.set_magnetic_variation(-7.0);
    nmea.set_mode_indicator('D');
}

void loop()
{
    Serial.println(nmea.get_gpgga().c_str());
    Serial.println(nmea.get_gphdt().c_str());
    Serial.println(nmea.get_gprmc().c_str());

    update_display(nmea);

    delay(1000);
}
