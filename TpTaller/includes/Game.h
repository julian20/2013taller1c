/*
 * Game.h
 *
 *  Created on: 22/03/2013
 *      Author: tomas
 */

#ifndef GAME_H_
#define GAME_H_

//STL
#include <ctime>

//SDL
#include <SDL/SDL_events.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <Events.h>

//Ours
#include <model/persistence/PersistentConfiguration.h>
#include <controller/MapController.h>
#include <controller/MapCameraController.h>
#include <controller/PlayerController.h>
#include <view/configuration/GameConfiguration.h>
#include <view/MapView.h>
#include <view/TextHandler.h>

class Game {
public:
        Game(PersistentConfiguration* configuration);
        MenuEvent run();
        virtual ~Game();
private:
        Mix_Music* musica;

        SDL_Surface* screen;
        SDL_Rect posFondo;
        SDL_Surface* fondo;

        GameConfiguration* gameConfig;

        MapView* mapView;
        MapController* mapController;
        MapCameraController* cameraController;

        Player* personaje;
        PersonajeVista* personajeVista;

        void initMusic();
        void initScreen();
        void draw();
        void setUpCharacters(MapView* map,MapData* mapData,EntityViewMap* viewMap);
        void setUpEntities(MapView* map,MapData* mapData);



        //FPS Crap
        void applyFPS(int timer);
        TextHandler* textHandler;
        int fps;
        int tempFps;
        int fpsUpdatingTimer;
};

#endif /* GAME_H_ */
