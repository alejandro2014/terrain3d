/*
 * cabecera.h
 *
 *  Created on: 22 Nov 2009
 *      Author: alejandro
 */

#ifndef CABECERA_H_
#define CABECERA_H_
#include <GL/gl.h>
#include <SDL.h>

//Elementos de la dcel
struct ESTR_VERT {
    float x,y,z;
    struct ESTR_ARIS *aris_incid;
    bool eliminado;
};

struct ESTR_ARIS {
    struct ESTR_ARIS *anterior;
    struct ESTR_ARIS *siguiente;
    struct ESTR_ARIS *gemela;
    struct ESTR_VERT *origen;
    struct ESTR_CARA *cara_incid;
    bool eliminada;
};

struct ESTR_CARA {
    struct ESTR_ARIS *aris_incid;
    bool eliminada;
};

typedef struct ESTR_VERT VERTICE;
typedef struct ESTR_ARIS ARISTA;
typedef struct ESTR_CARA CARA;

//Elementos del bitmap
typedef struct {
	unsigned char r, g, b;
} PIXELBMP;

typedef struct {
	PIXELBMP *arrPixeles;
} BITMAP;

#endif /* CABECERA_H_ */
