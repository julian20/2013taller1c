/* 
 * File:   PersonajeVista.h
 * Author: damian
 *
 * Created on 23 de marzo de 2013, 12:25
 */
#ifndef PERSONAJEVISTA_H
#define	PERSONAJEVISTA_H

#include "model/Personaje.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;



//The frames per second
const int FRAMES_PER_SECOND = 10;



//como es animado necesito la cantidad de clips por lado.
//int NUMERODECLIPS=4;

/*SDL_Rect clipsDerecha[ 4 ];
SDL_Rect clipsIzquierda[ 4 ];
SDL_Rect clipsArriba[ 4 ];
SDL_Rect clipsAbajo[ 4 ];*/
//Dimensiones de la figura Imagen.
const int PERSONAJE_ANCHO = 64;
const int PERSONAJE_ALTO = 205;

//APerfiles de la vista del personaje.


class PersonajeVista {
public:
    PersonajeVista(Personaje* unPersonaje,char* urlImagen);
    SDL_Surface* CargarImagen(char* path);
    void Mostrar(SDL_Surface* fondo);
    virtual ~PersonajeVista();
private:
         void EstablecerLosClips(int cantidadPorLado);
         Personaje* miPersonaje;


        //The surfaces
        SDL_Surface* personajeImagen;
        //Its current frame
         int marco;

        //Its animation status
         int estado;
};

#endif	/* PERSONAJEVISTA_H */

