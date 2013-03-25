#include <view/Map.h>

#define LeftMargin  100     // px
#define TopMargin   100     // px
#define TilesScale  0.4

Map::Map(MapData* _data) {
    data = _data;

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

        // No se porque la linea de aca abajo no anda
        //tilesTextures[i] = SDL_DisplayFormatAlpha(tilesTextures[i]);
    }

}

Map::~Map() {
}

void Map::Draw(SDL_Surface* pantalla) {
    SDL_Rect posTile;

    int widthTexture = tilesTextures[0]->w;
    int heightTexture = tilesTextures[0]->h;

    for (int col = 0; col < data->GetNCols(); col++) {
        for (int row = 0; row < data->GetNRows(); row++) {
            posTile.x = (row % 2) * widthTexture / 2 + LeftMargin + col * widthTexture;
            posTile.y = TopMargin + row * heightTexture / 2;
            posTile.w = widthTexture;
            posTile.h = heightTexture;

            SDL_BlitSurface(tilesTextures[ data->GetTileType(row, col) ],
            				NULL,
            				pantalla,
            				&posTile);
        }
    }
}



