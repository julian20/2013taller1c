/*
 * Game.h
 *
 *  Created on: 22/03/2013
 *      Author: tomas
 */

#ifndef GAME_H_
#define GAME_H_

#include <model/ConfigurationReader.h>
#include <SDL/SDL_events.h>
#include <Events.h>

class Game {
public:
	Game(ConfigurationReader* cfgReader);
	MenuEvent run();
	virtual ~Game();
};

#endif /* GAME_H_ */
