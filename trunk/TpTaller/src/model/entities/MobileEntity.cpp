/*
 * MobileEntity.cpp
 *
 *  Created on: May 25, 2013
 *      Author: mint
 */

#include <model/entities/MobileEntity.h>
#include <model/entities/Items/Item.h>
#include <model/entities/Items/LifeHeart.h>
#include <model/map/MapData.h>

using namespace std;

MobileEntity::MobileEntity() :
		Entity() {
	speed = new Speed();
	initSpeed = new Speed();
	hasChanged = true;
	attacking = false;
	endPos = new Vector3(0, 0);
	team = 0;
	attackToEntity = NULL;
//	ia = ArtificialIntelligence();
	this->speed = new Speed(0, new Vector2(0, 0));
	this->initSpeed = NULL;
	this->path = new list<Tile *>();
	this->currentTile = new Tile(new Coordinates(0, 0));
	attackTimer.start();
	lastAttackingDirection = 0;
	viewRange = 200;
	magicDamageDelay.start();
	frozen = false;
	walkable = false;
}

MobileEntity::MobileEntity(string name, Position* position, Speed* speed) {
	this->speed = speed;
	this->name = name;
	this->path = new list<Tile *>();
	currentTile = new Tile(new Coordinates(0, 0));
	this->currentPos = new Vector3(0, 0, 0);
	this->base = new Base();
	attackToEntity = NULL;
	endPos = new Vector3(0, 0, 0);
	endPos->setValues(currentPos->getX(), currentPos->getY());
	attacking = false;
	hasChanged = false;
	initSpeed = 0;
	team = 0;
	attackTimer.start();
	lastAttackingDirection = 0;
	viewRange = 200;
	magicDamageDelay.start();
	frozen = false;
	walkable = false;
}

list<PlayerEvent*> MobileEntity::getPlayerEvents() {
	list<PlayerEvent*> aux = events;
	events.clear();
	return aux;
}

void MobileEntity::addEvent(PlayerEvent* event) {
	events.push_back(event);
}

void MobileEntity::setPos(Position* pos) {
	setPos(pos->getX(), pos->getY(), pos->getZ());

	delete pos;
}

void MobileEntity::setPos(float x, float y, float z) {
	currentPos->setValues(x, y, z);

	endPos->setValues(currentPos->getX(), currentPos->getY());
}

void MobileEntity::moveTo(int x, int y, int z) {
	if (endPos)
		delete endPos;
	endPos = new Vector3(x, y, z);
}

bool MobileEntity::IsMoving() {
	return !(currentPos->isEqual(endPos));
}

string MobileEntity::getClassName() {
	return "MobileEntity";
}

bool MobileEntity::isRunning() {
	if (speed->getMagnitude() >= 2 * initSpeed->getMagnitude())
		return true;
	return false;
}

float MobileEntity::getLastAttackingDirecton() {
	return lastAttackingDirection;
}

void MobileEntity::moveImmediately(Coordinates coords) {
	Coordinates* newCoords = new Coordinates(coords);
	Tile* tile = new Tile(newCoords);
	Position* pos = tile->getPosition();

	currentPos->setValues(pos->getX(), pos->getY(), pos->getZ());
	endPos->setValues(pos->getX(), pos->getY(), pos->getZ());
	delete tile;

	this->hasChanged = true;
}

void MobileEntity::lookAtEnemy() {
	Vector3* enemyPos = attackToEntity->getCurrentPos();

	Vector3 attackerDirection;
	attackerDirection.setValues(enemyPos->getX() - currentPos->getX(),
			enemyPos->getY() - currentPos->getY(),
			enemyPos->getZ() - currentPos->getZ());

	lastAttackingDirection = attackerDirection.getAngle();
}

