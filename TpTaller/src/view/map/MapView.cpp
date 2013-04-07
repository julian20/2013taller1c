#include <view/MapView.h>

#define TilesScale     		1
#define CameraSpeed         15         // px

MapView::MapView(MapData* inputData, SDL_Surface* inputScreen, EntityViewMap* map) {
	screen = inputScreen;
	data = inputData;
	viewMap = map;
	camera = new Position(screen->w / 2, screen->h / 2);

	setLimitTiles();
	textureHolder = NULL;
}

void MapView::setLimitTiles() {
	SDL_Rect posTile = Tile::computePosition(data->GetNRows(), data->GetNCols());
	lastTilePosY = new Position(posTile.x, posTile.y);

	posTile = Tile::computePosition(0, data->GetNCols());
	lastTilePosXDer = new Position(posTile.x, posTile.y);

	posTile = Tile::computePosition(data->GetNRows(), 0);
	lastTilePosXIzq = new Position(posTile.x, posTile.y);
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

				posTile = Tile::computePosition(row, col, true);
				personaje->setPos((float) posTile.x, (float) posTile.y);
			}
		}
	}
}

void MapView::movePlayer(int x, int y) {
	// Selecciona la casilla mas o menos bien, idealizandola como un cuadrado.
	SDL_Rect firstTile = Tile::computePosition(0, 0);
	firstTile.x = camera->getX() + firstTile.x;
	firstTile.y = camera->getY() + firstTile.y;

	// Squared Map
	//int row = (y - firstTile.y) * 2 / firstTile.h;
	//int col = (x - firstTile.x) / firstTile.w;

	if (personaje != NULL) {
		/**
		 * TODO: esto no pierde memoria a lo loco?
		 */
		//Tile* toTile = new Tile(new Coordinates(row, col, true));
		//data->movePersonaje(personaje, toTile);
	//	printf("%i--%i\n",firstTile.x , firstTile.y );
		personaje->MoveTo(x - camera->getX(), y - camera->getY());
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

void MapView::checkBasicBoundaries() {
	// Basic boundaries checking
	if (camera->getX() > -lastTilePosXIzq->getX() + screen->w / 2)
		camera->setX(-lastTilePosXIzq->getX() + screen->w / 2);

	if (camera->getX() < -lastTilePosXDer->getX() + screen->w / 2)
		camera->setX(-lastTilePosXDer->getX() + screen->w / 2);

	if (camera->getY() > screen->h / 2)
		camera->setY(screen->h / 2);

	if (camera->getY() < -lastTilePosY->getY() + screen->h / 2)
		camera->setY(-lastTilePosY->getY() + screen->h / 2);
}

void MapView::checkAdvancedBoundaries() {
	// Advanced boundaries checking
	float XTopRightCondition = (float) ((screen->w / 2));
	float XBotRightCondition = (float) ((screen->w / 2 - lastTilePosY->getX()));
	float YTopRightCondition = (float) ((screen->h / 2 - lastTilePosXDer->getY()));
	float YTopLeftCondition = (float) ((screen->h / 2 - lastTilePosXIzq->getY()));

	bool cameraAtRightForTop = camera->getX() < XTopRightCondition;
	bool cameraAtRightForBot = camera->getX() < XBotRightCondition;
	bool cameraAtTopForRight = camera->getY() > YTopRightCondition;
	bool cameraAtTopForLeft = camera->getY() > YTopLeftCondition;

	if (cameraAtRightForTop && cameraAtTopForRight) {

		float m = (YTopRightCondition - (float) ((screen->h / 2)))
				/ ((float) ((-lastTilePosXDer->getX() + screen->w / 2))
						- XTopRightCondition);
		float b = (float) ((screen->h / 2)) - XTopRightCondition * m;
		float eval = camera->getX() * m + b;
		if (eval < camera->getY())	camera->setY(eval);
	} else if (cameraAtRightForBot && !cameraAtTopForRight) {

		float m = (YTopRightCondition
				- (float) ((-lastTilePosY->getY() + screen->h / 2)))
				/ ((float) ((screen->w / 2 - lastTilePosXDer->getX()))
						- XBotRightCondition);
		float b = (float) ((-lastTilePosY->getY() + screen->h / 2)) - XBotRightCondition * m;
		float eval = camera->getX() * m + b;
		if (eval > camera->getY())	camera->setY(eval);
	} else if (!cameraAtRightForTop && cameraAtTopForLeft) {

		float m = (YTopLeftCondition - (float) ((screen->h / 2)))
				/ ((float) ((-lastTilePosXIzq->getX() + screen->w / 2))
						- XTopRightCondition);
		float b = (float) ((screen->h / 2)) - XTopRightCondition * m;
		float eval = camera->getX() * m + b;
		if (eval < camera->getY()) camera->setY(eval);
	} else if (!cameraAtRightForBot && !cameraAtTopForLeft) {

		float m = (YTopLeftCondition
				- (float) ((-lastTilePosY->getY() + screen->h / 2)))
				/ ((float) ((screen->w / 2 - lastTilePosXIzq->getX()))
						- XBotRightCondition);
		float b = (float) ((-lastTilePosY->getY() + screen->h / 2)) - XBotRightCondition * m;
		float eval = camera->getX() * m + b;
		if (eval > camera->getY()) camera->setY(eval);
	}
}

void MapView::checkBoundaries() {
	checkBasicBoundaries();
	checkAdvancedBoundaries();
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

