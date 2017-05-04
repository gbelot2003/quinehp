/*********************************************************************
	Author: Leonardo Laguna Ruiz
   Date	: Wed, 27, Jul 2005
*********************************************************************/

#include "quine.h"
#include "nodo.h"
#include "tabla.h"

static void cleanUpRelations(quine *const me);

void quine_Init(quine *const me, int n)
{
   me->tablamn = NULL;
   me->soluciones = NULL;
   me->implicantes = NULL;
   me->escenciales = 0;
   me->exact_sol = 0;
   me->lista0 = 0;
   me->lista1 = 0;
   me->lista_base = 0;
   me->n_imp = 0;
   me->n_min = 0;
   me->pflag = 1;
   me->plines = 0;
   me->solucion = 0;
   me->strout = 0;
   me->variables = 0;
   {
      me->variables = n;
      me->lista_base = lista_Create(n);
   }
}

void quine_Cleanup(quine *const me)
{
   {
      lista_Destroy(me->lista0);
      lista_Destroy(me->lista_base);
      nodo_Destroy(me->implicantes);
      tabla_Destroy(me->tablamn);
   }
   cleanUpRelations(me);
}

char *quine_concat(quine *const me, char *s1, char *s2)
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

void quine_crear_tabla(quine *const me)
{
   {
      int i, j;
      nodo *nodo_t, *nodo_h;
      elemento *elem;
      paquete *pack;

      me->tablamn = tabla_Create(me->n_min, me->n_imp);

      nodo_t = me->implicantes;

      for (i = 0; i < me->n_imp; i++)
      {
         me->tablamn->implicantes[i] = nodo_t->dato;
         nodo_t = nodo_t->itsNodo;
      }

      i = 0;
      nodo_t = me->lista_base->paquetes;
      while (nodo_t != NULL)
      {
         pack = nodo_t->dato;

         nodo_h = pack->elementos;
         while (nodo_h != 0)
         {
            elem = nodo_h->dato;
            if (elem->id == 0)
            {
               me->tablamn->miniterminos[i] = elem->miniterminos->dato;
               i++;
            }
            nodo_h = nodo_h->itsNodo;
         }
         nodo_t = nodo_t->itsNodo;
      }
      for (i = 0; i < me->n_imp; i++)
         for (j = 0; j < me->n_min; j++)
            me->tablamn->matriz[i][j] = 0x30;

      for (i = 0; i < me->n_imp; i++)
      {
         nodo_t = me->tablamn->implicantes[i]->miniterminos;
         while (nodo_t != NULL)
         {
            for (j = 0; j < me->n_min; j++)
               if (nodo_t->dato == me->tablamn->miniterminos[j])
               {
                  me->tablamn->matriz[i][j] = 0x31;
               }
            nodo_t = nodo_t->itsNodo;
         }
      }
   }
}

