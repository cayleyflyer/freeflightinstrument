#ifndef _SHAREDSPISDCARD_H
#define _SHAREDSPISDCARD_H

#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include "simpletile.h"
#include "gpxtrack.h"
#include "globalconfig.h"

/*

    Wrapper class for the SD-Card reader with application-specific functionality.

*/
class SharedSPISDCard  {

    enum FileType {Map, GPXTrackIn, GPXTrackOut, None};

private:
    uint8_t _PIN_CS;  //convention for private variables is to start them with an underscore
    File file;
    FileType _currFileType;
    char* _mapPath;
    char* _gpxTrackInPath;
    char* _gpxTrackOutPath;
    
    bool openFile(const char* path);
    bool openFile(FileType fileType);
    void closeFile();

public:

    uint64_t read_bytes;

    SharedSPISDCard(uint8_t PIN_CS);  //CONSTRUCTOR
    void enableCS();                  //functions
    void disableCS();

    void initialize();

    void setMapPath(const char* mapPath);
    void setGPXTrackInPath(const char* gpxTrackInPath);
    void setGPXTrackOutPath(const char* gpxTrackOutPath);

    bool exists(const char* path);

    // Map reading
    bool readTile(SimpleTile::Header& header, int16_t* tile_node_buffer, uint64_t& tileSize, int tile_id); //int32_t
    bool readHeader(SimpleTile::Header& header);
    
    // Input GPX reading
    bool readGPX(SimpleTile::Header& header, GPXTrack& track);


};

#endif