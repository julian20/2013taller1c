/*
 * GameConfiguration.h
 *
 *  Created on: Apr 4, 2013
 *      Author: gonchub
 */

#ifndef GAMECONFIGURATION_H_
#define GAMECONFIGURATION_H_

class GameConfiguration {
public:
	GameConfiguration();
	virtual ~GameConfiguration();
	unsigned int getFps();
	unsigned int getDelay();
	void setFps(unsigned int fps);
	void setDelay(unsigned int delay);
private:
	unsigned int fps;
	unsigned int delay;
};

#endif /* GAMECONFIGURATION_H_ */
