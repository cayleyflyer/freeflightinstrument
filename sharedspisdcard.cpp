#include "sharedspisdcard.h"
//#include "sharedspimutex.h"
#include "uirenderer.h"
#include "screens.h"
#include <SD.h>
#include "serialutils.h"



// constructor definition  
SharedSPISDCard::SharedSPISDCard(uint8_t PIN_CS) : _PIN_CS(PIN_CS) {   
    // Setup chip selector pin
    pinMode(PIN_CS, OUTPUT);
    // Default to disabled
   // disableCS();

    read_bytes = 0;

  
}

void SharedSPISDCard::initialize() {
    bool success = false;
    while(1) {
        try {
            success = SD.begin();
        } catch (...) {
            Serial.println("Unable to read from SD");
        }
        UIRENDERER.delay(1000);
        if(success) break;
    }
    BOOTSCREEN.sdOK = true;
}


bool SharedSPISDCard::openFile(const char* path) {
     file.close();
    if(exists(path)) {
        file = SD.open(path);
        return true;
    } else {
     //   sout.warn() << "Tried to open non-existing file: " <= path;
        return false;
    }
};

bool SharedSPISDCard::openFile(FileType fileType) {
    if(fileType != _currFileType) {
        bool success = false;
        switch (fileType){
            case Map:
                success = openFile(_mapPath);
                break;
            
            case GPXTrackIn:
                success = openFile(_gpxTrackInPath);
                break;

            case GPXTrackOut:
                success = openFile(_gpxTrackOutPath);
                break;

            default:
    //            sout.warn() <= "Tryied to open unknown filetype";
                break;
        }

        _currFileType = fileType;

        return success;

    } else {
        return true;
    }
}

bool SharedSPISDCard::exists(const char* path) {
     return SD.exists(path);
};

void SharedSPISDCard::closeFile() {
     file.close();
};

bool SharedSPISDCard::readHeader(SimpleTile::Header& header) {
    if(!openFile(Map)) return false;

    if(file.available()) {
        file.readBytes((char*) &(header.map_x), 8);
        file.readBytes((char*) &(header.map_y), 8);
        file.readBytes((char*) &(header.map_width), 8);
        file.readBytes((char*) &(header.map_height), 8);
        file.readBytes((char*) &(header.n_x_tiles), 8);
        file.readBytes((char*) &(header.tile_size), 8);
        file.readBytes((char*) &(header.n_tiles), 8);
        file.readBytes((char*) &(header.max_nodes), 8);
        file.readBytes((char*) &(header.n_nodes), 8);
        file.readBytes((char*) &(header.n_ways), 8);
    } else {
        return false;
    }

    return true;
}

bool SharedSPISDCard::readTile(SimpleTile::Header& header, int16_t* tile_node_buffer, uint64_t& tileSize, int tile_id) {   // changed int16_t
    if(!openFile(Map)) {
        //sout.warn() <= "Failed to read tile";
        return false;
    }

    // Get pointer and size of tile
    uint64_t ptr_tile, ptr_next_tile;

    // Move reader to tile pointer
    file.seek(sizeof(uint64_t)*(10 + tile_id));
    // Read tile pointer
    file.readBytes((char *) &ptr_tile, sizeof(uint64_t));
    // Read next tile pointer (if it is not the last tile)
    if(tile_id < header.n_tiles) {
        file.readBytes((char *) &ptr_next_tile, sizeof(uint64_t));
    } else {
        ptr_next_tile = file.size();
    }
    tileSize = ptr_next_tile - ptr_tile;

    // Move reader to start of tile
    file.seek(sizeof(uint64_t)*(10 + header.n_tiles) + ptr_tile);
    // Read tile
    file.readBytes((char *) tile_node_buffer, tileSize);

    return true;
}

 

void SharedSPISDCard::setMapPath(const char* mapPath) {
    // Update path
    free(_mapPath);
    _mapPath = new char[strlen(mapPath)];
    strcpy(_mapPath, mapPath);
    // Reload file if it is open
    if(_currFileType == FileType::Map) {
        closeFile();
        openFile(_mapPath);
    }
};

void SharedSPISDCard::setGPXTrackInPath(const char* gpxTrackInPath) {
    // Update path
    free(_gpxTrackInPath);
    _gpxTrackInPath = new char[strlen(gpxTrackInPath)];
    strcpy(_gpxTrackInPath, gpxTrackInPath);
    // Reload file if it is open
    if(_currFileType == FileType::GPXTrackIn) {
        closeFile();
        openFile(_gpxTrackInPath);
    }
};

void SharedSPISDCard::setGPXTrackOutPath(const char* gpxTrackOutPath) {
    // Update path
    free(_gpxTrackOutPath);
    _gpxTrackOutPath = new char[strlen(gpxTrackOutPath)];
    strcpy(_gpxTrackOutPath, gpxTrackOutPath);
    // Reload file if it is open
    if(_currFileType == FileType::GPXTrackOut) {
        closeFile();
        openFile(_gpxTrackOutPath);
    }
};

