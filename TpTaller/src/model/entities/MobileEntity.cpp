/*
 * MobileEntity.cpp
 *
 *  Created on: May 25, 2013
 *      Author: mint
 */

#include <model/entities/MobileEntity.h>
#include <model/entities/Items/Item.h>
#include <model/map/MapData.h>
using namespace std;

MobileEntity::MobileEntity() {
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
}

list<PlayerEvent*> MobileEntity::getPlayerEvents() {
	list<PlayerEvent*>aux =events;
	events.clear();
	return aux;
}

void MobileEntity::addEvent(PlayerEvent* event){
	events.push_back(event);
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

	//Llego hasta el player
	if (this->currentTile->isNeighbor(enemyTile)) {
		delete enemyTile;
		// Aca se hace una llamada a:
		// this->reverseCollide( attackToEntity );
		// y adentro de esa llamada se hace:
		// entity->collideTo( this );
		// Sino nunca va a cambiar el tipo de clase de lo que colisionamos.
		// Solo vamos a poder colisionar con Entity, y nunca podremos
		// diferenciar si es un player, un item, o que.
		Entity& attackReference = *attackToEntity;
		this->reverseCollide(attackReference);
		lookAtEnemy();
//		cancelAttack();
		attackToEntity = NULL;
		return;
	}

	if (path->size() == 0) {
		assignPath(mapData->getPath(currentTile, enemyTile));
		delete enemyTile;
		return;
	}

	Tile* lastTile = path->back();

	if (lastTile->isNeighbor(enemyTile)) {
		delete enemyTile;
		return;
	}

	assignPath(mapData->getPath(currentTile, enemyTile));
	delete enemyTile;
}

void MobileEntity::update(MapData* mapData) {
	if (attackToEntity != NULL)
		checkAttackToNewPos(mapData);
	if (IsMoving() == false) {
		if (path->size() == 0)
			return;
		else
			loadNextPosition();
	}

	float relationSpeed = ((float) Tile::getTileHeight())
			/ ((float) Tile::getTileWidth());

	Vector3* moveDirection = new Vector3(endPos->getX() - currentPos->getX(),
			endPos->getY() - currentPos->getY(),
			endPos->getZ() - currentPos->getZ());

	if (moveDirection->getNorm() < getSpeed()->getMagnitude() + 1) {
		// Close enough to the end position to move in one step.
		currentPos->setValues(endPos->getX(), endPos->getY());
		loadNextPosition();
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

	Coordinates currentTileCoords = this->currentTile->getCoordinates();
	Coordinates nextTileCoords = update->getNextTile()->getCoordinates();
	if ((!currentTileCoords.isEqual(nextTileCoords)) && (this->path->empty())) {
		this->path->push_front(update->getNextTile());
	}

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
	if (!this->path->empty()) {
		update->setNextTile(this->path->front());
	} else {
		update->setNextTile(this->currentTile);
	}

	return update;
}

void MobileEntity::loadNextPosition() {
	if (path->empty())
		return;
	Tile* tile = path->front();
	path->remove(tile);

	Position* tilePos = tile->getPosition();
	moveTo(tilePos->getX(), tilePos->getY());

	setTile(tile);
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

void MobileEntity::assignPath(list<Tile *> *_path) {
	if (path) {
		path->erase(path->begin(), path->end());
		delete path;
	}
	this->path = _path;

	loadNextPosition();
	this->hasChanged = true;
}

void MobileEntity::attackTo(Entity* attackTo) {
	// TODO: Por ahora solo se banca entities de base 1x1
	attackToEntity = attackTo;
}

void MobileEntity::setSpeedMagnitude(int mag) {
	speed->setMagnitude(mag);
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

void MobileEntity::collideTo(Entity& entity){
	attack(entity);
}

void MobileEntity::reverseCollide(Entity& entity){
	entity.collideTo(*this);
}

void MobileEntity::reverseCollide(Item& item){
	item.collideTo(*this);
}

void MobileEntity::attack(Entity& entity){
	if (attackTimer.getTimeIntervalSinceStart()>ATTACK_TIMEOUT){
		attacking = true;
		attackTimer.start();
	}

	cout<<"mobile attack"<<endl;
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

Tile* MobileEntity::getTile() {
	// Devuelve una copia del tile
	Tile* retval = new Tile();
	retval->setCoordinates(currentTile->getCoordinates());

	return retval;
}

