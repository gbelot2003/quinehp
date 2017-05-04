/*********************************************************************
	Author: Leonardo Laguna Ruiz
   Date	: Sun, 24, Jul 2005
*********************************************************************/

#include "nodo.h"

static void cleanUpRelations(nodo *me);

void nodo_Init(nodo *me)
{
   me->dato = NULL;
   me->itsNodo = NULL;
}

void nodo_Cleanup(nodo *me)
{
   {
      nodo *nodo_t, *nodo_h;

      nodo_t = me->itsNodo;
      while (nodo_t != NULL)
      {
         nodo_h = nodo_t->itsNodo;
         free(nodo_t);
         nodo_t = nodo_h;
      }
   }
   cleanUpRelations(me);
}

int nodo_insertar(nodo *me, nodo **medir, void *d)
{
   {
      nodo *nuevo, *nodo_t;
      nuevo = NULL;

      //crea el nodo y lo inicializa
      nuevo = nodo_Create();
      if (nuevo == NULL)
         return 0;
      nuevo->dato = d;
      nuevo->itsNodo = 0;

      if (me == NULL)
         *medir = nuevo;
      else
      {
         nodo_t = me;
         while (nodo_t->itsNodo != NULL)
            nodo_t = nodo_t->itsNodo;
         nodo_t->itsNodo = nuevo;
      }

      return 1;
   }
}

struct nodo *nodo_getItsNodo(const nodo *me)
{
   return (struct nodo *)me->itsNodo;
}

void nodo_setItsNodo(nodo *me, struct nodo *p_nodo)
{
   me->itsNodo = p_nodo;
}

nodo *nodo_Create()
{
   nodo *me = (nodo *)malloc(sizeof(nodo));
   if (me != NULL)
   {
      nodo_Init(me);
   }
   return me;
}

void nodo_Destroy(nodo *me)
{
   if (me != NULL)
   {
      nodo_Cleanup(me);
   }
   free(me);
}

static void cleanUpRelations(nodo *me)
{
   if (me->itsNodo != NULL)
   {
      me->itsNodo = NULL;
   }
}
