/*********************************************************************
	Author: Leonardo Laguna Ruiz
   Date	: Sun, 24, Jul 2005
*********************************************************************/

#ifndef quine_H

#define quine_H

#include <hpgcc49.h>
#include "lista.h"

struct nodo;
struct tabla;

typedef struct quine quine;

struct quine
{

   char *escenciales;

   int *exact_sol;

   lista *lista0;

   lista *lista1;

   lista *lista_base;

   int n_imp;

   int n_min;

   char pflag;

   int plines;

   int solucion;

   char *strout;

   int variables;

   struct nodo *implicantes;
   struct nodo *soluciones;
   struct tabla *tablamn;
};

char *quine_concat(quine *const me, char *s1, char *s2);

void quine_crear_tabla(quine *const me);

void quine_exacta(quine *const me, int id, char *compuesto, int maxemp, int curemp);

void quine_insertar(quine *const me, int *n, char d);

void quine_obtener_implicantes(quine *const me, lista *l);

void quine_print_tabla(quine *const me);

void quine_reducir(quine *const me);

void quine_reducir_tabla(quine *const me);

/* Elimina las columnas y renglones cubiertos*/
char quine_reducir_tabla_cubierta(quine *const me);

char quine_reducir_tabla_escenciales(quine *const me);

char quine_tabla_vacia(quine *const me);

void quine_Init(quine *const me, int n);

void quine_Cleanup(quine *const me);

struct nodo *quine_getImplicantes(const quine *const me);

void quine_setImplicantes(quine *const me, struct nodo *p_nodo);

struct nodo *quine_getSoluciones(const quine *const me);

void quine_setSoluciones(quine *const me, struct nodo *p_nodo);

struct tabla *quine_getTablamn(const quine *const me);

void quine_setTablamn(quine *const me, struct tabla *p_tabla);

quine *quine_Create(int n);
void quine_Destroy(quine *const me);

#endif
