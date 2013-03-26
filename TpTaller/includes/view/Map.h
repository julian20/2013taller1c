#ifndef MAP_H
#define	MAP_H

#include <model/map/MapData.h>
#include <controller/MapController.h>
#include <string>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

using namespace std;

class Map {
public:
    Map(MapData* data);
    virtual ~Map();
    
    void Draw(SDL_Surface* pantalla);
private:
    MapData* data;
    string texturesPaths[ MapData::AMOUNT_TYPES ];
    SDL_Surface* tilesTextures[ MapData::AMOUNT_TYPES ];
    string backgroundPath;

    MapController* mapController;
    int cameraX, cameraY;

    void DefineTexturePaths();
    void GraphicalSetup();
    void CameraUpdate();
};

#endif	/* MAP_H */

