/*********************************************************************
	Author: Leonardo Laguna Ruiz
   Date	: Sun, 24, Jul 2005
*********************************************************************/

#include "lista.h"
#include "elemento.h"
#include "nodo.h"

static char *tobin(lista *me, int n);

static void cleanUpRelations(lista *me);

void lista_Init(lista *me, int n)
{
   me->paquetes = NULL;
   me->plines = 0;
   me->strout = 0;
   me->variables = 0;
   {
      me->variables = n;
   }
}

void lista_Cleanup(lista *me)
{
   {
      nodo_Destroy(me->paquetes);
   }
   cleanUpRelations(me);
}

char *lista_concat(lista *me, char *s1, char *s2)
{
   {
      int i, j;
      char *out;
      i = 0;
      j = 0;
      if (s1 != 0)
         while (s1[i] != 0)
         {
            i++;
            j++;
         }
      i = 0;
      while (s2[i] != 0)
      {
         i++;
         j++;
      }

      out = malloc(j + 1);
      out[j] = 0;

      i = 0;
      j = 0;

      if (s1 != 0)
         while (s1[i] != 0)
         {
            out[j] = s1[i];
            i++;
            j++;
         }
      i = 0;
      while (s2[i] != 0)
      {
         out[j] = s2[i];
         i++;
         j++;
      }

      if (s1)
         free(s1);
      return out;
   }
}

int lista_cuenta_unos(lista *me, char *s)
{
   {
      int i, j;
      i = 0;
      j = 0;

      while (s[i] != 0)
      {
         if (s[i] == '1')
            j++;
         i++;
      }
      return j;
   }
}

void lista_insertar_elem(lista *me, elemento *e, int unos)
{
   {
      paquete *pack;
      nodo *nodo_t;
      elemento *elem;
      int i, dif;

      nodo_t = me->paquetes;
      //buscar el paquete correspondiente al numero de unos
      while (nodo_t != NULL)
      {
         pack = nodo_t->dato;
         if (pack->num_unos == unos)
            break;
         nodo_t = nodo_t->itsNodo;
      }
      //si el paquete no existe, lo crea
      if (nodo_t == NULL)
      {
         pack = paquete_Create(unos);
         nodo_insertar(me->paquetes, &me->paquetes, pack);
      }
      //Verifica que el elemento no exista
      nodo_t = pack->elementos;
      while (nodo_t != NULL)
      {
         elem = nodo_t->dato;
         i = 0;
         dif = 0;
         while (elem->binario[i] != 0)
         {
            if (elem->binario[i] != e->binario[i])
               dif++;
            i++;
         }
         if (dif == 0)
            break;
         nodo_t = nodo_t->itsNodo;
      }

      if (nodo_t == NULL)
         nodo_insertar(pack->elementos, &pack->elementos, e);
   }
}

void lista_insertar_mini(lista *me, int *n, char d)
{
   {
      int unos;
      char *s;
      elemento *elem;

      s = tobin(me, *n);
      elem = elemento_Create(s, d);
      elemento_insertar_mini(elem, n);
      unos = lista_cuenta_unos(me, s);
      lista_insertar_elem(me, elem, unos);
   }
}

void lista_print(lista *me)
{
   {
      int i;
      char buff[100];
      paquete *pack;
      nodo *nodo_t, *nodo_h, *nodo_m;
      elemento *elem;
      for (i = 0; i <= me->variables; i++)
      {
         nodo_t = me->paquetes;
         while (nodo_t != NULL)
         {
            pack = nodo_t->dato;
            if (pack->num_unos == i)
            {
               sprintf(buff, " n:%i\n\0", i);
               me->strout = lista_concat(me, me->strout, buff);
               nodo_h = pack->elementos;

               while (nodo_h != NULL)
               {
                  elem = nodo_h->dato;
                  sprintf(buff, "%c%s:%c  \0", elem->id == 0 ? '+' : ' ', elem->binario, elem->estado == 1 ? '-' : '*');
                  me->strout = lista_concat(me, me->strout, buff);
                  nodo_m = elem->miniterminos;
                  while (nodo_m != NULL)
                  {
                     sprintf(buff, "%i \0", *(int *)(nodo_m->dato));
                     me->strout = lista_concat(me, me->strout, buff);
                     nodo_m = nodo_m->itsNodo;
                  }
                  sprintf(buff, "\n\0");
                  me->strout = lista_concat(me, me->strout, buff);
                  nodo_h = nodo_h->itsNodo;
               }
            }
            nodo_t = nodo_t->itsNodo;
         }
      }
      sprintf(buff, "|--------------------------|\n\0");
      me->strout = lista_concat(me, me->strout, buff);
   }
}

