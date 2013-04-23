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
	CheckRowColsValue(row, col);

	data[row + nrows * col].setType(tileType);
}

std::string MapData::GetTileType(int row, int col) {
	CheckRowColsValue(row, col);

	return data[row + nrows * col].getType();
}

void MapData::CheckRowColsValue(int row, int col) {
	if ((row < 0 || row >= nrows) || (col < 0 || col >= ncols)) {
		Logs::logErrorMessage("Se ha intentado agregar una entity a un tile inexistente");
	}
}

void MapData::addEntity(int row, int col, Entity* object) {
	TileData currentData = data[row + nrows * col];

	currentData.addEntity(object);
}

TileData* MapData::GetTileData(int row, int col) {
	return &data[row + nrows * col];
}

void MapData::addPersonaje(int row, int col, Player* personaje) {
	data[row + nrows * col].setPersonaje(personaje);

	Tile* personajeTile = new Tile(new Coordinates(row, col));
	personaje->setTile(personajeTile);
}

Player* MapData::GetPersonaje(int row, int col) {
	return data[row + nrows * col].getPersonaje();
}

void addTileToList(list<Tile *> *list, int row, int col){
	list->push_back( new Tile( new Coordinates(row, col) ) );
}

list<Tile *> MapData::getNeighborTiles(Tile* tile) {
	list<Tile *> neighborTiles;
	Coordinates coords = tile->getCoordinates();

	int col = coords.getCol();
	int row = coords.getRow();

	if (col > 0) addTileToList(&neighborTiles, row, col - 1);
	if (row > 0) addTileToList(&neighborTiles, row - 1, col);
	if (col > 0 && row > 0) addTileToList(&neighborTiles, row - 1, col - 1);
	if (col < ncols - 1) addTileToList(&neighborTiles, row, col + 1);
	if (row < nrows - 1) addTileToList(&neighborTiles, row + 1, col);
	if (col < ncols - 1 && row < nrows - 1) addTileToList(&neighborTiles, row + 1, col + 1);
	if (col > 0 && row < nrows - 1) addTileToList(&neighborTiles, row + 1, col - 1);
	if (col < ncols - 1 && row > 0) addTileToList(&neighborTiles, row - 1, col + 1);

	return neighborTiles;
}

bool tileExistInList(list<Tile *> list, Tile* tile) {
	std::list<Tile *>::const_iterator iter;
	for (iter = list.begin(); iter != list.end(); ++iter) {
		Tile* current = *iter;

		if (current->isEqual( tile )) return true;
	}

	return false;
}

bool compTileList(Tile* A, Tile* B) {
	float AScore = A->getFScore();
	float BScore = B->getFScore();
	return (AScore > BScore);
}

/**
 * TODO: ~~~ Gonchu ~~~
 * Tratar de usar list<Tile*> path en vez de
 * list<Tile*> *path = new list<Tile*>() ya que la primera
 * llama al destructor sobre cada elemento cuando se desreferencia,
 * la segunda hay que llamar al destructor a mano. Si se sabe donde
 * destruir no hay problema, pero es mas complicado me parece.
 */
list<Tile *> *MapData::GetPath(Tile* from, Tile* goal) {
	list<Tile *> closedSet;
	list<Tile *> openSet;
	openSet.push_back( from );
	map<int, Tile *> cameFrom;

	map<int, float> g_score;
	g_score[from->getHashValue()] = 0.0f;
	from->setFScore(g_score[from->getHashValue()] + heuristicCostEstimate(from, goal));
	Tile* current;

	while( openSet.size() > 0 ){
		openSet.sort(compTileList);
		current = openSet.back();	// node having the lowest fScore value

		if (current->isEqual( goal ))
			return reconstructPath(cameFrom, goal);

		openSet.remove(current);
		closedSet.push_back(current);
		list<Tile *> neighborTiles = getNeighborTiles(current);

		list<Tile *>::const_iterator iter;
		for (iter = neighborTiles.begin(); iter != neighborTiles.end(); ++iter) {
			Tile* neighbor = *iter;
			float tentativeGScore = g_score[current->getHashValue()] + distBetweenTiles(current, neighbor);

			bool existsGScore = !(g_score.find(neighbor->getHashValue()) == g_score.end());
			if (existsGScore && (tentativeGScore >= g_score[neighbor->getHashValue()]))
				continue;

			bool neighborInOpenSet = tileExistInList(openSet, neighbor);
			if (!neighborInOpenSet || (tentativeGScore < g_score[neighbor->getHashValue()])) {
				cameFrom[neighbor->getHashValue()] = current;
				g_score[neighbor->getHashValue()] = tentativeGScore;
				neighbor->setFScore(g_score[neighbor->getHashValue()] + heuristicCostEstimate(neighbor, goal));
				if (!neighborInOpenSet)
					openSet.push_back(neighbor);
			}
		}
	}

	/*list<Tile *> *path = new list<Tile *>();
	Tile* nextTile;

	nextTile = from;
	int row, col;
	Coordinates nextTileCords = nextTile->getCoordinates();
	Coordinates toCords = goal->getCoordinates();

	while (nextTileCords.getCol() != toCords.getCol()
		|| nextTileCords.getRow() != toCords.getRow()) {

		int nextTileCol = nextTileCords.getCol();
		int nextTileRow = nextTileCords.getRow();
		int toCol = toCords.getCol();
		int toRow = toCords.getRow();

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
		path->push_back(nextTile);
	}

	return path;*/
}

float MapData::heuristicCostEstimate(Tile* from, Tile* to) {
	float dist = distBetweenTiles(from, to);

	return dist * 2;
}

float MapData::distBetweenTiles(Tile* from, Tile* to)  {
	Coordinates fromPos = from->getCoordinates();
	Coordinates toPos = to->getCoordinates();

	// Devuelvo la norma del vector que une ambos puntos
	return sqrt( pow(toPos.getRow() - fromPos.getRow(), 2) + pow(toPos.getCol() - fromPos.getCol(), 2));

}

list<Tile *> *MapData::reconstructPath(map<int, Tile *> cameFrom, Tile* currentNode) {
	list<Tile *> *path;

	if (cameFrom.find(currentNode->getHashValue()) != cameFrom.end()) {	// if exists in cameFrom
		path = reconstructPath(cameFrom, cameFrom[currentNode->getHashValue()]);
	} else {
		 path = new list<Tile *>();
	}

	Coordinates coords = currentNode->getCoordinates();
	Tile *newTile = new Tile( new Coordinates(coords.getRow(), coords.getCol()) );

	path->push_back(newTile);
	return path;
}

void printPath( list<Tile *>* path )  {
	list<Tile *>::const_iterator iter;
	for (iter = path->begin(); iter != path->end(); ++iter) {
		Tile* tile = *iter;
		Coordinates coords = tile->getCoordinates();

		printf("row: %d  col:%d\n", coords.getRow(), coords.getCol());
	}
}

void MapData::movePersonaje(Player* personaje, Tile* toTile) {
	Tile fromTile = personaje->getTile();

	list<Tile *> *path = GetPath(&fromTile, toTile);
	delete (toTile);
	personaje->assignPath(path);

	/*list<Tile *> *path = new list<Tile *>();
	path->insert(path->end(), toTile);
	personaje->assignPath(path);*/
}

int MapData::GetNRows() {
	return nrows;
}

int MapData::GetNCols() {
	return ncols;
}