void MobileEntity::checkAttackToNewPos(MapData* mapData) {
	Tile* enemyTile = attackToEntity->getTile();
	if (currentTile->isEqual(enemyTile))
		return;

	//Llego hasta el player
	if (MapData::distBetweenTilesInTiles(currentTile, enemyTile) <= getAttackingDistance()) {
		delete enemyTile;
		MobileEntity& thisMobileEntity = *this;
		thisMobileEntity.reverseCollide(*attackToEntity);
		lookAtEnemy();
		//	cancelAttack();
		attackToEntity = NULL;
		return;
	}

	if (path->size() == 0) {
		assignPath(mapData->getPath(currentTile, enemyTile), mapData);
		delete enemyTile;
		return;
	}

	Tile* lastTile = path->back();

	if (lastTile->isNeighbor(enemyTile)) {
		cancelAttack();
		delete enemyTile;
		return;
	}

	assignPath(mapData->getPath(currentTile, enemyTile), mapData);
	delete enemyTile;
}

void MobileEntity::extraUpdate(MapData* mapData) {
	// Se overraidea en player
}

void MobileEntity::update(MapData* mapData) {

	if (frozen) {
		if (frozenTimer.getTimeIntervalSinceStart() > FROZEN_TIMEOUT)
			frozen = false;
		else
			return;
	}
	extraUpdate(mapData);

	if (attackToEntity != NULL)
		checkAttackToNewPos(mapData);
	if (IsMoving() == false) {
		if (path->size() == 0)
			return;
		else
			loadNextPosition(mapData);
	}

	float relationSpeed = ((float) Tile::getTileHeight())
			/ ((float) Tile::getTileWidth());

	Vector3* moveDirection = new Vector3(endPos->getX() - currentPos->getX(),
			endPos->getY() - currentPos->getY(),
			endPos->getZ() - currentPos->getZ());

	if (moveDirection->getNorm() < getSpeed()->getMagnitude() + 1) {
		// Close enough to the end position to move in one step.
		currentPos->setValues(endPos->getX(), endPos->getY());
		loadNextPosition(mapData);
	} else {
		moveDirection->normalize();
		moveDirection->multiplyBy(
				fabs(moveDirection->getY()) * (relationSpeed - 1) + 1);
		moveDirection->multiplyBy(getSpeed()->getMagnitude());
		currentPos->add(moveDirection);
	}
}

void MobileEntity::localUpdate(MapData* mapData) {
	if (frozen)
		return;
	extraUpdate(mapData);

	if (attackToEntity != NULL)
		checkAttackToNewPos(mapData);
	if (IsMoving() == false) {
		if (path->size() == 0)
			return;
		else
			loadNextPosition(mapData);
	}

	float relationSpeed = ((float) Tile::getTileHeight())
			/ ((float) Tile::getTileWidth());

	Vector3* moveDirection = new Vector3(endPos->getX() - currentPos->getX(),
			endPos->getY() - currentPos->getY(),
			endPos->getZ() - currentPos->getZ());

	if (moveDirection->getNorm() < getSpeed()->getMagnitude() + 1) {
		// Close enough to the end position to move in one step.
		currentPos->setValues(endPos->getX(), endPos->getY());
		loadNextPosition(mapData);
	} else {
		moveDirection->normalize();
		moveDirection->multiplyBy(
				fabs(moveDirection->getY()) * (relationSpeed - 1) + 1);
		moveDirection->multiplyBy(getSpeed()->getMagnitude());
		currentPos->add(moveDirection);
	}
}


void MobileEntity::updateFromServer(MobileEntityUpdate* update) {

	this->currentPos->setValues(update->getCurrentPos()->getX(),
			update->getCurrentPos()->getY(), update->getCurrentPos()->getZ());
	this->endPos->setValues(update->getEndPos()->getX(),
			update->getEndPos()->getY(), update->getEndPos()->getZ());
	this->speed->setMagnitude(update->getSpeed()->getMagnitude());
	this->speed->setDirection(update->getSpeed()->getDirection());
	this->attacking = update->isAttacking();
	this->currentTile = update->getTile();
	this->lastAttackingDirection = update->getLastAttackingDirection();
	this->viewRange = update->getViewRange();
	this->frozen = update->getFrozen();
	Coordinates currentTileCoords = this->currentTile->getCoordinates();
	Coordinates nextTileCoords = update->getNextTile()->getCoordinates();
	if ((!currentTileCoords.isEqual(nextTileCoords)) && (this->path->empty())) {
		this->path->push_front(update->getNextTile());
	}
	this->life = update->getLife();
	this->team = update->getTeam();

}

