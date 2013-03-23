/* 
 * File:   Personaje.cpp
 * Author: lucas
 * 
 * Created on March 21, 2013, 10:57 AM
 */
#include <stdlib.h>
#include <model/entidades/Personaje.h>
#include <model/utilidades/Posicion.h>

//Lucas: agregar mas parametros (level, armas que tiene, etc)?
Personaje::Personaje(){
	velocidad=0;
	posicionActual=NULL;
	posicionDestino=NULL;
}

Personaje* Personaje::crearEnPosicion(int x, int y, int z){
	Personaje* unPersonaje = new Personaje();
	Posicion *posicionInicial= new Posicion(x,y,z);

	unPersonaje->posicionActual=posicionInicial;
	unPersonaje->posicionActual=posicionInicial;


	return unPersonaje;
}