void quine_exacta(quine *const me, int id, char *compuesto, int maxemp, int curemp)
{
   {
      int i, j, k, t, n;
      int flag, flag2, contrib;
      char *comp;
      char *sol;

      comp = malloc(me->n_min);

      for (i = id; i < me->n_imp; i++)
      {
         t = i;
         flag = curemp;
         if (me->tablamn->estado_imp[i] == 1)
         {
            contrib = 0;
            for (j = 0; j < me->n_min; j++)
            {
               if (me->tablamn->estado_min[j] == 1)
               {
                  if (me->tablamn->matriz[i][j] == 0x31 && compuesto[j] == 0x30)
                     contrib = 1;
                  if (me->tablamn->matriz[i][j] == 0x31 && compuesto[j] == 0x31)
                  {
                     flag++;
                     if (flag > maxemp)
                        break;
                  }
               }
               else
                  comp[j] = 'X';
            }
            if (flag <= maxemp && contrib == 1)
            {
               me->exact_sol[t] = 1;
               for (j = 0; j < me->n_min; j++)
                  if (me->tablamn->estado_min[j] == 1)
                     if (compuesto[j] == 0x31 || me->tablamn->matriz[i][j] == 0x31)
                        comp[j] = 0x31;
                     else
                        comp[j] = 0x30;
                  else
                     comp[j] = 'X';

               flag2 = 0;
               for (k = 0; k < me->n_min; k++)
                  if (comp[k] == 0x30 && me->tablamn->estado_min[k] != 0)
                  {
                     flag2 = 1;
                     break;
                  }
               if (flag2 == 0)
               {
                  sol = malloc(me->n_imp);
                  for (k = 0; k < me->n_imp; k++)
                  {
                     sol[k] = me->exact_sol[k];
                  }
                  nodo_insertar(me->soluciones, &me->soluciones, sol);
                  me->solucion = 1;
               }
               else if (flag <= maxemp)
               {
                  n = i + 1;
                  while (me->tablamn->estado_imp[n] == 0 && n < me->n_imp)
                     n++;
                  if (n < me->n_imp)
                     quine_exacta(me, n, comp, maxemp, flag);
               }
            }
         }
         me->exact_sol[t] = 0;
      }

      if (comp)
         free(comp);
   }
}

void quine_insertar(quine *const me, int *n, char d)
{
   {
      lista_insertar_mini(me->lista_base, n, d);
      if (d == 0)
         me->n_min++;
   }
}

void quine_obtener_implicantes(quine *const me, lista *l)
{
   {
      nodo *nodo_t, *nodo_h;
      elemento *elem;
      paquete *pack;

      nodo_t = l->paquetes;
      while (nodo_t != NULL)
      {
         pack = nodo_t->dato;

         nodo_h = pack->elementos;
         while (nodo_h != 0)
         {
            elem = nodo_h->dato;
            if (elem->estado == 0)
            {
               nodo_insertar(me->implicantes, &me->implicantes, elem);
               me->n_imp = me->n_imp + 1;
            }
            nodo_h = nodo_h->itsNodo;
         }
         nodo_t = nodo_t->itsNodo;
      }
   }
}

void quine_print_tabla(quine *const me)
{
   {
      me->tablamn->strout = me->strout;
      tabla_print(me->tablamn);
      me->strout = me->tablamn->strout;
   }
}

