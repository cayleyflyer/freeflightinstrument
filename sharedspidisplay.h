#include <stdint.h>
#ifndef _SHAREDSPIDISPLAY_H
#define _SHAREDSPIDISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include "globalconfig.h"


/*******************************************************************************
 * Sequence of offsets for line thickening.
 * 
 * Determines offsets of additional parallel lines with respect 
 * to the original line.
 ******************************************************************************/
const int thickness_offsets[6] = {1, -1, 2, -2, 3, -3};

class SharedSPIDisplay {

private:

    TFT_eSPI disp = TFT_eSPI();            // Invoke custom library
    TFT_eSprite spr = TFT_eSprite(&disp);  // Declare Sprite object

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
    void draw_String(const char* str,int16_t x, int16_t y, bool hold =0);
    void write1(const char* str);
    void clearDisplay();
    void setRotation(int8_t rotation);
    void clearDisplayBuffer();
    void setCursor(int16_t x, int16_t y);
    void setTextFont(uint8_t f);
    void setTextSize(uint8_t s);
    void setTextColor(uint16_t c,uint16_t bg);
    void setText_Color(uint16_t c);
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
    
    //******************sprite functions********************************
    void createSprite(int16_t WIDTH, int16_t HEIGHT,uint8_t frames = 1);
    void fillSprite(uint32_t color);
    void pushSprite(int32_t x, int32_t y);
    void drawLine(int x0, int y0, int x1, int y1, uint8_t thickness, uint16_t color);
    
     };

#endif
