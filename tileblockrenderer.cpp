#include "tileblockrenderer.h"
#include "mathutils.h"
#include "serialutils.h"
#include "globalconfig.h"
#include <Arduino.h>
#include "zoomclass.h"


//Constructors are methods that are automatically executed every time you create an object
//The purpose of a constructor is to construct an object and assign values to the object’s members.
//A constructor takes the same name as the class to which it belongs



TileBlockRenderer::TileBlockRenderer()  // constructor definition tells the arduino what to do when objects are created
  : _hasPositionProvider(false), _hasHeader(false), _hasTrackIn(false) {
  _renderTileIds = new uint64_t[N_RENDER_TILES]{ 0 };    // Store tile IDs currently in view
  _renderTileSizes = new uint64_t[N_RENDER_TILES]{ 0 };  // Array to store tile size for each tile
  _prevCenterTileId = 0;                                 // Initialize previous center tile ID
  _prevTileUpdateTime = -TILE_UPDATE_DEBOUNCE_MS;        // Initialize previous tile update time.
  _heading = 0;                                          // Initialise heading to zero
  _rotMtxBuf = new float[4];                             // map rotation matrix
                                                         //_zoomLevel = DETAULT_ZOOM_LEVEL;                       // set default _zoomLevel from (globalconfig.h)
  _zoomLevel = ZoomClass::NewZoom;                       // set default _zoomLevel from (zoomclass.cpp)
}

void TileBlockRenderer::initialize(SimpleTile::Header* mapHeader, SharedSPISDCard* sd, SharedSPIDisplay* display) {
  _header = mapHeader;
  _sd = sd;
  _display = display;
  _zoomScale = ((float)_zoomLevel) * ((float)DISPLAY_WIDTH_HALF / (float)(_header->tile_size));
  _perTileBufferSize = _header->max_nodes * 2;
  _renderTileData = new int16_t[_perTileBufferSize * N_RENDER_TILES]{ 0 };  //changed int32_t
  _hasHeader = true;
}
void TileBlockRenderer::setPositionProvider(GeoPositionProvider* newPositionProvider) {
  _positionProvider = newPositionProvider;
  _hasPositionProvider = true;
}

void TileBlockRenderer::setGPXTrackIn(GPXTrack* track) {
  _track = track;
  _hasTrackIn = true;
}

void TileBlockRenderer::updateTileBuffer(LocalGeoPosition& center) {
  uint8_t c_curr, r_curr;
  int8_t idx_curr, idx_old, c_old, r_old;
  // Get tile IDs around new center
  center.getTileBlock(_renderTileIds);


  // Find out in which direction we moved

  // Column shift (number of tiles to the right)
  int8_t cshift = (center.tileId() % _header->n_x_tiles) - (_prevCenterTileId % _header->n_x_tiles);
  // Row shift (number of tiles up)
  int8_t rshift = (center.tileId() / _header->n_x_tiles) - (_prevCenterTileId / _header->n_x_tiles);

  if (abs(cshift) >= RENDER_TILES_PER_DIM || abs(rshift) >= RENDER_TILES_PER_DIM) {
    // Complete buffer reload required

    // Overwrite complete buffer with zeros
    memset(_renderTileData, 0, _perTileBufferSize * N_RENDER_TILES);
    // Load all tiles
    for (uint8_t i = 0; i < N_RENDER_TILES; i++) {
      _sd->readTile(*_header, _renderTileData + _perTileBufferSize * i, _renderTileSizes[i], _renderTileIds[i]);
    }
  } else {
    // Some tiles can be reused. Find out which tiles we can keep and which we need to load from SD
    // Iterate over columns (c) and rows (r) of new block.
    for (uint8_t c = 0; c < RENDER_TILES_PER_DIM; c++) {
      for (uint8_t r = 0; r < RENDER_TILES_PER_DIM; r++) {
        // Reverse iteration order based on shift direction.
        // This avoids writing new data to a tile before copying its data.
        if (cshift < 0) {
          c_curr = RENDER_TILES_PER_DIM - c - 1;
        } else {
          c_curr = c;
        }
        if (rshift < 0) {
          r_curr = RENDER_TILES_PER_DIM - r - 1;
        } else {
          r_curr = r;
        }

        // Determine index of current tile
        idx_curr = c_curr + r_curr * RENDER_TILES_PER_DIM;

        // Determine column and row index of current tile in old block
        c_old = c_curr + cshift;
        r_old = r_curr + rshift;

        // Check if current tile was in old block
        if (c_old >= 0 && c_old < RENDER_TILES_PER_DIM && r_old >= 0 && r_old < RENDER_TILES_PER_DIM) {
          // Current tile was in old block. We can copy it from the buffer.
          // Determine index of tile in old block.
          idx_old = c_old + r_old * RENDER_TILES_PER_DIM;
          // Copy tile data from old block position to new block position
          memcpy(_renderTileData + _perTileBufferSize * idx_curr, _renderTileData + _perTileBufferSize * idx_old, (_perTileBufferSize) * sizeof(int16_t));
          // Copy tile size from old block position to new block position
          _renderTileSizes[idx_curr] = _renderTileSizes[idx_old];
        } else {
          // Current tile was not in old block. Need to read it from SD.
          // Overwrite buffer with zeros
          memset(_renderTileData + _perTileBufferSize * idx_curr, 0, _perTileBufferSize * sizeof(int16_t));
          // Read new tile.
          _sd->readTile(*_header, _renderTileData + _perTileBufferSize * idx_curr, _renderTileSizes[idx_curr], _renderTileIds[idx_curr]);
        }
      }
    }
  }
}

