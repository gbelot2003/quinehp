/*********************************************************************
	Author: Leonardo Laguna Ruiz
   Date	: Sun, 24, Jul 2005
*********************************************************************/

#include "quine.h"
#include <hpgcc49.h>
#include "indata.h"

int main(void)
{
   // Initialize stack access
   unsigned rpl_stack_bias = sat_stack_init();
   int i;
   int *min, *don;
   VEC_IN minv, donv;
   quine *red;
   min = 0;
   don = 0;

   if (sat_stack_depth() > 0)
   {
      SAT_STACK_ELEMENT e1, e2;
      SAT_STACK_DATA stkmin, stkdon;

      sat_get_stack_element(1, &e1);
      sat_decode_stack_element(&stkmin, &e1);
      sat_get_stack_element(2, &e2);
      sat_decode_stack_element(&stkdon, &e2);

      if (stkmin.type == SAT_DATA_TYPE_STRING && stkdon.type == SAT_DATA_TYPE_STRING)
      {
         min = scanint(&minv, stkmin.sval); //obtiene size, max,
         don = scanint(&donv, stkdon.sval);
         if (minv.cont > 0 && minv.estado != INCORRECTO && donv.estado != INCORRECTO)
         {
            clearScreen();
            printf("-------Quine minimizer--------\nBy Leonardo Laguna Ruiz\nInstituto Tecnologico de Celaya\n\n\n");
            red = quine_Create(donv.size > minv.size ? donv.size : minv.size);
            for (i = 0; i < minv.cont; i++)
            {
               quine_insertar(red, min + i, 0);
            }
            if (donv.cont > 0)
            {
               for (i = 0; i < donv.cont; i++)
               {
                  quine_insertar(red, don + i, 1);
               }
            }
            quine_reducir(red);
            sat_stack_push_string(red->strout);
            printf("\nPress ON to continue");
            while (!keyb_isON())
               ;
            quine_Destroy(red);
         }
         else
         {
            clearScreen();
            printf("Usage:\n\n 2:\"d1 d2 d3 ... dn\" \n 1:\"m1 m2 m3 ... mn\"\n\nExample:\n\n 2:\"0 5\"\n 1:\"1 2 3 4 \"\n\n\n\nPress ON to continue ");
            while (!keyb_isON())
               ;
         }
         if (min)
            free(min);
         if (don)
            free(don);
      }
      else
      {
         clearScreen();
         printf("Usage:\n\n 2:\"d1 d2 d3 ... dn\" \n 1:\"m1 m2 m3 ... mn\"\n\nExample:\n\n 2:\"0 5\"\n 1:\"1 2 3 4 \"\n\n\n\nPress ON to continue ");
         while (!keyb_isON())
            ;
      }
   }
   else
   {
      clearScreen();
      printf("Usage:\n\n 2:\"d1 d2 d3 ... dn\" \n 1:\"m1 m2 m3 ... mn\"\n\nExample:\n\n 2:\"0 5\"\n 1:\"1 2 3 4 \"\n\n\n\nPress ON to continue ");
      while (!keyb_isON())
         ;
   }

   sat_stack_exit(rpl_stack_bias);
   return 0;
}
