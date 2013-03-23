/*
 * Game.cpp
 *
 *  Created on: 22/03/2013
 *      Author: tomas
 */
#include <stdio.h>

#include <model/Game.h>
#include <model/ConfigurationReader.h>

Game::Game() {
	// TODO Auto-generated constructor stub

}

Game::Game(ConfigurationReader* cfgReader){
	// TODO
}

bool gameEnd() {
    return true;
}

void getEvent() {
}

void refreshMap() {
}

void refreshEntities() {
}

void refreshCharacters() {
}

void draw() {
}

void Game::run(){

    while (!gameEnd()) {
    	printf("Hello World \n");
/*        // 1ro leo entrada del usuario y resuelvo segun dicha entrada
        getEvent();

        //Actualizo la parte visual, sin mostrarla todavia
        //1. Mapa
        refreshMap();
        // 2. Entidades estaticas
        refreshEntities();
        // 3. Personaje/s
        refreshCharacters();

        // En un futuro, aca se comprueban las colisiones.
        // y se corrige la posicion del personaje.

        // Dibujo
        draw();
*/
    }
}



Game::~Game() {
	// TODO Auto-generated destructor stub
}

