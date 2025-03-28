#ifndef _UIRENDERER_H
#define _UIRENDERER_H

#include "tileblockrenderer.h"
#include "geopositionprovider.h"
#include "sharedspidisplay.h"
#include "gnssmodule.h"
#include "screens.h"
#include "globalconfig.h"
#include "BMP280.h"
/*

    Class for rendering UI-elements, switching screens and ticking the main-loop

*/

class UIRenderer {

private:
    bool _hasGNSS, _hasBMP280, _hasPositionProvider, _hasHeader, _hasDisplay, _hasTrackIn;
    unsigned long _tLastRender, _tLoopRender;
    char* _textBuffer;
    Screen* _currentScreen;

    
    SharedSPIDisplay* _disp;
    GNSSModule* _gnss;
    BMP280* _bmp280;   //added to display altitude FL and QNH 
    SimpleTile::Header* _header;
    GeoPositionProvider* _posProvider;
    GPXTrack* _track;
    TileBlockRenderer _mapRenderer;

    void renderBootScreen();
    void renderInfoScreen();
    void renderStatusBar();
    void renderStat(char* textBuff);
    void renderMainScreen();                    // added for screen2 5/12/24
    void renderMainStat(char* textBuff);        // added for screen2 5/12/24
    void renderScreenThree();                   // added for screen3 5/12/24
    void renderScreenThreeStat(char* textBuff); // added for screen3 5/12/24
public:
    UIRenderer();


    bool initializeMap(SharedSPISDCard* sd);
    void setDisplay(SharedSPIDisplay* display);
    void setBMP280(BMP280* bmp280);
    void setGNSS(GNSSModule* gnss);
    void setPositionProvider(GeoPositionProvider* positionProvider);
    void setScreen(Screen* newScreen);
    void setHeader(SimpleTile::Header* header);
    void setGPXTrackIn(GPXTrack* track);
    bool step();
    void delay(uint64_t milliseconds);
};

extern UIRenderer UIRENDERER;

#endif