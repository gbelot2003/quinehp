/*********************************************************************
	Author: Leonardo Laguna Ruiz
   Date	: Sun, 24, Jul 2005
*********************************************************************/

#ifndef lista_H

#define lista_H

#include <hpgcc49.h>
#include "paquete.h"

struct elemento;
struct nodo;

typedef struct lista lista;

struct lista
{

   int plines;

   char *strout;

   int variables;

   struct nodo *paquetes;
};

char *lista_concat(lista *me, char *s1, char *s2);

int lista_cuenta_unos(lista *me, char *s);

void lista_insertar_elem(lista *me, elemento *e, int unos);

void lista_insertar_mini(lista *me, int *n, char d);

void lista_print(lista *me);

elemento *lista_reducir_elemento(lista *me, elemento *e1, elemento *e2);

int lista_reducir_lista(lista *me, lista **nueva_lista);

void lista_Init(lista *me, int n);

void lista_Cleanup(lista *me);

struct nodo *lista_getPaquetes(const lista *me);

void lista_setPaquetes(lista *me, struct nodo *p_nodo);

lista *lista_Create(int n);
void lista_Destroy(lista *me);

#endif
