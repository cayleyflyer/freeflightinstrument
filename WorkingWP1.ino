//GeoPosition currPos(53.897078, -0.166197);
#include <Arduino.h>

//The aim is to produce a small power efficient and simple map using Maurice Pannards bike-computer-32 code 
//addapted for freeflying built around a low power ESP32. Renders a map based on the pilots current position 
//showing UK airspace and overlays a GPX file showing user defined waypoints, railwaystations (for getting home) 
//and towns. Featuring OSM-based offline maps. To Use bluetooth gps and barometric altitude data derived from 
//XCTracer/Skydrop type flight instrumnet to reduce the projects physical size and power needs. 
//Uses Sharp memory sunlight readable 400 x 240 monchrome display.
//disclaimer:- Air space maps issued by the CAA are the only maps that can be used in air navigation. This project is not for use in primary air navigation.

//$LXWPW, kmh, baroaltitude, vario in m/s, , , , , , heading, windcourse, windspeed*checksum 


#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <MicroNMEA.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include "gnssmodule.h"
#include "sharedspisdcard.h"
#include "sharedspidisplay.h"
#include "simpletile.h"
#include "constgeoposition.h"
#include "tileblockrenderer.h"
#include "uirenderer.h"
#include "serialutils.h"
#include "globalconfig.h"
#include "interppositionprovider.h"
#include <ezButton.h>
#define DISPLAY_CS 2
#define SDCARD_CS 2

// Header of bin map file
SimpleTile::Header header;

uint16_t currHeading = 0;


SharedSPIDisplay display(DISPLAY_CS);
SharedSPISDCard sdcard(SDCARD_CS);
GNSSModule gnss(0);
GPXTrack track;
TileBlockRenderer togglezoom; //set default zoomLevel

InterpPositionProvider ipos(&gnss, ((float)GNSS_MIN_UPDATE_TIME_MS) / ((float)TARGET_FRAME_TIME_MS));

// Path to map-file on SD-card
const char binary_path[] = "/map.bin";
// Path to gpx-track file on SD-card
const char gpx_path[] = "/track.gpx";

// constants won't change
const int BUTTON_PIN = 2;  // Connect the Button to pin 2 for zoom change on the fly 
// variables will change:
int lastButtonState;        // the previous state of button
int currentButtonState;     // the current state of button
float zoomvalue = 1.0;      // default zoom level
ezButton button(BUTTON_PIN);  // create ezButton object that attach to pin 2;


void setup() {

  
  pinMode(BUTTON_PIN, INPUT);  // set arduino pin to input mode
  button.setDebounceTime(50); // set debounce time to 50 milliseconds

  sleep(2);
  SPI.setFrequency(SPI_FREQ);
  SPI.begin();
  Serial.begin(9600);
  display.initialize();
  display.setRotation(0);   // set screen to portrait mode
  display.setTextWrap(0);   // set text wrapp to off so name do not cut across two lines
  togglezoom.setZoom(1.0);
  gnss.initialize();
  sdcard.initialize();
  sdcard.setMapPath(binary_path);
  sdcard.setGPXTrackInPath(gpx_path);
  while (!sdcard.readHeader(header)) {
    UIRENDERER.delay(100);
  }
  UIRENDERER.step();
  if (sdcard.readGPX(header, track)) {
    UIRENDERER.setGPXTrackIn(&track);
    BOOTSCREEN.trackOK = true;
  } else {
    // Track could not be initialized. Setup continues.
    BOOTSCREEN.trackOK = -1;
    UIRENDERER.delay(500);
  }
  UIRENDERER.setHeader(&header);
  UIRENDERER.setGNSS(&gnss);
  if (!UIRENDERER.initializeMap(&sdcard)) {
    // Map could not be initialized due to insufficient memory. Setup stops.
    BOOTSCREEN.mapOK = -1;
    while (1) {
      UIRENDERER.delay(1000);
    }
  } else {
    BOOTSCREEN.mapOK = true;
  }
  UIRENDERER.setPositionProvider(&ipos);

  // Setup was successful. Render some more frames of the bootscreen to show it.
  UIRENDERER.delay(500);

  // Change to map
  UIRENDERER.setScreen(&MAPSCREEN);
}

void loop() {
   button.loop();                           // MUST call the loop() function first
    if(button.isPressed()) {
    Serial.println("The button is pressed");

if(zoomvalue == 1.0){
  zoomvalue = 0.5;
   Serial.println("zoomvalue = 0.5");
   togglezoom.setZoom(0.5);                // changes zoom number ,, but does not work changing zoom on screen  
   togglezoom.getZoom();
 //  togglezoom.test();
}
else{
  zoomvalue = 1.0;
   Serial.println("zoomvalue = 1.5");         // changes zoom ,, but does not work     changing zoom on screen
  togglezoom.setZoom(1.0); 
  togglezoom.getZoom();
   // togglezoom.test();
}

}

    
UIRENDERER.step();

}


