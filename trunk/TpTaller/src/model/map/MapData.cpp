#include <stdlib.h>
#include <model/map/MapData.h>

/**
 * TODO: agregar clases Tile y TileDefinition, cuyos structs
 * son parseados utilizando YAML.
 */

MapData::MapData(int _nrows, int _ncols) {
    nrows = _nrows;
    ncols = _ncols;
    
    if (nrows <= 0 || ncols <= 0) {
           // TODO: tirar error al log
    }
    
    data = (int*)malloc(sizeof(int) * nrows * ncols);
    if (!data){
        // TODO: tirar error al log
    }
    
    InitializeData();
}

MapData::~MapData() {
}

void MapData::InitializeData() {
    for (int i = 0; i < nrows*ncols; i++)
    {
        data[i] = NEUTRAL;
    }
}

void MapData::SetTileType( int tileType, int row, int col ) {
    // ACA deberia ir una comprobacion de si el tileType ese existe pero
    // creo q ya va a estar en en YAML 
    CheckRowColsValue(row, col);
    
    data[row + nrows*col] = tileType;
}

int MapData::GetTileType( int row, int col ){
    CheckRowColsValue(row, col);
    
    return data[row + nrows*col];
}

void MapData::CheckRowColsValue(int row, int col) {
    if (row < 0 || row >= nrows) {
        // TODO: tirar error al log 
    }
    if (col < 0 || col >= ncols) {
        // TODO: tirar error al log
    }
}

int MapData::GetNRows() {
    return nrows;
}

int MapData::GetNCols() {
    return ncols;
}
