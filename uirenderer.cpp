#include "uirenderer.h"
#include "serialutils.h"
#include "globalconfig.h"
#include <Arduino.h>

UIRenderer::UIRenderer()
  : _currentScreen(&BOOTSCREEN), _hasGNSS(false), _hasHeader(false), _hasPositionProvider(false) {
  _textBuffer = new char[(N_CHAR_PER_STAT + 1) * 2];
  _tLastRender = millis();
}

bool UIRenderer::initializeMap(SharedSPISDCard* sd) {
  if (_hasHeader) {
    _disp->createSprite(320, 480);  // create an LCD sprite for the size of LCD screen for map & status bar
    _disp->fillSprite(TFT_WHITE);   // fill the sprite background with a white colour
    _mapRenderer.initialize(_header, sd, _disp);
    if (_hasPositionProvider) {
      _mapRenderer.setPositionProvider(_posProvider);
    }
    return true;
  } else {
    return false;
  }
};


void UIRenderer::setDisplay(SharedSPIDisplay* display) {
  _disp = display;
  _hasDisplay = true;
}

void UIRenderer::setGNSS(GNSSModule* gnss) {
  _gnss = gnss;
  _hasGNSS = true;
}

void UIRenderer::setBMP280(BMP280* bmp280) {  // added to cater for pressure sensor
  _bmp280 = bmp280;
  _hasBMP280 = true;
}

void UIRenderer::setPositionProvider(GeoPositionProvider* positionProvider) {
  _posProvider = positionProvider;
  _hasPositionProvider = true;
  _mapRenderer.setPositionProvider(_posProvider);
}

void UIRenderer::setHeader(SimpleTile::Header* header) {
  _header = header;
  _hasHeader = true;
}

void UIRenderer::setGPXTrackIn(GPXTrack* track) {
  _track = track;
  _hasTrackIn = true;
  _mapRenderer.setGPXTrackIn(_track);
};

void UIRenderer::setScreen(Screen* newScreen) {
  _currentScreen = newScreen;
}

//************************************** step function ****************************************
// Note: This function always returns true such that it can be used to keep a while loop alive.
bool UIRenderer::step() {

  if (!_hasDisplay) return true;

  // _disp->clearDisplayBuffer(); for sharp display
  _currentScreen->render(_disp);
  if (_hasGNSS) {
    _gnss->step();
  }

  if (_currentScreen->hasMap && _hasPositionProvider) {
    _posProvider->step();

    _disp->pushSprite(0, 0);  //push the updated (map)sprite to the screen

    // Try to render map. If it is not ready, render the waiting screen instead
    if (!_mapRenderer.step(1)) {
      FIXWAITINGSCREEN.setCenterText(_textBuffer);
      FIXWAITINGSCREEN.render(_disp);
    }
  }

  // Note: Statusbar must be rendered after screen!
  if (_currentScreen->hasStatusBar) {
    renderStatusBar();
  }

  // Note:  VarioScreen must be rendered after screen!
  if (_currentScreen->hasVarioScreen) {

    renderVarioScreen();
  }

  // Note: Screen3 must be rendered after screen!
  if (_currentScreen->hasScreenThree) {
    renderScreenThree();
  }


  // Check if we need to wait before next render loop to achieve desired FPS.
  _tLoopRender = millis() - _tLastRender;
  if (_tLoopRender < TARGET_FRAME_TIME_MS) {
    delayMicroseconds((TARGET_FRAME_TIME_MS - _tLoopRender) * 1000);
  }
  _tLastRender = millis();

  return true;
}


void UIRenderer::renderStatusBar() {
  renderStat(_textBuffer);
}

void UIRenderer::renderVarioScreen() {  //Screen2 mainscreen
  renderVarioStat(_textBuffer);
}

void UIRenderer::renderScreenThree() {  //Screen3
  renderScreenThreeStat(_textBuffer);
}




void UIRenderer::renderStat(char* textBuff) {
  _disp->drawDataWindow();  // this draw lines/boarders of status bar area if needed

  snprintf(textBuff, N_CHAR_PER_STAT, "%.0f", _gnss->getAltitude());
  _disp->drawGPSdata(textBuff, 40, 310);  // added this to show text in the status bar

  snprintf(textBuff, N_CHAR_PER_STAT, "%.1f", _bmp280->calculatedvsi());
  //  snprintf(textBuff, N_CHAR_PER_STAT, "%.0f", _bmp280->AltiFeet(1013.25));
  _disp->drawGPSdatasmall(textBuff, 20, 365);  // added this to show text
                                               //Serial.println("Count");

  snprintf(textBuff, N_CHAR_PER_STAT, "%03i", _posProvider->getHeading());
  _disp->drawGPSdata(textBuff, 210, 365);  // added this to show text
                                           // snprintf(textBuff, N_CHAR_PER_STAT, "%s", _gnss->getDirection());
                                           //  _disp->drawGPSdata(textBuff, 360, 70);  // added this to show text
  snprintf(textBuff, 4, "%.0f", _gnss->getSpeed());
  _disp->drawGPSdata(textBuff, 210, 310);  // added this to show text
  //snprintf(textBuff, N_CHAR_PER_STAT, "%2.2f", _posProvider->getLatitude());
  // _disp->drawGPSdata(textBuff,256,170);    // added this to show text

  // snprintf(textBuff, N_CHAR_PER_STAT, "FL");
  // _disp->drawGPSdata(textBuff, 100, 380);  // added this to show text

  snprintf(textBuff, N_CHAR_PER_STAT, "%02i:%02i", _gnss->getHour(), _gnss->getMinute());
  _disp->drawGPSdatasmall(textBuff, 0, 430);  // added this to show text
                                              // snprintf(textBuff, N_CHAR_PER_STAT, "%2.2f", _gnss->getLatitude());
  snprintf(textBuff, N_CHAR_PER_STAT, "%.2f", _gnss->distancetowaypoint());
  _disp->drawGPSdatasmall(textBuff, 190, 430);  // added this to show text
  // _disp->drawWindIndicator();
}

//duplicated renderstartusbar for vario dial screen
void UIRenderer::renderVarioStat(char* textBuff) {
  _disp->drawVarioWindow(textBuff);
  snprintf(textBuff, N_CHAR_PER_STAT, "%.1f", _bmp280->calculatedvsi());
}

//duplicated renderstartusbar to show screen3
void UIRenderer::renderScreenThreeStat(char* textBuff) {
  _disp->drawScreenThree();
  //  snprintf(textBuff, N_CHAR_PER_STAT, "%.0f", _gnss->getAltitude());
  snprintf(textBuff, N_CHAR_PER_STAT, "%.0f", _bmp280->AltiFeet(1013.25));
  _disp->drawBigAlt(textBuff, 10, 100);  // added this to show text in the status bar
                                         // snprintf(textBuff, N_CHAR_PER_STAT, ":%03i", _posProvider->getHeading());
                                         // _disp->drawGPSdata2(textBuff, 170, 100);  // added this to show text
  snprintf(textBuff, N_CHAR_PER_STAT, "%.0f", _gnss->getSpeed());
  _disp->drawBigAlt(textBuff, 10, 230);  // added this to show text
}





// Perform a delay/sleep operation while keeping UI alive.
void UIRenderer::delay(uint64_t milliseconds) {
  long t_start = millis();
  while ((millis() - t_start) < milliseconds) {
    step();
  }
}

UIRenderer UIRENDERER;