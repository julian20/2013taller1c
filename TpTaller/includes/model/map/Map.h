/* 
 * File:   Mapa.h
 * Author: lucas
 *
 * Created on March 21, 2013, 3:06 PM
 */

#ifndef MAP_H
#define	MAP_H

class Map {
public:
    Map(int largoX, int largoY);
    Map(const Map& orig);
    virtual ~Map();
private:
    //Probablemente convenga tener un vector<void*> mas que con punteros directos, pendiente
    void*** matriz;
    int largoX, largoY;
};

#endif	/* MAP_H */

