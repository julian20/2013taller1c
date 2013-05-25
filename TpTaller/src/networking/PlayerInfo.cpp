/*
 * PlayerInfo.cpp
 *
 *  Created on: 23/04/2013
 *      Author: tomas
 */

#include <sstream>

#include <networking/PlayerInfo.h>
#include <model/map/TextureHolder.h>
#include <model/map/TextureDefinition.h>

using namespace std;

PlayerInfo::PlayerInfo() {

	name = string("DEFAULT");
	walkingImageSrc = string("DEFAULT");
	runningImageSrc = string("DEFAULT");
	idleImageSrc = string("DEFAULT");
	attackImageSrc = string("DEFAULT");
	idleBlockingImageScr = string("DEFAULT");

	imageWidth = 0;
	imageHeight = 0;
	anchorPixel = new Vector2(0,0);
}

void PlayerInfo::setName(string name){
	this->name = string(name);
}
string PlayerInfo::getName(){
	return this->name;
}

void PlayerInfo::setWalkingImageSrc(string img){
	this->walkingImageSrc.assign(img);
}
string PlayerInfo::getWalkingImageSrc(){
	return this->walkingImageSrc;
}

void PlayerInfo::setRunningImageSrc(string img){
	this->runningImageSrc.assign(img);
}
string PlayerInfo::getRunningImageSrc(){
	return this->runningImageSrc;
}

void PlayerInfo::setIdleImageSrc(string img){
	this->idleImageSrc.assign(img);
}
string PlayerInfo::getIdleImageSrc(){
	return this->idleImageSrc;
}

void PlayerInfo::setAttackImageSrc(string img){
	this->attackImageSrc.assign(img);
}
string PlayerInfo::getAttackImageSrc(){
	return this->attackImageSrc;
}

void PlayerInfo::setIdleBlockingImageSrc(string img){
	this->idleBlockingImageScr.assign(img);
}
string PlayerInfo::getIdleBlockingImageSrc(){
	return this->idleBlockingImageScr;
}

void PlayerInfo::setImageDimentions(int width, int height){
	this->imageHeight = height;
	this->imageWidth = width;
}
int PlayerInfo::getImageWidth(){
	return this->imageWidth;
}
int PlayerInfo::getImageHeight(){
	return this->imageHeight;
}

void PlayerInfo::setAnchorPixel(float x, float y){
	this->anchorPixel->setValues(x,y);
}
void PlayerInfo::setAnchorPixel(Vector2* anchorPixel){
	*(this->anchorPixel) = *anchorPixel;
}
Vector2* PlayerInfo::getAnchorPixel(){
	return this->anchorPixel;
}

void PlayerInfo::setInitCoordinates(Coordinates* coords){
	this->initCoords = coords;
}
Coordinates* PlayerInfo::getInitCoordinates(){
	return this->initCoords;
}

void PlayerInfo::setPlayer(Player* player){
	this->player = player;
}
Player* PlayerInfo::getPlayer(){
	return this->player;
}

void PlayerInfo::setDelay(int delay){
	this->delay = delay;
}
int PlayerInfo::getDelay(){
	return this->delay;

}

void PlayerInfo::setFPS(int fps){
	this->fps = fps;
}
int PlayerInfo::getFPS(){
	return this->fps;
}

