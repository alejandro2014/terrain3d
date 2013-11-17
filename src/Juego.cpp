/*
 * Juego.cpp
 *
 *  Created on: 22 Nov 2009
 *      Author: alejandro
 */

#include "Juego.h"

Juego::Juego(Ventana *vent) {
	this->ventana = vent;
	this->surface = ventana->getSurface();
	//this->bucleJuego();
	angAlfa = 0.0, angBeta = 0.0;
	acabar = false;

	//dcel = new Dcel("../cubo.dce");
	//dcel->mostrarInfoTexto();
}

Juego::~Juego() {
	// TODO Auto-generated destructor stub
}

void Juego::bucleJuego() {
	//Dcel *dcel = new Dcel("dcel_prueba.dat");


	while (!acabar) {
		procesarEventos(surface);
		procesarTeclado();

		if(!acabar) ventana->dibujarEscena(angAlfa, angBeta, dcel);

		this->espera();
	}
}

void Juego::procesarEventos(SDL_Surface *screen) {
	while (SDL_PollEvent(&evento)) {
		switch(evento.type) {
			case SDL_VIDEORESIZE:
				screen = SDL_SetVideoMode(evento.resize.w, evento.resize.h, 16,
                                    SDL_OPENGL|SDL_RESIZABLE);
				if (screen) {
					ventana->reshape(screen->w, screen->h);
				} else {
					/* Uh oh, we couldn't set the new video mode?? */
				}
				break;

			case SDL_QUIT:
				acabar = 1;
				break;
		}
	}
}

void Juego::procesarTeclado() {
	Uint8 *keys = SDL_GetKeyState(NULL);

	//TODO poner referencias en vez de punteros
    if (keys[SDLK_ESCAPE]) acabar = true;
    if (keys[SDLK_UP]) angAlfa += 5.0;
    if (keys[SDLK_DOWN]) angAlfa -= 5.0;
    if (keys[SDLK_LEFT]) angBeta += 5.0;
    if (keys[SDLK_RIGHT]) angBeta -= 5.0;
    if (keys[SDLK_q]) ventana->setDistancia(-2.0);
    if (keys[SDLK_a]) ventana->setDistancia(2.0);
    if (keys[SDLK_w]) dcel->incActual();
}

void Juego::espera(void) {
	usleep(100000);
}
