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

    void DefineTexturePaths();
    void GraphicalSetup();
    void CameraUpdate();
    SDL_Rect GetSquaredMapTilePos(int row, int col);
    SDL_Rect GetDiamondShapeMapTilePos(int row, int col);
};

#endif	/* MAP_H */

