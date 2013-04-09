/*
 * Timer.h
 *
 *  Created on: Apr 9, 2013
 *      Author: lucas
 */

#ifndef TIMER_H_
#define TIMER_H_
#include <SDL/SDL.h>

using namespace std;

class Timer {
public:
	Timer();
	int getTimeIntervalSinceStart();
	void start();
	void setTimeSinceLastAnimation(int time);
	int getTimeSinceLastAnimation();
	int getDelay();
	virtual ~Timer();
private:
	int startTime;
	int delay;
	int timeSinceLastAnimation;

};

#endif /* TIMER_H_ */