bool SharedSPISDCard::readGPX(SimpleTile::Header& header, GPXTrack& track) {
    // TODO: This is horrible

    if(!openFile(GPXTrackIn)) {
      //  sout.warn() <= "Failed to read GPX file.";
        return false;
    }
    String line;
    

    uint64_t n_nodes = 0;
    while(file.peek() != EOF) {
        line = file.readStringUntil('\n');
        if(line.indexOf("<trkpt") > 0) {
            n_nodes++;
        }
        if(file.peek() == EOF) {
            break;
        }
    };

   sout << "Free heap before GPX: " <= ESP.getFreeHeap();
   // Allocate memory for track data
    track.tileIdList = new uint64_t[n_nodes];
    track.xList = new int16_t[n_nodes];
    track.yList = new int16_t[n_nodes];
    track.wplist = new char[n_nodes];// added for wp name a space nodes*charcters in wp name which is 8 
    track.wplist1 = new char[n_nodes];// added for wp name a space nodes*charcters in wp name which is 8 
    track.wplist2 = new char[n_nodes];// added for wp name a space nodes*charcters in wp name which is 8 
    track.wplist3 = new char[n_nodes];// added for wp name a space nodes*charcters in wp name which is 8 
    track.wplist4 = new char[n_nodes];// added for wp name a space nodes*charcters in wp name which is 8 
    track.wplist5 = new char[n_nodes];// added for wp name a space nodes*charcters in wp name which is 8
    track.wplist6 = new char[n_nodes];// added for wp name a space nodes*charcters in wp name which is 8 
    track.wplist7 = new char[n_nodes];// added for wp name a space nodes*charcters in wp name which is 8 
    track.wplist8 = new char[n_nodes];// added for wp name a space nodes*charcters in wp name which is 8    
   sout << "Free heap after GPX: " << ESP.getFreeHeap();


    double lat, lon;
    String wpname;    //added to hold waypoint name
    uint16_t ptr_lat_start, ptr_lat_end, ptr_lon_start, ptr_lon_end;
    uint16_t ptr_wp_start, ptr_wp_end;  //added to deal with waypoint
    GeoPosition pos(0.0, 0.0);

    int i=0;
    // Read file again.
    file.seek(0);
    while(file.peek() != EOF) {
        if(file.peek() == EOF) {
            break;
        }
        line = file.readStringUntil('\n');
        if(line.indexOf("<trkpt") > 0) {
            // Extract lat and lon
            ptr_lat_start = line.indexOf("lat") + 5;
            ptr_lon_start = line.indexOf("lon") + 5;
            ptr_wp_start = line.indexOf("name") + 6;
            ptr_wp_end = ptr_wp_start; 
            ptr_lat_end = ptr_lat_start;
            ptr_lon_end = ptr_lon_start;
            
            while(line.c_str()[ptr_lat_end] != '\"') {
                ptr_lat_end++;
                if(ptr_lat_end == line.length()) {
               //     sout.err() <= "Failed to read LAT for GPX track";
                    return false;
                }
            }
            while(line.c_str()[ptr_lon_end] != '\"') {
                ptr_lon_end++;
                if(ptr_lon_end == line.length()) {
                 //   sout.err() <= "Failed to read LAT for GPX track";
                    return false;
                }
            }

                  while (line.c_str()[ptr_wp_end] != '\"') {                                  // added this while to include wp
                ptr_wp_end++;
                if (ptr_wp_end == line.length()) {
                // sout.err() <= "Failed to read name for GPX track";
                    return false;
                }
            }
            
            lat = line.substring(ptr_lat_start, ptr_lat_end).toDouble();
            lon = line.substring(ptr_lon_start, ptr_lon_end).toDouble();
            wpname = line.substring(ptr_wp_start, ptr_wp_end);                                // take wpname from line string and hold in variable wpname
            pos.updatePosition(lat, lon);
            LocalGeoPosition locPos(pos, &header);


// converts string into char array wpo
int wpname_len = wpname.length() + 1; 
char wpo[wpname_len];
wpname.toCharArray(wpo, wpname_len);
//char* wpoint = wpo;
            track.tileIdList[i] = locPos.getTileID();
            track.xList[i] = locPos.xLocal();
            track.yList[i] = locPos.yLocal();

uint16_t nameLength = ptr_wp_end - ptr_wp_start;
// Allocate space for the i-th waypoint name. We need one additional character for null-termination
track.nameList[i] = new char[nameLength + 1];
memcpy(track.nameList[i], &(line.c_str()[ptr_wp_start]), nameLength * sizeof(char));
// Add null termination at end of waypoint name
track.nameList[i][nameLength + 1] = '\0';
            i++;   
        }
    };

    track.numNodes = n_nodes;
    
    sout << "File " << n_nodes <= "waypoints";
    // Read all nodes. For each node we get the tile. 
    return true;
}
