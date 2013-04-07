#include <view/MapView.h>

#define TilesScale     		1
#define CameraSpeed         15         // px

using namespace std;
MapView::MapView(MapData* inputData, SDL_Surface* inputScreen, EntityViewMap* map) {
	screen = inputScreen;
	data = inputData;
	viewMap = map;
	Position* cameraPos = new Position(screen->w / 2, screen->h / 2);
	camera = new MapCameraView( cameraPos, screen , data->GetNRows() , data->GetNCols());

	textureHolder = NULL;
}

MapView::~MapView() {
}

void MapView::SetUpPersonajes() {
	// Setea la posicion por unica vez de los personajes (y en un futuro posiblemente
	// lo representable) en el mapa.
	SDL_Rect posTile;

	for (int col = 0; col < data->GetNCols(); col++) {
		for (int row = 0; row < data->GetNRows(); row++) {

			Personaje* personaje = data->GetPersonaje(row, col);
			if (personaje != NULL) {

				posTile = Tile::computePosition(row, col, true);
				personaje->setPos((float) posTile.x, (float) posTile.y);
			}
		}
	}
}

void MapView::movePlayer(int x, int y) {
	// Selecciona la casilla mas o menos bien, idealizandola como un cuadrado.
	// TODO: Que seleccione la casilla bien!

	Position* cameraPos = this->camera->getPosition();

	Coordinates* coor = Tile::getTileCoordinates(	x - cameraPos->getX(),
													y - cameraPos->getY());

	if( !(coor->getCol() < 0 || coor->getRow() < 0) &&
		!(coor->getCol() > data->GetNCols() - 1 || coor->getRow() > data->GetNRows() - 1)) {
		SDL_Rect firstTile = Tile::computePosition(0, 0);
		firstTile.x = cameraPos->getX() + firstTile.x;
		firstTile.y = cameraPos->getY() + firstTile.y;

		// Squared Map
		//int row = (y - firstTile.y) * 2 / firstTile.h;
		//int col = (x - firstTile.x) / firstTile.w;

		if (personaje != NULL) {
			// TODO: esto no pierde memoria a lo loco?
			// Tile* toTile = new Tile(new Coordinates(row, col, true));
			// data->movePersonaje(personaje, toTile);
			// printf("%i--%i\n",firstTile.x , firstTile.y );
			personaje->MoveTo(x - cameraPos->getX(), y - cameraPos->getY());

	}

	//printf("row: %d, col: %d\n", row, col);
	}delete coor;
}

void MapView::AssignPersonaje(Personaje* _personaje) {
	personaje = _personaje;
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

			std::string textureId = data->GetTileType(row, col);
			SDL_Surface* textureImage = getTextureHolder()->getTexture(
					textureId);

			SDL_BlitSurface(textureImage, NULL, screen, &posTile);

		}

	}

	for (int col = 0; col < data->GetNCols(); col++) {

			for (int row = 0; row < data->GetNRows(); row++) {


				list<EntityView*>aList = viewMap->getListAtRowAndCol(row,col);
				if(!aList.empty()){
					list<EntityView*>::iterator it;
					for (it = aList.begin(); it != aList.end(); ++it) {
						EntityView* view = *it;
						if (!view) continue;
						view->draw(screen,cam);
					}
				}
			}

		}


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

