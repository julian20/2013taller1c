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
#include <queue>
#include <model/map/Tile.h>
#include <model/entities/Entity.h>
#include <model/Chat.h>
#include <model/entities/MobileEntity.h>
#include <model/Vector2.h>	// Capas hay q volarlo
#include <model/Vector3.h>
#include <networking/PlayerUpdate.h>
#include <networking/ChatUpdate.h>
#include <model/Weapons/Weapon.h>
#include <model/Weapons/Sword.h>
#include <model/Weapons/FrostWandWeapon.h>
#include <model/entities/Items/Item.h>
#include <model/entities/Items/LifeHeart.h>
#include <model/entities/spells/SpellEffect.h>
#include <model/entities/Golem/Golem.h>


#define QUAKE_SPELL_ID "quake"
#define NONE_SPELL_ID "none"
#define RESPAWN_TIMEOUT 15000
#define CRYSTALBALL_MANA	3
#define SHIELD_MANA	5
#define EARTHQUAKE_TIMEOUT 2000
#define EARTHQUAKE_RADIUS 10	// Tiles
#define EARTHQUAKE_DAMAGE 25
#define FROZE_RADIUS	8

using namespace std;

class Inventory {
public:
	bool earthquake;
	bool crystalBall;
	bool shieldSpell;
	bool map, mapUsed;
	bool freeze;
	bool golemSpellItem;
};

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

	void extraUpdate(MapData* mapData);
	bool isMainPlayer();
	void setAsMainPlayer();

	void makeEarthquake(MapData* mapData);
	void setUsingCrystalBall(bool usingCrystalBall);
	bool getUsingCrystalBall();
	void setMakingEarthquake(bool makingEarthquake);
	bool getMakingEarthquake();

	void setUsingShieldSpell(bool usingShieldSpell);
	bool getUsingShieldSpell();

	void frozeEnemies();

	void setCastingSpell(bool castingSpell);
	bool isCastingSpell();
	void castSpell();
	SpellEffect* getCurrentSpell();

	void addEarthquakeSpell();
	void addCrystalBallSpell();
	void addShieldSpell();
	void addMap();
	void addFreeze();
	void enableGolem();

	void block();
	void setBlock(bool blocking);
	void cancelBlock();
	bool isBlocking();

	bool playerIsActive();
	void setActive();
	void SetUnactive();

	void respawn();

	virtual void collideTo(Entity& entity);
	virtual void reverseCollide(Entity& entity);
	virtual void attack(Entity& entity);
	void setLife(int life);

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const Player&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, Player&);

	Player& operator=(const Player &other);

	Chat* getChat();
	void setChat(Chat* chat);
	ChatUpdate* generateChatUpdate();
	Weapon* getCurrentWeapon();
	void createGolem();
	bool hasGolem();
	void changeWeapon();
	void updateDamageTaken();
private:
	void createWeapons();
	void frozeEnemiesNow(MapData* mapData);
	void usingMagic();
	void initializeSpellsInventory();
	void castSpellNow(MapData* mapData);
	void setSpellDirection(SpellEffect* spell,
			Coordinates starting, Coordinates ending);

	Chat* chat;

	bool mainPlayer;

	std::vector<Power*> powers;
	list<Weapon*>* weapons;
	list<SpellEffect*> spellEffects;
	Inventory inventory;

	bool usingCrystalBall,usingInvulnerability;
	bool makingEarthquake;
	bool needFrozeEnemies;
	bool castingSpell;
	bool needCastSpell;
	bool blocking;
	bool isActive;

	Timer earthquakeTimer;
	bool earthquakeLifeTaked;
	bool golem;
	int golemCost;

	queue<Entity*> attackQueue;
};



#endif	/* PERSONAJE_H */

