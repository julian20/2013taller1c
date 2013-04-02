/*
 * mapController.h
 *
 *  Created on: Mar 26, 2013
 *      Author: fede
 */

#ifndef MAPCONTROLLER_H_
#define MAPCONTROLLER_H_

#include <SDL/SDL.h>

class MapController {
public:
	MapController();
	virtual ~MapController();

	bool MoveScreenLeft();
	bool MoveScreenRight();
	bool MoveScreenUp();
	bool MoveScreenDown();
	bool MoveScreenLeftUp();
	bool MoveScreenLeftDown();
	bool MoveScreenRightUp();
	bool MoveScreenRightDown();
private:
	const SDL_VideoInfo* desktop;
	void UpdateMouseState();
	int mouseX, mouseY;
};

#endif /* MAPCONTROLLER_H_ */
