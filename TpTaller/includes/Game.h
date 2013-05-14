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

//pthread
#include <pthread.h>

//SDL
#include <SDL/SDL_events.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <Events.h>

//Ours
#include <model/persistence/PersistentConfiguration.h>
#include <model/Chat.h>
#include <controller/MapController.h>
#include <controller/MapCameraController.h>
#include <controller/PlayerController.h>
#include <view/configuration/GameConfiguration.h>
#include <view/MapView.h>
#include <view/MapCameraView.h>
#include <view/TextHandler.h>
#include <view/networking/ChatWindowsView.h>

class Game {
public:
        Game(PersistentConfiguration* configuration, bool multiplayer);
        virtual MenuEvent run();

        virtual void addNewPlayer(Player* player, PlayerView* view, Coordinates* coords);
        PlayerView* getPlayerView();

        MapCameraView* getMapCameraView();
        MapData* getMapData();

        Chat* getChat();

        list<PlayerEvent*> getEvents();
        void cleanEvents();

        virtual ~Game();
protected:

        MapView* mapView;
        int fps;
        int tempFps;
        int fpsUpdatingTimer;
        GameConfiguration* gameConfig;
private:
        Mix_Music* musica;

        SDL_Surface* screen;
        SDL_Rect posFondo;
        SDL_Surface* fondo;

        Chat* chat ;
        ChatWindowsView* chatView;

        Player* personaje;
        PlayerView* personajeVista;

        list<Player*> otherPlayers;
        list<PlayerView*> otherPlayerViews;

        MapController* mapController;
        MapCameraController* cameraController;

        void applyFPS(int timer);
        void initMusic();
        void initScreen();
        void draw();
        void setUpCharacters(MapView* map,MapData* mapData,EntityViewMap* viewMap);
        void setUpEntities(MapView* map,MapData* mapData);
        void playersUpdate();




        //FPS Crap

        TextHandler* textHandler;

        bool openAudio;
};

#endif /* GAME_H_ */
