
// include gaurds which prevent multiple includes of teh same file
#ifndef _TILEBLOCKRENDERER_H
#define _TILEBLOCKRENDERER_H

#include "sharedspidisplay.h"
#include "sharedspisdcard.h"
#include "geoposition.h"
#include "geopositionprovider.h"
#include "zoomclass.h"

class TileBlockRenderer {    //class definition begins with class

private: //access specifier private only be used insie the class

    bool _hasPositionProvider, _hasHeader, _hasTrackIn;
    int _heading;
    float* _rotMtxBuf;
    uint64_t* _renderTileIds;
    int16_t* _renderTileData;  // changed from int16_t
    uint64_t* _renderTileSizes;
    uint64_t _perTileBufferSize;    
    uint64_t _centerTileId, _prevCenterTileId;
    long long _prevCenterChangeTime, _prevTileUpdateTime;
    int* _offsetDirectionMap;
    
    
    SimpleTile::Header* _header;
    SharedSPISDCard* _sd;
    SharedSPIDisplay* _display;
    GPXTrack* _track;
    GeoPositionProvider* _positionProvider;
   

public:    //access specifier public can be used inside the arduino sketch
 ZoomClass _changezoom;
float _zoomLevel, _zoomScale;

    TileBlockRenderer(); // constructor decleration same name as class needs to public . will be defined in the cpp.
    void initialize(SimpleTile::Header* mapHeader, SharedSPISDCard* sd, SharedSPIDisplay* display); //added zoomscale perameter
    void setPositionProvider(GeoPositionProvider* newPositionProvider);
    void setGPXTrackIn(GPXTrack* track);
    bool step(bool holdOn=false);
    // below three methods were in 'private:' so have moved then here because I want access from loop() to change zoomlevel on the fly
    bool isOnDisplay(int disp_LL_x, int disp_LL_y, int disp_UR_x, int disp_UR_y, int16_t x0, int16_t y0);
    void updateTileBuffer(LocalGeoPosition& center);
    void render(LocalGeoPosition& center);
    void renderGPX(LocalGeoPosition& center);
    void setZoom(float newZoomLevel);
    float getZoom();
};

//extern TileBlockRenderer myObject;
#endif