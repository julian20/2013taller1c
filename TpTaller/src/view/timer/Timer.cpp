/*
 * Timer.cpp
 *
 *  Created on: Apr 9, 2013
 *      Author: lucas
 */

#include <view/timer/Timer.h>
#define DELAY 2000 //miliseconds
using namespace std;

Timer::Timer() {
	startTime = 0;
	timeSinceLastAnimation = 0;
	delay = DELAY;

}

void Timer::start() {
	startTime = SDL_GetTicks();
}

void Timer::setTimeSinceLastAnimation(int time) {
	timeSinceLastAnimation = time;
}

int Timer::getTimeSinceLastAnimation() {
	return SDL_GetTicks() - startTime;
}

int Timer::getTimeIntervalSinceStart() {
	return SDL_GetTicks() - startTime;

}

int Timer::getDelay() {
	return delay;
}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
}
