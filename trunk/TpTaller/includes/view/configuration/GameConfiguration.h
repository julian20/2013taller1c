/*
 * GameConfiguration.h
 *
 *  Created on: Apr 4, 2013
 *      Author: gonchub
 */

#ifndef GAMECONFIGURATION_H_
#define GAMECONFIGURATION_H_

#include <string>

class GameConfiguration {
public:
	GameConfiguration();
	virtual ~GameConfiguration();
	unsigned int getFps();
	unsigned int getDelay();
	unsigned int getDefaultScreenHeight();
	unsigned int getDefaultScreenWidth();
	unsigned int getDefaultBPP();
	std::string getGameMusicSrc();
	void setFps(unsigned int fps);
	void setDelay(unsigned int delay);
	void setGameMusicSrc(std::string musicSrc);
	void setDefaultScreenHeight(unsigned int height);
	void setDefaultScreenWidth(unsigned int width);
	void setDefaultBPP(unsigned int bpp);
private:
	unsigned int fps;
	unsigned int delay;
	unsigned int defaultScreenHeight;
	unsigned int defaultScreenWidth;
	unsigned int defaultBPP;
	std::string gameMusicSrc;
};

#endif /* GAMECONFIGURATION_H_ */
