/*
 * Game.h
 *
 *  Created on: 22/03/2013
 *      Author: tomas
 */

#ifndef GAME_H_
#define GAME_H_

#include <model/persistence/ConfigurationReader.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_mixer.h>
#include <view/MapView.h>
#include <Events.h>

class Game {
public:
	Game(ConfigurationReader* cfgReader);
	MenuEvent run();
	virtual ~Game();
private:
	Mix_Music* musica;
	void initMusic();
	void setUpCharacters(MapView* map,MapData* mapData);
};

#endif /* GAME_H_ */
