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

#define OFFSET_X	47
#define OFFSET_Y	30

#define RUTA_IMAGEN "resources/foo5.png"

//#define SCALE				0.2

//como es animado necesito la cantidad de clips por lado.
#define NUMERODECLIPS 9

SDL_Rect clipsDerecha[NUMERODECLIPS];
SDL_Rect clipsIzquierda[NUMERODECLIPS];
SDL_Rect clipsArriba[NUMERODECLIPS];
SDL_Rect clipsAbajo[NUMERODECLIPS];
SDL_Rect clipsAbajoIzq[NUMERODECLIPS];
SDL_Rect clipsAbajoDer[NUMERODECLIPS];
SDL_Rect clipsArribaDer[NUMERODECLIPS];
SDL_Rect clipsArribaIzq[NUMERODECLIPS];
SDL_Rect clipsQuieto[NUMERODECLIPS];


void PersonajeVista::draw(SDL_Surface* source, SDL_Surface* screen, SDL_Rect* clip) {
	SDL_Rect offset;

	Vector2* position = miPersonaje->GetCurrentPos();
	float x = position->GetX();
	float y = position->GetY();
	offset.x = (int) x + cameraX - clip->w + OFFSET_X;
	offset.y = (int) y + cameraY - clip->h + OFFSET_Y;
	offset.w = clip->w;
	offset.h = clip->h;

	SDL_BlitSurface(source, clip, screen, &offset);
}

void PersonajeVista::UpdateCameraPos(int x, int y) {
	cameraX = x;
	cameraY = y;
}

PersonajeVista::PersonajeVista(Personaje* unPersonaje)
	//Llamamos al constructor de la superclase
	:EntityView(){

	setImagePath(RUTA_IMAGEN);
	cameraX = cameraY = 0;
	marco = 0;
	estado = 0;
	/* TODO: Faltaria lanzar una excepcion en caso de que no recieba alguno de los parametros
	 if(!unPersonaje)
	 {
	 throw new FaltaParametroException("PersonajeVista");
	 }*/

	miPersonaje = unPersonaje;
	Vector2* anchorPixel = new Vector2(OFFSET_X,OFFSET_Y);
	miPersonaje->getBase()->setAnchorPixel(anchorPixel);

	// try
	//{
	//this->fondo = fondo;
	personajeImagen = this->CargarImagen(RUTA_IMAGEN);
	this->EstablecerLosClips(NUMERODECLIPS);
	/* }catch(ERROR e)
	 {
	 //TODO:cargo una imagen alternativa.
	 }*/
}

SDL_Surface* PersonajeVista::CargarImagen(std::string img) {
	//Load the sprite sheet
	if (img.compare("")) {
		//  throw new FaltaParametroException();
	}

	SDL_Surface* miPersonajeImagen = load_image(img);

	//If there was a problem in loading the sprite
	if (!miPersonajeImagen) {
		printf("NO SE HA ENCONTRADO LA IMAGEN\n");
		//return false;
		//TODO: cargo una alternativa
	}

	marco = 0;
	estado = PERSONAJE_DERECHA;
	return miPersonajeImagen;
}

void PersonajeVista::Mostrar(SDL_Surface* fondo) {
	Vector2* movementDirection = this->miPersonaje->GetMovementDirection();
	float direction = movementDirection->GetAngle();
	Vector2* pos = this->miPersonaje->GetCurrentPos();

	SDL_Rect* clipToDraw;

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

	if (miPersonaje->IsMoving())
		this->marco++;	// Move to the next marco in the animation
	else
	{
		clipToDraw= &clipsQuieto[marco];
		marco++;	// Stop Animation, unit standing.
	}
	if (marco >= NUMERODECLIPS)
		marco = 0;    // Loop the animation

	draw(this->personajeImagen, fondo, clipToDraw);
}

void PersonajeVista::EstablecerLosClips(int cantidadPorLado) {
	for (int i = 0; i < cantidadPorLado;i++) {
		clipsDerecha[i].x = PERSONAJE_ANCHO* i;
		clipsDerecha[i].y = 0;
		clipsDerecha[i].w = PERSONAJE_ANCHO;
		clipsDerecha[i].h = PERSONAJE_ALTO;

		clipsIzquierda[i].x = PERSONAJE_ANCHO* i;
		clipsIzquierda[i].y = PERSONAJE_ALTO;
		clipsIzquierda[i].w = PERSONAJE_ANCHO;
		clipsIzquierda[i].h = PERSONAJE_ALTO;

		clipsArriba[i].x = PERSONAJE_ANCHO* i;
		clipsArriba[i].y = PERSONAJE_ALTO * 3;
		clipsArriba[i].w = PERSONAJE_ANCHO;
		clipsArriba[i].h = PERSONAJE_ALTO;

		clipsAbajo[i].x = PERSONAJE_ANCHO* i;
		clipsAbajo[i].y = PERSONAJE_ALTO * 2;
		clipsAbajo[i].w = PERSONAJE_ANCHO;
		clipsAbajo[i].h = PERSONAJE_ALTO;

		clipsArribaIzq[i].x = PERSONAJE_ANCHO* i;
		clipsArribaIzq[i].y = PERSONAJE_ALTO * 5;
		clipsArribaIzq[i].w = PERSONAJE_ANCHO;
		clipsArribaIzq[i].h = PERSONAJE_ALTO;

		clipsArribaDer[i].x = PERSONAJE_ANCHO* i;
		clipsArribaDer[i].y = PERSONAJE_ALTO * 4;
		clipsArribaDer[i].w = PERSONAJE_ANCHO;
		clipsArribaDer[i].h = PERSONAJE_ALTO;

		clipsAbajoIzq[i].x = PERSONAJE_ANCHO* i;
		clipsAbajoIzq[i].y = PERSONAJE_ALTO * 7;
		clipsAbajoIzq[i].w = PERSONAJE_ANCHO;
		clipsAbajoIzq[i].h = PERSONAJE_ALTO;

		clipsAbajoDer[i].x = PERSONAJE_ANCHO* i;
		clipsAbajoDer[i].y = PERSONAJE_ALTO * 6;
		clipsAbajoDer[i].w = PERSONAJE_ANCHO;
		clipsAbajoDer[i].h = PERSONAJE_ALTO;

		clipsQuieto[i].x = PERSONAJE_ANCHO * i;
		clipsQuieto[i].y = PERSONAJE_ALTO * 8;
		clipsQuieto[i].w = PERSONAJE_ANCHO;
		clipsQuieto[i].h = PERSONAJE_ALTO;
	}
}

PersonajeVista::~PersonajeVista() {
	//libera la memoria que pide para La imagen
	SDL_FreeSurface(this->personajeImagen);

	//Quita SDL
	SDL_Quit();
}

