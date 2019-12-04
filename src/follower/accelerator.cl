__kernel void 
seed(int seed, __global uint* seed_global)
{
   int gl_id = get_global_id(0); //POPL_SIZE * POPF_SIZE
   seed_global[gl_id] = seed ^ (gl_id + 1);
}

__kernel void
follower( __global real_t* popL, __global real_t* popLValoresF, __global real_t* gl_popF, __local real_t* lo_popF, __local real_t* fit_popF, __local int* best_idx, __local real_t* uL, __global uint* seed_global, __global real_t* VF, __global real_t* VL, int initialization )
{
   int lo_id = get_local_id(0); //number of work itens -> <= POPF_SIZE
   int gr_id = get_group_id(0); //number of groups -> POPL_SIZE
   int gl_id = get_global_id(0); //POPL_SIZE * POPF_SIZE

   int lo_size = get_local_size(0);

   uint seed = seed_global[gl_id];

   int n;

   __local int idx[3]; 

   // 1 uL by group -> uL is represented by gr_id 
   // each group has #local_size work itens (lo_id)
   // local_size can be <= POPF_SIZE and local_size can be < DIML or DIMF.

   if( initialization )
   {
      // initialization of popL
      // start
      // solution S is a array of size D
      // popL => S0D0 | S1D0 | S2D0 | ... | S0D1 | S1D1 | S2D1 | ... 
      // popL -> POPL_SIZE * DIML
      // gr_id -> solution S (S0, S1, S2, ...)
      // lo_id -> dimension D (D0, D1, D2, ...)
      for( int j = 0; j < (int) ceil(DIML/(real_t)lo_size); ++j )
      {
         n = j * lo_size + lo_id;
         if( n < DIML )
         { 
            uL[n] = getLower_level_1( n ) + Real( &seed )*(getUpper_level_1( n ) - getLower_level_1( n )); //UPPER - LOWER
            popL[gr_id + n * POPL_SIZE] = uL[n];
         }
      }
      // initialization of popL
      // end
   }
   else
   {
      // leader generation -> uL is represented by gr_id
      // start
      // TODO: considerar a possibilidade de tirar esses loops
      if( lo_id == 0 )
      {
	      do
         {
	      	idx[0] = Int( &seed, POPL_SIZE );
	      } while(idx[0] == gr_id);
	      do
         {
	      	idx[1] = Int( &seed, POPL_SIZE );
	      } while(idx[1] == gr_id || idx[1] == idx[0]);
	      do
         {
	      	idx[2] = Int( &seed, POPL_SIZE );
	      } while(idx[2] == gr_id || idx[2] == idx[0] || idx[2] == idx[1]);
      }
      barrier(CLK_LOCAL_MEM_FENCE);

      // solution S is a array of size D
      // popL => S0D0 | S1D0 | S2D0 | ... | S0D1 | S1D1 | S2D1 | ... 
      // popL -> POPL_SIZE * DIML
      // idx -> solution S (S0, S1, S2, ...)
      // lo_id -> dimension D (D0, D1, D2, ...)
      for( int j = 0; j < (int) ceil(DIML/(real_t)lo_size); ++j )
      {
         n = j * lo_size + lo_id;
         if( n < DIML )
         { 
            int jRand = Int( &seed, DIML );
            if( n == jRand || (Real( &seed ) < CR) )
            {	
#if defined(VARIANT_rand) //DE/rand/1/bin
               uL[n] = popL[idx[0] + n * POPL_SIZE] + F*(popL[idx[1] + n * POPL_SIZE] - popL[idx[2] + n * POPL_SIZE]); 
#elif defined(VARIANT_target_to_rand) //DE/target-to-rand/1/bin
               uL[n] = popL[gr_id + n * POPL_SIZE] + F*(popL[idx[0] + n * POPL_SIZE] - popL[gr_id + n * POPL_SIZE]) + F*(popL[idx[1] + n * POPL_SIZE] - popL[idx[2] + n * POPL_SIZE]); 
#else
               "Variant not supported"
#endif
               if( uL[n] < getLower_level_1( n ) )
               {
                  uL[n] = getLower_level_1( n );
               }
               else 
               {
                  if( uL[n] > getUpper_level_1( n ) )
                  {
                     uL[n] = getUpper_level_1( n );
                  }
               }
            } 
            else 
            {
               uL[n] = popL[gr_id + n * POPL_SIZE]; 
            }
         }
      }
   }
   // leader generation -> uL
   // end

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
            lo_popF[n + i * POPF_SIZE] = getLower_level_2( i ) + Real( &seed )*(getUpper_level_2( i ) - getLower_level_2( i )); //UPPER - LOWER2
            gl_popF[gr_id * (POPF_SIZE * DIMF) + n + i * POPF_SIZE] = lo_popF[n + i * POPF_SIZE];
         }
         // follower population generation -> popF
         // end
      }
   }

   // Wait for all work itens because just some of them (lo_id < DIML) are responsible for the leader generation (uL).
   // And because each lo_id will access diferent positions of lo_popF.
   barrier(CLK_LOCAL_MEM_FENCE);

   for( int j = 0; j < (int) ceil(POPF_SIZE/(real_t)lo_size); ++j )
   {
      n = j * lo_size + lo_id;
      if( n < POPF_SIZE )
      {
         // follower population evaluation -> popF
         // start
         // fit_popF -> size of POPF_SIZE
         // TODO: mudar para fit_popF
         fit_popF[n] = evaluate_transpose_follower_level_2(n, uL, lo_popF);
         // follower population evaluation -> popF
         // end

         for( int g = 0; g < GENF_NUM; g++ )
         {
            // follower generation -> uF is represented by lo_id (here n)
            // lo_id (here n) is a follower uF
            // start
            // TODO: considerar a possibilidade de tirar esses loops
            int idx1; 
            do
            {
               idx1 = Int( &seed, POPF_SIZE );
            } while(idx1 == n);
            int idx2; 
            do
            {
               idx2 = Int( &seed, POPF_SIZE );
            } while(idx2 == n || idx2 == idx1);
            int idx3; 
            do
            {
               idx3 = Int( &seed, POPF_SIZE );
            } while(idx3 == n || idx3 == idx1 || idx3 == idx2);

            // solution S is a array of size D
            // popF => S0D0 | S1D0 | S2D0 | ... | S0D1 | S1D1 | S2D1 | ...
            // popF -> POPF_SIZE * DIMF
            // idx1 -> solution S (S0, S1, S2, ...)
            // lo_id -> follower uF
            int jRand = Int( &seed, DIMF );
            for( int i = 0; i < DIMF; i++ )
            {
               if( (i == jRand) || (Real( &seed ) < CR) )
               {	
#if defined(VARIANT_rand) //DE/rand/1/bin
                  lo_popF[n + i * POPF_SIZE] = lo_popF[idx1 + i * POPF_SIZE] + F*(lo_popF[idx2 + i * POPF_SIZE] - lo_popF[idx3 + i * POPF_SIZE]);
#elif defined(VARIANT_target_to_rand) //DE/target-to-rand/1/bin
                  lo_popF[n + i * POPF_SIZE] = lo_popF[n + i * POPF_SIZE] + F*(lo_popF[idx1 + i * POPF_SIZE] - lo_popF[n + i * POPF_SIZE]) + F*(lo_popF[idx2 + i * POPF_SIZE] - lo_popF[idx3 + i * POPF_SIZE]);
#else
   "Variant not supported"
#endif
                  if( lo_popF[n + i * POPF_SIZE] < getLower_level_2( i ) )
                  {
                     lo_popF[n + i * POPF_SIZE] = getLower_level_2( i );
                  }
                  else 
                  {
                     if( lo_popF[n + i * POPF_SIZE] > getUpper_level_2( i ) ) 
                     {
                        lo_popF[n + i * POPF_SIZE] = getUpper_level_2( i );
                     }
                  }
               }
            }
            // follower generation -> uF
            // end

            // follower evaluation -> uF
            // start
            real_t fit_popF_new;
            fit_popF_new = evaluate_transpose_follower_level_2(n, uL, lo_popF);
            // follower evaluation -> uF
            // end

            // new generation
            // start
            // The functions 1001, 1002, ..., 1008 are all minimization functions.
            // They do not have any restriction. 
            if( fit_popF_new <= fit_popF[n] )
            {
               fit_popF[n] = fit_popF_new;
               for( int i = 0; i < DIMF; i++ )
               {
                  gl_popF[gr_id * (POPF_SIZE * DIMF) + n + i * POPF_SIZE] = lo_popF[n + i * POPF_SIZE];
               }
            } 
            else 
            {
               for( int i = 0; i < DIMF; i++ )
               {
                  lo_popF[n + i * POPF_SIZE] = gl_popF[gr_id * (POPF_SIZE * DIMF) + n + i * POPF_SIZE];
               }
            }
            // new generation
            // end

            seed_global[gl_id] = seed;
         }
         best_idx[n] = n;
      }
   }

   // reduction: best individual for each uL or group gr_id
   // start
   // number of groups -> POPL_SIZE
   // VF -> POPL_SIZE * DIMF
   // VL -> POPL_SIZE * DIML
   for( int j = 0; j < (int) ceil(POPF_SIZE/(real_t)lo_size); ++j )
   {
      n = j * lo_size + lo_id;
      if( n < POPF_SIZE )
      {
         int next_power_of_2 = pown(2.0f, (int) ceil(log2((real_t)POPF_SIZE)));
         for( int k = next_power_of_2/2; k > 0; k/=2 )
         {
            barrier(CLK_LOCAL_MEM_FENCE);
            if( (n < k) && (n + k < POPF_SIZE) )
            {
               // The functions 1001, 1002, ..., 1008 are all minimization functions.
               //if( fit_popF[n + k] < fit_popF[n] ){
               if( fit_popF[best_idx[n + k]] <= fit_popF[best_idx[n]] )
               {
                  best_idx[n] = best_idx[n + k];
                  //fit_popF[n] = fit_popF[n + k];
               }
            }
         }
      }
   }
   // TODO
   barrier(CLK_LOCAL_MEM_FENCE);

   if( initialization )
   {
      for( int j = 0; j < (int) ceil(DIMF/(real_t)lo_size); ++j )
      {
         n = j * lo_size + lo_id;
         if( n < DIMF )
         { 
            popLValoresF[gr_id + n * POPL_SIZE] = lo_popF[best_idx[0] + n * POPF_SIZE];
         }
      }
   }
   else
   {
      for( int j = 0; j < (int) ceil(DIMF/(real_t)lo_size); ++j )
      {
         n = j * lo_size + lo_id;
         if( n < DIMF )
         { 
            VF[gr_id + n * POPL_SIZE] = lo_popF[best_idx[0] + n * POPF_SIZE];
         }
      }
      for( int j = 0; j < (int) ceil(DIML/(real_t)lo_size); ++j )
      {
         n = j * lo_size + n;
         if( n < DIML )
         { 
            VL[gr_id + n * POPL_SIZE] = uL[n];
         }
      }
   }
   // reduction: best individual
   // end
}

