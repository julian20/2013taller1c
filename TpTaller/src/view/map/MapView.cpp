#include <view/MapView.h>

#define TilesScale  	1
#define CameraSpeed		5		// px

MapView::MapView(MapData* _data) {
	data = _data;
	camera = new Position(0, 0);
	mapController = new MapController();

	DefineTexturePaths();
	GraphicalSetup();
}

Position* MapView::GetCamera() {
	return new Position( camera->getX(), camera->getY() );
}

void MapView::DefineTexturePaths() {
	/**
	 * TODO: Esto no tiene que estar (TextureHolder lo maneja)
	 */
	texturesPaths[MapData::NEUTRAL] = "texturesTiles/sand.png";
	texturesPaths[MapData::SOIL] = "texturesTiles/dirtTile.png";
	texturesPaths[MapData::WATER] = "texturesTiles/Water.png";
	texturesPaths[MapData::TREE] = "texturesTiles/stonebrick.png";
	backgroundPath = "./background.png";
}

void MapView::GraphicalSetup() {
	/**
	 * TODO: Esto no tiene que estar (TextureHolder lo maneja)
	 */
	for (int i = 0; i < MapData::AMOUNT_TYPES; i++) {
		tilesTextures[i] = IMG_Load(texturesPaths[i].c_str());
		if (tilesTextures[i] == NULL) {
			printf("Unable to load tile texture n°:%d: %s\n", i,
					SDL_GetError());
			exit(1);
		}
	}
	for (int i = 0; i < MapData::AMOUNT_TYPES; i++) {
		tilesTextures[i] = rotozoomSurfaceXY(tilesTextures[i], 0, TilesScale,
				TilesScale, 0);
		tilesTextures[i] = SDL_DisplayFormatAlpha(tilesTextures[i]);
	}
}

MapView::~MapView() {
}

void MapView::CameraUpdate() {
	if (mapController->MoveScreenUp()) {
		camera->setY( camera->getY() + CameraSpeed );
	} else if (mapController->MoveScreenDown()) {
		camera->setY( camera->getY() - CameraSpeed );
	} else if (mapController->MoveScreenLeft()) {
		camera->setX( camera->getX() + CameraSpeed );
	} else if (mapController->MoveScreenRight()) {
		camera->setX( camera->getX() - CameraSpeed );
	} else if (mapController->MoveScreenLeftUp()) {
		camera->setX( camera->getX() + CameraSpeed );
		camera->setY( camera->getY() + CameraSpeed );
	} else if (mapController->MoveScreenLeftDown()) {
		camera->setX( camera->getX() + CameraSpeed );
		camera->setY( camera->getY() - CameraSpeed );
	} else if (mapController->MoveScreenRightUp()) {
		camera->setX( camera->getX() - CameraSpeed );
		camera->setY( camera->getY() + CameraSpeed );
	} else if (mapController->MoveScreenRightDown()) {
		camera->setX( camera->getX() - CameraSpeed );
		camera->setY( camera->getY() - CameraSpeed );
	}
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
			/**
			 * TODO: El manejo de texturas se da aca (con el TextureHolder seteado).
			 */
//			std::string textureId = data->GetTileType(row, col);
//			SDL_Surface* textureImage = getTextureHolder().getImage(textureId);
//			textureImage = rotozoomSurfaceXY(textureImage, 0, TilesScale,
//					TilesScale, 0);
//			textureImage = SDL_DisplayFormatAlpha(textureImage);

			SDL_BlitSurface(tilesTextures[data->GetTileType(row, col)], NULL,
					screen, &posTile);

			TileData* tileData = data->GetTileData(row, col);

			for (int i = 0; i < (tileData->contentAmount); i++) {
				Entity* entity = tileData->content[i];
				/*Lucas: TODO!
				 if (entity)
				 entity->draw();
				 */
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