void quine_reducir(quine *const me)
{
   {
      char buff[150];
      char zbuff[30];
      int ibuff;
      int flag, i, j, nterm, iterm, k;
      nodo *sol;
      char *csol, *bin;

      ibuff = 0;

      printf("  Finding prime implicants\n");

      flag = lista_reducir_lista(me->lista_base, &(me->lista0));

      if (me->pflag == 1)
      {
         me->lista_base->strout = me->strout;
         lista_print(me->lista_base);
         me->strout = me->lista_base->strout;
      }
      quine_obtener_implicantes(me, me->lista_base);

      while (flag != 0)
      {
         flag = lista_reducir_lista(me->lista0, &(me->lista1));

         if (me->pflag == 1)
         {
            me->lista0->strout = me->strout;
            lista_print(me->lista0);
            me->strout = me->lista0->strout;
         }

         quine_obtener_implicantes(me, me->lista0);
         lista_Destroy(me->lista0);
         me->lista0 = me->lista1;
      }

      quine_crear_tabla(me);
      quine_reducir_tabla(me);

      if (me->solucion)
      {
         sol = me->soluciones;
         csol = sol->dato;
         nterm = 0;
         for (i = 0; i < me->n_imp; i++)
            if (csol[i] == 1)
               nterm++;
         if (nterm > 0)
         {
            sprintf(zbuff, "\nMinimized form:\n\n");
            k = 0;
            while (zbuff[k] != 0)
            {
               buff[ibuff] = zbuff[k];
               k++;
               ibuff++;
               if (ibuff > 120)
               {
                  buff[ibuff] = 0;
                  ibuff = 0;
                  me->strout = quine_concat(me, me->strout, buff);
               }
            }
            iterm = 0;
            for (i = 0; i < me->n_imp; i++)
            {
               if (csol[i] == 1)
               {
                  iterm++;
                  if (iterm != 1)
                  {
                     buff[ibuff] = ' ';
                     ibuff++;
                     buff[ibuff] = '+';
                     ibuff++;
                     buff[ibuff] = ' ';
                     ibuff++;
                  }
                  sprintf(zbuff, "P%i", i);
                  k = 0;
                  while (zbuff[k] != 0)
                  {
                     buff[ibuff] = zbuff[k];
                     k++;
                     ibuff++;
                     if (ibuff > 120)
                     {
                        buff[ibuff] = 0;
                        ibuff = 0;
                        me->strout = quine_concat(me, me->strout, buff);
                     }
                  }
               }
            }
            buff[ibuff] = '\n';
            ibuff++;
            iterm = 0;
            for (i = 0; i < me->n_imp; i++)
            {
               if (csol[i] == 1)
               {
                  iterm++;
                  if (iterm != 1)
                  {
                     buff[ibuff] = ' ';
                     ibuff++;
                     buff[ibuff] = '+';
                     ibuff++;
                     buff[ibuff] = ' ';
                     ibuff++;
                  }
                  bin = me->tablamn->implicantes[i]->binario;
                  for (j = 0; j < me->variables; j++)
                  {
                     switch (bin[j])
                     {
                     case '0':
                        sprintf(zbuff, "%c'", 'A' + j);
                        break;
                     case '1':
                        sprintf(zbuff, "%c", 'A' + j);
                        break;
                     default:
                        sprintf(zbuff, "");
                        break;
                     }
                     k = 0;
                     while (zbuff[k] != 0)
                     {
                        buff[ibuff] = zbuff[k];
                        k++;
                        ibuff++;
                        if (ibuff > 120)
                        {
                           buff[ibuff] = 0;
                           ibuff = 0;
                           me->strout = quine_concat(me, me->strout, buff);
                        }
                     }
                  }
               }
            }
         }
         if (sol->itsNodo != NULL)
            if (nterm > 0)
            {
               sprintf(zbuff, "\n\n+\n\n");
               k = 0;
               while (zbuff[k] != 0)
               {
                  buff[ibuff] = zbuff[k];
                  k++;
                  ibuff++;
                  if (ibuff > 120)
                  {
                     buff[ibuff] = 0;
                     ibuff = 0;
                     me->strout = quine_concat(me, me->strout, buff);
                  }
               }
            }
            else
            {
               sprintf(zbuff, "\nMinimized forms:\n\n");
               k = 0;
               while (zbuff[k] != 0)
               {
                  buff[ibuff] = zbuff[k];
                  k++;
                  ibuff++;
                  if (ibuff > 120)
                  {
                     buff[ibuff] = 0;
                     ibuff = 0;
                     me->strout = quine_concat(me, me->strout, buff);
                  }
               }
            }
         while (sol->itsNodo != NULL)
         {

            sol = sol->itsNodo;
            csol = sol->dato;
            nterm = 0;
            for (i = 0; i < me->n_imp; i++)
               if (csol[i] == 1)
                  nterm++;
            iterm = 0;
            for (i = 0; i < me->n_imp; i++)
            {
               if (csol[i] == 1)
               {
                  iterm++;
                  if (iterm != 1)
                  {
                     buff[ibuff] = ' ';
                     ibuff++;
                     buff[ibuff] = '+';
                     ibuff++;
                     buff[ibuff] = ' ';
                     ibuff++;
                  }
                  sprintf(zbuff, "P%i", i);
                  k = 0;
                  while (zbuff[k] != 0)
                  {
                     buff[ibuff] = zbuff[k];
                     k++;
                     ibuff++;
                     if (ibuff > 120)
                     {
                        buff[ibuff] = 0;
                        ibuff = 0;
                        me->strout = quine_concat(me, me->strout, buff);
                     }
                  }
               }
            }
            buff[ibuff] = '\n';
            ibuff++;
            iterm = 0;
            for (i = 0; i < me->n_imp; i++)
            {
               if (csol[i] == 1)
               {
                  iterm++;
                  if (iterm != 1)
                  {
                     buff[ibuff] = ' ';
                     ibuff++;
                     buff[ibuff] = '+';
                     ibuff++;
                     buff[ibuff] = ' ';
                     ibuff++;
                  }
                  bin = me->tablamn->implicantes[i]->binario;
                  for (j = 0; j < me->variables; j++)
                  {
                     switch (bin[j])
                     {
                     case '0':
                        sprintf(zbuff, "%c'", 'A' + j);
                        break;
                     case '1':
                        sprintf(zbuff, "%c", 'A' + j);
                        break;
                     default:
                        sprintf(zbuff, "");
                        break;
                     }
                     k = 0;
                     while (zbuff[k] != 0)
                     {
                        buff[ibuff] = zbuff[k];
                        k++;
                        ibuff++;
                        if (ibuff > 120)
                        {
                           buff[ibuff] = 0;
                           ibuff = 0;
                           me->strout = quine_concat(me, me->strout, buff);
                        }
                     }
                  }
               }
            }
            buff[ibuff] = '\n';
            ibuff++;
            buff[ibuff] = '\n';
            ibuff++;
         }
         buff[ibuff] = 0;
         ibuff = 0;
         me->strout = quine_concat(me, me->strout, buff);
      }
   }
}