elemento *lista_reducir_elemento(lista *me, elemento *e1, elemento *e2)
{
   {
      char *a, *b, *c;
      elemento *nuevo;
      int i, dif;
      nodo *nodo_m;

      a = e1->binario;
      b = e2->binario;
      dif = 0;
      i = 0;

      c = malloc(me->variables + 1);
      c[me->variables] = 0;

      while (a[i] != 0)
      {
         if (a[i] != b[i])
         {
            c[i] = '-';
            dif = dif + 1;
         }
         else
         {
            c[i] = a[i];
         }
         i = i + 1;
      }
      if (dif > 1)
      {
         free(c);
         return 0;
      }
      nuevo = elemento_Create(c, 1);

      nodo_m = e1->miniterminos;
      while (nodo_m != NULL)
      {
         elemento_insertar_mini(nuevo, (int *)(nodo_m->dato));
         nodo_m = nodo_m->itsNodo;
      }
      nodo_m = e2->miniterminos;
      while (nodo_m != NULL)
      {
         elemento_insertar_mini(nuevo, (int *)(nodo_m->dato));
         nodo_m = nodo_m->itsNodo;
      }
      return nuevo;
   }
}

int lista_reducir_lista(lista *me, lista **nueva_lista)
{
   {
      int i, unos;
      paquete *a, *b;
      elemento *an, *bn, *cn;
      nodo *nodo_t, *nodo_h;

      *nueva_lista = NULL;
      for (i = 0; i < me->variables; i++)
      {
         //busca el paquete de i unos
         nodo_t = me->paquetes;
         while (nodo_t != NULL)
         {
            a = nodo_t->dato;
            if (a->num_unos == i)
               break;
            nodo_t = nodo_t->itsNodo;
         }
         if (nodo_t == NULL)
            a = NULL;
         //busca el paquete de i+1 unos

         nodo_t = me->paquetes;
         while (nodo_t != NULL)
         {
            b = nodo_t->dato;
            if (b->num_unos == i + 1)
               break;
            nodo_t = nodo_t->itsNodo;
         }
         if (nodo_t == NULL)
            b = NULL;
         //Verifica si existen los dos paquetes
         if (a != NULL && b != NULL)
         {
            nodo_t = a->elementos;
            //se hace un recorrido de todos los elementos
            //del paquete de i unos
            while (nodo_t != NULL)
            {
               an = nodo_t->dato;
               //cada elemento del paquete de i unos se
               //compara con cada elemento del paquete de i+1 unos
               nodo_h = b->elementos;
               while (nodo_h != NULL)
               {
                  bn = nodo_h->dato;
                  //copara los elementos si se pueden reducir genera c
                  cn = lista_reducir_elemento(me, an, bn);
                  //si se creo nuevo elemento lo inserta en la sig lista
                  if (cn != 0)
                  {
                     an->estado = 1;
                     bn->estado = 1;
                     unos = lista_cuenta_unos(me, cn->binario);
                     if (*nueva_lista == NULL)
                        *nueva_lista = lista_Create(me->variables);
                     lista_insertar_elem(*nueva_lista, cn, unos);
                  }
                  nodo_h = nodo_h->itsNodo;
               }

               nodo_t = nodo_t->itsNodo;
            }
         }
      }
      if (*nueva_lista)
         return 1;
      else
         return 0;
   }
}

static char *tobin(lista *me, int n)
{
   {
      int i;
      char *c;
      c = (char *)malloc(me->variables + 1);
      c[me->variables] = 0;
      for (i = me->variables - 1; i >= 0; i--)
      {
         c[i] = (n % 2) + 0x30;
         n = n / 2;
      }
      return c;
   }
}

struct nodo *lista_getPaquetes(const lista *me)
{
   return (struct nodo *)me->paquetes;
}

void lista_setPaquetes(lista *me, struct nodo *p_nodo)
{
   me->paquetes = p_nodo;
}

lista *lista_Create(int n)
{
   lista *me = (lista *)malloc(sizeof(lista));
   if (me != NULL)
   {
      lista_Init(me, n);
   }
   return me;
}

void lista_Destroy(lista *me)
{
   if (me != NULL)
   {
      lista_Cleanup(me);
   }
   free(me);
}

static void cleanUpRelations(lista *me)
{
   if (me->paquetes != NULL)
   {
      me->paquetes = NULL;
   }
}
