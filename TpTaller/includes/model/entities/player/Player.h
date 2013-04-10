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

class Player: public Entity {
public:
	void MoveTo(int x, int y);
	Player();
	Player(std::string name, Position* position, Speed* speed,
			std::vector<Power*> powers);
	void setPos(float x, float y);
	void Update();
	Vector2* GetCurrentPos();
	Vector2* GetMovementDirection();
	bool IsMoving();
	virtual ~Player();
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
	void setSpeedMagnitude(int mag);
	string getClassName();
private:
	void setNextPosition();

	Vector2* endPos;
	Position* position;
	Speed* speed;
	std::vector<Power*> powers;
	std::string name;
	Tile* currentTile;
	list<Tile *> *path;
};

#endif	/* PERSONAJE_H */

