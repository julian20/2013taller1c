/* 
 * File:   PlayerView.h
 * Author: damian
 *
 * Created on 23 de marzo de 2013, 12:25
 */
#ifndef PERSONAJEVISTA_H
#define	PERSONAJEVISTA_H

#include <model/entities/player/Player.h>
#include <view/entities/EntityView.h>
#include <view/timer/Timer.h>
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

using namespace std;

class PlayerView: public EntityView {
public:
	PlayerView();
	void cargarImagen(string path);
	void Mostrar(SDL_Surface* fondo);
	void setPersonaje(Player* personaje);
	Player* getPersonaje();
	void UpdateCameraPos(int x, int y);
	Player* getEntity();
	void setEntity(Entity* entity);
	int getImageHeight();
	void setImageHeight(int height);
	int getImageWidth();
	void setImageWidth(int width);
	int getNClips();
	void setNClips(int clips);
	virtual ~PlayerView();
	void EstablecerLosClips();
	void draw(SDL_Surface* screen, Position* camera, Timer* timer);
private:
	void showFrame(SDL_Surface* source, SDL_Surface* screen, SDL_Rect* clip);

	Player* miPersonaje;
	//SDL_Surface* fondo;
	SDL_Surface* personajeImagen;
	int cameraX;
	int cameraY;
	int marco; 	// Current frame
	int estado; // Animation status
	SDL_Rect* clipToDraw;
	int animationChangeRate;
	int imageWidth;
	int imageHeight;
	int numberOfClips;

	//Vectores de clips
	vector<SDL_Rect> clipsDerecha;
	vector<SDL_Rect> clipsIzquierda;
	vector<SDL_Rect> clipsArriba;
	vector<SDL_Rect> clipsAbajo;
	vector<SDL_Rect> clipsAbajoIzq;
	vector<SDL_Rect> clipsAbajoDer;
	vector<SDL_Rect> clipsArribaDer;
	vector<SDL_Rect> clipsArribaIzq;
	vector<SDL_Rect> clipsQuieto;
	vector<SDL_Rect> clipsDerechaRun;
	vector<SDL_Rect> clipsIzquierdaRun;
	vector<SDL_Rect> clipsArribaRun;
	vector<SDL_Rect> clipsAbajoRun;
	vector<SDL_Rect> clipsAbajoIzqRun;
	vector<SDL_Rect> clipsAbajoDerRun;
	vector<SDL_Rect> clipsArribaDerRun;
	vector<SDL_Rect> clipsArribaIzqRun;

};

#endif	/* PERSONAJEVISTA_H */

