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
#include <model/entities/MobileEntity.h>
#include <model/Vector2.h>	// Capas hay q volarlo
#include <model/Vector3.h>
#include <networking/PlayerUpdate.h>
#include <networking/ChatUpdate.h>

using namespace std;

class Player: public MobileEntity {
public:
	Player();
	Player(std::string name, Position* position, Speed* speed,
			std::vector<Power*> powers);
	virtual ~Player();
	string getClassName();

	void updateFromServer(PlayerUpdate* update);
	PlayerUpdate* generatePlayerUpdate();
	std::vector<Power*> getPowers();
	void setPowers(std::vector<Power*> powers);
	void setViewRange(int _viewRange);
	int getViewRange();

	bool isMainPlayer();
	void setAsMainPlayer();


	void block();
	void setBlock(bool blocking);
	void cancelBlock();
	bool isBlocking();

	bool playerIsActive();
	void setActive();
	void SetUnactive();

	void setChange(bool change);
	bool getChange();

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const Player&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, Player&);

	Player& operator=(const Player &other);

	Chat* getChat();
	void setChat(Chat* chat);
	ChatUpdate* generateChatUpdate();
private:
	Chat* chat;

	int viewRange;
	bool mainPlayer;

	std::vector<Power*> powers;


	bool blocking;

	bool isActive;

};

#endif	/* PERSONAJE_H */

