/*********************************************************************
	Author: Leonardo Laguna Ruiz
   Date	: Sun, 24, Jul 2005
*********************************************************************/

#ifndef tabla_H

#define tabla_H

#include <hpgcc49.h>
#include "elemento.h"

typedef struct tabla tabla;

/* estructura*/
/*                      | miniterminos |*/
/*                      |  estado      |*/
/* elementos|  estado   |      X       |*/

struct tabla
{

   char *estado_imp;

   char *estado_min;

   elemento **implicantes;

   char **matriz;

   int **miniterminos;

   int n_imp;

   int n_min;

   char *strout;
};

char *tabla_concat(tabla *const me, char *s1, char *s2);

void tabla_print(tabla *const me);

void tabla_Init(tabla *const me, int m, int i);

void tabla_Cleanup(tabla *const me);

tabla *tabla_Create(int m, int i);
void tabla_Destroy(tabla *const me);

#endif
