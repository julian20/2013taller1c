#ifndef MAPVIEW_H
#define	MAPVIEW_H

#include <model/map/MapData.h>
#include <controller/MapController.h>
#include <model/entities/personaje/Personaje.h>
#include <view/entities/PersonajeVista.h>
#include <string>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

using namespace std;

class MapView {
public:
	MapView(MapData* data);
	virtual ~MapView();

	void Draw(SDL_Surface* pantalla);
	void ClickOn(int x, int y, int button);
	void Update();
	void SetUpPersonajes();
    void AssignPersonaje(Personaje* _personaje);
	Vector2* GetCamera();
private:
	MapData* data;
	string texturesPaths[MapData::AMOUNT_TYPES];
	SDL_Surface* tilesTextures[MapData::AMOUNT_TYPES];
	string backgroundPath;
    Personaje* personaje;

	MapController* mapController;
	int cameraX, cameraY;

	void DefineTexturePaths();
	void GraphicalSetup();
	void CameraUpdate();
	SDL_Rect GetSquaredMapTilePos(int row, int col);
	SDL_Rect GetDiamondShapeMapTilePos(int row, int col);
};

#endif	/* MAP_H */

