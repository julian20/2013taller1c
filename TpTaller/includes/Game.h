/*
 * Game.h
 *
 *  Created on: 22/03/2013
 *      Author: tomas
 */

#ifndef GAME_H_
#define GAME_H_

#include <model/ConfigurationReader.h>

class Game {
public:
	Game();
	Game(ConfigurationReader* cfgReader);
	void run();
	virtual ~Game();
};

#endif /* GAME_H_ */
