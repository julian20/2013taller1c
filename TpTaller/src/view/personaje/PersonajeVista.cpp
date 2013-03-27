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

#define FRAMES_PER_SECOND	10
#define SCALE				0.2

//como es animado necesito la cantidad de clips por lado.
int NUMERODECLIPS=4;

SDL_Rect clipsDerecha[ 4 ];
SDL_Rect clipsIzquierda[ 4 ];
SDL_Rect clipsArriba[ 4 ];
SDL_Rect clipsAbajo[ 4 ];

SDL_Surface *load_image( char* urlImagen )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load( urlImagen );
    if( loadedImage != NULL )
    {
    	loadedImage = rotozoomSurfaceXY(loadedImage, 0, SCALE, SCALE, 0);

        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        SDL_FreeSurface( loadedImage );

        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    return optimizedImage;
}

void PersonajeVista::Draw( float x, float y, SDL_Surface* source, SDL_Surface* screen, SDL_Rect* clip )
{
    SDL_Rect *offset;

    offset->x = (int)x + cameraX;
    offset->y = (int)y + cameraY;

    SDL_BlitSurface( source, clip, screen, offset );
}

void PersonajeVista::UpdateCameraPos(int x, int y){
	cameraX = x;
	cameraY = y;
}

PersonajeVista::PersonajeVista(Personaje* unPersonaje, char* urlImagen, SDL_Surface* fondo)
{
	cameraX = cameraY = 0;

	/* TODO: Faltaria lanzar una excepcion en caso de que no recieba alguno de los parametros
     if(!unPersonaje)
    {
        throw new FaltaParametroException("PersonajeVista");
    }*/
    
    miPersonaje = unPersonaje;

   // try
    //{
    	this->fondo = fondo;
        personajeImagen = this->CargarImagen(urlImagen);
        this->EstablecerLosClips(NUMERODECLIPS);
   /* }catch(ERROR e)
    {
        //TODO:cargo una imagen alternativa.
    }*/
}

SDL_Surface* PersonajeVista::CargarImagen(char* img)
{
    //Load the sprite sheet
    if(!img)
    {
      //  throw new FaltaParametroException();
    }
    
    SDL_Surface* miPersonajeImagen = load_image( img );

    //If there was a problem in loading the sprite
    if( !miPersonajeImagen)
    {
    	printf("NO SE HA ENCONTRADO LA IMAGEN\n");
        //return false;
        //TODO: cargo una alternativa
    }

    marco = 0;
    estado = PERSONAJE_DERECHA;
    return miPersonajeImagen;
}
void PersonajeVista::Mostrar()
{
	Vector2* movementDirection = this->miPersonaje->GetMovementDirection();
	Vector2* pos = this->miPersonaje->GetCurrentPos();

    if( movementDirection->GetX() < 0 )
    {
        estado = PERSONAJE_IZQUIERDA;
        marco++;         // Move to the next marco in the animation
    }
    else if( movementDirection->GetX() > 0 )
    {
        estado = PERSONAJE_DERECHA;
        marco++;
    }
    
    else if( movementDirection->GetY() > 0 )
    {
        estado = PERSONAJE_ABAJO;
        marco++;
    }
    else if( movementDirection->GetY() < 0 )
    {
        estado = PERSONAJE_ARRIBA;
        marco++;
    }
    else     // If Personaje standing
    {
        marco = 0; // Restart animation
    }

    //Loop the animation
    if( marco >= 4 )
    {
        marco = 0;
    }

    //Show the stick figure
    if( estado == PERSONAJE_DERECHA )
    {
        Draw( pos->GetX(), pos->GetY(), this->personajeImagen, fondo, &clipsDerecha[ marco ] );
    }
    else if( estado == PERSONAJE_IZQUIERDA )
    {
        Draw( pos->GetX(), pos->GetY(), this->personajeImagen, fondo, &clipsIzquierda[ marco ] );
    }else if (estado ==PERSONAJE_ARRIBA)
    {
        Draw(pos->GetX(), pos->GetY(), this->personajeImagen, fondo, &clipsArriba[ marco ] );
    }else if (estado ==PERSONAJE_ABAJO)
    {
        Draw(pos->GetX(), pos->GetY(), this->personajeImagen, fondo, &clipsAbajo[ marco ] );
    }
}


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

}

PersonajeVista::~PersonajeVista() 
{
    //libera la memoria que pide para La imagen
    SDL_FreeSurface( this->personajeImagen );

    //Quita SDL
    SDL_Quit();
}

