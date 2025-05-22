__kernel void 
seed(int seed, __global uint* seed_global)
{
   int gl_id = get_global_id(0); //POPL_SIZE * POPF_SIZE * NF
   seed_global[gl_id] = lcg( seed + gl_id + 1 ); // Make each work-item random stream have a different seed
}

__kernel void
leader( __global real_t* popL, __global uint* seed_global, __global real_t* VL, int initialization )
{
   int gl_id = get_global_id(0); //POPL_SIZE

   uint seed = seed_global[gl_id];

   if( initialization )
   {
      // initialization of popL
      // start
      // solution S is a array of size D
      // popL => S0D0 | S1D0 | S2D0 | ... | S0D1 | S1D1 | S2D1 | ... 
      // popL -> POPL_SIZE * DIML
      // gl_id -> solution S (S0, S1, S2, ...)
      for( int j = 0; j < DIML; j++ )
      {
         popL[gl_id + j * POPL_SIZE] = getLower_level_1( j ) + Real( &seed )*(getUpper_level_1( j ) - getLower_level_1( j )); //UPPER - LOWER
         // initialization of popL
         // end
      }
   }
   else
   {
      // start
      real_t uL[DIML];

      int idx1;
      do
      {
         idx1 = Int( &seed, POPL_SIZE );
      } while(idx1 == gl_id);
      int idx2;
      do
      {
         idx2 = Int( &seed, POPL_SIZE );
      } while(idx2 == gl_id || idx2 == idx1);
      int idx3;
      do
      {
         idx3 = Int( &seed, POPL_SIZE );
      } while(idx3 == gl_id || idx3 == idx1 || idx3 == idx2);

      // solution S is a array of size D
      // popL => S0D0 | S1D0 | S2D0 | ... | S0D1 | S1D1 | S2D1 | ... 
      // popL -> POPL_SIZE * DIML
      // idx -> solution S (S0, S1, S2, ...)
      int jRand = Int( &seed, DIML );
      for( int j = 0; j < DIML; j++ )
      {
         if( j == jRand || (Real( &seed ) < CR) )
         {
#if defined(VARIANT_rand) //DE/rand/1/bin
            uL[j] = popL[idx1 + j * POPL_SIZE] + F*(popL[idx2 + j * POPL_SIZE] - popL[idx3 + j * POPL_SIZE]); 
#elif defined(VARIANT_target_to_rand) //DE/target-to-rand/1/bin
            uL[j] = popL[gl_id + j * POPL_SIZE] + F*(popL[idx1 + j * POPL_SIZE] - popL[gl_id + j * POPL_SIZE]) + F*(popL[idx2 + j * POPL_SIZE] - popL[idx3 + j * POPL_SIZE]); 
#else
            "Variant not supported"
#endif
            if( uL[j] < getLower_level_1( j ) )
            {
               uL[j] = getLower_level_1( j );
            }
            else 
            {
               if( uL[j] > getUpper_level_1( j ) )
               {
                  uL[j] = getUpper_level_1( j );
               }
            }
         }
         else
         {
            uL[j] = popL[gl_id + j * POPL_SIZE]; 
         }
         VL[gl_id + j * POPL_SIZE] = uL[j];
      }
   }
   // end
}

