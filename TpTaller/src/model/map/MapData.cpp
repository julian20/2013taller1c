#include <model/map/MapData.h>
#include <model/entityProperties/Position.h>

#define VisibleTilesMargin	2

MapData::MapData(int _nrows, int _ncols) {
	this->mainPlayer = NULL;
	nrows = _nrows;
	ncols = _ncols;

	if (nrows <= 0 || ncols <= 0) {
		// TODO: tirar error al log
	}

	initializeData();
}

MapData::~MapData() {
}

void MapData::initializeData() {
	for (int i = 0; i < nrows * ncols; i++) {
		TileData tempData;
		tempData.setType("neutral");
		data.push_back(tempData);
	}
}

void MapData::setTileType(std::string tileType, int row, int col) {
	checkRowColsValue(row, col);

	data[row + nrows * col].setType(tileType);
}

std::string MapData::getTileType(int row, int col) {
	checkRowColsValue(row, col);

	TileData* tileData = getTileData(row, col);
	return tileData->getType();
}

void MapData::checkRowColsValue(int row, int col) {
	if ((row < 0 || row >= nrows) || (col < 0 || col >= ncols)) {
		Logs::logErrorMessage(
				"Se ha intentado agregar una entity a un tile inexistente");
	}
}

vector<MobileEntity*> MapData::getnewMobileEntities() {
	return newMobileEntities;
}

list<Entity*> MapData::getDeadEntities() {
	list<Entity*> retval;

	list<Entity*>::const_iterator iter;
	for (iter = addedEntities.begin(); iter != addedEntities.end(); ++iter) {
		Entity* current = *iter;

		if (current->getRemoveFromGame()) {
			retval.push_back(current);

			TileData* tileData = getTileData(current->getCoordinates());
			tileData->removeEntity(current);
		//	tileData->removeMobileEntity((MobileEntity*) current);

			current->onRemove(this);
		}
	}

	// Elimino las entities que se sacaron de la lista de entities
	for (iter = retval.begin(); iter != retval.end(); ++iter) {
		Entity* current = *iter;

		addedEntities.remove(current);
	}

	return retval;
}

list<MobileEntity*> MapData::getDeadMobiles() {
	list<MobileEntity*> retval;

	list<MobileEntity*>::const_iterator iter;
	for (iter = addedMobiles.begin(); iter != addedMobiles.end(); ++iter) {
		MobileEntity* current = *iter;

		if (current->getRemoveFromGame()) {
			retval.push_back(current);

			TileData* tileData = getTileData(current->getCoordinates());
			tileData->removeMobileEntity(current);

			if (current->getClassName() == "Golem") {
				cout << "GOLEM" << endl;
			}

			current->onRemove(this);
			//addedMobiles.remove(current);
		}
	}

	// Elimino las mobiles que se sacaron de la lista de entities
	for (iter = retval.begin(); iter != retval.end(); ++iter) {
		MobileEntity* current = *iter;

		addedMobiles.remove(current);
	}

	return retval;
}

void MapData::cleanNewMobileEntities() {
	newMobileEntities.erase(newMobileEntities.begin(), newMobileEntities.end());
}

void MapData::cleanNewEntities() {
	newEntities.erase(newEntities.begin(), newEntities.end());
}

vector<Entity*> MapData::getNewEntities() {
	return newEntities;
}

Player* MapData::getMainPlayer() {
	return mainPlayer;
}

void MapData::addEntity(int row, int col, Entity* object) {
	addedEntities.push_back(object);
	TileData* currentData = getTileData(row, col);
	currentData->addEntity(object);
	object->setTile(new Tile(new Coordinates(row, col)));

	Base* base = object->getBase();
	int baseRow = base->getRows();
	int baseCol = base->getCols();

	for (int offsetRow = 0; offsetRow < baseRow; offsetRow++) {
		for (int offsetCol = 0; offsetCol < baseCol; offsetCol++) {

			int currentRow = row + offsetRow - baseRow / 2;
			int currentCol = col + offsetCol - baseCol / 2;

			if (currentRow >= 0 && currentRow < nrows && currentCol >= 0
					&& currentCol < ncols) {

				if (row == currentRow && col == currentCol)
					continue;

				currentData = getTileData(currentRow, currentCol);

				//Ponemos una copia
				Entity* copy = new Entity(object);
				currentData->addEntity(copy);
				copy->setCoordinates(currentRow, currentCol);
				currentData->setWalkable(false);

				newEntities.push_back(object);
			}

		}
	}
}

