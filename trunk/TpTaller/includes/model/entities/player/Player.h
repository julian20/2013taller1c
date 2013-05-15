/* 
 * File:   Personaje.h
 * Author: damian
 *
 * Created on 23 de marzo de 2013, 12:13
 */
#ifndef PERSONAJE_H
#define	PERSONAJE_H

#include <stdio.h>
#include <iostream>
#include <list>
#include <model/map/Tile.h>
#include <model/Chat.h>
#include <model/entities/Entity.h>
#include <model/Vector2.h>	// Capas hay q volarlo
#include <model/Vector3.h>
#include <networking/PlayerUpdate.h>
#include <networking/ChatUpdate.h>

using namespace std;

class Player: public Entity {
public:
	Player();
	Player(std::string name, Position* position, Speed* speed,
			std::vector<Power*> powers);
	virtual ~Player();
	void moveTo(int x, int y, int z = 0);
	void setPos(float x, float y, float z = 0);
	void update();
	void update(PlayerUpdate* update);
	PlayerUpdate* generatePlayerUpdate();
	void moveImmediately(Coordinates coords);
	Vector2* getMovementDirection();
	bool IsMoving();
	void setPosition(Position* position);
	Speed* getSpeed();
	void setSpeed(Speed* speed);
	void setInitSpeed(Speed* initSpeed);
	std::string getName();
	void setName(std::string name);
	std::vector<Power*> getPowers();
	void setPowers(std::vector<Power*> powers);
	void setViewRange(int _viewRange);
	int getViewRange();
	void setTile(Tile* _tile);
	Tile* getTile();
	void assignPath(list<Tile *> *_path);
	bool isRunning();

	bool isMainPlayer();
	void setAsMainPlayer();

	void attack();
	void setAttack(bool attacking);
	bool isAttacking();
	void cancelAttack();

	void block();
	void setBlock(bool blocking);
	void cancelBlock();
	bool isBlocking();

	bool playerIsActive();
	void setActive();
	void SetUnactive();

	void setChange(bool change);
	bool getChange();

	void stop();
	Vector3* getEndPos();
	void setEndPos(float x, float y, float z);

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const Player&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, Player&);

	Player& operator=(const Player &other);

	void setSpeedMagnitude(int mag);
	string getClassName();
	Chat* getChat();
	ChatUpdate* generateChatUpdate();
private:
	Chat* chat;
	void loadNextPosition();
	void emptyPath();

	int viewRange;
	bool mainPlayer;
	Vector3* endPos;
	Speed* speed;
	Speed* initSpeed;
	std::vector<Power*> powers;
	std::string name;
	Tile* currentTile;
	list<Tile *> *path;

	bool attacking;
	bool blocking;

	bool isActive;

	bool hasChange;
};

#endif	/* PERSONAJE_H */

