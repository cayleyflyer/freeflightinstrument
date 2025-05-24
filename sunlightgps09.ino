


/*tft sprite for map routines
create sprite is in uirenderer.ccp UIRenderer::initializeMap
fillSprite    is in uirenderer.ccp UIRenderer::initializeMap
pushSprite    to the sceen is in uirenderer.cpp UIRenderer::step()
drawline      ie draw the map nodes is in tileBlockRenderer.cpp TileBlockRenderer::render
plot towns    ie draw the towns and names is in tileBlockRenderer.cpp TileBlockRenderer::renderGPX
*/



//$GPRMC,103959.00,A,5353.82145,N,00009.98861,W,0.261,,230724,,,A,V*17
//$GPGGA,103959.00,5353.82145,N,00009.98861,W,1,12,0.95,25.0,M,46.3,M,,*7D

#include <Arduino.h>

#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include "Free_Fonts.h"  // Include the header file attached to this sketch
#include <TinyGPSPlus.h>
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
#include "zoomclass.h"
#include "vsiclass.h"
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include "BMP280.h"
#include <TFT_eSPI.h>  // Hardware-specific library
#include <vector>
#include <esp_display_panel.hpp>
#include <Wire.h>

//#define I2C_SDA 7
//#define I2C_SCL 6

#define DISPLAY_CS 42
#define SDCARD_CS 21
//TwoWire I2CBMP = TwoWire(1);//Create a new TwoWire instance.

// Header of bin map file
SimpleTile::Header header;
uint16_t currHeading = 0;
SoftwareSerial SpeechSerial(6, 7);  // For speech chip RX, TX
SharedSPIDisplay display(DISPLAY_CS);
SharedSPISDCard sdcard(SDCARD_CS);
SPIClass vspi = SPIClass(VSPI);  // setup 2nd 'hspi' for SDCard

GNSSModule gnss(2);  // sets the Hardware UART to 0,1 or 2 on the ESPS3  0 is for the usb port so 1,2  should be used
BMP280 bmp280;
GPXTrack track;


InterpPositionProvider ipos(&gnss, ((float)GNSS_MIN_UPDATE_TIME_MS) / ((float)TARGET_FRAME_TIME_MS));


const char binary_path[] = "/map.bin";  // Path to map-file on SD-card
const char gpx_path[] = "/track.gpx";   // Path to gpx-track file on SD-card/
int incomingByte = 0;

//variable for vertical speed calculation
unsigned long previousVSIMillis = 0, currentMillis = 0;
float currentAltitude = 0;
float previousAltitude = 0;
float SmoothData;
float vertical_speed = 0;
bool calculatingvsi = false;

void setup() {

  Serial.begin(115200);
  SpeechSerial.begin(9600);  // setup sofware uart for speech command input
  sleep(2);
  Serial.println("setup");
  pinMode(5, OUTPUT);      // power for bmp280 ,,low current 70ua
  pinMode(47, OUTPUT);     // sets IM0,IM1 to 11  SPI mode on LCD
  pinMode(48, OUTPUT);     // sets IM0,IM1 to 11  SPI mode on LCD
  pinMode(13, OUTPUT);     // 13 is rst for lcd so needs to be an output
  digitalWrite(5, HIGH);   // set bmp280 voltage GPIOpin to high
  digitalWrite(13, HIGH);  // stets LCD RST initialy to HIGH
  digitalWrite(47, HIGH);  // sets IM0, to HIGH
  digitalWrite(48, HIGH);  // sets IM1 to HIGH
                           //uint32_t freq = 100000;
  SPI.setFrequency(SPI_FREQ);
  //  Wire1.begin(I2C_SDA, I2C_SCL, 400000); // For BMP280 I2C bus on pins 7clk,8sda
  Wire.begin(7, 6);  // For BMP280 I2C bus on pins 7clk,8sda
                     // I2Ctwo.begin(7, 6, freq); // Wire1.begin(7,6)
                     //bool status2 = bmp280.begin(0x76, &Wire1);
  bmp280.begin();
  vspi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);  // map spi pins to esp32-s3 board
  SD.begin(SD_CS, vspi, SPI_FREQ);              // Begin SDCard on 2nd SPI
  Serial2.begin(57600, SERIAL_8N1, 9, 8);       // map UART2 to pin rx9 and tx8
  display.initialize();                         // initialise LCD
  display.setRotation(0);                       // set screen to portrait mode 1 landscape 0
  display.fillScreen(TFT_WHITE);
  display.setTextWrap(0);  // set text wrapp to off so name do not cut across two lines
  display.invertDisplay(1);

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
  UIRENDERER.setBMP280(&bmp280);  // added for pressure sensor

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
 //  UIRENDERER.setScreen(&VARIOSCREEN);  // calls VarioScreen() in screens.h class sets hasVarioScreen=true and hasmap to false
}


// main loop calls only vsi and uirenderer
void loop() {
  currentMillis = millis();
  uirender();
  CalcVerticalSpeed();
}



// only steps  uirenderer if vsi calculation has been completed
void uirender() {
  if (calculatingvsi == false) {
    UIRENDERER.step();
  }
}
// Not sure if this is the way to do it!!!!!!
// But I have created a vsiclass.cpp and vsiclass.h to create a  "static float NewVsi;""
// I can then put the vertical speed reading into it here and make it accessable from the uirenderer.cpp 
// The VSI reading is the primary flight instrument data which is needed every 500ms with priority other other tasks.
// once the function is called, i set variable calculatingvsi = true; hopfully this will stop any other proccess starting as they check for a 
// false flag before running. ie the above uirenderer funtion will only run when this flag is false which controls the map rendering and gps stuff


// calclate the vertical speed in meters per second from the bmp280 pressure sensor
void CalcVerticalSpeed() {
  if (currentMillis - previousVSIMillis >= 500) {
    previousVSIMillis += currentMillis;
    calculatingvsi = true;                                       // set a flag to ensure this function completes calculation
    currentAltitude = bmp280.AltiMeters(1013.25);                // Read current QNH altitude relative to std atmos pressure of 101325 
    vertical_speed = (currentAltitude - previousAltitude) /0.5;  // Calculate vertical speed delta altitude /delta time ie 0.5 seconds
    SmoothData = SmoothData - (0.025 * (SmoothData - vertical_speed)); // apply filter to smooth out data 
    previousAltitude = currentAltitude;                          // Update previous altitude reading
    VSIClass::NewVsi = SmoothData;  // make smoothed data available to bmp280.cpp function calculatedvsi()
  }
    calculatingvsi = false;                                        // once calclation is complete set the flag to false allowing othe proccesses to be completed
}