void MapData::addItem(int row, int col, Item* object) {
	addedEntities.push_back(object);
	newEntities.push_back(object);
	TileData* currentData = getTileData(row, col);
	currentData->addEntity(object);
	object->setTile(new Tile(new Coordinates(row, col)));

	Base* base = object->getBase();
	int baseRow = base->getRows();
	int baseCol = base->getCols();

	for (int offsetRow = 0; offsetRow < baseRow; offsetRow++) {
		for (int offsetCol = 0; offsetCol < baseCol; offsetCol++) {

			int currentRow = row + offsetRow - baseRow / 2;
			int currentCol = col + offsetCol - baseCol / 2;

			if (currentRow >= 0 && currentRow < nrows && currentCol >= 0
					&& currentCol < ncols) {

				if (row == currentRow && col == currentCol)
					continue;

				currentData = getTileData(currentRow, currentCol);

				//Ponemos una copia
				Item* copy;
				if (object->getClassName() == "LifeHeart")
					copy = new LifeHeart(object);
				else if (object->getClassName() == "MagicBottle")
					copy = new MagicBottle(object);
				else if (object->getClassName() == "SpeedBoots")
					copy = new SpeedBoots(object);
				else if (object->getClassName() == "Lantern")
					copy = new Lantern(object);
				else if (object->getClassName() == "Shield")
					copy = new Shield(object);
				else if (object->getClassName() == "MapItem")
					copy = new MapItem(object);
				else if (object->getClassName() == "Chest")
					copy = new Chest(object);
				else if (object->getClassName() == "StrengthBuff")
					copy = new StrengthBuff(object);
				else if (object->getClassName() == "EarthquakeItem")
					copy = new EarthquakeItem(object);
				else if (object->getClassName() == "CrystalBallItem")
					copy = new CrystalBallItem(object);
				else if (object->getClassName() == "SpellShieldItem")
					copy = new SpellShieldItem(object);
				else if (object->getClassName() == "FreezeSpellItem")
					copy = new FreezeSpellItem(object);
				else if (object->getClassName() == "FrostWandItem")
					copy = new FrostWandItem(object);
				else if (object->getClassName() == "GolemSpellItem")
					copy = new GolemSpellItem(object);
				else
					copy = new Item(object);

				currentData->addEntity(copy);
				copy->setCoordinates(currentRow, currentCol);

			}

		}
	}
}

TileData* MapData::getTileData(int row, int col) {
	if ((row < 0 || row >= nrows) || (col < 0 || col >= ncols)) {
		std::cout << "Se esta intentando pedir un tile fuera de rango"
				<< std::endl;
		//ANTE ERROR DEVOLVEMOS el 0,0
		return &data[0];
	}

	return &data[row + nrows * col];
}

TileData* MapData::getTileData(Coordinates coords) {
	return getTileData(coords.getRow(), coords.getCol());
}

