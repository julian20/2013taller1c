/* 
 * File:   PersonajeControlador.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:25
 */

#include <controller/PersonajeControlador.h>
PersonajeControlador::PersonajeControlador(Personaje* unPersonaje) {
    
    miPersonaje=unPersonaje;
}

void PersonajeControlador::ControlarEventos(SDL_Event& event)
{

	int movVertical=PERSONAJE_ALTO/4;
	int movHorizontal=PERSONAJE_ANCHO/4;
    //If a key was pressed
   // SDL_Event event;
	SDL_PollEvent(&event);
    if( event.type == SDL_KEYDOWN )
    {
            //Es la vista la que le da la info al controlador de cuanto debe desplazar el personaje.
           if(event.key.keysym.sym == SDLK_RIGHT){ miPersonaje->PersonajeMoverDerecha(movHorizontal);}
           if(event.key.keysym.sym== SDLK_LEFT) miPersonaje->PersonajeMoverIzquierda(movHorizontal);
           if(event.key.keysym.sym==SDLK_UP) {printf("toco arriba\n");miPersonaje->PersonajeMoverArriba(movVertical);}
           if(event.key.keysym.sym== SDLK_DOWN) miPersonaje->PersonajeMoverAbajo(movVertical);
    }
    //If a key was released
    else if( event.type == SDL_KEYUP )
    {
    	  //Es la vista la que le da la info al controlador de cuanto debe desplazar el personaje.

    	           if(event.key.keysym.sym == SDLK_RIGHT)/* miPersonaje->FrenarX();*/miPersonaje->PersonajeMoverIzquierda(movHorizontal);
    	           if(event.key.keysym.sym== SDLK_LEFT) /*miPersonaje->FrenarX();*/miPersonaje->PersonajeMoverDerecha(movHorizontal);
    	           if(event.key.keysym.sym==SDLK_UP) /*miPersonaje->FrenarY();*/miPersonaje->PersonajeMoverAbajo(movVertical);
    	           if(event.key.keysym.sym== SDLK_DOWN) /* miPersonaje->FrenarY();*/miPersonaje->PersonajeMoverArriba(movVertical);
  /*  }else if(event.type==SDL_MOUSEBUTTONDOWN)
    {
    	miPersonaje->MoveTo(event.button.x,event.button.y,movHorizontal, movVertical);

    */}
}

void PersonajeControlador::PersonajeVerificarQueEsteEnRango()
{
    //Keep the stick figure in bounds
    if( ( miPersonaje->GetOffsetX() < 0 ) || ( miPersonaje->GetOffsetX() + PERSONAJE_ANCHO >  SCREEN_WIDTH ) )
    {
        miPersonaje->FrenarX();
    }
    
    if( ( miPersonaje->GetOffsetY() < 0 ) || ( miPersonaje->GetOffsetY() + PERSONAJE_ALTO > SCREEN_HEIGHT ) )
    {
        miPersonaje->FrenarY();
    }
}


PersonajeControlador::~PersonajeControlador() 
{
    
}

