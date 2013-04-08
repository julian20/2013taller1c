/* 
 * File:   PersonajeVista.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:25
 */

#include <view/entities/PersonajeVista.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <cmath>
#include <string>

using namespace std;

EntityView::EntityView() {
	this->imagePath = "";
	this->imageHeight=0;
	this->imageWidth=0;
	this->entity = NULL;
	this->image = NULL;
	this->nClips = 1;
	this->currentClip=0;
	this->anchorPixel = new Vector2(0, 0);
}
void EntityView::setImageWidth(int width)
{
	this->imageWidth = width;
}
void EntityView::setNClips(int clips)
{
	this->nClips=clips;
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

	loadedImage = IMG_Load(urlImagen.c_str());

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

void EntityView::draw(SDL_Surface* screen,Position* cam){
	SDL_Rect offset;


	clip.x =this->imageWidth*this->currentClip;
	clip.y =0;
	clip.w=this->imageWidth;
	clip.h=this->imageHeight;
	Vector2* position = entity->getCurrentPos();
	int x = (int) position->GetX();
	int y = (int) position->GetY();


	offset.x = (int) (Tile::computePosition(x,y).x + cam->getX() - clip.w/2);
	offset.y = (int) (Tile::computePosition(x,y).y + cam->getY() - clip.h/2);
	offset.h = clip.h;
	offset.w = clip.w;

	SDL_BlitSurface(image, &clip, screen, &offset);

	if(currentClip < this->nClips)currentClip++;
	else currentClip=0;

}

EntityView::~EntityView() {
	SDL_FreeSurface(this->image);
	delete anchorPixel;

}

