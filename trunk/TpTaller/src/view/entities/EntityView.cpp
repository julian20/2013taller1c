/* 
 * File:   PersonajeVista.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:25
 */

//#include <view/entities/PersonajeVista.h>
#include <view/entities/EntityView.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <cmath>
#include <string>
#include <SDL/SDL_rotozoom.h>
#include <model/Logs/Logs.h>

#define DELAY 3 //seconds
#define FPS 30
#define DEFAULT_IMAGE "resources/questionMark.png"

//the amount of times the animation is played before stopping
#define NUMBER_OF_REPEATS 4

using namespace std;

EntityView::EntityView() {
	this->baseHeight = 1;
	this->baseWidth = 1;
	this->scaleWidth = 1;
	this->scaleHeight = 1;
	this->tileHeight = 50;
	this->tileWidth = 70;
	this->imageHeight = 0;
	this->imageWidth = 0;
	this->entity = NULL;
	this->image = NULL;
	this->delay = 0;
	this->fps = 0;
	this->nClips = 1;
	this->currentClip = 0;
	this->anchorPixel = new Vector2(0, 0);
	this->movable = false;
	this->currentRepeat = 0;
	timer.start();
	animationRateTimer.start();

	timeSinceLastAnimation = 1000;

	setNumberOfRepeats(0);
}

bool EntityView::isMovable() {
	return movable;
}

void EntityView::setNumberOfRepeats(int repeats) {
	numberOfRepeats = repeats;
}

int EntityView::getNumberOfRepeats() {
	return this->numberOfRepeats;
}

void EntityView::setScale() {
//	baseHeight = entity->getBase()->getLength();
//	baseWidth = entity->getBase()->getWidth();
	this->scaleHeight = (float) (this->tileHeight) * float(this->baseHeight)
			/ (float) (this->imageHeight);
	this->scaleWidth = (float) (this->tileWidth) * (float) (this->baseWidth)
			/ (float) (this->imageWidth);
}

void EntityView::setImageWidth(int width) {
	this->imageWidth = width;
}

void EntityView::setNClips(int clips) {
	this->nClips = clips;
}

void EntityView::setTileWidth(int width) {
	this->tileWidth = width;
}
void EntityView::setTileHeight(int height) {
	this->tileHeight = height;
}

void EntityView::setBaseWidth(int width) {
	this->baseWidth = width;
}
void EntityView::setBaseHeight(int height) {
	this->baseHeight = height;
}

void EntityView::setImageHeight(int height) {
	this->imageHeight = height;
}

Vector2* EntityView::getAnchorPixel(){
	return this->anchorPixel;
}

void EntityView::setAnchorPixel(Vector2* anchorPixel){
	this->anchorPixel = anchorPixel;
}

int EntityView::getNClips() {
	return this->nClips;
}
int EntityView::getImageHeight() {
	return this->imageHeight;
}
int EntityView::getImageWidth() {
	return this->imageWidth;
}

void EntityView::loadImage() {
	this->image = textureHolder->getTexture(name);
	this->fogImage = textureHolder->getFogTexture(name);
}

void EntityView::setTextureHolder( TextureHolder* _textureHolder ) {
	textureHolder = _textureHolder;
}
TextureHolder* EntityView::getTextureHolder(){
	return this->textureHolder;
}

void EntityView::setDelay(int nuevoDelay) {
	this->delay = nuevoDelay;
}

int EntityView::getDelay() {
	return this->delay;
}

void EntityView::setFps(int nuevasFps) {
	this->fps = nuevasFps;
}

int EntityView::getFps() {
	return this->fps;
}

void EntityView::setEntity(Entity* entity) {
	this->entity = entity;
}

Entity* EntityView::getEntity() {
	return this->entity;
}

void EntityView::draw(SDL_Surface* screen, Position* cam, bool drawFog) {
	if (image == NULL) loadImage();

	SDL_Rect clipFog;
	clip.x = clipFog.x = this->imageWidth * this->currentClip * scaleWidth;
	clip.y = clipFog.y = 0;
	clip.w = clipFog.w = this->imageWidth * scaleWidth;
	clip.h = clipFog.h = this->imageHeight * scaleHeight;
	Vector3* position = entity->getCurrentPos();
	int x = (int) position->getX();
	int y = (int) position->getY();

	SDL_Rect offset, offsetFog;
	int tileH = Tile::computePositionTile(0,0,true).h;
	offset.x = offsetFog.x = (int) (Tile::computePositionTile(x, y, true).x + cam->getX() - (int)this->anchorPixel->getX());
	offset.y = offsetFog.y = (int) (Tile::computePositionTile(x, y, true).y + cam->getY() - (int)this->anchorPixel->getY() - tileH/2  );
	offset.h = offsetFog.h = clip.h;
	offset.w = offsetFog.w = clip.w;


	SDL_BlitSurface(image, &clip, screen, &offset);
	if (drawFog) SDL_BlitSurface(fogImage, &clipFog, screen, &offsetFog);

	timeSinceLastAnimation = timer.getTimeSinceLastAnimation();

	//Apply delay
	if (currentClip < this->nClips && timeSinceLastAnimation >= delay * 1000) {
		//Apply FPS cap
		if (animationRateTimer.getTimeSinceLastAnimation() >= 1000 / fps) {
			currentClip++;
			animationRateTimer.start();
		}
	} else {
		if ((currentRepeat >= numberOfRepeats) && (timeSinceLastAnimation >= delay * 1000)) {
			timer.start();
			currentRepeat = 0;
		}
		// Se agrega la condicion this->nClips != 0 para que en las animaciones staticas
		// currentRepeat no sume en cada ciclo
		if (currentClip >= this->nClips && this->nClips != 0) {
			currentClip = 0;
			currentRepeat++;
		}
	}

}

EntityView::~EntityView() {
	//SDL_FreeSurface(this->image);
	if (anchorPixel)
		delete anchorPixel;

}

std::string EntityView::getName() {
	return name;
}

void EntityView::setName(std::string name) {
	this->name = name;
}