void MapData::addPlayer(int row, int col, Player* player) {
	TileData* tileData = getTileData(row, col);

	Tile* personajeTile = new Tile(new Coordinates(row, col));
	Tile* current;
	if (tileData->isWalkable()) { // Si el tile es transitable se agrega el personaje ahi
		tileData->addMobileEntity(player);

		player->setTile(personajeTile);
	} else {	// Si no lo es se lo agrega a una posicion adyacente
		TileData* currentData;
		Coordinates currentCoords;
		map<int, Tile *> tilesContainer;// Uso esto para ir guardando los punteros
		list<Tile *> neighborTiles = getNeighborTiles(personajeTile,
				&tilesContainer, true);

		list<Tile *>::const_iterator iter;
		for (iter = neighborTiles.begin(); iter != neighborTiles.end();
				++iter) {
			current = *iter;

			currentCoords.setRow(current->getCoordinates().getRow());
			currentCoords.setCol(current->getCoordinates().getCol());
			currentData = getTileData(currentCoords);
			if (currentData->isWalkable())
				break;
		}

		currentData->addMobileEntity(player);

		delete personajeTile;
		Tile* personajeTile = new Tile(
				new Coordinates(currentCoords.getRow(),
						currentCoords.getCol()));
		player->setTile(personajeTile);
		player->moveImmediately(currentCoords);
		player->setCoordinates(currentCoords.getRow(), currentCoords.getCol());
	}

	if (player->isMainPlayer())
		mainPlayer = player;

}

void MapData::addMobileEntity(int row, int col, MobileEntity* mobileEntity) {
	addedMobiles.push_back(mobileEntity);
	newMobileEntities.push_back(mobileEntity);
	TileData* tileData = getTileData(row, col);

	Tile* entityTile = new Tile(new Coordinates(row, col));

	tileData->addMobileEntity(mobileEntity);

	mobileEntity->setTile(entityTile);
}

void MapData::updateMobilePos(int prevRow, int prevCol, int row, int col,
		MobileEntity* mobile) {

	if (prevRow == row && prevCol == col)
		return;

	TileData* tileDataPrev = getTileData(prevRow, prevCol);
	TileData* tileDataCurrent = getTileData(row, col);

	std::list<Entity*> mobileEntities = tileDataCurrent->getEntities();

	list<Entity *>::const_iterator iter;
	for (iter = mobileEntities.begin(); iter != mobileEntities.end(); ++iter) {
		Entity* current = *iter;

		MobileEntity& thisMobileEntity = *mobile;
		thisMobileEntity.reverseCollide(*current);
	}

	tileDataPrev->removeMobileEntity(mobile);
	tileDataCurrent->addMobileEntity(mobile);

	Tile* personajeTile = new Tile(new Coordinates(row, col));
	mobile->setTile(personajeTile);
	mobile->setCoordinates(row, col);
}

MobileEntity* MapData::getPlayer(int row, int col) {
	if ((row < 0 || row >= nrows) || (col < 0 || col >= ncols)) {
		std::cout << "Se esta intentando pedir un personaje fuera de rango"
				<< std::endl;
		return NULL;
	}

	return data[row + nrows * col].getAttackableMobileEntity();
}

Tile *getTileFromContainer(map<int, Tile *> *tilesContainer, int row, int col) {
	Tile *tile = new Tile(new Coordinates(row, col));
	int hashValue = tile->getHashValue();

	bool existsTile =
			!(tilesContainer->find(hashValue) == tilesContainer->end());

	if (!existsTile) {
		(*tilesContainer)[hashValue] = tile;
		return tile;
	} else {
		delete tile;
		return (*tilesContainer)[hashValue];
	}
}

void emptyTilesContainer(map<int, Tile *> tilesContainer) {
	std::map<int, Tile *>::iterator iter;
	for (iter = tilesContainer.begin(); iter != tilesContainer.end(); ++iter) {
		Tile* tile = iter->second;

		delete tile;
	}
}

Tile* MapData::getRandomTile() {
	int randRow;
	int randCol;

	srand(time(NULL));
	while (true) {
		randRow = rand() % getNRows();
		randCol = rand() % getNCols();

		Coordinates coords = Coordinates(randRow, randCol);

		if (getTileData(coords)->isWalkable())
			break;
	}

	Coordinates coor = Coordinates(randRow, randCol);
	return new Tile(coor);
}

void MapData::addTileToList(list<Tile *> *list,
		map<int, Tile *> *tilesContainer, int row, int col,
		bool getNoWalkableTiles) {

	if (row < 0 || row >= nrows || col < 0 || col >= ncols)
		return;

	TileData* tileData = getTileData(row, col);

	if (tileData->isWalkable() || getNoWalkableTiles) {
		Tile* tile = getTileFromContainer(tilesContainer, row, col);

		list->push_back(tile);
	}
}

