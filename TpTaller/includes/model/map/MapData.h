#ifndef MAPDATA_H
#define	MAPDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <model/entities/Entity.h>
#include <model/entities/Items/Item.h>
#include <model/entities/MobileEntity.h>
#include <model/entities/player/Player.h>
#include <model/Logs/Logs.h>
#include <model/map/Tile.h>
#include <model/map/TileData.h>
#include <list>
#include <map>
#include <math.h>

using namespace std;

class MapData {
public:
	static const int AMOUNT_TYPES = 4;  // Igual al mayor +1 index de consts

	MapData(int _nrows, int _ncols);
	virtual ~MapData();

	void addEntity(int row, int col, Entity* object);
	void addItem(int row, int col, Item* object);
	void setTileType(std::string tileType, int row, int col);
	std::string getTileType(int row, int col);
	int getNRows();
	int getNCols();
	TileData* getTileData(Coordinates coords);
	TileData* getTileData(int row, int col);
	void addPlayer(int row, int col, Player* personaje);
	void addMobileEntity(int row, int col, MobileEntity* player);
	MobileEntity* getPlayer(int row, int col);
	void updatePlayerPos(int prevRow, int prevCol,
							int row, int col, Player* personaje);
	list<Tile *> *getPath(Tile* from, Tile* to);
	list<Entity *> *Entities;
	void movePlayer(Player* personaje, Tile* toTile);
	void updateVisibleTiles();
	void cleanVisibleTilesVector();
	list<MobileEntity *> getClosestEntities(Coordinates centerCoordinates,
										int tilesRange);
private:
	float heuristicCostEstimate(Tile* from, Tile* to);
	float distBetweenTiles(Tile* from, Tile* to);
	int distBetweenTilesInTiles(Tile* from, Tile* to);
	void addTileToList(list<Tile *> *list, map<int, Tile *> *tilesContainer,
			int row, int col, bool getNoWalkableTiles);
	list<Tile *> *reconstructPath(map<int, Tile *> cameFrom, Tile* goal);
	list<Tile *> getNeighborTiles(Tile* tile, map<int, Tile *> *tilesContainer,
			bool getNoWalkableTiles = false);
	Tile* getValidTile(Tile* from, Tile* goal);

	Player* mainPlayer;
	vector<TileData *> visibleTiles;
	vector<TileData> data;
	int nrows;
	int ncols;

	void initializeData();
	void checkRowColsValue(int row, int col);
};

class MobileEntityWithCenterDistance{
public:
	MobileEntity* entity;
	int centerDistance;
};

#endif	/* MAPDATA_H */

