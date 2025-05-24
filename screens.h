#ifndef _SCREENS_H
#define _SCREENS_H

#include <Arduino.h>
#include "screenelements.h"
#include <vector>
#include "sharedspidisplay.h"
#include "globalconfig.h"


class Screen {

protected:
  uint64_t _tick;
  std::vector<ScreenElement*> _elements;

public:
  Screen()
    : hasStatusBar(false), hasMap(false), hasVarioScreen(false), hasScreenThree(false){};  //added hasmainscreen  for screen2 and screen3 5/12/24
  bool hasStatusBar, hasMap, hasVarioScreen, hasScreenThree;                               //added hasmainscreen  for screen2 and screen3 5/12/24
  virtual void reset() = 0;
  virtual bool render(SharedSPIDisplay* display) = 0;
};


class BootScreen : public Screen {

private:
  int textSize = 2;
  int spinnerSize = 15;
  int rows[7] = { 5, 35, 65, 95, 125, 155, 190 };
  int cols[2] = { 5, 120 };
 // disp->setFreeFont(FSB18);       // Select Free Serif 12 point font
  ScreenText _labelDisplay = ScreenText(cols[0], rows[0], textSize, "Display");
  ScreenText _labelGnss = ScreenText(cols[0], rows[1], textSize, "GNSS");
  ScreenText _labelSD = ScreenText(cols[0], rows[2], textSize, "SDCard");
  ScreenText _labelMap = ScreenText(cols[0], rows[3], textSize, "Map");
  ScreenText _labelTrack = ScreenText(cols[0], rows[4], textSize, "Track");
  ScreenText _SoftwareVersion = ScreenText(cols[0], rows[5], textSize, "sunlightgps05");
  ScreenText _MapVersion = ScreenText(cols[0], rows[6], textSize, "Map size 4096");

  // Note: these labels are used as center elements for the spinners. Do not add them to the _elements list.
  ScreenText _checkDisplay = ScreenText(cols[1], rows[0], textSize, "OK");
  ScreenText _checkGnss = ScreenText(cols[1], rows[1], textSize, "OK");
  ScreenText _checkSD = ScreenText(cols[1], rows[2], textSize, "OK");
  ScreenText _checkMap = ScreenText(cols[1], rows[3], textSize, "OK");
  ScreenText _checkTrack = ScreenText(cols[1], rows[4], textSize, "OK");

  ScreenDoubleCircleSpinner _spinnerDisplay = ScreenDoubleCircleSpinner(cols[1], rows[0], spinnerSize, &_checkDisplay);
  ScreenDoubleCircleSpinner _spinnerGnss = ScreenDoubleCircleSpinner(cols[1], rows[1], spinnerSize, &_checkGnss);
  ScreenDoubleCircleSpinner _spinnerSD = ScreenDoubleCircleSpinner(cols[1], rows[2], spinnerSize, &_checkSD);
  ScreenDoubleCircleSpinner _spinnerMap = ScreenDoubleCircleSpinner(cols[1], rows[3], spinnerSize, &_checkMap);
  ScreenDoubleCircleSpinner _spinnerTrack = ScreenDoubleCircleSpinner(cols[1], rows[4], spinnerSize, &_checkTrack);

public:
  int8_t displayOK, gnssOK, mapOK, sdOK, trackOK;

  BootScreen()
    : Screen(), displayOK(0), gnssOK(0), mapOK(0), sdOK(0), trackOK(0) {

    _elements.push_back(&_labelDisplay);
    _elements.push_back(&_labelGnss);
    _elements.push_back(&_labelMap);
    _elements.push_back(&_labelSD);
    _elements.push_back(&_labelTrack);
    _elements.push_back(&_SoftwareVersion);
    _elements.push_back(&_MapVersion);

    _elements.push_back(&_spinnerDisplay);
    _elements.push_back(&_spinnerGnss);
    _elements.push_back(&_spinnerMap);
    _elements.push_back(&_spinnerSD);
    _elements.push_back(&_spinnerTrack);
  };

  void reset(){

  };

  bool render(SharedSPIDisplay* display) {
    // Update elements based on status
    if (displayOK < 0) _checkDisplay.setText("ER");
    if (gnssOK < 0) _checkGnss.setText("ER");
    if (sdOK < 0) _checkSD.setText("ER");
    if (mapOK < 0) _checkMap.setText("ER");
    if (trackOK < 0) _checkTrack.setText("ER");

    _spinnerDisplay.setStatic(displayOK != 0);
    _spinnerDisplay.setCenterVisiblilty(displayOK != 0);
    _spinnerGnss.setStatic(gnssOK != 0);
    _spinnerGnss.setCenterVisiblilty(gnssOK != 0);
    _spinnerSD.setStatic(sdOK != 0);
    _spinnerSD.setCenterVisiblilty(sdOK != 0);
    _spinnerMap.setStatic(mapOK != 0);
    _spinnerMap.setCenterVisiblilty(mapOK != 0);
    _spinnerTrack.setStatic(trackOK != 0);
    _spinnerTrack.setCenterVisiblilty(trackOK != 0);

    // Render elements
    for (auto it : _elements) {
      it->step();
      it->draw(display);
    }
  //  display->clearDisplay();
    return true;
  };
};

extern BootScreen BOOTSCREEN;


class FixWaitingScreen : public Screen {

private:
  int textSize = 2;
  int spinnerSize = 15;
  int rows[1] = { 5 };
  int cols[2] = { 5, 120 };

  ScreenText _waitingText = ScreenText(cols[0], rows[0], textSize, "No Fix");
  ScreenText _dummyText = ScreenText(0, 0, textSize, " ");
  ScreenDoubleCircleSpinner _aquiringSpinner = ScreenDoubleCircleSpinner(cols[1], rows[0], spinnerSize, &_dummyText);
public:

  FixWaitingScreen()
    : Screen() {
    hasStatusBar = true;
    _elements.push_back(&_waitingText);
    _elements.push_back(&_aquiringSpinner);
  }

  void reset(){};

  bool render(SharedSPIDisplay* display) {
    // Render elements
    for (auto it : _elements) {
      it->step();
      it->draw(display);
    }

    return true;
  };

  void setCenterText(char* newText) {
    return;
  }
};

extern FixWaitingScreen FIXWAITINGSCREEN;


class MapScreen : public Screen {

public:
  MapScreen()
    : Screen() {
    hasStatusBar = true;
    hasMap = true;
  }
  void reset(){};
  bool render(SharedSPIDisplay* display) {
    return true;
  };
};
extern MapScreen MAPSCREEN;


// added additional Screen2 to switch to 5/12/24
class VarioScreen : public Screen {

public:
  VarioScreen()
    : Screen() {
    //    hasStatusBar = false;
    hasVarioScreen = true;
    hasMap = false;

  }

  void reset(){};
  
  bool render(SharedSPIDisplay* display) {
    return true;
  };
};

extern VarioScreen VARIOSCREEN;

// added additional Screen2 to switch to 5/12/24
class ScreenThree : public Screen {

public:
  ScreenThree()
    : Screen() {
    //    hasStatusBar = false;
    hasScreenThree = true;
    hasMap = true;
  }
  void reset(){};
  bool render(SharedSPIDisplay* display) {
    return true;
  };
};

extern ScreenThree SCREENTHREE;
#endif