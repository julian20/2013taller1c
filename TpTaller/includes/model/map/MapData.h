#ifndef MAPDATA_H
#define	MAPDATA_H

class MapData {
public:
    static const int NEUTRAL = 0;
    static const int SOIL = 1;
    static const int WATER = 2;
    static const int TREE = 3;
    static const int AMOUNT_TYPES = 4;  // Igual al mayor +1 index de consts
    
    MapData(int _nrows, int _ncols);
    virtual ~MapData();
    
    void SetTileType(int tileType, int row, int col);
    int GetTileType(int row, int col);
    int GetNRows();
    int GetNCols();
private:
    int *data;
    int nrows;
    int ncols;

    void InitializeData();
    void CheckRowColsValue(int row, int col);
};

#endif	/* MAPDATA_H */

