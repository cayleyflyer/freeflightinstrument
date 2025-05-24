#ifndef _GLOBALCONFIG_H
#define _GLOBALCONFIG_H

//SD Card pins using HSPI 
#define SPI_FREQ 8000000

#define SDCARD_CS  21
#define SD_MOSI 17
#define SD_MISO 16
#define SD_SCK 14
#define SD_CS 21


/**
 * 
 *      GNSS module
 * 
**/
#define GNSS_BAUD   9600
#define GNSS_ENABLE_MULTI_CONSTELLATION false
#define GNSS_ENABLE_LOW_POWER false
#define GNSS_ENABLE_HIGH_REFRESH false
#define GNSS_MIN_UPDATE_TIME_MS 1000
#define KMPH_MPS 0.277778

//vsi 
#define VSI_Sample_Interval  500  //in ms

/**
 * 
 *      Display module
 * 
**/
// any pins can be used
#define TFT_DC 41
#define TFT_CS 42
#define TFT_CLK 40
#define TFT_MISO 46
#define TFT_MOSI 45
#define TFT_RST 39


#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320
#define POSITION_MARKER_SIZE 8
// Width and height of sprite
//#define WIDTH  240
//#define HEIGHT 240

/**
 * 
 *      Geographic utilities
 * 
**/

// Radius of the earth.
#define R_EARTH 6378137.0

/**
 * 
 *      Map Rendering
 * 
**/

// Default map zoom level. 1.0 means that exactly one tile fits on the display.
#define DEFAULT_ZOOM_LEVEL 1

// Number of tiles per dimension to keep in memory. Must be an uneven number " 1
#define RENDER_TILES_PER_DIM 13

// If the map should be rotated to be alinged with heading.
#define RENDER_HEADING true

// Minimum time between two consecutive tile data updates in milliseconds.
#define TILE_UPDATE_DEBOUNCE_MS 5000
/**
 * 
 *      Settings for UI
 * 
**/
#define TARGET_FPS 60

#define BOOTSCREEN_SPINNER_SIZE 10
#define BOOTSCREEN_TEXT_SIZE 2

// Statusbar
// Number of characters per stat in the status bar, including terminator
#define N_CHAR_PER_STAT 6
//#define STAT1 0


/**
 * 
 * Derived globals (DO NOT CHANGE)
 * 
**/

#define TARGET_FRAME_TIME_MS 1000/TARGET_FPS
//#define DISPLAY_MAX_DIM sqrt(400*400/2) 
#define DISPLAY_MAX_DIM sqrt(DISPLAY_WIDTH*DISPLAY_WIDTH/2) 
#define DISPLAY_WIDTH_HALF 160 //DISPLAY_WIDTH/2
//#define DISPLAY_HEIGHT_HALF DISPLAY_HEIGHT/2
#define DISPLAY_HEIGHT_HALF 160 //DISPLAY_HEIGHT/2
#define RENDER_TILES_PER_DIM_HALF RENDER_TILES_PER_DIM / 2
#define N_RENDER_TILES RENDER_TILES_PER_DIM*RENDER_TILES_PER_DIM

#endif