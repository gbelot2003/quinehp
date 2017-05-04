/*********************************************************************
	Author		: Leonardo Laguna Ruiz
   Date	      : Sun, 24, Jul 2005
*********************************************************************/

#include "elemento.h"
#include "nodo.h"

static void cleanUpRelations(elemento *me);

void elemento_Init(elemento *me, char *s, char d)
{
   me->miniterminos = NULL;
   me->binario = 0;
   me->estado = 0;
   me->id = 0;
   {
      me->binario = s;
      me->id = d;
   }
}

void elemento_Cleanup(elemento *me)
{
   {
      nodo_Destroy(me->miniterminos);
      free(me->binario);
   }
   cleanUpRelations(me);
}

void elemento_insertar_mini(elemento *me, int *n)
{
   {
      nodo_insertar(me->miniterminos, &me->miniterminos, (void *)n);
   }
}

struct nodo *elemento_getMiniterminos(const elemento *me)
{
   return (struct nodo *)me->miniterminos;
}

void elemento_setMiniterminos(elemento *me, struct nodo *p_nodo)
{
   me->miniterminos = p_nodo;
}

elemento *elemento_Create(char *s, char d)
{
   elemento *me = (elemento *)malloc(sizeof(elemento));
   if (me != NULL)
   {
      elemento_Init(me, s, d);
   }
   return me;
}

void elemento_Destroy(elemento *me)
{
   if (me != NULL)
   {
      elemento_Cleanup(me);
   }
   free(me);
}

static void cleanUpRelations(elemento *me)
{
   if (me->miniterminos != NULL)
   {
      me->miniterminos = NULL;
   }
}
