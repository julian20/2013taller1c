/* 
 * File:   PlayerView.h
 * Author: damian
 *
 * Created on 23 de marzo de 2013, 12:25
 */
#ifndef PERSONAJEVISTA_H
#define	PERSONAJEVISTA_H

#include <model/entities/player/Player.h>
#include <view/entities/EntityView.h>
#include <model/entityProperties/Position.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <model/Vector2.h>
#include <string>

#define _USE_MATH_DEFINES	// To use math.h constants
#include <math.h>

#define		SCREEN_WIDTH	640
#define		SCREEN_HEIGHT  	480
#define		SCREEN_BPP  	32

using namespace std;

class PlayerView: public EntityView {
public:
	PlayerView();
	void Show(SDL_Surface* fondo);
	void setPersonaje(Player* personaje);
	Player* getPersonaje();
	void UpdateCameraPos(Position* _camPos);
	Player* getEntity();
	void setEntity(Entity* entity);
	int getImageHeight();
	void setImageHeight(int height);
	int getImageWidth();
	void setImageWidth(int width);
	int getNClips();
	void setNClips(int clips);
	virtual ~PlayerView();
	void EstablecerLosClips();
	void draw(SDL_Surface* screen, Position* camera);
private:
	void showFrame(SDL_Surface* source, SDL_Surface* screen, SDL_Rect* clip);
	void showStandingAnimation(float direction, SDL_Surface* fondo);
	void loadPlayerImage();
	Player* player;
	Position* camPos;
	int marco; 	// Current frame
	int animationChangeRate;
	int imageWidth;
	int imageHeight;
	int numberOfClips;
	int direction;
	bool wasStanding;
	enum {	LEFT, DOWN_LEFT, DOWN_RIGHT, DOWN, UP_LEFT, UP_RIGHT, UP, RIGHT,
			LEFT_RUN, DOWN_LEFT_RUN, DOWN_RIGHT_RUN, DOWN_RUN, UP_LEFT_RUN,
			UP_RIGHT_RUN, UP_RUN, RIGHT_RUN, STANDING};
};

#endif	/* PERSONAJEVISTA_H */

