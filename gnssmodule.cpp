#include "gnssmodule.h"
#include "screens.h"
#include "uirenderer.h"



GNSSModule::GNSSModule(uint8_t uartNumber)
    : gps(uartNumber), nmea(messageBuffer, sizeof(messageBuffer)), GeoPositionProvider() {};

void GNSSModule::initialize() {
   gps.begin(9600);
    
    while(!gps.available()) {
        UIRENDERER.step();
      }

  
    BOOTSCREEN.gnssOK = true;  //this only set er to ok on bootscree does not effect the rest

    _lastUpdateTime = 0;
    ready = true;
}

// Returns true if new data was read.
bool GNSSModule::readGNSS() {
    if((millis() - _lastUpdateTime) > GNSS_MIN_UPDATE_TIME_MS) {
        while(gps.available()) {
            nmea.process(gps.read());
        }
        _lastUpdateTime = millis();
        return true;
    } else {
        return false; //changed from false 2/9/24
    }
}


bool GNSSModule::isReady() {
    return ready && nmea.isValid();
};

bool GNSSModule::getPosition(GeoPosition& geoPosition) {
    geoPosition.updatePosition(getLatitude(), getLongitude());
    return nmea.isValid();
};

uint16_t GNSSModule::getHeading() {
    // NMEA module outputs heading as "thousandths of a degree"
    int newHeading = nmea.getCourse() * 1e-3;
    // Safety checks to avoid out-of range headings
    if((newHeading %= 360) < 0) {
        return newHeading + 360;
    } else {
        return newHeading;
    }
};


char* GNSSModule::getDirection() {
int newHeading = nmea.getCourse()* 1e-3;
char* direction = (char*)malloc(3);

 if(nmea.isValid()){

   
 if ((newHeading >=276) && (newHeading<= 354)){
direction = "NW";
}
if ((newHeading >=355) && (newHeading<= 360)){
direction = "N";
}
if ((newHeading >=0) && (newHeading<= 5)){
direction = "N";
}
if ((newHeading >=6) && (newHeading<= 84)){
direction = "NE";
}
if ((newHeading >=85) && (newHeading<= 95)){
direction = "E";
}
if ((newHeading >=96) && (newHeading<= 175)){
direction = "SE";
}
if ((newHeading >=176) && (newHeading<= 185)){
direction = "S";
}
if ((newHeading >=186) && (newHeading<= 265)){
direction = "SW";
}
if ((newHeading >=266) && (newHeading<= 275)){
direction = "W";
}

 }
 else{
  direction ="00";
 }
return direction;
};


uint8_t GNSSModule::getHour() {
    return nmea.getHour();
}

uint8_t GNSSModule::getMinute() {
    return nmea.getMinute();
}

uint8_t GNSSModule::getDay() {
    return nmea.getDay();
}

uint8_t GNSSModule::getMonth() {
    return nmea.getMonth();
}

float GNSSModule::getSpeed() {
   int kmh;
    float tmp = (float) nmea.getSpeed();
    // Convert to knot
    tmp /= 1000.0;
    // Convert to kph
    tmp *= 1.852;
    kmh= tmp;
    return kmh;
}
float GNSSModule::getAltitude(){
 long alt;
 nmea.getAltitude(alt);
 alt = (alt/1000) * 3.28084;   //Multiply 15 meters by 3.28084 and you get 49.2126 feet of tape.
return alt;
}

uint8_t GNSSModule::getSats() {
    return nmea.getNumSatellites();
}

double GNSSModule::getLatitude() {
    return (double) nmea.getLatitude() * 1e-6;
}

double GNSSModule::getLongitude() {
    return (double) nmea.getLongitude() *1e-6;
}

bool GNSSModule::step() {
    return readGNSS();
}
