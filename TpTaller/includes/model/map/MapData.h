#ifndef MAPDATA_H
#define	MAPDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <view/IRepresentable.h>
#include <model/Personaje.h>

#define MAX_TILE_CONTENT	5

struct tileData {
	int tileType;
	Personaje* personaje;
	IRepresentable *content[MAX_TILE_CONTENT];
	int contentAmount;
};

class MapData {
public:
	static const int NEUTRAL = 0;
	static const int SOIL = 1;
	static const int WATER = 2;
	static const int TREE = 3;
	static const int AMOUNT_TYPES = 4;  // Igual al mayor +1 index de consts

	MapData(int _nrows, int _ncols);
	virtual ~MapData();

	void AddRepresentable(int row, int col, IRepresentable *object);
	void SetTileType(int tileType, int row, int col);
	int GetTileType(int row, int col);
	int GetNRows();
	int GetNCols();
	tileData * GetTileData(int row, int col);
	void AddPersonaje(int row, int col, Personaje* personaje);
	Personaje* GetPersonaje(int row, int col);
private:
	tileData *data;
	int nrows;
	int ncols;

	void InitializeData();
	void CheckRowColsValue(int row, int col);
};

#endif	/* MAPDATA_H */

