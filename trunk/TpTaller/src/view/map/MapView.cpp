#include <view/MapView.h>
#include <model/map/TextureHolder.h>

#define MapMargin		40		// px
#define TilesScale  	1
#define CameraSpeed		15		// px
MapView::MapView(MapData* _data) {
	data = _data;
	camera = new Position(0, 0);
	mapController = new MapController();
	personaje = NULL;
	textureHolder = NULL;

	SDL_Rect posTile = Tile::computePosition(data->GetNRows(),
			data->GetNCols());
	lastTilePos = new Position(posTile.x, posTile.y);

}

Position* MapView::GetCamera() {
	return new Position(camera->getX(), camera->getY());
}

MapView::~MapView() {
}

void MapView::CameraUpdate() {
	if (mapController->MoveScreenUp()) {
		camera->setY(camera->getY() + CameraSpeed);
	} else if (mapController->MoveScreenDown()) {
		camera->setY(camera->getY() - CameraSpeed);
	} else if (mapController->MoveScreenLeft()) {
		camera->setX(camera->getX() + CameraSpeed);
	} else if (mapController->MoveScreenRight()) {
		camera->setX(camera->getX() - CameraSpeed);
	} else if (mapController->MoveScreenLeftUp()) {
		camera->setX(camera->getX() + CameraSpeed);
		camera->setY(camera->getY() + CameraSpeed);
	} else if (mapController->MoveScreenLeftDown()) {
		camera->setX(camera->getX() + CameraSpeed);
		camera->setY(camera->getY() - CameraSpeed);
	} else if (mapController->MoveScreenRightUp()) {
		camera->setX(camera->getX() - CameraSpeed);
		camera->setY(camera->getY() + CameraSpeed);
	} else if (mapController->MoveScreenRightDown()) {
		camera->setX(camera->getX() - CameraSpeed);
		camera->setY(camera->getY() - CameraSpeed);
	}

	if (camera->getX() > -MapMargin)
		camera->setX(-MapMargin);
	if (camera->getY() > -MapMargin)
		camera->setY(-MapMargin);
	if (camera->getX() < -lastTilePos->getX() + 1280)
		camera->setX(-lastTilePos->getX() + 1280);
	if (camera->getY() < -lastTilePos->getY() + 1024)
		camera->setY(-lastTilePos->getY() + 1024);
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

void MapView::Draw(SDL_Surface* screen) {

	CameraUpdate();

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

void MapView::setTextureHolder(TextureHolder* textureHolder) {
	this->textureHolder = textureHolder;
}

TextureHolder* MapView::getTextureHolder() {
	return this->textureHolder;
}
