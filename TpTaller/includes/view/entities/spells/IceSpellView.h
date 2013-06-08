/*
 * SpellEffectView.h
 *
 *  Created on: Jun 6, 2013
 *      Author: fede
 */

#ifndef IceSpellView_H_
#define IceSpellView_H_

#include <view/entities/MobileEntityView.h>
#include <model/entities/spells/SpellEffect.h>

class IceSpellView : MobileEntityView{
public:
	IceSpellView();
	void setSpellEffect(SpellEffect* spell);
	SpellEffect* getSpellEffect();
	virtual void draw(SDL_Surface* screen, Position* camera, bool drawFog);

	virtual ~IceSpellView();
private:
	virtual void loadImage();
	SDL_Surface* image;
	SpellEffect* spell;
};

#endif
