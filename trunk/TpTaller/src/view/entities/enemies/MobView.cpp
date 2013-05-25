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

//Posicion de los pies del personaje respecto de la base de la imagen
#define OFFSET_Y	15
#define ANIMATION_CHANGE_DELAY 1
#define STANDING_ANIMATION_LOCATION_IN_IMAGE_FILE 16
#define DEFAULT_CHARACTER_ID	"characterDefault"
#define NUMBER_OF_STANDING_FRAMES 3

MobView::MobView(){

}