list<Tile *> MapData::getNeighborTiles(Tile* tile,
		map<int, Tile *> *tilesContainer, bool getNoWalkableTiles) {
	list<Tile *> neighborTiles;
	Coordinates coords = tile->getCoordinates();

	int col = coords.getCol();
	int row = coords.getRow();

	addTileToList(&neighborTiles, tilesContainer, row, col - 1, getNoWalkableTiles);
	addTileToList(&neighborTiles, tilesContainer, row - 1, col, getNoWalkableTiles);
	addTileToList(&neighborTiles, tilesContainer, row - 1, col - 1, getNoWalkableTiles);
	addTileToList(&neighborTiles, tilesContainer, row, col + 1, getNoWalkableTiles);
	addTileToList(&neighborTiles, tilesContainer, row + 1, col, getNoWalkableTiles);
	addTileToList(&neighborTiles, tilesContainer, row + 1, col + 1, getNoWalkableTiles);
	addTileToList(&neighborTiles, tilesContainer, row + 1, col - 1, getNoWalkableTiles);
	addTileToList(&neighborTiles, tilesContainer, row - 1, col + 1, getNoWalkableTiles);

	return neighborTiles;
}

list<Tile *> MapData::getNeighborTiles(Tile* tile) {
	list<Tile *> neighborTiles;
	Coordinates coords = tile->getCoordinates();

	int col = coords.getCol();
	int row = coords.getRow();

	if (col > 0)
		neighborTiles.push_back(new Tile(new Coordinates(row, col - 1)));
	if (row > 0)
		neighborTiles.push_back(new Tile(new Coordinates(row - 1, col)));
	if (col > 0 && row > 0)
		neighborTiles.push_back(new Tile(new Coordinates(row - 1, col - 1)));
	if (col < ncols - 1)
		neighborTiles.push_back(new Tile(new Coordinates(row, col + 1)));
	if (row < nrows - 1)
		neighborTiles.push_back(new Tile(new Coordinates(row + 1, col)));
	if (col < ncols - 1 && row < nrows - 1)
		neighborTiles.push_back(new Tile(new Coordinates(row + 1, col + 1)));
	if (col > 0 && row < nrows - 1)
		neighborTiles.push_back(new Tile(new Coordinates(row + 1, col - 1)));
	if (col < ncols - 1 && row > 0)
		neighborTiles.push_back(new Tile(new Coordinates(row - 1, col + 1)));

	return neighborTiles;
}

bool tileExistInList(list<Tile *> list, Tile* tile) {
	std::list<Tile *>::const_iterator iter;
	for (iter = list.begin(); iter != list.end(); ++iter) {
		Tile* current = *iter;

		if (current->isEqual(tile))
			return true;
	}

	return false;
}

bool compTileList(Tile* A, Tile* B) {
	float AScore = A->getFScore();
	float BScore = B->getFScore();
	return (AScore > BScore);
}

// Devuelve los tiles que estan en el marco de un cuadrado
list<Tile *> MapData::getTilesInSquare(int startRow, int startCol, int endRow, int endCol,
			map<int, Tile *> *tilesContainer, bool getNoWalkableTiles) {
	list<Tile *> tiles;

	for (int row = startRow; row <= endRow; row++)
		addTileToList(&tiles, tilesContainer, row, startCol, getNoWalkableTiles);

	for (int row = endRow; row <= endRow; row++)
		addTileToList(&tiles, tilesContainer, row, startCol, getNoWalkableTiles);

	for (int col = startCol + 1; col < endCol; col++)
		addTileToList(&tiles, tilesContainer, startRow, col, getNoWalkableTiles);

	for (int col = startCol + 1; col < endCol; col++)
		addTileToList(&tiles, tilesContainer, endRow, col, getNoWalkableTiles);

	return tiles;
}

