#include <view/MapView.h>

#define TilesScale     		1

using namespace std;

MapView::MapView(MapData* inputData, SDL_Surface* inputScreen, EntityViewMap* map) {
	screen = inputScreen;
	data = inputData;
	viewMap = map;
	Position* cameraPos = new Position(screen->w / 2, screen->h / 2);
	camera = new MapCameraView( cameraPos, screen , data->GetNRows() , data->GetNCols());
	entitiesView = NULL;
	personaje = NULL;

	textureHolder = NULL;
	timer.start();
}

MapView::~MapView() {
}

void MapView::SetUpPersonajes() {
	// Setea la posicion por unica vez de los personajes (y en un futuro posiblemente
	// lo representable) en el mapa.
	SDL_Rect posTile;

	for (int col = 0; col < data->GetNCols(); col++) {
		for (int row = 0; row < data->GetNRows(); row++) {

			Player* personaje = data->GetPersonaje(row, col);
			if (personaje != NULL) {

				posTile = Tile::computePosition(row, col, true);
				personaje->setPos((float) posTile.x, (float) posTile.y);
			}
		}
	}
}

//TODO - este metodo iria en cfg reader
void MapView::SetUpEntity(EntityView* entityView,Coordinates coor)
{
	this->viewMap->positionEntityView(entityView,coor);
}

void MapView::Update() {

}

MapCameraView* MapView::getCamera(){
	return camera;
}

SDL_Surface* MapView::getDrawingSurface() {
	return screen;
}

void MapView::draw(Position* cam) {

	//Personaje* personajes = NULL;
	SDL_Rect posTile;


	/*
	 * Dibujamos los tiles
	 * TODO: agregar un metodo getEntities(row,col) a entityviewmap para dibujar toodo en el mismo for!
	 */
	for (int col = 0; col < data->GetNCols(); col++) {

		for (int row = 0; row < data->GetNRows(); row++) {

			posTile = Tile::computePosition(row, col);
			Position* cameraPos = this->camera->getPosition();
			posTile.x = cameraPos->getX() + posTile.x;
			posTile.y = cameraPos->getY() + posTile.y;
			delete cameraPos;

			std::string textureId = data->GetTileType(row, col);
			SDL_Surface* textureImage = getTextureHolder()->getTexture(textureId);

			SDL_BlitSurface(textureImage, NULL, screen, &posTile);

		}

	}

	viewMap->drawViews(screen,cam);
	if (timer.getTimeSinceLastAnimation()>= 2*timer.getDelay()) timer.start();
	// Luego se blitean todos los personajes despues de haber bliteado el piso
	// para que el piso no tape a los flacos.
	/*  if (personajes != NULL){
	 PersonajeVista* personajeVista =
	 new PersonajeVista(personajes, "resources/foo.png", screen);

	 personajeVista->UpdateCameraPos(cameraX, cameraY);
	 personajeVista->Mostrar();
	 }*/
}

TextureHolder* MapView::getTextureHolder() {
	return this->textureHolder;
}

void MapView::setTextureHolder(TextureHolder* textureHolder) {
	this->textureHolder = textureHolder;
}

