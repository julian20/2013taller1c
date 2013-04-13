#include <model/map/MapData.h>
#include <model/entityProperties/Position.h>
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

	InitializeData();
}

MapData::~MapData() {
}

void MapData::InitializeData() {
	for (int i = 0; i < nrows * ncols; i++) {
		TileData tempData;
		tempData.setType("neutral");
		data.push_back(tempData);
	}
}

void MapData::SetTileType(std::string tileType, int row, int col) {
	// ACA deberia ir una comprobacion de si el tileType ese existe pero
	// creo q ya va a estar en en YAML
	CheckRowColsValue(row, col);

	data[row + nrows * col].setType(tileType);
}

std::string MapData::GetTileType(int row, int col) {
	CheckRowColsValue(row, col);

	return data[row + nrows * col].getType();
}

void MapData::CheckRowColsValue(int row, int col) {
	if (row < 0 || row >= nrows) {
		// TODO: tirar error al log
	}
	if (col < 0 || col >= ncols) {
		// TODO: tirar error al log
	}
}

void MapData::addRepresentable(int row, int col, Entity* object) {
	TileData currentData = data[row + nrows * col];

	currentData.addEntity(object);

}

TileData* MapData::GetTileData(int row, int col) {
	return &data[row + nrows * col];
}

void MapData::addPersonaje(int row, int col, Player* personaje) {
	data[row + nrows * col].setPersonaje(personaje);

	Tile* personajeTile = new Tile(new Coordinates(row, col));
	//personaje->setTile(personajeTile);
}

Player* MapData::GetPersonaje(int row, int col) {
	return data[row + nrows * col].getPersonaje();
}

/**
 * TODO: ~~~ Gonchu ~~~
 * Tratar de usar list<Tile*> path en vez de
 * list<Tile*> *path = new list<Tile*>() ya que la primera
 * llama al destructor sobre cada elemento cuando se desreferencia,
 * la segunda hay que llamar al destructor a mano. Si se sabe donde
 * destruir no hay problema, pero es mas complicado me parece.
 *
 * Otra cosa, las Coordinates de la tile solo se usan aca, en todos
 * los otros lados (me parece) se usa la position, fijate si podes
 * usar la position y volamos la clase Coordinate a la mierda.
 */
list<Tile *> *MapData::GetPath(Tile* from, Tile* to) {
	/*list<Tile *> closedSet;
	 list<Tile *> openSet;
	 openSet.insert( openSet.end(), from );
	 list<Tile *> cameFrom;

	 map<Tile *, float> g_score;
	 g_score[from] = 0.0f;
	 map<Tile *, float> f_score;
	 f_score[from] = g_score[from] + heuristic_cost_estimate(from, to);

	 while( openSet.size() > 0 ){
	 }*/

	list<Tile *> *path = new list<Tile *>();
	Tile* nextTile;

	nextTile = from;
	int row, col;
	Coordinates* nextTileCords = nextTile->getCoordinates();
	Coordinates* toCords = to->getCoordinates();

	while (nextTileCords->getCol() != toCords->getCol()
		|| nextTileCords->getRow() != toCords->getRow()) {

		int nextTileCol = nextTileCords->getCol();
		int nextTileRow = nextTileCords->getRow();
		int toCol = toCords->getCol();
		int toRow = toCords->getRow();

		if (toCol > nextTileCol)
			col = nextTileCol + 1;
		else if (toCol < nextTileCol)
			col = nextTileCol - 1;
		else
			col = nextTileCol;

		if (toRow > nextTileRow)
			row = nextTileRow + 1;
		else if (toRow < nextTileRow)
			row = nextTileRow - 1;
		else
			row = nextTileRow;

		nextTile = new Tile(new Coordinates(row, col));
		nextTileCords = nextTile->getCoordinates();
		path->insert(path->end(), nextTile);
	}

	return path;
}

/*float heuristic_cost_estimate(Tile* from, Tile* to) {
 Coordinates* fromPos = from->getCoordinates();
 Coordinates* toPos = to->getCoordinates();

 // Devuelvo la norma del vector que une ambos puntos
 return sqrt( pow(toPos->row - fromPos->row, 2) + pow(toPos->col - fromPos->col, 2));
 }*/

void MapData::movePersonaje(Player* personaje, Tile* toTile) {
	/*Tile* fromTile = personaje->getTile();

	list<Tile *> *path = GetPath(fromTile, toTile);
	delete (toTile);
	personaje->assignPath(path);
*/
	list<Tile *> *path = new list<Tile *>();
	path->insert(path->end(), toTile);
	personaje->assignPath(path);
}

int MapData::GetNRows() {
	return nrows;
}

int MapData::GetNCols() {
	return ncols;
}


