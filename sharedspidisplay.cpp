
#include <SPI.h>
#include "screens.h"
#include "uirenderer.h"
#include "serialutils.h"




SharedSPIDisplay::SharedSPIDisplay(uint8_t _PIN_CS){};
//  : disp(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO){};

void SharedSPIDisplay::initialize() {
  disp.init();
  _spr->createSprite(SPRITE_WIDTH , SPRITE_HEIGHT); //sprite for moving map area
  // Update Bootscreen status
  BOOTSCREEN.displayOK = true;
  // Set display and screen for UI renderer
  UIRENDERER.setDisplay(this);
  UIRENDERER.setScreen(&BOOTSCREEN);
}

void SharedSPIDisplay::newPage() {
//  clearDisplay();
//  disp.setTextSize(1);
//  disp.setTextColor(ILI9341_BLACK,ILI9341_WHITE);
//  disp.setCursor(0, 5);
//  disp.cp437(true);
}

void SharedSPIDisplay::clearDisplay() {
  disp.fillScreen(TFT_WHITE);
}



// Just clear the buffer without rendering. Prevents "flashing"
void SharedSPIDisplay::clearDisplayBuffer() {
 // disp.clearDisplayBuffer();
  
}

void SharedSPIDisplay::drawCenterMarker() {
  int16_t x0, y0, x1, y1, x2, y2;
  x0 = 120 - POSITION_MARKER_SIZE;
  y0 = 120 + POSITION_MARKER_SIZE;
  x1 = 120;
  y1 = 120 - 2 * POSITION_MARKER_SIZE;
  x2 = 120 + POSITION_MARKER_SIZE;
  y2 = 120 + POSITION_MARKER_SIZE;
  disp.fillTriangle(x0, y0, x1, y1, x2, y2, ILI9341_RED);
};

// Screen2 basic flight data labels

 void SharedSPIDisplay::drawDataWindow() {
 // disp.fillRect(250, 0, 150, 240, ILI9341_WHITE);
 // disp.drawLine(250, 0, 250, 240,ILI9341_RED);
//  disp.setTextSize(1);
//  disp.setTextColor(ILI9341_BLACK);
//  disp.setCursor(256, 1);
//  disp.write("Alt (ft)");
//  disp.setCursor(256, 50);
//  disp.write("Hdg (deg)");
//  disp.setCursor(256, 100);
//  disp.write("Spd kmh");
//  disp.setCursor(256, 150);
//  disp.write("Wind speed/direction");
};

//Screen2 basic flight data when not needing map use this screen Altitude,
//heading,Vertical speed, speed over gound, windspeed/direction
 void SharedSPIDisplay::drawMainScreenWindow(){
  
//  disp.fillRect(0, 0, 400, 240, ILI9341_WHITE); //Draws a window over the map to display data the whole screen size
 // draw_line(250, 0, 250, 240, 4, BLACK);
 // disp.setTextSize(1);
//  disp.setFont(&FreeSerifBold24pt7b); //just numbers 0-9 32 pixel high
//  disp.setTextSize(1);
//  disp.setTextColor(ILI9341_BLACK);
//  disp.setCursor(10, 40);
//  disp.write("Alt(ft)");
    
//  disp.setTextColor(ILI9341_BLACK);
//  disp.setCursor(10, 100);
//  disp.write("Hdg ");

//  disp.setTextSize(1);
//  disp.setTextColor(ILI9341_BLACK);
//  disp.setCursor(10, 160);
//  disp.write("Spd km");

//  disp.setTextSize(1);
//  disp.setTextColor(ILI9341_BLACK);
//  disp.setCursor(10, 220);
//  disp.write("Wind dir");
//disp.setFont();
 };

//Screen3 basic GPS Altitude,

 void SharedSPIDisplay::drawScreenThree(){
//  disp.fillRect(0, 0, 400, 240, ILI9341_WHITE); //Draws a window over the map to display data the whole screen size
//  disp.setFont(&FreeSerifBold24pt7b); //just numbers 0-9 32 pixel high
//  disp.setTextSize(1);
//  disp.setTextColor(ILI9341_BLACK);
//  disp.setCursor(300, 40);
//  disp.write("FL");
//  disp.setCursor(300, 100);
//  disp.write("ft");
//  disp.setCursor(300, 230);
//  disp.write("kmh");
  };


 void SharedSPIDisplay::drawGPSdata(const char* statusStr,int lcdposx,int lcdposy){
 // disp.setFont();
  //disp.setTextSize(1);
  disp.setTextColor(TFT_BLACK,TFT_WHITE);
  disp.drawString(statusStr, lcdposx, lcdposy, 2);
//  disp.setCursor(lcdposx, lcdposy);
//  disp.cp437(true);
//  disp.write(statusStr);
 };
 //This prints the flight data string from unirender.cpp
 void SharedSPIDisplay::drawGPSdata2(const char* statusStr,int lcdposx,int lcdposy){
//  disp.setFont(&FreeSerifBold24pt7b);
//  disp.setTextSize(1);
//  disp.setTextColor(ILI9341_BLACK);
//  disp.setCursor(lcdposx, lcdposy);
//  disp.cp437(true);
//  disp.write(statusStr);
 };

 //This prints large altitude for screen3 data string from unirender.cpp
 void SharedSPIDisplay::drawBigAlt(const char* statusStr,int lcdposx,int lcdposy){
//  disp.setFont(&FreeSerifBold24pt7b);
//  disp.setTextSize(3);
//  disp.setTextColor(ILI9341_BLACK);
//  disp.setCursor(lcdposx, lcdposy);
//  disp.cp437(true);
//  disp.write(statusStr);
 };