void quine_reducir_tabla(quine *const me)
{
   {
      int i, j;
      char red, red1, escen, cubier, vacia;
      char *comp;

      printf("  Reducing table\n");
      quine_print_tabla(me);
      vacia = 0;
      red = 1;
      while (red)
      {
         red = 0;
         escen = quine_reducir_tabla_escenciales(me);
         vacia = quine_tabla_vacia(me);
         if (vacia == 1)
            break;
         if (escen == 1)
            quine_print_tabla(me);
         red = escen;
         cubier = quine_reducir_tabla_cubierta(me);
         vacia = quine_tabla_vacia(me);
         if (vacia == 1)
            break;
         if (cubier == 1)
            quine_print_tabla(me);
         red = cubier || red;
         red1 |= red;
      }
      vacia = quine_tabla_vacia(me);
      if (vacia)
         me->solucion = 1;

      if (me->solucion == 0)
      {
         printf("  Serching for the best solution\n");
         i = 0;
         while (me->solucion == 0 && i < 10)
         {
            comp = malloc(me->n_min);
            for (j = 0; j < me->n_min; j++)
               comp[j] = 0x30;
            me->exact_sol = malloc(me->n_imp * sizeof(int));
            for (j = 0; j < me->n_imp; j++)
               me->exact_sol[j] = 0;
            quine_exacta(me, 0, comp, i, 0);

            if (comp)
               free(comp);
            if (me->exact_sol)
               free(me->exact_sol);
            i++;
         }
         if (me->solucion == 0)
         {
            printf("Unable to minimize");
         }
         else
         {
            printf("  Solution found\n");
         }
      }
      else
      {
         printf("  Solution found\n");
         me->solucion = 1;
      }
   }
}