MobileEntityUpdate* MobileEntity::generateMobileEntityUpdate(int id) {

	if (!this->hasChanged)
		return NULL;

	MobileEntityUpdate* update = new MobileEntityUpdate();

	update->setId(id);
	update->setName(this->name);
	update->setCurrentPos(this->currentPos);
	update->setEndPos(this->endPos);
	update->setSpeed(this->speed);
	update->setAttacking(this->attacking);
	update->setTile(this->currentTile);
	update->setInitCoordinates(this->coord);
	update->setLastAttackingDirection(this->lastAttackingDirection);
	update->setLife(this->life);
	update->setMagic(this->magic);
	update->setTeam(this->team);
	update->setViewRange(this->viewRange);
	update->setFrozen(frozen);
	if (!this->path->empty()) {
		update->setNextTile(this->path->front());
	} else {
		update->setNextTile(this->currentTile);
	}

	return update;
}

bool MobileEntity::isFrozen() {
	return frozen;
}

void MobileEntity::loadNextPosition(MapData* mapData, bool checkColition) {
	if (path->empty())
		return;
	Tile* tile = path->front();

	if (checkColition) {
		if (! mapData->getTileData(tile->getCoordinates())->isWalkable()) {
			if (attackToEntity != NULL) {
				Tile* enemyTile = attackToEntity->getTile();
				assignPath(mapData->getPath(currentTile, enemyTile), mapData);
				delete enemyTile;
			} else {
				Tile* lastTile = path->back();
				assignPath(mapData->getPath(currentTile, lastTile), mapData);
			}
			return;
		}
	}

	path->remove(tile);

	Position* tilePos = tile->getPosition();
	moveTo(tilePos->getX(), tilePos->getY());

	int prevRow = currentTile->getCoordinates().getRow();
	int prevCol = currentTile->getCoordinates().getCol();

	setTile(tile);

	int row = currentTile->getCoordinates().getRow();
	int col = currentTile->getCoordinates().getCol();

	mapData->updateMobilePos(prevRow, prevCol, row, col, this);
}

Vector2* MobileEntity::getMovementDirection() {
	Vector2* moveDirection = new Vector2(endPos->getX() - currentPos->getX(),
			endPos->getY() - currentPos->getY());

	if (moveDirection->getNorm() == 0) {
		return moveDirection;
	}

	moveDirection->normalize();

	return moveDirection;
}

MobileEntity::~MobileEntity() {
	delete currentPos;
	delete endPos;
	delete this->speed;
	if (initSpeed != NULL)
		delete initSpeed;
	if (currentTile)
		delete currentTile;
}

void MobileEntity::setPosition(Position* position) {
	Position* pos = Tile::computePosition(position->getX(), position->getY());
	currentPos->setValues(pos->getX(), pos->getY());
	delete pos;
}

Speed* MobileEntity::getSpeed() {
	return speed;
}

void MobileEntity::setSpeed(Speed* speed) {
	if (this->speed)
		delete this->speed;
	this->speed = speed;
	if (initSpeed == NULL)
		initSpeed = new Speed(speed->getMagnitude(), new Vector2(0, 0));
}

void MobileEntity::setInitSpeed(Speed* initSpeed) {
	this->initSpeed = initSpeed;
}

void MobileEntity::setInitSpeedMagnitude(int _initSpeed) {
	initSpeed->setMagnitude(_initSpeed);
	if (initSpeed->getMagnitude() > MAXSPEED)
		initSpeed->setMagnitude(MAXSPEED);
}

Speed* MobileEntity::getInitSpeed() {
	return this->initSpeed;
}

void MobileEntity::assignPath(list<Tile *> *_path, MapData* mapData) {
	if (path) {
		path->erase(path->begin(), path->end());
		delete path;
	}
	this->path = _path;

	loadNextPosition(mapData, false);
	this->hasChanged = true;
}

void MobileEntity::froze() {
	frozen = true;
	frozenTimer.start();
	this->hasChanged = true;
}

void MobileEntity::attackTo(Entity* attackTo) {
	// TODO: Por ahora solo se banca entities de base 1x1
	attackToEntity = attackTo;
}

Entity* MobileEntity::getAttackToEntity() {
	return attackToEntity;
}

