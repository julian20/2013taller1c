/* 
 * File:   MobileEntityView.h
 * Author: damian
 *
 * Created on 23 de marzo de 2013, 12:25
 */
#ifndef MOBILEENTITYVIEW_H
#define	MOBILEENTITYVIEW_H

#include <model/entities/MobileEntity.h>
#include <view/networking/ChatWindowsView.h>
#include <view/entities/MobileEntityView.h>
#include <view/entities/EntityView.h>
#include <view/TextHandler.h>
#include <model/entityProperties/Position.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <model/Vector2.h>

#include <string>

#define _USE_MATH_DEFINES	// To use math.h constants
#include <math.h>

typedef enum {
	LEFT, UP_LEFT, UP, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT
} SpriteType;

#define		SCREEN_WIDTH	640
#define		SCREEN_HEIGHT  	480
#define		SCREEN_BPP  	32

using namespace std;



class MobileEntityView: public EntityView {
public:
	MobileEntityView();
	MobileEntityView(MobileEntityView* otherMobileEntity);
	void Show(SDL_Surface* fondo, bool drawFog);
	void UpdateCameraPos(Position* _camPos);
	MobileEntity* getEntity();
	void setEntity(Entity* entity);
	int getNClips();
	void setNClips(int clips);
	void setName(std::string name);
	void setShowableName(string name);
	void setMobileEntity(MobileEntity* entity);
	virtual ~MobileEntityView();
	void EstablecerLosClips();
	void draw(SDL_Surface* screen, Position* camera, bool drawFog);

protected:

	void showFrame(SDL_Surface* screen, SDL_Rect* clip, bool drawFog);
	void showStandingAnimation(SpriteType sprite, SDL_Surface* fondo, bool drawFog);
	void playAnimation(SpriteType sprite, SDL_Surface* screen, bool drawFog);
	void loadMobileEntityImage();
	MobileEntity* mobileEntity;
	Position* camPos;
	SpriteType currentSprite;
	int marco; 	// Current frame
	int animationChangeRate;
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

	int computeNumberOfClips(SDL_Surface* img);

	SDL_Surface* nameImage;
	TextHandler textHandler;

};

#endif	/* PERSONAJEVISTA_H */

