#ifndef MAP_H
#define	MAP_H

#include <model/map/MapData.h>
#include <controller/MapController.h>
#include <model/Personaje.h>
#include <view/PersonajeVista.h>
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
    void ClickOn(int x, int y, int button);
    void Update();
    void SetUpPersonajes();
    Vector2* GetCamera();
private:
    MapData* data;
    string texturesPaths[ MapData::AMOUNT_TYPES ];
    SDL_Surface* tilesTextures[ MapData::AMOUNT_TYPES ];
    string backgroundPath;

    MapController* mapController;
    int cameraX, cameraY;

    SDL_Rect GetTilePos(int row, int col);
    void DefineTexturePaths();
    void GraphicalSetup();
    void CameraUpdate();
};

#endif	/* MAP_H */

