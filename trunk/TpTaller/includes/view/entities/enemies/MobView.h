/* 
 * File:   MobView.h
 * Author: damian
 *
 * Created on 23 de marzo de 2013, 12:25
 */
#ifndef MobVIEW_H
#define	MobVIEW_H

#include <model/entities/player/Player.h>
#include <view/networking/ChatWindowsView.h>
#include <view/entities/MobileEntityView.h>
#include <view/TextHandler.h>
#include <model/entityProperties/Position.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <model/Vector2.h>

#include <string>

#define _USE_MATH_DEFINES	// To use math.h constants
#include <math.h>

using namespace std;

class MobView: public MobileEntityView {
public:
	MobView();
private:

};

#endif	/* PERSONAJEVISTA_H */

