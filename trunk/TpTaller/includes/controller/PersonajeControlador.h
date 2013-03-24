/* 
 * File:   PersonajeControlador.h
 * Author: damian
 *
 * Created on 23 de marzo de 2013, 12:25
 */


#ifndef PERSONAJECONTROLADOR_H
#define	PERSONAJECONTROLADOR_H

#include <view/PersonajeVista.h>


class PersonajeControlador {
public:
    PersonajeControlador(Personaje* unPersonaje);
    void PersonajeVerificarQueEsteEnRango();
    void ControlarEventos();
    PersonajeControlador(const PersonajeControlador& orig);
    virtual ~PersonajeControlador();
private:
    Personaje* miPersonaje;

};

#endif	/* PERSONAJECONTROLADOR_H */

