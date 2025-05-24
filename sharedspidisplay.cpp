#include <stdint.h>

#include <SPI.h>
#include "screens.h"
#include "uirenderer.h"
#include "serialutils.h"
#include "sharedspidisplay.h"
// Bmp image array attached to this sketch
#include "dial.h"
#include "needle.h"
#include "windpoint.h"


SharedSPIDisplay::SharedSPIDisplay(uint8_t _PIN_CS){};
//  : disp(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO){};

void SharedSPIDisplay::initialize() {

  dialspr.createSprite(318, 318);            // create an LCD sprite for vario dial
  needlespr.createSprite(12, 124);           // create the sprite image of needle 14x130 pixels bmp into memory
  windspr.createSprite(28, 158);             // create the sprite image of windpointer 13x145 pixels bmp into memory
  dialspr.setSwapBytes(true);                // colours are not correct so this rectifies
  needlespr.setSwapBytes(true);              // colours are not correct so this rectifies
  windspr.setSwapBytes(true);                // colours are not correct so this rectifies
  vsispr.createSprite(150, 60);              // creat a sprite box 150pix x 60pix to hold altitude text
  vsispr.setTextColor(TFT_BLUE, TFT_BLACK);  // make the altitude text Green on a Black background





  disp.init();
  // while (! disp.begin()) {
  //   delay(100);
  //  }
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



void SharedSPIDisplay::createSprite(int16_t WIDTH, int16_t HEIGHT, uint8_t frames) {
  spr.createSprite(WIDTH, HEIGHT, frames);
}


void SharedSPIDisplay::fillSprite(uint32_t color) {
  spr.fillSprite(color);
}

void SharedSPIDisplay::pushSprite(int32_t x, int32_t y) {
  spr.pushSprite(x, y);
}

void SharedSPIDisplay::clearDisplay() {
  // disp.clearDisplay();
}



// Just clear the buffer without rendering. Prevents "flashing"
void SharedSPIDisplay::clearDisplayBuffer() {
  // disp.clearDisplayBuffer();
}

void SharedSPIDisplay::drawCenterMarker() {
  int16_t x0, y0, x1, y1, x2, y2;
  x0 = 160 - POSITION_MARKER_SIZE;
  y0 = 160 + POSITION_MARKER_SIZE;
  x1 = 160;
  y1 = 160 - 2 * POSITION_MARKER_SIZE;
  x2 = 160 + POSITION_MARKER_SIZE;
  y2 = 160 + POSITION_MARKER_SIZE;
  spr.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_RED);  // draws centermarker on the sprite, not the TFT Screen!
};

// Screen2 basic flight data labels
void SharedSPIDisplay::drawDataWindow() {
  spr.drawLine(0, 300, 320, 300, TFT_RED);
  spr.drawLine(0, 301, 320, 301, TFT_RED);
  spr.drawLine(0, 302, 320, 302, TFT_RED);
  spr.drawString("Alt", 1, 310, 4);
  spr.drawString("ft", 1, 335, 4);
  spr.drawString("FL", 1, 370, 4);
  spr.drawString("Spd", 170, 310, 4);
  spr.drawString("km", 170, 335, 4);
  spr.drawString("Hdg", 170, 370, 4);
  spr.drawString("deg", 170, 390, 4);
  spr.drawString("Dist", 140, 420, 4);
  spr.drawString("/km", 140, 445, 4);
};


void deleteSprite();


//Vertical speed analog dial
void SharedSPIDisplay::drawVarioWindow(const char* statusStr) {
  int angle = _bmp280->calculatedvsi();
  int windangle = 90;
  char* floatStr;

  // int anglemap = map(SmoothData, 0, 5/100, 90, 250);
    dialspr.pushImage(0, 0, 318, 318, dial);              //puts "dial" bmp onto screen sprite at x0,y0,w,h,bmp
    vsispr.fillSprite(TFT_BLACK);   
    
    windspr.pushImage(0, 0, 28, 158, windpointer);        //draw image of needle into memory
    windspr.setPivot(15, 0);                              //x left to right reduce moves left y reduce moves down
    windspr.pushRotated(&dialspr, windangle, TFT_WHITE);  //rotates windarrow acording to angle passed to this routine
    
    needlespr.setPivot(8, 8);                             //places the neele pivot point in centre of dial
    needlespr.pushImage(0, 0, 12, 124, needlepointer);    //draw image of needle into memory
    needlespr.pushRotated(&dialspr, angle, TFT_RED);      //rotates needle acording to angle passed to this routine
   
    vsispr.fillSprite(TFT_BLACK);  
    vsispr.drawString(statusStr, 0, 0, 6);                 //drawString(text, lcdposx, lcdposy, font) draws data to the sprite not the TFT Screen
    vsispr.pushToSprite(&dialspr, 110, 140, TFT_BLACK);   //draw the altitude text onto dial and make the BLACK text box background transparent
    dialspr.pushSprite(0, 0);                             //now push that dial sprite to the screen

};



