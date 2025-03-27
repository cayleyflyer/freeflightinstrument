
#ifndef _MAPSPRITE_H
#define _MAPSPRITE_H

#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include "globalconfig.h"

const int linethick_offsets[6] = {1, -1, 2, -2, 3, -3};

class MapSprite {

private:
TFT_eSPI    tft = TFT_eSPI();         // Declare object "tft"
TFT_eSprite mapsprite = TFT_eSprite(&tft);  // Declare Sprite object "mapsprite" with pointer to "tft" object
uint8_t _PINN_CS;

public:
MapSprite(uint8_t PINN_CS);
void  createSprite(int16_t w, int16_t h, uint8_t frames);
void  pushSprite(int32_t x, int32_t y);
void  fillSprite(uint32_t color);
void  drawPixel(int32_t x, int32_t y, uint32_t color);
void  setColorDepth(int8_t b);
void  drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint8_t thickness,uint32_t color);

};

#endif
