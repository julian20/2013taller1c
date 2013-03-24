/* 
 * File:   Personaje.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:13
 */

#include <model/Personaje.h>

Personaje::Personaje()
{
    //Initialize movement variables
    offSetX = 0;
    offSetY = 0;
    velocidadX = 0;
    velocidadY = 0;
    //Initialize animation variables
    //marco = 0;
    //estado = PERSONAJE_DERECHA;
}

int Personaje::GetVelocidadX()
{
    return this->velocidadX;
}
int Personaje::GetVelocidadY()
{
    return this->velocidadY;
}

int Personaje::GetOffsetX()
{
    return this->offSetX;
}
int Personaje::GetOffsetY()
{
    return this->offSetY;
}
void Personaje::PersonajeMover()
{
    //Move
    offSetX += velocidadX;
    offSetY += velocidadY;
}

void Personaje::FrenarX()
{
    this->offSetX-=this->velocidadX;
}
void Personaje::FrenarY()
{
     this->offSetY-=this->velocidadY;
}

void Personaje::PersonajeMoverDerecha(int desplazamiento)
{
    this->velocidadX += desplazamiento;
}

void Personaje::PersonajeMoverIzquierda(int desplazamiento)
{
    this->velocidadX -=desplazamiento;
}
void Personaje::PersonajeMoverArriba(int desplazamiento)
{
    this->velocidadY -=desplazamiento;
}
void Personaje::PersonajeMoverAbajo(int desplazamiento)
{
    this->velocidadY +=desplazamiento;
}

Personaje::~Personaje() {
}

