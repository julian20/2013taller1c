#include <view/MapView.h>

#define MapMargin               40              // px
#define TilesScale      1
#define CameraSpeed             15              // px

MapView::MapView(MapData* _data, SDL_Surface* screen) {
	this->screen = screen;
	data = _data;
	camera = new Position(data->GetNCols() / 2, data->GetNRows() / 2);

	SDL_Rect posTile = Tile::computePosition(data->GetNRows(),
			data->GetNCols());
	lastTilePos = new Position(posTile.x, posTile.y);
	textureHolder = NULL;

	checkBoundaries();
}

Position* MapView::GetCamera() {
	return new Position(camera->getX(), camera->getY());
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
				posTile = Tile::computePosition(row, col);
				posTile.x = camera->getX() + posTile.x;
				posTile.y = camera->getY() + posTile.y;

				personaje->setPos((float) posTile.x, (float) posTile.y);
			}
		}
	}
}

void MapView::ClickOn(int x, int y, int button) {
	// Selecciona la casilla mas o menos bien, idealizandola como un cuadrado.
	SDL_Rect firstTile = Tile::computePosition(0, 0);
	firstTile.x = camera->getX() + firstTile.x;
	firstTile.y = camera->getY() + firstTile.y;

	int row = (y - firstTile.y) * 2 / firstTile.h;
	int col = (x - firstTile.x) / firstTile.w;

	printf("A\n");
	if (personaje != NULL) {
		Tile* toTile = new Tile(new Coordinates(row, col));
		data->movePersonaje(personaje, toTile);
		printf("B\n");
		//personaje->MoveTo(x - camera->getX(), y - camera->getY());
	}

	//printf("row: %d, col: %d\n", row, col);
}

void MapView::AssignPersonaje(Personaje* _personaje) {
	personaje = _personaje;
}

void MapView::Update() {

}

SDL_Surface* MapView::getDrawingSurface() {
	return screen;
}

void MapView::checkBoundaries() {
	if (camera->getX() > -MapMargin)
		camera->setX(-MapMargin);
	if (camera->getY() > -MapMargin)
		camera->setY(-MapMargin);
	if (camera->getX() < -lastTilePos->getX() + screen->w)
		camera->setX(-lastTilePos->getX() + screen->w);
	if (camera->getY() < -lastTilePos->getY() + screen->h)
		camera->setY(-lastTilePos->getY() + screen->h);
}

void MapView::moveCamera(CameraMove move) {

	switch (move) {
	case MOVE_UP:
		camera->setY(camera->getY() + CameraSpeed);
		break;
	case MOVE_DOWN:
		camera->setY(camera->getY() - CameraSpeed);
		break;
	case MOVE_LEFT:
		camera->setX(camera->getX() + CameraSpeed);
		break;
	case MOVE_RIGHT:
		camera->setX(camera->getX() - CameraSpeed);
		break;
	}

	checkBoundaries();

}

void MapView::Draw() {

	//Personaje* personajes = NULL;
	SDL_Rect posTile;

	for (int col = 0; col < data->GetNCols(); col++) {

		for (int row = 0; row < data->GetNRows(); row++) {

			posTile = Tile::computePosition(row, col);
			posTile.x = camera->getX() + posTile.x;
			posTile.y = camera->getY() + posTile.y;

			std::string textureId = data->GetTileType(row, col);
			SDL_Surface* textureImage = getTextureHolder()->getTexture(
					textureId);

			SDL_BlitSurface(textureImage, NULL, screen, &posTile);

			TileData* tileData = data->GetTileData(row, col);

			Entity* entity = tileData->getNextEntity();
			while (entity != NULL) {
				/*Lucas: TODO!
				 entity->draw();
				 */
				entity = tileData->getNextEntity();
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
