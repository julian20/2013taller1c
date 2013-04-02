/* 
 * File:   PersonajeVista.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:25
 */

#include <view/PersonajeVista.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <cmath>
#include <string>

#define OFFSET_X	40
#define OFFSET_Y	10


//#define SCALE				0.2

//como es animado necesito la cantidad de clips por lado.
#define NUMERODECLIPS 8

SDL_Rect clipsDerecha[NUMERODECLIPS];
SDL_Rect clipsIzquierda[NUMERODECLIPS];
SDL_Rect clipsArriba[NUMERODECLIPS];
SDL_Rect clipsAbajo[NUMERODECLIPS];
SDL_Rect clipsAbajoIzq[NUMERODECLIPS];
SDL_Rect clipsAbajoDer[NUMERODECLIPS];
SDL_Rect clipsArribaDer[NUMERODECLIPS];
SDL_Rect clipsArribaIzq[NUMERODECLIPS];
SDL_Rect clipsQuieto[NUMERODECLIPS];

SDL_Surface* load_image(std::string urlImagen) {
	//The image that's loaded
	SDL_Surface* loadedImage = NULL;

	//The optimized surface that will be used
	SDL_Surface* optimizedImage = NULL;

	loadedImage = IMG_Load(urlImagen.c_str());

	if (loadedImage != NULL) {
		//loadedImage = rotozoomSurfaceXY(loadedImage, 0, SCALE, SCALE, 0);

		//Create an optimized surface
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);

		SDL_FreeSurface(loadedImage);

		if (optimizedImage != NULL) {
			//Color key surface
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY,
					SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
		}
	}

	return optimizedImage;
}

void PersonajeVista::Draw(float x, float y, SDL_Surface* source, SDL_Surface* screen, SDL_Rect* clip) {
	SDL_Rect offset;

	//offset.x = (int)x + cameraX - (clip->w/SCALE)/2;
	offset.x = (int) x + cameraX - clip->w;
	offset.y = (int) y + cameraY - clip->h;
	offset.w = clip->w;
	offset.h = clip->h;


	SDL_BlitSurface(source, clip, screen, &offset);
}

void PersonajeVista::UpdateCameraPos(int x, int y) {
	cameraX = x;
	cameraY = y;
}

