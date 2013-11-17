/*
 * Dcel.cpp
 *
 *  Created on: 22 Nov 2009
 *      Author: alejandro
 */

/*TODO
 * 2) Métodos de asignación de valores a los elementos
 * 3) Método de guardado de la dcel en un fichero (proceso inverso a Dcel(char* nombre)
 */
#include "Dcel.h"

using namespace std;

/******** Dcel vacía ********/
Dcel::Dcel(void) {
    lista_v = NULL; lista_a = NULL; lista_c = NULL;
    num_v = 0; num_a = 0; num_c = 0;
}

/******** Dcel vacía con el tamaño reservado ********/
Dcel::Dcel(int numVertices, int numCaras, int numAristas) {
	lista_v = new VERTICE[numVertices];
	lista_c = new CARA[numCaras];
	lista_a = new ARISTA[numAristas];
	num_v = numVertices; num_c = numCaras; num_a = numAristas;
	memset(lista_v, 0, num_v*sizeof(VERTICE));
	memset(lista_c, 0, num_c*sizeof(CARA));
	memset(lista_a, 0, num_a*sizeof(ARISTA));
	actual = 0;
}

Dcel::Dcel(const char *nombreFichero) {
	cargarDcel(nombreFichero);
	//cargarDcelBmp(nombreFichero);
}

/******** Carga de una dcel a partir del fichero ********/
/*TODO Paramaterizar los tipos de ficheros para que sean compatibles unos con
 * otros cuyos enteros y float tienen diferente longitud
 */
void Dcel::cargarDcel(const char *nombreFichero) {
	char *ficheroBruto = NULL;
	struct stat estadoFichero;
	long int tamanio = 0;
	char *desplaz = NULL;

    ifstream fichero(nombreFichero);
    int arinc, anter, sigui, geme, orig, cinc;
    int cont;

    //Lectura de la dcel en bruto para leer una sola vez de disco
    stat(nombreFichero, &estadoFichero);
    tamanio = estadoFichero.st_size;
    ficheroBruto = new char[tamanio];
    fichero.read((char *)ficheroBruto, tamanio);

    //Lectura del número de vértices, aristas y caras
    memcpy(&num_v,ficheroBruto , 4);
    memcpy(&num_c,ficheroBruto + 4 ,4);
    memcpy(&num_a,ficheroBruto + 8 ,4);

    /* Crea las listas de elementos */
    lista_v = new VERTICE[num_v];
    lista_a = new ARISTA[num_a];
    lista_c = new CARA[num_c];

    desplaz = ficheroBruto + 12;
    /* Lee los vertices del fichero */
    for(cont = 0; cont < num_v; cont++) {
    	memcpy(lista_v + cont,cont*16 + desplaz,12);
    	memcpy(&arinc,cont*16 + desplaz + 12,sizeof(int));
    	(lista_v + cont)->aris_incid = lista_a + arinc;
    }
    desplaz += num_v*16;

    /* Lee las caras del fichero */
    for(cont = 0; cont < num_c; cont++) {
        memcpy(&arinc,cont*4 + desplaz,sizeof(int));
        (lista_c + cont)->aris_incid = lista_a + arinc;
    }
    desplaz += num_c*4;

    /* Lee las aristas del fichero */
    for(cont = 0; cont < num_a; cont++) {
    	memcpy(&anter,cont*20 + desplaz,sizeof(int));
    	memcpy(&sigui,cont*20 + desplaz + 4,sizeof(int));
    	memcpy(&geme,cont*20 + desplaz + 8,sizeof(int));
    	memcpy(&orig,cont*20 + desplaz + 12,sizeof(int));
    	memcpy(&cinc,cont*20 + desplaz + 16,sizeof(int));

        (lista_a + cont)->anterior = lista_a + anter;
        (lista_a + cont)->siguiente = lista_a + sigui;
        (lista_a + cont)->gemela = lista_a + geme;
        (lista_a + cont)->origen = lista_v + orig;
        (lista_a + cont)->cara_incid = lista_c + cinc;
    }

    /* Cierra el fichero */
    fichero.close();
    delete ficheroBruto;
}

