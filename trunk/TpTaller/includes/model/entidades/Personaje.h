/* 
 * File:   Personaje.h
 * Author: lucas
 *
 * Created on March 21, 2013, 10:57 AM
 */

#ifndef PERSONAJE_H
#define	PERSONAJE_H

#include <model/utilidades/Posicion.h>

class Personaje {
public:
    Personaje* crearEnPosicion(int x, int y, int z);

private:
//Metodos
    Personaje();

//Atributos
    Posicion *posicionActual, *posicionDestino;
    float velocidad; //esto hay que cambiarlo cuando veamos como se mueve

};

#endif	/* PERSONAJE_H */

