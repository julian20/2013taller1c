#include <view/Map.h>

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

void Map::Draw(SDL_Surface* pantalla) {
	CameraUpdate();

    SDL_Rect posTile;

    int widthTexture = tilesTextures[0]->w - TilesOverlap;
    int heightTexture = tilesTextures[0]->h - TilesOverlap;

    for (int col = 0; col < data->GetNCols(); col++) {
        for (int row = 0; row < data->GetNRows(); row++) {
            posTile.x = cameraX - MapMargin + (row % 2) * widthTexture / 2 + col * widthTexture;
            posTile.y = cameraY - MapMargin + row * heightTexture / 2;
            posTile.w = widthTexture;
            posTile.h = heightTexture;

            SDL_BlitSurface(tilesTextures[ data->GetTileType(row, col) ],
            				NULL,
            				pantalla,
            				&posTile);
        }
    }
}



