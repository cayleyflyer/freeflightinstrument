#include <stdint.h>
#ifndef _SHAREDSPIDISPLAY_H
#define _SHAREDSPIDISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include "globalconfig.h"
#include "mapsprite.h"

/*******************************************************************************
 * Sequence of offsets for line thickening.
 * 
 * Determines offsets of additional parallel lines with respect 
 * to the original line.
 ******************************************************************************/
const int thickness_offsets[6] = {1, -1, 2, -2, 3, -3};

class SharedSPIDisplay {

private:
   MapSprite* _spr;
   // Adafruit_SharpMem disp;
   //Adafruit_ILI9341 disp;
    TFT_eSPI disp = TFT_eSPI();       // Invoke custom library
    
    float zoom;
    uint8_t _PIN_CS;

public:
    SharedSPIDisplay(uint8_t PIN_CS);
    void enableCS();
    void disableCS();

    void initialize();
    void newPage();

    void write(const char* str, bool hold=0);
    void drawString(const char* str,int16_t x, int16_t y, bool hold =0);
    void write1(const char* str);
    void clearDisplay();
    void setRotation(int8_t rotation);
    void clearDisplayBuffer();
    void setCursor(int16_t x, int16_t y);
    void setTextFont(uint8_t f);
    void setTextSize(uint8_t s);
    void setTextColor(uint16_t c,uint16_t bg);
    void fillScreen(uint16_t c);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawQuaterCircle(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
    void setTextWrap(uint8_t wrap);

 
    void refresh();
    void drawCenterMarker();
    void drawDataWindow();
    void drawMainScreenWindow();   // added to draw screen2 data window over map
    void drawScreenThree();        // added to draw screen3 data window over map
    void drawWindIndicator();
    void drawGPSdata(const char* statusStr,int lcdposx,int lcdposy);
    void drawGPSdata2(const char* statusStr,int lcdposx,int lcdposy);
    void drawBigAlt(const char* statusStr,int lcdposx,int lcdposy);
 
    void draw_line(int x0, int y0, int x1, int y1, uint8_t thickness, uint16_t color);

    void draw_dashedline(int x0, int y0, int x1, int y1, uint8_t thickness, uint8_t dashLength, uint16_t color);

};

#endif
