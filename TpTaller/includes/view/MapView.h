#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <model/map/MapData.h>
#include <model/entities/personaje/Personaje.h>
#include <view/entities/PersonajeVista.h>
#include <model/map/TextureHolder.h>

#include <string>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

using namespace std;

typedef enum {
	MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT
} CameraMove;

class MapView {
public:
	MapView(MapData* data, SDL_Surface* screen);
	virtual ~MapView();

	SDL_Surface* getDrawingSurface();

	void Draw();
	void movePlayer(int x, int y);
	void Update();
	void SetUpPersonajes();
	void AssignPersonaje(Personaje* _personaje);
	Position* GetCamera();

	void moveCamera(CameraMove move);

	TextureHolder* getTextureHolder();
	void setTextureHolder(TextureHolder* textureHolder);
private:
	TextureHolder* textureHolder;
	MapData* data;
	SDL_Surface* screen;

	string texturesPaths[MapData::AMOUNT_TYPES];
	SDL_Surface* tilesTextures[MapData::AMOUNT_TYPES];
	string backgroundPath;
	Personaje* personaje;
	Position* lastTilePosY;
	Position* lastTilePosXIzq;
	Position* lastTilePosXDer;

	Position* camera;

	void DefineTexturePaths();
	void checkBoundaries();
	void GraphicalSetup();
	void CameraUpdate();
	SDL_Rect GetSquaredMapTilePos(int row, int col);
	SDL_Rect GetDiamondShapeMapTilePos(int row, int col);
	void setLimitTiles();
};

#endif  /* MAP_H */
