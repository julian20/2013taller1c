/*
 * MapCameraController.h
 *
 *  Created on: 07/04/2013
 *      Author: tomas
 */

#ifndef MAPCAMERACONTROLLER_H_
#define MAPCAMERACONTROLLER_H_

#include <SDL/SDL.h>
#include <view/MapCameraView.h>

namespace std {

typedef enum {
	MOUSE_TOP,
	MOUSE_BOTTOM,
	MOUSE_RIGHT,
	MOUSE_LEFT,
	MOUSE_CENTER,
	MOUSE_TOP_RIGHT,
	MOUSE_TOP_LEFT,
	MOUSE_BOTTOM_RIGHT,
	MOUSE_BOTTOM_LEFT
} MousePosition;

class MapCameraController {
public:
	MapCameraController(MapCameraView* camera, unsigned int movementMargin);
	void cameraMoveListener();
	virtual ~MapCameraController();
private:
	MousePosition getMousePosition();
	int computeMovementSpeed(MousePosition pos);
	bool mouseAtLeft();
	bool mouseAtRight();
	bool mouseAtTop();
	bool mouseAtBottom();

	MapCameraView* camera;
	int mouseX, mouseY;
	SDL_Surface* screen;
	int movementMargin;
};

} /* namespace std */
#endif /* MAPCAMERACONTROLLER_H_ */
