#include <SPI.h>
#include "screens.h"
#include "uirenderer.h"
#include "serialutils.h"




SharedSPIDisplay::SharedSPIDisplay(uint8_t _PIN_CS){};
//  : disp(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO){};

void MapSprite::createSprite(int16_t w, int16_t h, uint8_t frames =1){
  mapsprite.createSprite();
}
void MapSprite::pushSprite(int32_t x, int32_t y){
  mapsprite.pushSprite();
}

void MapSprite::fillSprite(uint32_t color){
  mapsprite.fillSprite();
}

void MapSprite::drawPixel(int32_t x, int32_t y, uint32_t color){
  mapsprite.drawPixel();
}

void MapSprite::setColorDepth(int8_t b){
  mapsprite.setColorDepth();
}



void SharedSPIDisplay::draw_line(int x0, int y0, int x1, int y1, uint8_t thickness, uint16_t color) {
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;

  if (dx < -dy) {
    while (1) {
      mapsprite.drawPixel(x0, y0, color);
      for (int i = 0; i < thickness - 1; i++) {
        mapsprite.drawPixel(x0 + linethick_offsets[i], y0, color);
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
      mapsprite.drawPixel(x0, y0, color);
      for (int i = 0; i < thickness - 1; i++) {
        mapsprite.drawPixel(x0, y0 + thickness_offsets[i], color);
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
