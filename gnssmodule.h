#ifndef _GNSSMODULE_H
#define _GNSSMODULE_H

#include "Arduino.h"
#include "TinyGPSPlus.h"
#include "geopositionprovider.h"
#include "globalconfig.h"
#include <HardwareSerial.h>


class GNSSModule : public GeoPositionProvider {

public:
  TinyGPSPlus nmea;

// GNSSModule();
  GNSSModule(uint8_t uartNumber);

  bool readGNSS();
  void initialize();
  bool step();
  bool isReady();
  bool getPosition(GeoPosition& geoPosition);
  uint16_t getHeading();
  char* getDirection();
  uint8_t getHour();
  uint8_t getMinute();
  uint8_t getSecond();
  uint8_t getDay();
  uint8_t getMonth();
  float getSpeed();
  uint8_t getSats();
  double getLatitude();
  double getLongitude();
  float getAltitude();
    // A sample NMEA stream.
const char *gpsStream =
"$GPRMC,103959.00,A,5353.82145,N,00009.98861,W,0.261,,230724,,,A,V*17\r\n"
"$GPGGA,103959.00,5353.82145,N,00009.98861,W,1,12,0.95,25.0,M,46.3,M,,*7D\r\n"
"$GPRMC,103959.00,A,5353.82145,N,00009.98861,W,0.261,,230724,,,A,V*17\r\n"
"$GPGGA,103959.00,5353.82145,N,00009.98861,W,1,12,0.95,25.0,M,46.3,M,,*7D\r\n"
"$GPRMC,103959.00,A,5353.82145,N,00009.98861,W,0.261,,230724,,,A,V*17\r\n"
"$GPGGA,103959.00,5353.82145,N,00009.98861,W,1,12,0.95,25.0,M,46.3,M,,*7D\r\n"
;

 

private:

  HardwareSerial Serial2;   // GPS UART2 Configuerd in the main INO file
  char messageBuffer[128];  // do not think this is used????????
  long _lastUpdateTime;
};



#endif