
#ifndef QUAKEEFFECT_H_
#define QUAKEEFFECT_H_

#include <model/entities/spells/SpellEffect.h>
#include <model/entities/MobileEntity.h>

class QuakeEffect: public SpellEffect {
public:
	QuakeEffect();
	virtual void applyEffects(Entity& entity);
	virtual void applyEffects(MobileEntity& entity);
	virtual void collideTo(MobileEntity& entity);
	virtual string getClassName();
	virtual ~QuakeEffect();
};

#endif /* quakeEFFECT_H_ */
