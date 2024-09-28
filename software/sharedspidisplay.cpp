#include <Arduino.h>

#include <SPI.h>
#include "sharedspidisplay.h"
#include "screens.h"
#include "uirenderer.h"
#include "serialutils.h"

SharedSPIDisplay::SharedSPIDisplay(uint8_t _PIN_CS): 
    disp(SHARP_SCK, SHARP_MOSI, SHARP_SS, DISPLAY_WIDTH, DISPLAY_HEIGHT, SPI_FREQ) {
   };

 
void SharedSPIDisplay::initialize() {
    while(!disp.begin()) {
        delay(100);
    }
  //  sout <= "Found display.";
    // Update Bootscreen status
    BOOTSCREEN.displayOK = true;
    // Set display and screen for UI renderer
    UIRENDERER.setDisplay(this);
    UIRENDERER.setScreen(&BOOTSCREEN);
}

void SharedSPIDisplay::newPage() {
    clearDisplay();
    disp.setTextSize(1);
    disp.setTextColor(BLACK);
    disp.setCursor(0, 5);
    disp.cp437(true);
}

void SharedSPIDisplay::clearDisplay() { 
    disp.clearDisplay();
}



// Just clear the buffer without rendering. Prevents "flashing"
void SharedSPIDisplay::clearDisplayBuffer() {
    disp.clearDisplayBuffer();
}
/*
void SharedSPIDisplay::drawCenterMarker() {
//    SMUTEX.aquireSPI(this);
    int16_t x0, y0, x1, y1, x2, y2;
    x0 = DISPLAY_WIDTH_HALF - POSITION_MARKER_SIZE;
    y0 = DISPLAY_HEIGHT_HALF + POSITION_MARKER_SIZE;
    x1 = DISPLAY_WIDTH_HALF;
    y1 = DISPLAY_HEIGHT_HALF - 2*POSITION_MARKER_SIZE;
    x2 = DISPLAY_WIDTH_HALF + POSITION_MARKER_SIZE;
    y2 = DISPLAY_HEIGHT_HALF + POSITION_MARKER_SIZE;
    disp.fillTriangle(x0, y0, x1, y1, x2, y2, BLACK);
};
*/

void SharedSPIDisplay::drawCenterMarker() {
    int16_t x0, y0, x1, y1, x2, y2;
    x0 = 120 - POSITION_MARKER_SIZE;
    y0 = 240 + POSITION_MARKER_SIZE;
    x1 = 120;
    y1 = 240 - 2*POSITION_MARKER_SIZE;
    x2 = 120 + POSITION_MARKER_SIZE;
    y2 = 240 + POSITION_MARKER_SIZE;
    disp.fillTriangle(x0, y0, x1, y1, x2, y2, BLACK);
};

// altered to get status bar at bottom of portrait sceen also its bigger to accomodate two lines of text

void SharedSPIDisplay::drawStatusBar(const char* statusStr) {
    disp.fillRect(0, 330, 240, 70, WHITE);
    draw_line(0, 330, 240, 330, 4, BLACK);
  //  draw_line(0, 240, 240, 240, 4, BLACK); //line through my position trangle, horizontal
  //  draw_line(120, 0, 120, 400, 4, BLACK); //line through my position trangle, vertical
    disp.setTextSize(3);
    disp.setTextColor(BLACK);
    disp.setCursor(5, 340);
    disp.cp437(true);
    disp.write(statusStr);
    disp.refresh();

};

void SharedSPIDisplay::refresh() {
    disp.refresh();
}

void SharedSPIDisplay::setCursor(int16_t x, int16_t y) {
    disp.setCursor(x, y);
}

void SharedSPIDisplay::setTextSize(uint8_t s) {
    disp.setTextSize(s);
}

void SharedSPIDisplay::setRotation(int8_t rotation) {
  disp.setRotation(rotation);
}

void SharedSPIDisplay::setTextColor(uint16_t c) {
    disp.setTextColor(c);
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
    disp.setTextColor(BLACK);
    disp.write(str);
    if(!hold) {
         disp.refresh();
    }
}
void SharedSPIDisplay::write1(char str) {
    disp.setTextColor(BLACK);
    disp.write(str);
  }

void SharedSPIDisplay::draw_line(int x0, int y0, int x1, int y1, uint8_t thickness, uint16_t color) {
    int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    if(dx < -dy) {
        while (1) {
        disp.drawPixel(x0, y0, color);
        for(int i=0; i<thickness-1; i++) {
            disp.drawPixel(x0+thickness_offsets[i], y0, color);
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 > dy) { err += dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
        }
    } else {
        while (1) {
        disp.drawPixel(x0, y0, color);
        for(int i=0; i<thickness-1; i++) {
            disp.drawPixel(x0, y0+thickness_offsets[i], color);
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 > dy) { err += dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
        }
    }

}


void SharedSPIDisplay::draw_dashedline(int x0, int y0, int x1, int y1, uint8_t thickness, uint8_t dashLength, uint16_t color) {
    int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    uint8_t cnt = 0;

    if(dx < -dy) {
        while (1) {
        ++cnt %= dashLength;
        if(!cnt) color = !color;

        disp.drawPixel(x0, y0, color);
        for(int i=0; i<thickness-1; i++) {
            disp.drawPixel(x0+thickness_offsets[i], y0, color);
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 > dy) { err += dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
        }
    } else {
        while (1) {
        ++cnt %= dashLength;
        if(!cnt) color = !color;

        disp.drawPixel(x0, y0, color);
        for(int i=0; i<thickness-1; i++) {
            disp.drawPixel(x0, y0+thickness_offsets[i], color);
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 > dy) { err += dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
        }
    }

}