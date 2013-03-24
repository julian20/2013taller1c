/* 
 * File:   Personaje.h
 * Author: damian
 *
 * Created on 23 de marzo de 2013, 12:13
 */
#ifndef PERSONAJE_H
#define	PERSONAJE_H

//Constantes para definir la direccion. Faltan los laterales.
const int PERSONAJE_DERECHA = 0;
const int PERSONAJE_IZQUIERDA = 1;
const int PERSONAJE_ARRIBA = 2;
const int PERSONAJE_ABAJO = 3;


class Personaje {
public:
      private:
    //The offset
    int offSetX;

    //The offset
    int offSetY;
    
    //Its rate of movement
    int velocidadX;

    //Its rate of movement
    int velocidadY;

   

    public:
    //Initializes the variables
    Personaje();


    //Moves the stick figure
    void PersonajeMover();
    void PersonajeMoverIzquierda(int desplazamiento);
    void PersonajeMoverDerecha(int desplazamiento);
    void PersonajeMoverArriba(int desplazamiento);
    void PersonajeMoverAbajo(int desplazamiento);
    int GetVelocidadX();
    int GetVelocidadY();
    void FrenarX();
    void FrenarY();
    int GetOffsetX();
    int GetOffsetY();
    void SetEstado(int estado);
    Personaje(const Personaje& orig);
    virtual ~Personaje();
private:

};

#endif	/* PERSONAJE_H */

