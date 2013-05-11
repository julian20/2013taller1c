#include <view/MapView.h>

#define TilesScale     		1
#define TilesVisibleMargin	10

using namespace std;

MapView::MapView(MapData* inputData, SDL_Surface* inputScreen,
		EntityViewMap* map) {
	screen = inputScreen;
	data = inputData;
	viewMap = map;
	if (inputScreen != NULL) {
	Position* cameraPos = new Position(screen->w / 2, screen->h / 2);
	camera = new MapCameraView(cameraPos, screen, data->getNRows(),
			data->getNCols());
	}
	entitiesView = NULL;
	player = NULL;

	textureHolder = NULL;

}

MapView::~MapView() {
}

void MapView::setUpPersonajes() {
	// Setea la posicion por unica vez de los personajes en el mapa.
	SDL_Rect posTile;

	for (int col = 0; col < data->getNCols(); col++) {
		for (int row = 0; row < data->getNRows(); row++) {

			Player* personaje = data->getPersonaje(row, col);
			if (personaje != NULL) {

				posTile = Tile::computePositionTile(row, col);
				Coordinates* coords = Tile::getTileCoordinates(posTile.x,
						posTile.y);
				personaje->setPos((float) posTile.x, (float) posTile.y);
			}
		}
	}
}

//TODO - este metodo iria en cfg reader
void MapView::setUpEntity(EntityView* entityView, Coordinates coor) {
	this->viewMap->positionEntityView(entityView, coor);
}

void MapView::assignPersonaje(Player* _personaje) {
	player = _personaje;
	Position* pos = new Position(player->getCurrentPos()->getX(),
			player->getCurrentPos()->getY());
	camera->setPosition(pos);
}

MapCameraView* MapView::getCamera() {
	return camera;
}

SDL_Surface* MapView::getDrawingSurface() {
	return screen;
}

map<string, int> MapView::getVisibleTilesLimit(Position* cam) {
// TODO: Esto en vez de devolver un diccionario capas q estaria bueno q devuelva una clase, q dicen?

// Obtiene las casillas de los extremos de la pantalla
	Coordinates *supLeft = Tile::getTileCoordinates(-cam->getX(), -cam->getY());
	Coordinates *supRight = Tile::getTileCoordinates(screen->w - cam->getX(),
			-cam->getY());
	Coordinates *infLeft = Tile::getTileCoordinates(-cam->getX(),
			screen->h - cam->getY());
	Coordinates *infRight = Tile::getTileCoordinates(screen->w - cam->getX(),
			screen->h - cam->getY());

	if (supLeft->getCol() < 0)
		supLeft->setCol(0);
	if (supRight->getRow() < 0)
		supRight->setRow(0);
	if (infLeft->getRow() > data->getNRows())
		infLeft->setRow(data->getNRows());
	if (infRight->getCol() > data->getNCols())
		infRight->setCol(data->getNCols());

	// Se agregan los tiles de margen
	if (supLeft->getCol() <= TilesVisibleMargin)
		supLeft->setCol(0);
	else
		supLeft->setCol(supLeft->getCol() - TilesVisibleMargin);

	if (supRight->getRow() <= TilesVisibleMargin)
		supRight->setRow(0);
	else
		supRight->setRow(supRight->getRow() - TilesVisibleMargin);

	if (infRight->getCol() >= data->getNCols() - TilesVisibleMargin)
		infRight->setCol(data->getNCols());
	else
		infRight->setCol(infRight->getCol() + TilesVisibleMargin);

	if (infLeft->getRow() >= data->getNRows() - TilesVisibleMargin)
		infLeft->setRow(data->getNRows());
	else
		infLeft->setRow(infLeft->getRow() + TilesVisibleMargin);

	std::map<std::string, int> mapVisibleLimits;
	mapVisibleLimits["StartCol"] = supLeft->getCol();
	mapVisibleLimits["EndCol"] = infRight->getCol();
	mapVisibleLimits["StartRow"] = supRight->getRow();
	mapVisibleLimits["EndRow"] = infLeft->getRow();

	delete supLeft;
	delete supRight;
	delete infLeft;
	delete infRight;
	return mapVisibleLimits;
}

void MapView::draw(Position* cam) {
	data->updateVisibleTiles();
	SDL_Rect posTile, posFog;
	map<string, int> mapVisibleLimits = getVisibleTilesLimit(cam);

	for (int col = mapVisibleLimits["StartCol"];
			col < mapVisibleLimits["EndCol"]; col++) {

		for (int row = mapVisibleLimits["StartRow"];
				row < mapVisibleLimits["EndRow"]; row++) {

			TileData* tileData = data->getTileData(row, col);

			if (tileData->getWasVisible()) {
				posFog = posTile = Tile::computePositionTile(row, col - 1);
				Position* cameraPos = this->camera->getPosition();
				posFog.x = posTile.x = posTile.x + cameraPos->getX();
				posFog.y = posTile.y = posTile.y + cameraPos->getY();
				delete cameraPos;

				std::string textureId = data->getTileType(row, col);
				SDL_Surface* textureImage = textureHolder->getTexture(
						textureId);
				SDL_Surface* fogImage = textureHolder->getFogTexture(textureId);
				SDL_BlitSurface(textureImage, NULL, screen, &posTile);
				if (!(tileData->getIsVisible()))
					SDL_BlitSurface(fogImage, NULL, screen, &posFog);
			}
		}

	}

	viewMap->drawViews(screen, cam, mapVisibleLimits);

}

MapData* MapView::getMapData(){
	return data;
}
TextureHolder* MapView::getTextureHolder() {
	return this->textureHolder;
}

void MapView::setTextureHolder(TextureHolder* textureHolder) {
	this->textureHolder = textureHolder;
}

void MapView::addNewPlayerView(PlayerView* view, Coordinates initCoord) {
	viewMap->positionEntityView(view, initCoord);
}
