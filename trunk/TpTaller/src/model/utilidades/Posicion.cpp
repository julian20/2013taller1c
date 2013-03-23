/* 
 * File:   Posicion.cpp
 * Author: lucas
 * 
 * Created on March 21, 2013, 10:58 AM
 */

#include <model/utilidades/Posicion.h>

Posicion::Posicion(){
	x=0;
	y=0;
	z=0;
}

Posicion::Posicion(int coordX, int coordY, int coordZ) {
    x = coordX;
    y = coordY;
    z = coordZ;

}

void Posicion::cambiarA(int nuevaX, int nuevaY, int nuevaZ) {
    x = nuevaX;
    y = nuevaY;
    z = nuevaZ;

}

