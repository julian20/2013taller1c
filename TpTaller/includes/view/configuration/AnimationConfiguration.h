/*
 * AnimationConfiguration.h
 *
 *  Created on: Apr 4, 2013
 *      Author: gonchub
 */

#ifndef ANIMATIONCONFIGURATION_H_
#define ANIMATIONCONFIGURATION_H_

class AnimationConfiguration {
public:
	AnimationConfiguration();
	virtual ~AnimationConfiguration();
	unsigned int getFps();
	unsigned int getDelay();
	void setFps(unsigned int fps);
	void setDelay(unsigned int delay);
private:
	unsigned int fps;
	unsigned int delay;
};

#endif /* ANIMATIONCONFIGURATION_H_ */