PersonajeVista::PersonajeVista(Personaje* unPersonaje, std::string urlImagen) {
	cameraX = cameraY = 0;
	marco = 0;
	estado = 0;
	/* TODO: Faltaria lanzar una excepcion en caso de que no recieba alguno de los parametros
	 if(!unPersonaje)
	 {
	 throw new FaltaParametroException("PersonajeVista");
	 }*/

	miPersonaje = unPersonaje;

	// try
	//{
	//this->fondo = fondo;
	personajeImagen = this->CargarImagen(urlImagen);
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

	int x = pos->GetX() + OFFSET_X;
	int y = pos->GetY() + OFFSET_Y;

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
	if (marco >= 4)
		marco = 0;    // Loop the animation

	Draw(x, y, this->personajeImagen, fondo, clipToDraw);
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
		clipsArriba[i].y = PERSONAJE_ALTO * 2;
		clipsArriba[i].w = PERSONAJE_ANCHO;
		clipsArriba[i].h = PERSONAJE_ALTO;

		clipsAbajo[i].x = PERSONAJE_ANCHO* i;
		clipsAbajo[i].y = PERSONAJE_ALTO * 3;
		clipsAbajo[i].w = PERSONAJE_ANCHO;
		clipsAbajo[i].h = PERSONAJE_ALTO;

		clipsArribaIzq[i].x = PERSONAJE_ANCHO* i;
		clipsArribaIzq[i].y = PERSONAJE_ALTO * 6;
		clipsArribaIzq[i].w = PERSONAJE_ANCHO;
		clipsArribaIzq[i].h = PERSONAJE_ALTO;

		clipsArribaDer[i].x = PERSONAJE_ANCHO* i;
		clipsArribaDer[i].y = PERSONAJE_ALTO * 7;
		clipsArribaDer[i].w = PERSONAJE_ANCHO;
		clipsArribaDer[i].h = PERSONAJE_ALTO;

		clipsAbajoIzq[i].x = PERSONAJE_ANCHO* i;
		clipsAbajoIzq[i].y = PERSONAJE_ALTO * 5;
		clipsAbajoIzq[i].w = PERSONAJE_ANCHO;
		clipsAbajoIzq[i].h = PERSONAJE_ALTO;

		clipsAbajoDer[i].x = PERSONAJE_ANCHO* i;
		clipsAbajoDer[i].y = PERSONAJE_ALTO * 4;
		clipsAbajoDer[i].w = PERSONAJE_ANCHO;
		clipsAbajoDer[i].h = PERSONAJE_ALTO;

		clipsQuieto[i].x = PERSONAJE_ANCHO * i;
		clipsQuieto[i].y = PERSONAJE_ALTO * 8;
		clipsQuieto[i].w = PERSONAJE_ANCHO;
		clipsQuieto[i].h = PERSONAJE_ALTO;
	}
}
/*
 void PersonajeVista::EstablecerLosClips(int cantidadPorLado)
 {
 //Clip the sprites
 clipsDerecha[ 0 ].x = 0;
 clipsDerecha[ 0 ].y = 0;
 clipsDerecha[ 0 ].w = PERSONAJE_ANCHO*SCALE;
 clipsDerecha[ 0 ].h = PERSONAJE_ALTO*SCALE;

 clipsDerecha[ 1 ].x = PERSONAJE_ANCHO*SCALE;
 clipsDerecha[ 1 ].y = 0;
 clipsDerecha[ 1 ].w = PERSONAJE_ANCHO*SCALE;
 clipsDerecha[ 1 ].h = PERSONAJE_ALTO*SCALE;

 clipsDerecha[ 2 ].x = PERSONAJE_ANCHO * 2*SCALE;
 clipsDerecha[ 2 ].y = 0;
 clipsDerecha[ 2 ].w = PERSONAJE_ANCHO*SCALE;
 clipsDerecha[ 2 ].h = PERSONAJE_ALTO*SCALE;

 clipsDerecha[ 3 ].x = PERSONAJE_ANCHO * 3*SCALE;
 clipsDerecha[ 3 ].y = 0;
 clipsDerecha[ 3 ].w = PERSONAJE_ANCHO*SCALE;
 clipsDerecha[ 3 ].h = PERSONAJE_ALTO*SCALE;

 clipsIzquierda[ 0 ].x = 0;
 clipsIzquierda[ 0 ].y = PERSONAJE_ALTO*SCALE;
 clipsIzquierda[ 0 ].w = PERSONAJE_ANCHO*SCALE;
 clipsIzquierda[ 0 ].h = PERSONAJE_ALTO*SCALE;

 clipsIzquierda[ 1 ].x = PERSONAJE_ANCHO*SCALE;
 clipsIzquierda[ 1 ].y = PERSONAJE_ALTO*SCALE;
 clipsIzquierda[ 1 ].w = PERSONAJE_ANCHO*SCALE;
 clipsIzquierda[ 1 ].h = PERSONAJE_ALTO*SCALE;

 clipsIzquierda[ 2 ].x = PERSONAJE_ANCHO * 2*SCALE;
 clipsIzquierda[ 2 ].y = PERSONAJE_ALTO*SCALE;
 clipsIzquierda[ 2 ].w = PERSONAJE_ANCHO*SCALE;
 clipsIzquierda[ 2 ].h = PERSONAJE_ALTO*SCALE;

 clipsIzquierda[ 3 ].x = PERSONAJE_ANCHO * 3*SCALE;
 clipsIzquierda[ 3 ].y = PERSONAJE_ALTO*SCALE;
 clipsIzquierda[ 3 ].w = PERSONAJE_ANCHO*SCALE;
 clipsIzquierda[ 3 ].h = PERSONAJE_ALTO*SCALE;

 clipsArriba[ 0 ].x = 0;
 clipsArriba[ 0 ].y = PERSONAJE_ALTO * 2*SCALE;
 clipsArriba[ 0 ].w = PERSONAJE_ANCHO*SCALE;
 clipsArriba[ 0 ].h = PERSONAJE_ALTO*SCALE;

 clipsArriba[ 1 ].x = PERSONAJE_ANCHO*SCALE;
 clipsArriba[ 1 ].y = PERSONAJE_ALTO * 2*SCALE;
 clipsArriba[ 1 ].w = PERSONAJE_ANCHO*SCALE;
 clipsArriba[ 1 ].h = PERSONAJE_ALTO*SCALE;

 clipsArriba[ 2 ].x = PERSONAJE_ANCHO * 2*SCALE;
 clipsArriba[ 2 ].y = PERSONAJE_ALTO * 2*SCALE;
 clipsArriba[ 2 ].w = PERSONAJE_ANCHO*SCALE;
 clipsArriba[ 2 ].h = PERSONAJE_ALTO*SCALE;

 clipsArriba[ 3 ].x = PERSONAJE_ANCHO * 3*SCALE;
 clipsArriba[ 3 ].y = PERSONAJE_ALTO * 2*SCALE;
 clipsArriba[ 3 ].w = PERSONAJE_ANCHO*SCALE;
 clipsArriba[ 3 ].h = PERSONAJE_ALTO*SCALE;


 clipsAbajo[ 0 ].x = 0;
 clipsAbajo[ 0 ].y = PERSONAJE_ALTO * 3*SCALE;
 clipsAbajo[ 0 ].w = PERSONAJE_ANCHO*SCALE;
 clipsAbajo[ 0 ].h = PERSONAJE_ALTO*SCALE;

 clipsAbajo[ 1 ].x = PERSONAJE_ANCHO*SCALE;
 clipsAbajo[ 1 ].y = PERSONAJE_ALTO * 3*SCALE;
 clipsAbajo[ 1 ].w = PERSONAJE_ANCHO*SCALE;
 clipsAbajo[ 1 ].h = PERSONAJE_ALTO*SCALE;

 clipsAbajo[ 2 ].x = PERSONAJE_ANCHO * 2*SCALE;
 clipsAbajo[ 2 ].y = PERSONAJE_ALTO * 3*SCALE;
 clipsAbajo[ 2 ].w = PERSONAJE_ANCHO*SCALE;
 clipsAbajo[ 2 ].h = PERSONAJE_ALTO*SCALE;

 clipsAbajo[ 3 ].x = PERSONAJE_ANCHO * 3*SCALE;
 clipsAbajo[ 3 ].y = PERSONAJE_ALTO * 3*SCALE;
 clipsAbajo[ 3 ].w = PERSONAJE_ANCHO*SCALE;
 clipsAbajo[ 3 ].h = PERSONAJE_ALTO*SCALE;

 }*/

PersonajeVista::~PersonajeVista() {
	//libera la memoria que pide para La imagen
	SDL_FreeSurface(this->personajeImagen);

	//Quita SDL
	SDL_Quit();
}

