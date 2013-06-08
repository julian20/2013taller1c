/*
 * SpellEffectView.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: fede
 */

#include <view/entities/spells/IceSpellView.h>
#define WIDTH 64
IceSpellView::IceSpellView() : MobileEntityView() {
	this->image = NULL;
	this->spell = NULL;
}

void IceSpellView::loadImage() {
	this->image = textureHolder->getTexture(string(ICE_IMG));
	this->imageWidth = WIDTH;
	this->numberOfClips = computeNumberOfClips(image);
}

void IceSpellView::setSpellEffect(SpellEffect* spell) {
	this->spell = spell;
}

SpellEffect* IceSpellView::getSpellEffect() {
	return spell;
}

void IceSpellView::draw(SDL_Surface* screen, Position* camera, bool drawFog) {
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

IceSpellView::~IceSpellView() {

}

