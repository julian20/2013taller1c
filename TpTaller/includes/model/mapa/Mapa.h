/* 
 * File:   Mapa.h
 * Author: lucas
 *
 * Created on March 21, 2013, 3:06 PM
 */

#ifndef MAPA_H
#define	MAPA_H

class Mapa {
public:
    Mapa(int largoX, int largoY);
    Mapa(const Mapa& orig);
    virtual ~Mapa();
private:
    //Probablemente convenga tener un vector<void*> mas que con punteros directos, pendiente
    void*** matriz;
    int largoX, largoY;
};

#endif	/* MAPA_H */

