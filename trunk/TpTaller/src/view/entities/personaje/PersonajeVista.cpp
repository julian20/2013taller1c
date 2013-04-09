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

//Posicion de los pies del personaje respecto de la base de la imagen
#define OFFSET_Y	15

#define RUTA_IMAGEN "resources/soldierSheet.png"
#define ANIMATION_CHANGE_DELAY 1

//#define SCALE				0.2

void PersonajeVista::showFrame(SDL_Surface* source, SDL_Surface* screen,
		SDL_Rect* clip) {
	SDL_Rect offset;

	Vector2* position = miPersonaje->GetCurrentPos();
	float x = position->GetX();
	float y = position->GetY();
	offset.x = (int) x + cameraX - clip->w / 2;
	offset.y = (int) y + cameraY - clip->h + OFFSET_Y;
	offset.w = clip->w;
	offset.h = clip->h;

	SDL_BlitSurface(source, clip, screen, &offset);
}

void PersonajeVista::draw(SDL_Surface* screen, Position* cam, Timer* timer) {
	UpdateCameraPos(cam->getX(), cam->getY());
	Mostrar(screen);
}

void PersonajeVista::UpdateCameraPos(int x, int y) {
	cameraX = x;
	cameraY = y;
}

PersonajeVista::PersonajeVista()
//Llamamos al constructor de la superclase
:
		EntityView() {
	cout << "I made it" << endl;
	cameraX = cameraY = 0;
	miPersonaje = NULL;
	marco = 0;
	estado = 0;
	clipToDraw = NULL;
	animationChangeRate = 0;
	personajeImagen = NULL;
	imageHeight = 0;
	imageWidth = 0;
	numberOfClips = 0;
	movable = true;
	// try
	//{
	//this->fondo = fondo;
	/* }catch(ERROR e)
	 {
	 //TODO:cargo una imagen alternativa.
	 }*/
}

void PersonajeVista::setPersonaje(Personaje* personaje) {
	this->miPersonaje = personaje;
	Vector2* anchorPixel = new Vector2(clip.w / 2, OFFSET_Y);
	miPersonaje->getBase()->setAnchorPixel(anchorPixel);
}

Personaje* PersonajeVista::getPersonaje() {
	return this->miPersonaje;
}

void PersonajeVista::cargarImagen(std::string img) {
	//Load the sprite sheet
	if (img.compare("")) {
		//  throw new FaltaParametroException();
	}

	SDL_Surface* miPersonajeImagen = load_image(img);

	//If there was a problem in loading the sprite
	if (!miPersonajeImagen) {
		printf("NO SE HA ENCONTRADO LA IMAGEN DEL PERSONAJE\n");
		//return false;
		//TODO: cargo una alternativa
		miPersonajeImagen = load_image(RUTA_IMAGEN);
	}

	marco = 0;
	estado = PERSONAJE_DERECHA;
	personajeImagen = miPersonajeImagen;
}

Personaje* PersonajeVista::getEntity() {
	return miPersonaje;

}

void PersonajeVista::setEntity(Entity* entity) {
	//TODO: Error check (si no es un personaje)
	Personaje* aux = (Personaje*) entity;
	miPersonaje = aux;
}

void PersonajeVista::Mostrar(SDL_Surface* fondo) {
	Vector2* movementDirection = this->miPersonaje->GetMovementDirection();
	float direction = movementDirection->GetAngle();

	if (M_PI * 15 / 8 < direction || direction < M_PI * 1 / 8)
		clipToDraw = &clipsDerecha[marco];
	else if (M_PI * 1 / 8 < direction && direction < M_PI * 3 / 8)
		clipToDraw = &clipsAbajoDer[marco];
	else if (M_PI * 3 / 8 < direction && direction < M_PI * 5 / 8)
		clipToDraw = &clipsAbajo[marco];
	else if (M_PI * 5 / 8 < direction && direction < M_PI * 7 / 8)
		clipToDraw = &clipsAbajoIzq[marco];
	else if (M_PI * 7 / 8 < direction && direction < M_PI * 9 / 8)
		clipToDraw = &clipsIzquierda[marco];
	else if (M_PI * 9 / 8 < direction && direction < M_PI * 11 / 8)
		clipToDraw = &clipsArribaIzq[marco];
	else if (M_PI * 11 / 8 < direction && direction < M_PI * 13 / 8)
		clipToDraw = &clipsArriba[marco];
	else if (M_PI * 13 / 8 < direction && direction < M_PI * 15 / 8)
		clipToDraw = &clipsArribaDer[marco];

	if (miPersonaje->IsMoving()) {
		if (animationChangeRate == ANIMATION_CHANGE_DELAY) {
			this->marco++;
			animationChangeRate = 0;// Move to the next marco in the animation
		} else
			animationChangeRate++;
	} else {
		clipToDraw = &clipsQuieto[marco];
		if (animationChangeRate == ANIMATION_CHANGE_DELAY) {
			this->marco++;
			animationChangeRate = 0;// Move to the next marco in the animation
		} else
			animationChangeRate++;
	}
	if (marco >= numberOfClips)
		marco = 0;    // Loop the animation

	showFrame(this->personajeImagen, fondo, clipToDraw);
}

