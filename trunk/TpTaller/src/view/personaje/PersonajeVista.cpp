/* 
 * File:   PersonajeVista.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:25
 */

#include <view/PersonajeVista.h>
#include <stdio.h>
#include <SDL/SDL.h>

//The frames per second
const int FRAMES_PER_SECOND = 10;



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

    //Load the image
    loadedImage = IMG_Load( urlImagen );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

void dibujar( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}


PersonajeVista::PersonajeVista(Personaje* unPersonaje,char* urlImagen,SDL_Surface* fondo)
{
  /* Faltaria lanzar una excepcion en caso de que no recieba alguno de los parametros
   *  if(!unPersonaje)
    {
        throw new FaltaParametroException("PersonajeVista");
    }*/
    
    miPersonaje=unPersonaje;

   // try
    //{
    	this->fondo=fondo;
        personajeImagen=this->CargarImagen(urlImagen);
        this->EstablecerLosClips(NUMERODECLIPS);
        this->Mostrar();
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
        //TODO:cargo una alternativa
    }
    marco=0;
    estado=PERSONAJE_DERECHA;
    return miPersonajeImagen;
}
void PersonajeVista::Mostrar()
{
    //If Personaje is moving left
    if( this->miPersonaje->GetVelocidadX() < 0 )
    {
        //Set the animation to left
        estado = PERSONAJE_IZQUIERDA;

        //Move to the next marco in the animation
        marco++;
    }
    //If Personaje is moving right
    else if( miPersonaje->GetVelocidadX() > 0 )
    {
        //Set the animation to right
        estado = PERSONAJE_DERECHA;

        //Move to the next marco in the animation
        marco++;
    }
    
    else if( miPersonaje->GetVelocidadY() > 0 )
    {
        //Set the animation to right
        estado = PERSONAJE_ABAJO;

        //Move to the next marco in the animation
        marco++;
    }
    else if( miPersonaje->GetVelocidadY() < 0 )
    {
        //Set the animation to right
        estado = PERSONAJE_ARRIBA;

        //Move to the next marco in the animation
        marco++;
    }
    //If Personaje standing
    else
    {
        //Restart the animation
        marco = 0;
    }

    //Loop the animation
    if( marco >= 4 )
    {
        marco = 0;
    }
    printf("Estado del personaje %i \n", estado);
    //Show the stick figure
    if( estado == PERSONAJE_DERECHA )
    {
        dibujar( miPersonaje->GetOffsetX(),miPersonaje->GetOffsetY(), this->personajeImagen, fondo, &clipsDerecha[ marco ] );
    }
    else if( estado == PERSONAJE_IZQUIERDA )
    {
        dibujar( miPersonaje->GetOffsetX(),miPersonaje->GetOffsetY(), this->personajeImagen, fondo, &clipsIzquierda[ marco ] );
    }else if (estado ==PERSONAJE_ARRIBA)
    {
        
        dibujar(miPersonaje->GetOffsetX(),miPersonaje->GetOffsetY(), this->personajeImagen, fondo, &clipsArriba[ marco ] );
        
    }else if (estado ==PERSONAJE_ABAJO)
    {
        dibujar(miPersonaje->GetOffsetX(),miPersonaje->GetOffsetY(), this->personajeImagen, fondo, &clipsAbajo[ marco ] );
    }
}


