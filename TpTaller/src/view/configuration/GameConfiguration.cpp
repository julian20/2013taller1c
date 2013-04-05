/*
 * AnimationConfiguration.cpp
 *
 *  Created on: Apr 4, 2013
 *      Author: gonchub
 */

#include <view/configuration/GameConfiguration.h>

#define DEFAULT_DELAY 		30  //ms supongo?
#define DEFAULT_FPS 		30  //frames
#define DEFAULT_W       	800 // px
#define DEFAULT_H       	600 // px
#define DEFAULT_BPP 		0   // px?
#define DEFAULT_GAME_MUSIC "resources/sound/gameMusic.ogg"

GameConfiguration::GameConfiguration() {
	this->delay = DEFAULT_DELAY;
	this->fps = DEFAULT_FPS;
	this->defaultScreenHeight = DEFAULT_H;
	this->defaultScreenWidth = DEFAULT_W;
	this->defaultBPP = DEFAULT_BPP;
	this->gameMusicSrc = DEFAULT_GAME_MUSIC;
}

GameConfiguration::~GameConfiguration() {
}

unsigned int GameConfiguration::getFps() {
	return this->fps;
}

unsigned int GameConfiguration::getDelay() {
	return this->delay;
}

unsigned int GameConfiguration::getDefaultScreenHeight() {
	return this->defaultScreenHeight;
}

unsigned int GameConfiguration::getDefaultScreenWidth() {
	return this->defaultScreenWidth;
}

unsigned int GameConfiguration::getDefaultBPP() {
	return this->defaultBPP;
}

std::string GameConfiguration::getGameMusicSrc() {
	return this->gameMusicSrc;
}

void GameConfiguration::setFps(unsigned int fps) {
	this->fps = fps;
}

void GameConfiguration::setDelay(unsigned int delay) {
	this->delay = delay;
}

void GameConfiguration::setGameMusicSrc(std::string musicSrc) {
	this->gameMusicSrc = musicSrc;
}

void GameConfiguration::setDefaultScreenHeight(unsigned int height) {
	this->defaultScreenHeight = height;
}

void GameConfiguration::setDefaultScreenWidth(unsigned int width) {
	this->defaultScreenWidth = width;
}

void GameConfiguration::setDefaultBPP(unsigned int bpp) {
	this->defaultBPP = bpp;
}