/******** Carga de una dcel a partir de un fichero bmp (mapa de alturas) ********/
/*void Dcel::cargarDcelBmp(const char *nombreFichero) {
	ifstream fichero(nombreFichero);
	unsigned char *ficheroBruto;
	long int tamanio = 0;
	struct stat estadoFichero;
	int indice;
	GLfloat altura = 0;
	long int desplaz = 0;
	unsigned int i, j;
	int desplArista;
	int desplBordes;
	//long int mitadAristas;

	VERTICE *vertActual = NULL;
	ARISTA *arisActual = NULL;

	ARISTA *bordeSup = NULL;
	ARISTA *bordeInf = NULL;
	ARISTA *bordeIzq = NULL;
	ARISTA *bordeDer = NULL;

	//Lectura del fichero a un buffer de memoria
	stat(nombreFichero, &estadoFichero);
	tamanio = estadoFichero.st_size;
	ficheroBruto = new unsigned char[tamanio];
	fichero.read((char *)ficheroBruto, tamanio);
	fichero.close();

	memcpy(&ancho,ficheroBruto + 18,4);
	memcpy(&alto,ficheroBruto + 22,4);

	num_v = ancho * alto;
	ancho--; alto--; //Para calcular el resto de elementos
	num_c = 2*ancho*alto + 1;
	num_a = 3*ancho*alto + 2*ancho + 2*alto;

	lista_v = new VERTICE[num_v];
	lista_c = new CARA[num_c];
	lista_a = new ARISTA[num_a];

	bordeSup = new ARISTA[ancho];
	bordeInf = new ARISTA[ancho];
	bordeIzq = new ARISTA[alto];
	bordeDer = new ARISTA[alto];
	ancho++; alto++; //Restaura los valores originales

	//Determinación de las coordenadas de los vértices
	for(i = 0; i < alto; i++) {
		for(j = 0; j < ancho; j++) {
			indice = ancho * i + j;
			vertActual = lista_v + indice;

			vertActual->x = (GLfloat)i * 0.125;
			vertActual->y = (GLfloat)j * 0.125;

			desplaz = (num_v + j - ancho*(i + 1))*3 + 56;
			altura = (GLfloat)*(ficheroBruto + desplaz);
			vertActual->z = altura/64;
		}
	}

	//Determinación de los bordes de la dcel (aristas externas)
	for(i = 0; i < ancho; i++) {
		bordeSup + i = lista_a + i;
		(bordeSup + i)->cara_incid = lista_c;
		(bordeSup + i)->origen = lista_v + i;
	}

	for(i = 0; i < ancho; i++) {
		bordeInf + i = lista_a + ancho + i;
		(bordeInf + i)->cara_incid = lista_c;
		(bordeInf + i)->origen = lista_v + ((i - ancho + 1)*ancho) - 1;
	}

	for(i = 0; i < alto; i++) {
		bordeIzq + i = lista_a + ancho + alto + i;
		(bordeIzq + i)->cara_incid = lista_c;
		(bordeIzq + i)->origen = lista_v + ();
	}

	for(i = 0; i < alto; i++) {
		bordeDer + i = lista_a + 2*ancho + alto + i;
		(bordeDer + i)->cara_incid = lista_c;
		(bordeDer + i)->origen = ;
	}*/
	//Se averiguan los bordes del "marco" que representa el terreno
	/*for(i = 0; i < ancho; i++) {

	}*/
	/*lista_c->aris_incid = lista_a + num_a - 1;
	for(i = 1; i < num_c; i++) {
		desplArista = (i-1)/3;
		(lista_c + i)->aris_incid = lista_a + desplArista;
		(lista_a + desplArista)->cara_incid = lista_c + i;

		arisActual = (lista_c + i)->aris_incid;
		arisActual->siguiente = arisActual + 1;
		arisActual->anterior = arisActual + 2;

		(arisActual + 1)->siguiente = arisActual + 2;
		(arisActual + 1)->anterior = arisActual;

		(arisActual + 2)->siguiente = arisActual;
		(arisActual + 2)->anterior = arisActual + 1;
	}*/
	/*delete bordeSup; delete bordeInf;
	delete bordeIzq; delete bordeDer;
}*/

