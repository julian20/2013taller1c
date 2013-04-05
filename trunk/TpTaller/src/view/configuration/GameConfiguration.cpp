/*
 * AnimationConfiguration.cpp
 *
 *  Created on: Apr 4, 2013
 *      Author: gonchub
 */

#include <view/configuration/GameConfiguration.h>

#define DEFAULT_DELAY 30 //ms supongo?
#define DEFAULT_FPS 30 //frames

GameConfiguration::GameConfiguration() {
	this->delay = DEFAULT_DELAY;
	this->fps = DEFAULT_FPS;
}

GameConfiguration::~GameConfiguration() {
}

unsigned int GameConfiguration::getFps() {
	return this->fps;
}

unsigned int GameConfiguration::getDelay() {
	return this->delay;
}

void GameConfiguration::setFps(unsigned int fps) {
	this->fps = fps;
}

void GameConfiguration::setDelay(unsigned int delay) {
	this->delay = delay;
}