Tile* MapData::getValidTile(Tile* from, Tile* goal) {
	// Si el tile al que se quiere ir no es walkable se intenta ir al tile
	// walkable mas proximo a la posicion de partida

	TileData* tileData = getTileData(goal->getCoordinates());
	if (tileData->isWalkable())
		return goal;

	map<int, Tile *> tilesContainer;// Uso esto para ir guardando los punteros

	Coordinates fromCoords = from->getCoordinates();
	int row = goal->getCoordinates().getRow();
	int col = goal->getCoordinates().getCol();
	int squareSize = 1;

	while (true) {
		list<Tile *> tiles = getTilesInSquare(row - squareSize, col - squareSize,
											  row + squareSize, col + squareSize,
											  &tilesContainer);

		list<Tile *>::const_iterator iter;
		for (iter = tiles.begin(); iter != tiles.end();	++iter) {
			// Cargo la distancia con el punto de partida a cada tile
			Tile* current = *iter;
			current->setFScore(distBetweenTiles(current, from));
		}

		tiles.sort(compTileList);

		while (tiles.size() > 0) {
			Tile* current = tiles.back();
			tiles.remove(current);

			Coordinates currentCooords = current->getCoordinates();
			TileData* tileData = getTileData(currentCooords);

			if (tileData->isWalkable() || currentCooords.isEqual(fromCoords)) {
				current = new Tile(new Coordinates(current->getCoordinates()));
				emptyTilesContainer(tilesContainer);
				return current;
			}
		}

		squareSize++;
	}

	return NULL;
}

void printPath(list<Tile *>* path) {
	list<Tile *>::const_iterator iter;
	for (iter = path->begin(); iter != path->end(); ++iter) {
		Tile* tile = *iter;
		Coordinates coords = tile->getCoordinates();

		printf("row: %d  col:%d\n", coords.getRow(), coords.getCol());
	}
}

list<Tile *> *MapData::getPath(Tile* from, Tile* goal) {
	TileData* data = getTileData(from->getCoordinates());
	if (data == NULL) {
		std::cout << "Segfault avoided like a champ" << std::endl;
		return new list<Tile *>();
	}
	if (data->isWalkable(true) == false) {
		std::cout << "El tile donde se encuentra el jugador no es transitable"
				<< std::endl;
		return new list<Tile *>();
	}

	Tile* currentGoal = getValidTile(from, goal);
	map<int, Tile *> tilesContainer; // Uso esto para ir guardando los punteros
	list<Tile *> closedSet;
	list<Tile *> openSet;
	openSet.push_back(from);
	map<int, Tile *> cameFrom;

	map<int, float> g_score;
	g_score[from->getHashValue()] = 0.0f;
	from->setFScore(
			g_score[from->getHashValue()]
					+ heuristicCostEstimate(from, currentGoal));
	Tile* current;

	while (openSet.size() > 0) {
		openSet.sort(compTileList);
		current = openSet.back();	// node having the lowest fScore value

		if (current->isEqual(currentGoal)) {
			list<Tile *>* path = reconstructPath(cameFrom, currentGoal);
			emptyTilesContainer(tilesContainer);
			return path;
		}

		openSet.remove(current);
		closedSet.push_back(current);
		list<Tile *> neighborTiles = getNeighborTiles(current, &tilesContainer);

		list<Tile *>::const_iterator iter;
		for (iter = neighborTiles.begin(); iter != neighborTiles.end();
				++iter) {
			Tile* neighbor = *iter;
			float tentativeGScore = g_score[current->getHashValue()]
					+ distBetweenTiles(current, neighbor);

			bool existsGScore = !(g_score.find(neighbor->getHashValue())
					== g_score.end());
			if (existsGScore
					&& (tentativeGScore >= g_score[neighbor->getHashValue()]))
				continue;

			bool neighborInOpenSet = tileExistInList(openSet, neighbor);
			if (!neighborInOpenSet
					|| (tentativeGScore < g_score[neighbor->getHashValue()])) {
				cameFrom[neighbor->getHashValue()] = current;
				g_score[neighbor->getHashValue()] = tentativeGScore;
				neighbor->setFScore(
						g_score[neighbor->getHashValue()]
								+ heuristicCostEstimate(neighbor, currentGoal));
				if (!neighborInOpenSet)
					openSet.push_back(neighbor);
			}
		}
	}

	emptyTilesContainer(tilesContainer);
	return new list<Tile *>();
}

