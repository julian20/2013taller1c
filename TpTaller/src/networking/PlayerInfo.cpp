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

	name = originalName = string("DEFAULT");

	imageWidth = 0;
	imageHeight = 0;
	anchorPixel = new Vector2(0, 0);
	fps = 0;
	delay = 0;

	player = NULL;
	initCoords = NULL;
}

void PlayerInfo::setName(string name) {
	this->name = name;
}

string PlayerInfo::getName() {
	return this->name;
}

void PlayerInfo::setOriginalName(string name){
	this->originalName = name;
}
string PlayerInfo::getOriginalName(){
	return this->originalName;
}

void PlayerInfo::setImages(map<string,string> images){
	this->images = images;
}
map<string,string> PlayerInfo::getImages(){
	return this->images;
}

void PlayerInfo::setImageDimentions(int width, int height) {
	this->imageHeight = height;
	this->imageWidth = width;
}
int PlayerInfo::getImageWidth() {
	return this->imageWidth;
}
int PlayerInfo::getImageHeight() {
	return this->imageHeight;
}

void PlayerInfo::setAnchorPixel(float x, float y) {
	this->anchorPixel->setValues(x, y);
}
void PlayerInfo::setAnchorPixel(Vector2* anchorPixel) {
	*(this->anchorPixel) = *anchorPixel;
}
Vector2* PlayerInfo::getAnchorPixel() {
	return this->anchorPixel;
}

void PlayerInfo::setInitCoordinates(Coordinates* coords) {
	this->initCoords = coords;
}
Coordinates* PlayerInfo::getInitCoordinates() {
	return this->initCoords;
}

void PlayerInfo::setPlayer(Player* player) {
	this->player = player;
}
Player* PlayerInfo::getPlayer() {
	return this->player;
}

void PlayerInfo::setDelay(float delay) {
	this->delay = delay;
}
float PlayerInfo::getDelay() {
	return this->delay;

}

void PlayerInfo::setFPS(int fps) {
	this->fps = fps;
}
int PlayerInfo::getFPS() {
	return this->fps;
}

PlayerView* PlayerInfo::createPlayerView() {
	PlayerView* view = new PlayerView();
	Player* player = this->getPlayer();
	view->setName(originalName);
	view->setShowableName(player->getName());
	view->setAnchorPixel(this->getAnchorPixel());
	view->setBaseSizes(player->getBase()->getRows(),
			player->getBase()->getCols());
	view->setDelay(this->getDelay());
	view->setFps(this->getFPS());
	view->setImageHeight(this->getImageHeight());
	view->setImageWidth(this->getImageWidth());
	view->setPersonaje(player);

	TextureHolder* th = new TextureHolder();

	for (map<string,string>::iterator it = images.begin() ; it != images.end() ; ++it){
		th->addTexture(new TextureDefinition(it->first,it->second));
	}

	view->setTextureHolder(th);

	return view;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const PlayerInfo& info) {
	string name = info.name;

	int imageWidth = info.imageWidth;
	int imageHeight = info.imageHeight;
	Vector2* anchorPixel = info.anchorPixel;
	int fps = info.fps;
	float delay = info.delay;

	Player* player = info.player;
	Coordinates* initCoords = info.initCoords;

	out << name << " " << info.originalName << " " << imageWidth << " " << imageHeight << " "
			<< anchorPixel->getX() << " " << anchorPixel->getY() << " " << fps
			<< " " << delay << " " << *initCoords << " " << *player << " ";

	out << info.images.size() << " ";
	map<string,string> images = info.images;
	for (map<string,string>::iterator it = images.begin() ; it != images.end() ; ++it){
		out << it->first << " " << it->second << " ";
	}

	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, PlayerInfo& info) {
	float x, y;

	string name;
	int imageWidth;
	int imageHeight;
	int fps;
	float delay;
	Player* player = new Player();
	Coordinates* initCoords = new Coordinates();
	in >> name;
	info.setName(name);
	in >> info.originalName;

	in >> imageWidth;
	in >> imageHeight;
	info.setImageDimentions(imageWidth, imageHeight);

	in >> x;
	in >> y;
	info.setAnchorPixel(x, y);

	in >> fps;
	in >> delay;
	info.setFPS(fps);
	info.setDelay(delay);

	in >> *initCoords;
	info.setInitCoordinates(initCoords);

	in >> *player;
	info.setPlayer(player);

	int size;
	in >> size;
	for (int i = 0 ; i < size ; i++){
		string id;
		string imagePath;
		in >> id;
		in >> imagePath;
		info.images.insert(pair<string,string>(id,imagePath));
	}

	return in;
}

void PlayerInfo::updatePlayer(PlayerUpdate* update) {
	Player* player = this->getPlayer();
	player->setCoordinates(update->getInitCoordinates()->getRow(),
			update->getInitCoordinates()->getCol());
	player->setSpeed(update->getSpeed());
	player->setPos(update->getCurrentPos()->getX(),
			update->getCurrentPos()->getY(), update->getCurrentPos()->getZ());
	player->setEndPos(update->getEndPos()->getX(), update->getEndPos()->getY(),
			update->getEndPos()->getZ());
	player->setAttack(update->isAttacking());
	player->setBlock(update->isBlocking());
	player->setTile(update->getTile());
	player->setLife(update->getLife());
	player->setMagic(update->getMagic());
}

PlayerInfo::~PlayerInfo() {
	if (anchorPixel)
		delete anchorPixel;
	if (player)
		delete player;
	if (initCoords)
		delete initCoords;
}

