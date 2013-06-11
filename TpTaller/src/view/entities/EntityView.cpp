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
	this->base = new Base();
	this->scaleWidth = 1;
	this->scaleHeight = 1;
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
	declareTeamColorConstant();
}

EntityView::EntityView(EntityView* otherEntity) {
	Vector2* anchor = otherEntity->getAnchorPixel();
	Vector2* anchorCopy = new Vector2(anchor->getX(), anchor->getY());

	this->base = new Base();
	this->setBaseSizes(otherEntity->getBaseRows(),
					   otherEntity->getBaseCols());
	this->name = otherEntity->getName();
	this->scaleWidth = 1;
	this->scaleHeight = 1;
	this->imageHeight = otherEntity->getImageHeight();
	this->imageWidth = otherEntity->getImageWidth();
	this->entity = NULL;
	this->image = NULL;
	this->delay = otherEntity->getDelay();
	this->fps = otherEntity->getFps();
	this->nClips = otherEntity->getNClips();
	this->anchorPixel = anchorCopy;
	this->movable = otherEntity->isMovable();
	this->textureHolder = otherEntity->getTextureHolder();
	this->currentRepeat = 0;
	this->currentClip = 0;
	timer.start();
	animationRateTimer.start();

	timeSinceLastAnimation = 1000;

	setNumberOfRepeats(otherEntity->getNumberOfRepeats());
	declareTeamColorConstant();
}

void EntityView::declareTeamColorConstant() {
	// 0xAARRGGBB	A->Alpha	R->Red	G->Green	B->Blue
	const Uint32 alpha = 0x40000000;

	teamColors[0] = 0x00000000 + alpha;	// Sin color
	teamColors[1] = 0x00FF0000 + alpha;	// Rojo
	teamColors[2] = 0x0000FF00 + alpha;	// Verde
	teamColors[3] = 0x000000FF + alpha;	// Azul
	teamColors[4] = 0x00FF00FF + alpha;	// Violeta
	teamColors[5] = 0x00FFFF00 + alpha;	// Cian
	teamColors[6] = 0x0000FFFF + alpha;	// Amarillo
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
	this->scaleHeight = (float) (Tile::getTileHeight())
			* float(this->base->getCols()) / (float) (this->imageHeight);
	this->scaleWidth = (float) (Tile::getTileWidth())
			* (float) (this->base->getRows()) / (float) (this->imageWidth);
}

void EntityView::setImageWidth(int width) {
	this->imageWidth = width;
}

void EntityView::setNClips(int clips) {
	this->nClips = clips;
}

void EntityView::setBaseSizes(int _rows, int _cols) {
	this->base->setRows(_rows);
	this->base->setCols(_cols);
}

int EntityView::getBaseRows() {
	return this->base->getRows();
}

int EntityView::getBaseCols() {
	return this->base->getCols();
}

void EntityView::setImageHeight(int height) {
	this->imageHeight = height;
}

Vector2* EntityView::getAnchorPixel() {
	return this->anchorPixel;
}

void EntityView::setAnchorPixel(Vector2* anchorPixel) {
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

void EntityView::setTextureHolder(TextureHolder* _textureHolder) {
	textureHolder = _textureHolder;
}
TextureHolder* EntityView::getTextureHolder() {
	return this->textureHolder;
}

void EntityView::setDelay(float nuevoDelay) {
	this->delay = nuevoDelay;
}

float EntityView::getDelay() {
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

	if (this->entity)
		this->entity->setBaseSizes(base->getRows(), base->getCols());
}

Entity* EntityView::getEntity() {
	return this->entity;
}

void EntityView::draw(SDL_Surface* screen, Position* cam, bool drawFog) {
	if (image == NULL)
		loadImage();

	SDL_Rect clipFog;
	clip.x = clipFog.x = this->imageWidth * this->currentClip * scaleWidth;
	clip.y = clipFog.y = 0;
	clip.w = clipFog.w = this->imageWidth * scaleWidth;
	clip.h = clipFog.h = this->imageHeight * scaleHeight;
	Coordinates coords = entity->getCoordinates();
	int row = coords.getRow();
	int col = coords.getCol();

	SDL_Rect offset, offsetFog;
	int tileH = Tile::computePositionTile(0, 0).h;
	offset.x = offsetFog.x = (int) (Tile::computePositionTile(row, col).x
			+ cam->getX() - (int) this->anchorPixel->getX());
	offset.y = offsetFog.y = (int) (Tile::computePositionTile(row, col).y
			+ cam->getY() - (int) this->anchorPixel->getY() - tileH / 2);
	offset.h = offsetFog.h = clip.h;
	offset.w = offsetFog.w = clip.w;

	if (drawFog == false)
		SDL_BlitSurface(image, &clip, screen, &offset);

	if (drawFog && entity->getHideInFog() == false) {
		SDL_BlitSurface(image, &clip, screen, &offset);
		if (drawFog)
			SDL_BlitSurface(fogImage, &clipFog, screen, &offsetFog);
	}

	timeSinceLastAnimation = timer.getTimeSinceLastAnimation();

	//Apply delay
	if (currentClip < this->nClips && timeSinceLastAnimation >= delay * 1000) {
		//Apply FPS cap
		if (animationRateTimer.getTimeSinceLastAnimation() >= 1000 / fps) {
			currentClip++;
			animationRateTimer.start();
		}
	} else {
		if ((currentRepeat >= numberOfRepeats)
				&& (timeSinceLastAnimation >= delay * 1000)) {
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
	delete this->base;
}

std::string EntityView::getName() {
	return name;
}

void EntityView::setName(std::string name) {
	this->name = name;
}