float MapData::heuristicCostEstimate(Tile* from, Tile* to) {
	float dist = distBetweenTiles(from, to);

	return dist * 2;
}

float MapData::distBetweenTiles(Tile* from, Tile* to) {
	Coordinates fromCoords = from->getCoordinates();
	Coordinates toCoords = to->getCoordinates();

	Position* fromPos = Tile::computePosition(fromCoords.getRow(),
			fromCoords.getCol());
	Position* toPos = Tile::computePosition(toCoords.getRow(),
			toCoords.getCol());

	float result = sqrt(
			pow(toPos->getX() - fromPos->getX(), 2)
					+ pow(toPos->getY() - fromPos->getY(), 2));

	delete fromPos;
	delete toPos;

	// Devuelvo la norma del vector que une ambos puntos
	return result;
}

// Devuelve la distancia entre dos tiles sin considerar obstaculos ni nada
int MapData::distBetweenTilesInTiles(Tile* from, Tile* to) {
	Coordinates fromCoords = from->getCoordinates();
	Coordinates toCoords = to->getCoordinates();

	int colDiff = abs(fromCoords.getCol() - toCoords.getCol());
	int rowDiff = abs(fromCoords.getRow() - toCoords.getRow());

	// Magic below this comment
	if (colDiff < rowDiff) {
		return rowDiff;
	} else {
		return colDiff;
	}
}

list<Tile *> *MapData::reconstructPath(map<int, Tile *> cameFrom,
		Tile* currentNode) {
	list<Tile *> *path;

	if (cameFrom.find(currentNode->getHashValue()) != cameFrom.end()) {	// if exists in cameFrom
		path = reconstructPath(cameFrom, cameFrom[currentNode->getHashValue()]);
	} else {
		path = new list<Tile *>();
	}

	Coordinates coords = currentNode->getCoordinates();
	Tile *newTile = new Tile(new Coordinates(coords.getRow(), coords.getCol()));

	path->push_back(newTile);
	return path;
}

void MapData::moveMobileEntity(MobileEntity* mobile, Tile* toTile) {
	Tile* fromTile = mobile->getTile();
	Coordinates fromCoords = fromTile->getCoordinates();
	Coordinates toCoords = toTile->getCoordinates();

	// Si la posicion inicial y final son distintas calcula el path
	if (!fromCoords.isEqual(toCoords)) {
		list<Tile *> *path = getPath(fromTile, toTile);
		mobile->assignPath(path, this);
	}

	delete fromTile;
}
void MapData::cleanVisibleTilesVector() {
	std::vector<TileData *>::iterator iter;
	for (iter = visibleTiles.begin(); iter != visibleTiles.end(); ++iter) {
		TileData* tileData = *iter;

		tileData->setVisibility(false);
	}

	visibleTiles.erase(visibleTiles.begin(), visibleTiles.end());
}

void MapData::showAllMap() {

	for (int row = 0; row < nrows; row++) {
		for (int col = 0; col < ncols; col++) {

			getTileData(row, col)->setVisibility(true);
			getTileData(row, col)->setVisibility(false);

		}
	}

}

