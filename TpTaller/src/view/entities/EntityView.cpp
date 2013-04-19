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

void EntityView::setPixelInvisible(SDL_Surface * surface, int x, int y) {
	Uint32 mask = 0x00000000;

	Uint8 * pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + (x * sizeof(Uint32));
	*((Uint32*)pixel) = mask;
}

SDL_Surface* EntityView::createFogSurface(int width, int heigth) {
	Uint32 rmask, gmask, bmask, amask;
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;

	SDL_Surface* retval = SDL_CreateRGBSurface(SDL_SWSURFACE, width, heigth,
    										32, rmask, gmask, bmask, amask);

    SDL_FillRect(retval, NULL, 0x90000000);	// Si quieren tocar que tan oscuro es el fog cambien
    										// los dos primeros valores del hexa ese.

    if(retval == NULL) {
       Logs::logErrorMessage("Fog surface cannot be loaded");
       // TODO: que mas acemo' aca?
    }

    return retval;
}

void EntityView::loadImage() {
	image = textureHolder->getTexture(name);
	loadFog(image);
}

void EntityView::loadFog(SDL_Surface* image) {
	SDL_LockSurface(image);
	this->fogImage = createFogSurface(image->w, image->h);

	Uint32 *pixels = (Uint32 *)image->pixels;
	Uint32 pixelValue;
	Uint8 red, green, blue, alpha;	// channels

	for (int x = 0; x < image->w; x++) {
		for (int y = 0; y < image->h; y++) {
			pixelValue = pixels[ ( y * image->w ) + x ];
			SDL_GetRGBA(pixelValue, image->format, &red, &green, &blue, &alpha);

			if (alpha == 0) setPixelInvisible(this->fogImage, x, y);
		}
	}

	SDL_UnlockSurface(image);
}

/*SDL_Surface* EntityView::loadImage(string urlImagen, map<string,SDL_Surface*> *images) {
	//The image that's loaded
	SDL_Surface* loadedImageTmp = NULL;

	//The optimized surface that will be used
	SDL_Surface* optimizedImage = NULL;

	if (images->count(urlImagen) > 0){
		return images->at(urlImagen);
	}

	loadedImageTmp = IMG_Load(urlImagen.c_str());
	SDL_Surface* loadedImage = rotozoomSurfaceXY(loadedImageTmp, 0, scaleWidth, scaleHeight, 0);
	SDL_FreeSurface(loadedImageTmp);
	if (loadedImage != NULL) {
		//loadedImage = rotozoomSurfaceXY(loadedImage, 0, 2, 2, 0);

		//Create an optimized surface
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);

		if (optimizedImage != NULL) {
			//Color key surface
			SDL_FreeSurface(loadedImage);
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY,
					SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
		} else {
			optimizedImage = loadedImage;
		}
	}

	images->insert(std::pair<string,SDL_Surface*>(urlImagen,optimizedImage));

	return optimizedImage;
}*/

/*void EntityView::setImagePath(string image_path, map<string,SDL_Surface*> *images) {
	this->imagePath = image_path;
	this->image = loadImage(image_path, images);
	if (!image) { //TODO al log / loadear alternativa

		//Logs(string("Error al cargar imagen de la vista ") + image_path);
		this->image=loadImage(DEFAULT_IMAGE,images);
		this->nClips=0;
		//printf("cargo la imagen\n");
	}

}*/

void EntityView::setTextureHolder( TextureHolder* _textureHolder ) {
	textureHolder = _textureHolder;
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

void EntityView::draw(SDL_Surface* screen, Position* cam) {
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
	SDL_BlitSurface(fogImage, &clipFog, screen, &offsetFog);

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
