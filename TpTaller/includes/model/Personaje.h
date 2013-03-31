/* 
 * File:   Personaje.h
 * Author: damian
 *
 * Created on 23 de marzo de 2013, 12:13
 */
#ifndef PERSONAJE_H
#define	PERSONAJE_H

#include <stdio.h>
#include <view/IRepresentable.h>
#include <model/Vector2.h>

//Constantes para definir la direccion. Faltan los laterales.
const int PERSONAJE_DERECHA = 0;
const int PERSONAJE_IZQUIERDA = 1;
const int PERSONAJE_ARRIBA = 2;
const int PERSONAJE_ABAJO = 3;
const int PERSONAJE_ABAJODER = 4;
const int PERSONAJE_ABAJOIZQ = 5;
const int PERSONAJE_ARRIBADER = 7;
const int PERSONAJE_ARRIBAIZQ = 6;

class Personaje : public IRepresentable{
public:
	void MoveTo(int x , int y);
	Personaje();
	void SetPos(float x, float y);
	void Update();
	Vector2* GetCurrentPos();
	Vector2* GetMovementDirection();
	bool IsMoving();
	virtual ~Personaje();
private:
	Vector2* currentPos;
	Vector2* endPos;
	float velocity;
};

#endif	/* PERSONAJE_H */