/*
    Render tiles from buffer to screen based on current location
*/
void TileBlockRenderer::render(LocalGeoPosition& center) {
  // Now we have the tile data in the buffer and the current position
  _zoomLevel = ZoomClass::NewZoom;  // sets zoomlevel to the static variable NewZoom delcared in zoomclass
  _zoomScale = ((float)_zoomLevel) * ((float)DISPLAY_WIDTH_HALF / (float)(_header->tile_size));

  int x0, y0, x1, y1;
  int curr_tile_offset_x, curr_tile_offset_y;
  int64_t curr_tile_LL_x, curr_tile_LL_y;
  int disp_LL_x, disp_LL_y, disp_UR_x, disp_UR_y;

  for (int tidx = 0; tidx < N_RENDER_TILES; tidx++) {

    // Get lower left corner of tile in global (x, y) coordinates
    LocalGeoPosition::getTileLL(_renderTileIds[tidx], _header, &curr_tile_LL_x, &curr_tile_LL_y);

    // Current position relative to current tile.
    curr_tile_offset_x = center.x() - curr_tile_LL_x;
    curr_tile_offset_y = center.y() - curr_tile_LL_y;

    // Get lower left and upper right corner of display relative to tile origin.
    // TODO: make this a bit nicer
    if (std::abs(_heading % 180) > 15) {

      disp_LL_x = curr_tile_offset_x - DISPLAY_MAX_DIM / (_zoomScale);  // DISPLAY_MAX_DIM sqrt(DISPLAY_WIDTH*DISPLAY_WIDTH/2)
      disp_UR_x = curr_tile_offset_x + DISPLAY_MAX_DIM / (_zoomScale);
      disp_LL_y = curr_tile_offset_y - DISPLAY_MAX_DIM / (_zoomScale);
      disp_UR_y = curr_tile_offset_y + DISPLAY_MAX_DIM / (_zoomScale);
    } else {
      disp_LL_x = curr_tile_offset_x - DISPLAY_WIDTH_HALF / (_zoomScale);  // DISPLAY_WIDTH_HALF/(_zoomScale)
      disp_UR_x = curr_tile_offset_x + DISPLAY_WIDTH_HALF / (_zoomScale);
      disp_LL_y = curr_tile_offset_y - DISPLAY_WIDTH_HALF / (_zoomScale);
      disp_UR_y = curr_tile_offset_y + DISPLAY_WIDTH_HALF / (_zoomScale);
    }

    uint64_t p = _perTileBufferSize * tidx;
    uint64_t pEnd = p + _renderTileSizes[tidx];

    while (p < pEnd) {

      // Check if current or next coordinate is a separator, skip otherwise.
      if ((!_renderTileData[p] && !_renderTileData[p + 1]) || (!_renderTileData[p + 2] && !_renderTileData[p + 3])) {
        p += 2;
        continue;
      }

      // Check if current or next coordinate is on display, skip otherwise.
      if (!isOnDisplay(disp_LL_x, disp_LL_y, disp_UR_x, disp_UR_y, _renderTileData[p], _renderTileData[p + 1])
          && !isOnDisplay(disp_LL_x, disp_LL_y, disp_UR_x, disp_UR_y, _renderTileData[p + 2], _renderTileData[p + 3])) {
        p += 2;
        continue;
      }

      // Calculate non-rotated position on screen.
      // I will use the screen in portrait mode so I see more km in front of me. I need the centre of the screen x=120 y= 240 !
      // if i change screen size in the globalconfig.h it does not work but does typing in manually, but rotation need doing (maths.h)

      x0 = DISPLAY_WIDTH_HALF + (_renderTileData[p] - curr_tile_offset_x) * _zoomScale;
      y0 = DISPLAY_WIDTH_HALF - (_renderTileData[p + 1] - curr_tile_offset_y) * _zoomScale;
      x1 = DISPLAY_WIDTH_HALF + (_renderTileData[p + 2] - curr_tile_offset_x) * _zoomScale;
      y1 = DISPLAY_WIDTH_HALF - (_renderTileData[p + 3] - curr_tile_offset_y) * _zoomScale;

      // Calculate rotated position on screen.
      if (_heading != 0) {
        rotatePointInplaceAroundScreenCenter(x0, y0, _rotMtxBuf);
        rotatePointInplaceAroundScreenCenter(x1, y1, _rotMtxBuf);
      }

      _display->drawLine(  // tftsprite
        x0,
        y0,
        x1,
        y1,
        2, TFT_BLACK);

      p += 2;
    }
  }
}