char quine_reducir_tabla_cubierta(quine *const me)
{
   {
      int i, j, k;
      char **matriz;
      char flag;
      char retvalue;
      retvalue = 0;

      matriz = me->tablamn->matriz;

      for (i = 0; i < me->n_imp; i++)
      {
         if (me->tablamn->estado_imp[i] == 1)
            for (k = 0; k < me->n_imp; k++)
            {
               if (me->tablamn->estado_imp[k] != 0)
               {
                  flag = 0;
                  if (k != i)
                  {
                     for (j = 0; j < me->n_min; j++)
                        if (me->tablamn->estado_min[j] == 1 && matriz[i][j] == 0x30 && matriz[k][j] == 0x31)
                        {
                           flag = 1;
                           break;
                        }
                     if (flag == 0)
                     {
                        me->tablamn->estado_imp[k] = 0;
                        retvalue = 1;
                     }
                  }
               }
            }
      }

      for (j = 0; j < me->n_min; j++)
      {
         if (me->tablamn->estado_min[j] == 1)
            for (k = 0; k < me->n_min; k++)
            {
               if (me->tablamn->estado_min[k] == 1)
               {
                  flag = 0;
                  if (k != j)
                  {
                     for (i = 0; i < me->n_imp; i++)
                        if (me->tablamn->estado_imp[i] == 1 && matriz[i][j] == 0x30 && matriz[i][k] == 0x31)
                        {
                           flag = 1;
                           break;
                        }
                     if (flag == 0)
                     {
                        me->tablamn->estado_min[j] = 0;
                        retvalue = 1;
                     }
                  }
               }
            }
      }
      //Verifica si la tabla esta vacia y regresa
      return retvalue;
   }
}

char quine_reducir_tabla_escenciales(quine *const me)
{
   {
      int i, j, rep, k, term, i_imp, retvalue;
      char **matriz;

      if (me->escenciales == 0)
      {
         me->escenciales = malloc(me->n_imp);
         for (i = 0; i < me->n_imp; i++)
            me->escenciales[i] = 0;
         nodo_insertar(me->soluciones, &me->soluciones, me->escenciales);
      }

      retvalue = 0;

      matriz = me->tablamn->matriz;

      /// Primera pasada
      for (j = 0; j < me->n_min; j++)
      {
         if (me->tablamn->estado_min[j] == 1)
         {
            rep = 0;
            for (i = 0; i < me->n_imp; i++)
            {
               if (me->tablamn->estado_imp[i] == 1)
                  if (matriz[i][j] == 0x31)
                  {
                     rep++;
                     i_imp = i;
                  }
            }
            if (rep == 1)
            {
               me->escenciales[i_imp] = 1;
               retvalue = 1;
               term++;
               me->tablamn->estado_imp[i_imp] = 0;
               me->tablamn->estado_min[j] = 0;
               for (k = 0; k < me->n_min; k++)
                  if (matriz[i_imp][k] == 0x31)
                     me->tablamn->estado_min[k] = 0;
            }
         }
      }

      return retvalue;
   }
}

char quine_tabla_vacia(quine *const me)
{
   {
      int j;
      for (j = 0; j < me->n_min; j++)
         if (me->tablamn->estado_min[j] == 1)
            return 0;
      return 1;
   }
}

struct nodo *quine_getImplicantes(const quine *const me)
{
   return (struct nodo *)me->implicantes;
}

void quine_setImplicantes(quine *const me, struct nodo *p_nodo)
{
   me->implicantes = p_nodo;
}

struct nodo *quine_getSoluciones(const quine *const me)
{
   return (struct nodo *)me->soluciones;
}

void quine_setSoluciones(quine *const me, struct nodo *p_nodo)
{
   me->soluciones = p_nodo;
}

struct tabla *quine_getTablamn(const quine *const me)
{
   return (struct tabla *)me->tablamn;
}

void quine_setTablamn(quine *const me, struct tabla *p_tabla)
{
   me->tablamn = p_tabla;
}

quine *quine_Create(int n)
{
   quine *me = (quine *)malloc(sizeof(quine));
   if (me != NULL)
   {
      quine_Init(me, n);
   }
   return me;
}

void quine_Destroy(quine *const me)
{
   if (me != NULL)
   {
      quine_Cleanup(me);
   }
   free(me);
}

static void cleanUpRelations(quine *const me)
{
   if (me->implicantes != NULL)
   {
      me->implicantes = NULL;
   }
   if (me->soluciones != NULL)
   {
      me->soluciones = NULL;
   }
   if (me->tablamn != NULL)
   {
      me->tablamn = NULL;
   }
}
