


/*tft sprite for map routines
create sprite is in uirenderer.ccp UIRenderer::initializeMap
fillSprite    is in uirenderer.ccp UIRenderer::initializeMap
pushSprite    to the sceen is in uirenderer.cpp UIRenderer::step()
drawline      ie draw the map nodes is in tileBlockRenderer.cpp TileBlockRenderer::render
plot towns    ie draw the towns and names is in tileBlockRenderer.cpp TileBlockRenderer::renderGPX
*/





#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
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
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include "BMP280.h"
#include <TFT_eSPI.h> // Hardware-specific library

#define DISPLAY_CS 6
#define SDCARD_CS 1

// Header of bin map file
SimpleTile::Header header;
uint16_t currHeading = 0;
SoftwareSerial SpeechSerial(13, 14);  // For speech chip RX, TX
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


void setup() {
   Serial.begin(115200);
  SpeechSerial.begin(9600);  //setup sofware uart for speech command input
  sleep(2);
  SPI.setFrequency(SPI_FREQ);
  //  Wire.begin(11, 10);  //Join I2C bus
  bmp280.begin();
  // SD card pin connections ESP32-S3
  vspi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);  // map spi pins to esp32-s3 board
  SD.begin(SD_CS, vspi, SPI_FREQ);              // Begin SDCard on 2nd SPI
 
  Serial2.begin(57600, SERIAL_8N1, 11, 12);  // map UART2 to pin 8 and 9
  display.initialize();                      // initialise SharpMemory LCD
  display.setRotation(1);                    // set screen to portrait mode 1 landscape 0
  display.fillScreen(TFT_WHITE);
  display.setTextWrap(0);  // set text wrapp to off so name do not cut across two lines


  gnss.initialize();
  //Serial2.begin(9600, SERIAL_8N1,8,9); //map uart2 to pin 8 and 9//

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
}

void loop() {
  Serial.println("loop");
  while (SpeechSerial.available() > 0) {
    incomingByte = SpeechSerial.read();  // read the incoming byte:

    if (incomingByte > 91) {
      Serial.print(" I received:");
      Serial.println(incomingByte);

      switch (incomingByte) {
        case 202:
          ZoomClass::NewZoom = 0.5;
          break;
        case 201:
          ZoomClass::NewZoom = 1.5;
          break;
        case 210:
          BOOTSCREEN.mapOK = true;
          UIRENDERER.setScreen(&MAPSCREEN);  //Screen1 Map screen
          break;
        case 211:
          UIRENDERER.setScreen(&MAINSCREEN);  //Screen2 Basic Flight Data
          break;
        case 212:
          UIRENDERER.setScreen(&SCREENTHREE);  //Screen3 big digits alt/vario
          break;
        default:
          break;
      }
    }
  }
  UIRENDERER.step();
}