//Screen3 basic GPS Altitude,
void SharedSPIDisplay::drawScreenThree(){};


void SharedSPIDisplay::drawGPSdata(const char* statusStr, int16_t lcdposx, int16_t lcdposy) {
  //disp.setFont(&FreeSerifBold24pt7b);
  spr.setTextColor(TFT_BLACK, TFT_WHITE);          // draws data to the sprite not the TFT Screen
  spr.drawString(statusStr, lcdposx, lcdposy, 7);  // drawString(text, lcdposx, lcdposy, font) draws data to the sprite not the TFT Screen
};

void SharedSPIDisplay::drawGPSdatasmall(const char* statusStr, int16_t lcdposx, int16_t lcdposy) {
  //disp.setFont(&FreeSerifBold24pt7b);
  spr.setFreeFont(FSB24);
  spr.setTextColor(TFT_BLUE, TFT_WHITE);               // draws data to the sprite not the TFT Screen
  spr.drawString(statusStr, lcdposx, lcdposy, GFXFF);  // drawString(text, lcdposx, lcdposy, font) draws data to the sprite not the TFT Screen
};

//This prints the flight data string from unirender.cpp
void SharedSPIDisplay::drawGPSdata2(const char* statusStr, int16_t lcdposx, int16_t lcdposy){
  //  disp.setFont(&FreeSerifBold24pt7b);
  //  disp.setTextSize(1);
  //  disp.setTextColor(ILI9341_BLACK);
  //  disp.setCursor(lcdposx, lcdposy);
  //  disp.cp437(true);
  //  disp.write(statusStr);
};

//This prints large altitude for screen3 data string from unirender.cpp
void SharedSPIDisplay::drawBigAlt(const char* statusStr, int lcdposx, int lcdposy) {
  vsispr.setFreeFont(FSB24);
  vsispr.setTextColor(TFT_BLUE, TFT_WHITE);               // draws data to the sprite not the TFT Screen
  vsispr.drawString(statusStr, lcdposx, lcdposy, GFXFF);  // drawString(text, lcdposx, lcdposy, font) draws data to the sprite not the TFT Screen
};


void SharedSPIDisplay::refresh() {
  // disp.refresh();
}
void SharedSPIDisplay::invertDisplay(uint8_t invert) {
  disp.invertDisplay(invert);
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

void SharedSPIDisplay::setTextColor(uint16_t c, uint16_t bg) {
  disp.setTextColor(c, bg);
}

void SharedSPIDisplay::setText_Color(uint16_t c) {
  spr.setTextColor(c);
}



void SharedSPIDisplay::fillScreen(uint16_t c) {
  disp.fillScreen(c);
}

void SharedSPIDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {
  disp.drawPixel(x, y, color);
}

void SharedSPIDisplay::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  spr.fillCircle(x0, y0, r, color);
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

// draw string to tft screen
void SharedSPIDisplay::draw_String(const char* str, int16_t x, int16_t y, bool hold) {
  // void draw_String(const char *string, int32_t x, int32_t y, uint8_t font);
  disp.drawString(str, x, y);
  if (!hold) {
  }
}

//draws place names onto sprite from gpx file doing it this way place name remain horizontal as map moves ie when flying in circles
void SharedSPIDisplay::drawText(const char* string, int16_t x, int16_t y, int8_t f) {
  spr.setFreeFont(FMB12);
  spr.setTextColor(TFT_BLACK, TFT_WHITE);
  spr.drawString(string, x, y, f);
}


//draws string to sprite
void SharedSPIDisplay::drawString(const char* string, int16_t x, int16_t y, bool hold) {
  spr.drawString(string, x, y);
  if (!hold) {
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
// in this drawline note the spr.drawpixel to draw pixel to sprite instead of screen!
void SharedSPIDisplay::drawLine(int x0, int y0, int x1, int y1, uint8_t thickness, uint16_t color) {
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;

  if (dx < -dy) {
    while (1) {
      spr.drawPixel(x0, y0, color);
      for (int i = 0; i < thickness - 1; i++) {
        spr.drawPixel(x0 + thickness_offsets[i], y0, color);
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
      spr.drawPixel(x0, y0, color);
      for (int i = 0; i < thickness - 1; i++) {
        spr.drawPixel(x0, y0 + thickness_offsets[i], color);
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