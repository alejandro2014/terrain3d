/*
 * Ventana.cpp
 *
 *  Created on: 22 Nov 2009
 *      Author: alejandro
 */

#include "Ventana.h"
#include <math.h>

Ventana::Ventana() {
	done = 0;
	distancia = 5.0;
	heightFactor = 0.01;
	heightIncreaseFactor = 1;

	SDL_Init(SDL_INIT_VIDEO);

	screen = SDL_SetVideoMode(800, 600, 16, SDL_OPENGL|SDL_RESIZABLE);
	if (!screen) {
		fprintf(stderr, "No se puede inicializar el modo de vídeo: %s\n", SDL_GetError());
	    SDL_Quit();
	    exit(2);
	}
	SDL_WM_SetCaption("Prueba de opengl", "gears");

	reshape(screen->w, screen->h);
	glClearColor(0.5,0.5,0.5,0.0);

	//Como estamos fuera eliminamos las caras de dentro
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //Muestra sólo los polígonos que se pueden ver
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

Ventana::~Ventana() {

}

//
void Ventana::reshape(int width, int height) {
	GLfloat h = (GLfloat) height / (GLfloat) width;

	glViewport(0, 0, (GLint) width, (GLint) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -h, h, 5.0, 450.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -40.0);
}

void Ventana::dibujarEscena(GLfloat angAlfa, GLfloat angBeta, Dcel *dcel) {
	int i;
	int z = 0;
	float angle = 0.0;
	float radius = 1.0;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glTranslatef(0.0,0.0,distancia);
  glRotatef(angAlfa - 90,1.0,0.0,0.0);
  glRotatef(angBeta,0.0,0.0,1.0);

  dibujarEjes();
  
  glColor3f(0.8, 0.8, 0.0);
  glBegin(GL_LINE_STRIP);
  for(i = 0; i < 100; i++) {
	  angle += M_PI / 8;
	  glVertex3f(i * heightFactor, radius * cos(angle), radius * sin(angle));
	  radius -= 0.01;
  }
  glEnd();
  
  adjustHeightFactor();
  //dcel->dibujarVertices(0.0,0.0,0.0);
  //dcel->dibujarCuadricula();

  glPopMatrix();

  SDL_GL_SwapBuffers();
}

void Ventana::adjustHeightFactor() {
	if(heightIncreaseFactor == 1) {
		heightFactor += 0.01;
	} else {
		heightFactor -= 0.01;
	}
	
	if(heightFactor > 0.1) {
		heightIncreaseFactor = -1;
	} else if(heightFactor < 0.01) {
		heightIncreaseFactor = 1;
	}
}

void Ventana::dibujarEjes() {
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(1.0,0.0,0.0);

		glColor3f(0.0,1.0,0.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,1.0,0.0);

		glColor3f(0.0,0.0,1.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,0.0,1.0);
	glEnd();
}
