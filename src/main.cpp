#include <M5Unified.h>
#include "nmea_builder.h"

NmeaBuilder nmea;

void setup()
{
    M5.begin();
    Serial.begin(115200);

    nmea.set_utc(0, 34, 14.30);
    nmea.set_position(35.68114, 139.767061);
    nmea.set_quality(4);
    nmea.set_num_satellites(14);
    nmea.set_hdop(0.7);
    nmea.set_altitude(12.635);
    nmea.set_geoid_height(34.091);
    nmea.set_dgps(1.3, 1686);
    nmea.set_heading(62.528);
}

void loop()
{
    Serial.println(nmea.get_gpgga().c_str());
    Serial.println(nmea.get_gphdt().c_str());
    delay(1000);
}
