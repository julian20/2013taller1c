/* 
 * File:   Personaje.cpp
 * Author: lucas
 * 
 * Created on March 21, 2013, 10:57 AM
 */
#include <stdlib.h>
#include <model/entities/player/Player.h>
#include <model/entityProperties/Position.h>

//Lucas: agregar mas parametros (level, armas que tiene, etc)?
Player::Player(){
	speed=0;
	currentLocation=NULL;
	destinyLocation=NULL;
}

Player* Player::createInPosition(int x, int y, int z){
	Player* aPlayer = new Player();
	Position *initialLocation= new Position(x,y,z);

	aPlayer->currentLocation=initialLocation;
	aPlayer->destinyLocation=initialLocation;


	return aPlayer;
}
