/*
 * SpellEffectView.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: fede
 */

#include <view/entities/spells/FireballView.h>
#define WIDTH 64

FireballView::FireballView() : MobileEntityView() {
	this->image = NULL;
	this->spell = NULL;
}

void FireballView::loadImage() {
	this->walkingImage = textureHolder->getTexture(string(FIREBALL_IMG));
	this->idleImage = walkingImage;
	this->imageWidth = WIDTH;
	this->numberOfWalkingClips = computeNumberOfClips(walkingImage);
	this->numberOfIdleClips = computeNumberOfClips(walkingImage);
}

void FireballView::setSpellEffect(SpellEffect* spell) {
	this->spell = spell;
}

SpellEffect* FireballView::getSpellEffect() {
	return spell;
}

FireballView::~FireballView() {

}

