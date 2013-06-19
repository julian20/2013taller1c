/* 
 * File:   PlayerView.h
 * Author: damian
 *
 * Created on 23 de marzo de 2013, 12:25
 */
#ifndef PLAYERVIEW_H
#define	PLAYERVIEW_H

#include <model/entities/player/Player.h>
#include <view/networking/ChatWindowsView.h>
#include <view/entities/MobileEntityView.h>
#include <view/TextHandler.h>
#include <model/entityProperties/Position.h>
#include <networking/PlayerEvent.h>
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

class FoggedSprite{
public:
	SDL_Surface* image;
	SDL_Surface* foggedImage;
	SDL_Surface* teamColorImage;
	int numberOfClips;
};

class PlayerView: public MobileEntityView {
public:
	PlayerView();
	PlayerView(PlayerView* otherPlayer);
	void Show(SDL_Surface* fondo, bool drawFog);
	void setPersonaje(Player* personaje);
	Player* getPersonaje();
	void UpdateCameraPos(Position* _camPos);
	Player* getEntity();
	void setEntity(Entity* entity);
	int getNClips();
	void setNClips(int clips);
	void setName(std::string name);
	void setShowableName(string name);
	void EstablecerLosClips();
	void draw(SDL_Surface* screen, Position* camera, bool drawFog);
	void showWeaponsHud(SDL_Surface* screen);
	list<PlayerEvent*> getPlayerViewEvents();
	string getClassName();

	virtual ~PlayerView();
private:
	void loadWeaponsHudSurfaces();
	void showFrame(SDL_Surface* screen, SDL_Rect* clip, bool drawFog);

	void showStandingAnimation(SpriteType sprite, SDL_Surface* fondo,
			bool drawFog);
	void playAnimation(SpriteType sprite, SDL_Surface* screen, bool drawFog);
	void loadPlayerImage();
	FoggedSprite loadFoggedSprite(const char* modifier);
	FoggedSprite loadFoggedSpellSprite(const char* modifier);
	map<string, FoggedSprite> loadSwordImages();
	map<string, FoggedSprite> loadBowImages();
	map<string, FoggedSprite> loadSpellImages();

	void blitSpellEffect(SDL_Surface* screen, int x, int y);
	void blitName(SDL_Surface* screen, int x, int y);
	void blitHPBar(SDL_Surface* screen, int x, int y);
	void showCorpse(SDL_Surface* fondo, bool drawFog, SpriteType sprite);
	void blitIceSpellEffect(SDL_Surface* screen, int x, int y);
	ChatWindowsView* chatView;

	Player* player;
	Position* camPos;
	SpriteType currentSprite;
	int marco, currentSpellClip, currentIceSpellClip; 	// Current frame
	int animationChangeRate;
	int numberOfClips;
	int direction;
	float lastDirection;
	bool wasStanding,attacking;
	bool attacked, spellBeingCast, spellHasEnded;
	int previousLife;
	SDL_Surface* nameImage;
	TextHandler textHandler;
	list<PlayerEvent*> events;
	Timer damageReceivedTimer;
	/* Map: key: weapon. Value: map of images; key: animation name,
	 * value: vector with FoggedSprites
	 */
	map<string, map<string, FoggedSprite > > weaponViewMap;
	map<string, FoggedSprite> spriteMap;
	map<string,FoggedSprite> spellMap;
	map<string, SDL_Surface* > weaponsHud;
	bool loaded;

};

#endif	/* PERSONAJEVISTA_H */

