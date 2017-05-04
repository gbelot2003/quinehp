/*********************************************************************
	Author: Leonardo Laguna Ruiz
   Date	: Sun, 24, Jul 2005
*********************************************************************/

#include "tabla.h"

void tabla_Init(tabla *const me, int m, int i)
{
   {
      int j;
      me->n_min = m;
      me->n_imp = i;
      me->estado_min = malloc(m);
      me->estado_imp = malloc(i);
      me->miniterminos = malloc(m * sizeof(int *));
      me->implicantes = malloc(i * sizeof(elemento *));

      me->matriz = malloc(i * sizeof(char *));
      for (j = 0; j < i; j++)
      {
         me->matriz[j] = malloc(m);
      }

      for (j = 0; j < me->n_imp; j++)
         me->estado_imp[j] = 1;
      for (j = 0; j < me->n_min; j++)
         me->estado_min[j] = 1;
   }
}

void tabla_Cleanup(tabla *const me)
{
   {
      int j;

      free(me->estado_min);
      free(me->estado_imp);
      free(me->miniterminos);
      free(me->implicantes);

      for (j = 0; j < me->n_imp; j++)
      {
         free(me->matriz[j]);
      }
      free(me->matriz);
   }
}

char *tabla_concat(tabla *const me, char *s1, char *s2)
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

void tabla_print(tabla *const me)
{
   {
      int i, j, k;
      char zbuff[20];
      char buff[150];
      int ibuff;

      ibuff = 0;

      buff[ibuff] = '\n';
      ibuff++;

      i = 0;
      while (me->implicantes[0]->binario[i] != 0)
      {
         buff[ibuff] = ' ';
         ibuff++;
         i++;
      }
      buff[ibuff] = ' ';
      ibuff++;
      buff[ibuff] = ' ';
      ibuff++;
      buff[ibuff] = ' ';
      ibuff++;
      buff[ibuff] = ' ';
      ibuff++;
      buff[ibuff] = ' ';
      ibuff++;
      buff[ibuff] = ' ';
      ibuff++;

      for (j = 0; j < me->n_min; j++)
      {
         if (me->estado_min[j] != 0)
         {
            sprintf(zbuff, "%i ", *me->miniterminos[j]);
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
                  me->strout = tabla_concat(me, me->strout, buff);
               }
            }
         }
      }
      buff[ibuff] = '\n';
      ibuff++;

      for (i = 0; i < me->n_imp; i++)
      {
         if (me->estado_imp[i] != 0)
         {
            if (i > 9)
               sprintf(zbuff, "P%i %s: ", i, me->implicantes[i]->binario);
            else
               sprintf(zbuff, "P%i  %s: ", i, me->implicantes[i]->binario);
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
                  me->strout = tabla_concat(me, me->strout, buff);
               }
            }
            for (j = 0; j < me->n_min; j++)
            {
               if (me->estado_min[j] != 0)
               {
                  sprintf(zbuff, "%c ", me->matriz[i][j]);
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
                        me->strout = tabla_concat(me, me->strout, buff);
                     }
                  }
                  if (*me->miniterminos[j] >= 10)
                  {
                     buff[ibuff] = ' ';
                     ibuff++;
                  }
                  if (*me->miniterminos[j] >= 100)
                  {
                     buff[ibuff] = ' ';
                     ibuff++;
                  }
               }
            }
            buff[ibuff] = '\n';
            ibuff++;
         }
      }
      buff[ibuff] = 0;
      ibuff++;
      me->strout = tabla_concat(me, me->strout, buff);
   }
}

tabla *tabla_Create(int m, int i)
{
   tabla *me = (tabla *)malloc(sizeof(tabla));
   if (me != NULL)
   {
      tabla_Init(me, m, i);
   }
   return me;
}

void tabla_Destroy(tabla *const me)
{
   if (me != NULL)
   {
      tabla_Cleanup(me);
   }
   free(me);
}