/******** Método para mostrar la información de la dcel en pantalla ********/
void Dcel::mostrarInfoTexto(void) {
    int contador, aris;
    int ant, sig, gem, ori, car;

    cout << "INFORMACION DE LA DCEL" << endl;
    cout << num_v << " vértices, " << num_c << " caras, " << num_a << " aristas" << endl;
    cout << "----------------------" << endl;
    cout << "@Tabla de vertices" << endl;
    for(contador = 0; contador < num_v; contador++) {
        aris = aris2int((lista_v + contador)->aris_incid,lista_a);
        cout << contador << ") " << (lista_v + contador)->x << " " << (lista_v + contador)->y << " " <<
                        (lista_v + contador)->z << " " << aris << endl;
    }

    cout << "@Tabla de caras" << endl;
    cout << "---------------" << endl;
    for(contador = 0; contador < num_c; contador++) {
        aris = aris2int((lista_c + contador)->aris_incid,lista_a);
        cout << contador << ") " << aris << endl;
    }

    cout << "@Tabla de aristas" << endl;
    cout << "-----------------" << num_a << endl;
    for(contador = 0; contador < num_a; contador++) {
        ant = aris2int((lista_a + contador)->anterior,lista_a);
        sig = aris2int((lista_a + contador)->siguiente,lista_a);
        gem = aris2int((lista_a + contador)->gemela,lista_a);
        ori = vert2int((lista_a + contador)->origen,lista_v);
        car = cara2int((lista_a + contador)->cara_incid,lista_c);

        cout << contador << ") " << ant << " " << sig << " " << gem << " " << ori << " " << car
			 << " (" << vert2int((lista_a + contador)->origen,lista_v) << ","
			 << vert2int((lista_a + contador)->gemela->origen,lista_v) << ")" << endl;
    }

}

/******** Dibujado de los vértices de la dcel en las coordenadas indicadas ********/
void Dcel::dibujarVertices(float obx, float oby, float obz) {
	int contador;
	glColor3f(1.0,1.0,0.0);
	glBegin(GL_POINTS);
		for(contador = 0; contador < num_v; contador++) {
			glVertex3f((lista_v + contador)->x + obx,
					   (lista_v + contador)->y + oby,
					   (lista_v + contador)->z + obz);
		}
	glEnd();
}

/******** Dibujado de las aristas de la dcel en las coordenadas indicadas ********/
void Dcel::dibujarAristas(float obx, float oby, float obz) {
    int contador;
    int arDibujadas = num_a >> 1;

    //glColor3f(1.0,1.0,0.0);
    glBegin(GL_LINES);
    for(contador = 0; contador < arDibujadas; contador++) {
    	if(contador == actual || contador + 18 == actual)
    		glColor3f(1.0,0.0,0.0);
    	else
    		glColor3f(0.0,0.0,0.0);

        glVertex3f((lista_a + contador)->origen->x + obx,
                   (lista_a + contador)->origen->y + oby,
                   (lista_a + contador)->origen->z + obz);

        glVertex3f((lista_a + contador)->gemela->origen->x + obx,
                   (lista_a + contador)->gemela->origen->y + oby,
                   (lista_a + contador)->gemela->origen->z + obz);
    }
    glEnd();
}

/******** Dibujado de las caras de la dcel en las coordenadas indicadas ********/
void Dcel::dibujarCaras(float obx, float oby, float obz) {
	ARISTA *arisIncid = NULL;

	glBegin(GL_TRIANGLES);
		for(int contador = 0; contador < num_c; contador++) {
			arisIncid = (lista_c + contador)->aris_incid;
			glColor3f(0.0,0.0,0.0);
			glVertex3f(arisIncid->origen->x + obx,
					   arisIncid->origen->y + oby,
					   arisIncid->origen->z + obz);

			glColor3f(1.0,0.0,0.0);
			glVertex3f(arisIncid->siguiente->origen->x + obx,
					   arisIncid->siguiente->origen->y + oby,
					   arisIncid->siguiente->origen->z + obz);

			glColor3f(1.0,1.0,0.0);
			glVertex3f(arisIncid->anterior->origen->x + obx,
					   arisIncid->anterior->origen->y + oby,
					   arisIncid->anterior->origen->z + obz);
		}
	glEnd();
}

