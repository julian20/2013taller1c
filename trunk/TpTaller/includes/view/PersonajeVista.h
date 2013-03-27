/* 
 * File:   PersonajeVista.h
 * Author: damian
 *
 * Created on 23 de marzo de 2013, 12:25
 */
#ifndef PERSONAJEVISTA_H
#define	PERSONAJEVISTA_H

#include <model/Personaje.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <model/Vector2.h>

#define		SCREEN_WIDTH	640
#define		SCREEN_HEIGHT  	480
#define		SCREEN_BPP  	32

//Dimensiones de la figura Imagen.
#define PERSONAJE_ANCHO  64
#define PERSONAJE_ALTO  205


//APerfiles de la vista del personaje.


class PersonajeVista {
public:
    PersonajeVista(Personaje* unPersonaje,char* urlImagen,SDL_Surface* fondo);
    SDL_Surface* CargarImagen(char* path);
    void Mostrar();
    void UpdateCameraPos(int x, int y);
    virtual ~PersonajeVista();
private:
    void Draw( float x, float y, SDL_Surface* source, SDL_Surface* screen, SDL_Rect* clip );
	void EstablecerLosClips(int cantidadPorLado);
	Personaje* miPersonaje;
	SDL_Surface* fondo;
	SDL_Surface* personajeImagen;
	int cameraX;
	int cameraY;
	int marco; 	// Current frame
	int estado; // Animation status
};

#endif	/* PERSONAJEVISTA_H */

