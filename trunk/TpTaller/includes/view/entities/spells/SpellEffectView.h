/*
 * SpellEffectView.h
 *
 *  Created on: Jun 6, 2013
 *      Author: fede
 */

#ifndef SPELLEFFECTVIEW_H_
#define SPELLEFFECTVIEW_H_

#include <view/entities/MobileEntityView.h>
#include <model/entities/spells/SpellEffect.h>

class SpellEffectView : MobileEntityView{
public:
	SpellEffectView();
	void setSpellEffect(SpellEffect* spell);
	SpellEffect* getSpellEffect();
	virtual void draw(SDL_Surface* screen, Position* camera, bool drawFog);

	virtual ~SpellEffectView();
private:
	virtual void loadImage();
	SDL_Surface* image;
	SpellEffect* spell;
};

#endif /* SPELLEFFECTVIEW_H_ */
