/*
 * Juego.h
 *
 *  Created on: 22 Nov 2009
 *      Author: alejandro
 */

#ifndef JUEGO_H_
#define JUEGO_H_
#include <unistd.h>
#include "cabecera.h"
#include "Dcel.h"
#include "Ventana.h"

class Juego {
	GLfloat angAlfa, angBeta;
	SDL_Event evento;
	SDL_Surface *surface;
	Ventana *ventana;
	Dcel *dcel;
	bool acabar;
public:
	Juego(Ventana *vent);
	~Juego();

	void bucleJuego();
	void procesarEventos(SDL_Surface *screen);
	void procesarTeclado();
	void espera(void);
};

#endif /* JUEGO_H_ */
