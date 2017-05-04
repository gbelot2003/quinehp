/*********************************************************************
	Author: Leonardo Laguna Ruiz
   Date	: Sun, 24, Jul 2005
*********************************************************************/

#ifndef paquete_H

#define paquete_H

#include <hpgcc49.h>
#include "elemento.h"

struct nodo;

typedef struct paquete paquete;

struct paquete
{

   int num_unos;

   struct nodo *elementos;
};

void paquete_Init(paquete *const me, int n);

void paquete_Cleanup(paquete *const me);

struct nodo *paquete_getElementos(const paquete *const me);

void paquete_setElementos(paquete *const me, struct nodo *p_nodo);

paquete *paquete_Create(int n);
void paquete_Destroy(paquete *const me);

#endif
