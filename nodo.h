/*********************************************************************
	Author: Leonardo Laguna Ruiz
   Date	: Sun, 24, Jul 2005
*********************************************************************/

#ifndef nodo_H

#define nodo_H

#include <hpgcc49.h>

typedef struct nodo nodo;

struct nodo
{

   void *dato;

   struct nodo *itsNodo;
};

int nodo_insertar(nodo *me, nodo **medir, void *d);

void nodo_Init(nodo *me);

void nodo_Cleanup(nodo *me);

struct nodo *nodo_getItsNodo(const nodo *me);

void nodo_setItsNodo(nodo *me, struct nodo *p_nodo);

nodo *nodo_Create();
void nodo_Destroy(nodo *me);

#endif
