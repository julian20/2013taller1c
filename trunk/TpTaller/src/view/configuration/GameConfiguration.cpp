/*
 * AnimationConfiguration.cpp
 *
 *  Created on: Apr 4, 2013
 *      Author: gonchub
 */

#include <view/configuration/GameConfiguration.h>

#define SCREEN_AUTOCONFIG   false
#define FULLSCREEN          false
#define DEFAULT_DELAY 		30  //ms supongo?
#define DEFAULT_FPS 		30  //frames
#define DEFAULT_W       	800 // px
#define DEFAULT_H       	600 // px
#define DEFAULT_BPP 		0   // px?
#define DEFAULT_GAME_MUSIC "resources/sound/stalemate.ogg"
#define MENU_IMG "resources/menu/wastelands.jpg"
#define MENU_MUSIC  "resources/menu/sound/QuinnLogin.ogg"
#define DEFAULT_TILE_WIDTH	70
#define DEFAULT_TILE_HEIGHT	50
#define DEFAULT_IP "192.168.190.136"
#define DEFAULT_PORT 32001

GameConfiguration::GameConfiguration() {
	this->serverIP = DEFAULT_IP;
	this->serverPort = DEFAULT_PORT;
	this->delay = DEFAULT_DELAY;
	this->fps = DEFAULT_FPS;
	this->defaultScreenHeight = DEFAULT_H;
	this->defaultScreenWidth = DEFAULT_W;
	this->defaultBPP = DEFAULT_BPP;
	this->gameMusicSrc = DEFAULT_GAME_MUSIC;
	this->tileWidth = DEFAULT_TILE_WIDTH;
	this->tileHeight = DEFAULT_TILE_HEIGHT;
	bFullscreen=NULL;
	bScreenAutoConfig=false;
	movementMargin=0;
}

GameConfiguration::~GameConfiguration() {
}

bool GameConfiguration::screenAutoConfig() {
	return bScreenAutoConfig || bFullscreen;
}

bool GameConfiguration::fullscreen() {
	return bFullscreen;
}

unsigned int GameConfiguration::getFps() {
	return this->fps;
}

unsigned int GameConfiguration::getDelay() {
	return this->delay;
}

unsigned int GameConfiguration::getMovementMargin() {
	return this->movementMargin;
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

std::string GameConfiguration::getMenuBackImageSrc() {
	return this->menuBackImageSrc;
}

std::string GameConfiguration::getMenuBackMusicSrc() {
	return this->menuBackMusicSrc;
}

std::string GameConfiguration::getMission() {
	return this->mission;
}

unsigned int GameConfiguration::getTileWidth() {
	return this->tileWidth;
}

unsigned int GameConfiguration::getTileHeight() {
	return this->tileHeight;
}

void GameConfiguration::setFps(unsigned int fps) {
	this->fps = fps;
}

void GameConfiguration::setDelay(unsigned int delay) {
	this->delay = delay;
}

void GameConfiguration::setMovementMargin(unsigned int movementMargin) {
	this->movementMargin = movementMargin;
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

void GameConfiguration::setMenuBackImageSrc(std::string src) {
	this->menuBackImageSrc = src;
}

void GameConfiguration::setMenuBackMusicSrc(std::string src) {
	this->menuBackMusicSrc = src;
}

void GameConfiguration::setTileWidth(unsigned int _tileWidth) {
	this->tileWidth = _tileWidth;
}

void GameConfiguration::setTileHeight(unsigned int _tileHeight) {
	this->tileHeight = _tileHeight;
}
void GameConfiguration::setScreenAutoConfig(bool screenAutoConfig) {
	this->bScreenAutoConfig = screenAutoConfig;
}
void GameConfiguration::setFullscreen(bool fullscreen) {
	this->bFullscreen = fullscreen;
}

void GameConfiguration::setServerIP(std::string ipAddress) {
	this->serverIP = ipAddress;
}

void GameConfiguration::setServerPort(unsigned int serverPort) {
	this->serverPort = serverPort;
}

void GameConfiguration::setMission(std::string mission) {
	this->mission = mission;
}

unsigned int GameConfiguration::getServerPort() {
	return this->serverPort;
}

std::string GameConfiguration::getServerIP() {
	return this->serverIP;
}
