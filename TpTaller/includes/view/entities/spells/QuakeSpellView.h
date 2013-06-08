/*
 * SpellEffectView.h
 *
 *  Created on: Jun 6, 2013
 *      Author: fede
 */

#ifndef QuakeSpellView_H_
#define QuakeSpellView_H_

#include <view/entities/MobileEntityView.h>
#include <model/entities/spells/SpellEffect.h>

class QuakeSpellView : MobileEntityView{
public:
	QuakeSpellView();
	void setSpellEffect(SpellEffect* spell);
	SpellEffect* getSpellEffect();
	virtual void draw(SDL_Surface* screen, Position* camera, bool drawFog);

	virtual ~QuakeSpellView();
private:
	virtual void loadImage();
	SDL_Surface* image;
	SpellEffect* spell;
};

#endif