PlayerView* PlayerInfo::createPlayerView(){
	PlayerView* view = new PlayerView();
	Player* player = this->getPlayer();
	string playerName = player->getName();
	view->setName(playerName);
	view->setAnchorPixel(this->getAnchorPixel());
	view->setBaseSizes(player->getBase()->getRows(),player->getBase()->getCols());
	view->setDelay(this->getDelay());
	view->setFps(this->getFPS());
	view->setImageHeight(this->getImageHeight());
	view->setImageWidth(this->getImageWidth());
	view->setPersonaje(player);

	string walking = this->getWalkingImageSrc();
	string running = this->getRunningImageSrc();
	string attack = this->getAttackImageSrc();
	string idle = this->getIdleImageSrc();
	string idle_blocking = this->getIdleBlockingImageSrc();

	TextureHolder* th = new TextureHolder();
	th->addTexture(new TextureDefinition(playerName + string(WALKING_MODIFIER), walking ));
	th->addTexture(new TextureDefinition(playerName + string(RUNNING_MODIFIER), running));
	th->addTexture(new TextureDefinition(playerName + string(ATTACK_MODIFIER), attack ));
	th->addTexture(new TextureDefinition(playerName + string(IDLE_MODIFIER), idle ));
	th->addTexture(new TextureDefinition(playerName + string(IDLE_BLOCKING_MODIFIER), idle_blocking ));

	view->setTextureHolder(th);

	return view;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out , const PlayerInfo& info){
	string name = info.name;
	string walkingImageSrc = info.walkingImageSrc;
	string runningImageSrc = info.runningImageSrc;
	string idleImageSrc = info.idleImageSrc;
	string attackImageSrc = info.attackImageSrc;
	string idleBlockingImageSrc = info.idleBlockingImageScr;
	int imageWidth = info.imageWidth;
	int imageHeight = info.imageHeight;
	Vector2* anchorPixel = info.anchorPixel;
	int fps = info.fps;
	int delay = info.delay;

	Player* player = info.player;
	Coordinates* initCoords = info.initCoords;

	out << name << " " << walkingImageSrc << " " << runningImageSrc << " " <<
			idleImageSrc << " " << attackImageSrc << " " << idleBlockingImageSrc<< " " << imageWidth << " " << imageHeight << " " <<
			anchorPixel->getX() << " " << anchorPixel->getY() << " " << fps << " " << delay << " " << *initCoords << " "
			<< *player;
	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in , PlayerInfo& info){
	float x, y;

	string name;
	string walkingImageSrc;
	string runningImageSrc;
	string idleImageSrc;
	string attackImageSrc;
	string idleBlockingImageSrc;
	int imageWidth;
	int imageHeight;
	int delay,fps;
	Player* player = new Player();
	Coordinates* initCoords = new Coordinates();
	in >> name;
	info.setName(name);
	in >> walkingImageSrc;
	info.setWalkingImageSrc(walkingImageSrc);
	in >> runningImageSrc;
	info.setRunningImageSrc(runningImageSrc);
	in >> idleImageSrc;
	info.setIdleImageSrc(idleImageSrc);
	in >> attackImageSrc;
	info.setAttackImageSrc(attackImageSrc);
	in >> idleBlockingImageSrc;
	info.setIdleBlockingImageSrc(idleBlockingImageSrc);

	in >> imageWidth;
	in >> imageHeight;
	info.setImageDimentions(imageWidth,imageHeight);

	in >> x;
	in >> y;
	info.setAnchorPixel(x,y);

	in >> fps;
	in >> delay;
	info.setFPS(fps);
	info.setDelay(delay);

	in >> *initCoords;
	info.setInitCoordinates(initCoords);

	in >> *player;
	info.setPlayer(player);

	return in;
}

void PlayerInfo::updatePlayer(PlayerUpdate* update){
	Player* player = this->getPlayer();
	player->setCoordinates(update->getInitCoordinates()->getRow(),update->getInitCoordinates()->getCol());
	player->setSpeed(update->getSpeed());
	player->setPos(update->getCurrentPos()->getX(),update->getCurrentPos()->getY(),update->getCurrentPos()->getZ());
	player->setEndPos(update->getEndPos()->getX(),update->getEndPos()->getY(), update->getEndPos()->getZ());
	player->setAttack(update->isAttacking());
	player->setBlock(update->isBlocking());
	player->setTile(update->getTile());
}

PlayerInfo::~PlayerInfo() {
	if (anchorPixel) delete anchorPixel;
	if (player) delete player;
	if (initCoords) delete initCoords;
}