__kernel void
follower( __global real_t* popL, __global real_t* popLValoresF, __global real_t* gl_popF, __local real_t* lo_popF, __local real_t* fit_popF, __local int* best_idx, __local real_t* uL, __local real_t* uFs, __global uint* seed_global, __global real_t* VF, __global real_t* VL, int initialization, int follower )
{
   int lo_id = get_local_id(0); //number of work itens -> <= POPF_SIZE
   int gr_id = get_group_id(0); //number of groups -> POPL_SIZE * NF
   int gl_id = get_global_id(0); //POPL_SIZE * POPF_SIZE * NF

   int lo_size = get_local_size(0);

   uint seed = seed_global[gl_id];

   int n;

   // 1 (uL, follower) by group -> uL is represented by (gr_id % POPL_SIZE) 
   // each group has #local_size work itens (lo_id)
   // local_size can be <= POPF_SIZE and local_size can be < DIML or DIMF.
   //
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
         if( initialization )
         {
            uL[n] = popL[(gr_id % POPL_SIZE) + n * POPL_SIZE];
         }
         else
         {
            uL[n] = VL[(gr_id % POPL_SIZE) + n * POPL_SIZE];
         }
      }
   }

   // popLValoresF contains the best solution for each (uL=S, follower=F) population
   // popLValoreF => S0D0F0 | S1D0F0 | S2D0F0 | ... | S0D1F0 | S1D1F0 | S2D1F0 | ... | S0D0F1 ...
   // For a given uL, uFs contains the best solution for each follower
   // uFs => D0F0 | D1F0 | D2F0 | ... | D0F1 | D1F1 | D2F1 | ... | D0F2 ...
   // lo_id -> dimension D (D0, D1, D2, ...)
   //for( int j = 0; j < (int) ceil(DIMF/(real_t)lo_size); ++j )
   //{
   //   n = j * lo_size + lo_id;
   //   if( n < DIMF )
   //   {
   //      // TODO: pensar qdo NF = 1, generalizar
   //      for( int f = 0; f < NF; ++f )
   //      {
   //         if( initialization )
   //         {
   //            uFs[f * DIMF + n] = getLower_level_2( n ) + Real( &seed )*(getUpper_level_2( n ) - getLower_level_2( n )); //UPPER - LOWER2
   //         }
   //         else
   //         {
   //            uFs[f * DIMF + n] = popLValoresF[f * POPL_SIZE * DIMF + (gr_id % POPL_SIZE) + n * POPL_SIZE]; 
   //         }
   //      }
   //   }
   //}

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

         // follower population evaluation -> popF
         // start
         // fit_popF -> size of POPF_SIZE
         fit_popF[n] = evaluate_transpose_follower_level_2(n, uL, lo_popF);
         // TODO: seguidores dependentes; pensar qdo NF = 1, e generalizar
         //fit_popF[n] = evaluate_transpose_follower_level_2(n, uL, uFs, lo_popF, follower);
         // follower population evaluation -> popF
         // end
      }
   }

   // Wait for all work itens because just some of them (lo_id < DIML) are responsible for the leader generation (uL).
   // And because each lo_id will accesses diferent positions of lo_popF.
   barrier(CLK_LOCAL_MEM_FENCE);

   for( int g = 0; g < GENF_NUM; g++ )
   {
      for( int j = 0; j < (int) ceil(POPF_SIZE/(real_t)lo_size); ++j )
      {
         n = j * lo_size + lo_id;
         if( n < POPF_SIZE )
         {
            // follower generation -> uF is represented by lo_id (here n)
            // lo_id (here n) is a follower uF
            // start
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
                  //TODO
#if defined(VARIANT_rand) //DE/rand/1/bin
                  //lo_popF[n + i * POPF_SIZE] = lo_popF[idx1 + i * POPF_SIZE] + F*(lo_popF[idx2 + i * POPF_SIZE] - lo_popF[idx3 + i * POPF_SIZE]);
                  lo_popF[n + i * POPF_SIZE] = gl_popF[gr_id * (POPF_SIZE * DIMF) + idx1 + i * POPF_SIZE] + F*(gl_popF[gr_id * (POPF_SIZE * DIMF) + idx2 + i * POPF_SIZE] - gl_popF[gr_id * (POPF_SIZE * DIMF) + idx3 + i * POPF_SIZE]);
#elif defined(VARIANT_target_to_rand) //DE/target-to-rand/1/bin
                  //lo_popF[n + i * POPF_SIZE] = lo_popF[n + i * POPF_SIZE] + F*(lo_popF[idx1 + i * POPF_SIZE] - lo_popF[n + i * POPF_SIZE]) + F*(lo_popF[idx2 + i * POPF_SIZE] - lo_popF[idx3 + i * POPF_SIZE]);
                  lo_popF[n + i * POPF_SIZE] = gl_popF[gr_id * (POPF_SIZE * DIMF) + n + i * POPF_SIZE] + F*(gl_popF[gr_id * (POPF_SIZE * DIMF) + idx1 + i * POPF_SIZE] - gl_popF[gr_id * (POPF_SIZE * DIMF) + n + i * POPF_SIZE]) + F*(gl_popF[gr_id * (POPF_SIZE * DIMF) + idx2 + i * POPF_SIZE] - gl_popF[gr_id * (POPF_SIZE * DIMF) + idx3 + i * POPF_SIZE]);
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
         //}
      //}

      // each lo_id accesses diferent positions of gl_popF in the previous loop
      // and updates gl_popF in the loop below.
      // we can remove this barrier and eventually provide a kind of random mutation
      //barrier(CLK_LOCAL_MEM_FENCE);

      //for( int j = 0; j < (int) ceil(POPF_SIZE/(real_t)lo_size); ++j )
      //{
      //   n = j * lo_size + lo_id;
      //   if( n < POPF_SIZE )
      //   {
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
                  gl_popF[gr_id * (POPF_SIZE * DIMF) + n + (i * POPF_SIZE)] = lo_popF[n + (i * POPF_SIZE)];
               }
            } 
            else 
            {
               for( int i = 0; i < DIMF; i++ )
               {
                  lo_popF[n + (i * POPF_SIZE)] = gl_popF[gr_id * (POPF_SIZE * DIMF) + n + (i * POPF_SIZE)];
               }
            }
            // new generation
            // end

            // initialization of best_idx
            best_idx[n] = n;
         }
      }

      // each lo_id updates gl_popF in the previous loop
      // and will accesses diferent positions of gl_popF in the next generation.
      // we can remove this barrier and eventually provide a kind of random mutation
      //barrier(CLK_LOCAL_MEM_FENCE);

      seed_global[gl_id] = seed;
   }

   // reduction: best individual for each (uL, follower) or group gr_id
   // start
   // number of groups -> POPL_SIZE
   // VF -> POPL_SIZE * DIMF
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
               //if( fit_popF[n + k] < fit_popF[n] )
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

   // TODO: rever organizacao de popLValoresF, VF e VL, popL
   if( initialization )
   {
      for( int j = 0; j < (int) ceil(DIMF/(real_t)lo_size); ++j )
      {
         n = j * lo_size + lo_id;
         if( n < DIMF )
         {
            popLValoresF[follower * POPL_SIZE * DIMF + (gr_id % POPL_SIZE) + (n * POPL_SIZE)] = lo_popF[best_idx[0] + (n * POPF_SIZE)];
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
            VF[follower * POPL_SIZE * DIMF + (gr_id % POPL_SIZE) + (n * POPL_SIZE)] = lo_popF[best_idx[0] + (n * POPF_SIZE)];
         }
      }
   }
   // reduction: best individual
   // end
}