void SharedSPIDisplay::refresh() {
 // disp.refresh();
}

void SharedSPIDisplay::setCursor(int16_t x, int16_t y) {
  disp.setCursor(x, y);
}

void SharedSPIDisplay::setTextFont(uint8_t f) {
  disp.setTextFont(f);
}

void SharedSPIDisplay::setTextSize(uint8_t s) {
  disp.setTextSize(s);
}


void SharedSPIDisplay::setTextWrap(uint8_t wrap) {
  disp.setTextWrap(wrap);
}

void SharedSPIDisplay::setRotation(int8_t rotation) {
  disp.setRotation(rotation);
}

void SharedSPIDisplay::setTextColor(uint16_t c,uint16_t bg) {
  disp.setTextColor(c,bg);
}

void SharedSPIDisplay::fillScreen(uint16_t c) {
  disp.fillScreen(c);
}

void SharedSPIDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {
  disp.drawPixel(x, y, color);
}

void SharedSPIDisplay::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  disp.fillCircle(x0, y0, r, color);
}

void SharedSPIDisplay::drawQuaterCircle(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) {
  disp.drawCircleHelper(x0, y0, r, cornername, color);
};

void SharedSPIDisplay::write(const char* str, bool hold) {
//  disp.setTextColor(ILI9341_BLACK);
//  disp.write(str);
  if (!hold) {
//    disp.refresh();
  }
}

void SharedSPIDisplay::drawString(const char* str,int16_t x, int16_t y, bool hold){
disp.drawString(str,x,y);
if (!hold){

}

}

void SharedSPIDisplay::write1(const char* str) {
//  disp.setTextColor(ILI9341_BLACK);
//  disp.write(str);
}
void SharedSPIDisplay::draw_line(int x0, int y0, int x1, int y1, uint8_t thickness, uint16_t color) {
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;

  if (dx < -dy) {
    while (1) {
      disp.drawPixel(x0, y0, color);
      for (int i = 0; i < thickness - 1; i++) {
        disp.drawPixel(x0 + thickness_offsets[i], y0, color);
      }
      if (x0 == x1 && y0 == y1) break;
      e2 = 2 * err;
      if (e2 > dy) {
        err += dy;
        x0 += sx;
      }
      if (e2 < dx) {
        err += dx;
        y0 += sy;
      }
    }
  } else {
    while (1) {
      disp.drawPixel(x0, y0, color);
      for (int i = 0; i < thickness - 1; i++) {
        disp.drawPixel(x0, y0 + thickness_offsets[i], color);
      }
      if (x0 == x1 && y0 == y1) break;
      e2 = 2 * err;
      if (e2 > dy) {
        err += dy;
        x0 += sx;
      }
      if (e2 < dx) {
        err += dx;
        y0 += sy;
      }
    }
  }
}


void SharedSPIDisplay::draw_dashedline(int x0, int y0, int x1, int y1, uint8_t thickness, uint8_t dashLength, uint16_t color) {
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;

  uint8_t cnt = 0;

  if (dx < -dy) {
    while (1) {
      ++cnt %= dashLength;
      if (!cnt) color = !color;

      disp.drawPixel(x0, y0, color);
      for (int i = 0; i < thickness - 1; i++) {
        disp.drawPixel(x0 + thickness_offsets[i], y0, color);
      }
      if (x0 == x1 && y0 == y1) break;
      e2 = 2 * err;
      if (e2 > dy) {
        err += dy;
        x0 += sx;
      }
      if (e2 < dx) {
        err += dx;
        y0 += sy;
      }
    }
  } else {
    while (1) {
      ++cnt %= dashLength;
      if (!cnt) color = !color;

      disp.drawPixel(x0, y0, color);
      for (int i = 0; i < thickness - 1; i++) {
        disp.drawPixel(x0, y0 + thickness_offsets[i], color);
      }
      if (x0 == x1 && y0 == y1) break;
      e2 = 2 * err;
      if (e2 > dy) {
        err += dy;
        x0 += sx;
      }
      if (e2 < dx) {
        err += dx;
        y0 += sy;
      }
    }
  }
}