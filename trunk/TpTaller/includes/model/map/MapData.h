#ifndef MAPDATA_H
#define	MAPDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <model/entities/Entity.h>
#include <model/entities/player/Player.h>
#include <model/Logs/Logs.h>
#include <model/map/Tile.h>
#include <model/map/TileData.h>
#include <list>
#include <map>

using namespace std;

class MapData {
public:
    static const int AMOUNT_TYPES = 4;  // Igual al mayor +1 index de consts

    MapData(int _nrows, int _ncols);
    virtual ~MapData();
    
    void addEntity(int row, int col, Entity* object);
    void SetTileType(std::string tileType, int row, int col);
    std::string GetTileType(int row, int col);
    int GetNRows();
    int GetNCols();
    TileData * GetTileData(int row, int col);
    void addPersonaje(int row, int col, Player* personaje);
    Player* GetPersonaje(int row, int col);
	list<Tile *> *GetPath(Tile* from, Tile* to);
	list<Entity *> *Entities;
	void movePersonaje(Player* personaje, Tile* toTile);
	//void IdentifyTile(int x, int y);
private:
	float heuristicCostEstimate(Tile* from, Tile* to);
	float distBetweenTiles(Tile* from, Tile* to);
	list<Tile *> *reconstructPath(map<int, Tile *> cameFrom, Tile* goal);
	list<Tile *> getNeighborTiles(Tile* tile, map<int, Tile *> *tilesContainer);

    vector<TileData> data;
    int nrows;
    int ncols;

    void InitializeData();
    void CheckRowColsValue(int row, int col);
};

#endif	/* MAPDATA_H */

