#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <model/map/MapData.h>
#include <model/entities/personaje/Personaje.h>
#include <view/entities/PersonajeVista.h>
#include <model/map/TextureHolder.h>
#include <view/EntityViewMap.h>
#include <view/MapCameraView.h>

#include <string>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

using namespace std;

class MapView {
public:
	MapView(MapData* data, SDL_Surface* screen, EntityViewMap* viewMap);
	virtual ~MapView();

	SDL_Surface* getDrawingSurface();

	void draw(Position* cam);
	void movePlayer(int x, int y);
	void Update();
	void SetUpPersonajes();
	void AssignPersonaje(Personaje* _personaje);
	MapCameraView* getCamera();

	TextureHolder* getTextureHolder();
	void setTextureHolder(TextureHolder* textureHolder);
private:
	TextureHolder* textureHolder;
	MapData* data;
	SDL_Surface* screen;
	EntityViewMap* viewMap;

	string texturesPaths[MapData::AMOUNT_TYPES];
	SDL_Surface* tilesTextures[MapData::AMOUNT_TYPES];
	string backgroundPath;
	Personaje* personaje;

	MapCameraView* camera;
};

#endif  /* MAP_H */
