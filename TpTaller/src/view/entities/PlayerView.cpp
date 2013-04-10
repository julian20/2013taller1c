/* 
 * File:   PersonajeVista.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:25
 */

#include <view/entities/PlayerView.h>
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

void PlayerView::showFrame(SDL_Surface* source, SDL_Surface* screen,
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

void PlayerView::draw(SDL_Surface* screen, Position* cam, Timer* timer) {
	UpdateCameraPos(cam->getX(), cam->getY());
	Mostrar(screen);
}

void PlayerView::UpdateCameraPos(int x, int y) {
	cameraX = x;
	cameraY = y;
}

PlayerView::PlayerView()
//Llamamos al constructor de la superclase
:
		EntityView() {
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

void PlayerView::setPersonaje(Player* personaje) {
	this->miPersonaje = personaje;
	Vector2* anchorPixel = new Vector2(clip.w / 2, OFFSET_Y);
	miPersonaje->getBase()->setAnchorPixel(anchorPixel);
}

Player* PlayerView::getPersonaje() {
	return this->miPersonaje;
}

void PlayerView::cargarImagen(std::string img) {
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

Player* PlayerView::getEntity() {
	return miPersonaje;

}

void PlayerView::setEntity(Entity* entity) {
	//TODO: Error check (si no es un personaje)
	Player* aux = (Player*) entity;
	miPersonaje = aux;
}

void PlayerView::Mostrar(SDL_Surface* fondo) {
	Vector2* movementDirection = this->miPersonaje->GetMovementDirection();
	float direction = movementDirection->GetAngle();

	if ((M_PI * 15 / 8 < direction || direction < M_PI * 1 / 8)
			&& miPersonaje->isRunning())
		clipToDraw = &clipsDerechaRun[marco];
	else if (M_PI * 15 / 8 < direction || direction < M_PI * 1 / 8)
		clipToDraw = &clipsDerecha[marco];
	else if ((M_PI * 1 / 8 < direction && direction < M_PI * 3 / 8)
			&& miPersonaje->isRunning())
		clipToDraw = &clipsAbajoDerRun[marco];
	else if (M_PI * 1 / 8 < direction && direction < M_PI * 3 / 8)
		clipToDraw = &clipsAbajoDer[marco];
	else if ((M_PI * 3 / 8 < direction && direction < M_PI * 5 / 8)
			&& miPersonaje->isRunning())
		clipToDraw = &clipsAbajoRun[marco];
	else if (M_PI * 3 / 8 < direction && direction < M_PI * 5 / 8)
		clipToDraw = &clipsAbajo[marco];
	else if ((M_PI * 5 / 8 < direction && direction < M_PI * 7 / 8)
			&& miPersonaje->isRunning())
		clipToDraw = &clipsAbajoIzqRun[marco];
	else if (M_PI * 5 / 8 < direction && direction < M_PI * 7 / 8)
		clipToDraw = &clipsAbajoIzq[marco];
	else if ((M_PI * 7 / 8 < direction && direction < M_PI * 9 / 8)
			&& miPersonaje->isRunning())
		clipToDraw = &clipsIzquierdaRun[marco];
	else if (M_PI * 7 / 8 < direction && direction < M_PI * 9 / 8)
		clipToDraw = &clipsIzquierda[marco];
	else if ((M_PI * 9 / 8 < direction && direction < M_PI * 11 / 8)
			&& miPersonaje->isRunning())
		clipToDraw = &clipsArribaIzqRun[marco];
	else if (M_PI * 9 / 8 < direction && direction < M_PI * 11 / 8)
		clipToDraw = &clipsArribaIzq[marco];
	else if ((M_PI * 11 / 8 < direction && direction < M_PI * 13 / 8)
			&& miPersonaje->isRunning())
		clipToDraw = &clipsArribaRun[marco];
	else if (M_PI * 11 / 8 < direction && direction < M_PI * 13 / 8)
		clipToDraw = &clipsArriba[marco];
	else if ((M_PI * 13 / 8 < direction && direction < M_PI * 15 / 8)
			&& miPersonaje->isRunning())
		clipToDraw = &clipsArribaDerRun[marco];
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

void PlayerView::EstablecerLosClips() {

	for (int i = 0; i < numberOfClips; i++) {
		SDL_Rect rect;
		int j = 0;
		clipsIzquierda.push_back(rect);
		clipsIzquierda[i].x = imageWidth * i;
		clipsIzquierda[i].y = imageHeight * j;
		clipsIzquierda[i].w = imageWidth;
		clipsIzquierda[i].h = imageHeight;
		j++;

		clipsAbajoIzq.push_back(rect);
		clipsAbajoIzq[i].x = imageWidth * i;
		clipsAbajoIzq[i].y = imageHeight * j;
		clipsAbajoIzq[i].w = imageWidth;
		clipsAbajoIzq[i].h = imageHeight;
		j++;

		clipsAbajoDer.push_back(rect);
		clipsAbajoDer[i].x = imageWidth * i;
		clipsAbajoDer[i].y = imageHeight * j;
		clipsAbajoDer[i].w = imageWidth;
		clipsAbajoDer[i].h = imageHeight;
		j++;

		clipsAbajo.push_back(rect);
		clipsAbajo[i].x = imageWidth * i;
		clipsAbajo[i].y = imageHeight * j;
		clipsAbajo[i].w = imageWidth;
		clipsAbajo[i].h = imageHeight;
		j++;

		clipsArribaIzq.push_back(rect);
		clipsArribaIzq[i].x = imageWidth * i;
		clipsArribaIzq[i].y = imageHeight * j;
		clipsArribaIzq[i].w = imageWidth;
		clipsArribaIzq[i].h = imageHeight;
		j++;

		clipsArribaDer.push_back(rect);
		clipsArribaDer[i].x = imageWidth * i;
		clipsArribaDer[i].y = imageHeight * j;
		clipsArribaDer[i].w = imageWidth;
		clipsArribaDer[i].h = imageHeight;
		j++;

		clipsArriba.push_back(rect);
		clipsArriba[i].x = imageWidth * i;
		clipsArriba[i].y = imageHeight * j;
		clipsArriba[i].w = imageWidth;
		clipsArriba[i].h = imageHeight;
		j++;

		clipsDerecha.push_back(rect);
		clipsDerecha[i].x = imageWidth * i;
		clipsDerecha[i].y = imageHeight * j;
		clipsDerecha[i].w = imageWidth;
		clipsDerecha[i].h = imageHeight;
		j++;

		clipsIzquierdaRun.push_back(rect);
		clipsIzquierdaRun[i].x = imageWidth * i;
		clipsIzquierdaRun[i].y = imageHeight * j;
		clipsIzquierdaRun[i].w = imageWidth;
		clipsIzquierdaRun[i].h = imageHeight;
		j++;

		clipsAbajoIzqRun.push_back(rect);
		clipsAbajoIzqRun[i].x = imageWidth * i;
		clipsAbajoIzqRun[i].y = imageHeight * j;
		clipsAbajoIzqRun[i].w = imageWidth;
		clipsAbajoIzqRun[i].h = imageHeight;
		j++;

		clipsAbajoDerRun.push_back(rect);
		clipsAbajoDerRun[i].x = imageWidth * i;
		clipsAbajoDerRun[i].y = imageHeight * j;
		clipsAbajoDerRun[i].w = imageWidth;
		clipsAbajoDerRun[i].h = imageHeight;
		j++;

		clipsAbajoRun.push_back(rect);
		clipsAbajoRun[i].x = imageWidth * i;
		clipsAbajoRun[i].y = imageHeight * j;
		clipsAbajoRun[i].w = imageWidth;
		clipsAbajoRun[i].h = imageHeight;
		j++;

		clipsArribaIzqRun.push_back(rect);
		clipsArribaIzqRun[i].x = imageWidth * i;
		clipsArribaIzqRun[i].y = imageHeight * j;
		clipsArribaIzqRun[i].w = imageWidth;
		clipsArribaIzqRun[i].h = imageHeight;
		j++;

		clipsArribaDerRun.push_back(rect);
		clipsArribaDerRun[i].x = imageWidth * i;
		clipsArribaDerRun[i].y = imageHeight * j;
		clipsArribaDerRun[i].w = imageWidth;
		clipsArribaDerRun[i].h = imageHeight;
		j++;

		clipsArribaRun.push_back(rect);
		clipsArribaRun[i].x = imageWidth * i;
		clipsArribaRun[i].y = imageHeight * j;
		clipsArribaRun[i].w = imageWidth;
		clipsArribaRun[i].h = imageHeight;
		j++;

		clipsDerechaRun.push_back(rect);
		clipsDerechaRun[i].x = imageWidth * i;
		clipsDerechaRun[i].y = imageHeight * j;
		clipsDerechaRun[i].w = imageWidth;
		clipsDerechaRun[i].h = imageHeight;
		j++;

		clipsQuieto.push_back(rect);
		clipsQuieto[i].x = imageWidth * i;
		clipsQuieto[i].y = imageHeight * j;
		clipsQuieto[i].w = imageWidth;
		clipsQuieto[i].h = imageHeight;
	}
}

PlayerView::~PlayerView() {
	//libera la memoria que pide para La imagen
	SDL_FreeSurface(this->personajeImagen);

	//Quita SDL
	SDL_Quit();
}

int PlayerView::getImageHeight() {
	return this->imageHeight;
}

void PlayerView::setImageHeight(int height) {
	this->imageHeight = height;
}

int PlayerView::getImageWidth() {
	return this->imageWidth;
}

void PlayerView::setImageWidth(int width) {
	this->imageWidth = width;
}

int PlayerView::getNClips() {
	return this->numberOfClips;
}

void PlayerView::setNClips(int clips) {
	this->numberOfClips = clips;
}

