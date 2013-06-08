/*
 * SpellEffectView.h
 *
 *  Created on: Jun 6, 2013
 *      Author: fede
 */

#ifndef FireballView_H_
#define FireballView_H_

#include <view/entities/MobileEntityView.h>
#include <model/entities/spells/SpellEffect.h>

class FireballView : MobileEntityView{
public:
	FireballView();
	void setSpellEffect(SpellEffect* spell);
	SpellEffect* getSpellEffect();
	virtual ~FireballView();
private:
	virtual void loadImage();
	SDL_Surface* image;
	SpellEffect* spell;
};

#endif
