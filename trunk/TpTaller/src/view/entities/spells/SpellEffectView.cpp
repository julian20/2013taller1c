/*
 * SpellEffectView.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: fede
 */

#include <view/entities/spells/SpellEffectView.h>

SpellEffectView::SpellEffectView() : MobileEntityView() {
	this->image = NULL;
	this->spell = NULL;
}

void SpellEffectView::loadImage() {
	this->image = textureHolder->getTexture(name);
}

void SpellEffectView::setSpellEffect(SpellEffect* spell) {
	this->spell = spell;
}

SpellEffect* SpellEffectView::getSpellEffect() {
	return spell;
}

void SpellEffectView::draw(SDL_Surface* screen, Position* camera, bool drawFog) {
	UpdateCameraPos(camera);

	if (this->image == NULL)
		loadImage();

	if (marco >= numberOfClips)
		marco = 0;

	SDL_Rect offset, clipToDraw;

	Vector3* position = spell->getCurrentPos();
	int x = position->getX();
	int y = position->getY();

	clipToDraw.x = imageWidth * marco * scaleWidth;
	clipToDraw.y = imageHeight;
	clipToDraw.w = offset.w = imageWidth * scaleWidth;
	clipToDraw.h = offset.h = imageHeight * scaleHeight;

	int h = Tile::computePositionTile(0, 0).h;
	offset.x = x + camPos->getX() - this->anchorPixel->getX();
	offset.y = y + camPos->getY() - this->anchorPixel->getY() - h / 2;

	SDL_BlitSurface(this->image, &clipToDraw, screen, &offset);

	marco++;
}

SpellEffectView::~SpellEffectView() {

}

