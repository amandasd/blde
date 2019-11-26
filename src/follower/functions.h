#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_


#define LCG_RAND_MAX 2147483647

/* Generates a pseudo random number using a special case of Linear Congruential
   Generator (Lehmer). For more information, please refer to:

   https://en.wikipedia.org/wiki/Lehmer_random_number_generator

   seed must be:  0 < seed < 2147483647
*/
uint lcg( uint seed )
{
   ulong product = (ulong) seed * 48271;
   uint x = (product & 0x7fffffff) + (product >> 31);
   return (x & 0x7fffffff) + (x >> 31);
}

/* Returns a float random number in (0,1] */
real_t Real( uint * seed )
{
   *seed = lcg( *seed );
   return *seed / (real_t) LCG_RAND_MAX;
}

/* Returns an integer (unsigned) random number in [0,n) */
uint Int( uint * seed, uint n )
{
   *seed = lcg( *seed );
   return *seed % n;
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
real_t evaluate_transpose_leader(int idx, int nivel, global real_t* uL, global real_t* uF)
{
   real_t F1 = 0.0, F2 = 0.0, F3 = 0.0;

   real_t x[DIML];
   real_t y[DIMF];

   for(int i = 0; i < DIML; i++)
   {
      x[i] = uL[idx + i * POPL_SIZE];                        
   }                    
   for(int i = 0; i < DIMF; i++)  
   {                       
      y[i] = uF[idx + i * POPL_SIZE];
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

real_t evaluate_transpose_follower(int idx, local real_t* uL, local real_t* popF)
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
   for(int i = 0; i < r; i++)
   {                        
      F3 += ((x[p+i]-tan(y[q+i]))*(x[p+i]-tan(y[q+i])));
   }                    

   return F1+F2+F3;
}
#endif
