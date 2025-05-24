#include "gnssmodule.h"
#include "screens.h"
#include "uirenderer.h"



GNSSModule::GNSSModule(uint8_t uartNumber)
  : Serial2(uartNumber), GeoPositionProvider(){};

//: gps(uartNumber), nmea(messageBuffer, sizeof(messageBuffer)), GeoPositionProvider() {};

void GNSSModule::initialize() {
Serial2.begin(57600, SERIAL_8N1,9,8); //map uart2 to pin 10 and 11
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
       nmea.encode(Serial2.read());

       //   while (*gpsStream) {
    //           nmea.encode(*gpsStream++);  // swapped with abpove line for TinyGPS
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
  return nmea.location.isValid();
  if (nmea.location.isValid()){
  return true;
  }
  else {
  return false;
  }
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

double GNSSModule::distancetowaypoint(){
  //41.77km
double MYcurrentLAT = 54.130179;
double MYcurrentLNG = -0.667634;
double distanceKm = nmea.distanceBetween(nmea.location.lat(),nmea.location.lng(),MYcurrentLAT,MYcurrentLNG) / 1000.0;
return distanceKm;
}

double GNSSModule::coursetowaypoint(){
  //308 degrees
double MYcurrentLAT = 54.130179;
double MYcurrentLNG = -0.667634;
double courseTo = nmea.courseTo(nmea.location.lat(),nmea.location.lng(),MYcurrentLAT,MYcurrentLNG);
return courseTo;
//nmea.cardinal(courseTo);
}
