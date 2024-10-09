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


// Note: This function always returns true such that it can be used to keep a while loop alive.
bool UIRenderer::step() {

  if (!_hasDisplay) return true;
  _disp->clearDisplayBuffer();
  _currentScreen->render(_disp);

  if (_hasGNSS) {
    _gnss->step();
  }

  if (_currentScreen->hasMap && _hasPositionProvider) {
    _posProvider->step();

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
  _disp->refresh();  // -> It is used to access the public members of a class, structure.

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

void UIRenderer::renderStat( char* textBuff) {
  _disp->drawDataWindow();          // added this to show text in the status bar
  snprintf(textBuff, N_CHAR_PER_STAT, "Ft:%.0f", _gnss->getAltitude());
  _disp->drawGPSdata(textBuff, 260, 20);  // added this to show text in the status bar
  snprintf(textBuff, N_CHAR_PER_STAT, "HE%03i", _posProvider->getHeading());
  _disp->drawGPSdata(textBuff, 260, 70);  // added this to show text
  snprintf(textBuff, N_CHAR_PER_STAT, "%s", _gnss->getDirection());
   _disp->drawGPSdata(textBuff, 360, 70);  // added this to show text
  snprintf(textBuff, N_CHAR_PER_STAT, "Km:%.0f", _gnss->getSpeed());
  _disp->drawGPSdata(textBuff, 260, 120);  // added this to show text
  //snprintf(textBuff, N_CHAR_PER_STAT, "%2.2f", _posProvider->getLatitude());
  // _disp->drawGPSdata(textBuff,256,170);    // added this to show text
  //snprintf(textBuff, N_CHAR_PER_STAT, "%2.2f", _posProvider->getLongitude());
  // _disp->drawGPSdata(textBuff,300,170);    // added this to show text
  snprintf(textBuff, N_CHAR_PER_STAT, "%02i:%02i", _gnss->getHour(), _gnss->getMinute());
  _disp->drawGPSdata(textBuff, 260, 215);  // added this to show text

  _disp->drawWindIndicator();
}


// Perform a delay/sleep operation while keeping UI alive.
void UIRenderer::delay(uint64_t milliseconds) {
  long t_start = millis();
  while ((millis() - t_start) < milliseconds) {
    step();
  }
}

UIRenderer UIRENDERER;