__kernel void
leader( __global real_t* popL, __global real_t* popLValoresF, __global real_t* VF, __global real_t* VL, __global real_t* fit_popL, __global real_t* fit_popLValoresF, int generation )
{
   int gl_id = get_global_id(0); //POPL_SIZE

   // solution S is a array of size D
   // popL, popLValoreF, VF e VL => S0D0 | S1D0 | S2D0 | ... | S0D1 | S1D1 | S2D1 | ...
   // gl_id -> solution S (S0, S1, S2, ...) of POPL

   real_t fit_VL = evaluate_transpose_leader_level_1( gl_id, VL, VF );
   real_t fit_VF = evaluate_transpose_leader_level_2( gl_id, VL, VF );
   if( generation == 0 ) 
   {
      fit_popL[gl_id] = evaluate_transpose_leader_level_1( gl_id, popL, popLValoresF );
      fit_popLValoresF[gl_id] = evaluate_transpose_leader_level_2( gl_id, popL, popLValoresF );
   }

	if( fit_VL <= fit_popL[gl_id] && fit_VF <= fit_popLValoresF[gl_id])
   {
      for( int j = 0; j < DIML; j++ )
      {
         popL[gl_id + j * POPL_SIZE] = VL[gl_id + j * POPL_SIZE];
      }
      for( int j = 0; j < DIMF; j++ )
      {
         popLValoresF[gl_id + j * POPL_SIZE] = VF[gl_id + j * POPL_SIZE];
      }
      fit_popL[gl_id] = fit_VL;
      fit_popLValoresF[gl_id] = fit_VF;
   }
}