__kernel void
evaluate( __global real_t* popL, __global real_t* popLValoresF, __global real_t* VF, __global real_t* VL, __global real_t* fit_popL, __global real_t* fit_popLValoresF, int generation )
{
   int gl_id = get_global_id(0); //POPL_SIZE

   // solution S is a array of size D
   // popL, VL => S0D0 | S1D0 | S2D0 | ... | S0D1 | S1D1 | S2D1 | ...
   // popLValoreF, VF => S0D0F0 | S1D0F0 | S2D0F0 | ... | S0D1F0 | S1D1F0 | S2D1F0 | ... | S0D0F1 ...
   // gl_id -> solution S (S0, S1, S2, ...) of POPL

   real_t fit_VF[NF];
   real_t fit_VL = 0.;
   for( int follower = 0; follower < NF; follower++ )
   {
      fit_VL += evaluate_transpose_leader_level_1( gl_id, VL, VF, follower );
      fit_VF[follower] = evaluate_transpose_leader_level_2( gl_id, VL, VF, follower );
   }

   if( generation == 0 )
   {
      fit_popL[gl_id] = 0.;
      for( int follower = 0; follower < NF; follower++ )
      {
         fit_popL[gl_id] += evaluate_transpose_leader_level_1( gl_id, popL, popLValoresF, follower );
         fit_popLValoresF[(gl_id * NF) + follower] = evaluate_transpose_leader_level_2( gl_id, popL, popLValoresF, follower );
      }
   }

   if( fit_VL <= fit_popL[gl_id] )
   //if( fit_VL <= fit_popL[gl_id] && fit_VF <= fit_popLValoresF[gl_id] )
   //if( fit_VL <= fit_popL[gl_id] && fit_VF <= fit_popLValoresF[gl_id] && fit_VL > 0. && fit_VF > 0. )
   {
      for( int j = 0; j < DIML; j++ )
      {
         popL[gl_id + (j * POPL_SIZE)] = VL[gl_id + (j * POPL_SIZE)];
      }
      fit_popL[gl_id] = fit_VL;
      // popLValoreF, VF => S0D0F0 | S1D0F0 | S2D0F0 | ... | S0D1F0 | S1D1F0 | S2D1F0 | ... | S0D0F1 ...
      for( int follower = 0; follower < NF; follower++ )
      {
         for( int j = 0; j < DIMF; j++ )
         {
            popLValoresF[(follower * POPL_SIZE * DIMF) + gl_id + (j * POPL_SIZE)] = VF[(follower * POPL_SIZE * DIMF) + gl_id + (j * POPL_SIZE)];
         }
         fit_popLValoresF[(gl_id * NF) + follower] = fit_VF[follower];
      }
   }
}

