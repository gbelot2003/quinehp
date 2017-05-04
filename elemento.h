/*********************************************************************
	Author : Leonardo Laguna Ruiz
   Date   : Sun, 24, Jul 2005
*********************************************************************/

#ifndef elemento_H

#define elemento_H

#include <hpgcc49.h>

struct nodo;

typedef struct elemento elemento;

struct elemento
{

   char *binario;

   char estado;

   char id;

   struct nodo *miniterminos;
};

void elemento_insertar_mini(elemento *me, int *n);

void elemento_Init(elemento *me, char *s, char d);

void elemento_Cleanup(elemento *me);

struct nodo *elemento_getMiniterminos(const elemento *me);

void elemento_setMiniterminos(elemento *me, struct nodo *p_nodo);

elemento *elemento_Create(char *s, char d);
void elemento_Destroy(elemento *me);

#endif
