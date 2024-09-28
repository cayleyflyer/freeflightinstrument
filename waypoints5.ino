//GeoPosition currPos(53.897078, -0.166197);
#include <Arduino.h>
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

TileBlockRenderer togglezoom;
void setup() {

  
  pinMode(BUTTON_PIN, INPUT);  // set arduino pin to input mode
  button.setDebounceTime(50); // set debounce time to 50 milliseconds

  sleep(2);
  SPI.setFrequency(SPI_FREQ);
  SPI.begin();
  Serial.begin(9600);
  display.initialize();
  display.setRotation(1);   // set screen to portrait mode
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


  // button to toggle between two  zoom levels 0.5 an 1.0 so as i get close to a waypoint i can increase resolution  the center marker is 800meters tall at zoom 0.5
   button.loop();                           // MUST call the loop() function first
    if(button.isPressed()) {
    Serial.println("The button is pressed");

if(zoomvalue == 1.0){
  zoomvalue = 0.5;
  togglezoom.setZoom(0.5);                 // changes zoom number ,, but does not work changing zoom on screen
   Serial.println("zoomvalue = 0.5");
}
else{
  zoomvalue = 1.0;
  togglezoom.setZoom(1.0); 
  Serial.println("zoomvalue = 1.0");         // changes zoom ,, but does not work     changing zoom on screen
}

}

    
UIRENDERER.step();

}

