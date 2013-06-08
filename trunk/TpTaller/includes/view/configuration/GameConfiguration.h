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
	bool screenAutoConfig();
	bool fullscreen();
	unsigned int getFps();
	unsigned int getDelay();
	unsigned int getMovementMargin();
	unsigned int getDefaultScreenHeight();
	unsigned int getDefaultScreenWidth();
	unsigned int getDefaultBPP();
	unsigned int getServerPort();
	std::string getServerIP();
	std::string getGameMusicSrc();
	std::string getMenuBackImageSrc();
	std::string getMenuBackMusicSrc();
	std::string getMission();
	unsigned int getTileWidth();
	unsigned int getTileHeight();
	void setServerIP(std::string ipAddress);
	void setServerPort(unsigned int serverPort);
	void setFps(unsigned int fps);
	void setDelay(unsigned int delay);
	void setMovementMargin(unsigned int movementMargin);
	void setDefaultScreenHeight(unsigned int height);
	void setDefaultScreenWidth(unsigned int width);
	void setDefaultBPP(unsigned int bpp);
	void setGameMusicSrc(std::string musicSrc);
	void setMenuBackImageSrc(std::string imageSrc);
	void setMenuBackMusicSrc(std::string imageSrc);
	void setTileWidth(unsigned int _tileWidth);
	void setTileHeight(unsigned int _tileHeight);
	void setScreenAutoConfig(bool screenAutoConfig);
	void setFullscreen(bool fullscreen);
	void setMission(std::string mission);
private:
	unsigned int fps;
	unsigned int delay;
	unsigned int movementMargin;
	unsigned int defaultScreenHeight;
	unsigned int defaultScreenWidth;
	unsigned int defaultBPP;
	unsigned int tileWidth;
	unsigned int tileHeight;
	unsigned int serverPort;
	bool bScreenAutoConfig;
	bool bFullscreen;
	std::string serverIP;
	std::string gameMusicSrc;
	std::string menuBackImageSrc;
	std::string menuBackMusicSrc;
	std::string mission;
};

#endif /* GAMECONFIGURATION_H_ */
