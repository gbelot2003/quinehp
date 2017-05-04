#define NADA 0
#define NUMERO 1
#define INCORRECTO 2
#define FIN 3

#define CONTEO 0
#define ALMACEN 1

typedef struct entrada
{
   int cont, max, acc, size;
   char estado, op;
} VEC_IN;

void eval_est(VEC_IN *v, char c, int *in_min)
{
   switch (v->estado)
   {
   case NADA:
      if (c >= '0' && c <= '9')
      {
         v->estado = NUMERO;
         if (v->op == ALMACEN)
            v->acc = c - '0';
         break;
      }
      if (c == ' ')
      {
         v->estado = NADA;
         break;
      }
      if (c == 0)
      {
         v->estado = FIN;
         break;
      }
      v->estado = INCORRECTO;
      break;
   case NUMERO:
      if (c >= '0' && c <= '9')
      {
         v->estado = NUMERO;
         if (v->op == ALMACEN)
            v->acc = v->acc * 10 + c - '0';
         break;
      }
      if (c == ' ')
      {
         v->estado = NADA;
         if (v->op == ALMACEN)
         {
            in_min[v->cont] = v->acc;
            if (v->acc > v->max)
               v->max = v->acc;
         }
         v->cont++;
         break;
      }
      if (c == 0)
      {
         v->estado = FIN;
         if (v->op == ALMACEN)
         {
            in_min[v->cont] = v->acc;
            if (v->acc > v->max)
               v->max = v->acc;
         }
         v->cont++;
         break;
      }
      v->estado = INCORRECTO;
      break;
   default:
      break;
   }
}

int *scanint(VEC_IN *v, char *in_str)
{
   int i, j;
   int *in_min;
   in_min = 0;
   i = 0;
   v->cont = 0;
   v->max = 0;
   v->estado = NADA;
   v->op = CONTEO;
   v->size = 0;
   while (v->estado != FIN && v->estado != INCORRECTO)
   {
      eval_est(v, in_str[i], in_min);
      i++;
   }
   if (v->estado != INCORRECTO && v->cont > 0)
   {
      in_min = malloc(v->cont * sizeof(int));
      v->op = ALMACEN;
      v->estado = NADA;
      i = 0;
      v->cont = 0;
      while (v->estado != FIN)
      {
         eval_est(v, in_str[i], in_min);
         i++;
      }
      if (v->cont > 0)
      {
         i = 0x0001;
         for (j = 0; j <= 15; j++)
         {
            if (i & v->max)
               v->size = j;
            i = i << 1;
         }
         v->size++;
      }
      else
         v->size = 0;
   }
   return in_min;
}
