/*
 * EntityViewData.cpp
 *
 *  Created on: 13/04/2013
 *      Author: damian
 */

#include <view/entities/EntityViewData.h>
Logs unlog;

EntityViewData::EntityViewData() {
	this->openImage=NULL;
	this->baseHeight = 1;
	this->baseWidth = 1;
	this->scaleWidth = 1;
	this->scaleHeight = 1;
	this->entityImage = "";
	this->imageHeight = 0;
	this->imageWidth = 0;
	this->nClips=0;
	this->anchorPixel = new Vector2(0, 0);
}

void EntityViewData::setScale() {
	this->scaleHeight = (float) (Tile::getTileHeight()) * float(this->baseHeight)
			/ (float) (this->imageHeight);
	this->scaleWidth = (float) (Tile::getTileWidth()) * (float) (this->baseWidth)
			/ (float) (this->imageWidth);
	printf("ancho %f---- alto %f", scaleWidth, scaleHeight);
}
int EntityViewData::getNClips() {
	return this->nClips;
}
void EntityViewData::setNClips(int clips) {
	this->nClips=clips;
}
void EntityViewData::setBaseWidth(int width) {
	this->baseWidth = width;
}
void EntityViewData::setBaseHeight(int height) {
	this->baseHeight = height;
}

void EntityViewData::setImageHeight(int height) {
	this->imageHeight = height;
}

void EntityViewData::setImageWidth(int width) {
	this->imageWidth = width;
}

Vector2* EntityViewData::getAnchorPixel(){
	return this->anchorPixel;
}

void EntityViewData::setAnchorPixel(Vector2* anchorPixel){
	this->anchorPixel = anchorPixel;
}

int EntityViewData::getImageHeight() {
	return this->imageHeight;
}
int EntityViewData::getImageWidth() {
	return this->imageWidth;
}

void EntityViewData::setEntityId(string id)
{
	this->entityId= id;
}

void EntityViewData::setDelay(int nuevoDelay) {
	this->delay = nuevoDelay;
}

int EntityViewData::getDelay() {
	return this->delay;
}
void EntityViewData::setFps(int nuevasFps) {
	this->fps = nuevasFps;
}
void EntityViewData::setNumberOfRepeats(int repeat)
{
	this->nOfRepeats=repeat;
}

int EntityViewData::getFps() {
	return this->fps;
}
string EntityViewData::getEntityId()
{
	return this->entityId;
}/*
void EntityViewData::setName(string name)
{
	this->name=name;
}
string EntityViewData::getName()
{
	return this->name;
}*/
void EntityViewData::setEntityImage(string image)
{
	this->entityImage= image;
}

EntityViewData::~EntityViewData() {
	SDL_FreeSurface(openImage);
	if (anchorPixel) delete anchorPixel;
}

SDL_Surface* EntityViewData::getEntityImage() {
	if (this->openImage == NULL) {

		SDL_Surface* loadedImageTmp = IMG_Load(this->entityImage.c_str());
		if (loadedImageTmp == NULL) {
			printf("Unable to load Entity Image. %s\n", SDL_GetError());
			unlog.logErrorMessage(string("Unable to load Entity Image.")+string(SDL_GetError()));
			return NULL;
		}

		SDL_Surface* loadedImageRot = rotozoomSurfaceXY(loadedImageTmp, 0, this->scaleWidth,
				this->scaleHeight, 0);
		SDL_FreeSurface(loadedImageTmp);
		SDL_Surface* loadedImage = SDL_DisplayFormatAlpha(loadedImageRot);
		SDL_FreeSurface(loadedImageRot);
		this->openImage = loadedImage;
	}
	return this->openImage;
}

