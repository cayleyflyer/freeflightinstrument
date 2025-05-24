#include <stdint.h>
#ifndef _SHAREDSPIDISPLAY_H
#define _SHAREDSPIDISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>  // Hardware-specific library
#include "globalconfig.h"
#include "Free_Fonts.h"  // Include the header file attached to this sketch
#include "BMP280.h"
/*******************************************************************************
 * Sequence of offsets for line thickening.
 * 
 * Determines offsets of additional parallel lines with respect 
 * to the original line.
 ******************************************************************************/
const int thickness_offsets[6] = { 1, -1, 2, -2, 3, -3 };

class SharedSPIDisplay {

private:

  TFT_eSPI disp = TFT_eSPI();                  // Invoke custom library
  TFT_eSprite spr = TFT_eSprite(&disp);        // Declare Sprite object
  TFT_eSprite needlespr = TFT_eSprite(&disp);  // Sprite object for needle
  TFT_eSprite dialspr = TFT_eSprite(&disp);    // Sprite for meter reading
  TFT_eSprite altspr = TFT_eSprite(&disp);     // Sprite for meter reading
  TFT_eSprite vsispr = TFT_eSprite(&disp);     // Sprite for meter reading
  TFT_eSprite windspr = TFT_eSprite(&disp);    // sprite for windpointer
   float zoom;
  uint8_t _PIN_CS;
 BMP280* _bmp280;   //added to display altitude FL and QNH 
public:
  SharedSPIDisplay(uint8_t PIN_CS);
  void enableCS();
  void disableCS();

  void initialize();
  void newPage();

  void write(const char* str, bool hold = 0);
  void draw_String(const char* str, int16_t x, int16_t y, bool hold = 0);
  //void drawString(const char* str,int16_t x, int16_t y, bool hold =0);
  void drawString(const char* string, int16_t x, int16_t y, bool hold = 0);
  void drawText(const char* string, int16_t x, int16_t y, int8_t f);


  void write1(const char* str);
  void clearDisplay();
  void setRotation(int8_t rotation);
  void clearDisplayBuffer();
  void setCursor(int16_t x, int16_t y);
  void setTextFont(uint8_t f);
  void setTextSize(uint8_t s);
  void setTextColor(uint16_t c, uint16_t bg);
  void setText_Color(uint16_t c);
  void fillScreen(uint16_t c);
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  void drawQuaterCircle(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
  void setTextWrap(uint8_t wrap);
  void invertDisplay(uint8_t invert);
  void refresh();
  void drawCenterMarker();
  void drawDataWindow();
  void drawVarioWindow(const char* statusStr);  // drawvario dial and send vsi data to dial in variable "statusStr"
  void drawScreenThree();  // added to draw screen3 data window over map
  void drawWindIndicator();
  void drawGPSdata(const char* statusStr, int16_t lcdposx, int16_t lcdposy);
  void drawGPSdatasmall(const char* statusStr, int16_t lcdposx, int16_t lcdposy);
  void drawGPSdata2(const char* statusStr, int16_t lcdposx, int16_t lcdposy);
  void drawBigAlt(const char* statusStr, int lcdposx, int lcdposy);
  void draw_line(int x0, int y0, int x1, int y1, uint8_t thickness, uint16_t color);
  void draw_dashedline(int x0, int y0, int x1, int y1, uint8_t thickness, uint8_t dashLength, uint16_t color);

  //******************sprite functions********************************
  void createSprite(int16_t WIDTH, int16_t HEIGHT, uint8_t frames = 1);
  //void createDial(int16_t WIDTH, int16_t HEIGHT, uint8_t frames = 1);   // create the sprite image of dial into memory  
  void fillSprite(uint32_t color);
  void pushSprite(int32_t x, int32_t y);
  //void pushVarioImage (int32_t x0, int32_t y0, int32_t w, int32_t h, const uint16_t *data);

  void drawLine(int x0, int y0, int x1, int y1, uint8_t thickness, uint16_t color);
};

#endif