/******** Dibujado de la dcel en forma cuadriculada ********/
void Dcel::dibujarCuadricula() {
	long int indice = 0;

	glColor3f(0.6,0.6,0.0);
	glBegin(GL_LINES);
	for(unsigned int i = 0; i < alto; i++) {
		for(unsigned int j = 0; j < ancho; j++) {
			indice = ancho*i + j;

			//Líneas horizontales
			if(indice % ancho) {
				glVertex3f((lista_v + indice)->x,
						   (lista_v + indice)->y,
						   (lista_v + indice)->z);
				glVertex3f((lista_v + indice - 1)->x,
						   (lista_v + indice - 1)->y,
						   (lista_v + indice - 1)->z);
			}

			//Líneas verticales
			if (i < alto - 1) {
				glVertex3f((lista_v + indice)->x,
						   (lista_v + indice)->y,
						   (lista_v + indice)->z);
				glVertex3f((lista_v + indice + ancho)->x,
						   (lista_v + indice + ancho)->y,
						   (lista_v + indice + ancho)->z);
			}
		}
	}
	glEnd();
}

/******** Establecimiento de los atributos de vértices, caras y aristas ********/
void Dcel::setVertice(int numV, float x, float y, float z, int aIncid) {
	(lista_v + numV)->x = x;
	(lista_v + numV)->y = y;
	(lista_v + numV)->z = z;
	(lista_v + numV)->aris_incid = lista_a + aIncid;
}

void Dcel::setCara(int numC, int aIncid) {
	(lista_c + numC)->aris_incid = lista_a + aIncid;
}

void Dcel::setArista(int numA, int aAnterior, int aSiguiente,
					 int aGemela, int vOrigen, int cIncid) {
	(lista_a + numA)->anterior = lista_a + aAnterior;
	(lista_a + numA)->siguiente = lista_a + aSiguiente;
	(lista_a + numA)->gemela = lista_a + aGemela;
	(lista_a + numA)->origen = lista_v + vOrigen;
	(lista_a + numA)->cara_incid = lista_c + cIncid;
}

/*OBS**** Cambia la arista seleccionada ********/
void Dcel::incActual() {
	actual++;
	actual = (actual == num_a) ? 0 : actual;
}

/******** Guardado de una dcel en un fichero ********/
void Dcel::guardar(const char *nombreFichero) {
	long int tamanio = num_v*16 + num_c*4 + num_a*20 + 12;
	long int desplaz = 12;
	char *ficheroBruto = new char[tamanio];

	int arinc, anter, sigui, geme, orig, cinc;
	int i;

	//Copia de la cabecera de la dcel
	memcpy(ficheroBruto,&num_v,4);
	memcpy(ficheroBruto + 4,&num_c,4);
	memcpy(ficheroBruto + 8,&num_a,4);

	//Relleno del buffer en bruto
	for(i = 0; i < num_v; i++) {
		memcpy(ficheroBruto + desplaz, lista_v + i, 12);
		arinc = aris2int((lista_v + i)->aris_incid,lista_a);
		memcpy(ficheroBruto + desplaz + 12, &arinc, 4);
		desplaz += 16;
	}

	for(i = 0; i < num_c; i++) {
		arinc = aris2int((lista_c + i)->aris_incid, lista_a);
		memcpy(ficheroBruto + desplaz,&arinc,4);
		desplaz += 4;
	}

	for(i = 0; i < num_a; i++) {
		anter = aris2int((lista_a + i)->anterior, lista_a);
		sigui = aris2int((lista_a + i)->siguiente, lista_a);
		geme = aris2int((lista_a + i)->gemela, lista_a);
		orig = vert2int((lista_a + i)->origen, lista_v);
		cinc = cara2int((lista_a + i)->cara_incid, lista_c);
		memcpy(ficheroBruto + desplaz,&anter,4);
		memcpy(ficheroBruto + desplaz + 4,&sigui,4);
		memcpy(ficheroBruto + desplaz + 8,&geme,4);
		memcpy(ficheroBruto + desplaz + 12,&orig,4);
		memcpy(ficheroBruto + desplaz + 16,&cinc,4);
		desplaz += 20;
	}

	//Guardado del fichero
	ofstream fichero(nombreFichero);
	fichero.write((const char *)ficheroBruto,tamanio);
	fichero.close();
	delete ficheroBruto;
}

Dcel::~Dcel() {
    delete [] lista_v;
    delete [] lista_a;
    delete [] lista_c;
    lista_v = NULL;
    lista_a = NULL;
    lista_c = NULL;
}