void MapData::updateVisibleTiles(MobileEntity* mobile) {
	if (mobile == NULL)
		mobile = mainPlayer;

	Vector3* playerPos = mobile->getCurrentPos();
	int pX = playerPos->getX();
	int pY = playerPos->getY();
	Tile* entityTile = new Tile(Tile::getTileCoordinates(pX, pY));

	int viewRange = mobile->getViewRange() / sqrt(2);

	// La posicion vertical son rows y la horizontal cols.
	Tile topTile = Tile::getTileCoordinates(pX + viewRange, pY - viewRange);
	Tile bottomTile = Tile::getTileCoordinates(pX - viewRange, pY + viewRange);
	Tile leftTile = Tile::getTileCoordinates(pX - viewRange, pY - viewRange);
	Tile rightTile = Tile::getTileCoordinates(pX + viewRange, pY + viewRange);

	int topRow = topTile.getCoordinates().getRow();
	int bottomRow = bottomTile.getCoordinates().getRow();
	int leftCol = leftTile.getCoordinates().getCol();
	int rightCol = rightTile.getCoordinates().getCol();

	if (topRow < VisibleTilesMargin)
		topRow = VisibleTilesMargin;
	if (leftCol < VisibleTilesMargin)
		leftCol = VisibleTilesMargin;
	if (bottomRow > nrows - VisibleTilesMargin)
		bottomRow = nrows - VisibleTilesMargin;
	if (rightCol > ncols - VisibleTilesMargin)
		rightCol = ncols - VisibleTilesMargin;

	for (int row = topRow - VisibleTilesMargin;
			row < bottomRow + VisibleTilesMargin; row++) {

		for (int col = leftCol - VisibleTilesMargin;
				col < rightCol + VisibleTilesMargin; col++) {

			Tile currentTile;
			currentTile.setCoordinates(row, col);

			if (distBetweenTiles(&currentTile, entityTile)
					< mobile->getViewRange()) {

				TileData* tileData = getTileData(row, col);

				tileData->setVisibility(true);
				visibleTiles.push_back(tileData);
			}

		}
	}

	delete entityTile;
}

bool compMobileEntitiesList(MobileEntityWithCenterDistance A,
		MobileEntityWithCenterDistance B) {
	return (A.centerDistance > B.centerDistance);
}

// Devuelve una lista ordenada por mas cercano a mas lejano de mobileEntities
// que esten a 'tilesRange' o menos tiles de distancia de centerCoordinates
// Si removeEntityInPosition es true no se devuelve la entidad que se encuentre en
// centerCoordinates.
list<MobileEntity *> MapData::getClosestEntities(Coordinates centerCoordinates,
		int tilesRange, bool removeEntityInPosition) {
	list<MobileEntityWithCenterDistance> entitiesToSort;
	Tile* centerTile = new Tile(centerCoordinates);

	int topRow = centerCoordinates.getRow() - tilesRange;
	int bottomRow = centerCoordinates.getRow() + tilesRange;
	int leftCol = centerCoordinates.getCol() - tilesRange;
	int rightCol = centerCoordinates.getCol() + tilesRange;

	if (topRow < 0)
		topRow = 0;
	if (leftCol < 0)
		leftCol = 0;
	if (bottomRow > nrows)
		bottomRow = nrows;
	if (rightCol > ncols)
		rightCol = ncols;

	for (int row = topRow; row < bottomRow; row++) {

		for (int col = leftCol; col < rightCol; col++) {

			Tile currentTile;
			currentTile.setCoordinates(row, col);

			if (removeEntityInPosition
					&& currentTile.getCoordinates().isEqual(centerCoordinates))
				continue;

			int distance = distBetweenTilesInTiles(centerTile, &currentTile);

			if (distance <= tilesRange) {

				TileData* tileData = getTileData(row, col);
				MobileEntity* currentMobileEntity =
						tileData->getAttackableMobileEntity();

				if (currentMobileEntity) {
					MobileEntityWithCenterDistance toSortEntity;
					toSortEntity.entity = currentMobileEntity;
					toSortEntity.centerDistance = distance;

					entitiesToSort.push_back(toSortEntity);
				}
			}

		}
	}

	entitiesToSort.sort(compMobileEntitiesList);

	MobileEntityWithCenterDistance current;
	list<MobileEntity *> retval;
	list<MobileEntityWithCenterDistance>::const_iterator iter;
	for (iter = entitiesToSort.begin(); iter != entitiesToSort.end(); ++iter) {
		current = *iter;

		retval.push_back(current.entity);
	}

	return retval;
}

int MapData::getNRows() {
	return nrows;
}

int MapData::getNCols() {
	return ncols;
}

