/* 
 * File:   Personaje.h
 * Author: damian
 *
 * Created on 23 de marzo de 2013, 12:13
 */
#ifndef PERSONAJE_H
#define	PERSONAJE_H

#include <stdio.h>
#include <list>
#include <model/map/Tile.h>
#include <model/entities/Entity.h>
#include <model/Vector2.h>

using namespace std;

//Constantes para definir la direccion. Faltan los laterales.
const int PERSONAJE_DERECHA = 0;
const int PERSONAJE_IZQUIERDA = 1;
const int PERSONAJE_ARRIBA = 2;
const int PERSONAJE_ABAJO = 3;
const int PERSONAJE_ABAJODER = 4;
const int PERSONAJE_ABAJOIZQ = 5;
const int PERSONAJE_ARRIBADER = 7;
const int PERSONAJE_ARRIBAIZQ = 6;

class Personaje: public Entity {
public:
	void MoveTo(int x, int y);
	Personaje();
	Personaje(std::string name, Position* position, Speed* speed,
			std::vector<Power*> powers);
	void setPos(float x, float y);
	void Update();
	Vector2* GetCurrentPos();
	Vector2* GetMovementDirection();
	bool IsMoving();
	virtual ~Personaje();
	Position* getPosition();
	void setPosition(Position* position);
	Speed* getSpeed();
	void setSpeed(Speed* speed);
	std::string getName();
	void setName(std::string name);
	std::vector<Power*> getPowers();
	void setPowers(std::vector<Power*> powers);
	void setTile( Tile* _tile );
	Tile* getTile();
	void assignPath(list<Tile *> *_path);
	bool isRunning();

	string getClassName();
private:
	void setNextPosition();

	Vector2* endPos;
	float velocity;
	Position* position;
	Speed* speed;
	std::vector<Power*> powers;
	std::string name;
	Tile* currentTile;
	list<Tile *> *path;
};

#endif	/* PERSONAJE_H */

