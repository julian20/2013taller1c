#include <view/Map.h>

#define TILE_POS_FUNC	GetSquaredMapTilePos

#define MapMargin		40		// px
#define TilesOverlap	2		// px
#define TilesScale  	1
#define CameraSpeed		5		// px

Map::Map(MapData* _data) {
    data = _data;
    cameraX = cameraY = 0;
    mapController = new MapController();

    DefineTexturePaths();
    GraphicalSetup();
}

void Map::DefineTexturePaths() {
    texturesPaths[ MapData::NEUTRAL ] = "texturesTiles/sand.png";
    texturesPaths[ MapData::SOIL ] = "texturesTiles/dirtTile.png";
    texturesPaths[ MapData::WATER ] = "texturesTiles/Water.png";
    texturesPaths[ MapData::TREE ] = "texturesTiles/stonebrick.png";

    backgroundPath = "./background.png";
}

Vector2* Map::GetCamera(){
	return new Vector2(cameraX, cameraY);
}

void Map::GraphicalSetup() {
    //Cargamos las textura de los tiles
    for (int i = 0; i < MapData::AMOUNT_TYPES; i++) {
        tilesTextures[i] = IMG_Load(texturesPaths[i].c_str());
        if (tilesTextures[i] == NULL) {
            printf("Unable to load tile texture n°:%d: %s\n", i, SDL_GetError());
            exit(1);
        }
    }

    // Convertimos las imagenes a display format para que 
    // se muestre mas rapido por pantalla

    for (int i = 0; i < MapData::AMOUNT_TYPES; i++) {
        tilesTextures[i] = rotozoomSurfaceXY(tilesTextures[i], 0, TilesScale, TilesScale, 0);
        tilesTextures[i] = SDL_DisplayFormatAlpha(tilesTextures[i]);
    }

}

Map::~Map() {
}

void Map::CameraUpdate() {
	if (mapController->MoveScreenUp()) {
		cameraY += CameraSpeed;
	} else if (mapController->MoveScreenDown()) {
		cameraY -= CameraSpeed;
	} else if (mapController->MoveScreenLeft()) {
		cameraX += CameraSpeed;
	} else if (mapController->MoveScreenRight()) {
		cameraX -= CameraSpeed;
	} else if (mapController->MoveScreenLeftUp()) {
		cameraX += CameraSpeed;
		cameraY += CameraSpeed;
	} else if (mapController->MoveScreenLeftDown()) {
		cameraX += CameraSpeed;
		cameraY -= CameraSpeed;
	} else if (mapController->MoveScreenRightUp()) {
		cameraX -= CameraSpeed;
		cameraY += CameraSpeed;
	} else if (mapController->MoveScreenRightDown()) {
		cameraX -= CameraSpeed;
		cameraY -= CameraSpeed;
	}
}

void Map::SetUpPersonajes(){
	// Setea la posicion por unica vez de los personajes (y en un futuro posiblemente
	// todo lo representable) en el mapa.
    SDL_Rect posTile;

    for (int col = 0; col < data->GetNCols(); col++) {
        for (int row = 0; row < data->GetNRows(); row++) {

            Personaje* personaje = data->GetPersonaje(row, col);
            if (personaje != NULL) {
            	posTile = TILE_POS_FUNC(row, col);
            	personaje->SetPos((float)posTile.x, (float)posTile.y);
            }
        }
    }
}

void Map::Draw(SDL_Surface* screen) {

	CameraUpdate();

	//Personaje* personajes = NULL;
    SDL_Rect posTile;

    for (int col = 0; col < data->GetNCols(); col++) {

        for (int row = 0; row < data->GetNRows(); row++) {

        	posTile = TILE_POS_FUNC(row, col);

            SDL_BlitSurface(tilesTextures[ data->GetTileType(row, col) ],
            				NULL,
            				screen,
            				&posTile);

            tileData* tileData = data->GetTileData(row, col);

            // TODO: Dibujar las cosas de tileData
            // En esta parte deberia agregarlo a una lista, como solo hay uno por
            // ahora solo se guarda en una variable

           // Personaje* personaje = data->GetPersonaje(row, col);
           // if (personaje != NULL) personajes = personaje;
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

SDL_Rect Map::GetSquaredMapTilePos(int row, int col) {
    int widthTexture = tilesTextures[0]->w - TilesOverlap;
    int heightTexture = tilesTextures[0]->h - TilesOverlap;

    SDL_Rect posTile;

    posTile.x = cameraX - MapMargin + (row % 2) * widthTexture / 2 + col * widthTexture;
    posTile.y = cameraY - MapMargin + row * heightTexture / 2;
    posTile.w = widthTexture;
    posTile.h = heightTexture;

    return posTile;
}

SDL_Rect Map::GetDiamondShapeMapTilePos(int row, int col) {
    int widthTexture = tilesTextures[0]->w - TilesOverlap;
    int heightTexture = tilesTextures[0]->h - TilesOverlap;

    SDL_Rect posTile;

    posTile.x = cameraX + (col + row) * widthTexture / 2;
    posTile.y = cameraY + (col - row) * heightTexture / 2;
    posTile.w = widthTexture;
    posTile.h = heightTexture;

    return posTile;
}

void Map::ClickOn(int x, int y, int button) {
	// Selecciona la casilla mas o menos bien, idealizandola como un cuadrado.
	SDL_Rect firstTile = TILE_POS_FUNC(0, 0);

	int row = (y - firstTile.y)*2/firstTile.h;
	int col = (x - firstTile.x)/firstTile.w;

	printf("row: %d, col: %d\n", row, col);
}

void Map::Update() {

}



