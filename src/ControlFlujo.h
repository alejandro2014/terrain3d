/*
 * ControlFlujo.h -> Clase encargada de controlar el flujo del programa
 *
 *  Created on: 22 Nov 2009
 *      Author: alejandro
 */

#ifndef CONTROLFLUJO_H_
#define CONTROLFLUJO_H_
#include "cabecera.h"
#include "Juego.h"
#include "Ventana.h"

class ControlFlujo {
private:
	Juego *juego;
	Ventana *ventana;
public:
	ControlFlujo();
	~ControlFlujo();
};

#endif /* CONTROLFLUJO_H_ */
