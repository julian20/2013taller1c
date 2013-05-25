/* 
 * File:   PersonajeVista.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:25
 */

#include <view/entities/enemies/MobView.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_ttf.h>
#include <model/Logs/Logs.h>

#include <cmath>
#include <string>
#define STANDING "zombieStanding"
#define STANDING_SRC "resources/zombie/zombieStanding.png"
#define WALKING "zombieWalking"
#define WALKING_SRC "resources/zombie/zombieWalking.png"

MobView::MobView(){
	textureHolder->addTexture(new TextureDefinition(STANDING,STANDING_SRC));
	textureHolder->addTexture(new TextureDefinition(WALKING,WALKING_SRC));
}

