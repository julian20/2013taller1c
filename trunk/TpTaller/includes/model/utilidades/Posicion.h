/* 
 * File:   Posicion.h
 * Author: lucas
 *
 * Created on March 21, 2013, 10:58 AM
 */

#ifndef POSICION_H
#define	POSICION_H

class Posicion {
public:
    Posicion(int x, int y, int z);
    void cambiarA(int nuevaX, int nuevaY, int nuevaZ);
private:
    Posicion();
    int x, y, z;
};

#endif	/* POSICION_H */

