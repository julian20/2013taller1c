/*
 * Speed.h
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#ifndef SPEED_H_
#define SPEED_H_

#include <model/entityProperties/position/Position.h>

/**
 * TODO Uso la posicion como un vector para definir la direccion, en realidad
 * la clase Posicion deberia ser un vector, que nosotros usamos para describir
 * la posicion. O de ultima que Posicion herede de un vector y lo reutilizamos
 * para describir distintas cosas.
 */

class Speed {
public:
	virtual ~Speed();
	Speed(int magnitude, Position* direction);
	int getMagnitude() const;
	void setMagnitude(int magnitude);
	const Position* getDirection() const;
	void setDirection(Position* direction);

private:
	Speed();
	Position* direction;
	int magnitude;
};

#endif /* SPEED_H_ */
