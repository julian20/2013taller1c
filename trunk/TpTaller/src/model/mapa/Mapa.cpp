/* 
 * File:   Mapa.cpp
 * Author: lucas
 * 
 * Created on March 21, 2013, 3:06 PM
 */

#include <model/mapa/Mapa.h>

Mapa::Mapa(int largoX, int largoY) {
    //TODO - UNTESTED
    matriz = new void** [largoX];
    
    for (int i = 0; i < largoX; i++) {
        matriz[i] = new void* [largoY];
    }
    this->largoX=largoX;
    this->largoY=largoY;
}

//Destructor
Mapa::~Mapa() {
    for (int i = 0; i < largoX; i++) {
        delete matriz[i];
    }
    delete matriz;
}

