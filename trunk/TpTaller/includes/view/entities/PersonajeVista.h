/* 
 * File:   PersonajeVista.h
 * Author: damian
 *
 * Created on 23 de marzo de 2013, 12:25
 */
#ifndef PERSONAJEVISTA_H
#define	PERSONAJEVISTA_H

#include <model/entities/personaje/Personaje.h>
#include <view/entities/EntityView.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <model/Vector2.h>
#include <string>

#define _USE_MATH_DEFINES	// To use math.h constants
#include <math.h>

#define		SCREEN_WIDTH	640
#define		SCREEN_HEIGHT  	480
#define		SCREEN_BPP  	32

//Dimensiones de la figura Imagen.
#define PERSONAJE_ANCHO  96
#define PERSONAJE_ALTO  96


//APerfiles de la vista del personaje.


class PersonajeVista : public EntityView {
public:
    PersonajeVista();
    SDL_Surface* CargarImagen(std::string path);
    void Mostrar(SDL_Surface* fondo);
    void setPersonaje(Personaje* personaje);
    Personaje* getPersonaje();
    void UpdateCameraPos(int x, int y);
    Personaje* getEntity();
    void setEntity(Entity* entity);
    virtual ~PersonajeVista();
    void draw(SDL_Surface* screen, Position* camera);
private:
    void showFrame(SDL_Surface* source, SDL_Surface* screen, SDL_Rect* clip);
	void EstablecerLosClips(int cantidadPorLado);
	Personaje* miPersonaje;
	//SDL_Surface* fondo;
	SDL_Surface* personajeImagen;
	int cameraX;
	int cameraY;
	int marco; 	// Current frame
	int estado; // Animation status
	SDL_Rect* clipToDraw;
	int animationChangeRate;
};

#endif	/* PERSONAJEVISTA_H */

