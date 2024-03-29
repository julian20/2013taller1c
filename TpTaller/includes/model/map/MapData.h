#ifndef MAPDATA_H
#define	MAPDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <model/entities/Entity.h>
#include <model/entities/Flag.h>
#include <model/entities/Items/Item.h>
#include <model/entities/Items/LifeHeart.h>
#include <model/entities/Items/MagicBottle.h>
#include <model/entities/Items/SpeedBoots.h>
#include <model/entities/Items/Lantern.h>
#include <model/entities/Items/Shield.h>
#include <model/entities/Items/MapItem.h>
#include <model/entities/Items/Chest.h>
#include <model/entities/Items/StrengthBuff.h>
#include <model/entities/Items/EarthquakeItem.h>
#include <model/entities/Items/CrystalBallItem.h>
#include <model/entities/Items/SpellShieldItem.h>
#include <model/entities/Items/FreezeSpellItem.h>
#include <model/entities/Items/GolemSpellItem.h>
#include <model/entities/Items/FrostWandItem.h>
#include <model/entities/MobileEntity.h>
#include <model/entities/player/Player.h>
#include <model/Logs/Logs.h>
#include <model/map/Tile.h>
#include <model/map/TileData.h>
#include <model/entities/enemies/Mob.h>
#include <list>
#include <map>
#include <math.h>

using namespace std;

class MapData {
public:
	MapData(int _nrows, int _ncols);
	virtual ~MapData();

	void addEntity(int row, int col, Entity* object);
	void addItem(int row, int col, Item* object);
	//void addFlag(int row, int col, Flag* flag);

	void setTileType(std::string tileType, int row, int col);
	std::string getTileType(int row, int col);
	int getNRows();
	int getNCols();
	TileData* getTileData(Coordinates coords);
	TileData* getTileData(int row, int col);

	list<Entity*> getDeadEntities();
	list<MobileEntity*> getDeadMobiles();
	vector<MobileEntity* > getnewMobileEntities();
	void cleanNewMobileEntities();
	vector<Entity* > getNewEntities();
	void cleanNewEntities();

	void addPlayer(int row, int col, Player* personaje);
	void addMobileEntity(int row, int col, MobileEntity* player);

	MobileEntity* getPlayer(int row, int col);
	Player* getMainPlayer();
	void updateMobilePos(int prevRow, int prevCol,
							int row, int col, MobileEntity* personaje);
	list<Tile *> *getPath(Tile* from, Tile* to);
	void moveMobileEntity(MobileEntity* mobile, Tile* toTile);
	list<MobileEntity *> getClosestEntities(Coordinates centerCoordinates,
			int tilesRange, bool removeEntityInPosition = true);
	list<Tile *> getNeighborTiles(Tile* tile);
	void cleanVisibleTilesVector();
	void updateVisibleTiles(MobileEntity* mobile = NULL);
	void showAllMap();
	static int distBetweenTilesInTiles(Tile* from, Tile* to);
	static float distBetweenTiles(Tile* from, Tile* to);
	Tile* getValidTile(Tile* from, Tile* goal);
	Tile* getRandomTile();
private:
	float heuristicCostEstimate(Tile* from, Tile* to);
	void addTileToList(list<Tile *> *list, map<int, Tile *> *tilesContainer,
			int row, int col, bool getNoWalkableTiles);
	list<Tile *> *reconstructPath(map<int, Tile *> cameFrom, Tile* goal);
	list<Tile *> getNeighborTiles(Tile* tile, map<int, Tile *> *tilesContainer,
			bool getNoWalkableTiles = false);
	void initializeData();
	void checkRowColsValue(int row, int col);
	list<Tile *> getTilesInSquare(int startRow, int startCol, int endRow, int endCol,
			map<int, Tile *> *tilesContainer, bool getNoWalkableTiles = false);

	Player* mainPlayer;
	vector<TileData *> visibleTiles;
	vector<TileData> data;
	vector<MobileEntity* > newMobileEntities;
	vector<Entity* > newEntities;
	list<Entity*> addedEntities;
	list<MobileEntity*> addedMobiles;

	int nrows;
	int ncols;
};

class MobileEntityWithCenterDistance{
public:
	MobileEntity* entity;
	int centerDistance;
};

#endif	/* MAPDATA_H */