void PersonajeVista::EstablecerLosClips() {

	for (int i = 0; i < numberOfClips; i++) {
		SDL_Rect rect;

		clipsIzquierda.push_back(rect);
		clipsIzquierda[i].x = imageWidth * i;
		clipsIzquierda[i].y = imageHeight * 0;
		clipsIzquierda[i].w = imageWidth;
		clipsIzquierda[i].h = imageHeight;

		clipsAbajoIzq.push_back(rect);
		clipsAbajoIzq[i].x = imageWidth * i;
		clipsAbajoIzq[i].y = imageHeight * 1;
		clipsAbajoIzq[i].w = imageWidth;
		clipsAbajoIzq[i].h = imageHeight;

		clipsAbajoDer.push_back(rect);
		clipsAbajoDer[i].x = imageWidth * i;
		clipsAbajoDer[i].y = imageHeight * 2;
		clipsAbajoDer[i].w = imageWidth;
		clipsAbajoDer[i].h = imageHeight;

		clipsAbajo.push_back(rect);
		clipsAbajo[i].x = imageWidth * i;
		clipsAbajo[i].y = imageHeight * 3;
		clipsAbajo[i].w = imageWidth;
		clipsAbajo[i].h = imageHeight;

		clipsArribaIzq.push_back(rect);
		clipsArribaIzq[i].x = imageWidth * i;
		clipsArribaIzq[i].y = imageHeight * 4;
		clipsArribaIzq[i].w = imageWidth;
		clipsArribaIzq[i].h = imageHeight;

		clipsArribaDer.push_back(rect);
		clipsArribaDer[i].x = imageWidth * i;
		clipsArribaDer[i].y = imageHeight * 5;
		clipsArribaDer[i].w = imageWidth;
		clipsArribaDer[i].h = imageHeight;

		clipsArriba.push_back(rect);
		clipsArriba[i].x = imageWidth * i;
		clipsArriba[i].y = imageHeight * 6;
		clipsArriba[i].w = imageWidth;
		clipsArriba[i].h = imageHeight;

		clipsDerecha.push_back(rect);
		clipsDerecha[i].x = imageWidth * i;
		clipsDerecha[i].y = imageHeight * 7;
		clipsDerecha[i].w = imageWidth;
		clipsDerecha[i].h = imageHeight;

		clipsQuieto.push_back(rect);
		clipsQuieto[i].x = imageWidth * i;
		clipsQuieto[i].y = imageHeight * 8;
		clipsQuieto[i].w = imageWidth;
		clipsQuieto[i].h = imageHeight;

	}
}

PersonajeVista::~PersonajeVista() {
	//libera la memoria que pide para La imagen
	SDL_FreeSurface(this->personajeImagen);

	//Quita SDL
	SDL_Quit();
}

int PersonajeVista::getImageHeight() {
	return this->imageHeight;
}

void PersonajeVista::setImageHeight(int height) {
	this->imageHeight = height;
}

int PersonajeVista::getImageWidth() {
	return this->imageWidth;
}

void PersonajeVista::setImageWidth(int width) {
	this->imageWidth = width;
}

int PersonajeVista::getNClips() {
	return this->numberOfClips;
}

void PersonajeVista::setNClips(int clips) {
	this->numberOfClips = clips;
}

