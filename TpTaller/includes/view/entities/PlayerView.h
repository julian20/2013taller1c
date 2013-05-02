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
#include <view/TextHandler.h>
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

typedef enum {
	LEFT, UP_LEFT, UP, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT
} SpriteType;

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
	void setName(std::string name);
	virtual ~PlayerView();
	void EstablecerLosClips();
	void draw(SDL_Surface* screen, Position* camera, bool drawFog);
	void playAnimation(SpriteType sprite, SDL_Surface* screen);
private:
	void showFrame(SDL_Surface* source, SDL_Surface* screen, SDL_Rect* clip);
	void showStandingAnimation(SpriteType sprite, SDL_Surface* fondo);
	void loadPlayerImage();
	Player* player;
	Position* camPos;
	SpriteType currentSprite;
	int marco; 	// Current frame
	int animationChangeRate;
	int imageWidth;
	int imageHeight;
	int numberOfClips;
	int direction;
	float lastDirection;
	bool wasStanding;

	SDL_Surface* attackImage;
	int numberOfAttackClips;
	SDL_Surface* walkingImage;
	int numberOfWalkingClips;
	SDL_Surface* idleImage;
	int numberOfIdleClips;
	SDL_Surface* runningImage;
	int numberOfRunningClips;
	SDL_Surface* idleBlockImage;
	int numberOfIdleBlockClips;

	int computeNumberOfClips(SDL_Surface* img);

	SDL_Surface* nameImage;
	TextHandler textHandler;

};

#endif	/* PERSONAJEVISTA_H */

