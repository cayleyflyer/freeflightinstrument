#pragma once
#include <Adafruit_GFX.h>

const uint8_t LargeNumbers_Bitmaps[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const GFXglyph LargeNumbers_Glyphs[] PROGMEM = {
  {     0,   1,   1,   4,    0,  -36 },   // 0x2E '.'
  {     0,   0,   0,   0,    0,    0 },   // 0x2F '/'
  {     1,   1,   1,   4,    0,  -36 },   // 0x30 '0'
  {     2,   1,   1,   4,    0,  -36 },   // 0x31 '1'
  {     3,   1,   1,   4,    0,  -36 },   // 0x32 '2'
  {     4,   1,   1,   4,    0,  -36 },   // 0x33 '3'
  {     5,   1,   1,   4,    0,  -36 },   // 0x34 '4'
  {     6,   1,   1,   4,    0,  -36 },   // 0x35 '5'
  {     7,   1,   1,   4,    0,  -36 },   // 0x36 '6'
  {     8,   1,   1,   4,    0,  -36 },   // 0x37 '7'
  {     9,   1,   1,   4,    0,  -36 },   // 0x38 '8'
  {    10,   1,   1,   4,    0,  -36 }    // 0x39 '9'
};

const GFXfont LargeNumbers PROGMEM = {(uint8_t *) LargeNumbers_Bitmaps,  (GFXglyph *)LargeNumbers_Glyphs, 0x2E, 0x39,  36};