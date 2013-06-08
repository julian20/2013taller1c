/*
 * SpellEffectView.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: fede
 */

#include <view/entities/spells/QuakeSpellView.h>

QuakeSpellView::QuakeSpellView() : MobileEntityView() {
	this->image = NULL;
	this->spell = NULL;
}

void QuakeSpellView::loadImage() {
//	this->image = textureHolder->getTexture(string(ICE_IMG));
//	this->imageWidth = WIDTH;
//	this->numberOfClips = computeNumberOfClips(image);
}

void QuakeSpellView::setSpellEffect(SpellEffect* spell) {
	this->spell = spell;
}

SpellEffect* QuakeSpellView::getSpellEffect() {
	return spell;
}

void QuakeSpellView::draw(SDL_Surface* screen, Position* camera, bool drawFog) {
}

QuakeSpellView::~QuakeSpellView() {

}

