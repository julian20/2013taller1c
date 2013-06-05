/*
 * SpellEffect.h
 *
 *  Created on: Jun 5, 2013
 *      Author: mint
 */

#ifndef SPELLEFFECT_H_
#define SPELLEFFECT_H_
#include <model/entities/Entity.h>
#include <model/entities/MobileEntity.h>

class SpellEffect: public MobileEntity {
public:
	SpellEffect();
	virtual void applyEffects(Entity& entity);
	virtual void applyEffects(MobileEntity& entity);
	virtual void collideTo(MobileEntity& entity);
	virtual string getClassName();
	virtual ~SpellEffect();
};

#endif /* SPELLEFFECT_H_ */