void PersonajeVista::EstablecerLosClips(int cantidadPorLado)
{
    //Clip the sprites
    clipsDerecha[ 0 ].x = 0;
    clipsDerecha[ 0 ].y = 0;
    clipsDerecha[ 0 ].w = PERSONAJE_ANCHO;
    clipsDerecha[ 0 ].h = PERSONAJE_ALTO;

    clipsDerecha[ 1 ].x = PERSONAJE_ANCHO;
    clipsDerecha[ 1 ].y = 0;
    clipsDerecha[ 1 ].w = PERSONAJE_ANCHO;
    clipsDerecha[ 1 ].h = PERSONAJE_ALTO;

    clipsDerecha[ 2 ].x = PERSONAJE_ANCHO * 2;
    clipsDerecha[ 2 ].y = 0;
    clipsDerecha[ 2 ].w = PERSONAJE_ANCHO;
    clipsDerecha[ 2 ].h = PERSONAJE_ALTO;

    clipsDerecha[ 3 ].x = PERSONAJE_ANCHO * 3;
    clipsDerecha[ 3 ].y = 0;
    clipsDerecha[ 3 ].w = PERSONAJE_ANCHO;
    clipsDerecha[ 3 ].h = PERSONAJE_ALTO;

    clipsIzquierda[ 0 ].x = 0;
    clipsIzquierda[ 0 ].y = PERSONAJE_ALTO;
    clipsIzquierda[ 0 ].w = PERSONAJE_ANCHO;
    clipsIzquierda[ 0 ].h = PERSONAJE_ALTO;

    clipsIzquierda[ 1 ].x = PERSONAJE_ANCHO;
    clipsIzquierda[ 1 ].y = PERSONAJE_ALTO;
    clipsIzquierda[ 1 ].w = PERSONAJE_ANCHO;
    clipsIzquierda[ 1 ].h = PERSONAJE_ALTO;

    clipsIzquierda[ 2 ].x = PERSONAJE_ANCHO * 2;
    clipsIzquierda[ 2 ].y = PERSONAJE_ALTO;
    clipsIzquierda[ 2 ].w = PERSONAJE_ANCHO;
    clipsIzquierda[ 2 ].h = PERSONAJE_ALTO;

    clipsIzquierda[ 3 ].x = PERSONAJE_ANCHO * 3;
    clipsIzquierda[ 3 ].y = PERSONAJE_ALTO;
    clipsIzquierda[ 3 ].w = PERSONAJE_ANCHO;
    clipsIzquierda[ 3 ].h = PERSONAJE_ALTO;
    
    clipsArriba[ 0 ].x = 0;
    clipsArriba[ 0 ].y = PERSONAJE_ALTO * 2;
    clipsArriba[ 0 ].w = PERSONAJE_ANCHO;
    clipsArriba[ 0 ].h = PERSONAJE_ALTO;

    clipsArriba[ 1 ].x = PERSONAJE_ANCHO;
    clipsArriba[ 1 ].y = PERSONAJE_ALTO * 2;
    clipsArriba[ 1 ].w = PERSONAJE_ANCHO;
    clipsArriba[ 1 ].h = PERSONAJE_ALTO;

    clipsArriba[ 2 ].x = PERSONAJE_ANCHO * 2;
    clipsArriba[ 2 ].y = PERSONAJE_ALTO * 2;
    clipsArriba[ 2 ].w = PERSONAJE_ANCHO;
    clipsArriba[ 2 ].h = PERSONAJE_ALTO;

    clipsArriba[ 3 ].x = PERSONAJE_ANCHO * 3;
    clipsArriba[ 3 ].y = PERSONAJE_ALTO * 2;
    clipsArriba[ 3 ].w = PERSONAJE_ANCHO;
    clipsArriba[ 3 ].h = PERSONAJE_ALTO;

    
    clipsAbajo[ 0 ].x = 0;
    clipsAbajo[ 0 ].y = PERSONAJE_ALTO * 3;
    clipsAbajo[ 0 ].w = PERSONAJE_ANCHO;
    clipsAbajo[ 0 ].h = PERSONAJE_ALTO;

    clipsAbajo[ 1 ].x = PERSONAJE_ANCHO;
    clipsAbajo[ 1 ].y = PERSONAJE_ALTO * 3;
    clipsAbajo[ 1 ].w = PERSONAJE_ANCHO;
    clipsAbajo[ 1 ].h = PERSONAJE_ALTO;

    clipsAbajo[ 2 ].x = PERSONAJE_ANCHO * 2;
    clipsAbajo[ 2 ].y = PERSONAJE_ALTO * 3;
    clipsAbajo[ 2 ].w = PERSONAJE_ANCHO;
    clipsAbajo[ 2 ].h = PERSONAJE_ALTO;

    clipsAbajo[ 3 ].x = PERSONAJE_ANCHO * 3;
    clipsAbajo[ 3 ].y = PERSONAJE_ALTO * 3;
    clipsAbajo[ 3 ].w = PERSONAJE_ANCHO;
    clipsAbajo[ 3 ].h = PERSONAJE_ALTO;

}

PersonajeVista::~PersonajeVista() 
{
    //libera la memoria que pide para La imagen
    SDL_FreeSurface( this->personajeImagen );

    //Quita SDL
    SDL_Quit();
}

