/*
 * AnimationConfiguration.cpp
 *
 *  Created on: Apr 4, 2013
 *      Author: gonchub
 */

#include <view/configuration/AnimationConfiguration.h>

#define DEFAULT_DELAY 30 //ms supongo?
#define DEFAULT_FPS 30 //frames

AnimationConfiguration::AnimationConfiguration() {
	this->delay = DEFAULT_DELAY;
	this->fps = DEFAULT_FPS;
}

AnimationConfiguration::~AnimationConfiguration() {
}

unsigned int AnimationConfiguration::getFps() {
	return this->fps;
}

unsigned int AnimationConfiguration::getDelay() {
	return this->delay;
}

void AnimationConfiguration::setFps(unsigned int fps) {
	this->fps = fps;
}

void AnimationConfiguration::setDelay(unsigned int delay) {
	this->delay = delay;
}
