#include "gnssmodule.h"
#include "screens.h"
#include "uirenderer.h"



GNSSModule::GNSSModule(uint8_t uartNumber)
  : Serial2(uartNumber), GeoPositionProvider(){};

//: gps(uartNumber), nmea(messageBuffer, sizeof(messageBuffer)), GeoPositionProvider() {};

void GNSSModule::initialize() {
//  gps.begin(9600);
Serial2.begin(57600, SERIAL_8N1,11,12); //map uart2 to pin 8 and 9
   while(!Serial2.available()) {
       UIRENDERER.step();
      }

  BOOTSCREEN.gnssOK = true;  //this only set er to ok on bootscree does not effect the rest

  _lastUpdateTime = 0;
  ready = true;
}
// Returns true if new data was read.
bool GNSSModule::readGNSS() {
  if ((millis() - _lastUpdateTime) > GNSS_MIN_UPDATE_TIME_MS) {
    while (Serial2.available()) {
     // Serial.write(Serial2.read());
      nmea.encode(Serial2.read());

       //   while (*gpsStream) {
       //         nmea.encode(*gpsStream++);  // swapped with abpove line for TinyGPS
    }

    _lastUpdateTime = millis();
    return true;
  } else {
    return false;  //change//d from false 2/9/24
  }
  
}
bool GNSSModule::isReady() {
  //  return ready && nmea.isValid();
  return ready && true;
};

bool GNSSModule::getPosition(GeoPosition& geoPosition) {
  geoPosition.updatePosition(getLatitude(), getLongitude());
  // return nmea.isValid();
  //if (nmea.location.isValid()){
  return true;
  //}
  //else {
  //return false;
  //}
};

uint16_t GNSSModule::getHeading() {
  // NMEA module outputs heading as "thousandths of a degree"
  //  int newHeading = nmea.getCourse() * 1e-3;
  int newHeading = nmea.course.deg();  //* 1e-3;  // tinygps++
  // Safety checks to avoid out-of range headings
  if ((newHeading %= 360) < 0) {
    return newHeading + 360;
  } else {
    return newHeading;
  }
};


char* GNSSModule::getDirection() {
  //int newHeading = nmea.getCourse()* 1e-3;
  int newHeading = nmea.course.deg();  //* 1e-3;  //Tinygps++
  char* direction = (char*)malloc(3);

  if (nmea.course.isValid()) {


    if ((newHeading >= 276) && (newHeading <= 354)) {
      direction = "NW";
    }
    if ((newHeading >= 355) && (newHeading <= 360)) {
      direction = "N";
    }
    if ((newHeading >= 0) && (newHeading <= 5)) {
      direction = "N";
    }
    if ((newHeading >= 6) && (newHeading <= 84)) {
      direction = "NE";
    }
    if ((newHeading >= 85) && (newHeading <= 95)) {
      direction = "E";
    }
    if ((newHeading >= 96) && (newHeading <= 175)) {
      direction = "SE";
    }
    if ((newHeading >= 176) && (newHeading <= 185)) {
      direction = "S";
    }
    if ((newHeading >= 186) && (newHeading <= 265)) {
      direction = "SW";
    }
    if ((newHeading >= 266) && (newHeading <= 275)) {
      direction = "W";
    }

  } else {
    direction = "00";
  }
  return direction;
};


uint8_t GNSSModule::getHour() {
  // return nmea.getHour();
  return nmea.time.hour();  //Tinygps++
}

uint8_t GNSSModule::getMinute() {
  //  return nmea.getMinute();
  return nmea.time.minute();  //Tinygps++
}
uint8_t GNSSModule::getSecond() {
  //  return nmea.getMinute();
  return nmea.time.second();  //Tinygps++
}

uint8_t GNSSModule::getDay() {
  return nmea.date.day();
}

uint8_t GNSSModule::getMonth() {
  return nmea.date.month();
}

float GNSSModule::getSpeed() {
  return nmea.speed.kmph();
}
float GNSSModule::getAltitude() {
  return nmea.altitude.feet();
}

uint8_t GNSSModule::getSats() {
  return nmea.satellites.value();
}

double GNSSModule::getLatitude() {
  // Serial.print("lat: ");
  // Serial.println(nmea.location.lat(), 6);
  return (double)nmea.location.lat();
}

double GNSSModule::getLongitude() {
  return (double)nmea.location.lng();
}

bool GNSSModule::step() {
  return readGNSS();
}
