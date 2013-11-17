/*
 * Juego.h
 *
 *  Created on: 22 Nov 2009
 *      Author: alejandro
 */

#ifndef VENTANA_H_
#define VENTANA_H_
#include "cabecera.h"
#include "Dcel.h"
#include "Ventana.h"

class Ventana {
private:
	GLfloat angAlfa, angBeta;
	GLfloat distancia;
	SDL_Surface *screen;
	int done;
	float heightFactor;
	int heightIncreaseFactor;
public:
	Ventana();
	~Ventana();

	void reshape(int width, int height);
	void dibujarEscena(GLfloat angAlfa, GLfloat angBeta, Dcel *dcel);
	void dibujarEjes(void);

	SDL_Surface* getSurface() {return screen;}
	void setDistancia(GLfloat increm) {distancia -= increm;}
	void adjustHeightFactor();
};

#endif /* VENTANA_H_ */
