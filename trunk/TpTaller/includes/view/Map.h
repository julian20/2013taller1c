#ifndef MAP_H
#define	MAP_H

#include <model/map/MapData.h>
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
    void DefineTexturePaths();
    void GraphicalSetup();
};

#endif	/* MAP_H */