void MobileEntity::setSpeedMagnitude(int mag) {
	speed->setMagnitude(mag);
	if (speed->getMagnitude() > MAXSPEED)
		speed->setMagnitude(MAXSPEED);
}

bool MobileEntity::isAttacking() {
	return attacking;
}

void MobileEntity::setAttack(bool attacking) {
	this->attacking = attacking;
}

void MobileEntity::cancelAttack() {
	attacking = false;
	attackToEntity = NULL;
	hasChanged = true;
}

void MobileEntity::emptyPath() {
	std::list<Tile *>::iterator iter;
	for (iter = path->begin(); iter != path->end(); ++iter) {
		Tile* tile = *iter;

		delete tile;
	}

	delete path;
	this->path = new list<Tile *>();
}

void MobileEntity::stop() {
	emptyPath();
}

void MobileEntity::collideTo(Entity& entity) {
	attack(entity);
}

void MobileEntity::reverseCollide(Entity& entity) {
	entity.collideTo(*this);
}

void MobileEntity::attack(Entity& entity) {
	if (attackTimer.getTimeIntervalSinceStart() > ATTACK_TIMEOUT) {
		attacking = true;
		attackTimer.start();
	}
}

MobileEntity& MobileEntity::operator=(const MobileEntity &other) {

	this->name = other.name;
	*(this->currentPos) = *(other.currentPos);
	*(this->endPos) = *(other.endPos);
	*(this->speed) = *(other.speed);
	*(this->initSpeed) = *(other.initSpeed);
	*(this->base) = *(other.base);
	*(this->currentTile) = *(other.currentTile);
	*(this->path) = *(other.path);
	return *this;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const MobileEntity& MobileEntity) {
	out << MobileEntity.name << " " << *(MobileEntity.currentPos) << " "
			<< *(MobileEntity.endPos) << " " << *(MobileEntity.speed) << " "
			<< *(MobileEntity.initSpeed) << " " << *(MobileEntity.base) << " ";

	out << *(MobileEntity.currentTile);
	out << " " << MobileEntity.life << " " << MobileEntity.team;
	out << " " << MobileEntity.lastAttackingDirection;
	out << " " << MobileEntity.viewRange;
	out << " " << MobileEntity.frozen;

	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, MobileEntity& MobileEntity) {
	string name;
	in >> name;
	MobileEntity.setName(name);
	Vector3 pos;
	in >> pos;
	MobileEntity.setPos(pos.getX(), pos.getY(), pos.getZ());
	in >> pos;
	MobileEntity.moveTo(pos.getX(), pos.getY(), pos.getZ());
	Speed* speed = new Speed();
	in >> *speed;
	MobileEntity.setSpeed(speed);
	Speed* initSpeed = new Speed();
	in >> *initSpeed;
	MobileEntity.setInitSpeed(initSpeed);
	Base* base = new Base();
	in >> *base;
	MobileEntity.setBase(base);
	Tile* tile = new Tile();
	in >> *tile;
	MobileEntity.setTile(tile);
	int life;
	in >> life;
	MobileEntity.life = life;
	int team;
	in >> team;
	MobileEntity.team = team;
	float lastDir;
	in >> lastDir;
	MobileEntity.lastAttackingDirection = lastDir;
	int view;
	in >> view;
	MobileEntity.viewRange = view;
	bool froze;
	in >> froze;
	MobileEntity.frozen = froze;
	return in;
}

Vector3* MobileEntity::getEndPos() {
	return endPos;
}

void MobileEntity::setEndPos(float x, float y, float z) {
	endPos->setValues(x, y, z);
}

void MobileEntity::setHasChanged(bool change) {
	hasChanged = change;
}

bool MobileEntity::getHasChanged() {
	return hasChanged;
}

void MobileEntity::setViewRange(int _viewRange) {
	this->viewRange = _viewRange;
}

int MobileEntity::getViewRange() {
	return this->viewRange;
}

Tile* MobileEntity::getTile() {
	// Devuelve una copia del tile
	Tile* retval = new Tile();
	retval->setCoordinates(currentTile->getCoordinates());

	return retval;
}

int MobileEntity::getAttackingDistance() {
	return 1;
}
