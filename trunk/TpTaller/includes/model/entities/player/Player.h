/* 
 * File:   Personaje.h
 * Author: lucas
 *
 * Created on March 21, 2013, 10:57 AM
 */

#ifndef PLAYER_H
#define	PLAYER_H

#include <model/utilities/Position.h>

class Player {
public:
    Player* createInPosition(int x, int y, int z);

private:
//Metodos
    Player();

//Atributos
    Position *currentLocation, *destinyLocation;
    float speed; //esto hay que cambiarlo cuando veamos como se mueve

};

#endif	/* PLAYER_H */

