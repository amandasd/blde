// Check for the availability (and enable) of double precision support
#ifdef CONFIG_USE_DOUBLE
#if defined(cl_khr_fp64)
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#define DOUBLE_SUPPORT_AVAILABLE
#elif defined(cl_amd_fp64)
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#define DOUBLE_SUPPORT_AVAILABLE
#endif
#endif /* CONFIG_USE_DOUBLE */

#include <functions.h>

__kernel void
blde( __global real_t* popL, __global real_t* gl_popF, __local real_t* lo_popF, __local real_t* fitF, __local int* best_idx, __local real_t* uL, __global uint* seed_global, __global real_t* VF, __global real_t* VL, int initialization )
{
   int lo_id = get_local_id(0); //number of work itens -> <= POPF_SIZE
   int gr_id = get_group_id(0); //number of groups -> POPL_SIZE
   int gl_id = get_global_id(0); //POPL_SIZE * POPF_SIZE

   int lo_size = get_local_size(0);
   int next_power_of_2 = pown(2.0f, (int) ceil(log2((real_t)lo_size)));

   uint seed = seed_global[gl_id];

   int n;

   // 1 uL by group -> uL is represented by gr_id 
   // each group has #local_size work itens (lo_id)
   // local_size can be <= POPF_SIZE and local_size can be < DIML or DIMF.

   if( initialization )
   {
      // initialization of popL
      // start
      // solution S is a array of size D
      // popL => S0D0 | S0D1 | S0D2 | ... | S1D0 | S1D1 | S1D2 | ...
      // popL -> POPL_SIZE * DIML
      // gr_id -> solution S (S0, S1, S2, ...)
      // lo_id -> dimension D (D0, D1, D2, ...)
      for( int j = 0; j < (int) ceil(DIML/(real_t)lo_size); ++j )
      {
         n = j * lo_size + lo_id;
         if( n < DIML )
         { 
            seed = aleatorio(seed);
            uL[n] = getLower(1, n) + (seed/(real_t)RAND_MAX)*(getUpper(1, n) - getLower(1, n)); //UPPER - LOWER
            popL[gr_id * DIML + n] = uL[n];
         }
      }
      // initialization of popL
      // end
   }
   else
   {
      // leader generation -> uL is represented by gr_id
      // start
      __local int idx[3]; 
      if( lo_id == 0 )
      {
	      do
         {
            seed = aleatorio(seed);
	      	idx[0] = seed%POPL_SIZE;
	      } while(idx[0] == gr_id);
	      do
         {
            seed = aleatorio(seed);
	      	idx[1] = seed%POPL_SIZE;
	      } while(idx[1] == gr_id || idx[1] == idx[0]);
	      do
         {
            seed = aleatorio(seed);
	      	idx[2] = seed%POPL_SIZE;
	      } while(idx[2] == gr_id || idx[2] == idx[0] || idx[2] == idx[1]);
      }
      barrier(CLK_LOCAL_MEM_FENCE);

      // solution S is a array of size D
      // popL => S0D0 | S0D1 | S0D2 | ... | S1D0 | S1D1 | S1D2 | ...
      // popL -> POPL_SIZE * DIML
      // idx -> solution S (S0, S1, S2, ...)
      // lo_id -> dimension D (D0, D1, D2, ...)
      for( int j = 0; j < (int) ceil(DIML/(real_t)lo_size); ++j )
      {
         n = j * lo_size + lo_id;
         if( n < DIML )
         { 
            seed = aleatorio(seed);
            int jRand = seed%DIML;
            seed = aleatorio(seed);
            if( n == jRand || (seed/(real_t)RAND_MAX < CR) )
            {	
               // TODO: var usa ifdef (VAR) e passa como opcao do build qual esta definido
               uL[n] = popL[idx[0] * DIML + n] + F*(popL[idx[1] * DIML + n] - popL[idx[2] * DIML + n]); //DE/rand/1/bin
               //uL[n] = popL[gr_id * DIML + n] + F*(popL[idx[0] * DIML + n] - popL[gr_id * DIML + n]) + F*(popL[idx[1] * DIML + n] - popL[idx[2] * DIML + n]); //DE/target-to-rand/1/bin
               if( uL[n] < getLower(1, n) )
               {
                  uL[n] = getLower(1, n);
               }
               else 
               {
                  if( uL[n] > getUpper(1, n) )
                  {
                     uL[n] = getUpper(1, n);
                  }
               }
            } 
            else 
            {
               uL[n] = popL[gr_id * DIML + n]; 
            }
         }
      }
      // leader generation -> uL
      // end
   }

   // Wait for all work itens because just some of them (lo_id < DIML) are responsible for the leader generation (uL).
   barrier(CLK_LOCAL_MEM_FENCE);

   // local_size can be <= POPF_SIZE. POPF_SIZE > local_size when max_local_size < POPF_SIZE.
   // the total number of uFs is equal POPF_SIZE.
   // lo_id is at least 1 uF. It can be more than one uF.
   for( int j = 0; j < (int) ceil(POPF_SIZE/(real_t)lo_size); ++j )
   {
      n = j * lo_size + lo_id;
      if( n < POPF_SIZE )
      {
         // follower population generation -> popF
         // start
         // popF => S0D0 | S1D0 | S2D0 | ... | S0D1 | S1D1 | S2D1 | ...
         // popF -> POPF_SIZE * DIMF
         // lo_id -> solution S (S0, S1, S2, ...) of POPF
         // i -> dimension D (D0, D1, D2, ...)
         for( int i = 0; i < DIMF; i++ )
         {
            seed = aleatorio(seed);
            lo_popF[n + i * POPF_SIZE] = getLower(2, i) + (seed/(real_t)RAND_MAX)*(getUpper(2, i) - getLower(2, i)); //UPPER - LOWER2
            gl_popF[n + i * POPF_SIZE] = lo_popF[n + i * POPF_SIZE];
         }
         // follower population generation -> popF
         // end

         // follower population evaluation -> popF
         // start
         // fitF -> size of POPF_SIZE
         fitF[n] = evaluate_transpose(n, 2, uL, lo_popF);
         // follower population evaluation -> popF
         // end

         for( int g = 0; g < GENF_NUM; g++ )
         {
            // follower generation -> uF is represented by lo_id (here n)
            // lo_id (here n) is a follower uF
            // start
            int idx1; 
            do
            {
               seed = aleatorio(seed);
               idx1 = seed%POPF_SIZE;
            } while(idx1 == n);
            int idx2; 
            do
            {
               seed = aleatorio(seed);
               idx2 = seed%POPF_SIZE;
            } while(idx2 == n || idx2 == idx1);
            int idx3; 
            do
            {
               seed = aleatorio(seed);
               idx3 = seed%POPF_SIZE;
            } while(idx3 == n || idx3 == idx1 || idx3 == idx2);

            // solution S is a array of size D
            // popF => S0D0 | S1D0 | S2D0 | ... | S0D1 | S1D1 | S2D1 | ...
            // popF -> POPF_SIZE * DIMF
            // idx1 -> solution S (S0, S1, S2, ...)
            // lo_id -> follower uF
            seed = aleatorio(seed);
            int jRand = seed%DIMF;
            for( int i = 0; i < DIMF; i++ )
            {
               seed = aleatorio(seed);
               if( (i == jRand) || (seed/(real_t)RAND_MAX < CR) )
               {	
                  // TODO: var usa ifdef (VAR) e passa como opcao do build qual esta definido
                  lo_popF[n + i * POPF_SIZE] = lo_popF[idx1 + i * POPF_SIZE] + F*(lo_popF[idx2 + i * POPF_SIZE] - lo_popF[idx3 + i * POPF_SIZE]); //DE/rand/1/bin
                  //lo_popF[n + i * POPF_SIZE] = lo_popF[n + i * POPF_SIZE] + F*(lo_popF[idx1 + i * POPF_SIZE] - lo_popF[n + i * POPF_SIZE]) + F*(lo_popF[idx2 + i * POPF_SIZE] - lo_popF[idx3 + i * POPF_SIZE]); //DE/target-to-rand/1/bin
                  if( lo_popF[n + i * POPF_SIZE] < getLower(2, i) )
                  {
                     lo_popF[n + i * POPF_SIZE] = getLower(2, i);
                  }
                  else 
                  {
                     if( lo_popF[n + i * POPF_SIZE] > getUpper(2, i) ) 
                     {
                        lo_popF[n + i * POPF_SIZE] = getUpper(2, i);
                     }
                  }
               }
            }
            // follower generation -> uF
            // end

            // follower evaluation -> uF
            // start
            real_t fitF_new;
            fitF_new = evaluate_transpose(n, 2, uL, lo_popF);
            // follower evaluation -> uF
            // end

            // new generation
            // start
            // The functions 1001, 1002, ..., 1008 are all minimization functions.
            // They do not have any restriction. 
            if( fitF_new <= fitF[n] )
            {
               fitF[n] = fitF_new;
               for( int i = 0; i < DIMF; i++ )
               {
                  gl_popF[n + i * POPF_SIZE] = lo_popF[n + i * POPF_SIZE];
               }
            } 
            else 
            {
               for( int i = 0; i < DIMF; i++ )
               {
                  lo_popF[n + i * POPF_SIZE] = gl_popF[n + i * POPF_SIZE];
               }
            }
            // new generation
            // end

            seed_global[gl_id] = seed;
         }
      }
   }

   // reduction: best individual for each uL or group gr_id
   // start
   // number of groups -> POPL_SIZE
   // VF -> POPL_SIZE * DIMF
   // VL -> POPL_SIZE * DIML
   best_idx[lo_id] = lo_id;
   for( int k = next_power_of_2/2; k > 0; k >>= 1 )
   {
      barrier(CLK_LOCAL_MEM_FENCE);
      if( (lo_id < k) && (lo_id + k < lo_size) )
      {
         // The functions 1001, 1002, ..., 1008 are all minimization functions.
         //if( fitF[lo_id + k] < fitF[lo_id] ){
         if( fitF[best_idx[lo_id + k]] < fitF[best_idx[lo_id]] )
         {
            best_idx[lo_id] = lo_id + k;
            //fitF[lo_id] = fitF[lo_id + k];
         }
      }
   }

   for( int j = 0; j < (int) ceil(DIMF/(real_t)lo_size); ++j )
   {
      n = j * lo_size + lo_id;
      if( n < DIMF )
      { 
         VF[gr_id * DIMF + n] = lo_popF[best_idx[0] + n * POPF_SIZE];
      }
   }
   for( int j = 0; j < (int) ceil(DIML/(real_t)lo_size); ++j )
   {
      n = j * lo_size + n;
      if( n < DIML )
      { 
         VL[gr_id * DIML + n] = uL[n];
      }
   }
   // reduction: best individual
   // end
}
