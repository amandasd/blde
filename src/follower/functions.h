#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "definitions.h"

#ifndef M
   #define M 2147483648
#endif
#ifndef A
   #define A 1103515245
#endif
#ifndef C
   #define C 12345
#endif

uint aleatorio( int semente )
{
   return (A * semente + C) % M;
}

real_t getLower(int nivel, int indice)
{
   if (nivel == 1)
   {

      return -5.;
   }
   else 
   {

      if (indice < q)
      {
        return -5.;
      } 
      else 
      {
        return -(M_PI_F/2.) + 0.000000000001;
      }
   }
}

real_t getUpper(int nivel, int indice)
{
   if (nivel == 1)
   {

      return 10.;
   }
   else 
   {

      if (indice < q)
      {
          return 10.;
      } 
      else 
      {
          return M_PI_F/2. - 0.000000000001;
      }
   }
}

real_t evaluate_transpose(int idx, int nivel, local real_t* uL, local real_t* popF)
{
   real_t F1 = 0.0, F2 = 0.0, F3 = 0.0;

   real_t x[DIML];
   real_t y[DIMF];

   for(int i = 0; i < DIML; i++)
   {
      x[i] = uL[i];                        
   }                    
   for(int i = 0; i < DIMF; i++)  
   {                       
      y[i] = popF[idx + i * POPF_SIZE];
   }                                         

   for(int i = 0; i < p; i++)
   {
      F1 += (x[i]*x[i]);
   }
   for(int i = 0; i < q; i++)
   {
      F2 += (y[i]*y[i]);
   }                           
   if(nivel == 1)
   { 
      real_t sum1 = 0.0, sum2 = 0.0;
      for(int i = 0; i < r; i++)
      {
         sum1 += (x[p+i]*x[p+i]);
         sum2 += ((x[p+i]-tan(y[q+i]))*(x[p+i]-tan(y[q+i])));
      }
      F3 = sum1+sum2;
   } 
   else 
   {
      if(nivel == 2)
      {
         for(int i = 0; i < r; i++)
         {                        
            F3 += ((x[p+i]-tan(y[q+i]))*(x[p+i]-tan(y[q+i])));
         }                    
      }
   }

   return F1+F2+F3;
}

#endif
