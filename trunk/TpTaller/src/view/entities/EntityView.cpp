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


using namespace std;
#define DELAY 2

EntityView::EntityView() {
	this->baseHeight=1;
	this->baseWidth=1;
	this->scaleWidth=1;
	this->scaleHeight=1;
	this->imagePath = "";
	this->tileHeight=50;
	this->tileWidth=70;
	this->imageHeight=0;
	this->imageWidth=0;
	this->entity = NULL;
	this->image = NULL;
	this->nClips = 1;
	this->currentClip=0;
	this->anchorPixel = new Vector2(0, 0);
	this->movable = false;
	timer.start();
	timeSinceLastAnimation = 1000;

}

bool EntityView::isMovable(){
	return movable;
}
void EntityView::setScale()
{
	//baseHeight = entity->getBase()->getLength();
	//baseWidth = entity->getBase()->getWidth();
	this->scaleHeight=(float)(this->tileHeight)*float(this->baseHeight)/(float)(this->imageHeight);
	this->scaleWidth=(float)(this->tileWidth)*(float)(this->baseWidth)/(float)(this->imageWidth);
	printf("ancho %f---- alto %f",scaleWidth,scaleHeight);
}

void EntityView::setImageWidth(int width)
{
	this->imageWidth = width;
}

void EntityView::setNClips(int clips)
{
	this->nClips=clips;
}

void EntityView::setTileWidth(int width)
{
	this->tileWidth=width;
}
void EntityView::setTileHeight(int height)
{
	this->tileHeight=height;
}
void EntityView::setBaseWidth(int width)
{
	this->baseWidth=width;
}
void EntityView::setBaseHeight(int height)
{
	this->baseHeight=height;
}

void EntityView::setImageHeight(int height)
{
	this->imageHeight = height;
}

SDL_Surface* EntityView::load_image(string urlImagen) {

	//The image that's loaded
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* loadedImage = NULL;

	//The optimized surface that will be used
	SDL_Surface* optimizedImage = NULL;
	//this->setScale();
	loadedImage = IMG_Load(urlImagen.c_str());
	loadedImage = rotozoomSurfaceXY( loadedImage, 0, scaleWidth, scaleHeight, 0 );
	if (loadedImage != NULL) {
		//loadedImage = rotozoomSurfaceXY(loadedImage, 0, 2, 2, 0);

		//Create an optimized surface
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);



		if (optimizedImage != NULL) {
			//Color key surface
			SDL_FreeSurface(loadedImage);
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY,
					SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
		}
		else
			optimizedImage = loadedImage;
	}

	return optimizedImage;
}
/*void EntityView::Draw()
{
	SDL_BlitSurface(source, clip, screen, &offset);
}*/

void EntityView::setImagePath(string image_path) {
	this->imagePath = image_path;
	cout << image_path << endl;
	this->image = load_image(image_path);
	if (!image) { //TODO al log / loadear alternativa
		cout << "Error al cargar imagen de la vista " << image_path << endl;
	}
}

string EntityView::getImagePath() {
	return this->imagePath;
}

void EntityView::setEntity(Entity* entity) {
	this->entity = entity;
}

Entity* EntityView::getEntity() {
	return this->entity;
}

void EntityView::setAnchorPixel(Vector2* anchorPixel) {
	delete this->anchorPixel;
	this->anchorPixel = anchorPixel;
}

Vector2* EntityView::getAnchorPixel() {
	return this->anchorPixel;
}
/*
void EntityView::EstablecerClips()
{
	for(int x=0; x < this->nClips ; x++)
	{
		clip[x].x =this->imageWidth*x;
		clip[x].y =0;
		clip[x].w=this->imageWidth;
		clip[x].h=this->imageHeight;

	}
}*/

void EntityView::draw(SDL_Surface* screen, Position* cam) {

	clip.x =this->imageWidth*this->currentClip*scaleWidth;
	clip.y =0;
	clip.w=this->imageWidth*scaleWidth;
	clip.h=this->imageHeight*scaleHeight;
	Vector2* position = entity->getCurrentPos();
	int x = (int) position->GetX();
	int y = (int) position->GetY();

	SDL_Rect offset;
	offset.x = (int) (Tile::computePosition(x,y).x + cam->getX() - clip.w/2);
	offset.y = (int) (Tile::computePosition(x,y).y + cam->getY() - clip.h/2);
	offset.h = clip.h;
	offset.w = clip.w;

	SDL_BlitSurface(image, &clip, screen, &offset);
	timeSinceLastAnimation = timer.getTimeSinceLastAnimation();
	if (currentClip < this->nClips && timeSinceLastAnimation >= DELAY*1000) {
		currentClip++;
	}
	else {
		if (timeSinceLastAnimation >= DELAY*1000)timer.start();
		currentClip = 0;
	}
	cout<<timer.getTimeSinceLastAnimation()<< endl;
}

EntityView::~EntityView() {
	SDL_FreeSurface(this->image);
	delete anchorPixel;

}

