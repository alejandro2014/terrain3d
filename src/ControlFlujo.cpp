/*
 * ControlFlujo.cpp
 *
 *  Created on: 22 Nov 2009
 *      Author: alejandro
 */

#include "ControlFlujo.h"

ControlFlujo::ControlFlujo() {
	ventana = new Ventana();
	juego = new Juego(ventana);
	juego->bucleJuego();
}

ControlFlujo::~ControlFlujo() {
	delete ventana;
	delete juego;
	SDL_Quit();
}