/*
    Render GPX track
*/
void TileBlockRenderer::renderGPX(LocalGeoPosition& center) {
  if (!_hasTrackIn) return;
  // TODO: this is horrible

  long t_start = millis();

  int x0, y0, x1, y1;

  int curr_tile_offset_x, curr_tile_offset_y, next_tile_offset_x, next_tile_offset_y;
  int64_t curr_tile_LL_x, curr_tile_LL_y, next_tile_LL_x, next_tile_LL_y;

  for (int tidx = 0; tidx < N_RENDER_TILES; tidx++) {
    for (uint32_t nidx = 0; nidx < (_track->numNodes - 1); nidx++) {
      if (_renderTileIds[tidx] == _track->tileIdList[nidx]) {
        // Found GPX waypoint on current tile.
        // Get lower left corner of tile in global (x, y) coordinates
        LocalGeoPosition::getTileLL(_track->tileIdList[nidx], _header, &curr_tile_LL_x, &curr_tile_LL_y);
        LocalGeoPosition::getTileLL(_track->tileIdList[nidx + 1], _header, &next_tile_LL_x, &next_tile_LL_y);

        // Current position relative to current tile.
        curr_tile_offset_x = center.x() - curr_tile_LL_x;
        curr_tile_offset_y = center.y() - curr_tile_LL_y;
        next_tile_offset_x = center.x() - next_tile_LL_x;
        next_tile_offset_y = center.y() - next_tile_LL_y;

        // Calculate non-rotated position on screen.
        x0 = DISPLAY_WIDTH_HALF + (_track->xList[nidx] - curr_tile_offset_x) * _zoomScale;
        y0 = DISPLAY_WIDTH_HALF - (_track->yList[nidx] - curr_tile_offset_y) * _zoomScale;
        x1 = DISPLAY_WIDTH_HALF + (_track->xList[nidx + 1] - next_tile_offset_x) * _zoomScale;
        y1 = DISPLAY_WIDTH_HALF - (_track->yList[nidx + 1] - next_tile_offset_y) * _zoomScale;

        char* wp = _track->nameList[nidx];


        // Calculate rotated position on screen.
        if (_heading != 0) {
          rotatePointInplaceAroundScreenCenter(x0, y0, _rotMtxBuf);
          rotatePointInplaceAroundScreenCenter(x1, y1, _rotMtxBuf);
        }

        _display->fillCircle(x0, y0, 5, TFT_GREEN);  // added these lines to draw circle waypoints on 5/9/24
        _display->setTextColor(TFT_BLACK, TFT_WHITE);
        _display->drawText(wp, x0 + 6, y0,GFXFF);  // draw the way point twice to display last wp in the list????
        //  _display->drawString(wp, x0 + 6, y0);           // draw the way point twicw to display last wp in the list????
      }
    }
  }

  long t_end = millis() - t_start;

  // sout << "Rendering tack took " << t_end <= "ms";
}





/*
    Determine if point is on display.
    All coordinates are given in current tile coordinates.
*/
bool TileBlockRenderer::isOnDisplay(int disp_LL_x, int disp_LL_y, int disp_UR_x, int disp_UR_y, int16_t x0, int16_t y0) {
  return (disp_LL_x <= x0) && (x0 <= disp_UR_x) && (disp_LL_y <= y0) && (y0 <= disp_UR_y);
}

bool TileBlockRenderer::step(bool holdOn) {

  _display->fillSprite(TFT_WHITE);  // fill the map sprite background with white colour to erase last map

  GeoPosition globcenter(0.0, 0.0);

  if (!_hasPositionProvider) {
    return false;
  }
  if (!(_positionProvider->isReady())) {
    return false;
  }
  if (!(_positionProvider->getPosition(globcenter))) {
    return false;
  }

  _heading = _positionProvider->getHeading();
  rad2rotMtx(_rotMtxBuf, (360 - _heading) * DEG_TO_RAD);

  // Get current position and tileID
  LocalGeoPosition center(globcenter, _header);
  _centerTileId = center.getTileID();
  // If we changed tile and are over the debounce time, we need to update the tilebuffer
  if (_centerTileId != _prevCenterTileId && (millis() - _prevTileUpdateTime) > TILE_UPDATE_DEBOUNCE_MS) {
    updateTileBuffer(center);
    _prevCenterTileId = _centerTileId;
    _prevTileUpdateTime = millis();
  }

  render(center);
  if (_hasTrackIn) renderGPX(center);
  _display->drawCenterMarker();  // draws center marker onto map sprite
  if (!holdOn) {
    _display->refresh();
  }

  return true;
}