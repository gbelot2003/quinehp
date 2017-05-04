/*********************************************************************
	Author: Leonardo Laguna Ruiz
   Date	: Sun, 24, Jul 2005
*********************************************************************/

#include "paquete.h"
#include "nodo.h"

static void cleanUpRelations(paquete *const me);

void paquete_Init(paquete *const me, int n)
{
   me->elementos = NULL;
   me->num_unos = 0;
   {
      me->num_unos = n;
   }
}

void paquete_Cleanup(paquete *const me)
{
   {
      nodo_Destroy(me->elementos);
   }
   cleanUpRelations(me);
}

struct nodo *paquete_getElementos(const paquete *const me)
{
   return (struct nodo *)me->elementos;
}

void paquete_setElementos(paquete *const me, struct nodo *p_nodo)
{
   me->elementos = p_nodo;
}

paquete *paquete_Create(int n)
{
   paquete *me = (paquete *)malloc(sizeof(paquete));
   if (me != NULL)
   {
      paquete_Init(me, n);
   }
   return me;
}

void paquete_Destroy(paquete *const me)
{
   if (me != NULL)
   {
      paquete_Cleanup(me);
   }
   free(me);
}

static void cleanUpRelations(paquete *const me)
{
   if (me->elementos != NULL)
   {
      me->elementos = NULL;
   }
}
