#ifndef _GPXTRACK_H
#define _GPXTRACK_H

#include "Arduino.h"


/*

    GPX-Track

*/
struct GPXTrack {
public:
    int16_t* xList;
    int16_t* yList;
    uint64_t* tileIdList;
    char* wplist; //char* wplist works,  char*wplist[8] and char wplist does not work
    char* wplist1; //char* wplist works,  char*wplist[8] and char wplist does not work
    char* wplist2; //char* wplist works,  char*wplist[8] and char wplist does not work
    char* wplist3; //char* wplist works,  char*wplist[8] and char wplist does not work
    char* wplist4; //char* wplist works,  char*wplist[8] and char wplist does not work
    char* wplist5; //char* wplist works,  char*wplist[8] and char wplist does not work
    char* wplist6; //char* wplist works,  char*wplist[8] and char wplist does not work
    char* wplist7; //char* wplist works,  char*wplist[8] and char wplist does not work
    char* wplist8; //char* wplist works,  char*wplist[8] and char wplist does not work
    uint32_t numNodes;
    uint32_t nearestNodeId;
};

#endif