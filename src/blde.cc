#include <stdio.h> 
#include <stdlib.h>
#include <cmath>    
#include <limits>
#include <ctime>
#include <string>   
#include <sstream>
#include <iostream> 
#include <fstream> 
#include "follower/accelerator.h"
#include "blde.h"
#include "util/CmdLineParser.h"
#include "util/Util.h"
//#ifdef _OPENMP
//#include <omp.h>
//#endif

//TODO: OpenMP
//TODO: cmake
//TODO: testar se eh PROFILE, etc...
//TODO: VAR

using namespace std;

namespace { static struct t_data { int num_generation_leader; int population_leader_size; int leader_dimension; int follower_dimension; int r; int p; int q; int s; int parallel_version; bool verbose; } data; };

#include "blde_evaluate"

void blde_init( int argc, char** argv ) 
{
   CmdLine::Parser Opts( argc, argv );

   Opts.Bool.Add( "-v", "--verbose" );

   Opts.Bool.Add( "-acc" );

   Opts.Int.Add( "-gl", "--generations-leader", 10, 0, std::numeric_limits<int>::max() );
   Opts.Int.Add( "-pls", "--population-leader-size", 64, 1, std::numeric_limits<int>::max() );

   Opts.Int.Add( "-dl", "--dimension-leader", 8, 0, std::numeric_limits<int>::max() );
   Opts.Int.Add( "-df", "--dimension-follower", 8, 0, std::numeric_limits<int>::max() );

   Opts.Int.Add( "-t", "--threads", -1, 0);

   //TODO: pq o NULL? Como funciona essas opcoes?
   Opts.String.Add( "-function", "", "1001", "1002", "1003", "1004", "1005", "1006", "1007", "1008", "1009", NULL );

   // processing the command-line
   Opts.Process();

   // getting the results!
   data.verbose = Opts.Bool.Get("-v");

   data.num_generation_leader = Opts.Int.Get("-gl");
   data.population_leader_size = Opts.Int.Get("-pls");

   data.leader_dimension = Opts.Int.Get("-dl");
   data.follower_dimension = Opts.Int.Get("-df");

   data.r = floor( data.leader_dimension/2. );
   data.p = data.leader_dimension - data.r;
   if( Opts.String.Get("-function") == "1006" )
   {
      data.q = floor( ( data.follower_dimension - data.r )/2. - EPS );
   }
   else
   {
      data.q = data.follower_dimension - data.r;
   }
   data.s = ceil( ( data.follower_dimension - data.r )/2. + EPS );

   /*
      //////////////////////////////////////////////////////////////////////////
         Number of threads:
      //////////////////////////////////////////////////////////////////////////
      if '-t' is NOT given (i.e., -1), than uses only one thread (sequential)
      if '-t' is given as any value greater than 0, just set the number of threads to that value
      if '-t' is given as ZERO, than uses the maximum number of threads (= number of cores) or the value of the environment variable OMP_NUM_THREADS
   */
//#ifdef _OPENMP
//   if ( Opts.Int.Get("-t") < 0 ) // Uses the default (threads = 1 = sequencial)
//      omp_set_num_threads(1);
//   else if ( Opts.Int.Get("-t") > 0 ) // Uses the specified number of threads
//      omp_set_num_threads(Opts.Int.Get("-t"));
//#endif

   data.parallel_version = Opts.Bool.Get("-acc");
   if( data.parallel_version )
   {
      if( acc_follower_init( argc, argv, data.r, data.p, data.q, data.s ) )
      {
         fprintf(stderr,"Error in initialization phase.\n");
      }
   }
   //TODO
   //else
   //{
   //   seq_follower_init( );
   //}
}

int best_individual( real_t* fitness )
{
   int idx = 0;
   real_t fit_best = fitness[0];
   for( int i = 1; i < data.population_leader_size; i++ )
   {
      // The functions 1001, 1002, ..., 1008 are all minimization functions.
      // They do not have any restriction. 
      if( fitness[i] <= fit_best )
      {     
         fit_best = fitness[i];
         idx = i;
      }
   }
   return idx;
}

void blde_evolve()
{
   real_t* popL = new real_t[data.population_leader_size * data.leader_dimension]; 
   real_t* popLValoresF = new real_t[data.population_leader_size * data.follower_dimension];
   real_t* VF = new real_t[data.population_leader_size * data.follower_dimension];
   real_t* VL = new real_t[data.population_leader_size * data.leader_dimension];
   real_t* fit_popL = new real_t[data.population_leader_size]; 
   real_t  fit_VL;

   // solution S is a array of size D
   // popL, popLValoreF, VF e VL => S0D0 | S0D1 | S0D2 | ... | S1D0 | S1D1 | S1D2 | ...

   //TODO: sequential
   acc_follower( popLValoresF, VL, NULL, -1, 1 );

   //TODO: openMP
	for( int g = 0; g < 2; g++ )
	//for( int g = 0; g < data.num_generation_leader; g++ )
   {                           
      acc_follower( VF, VL, popL, g, 0 );

      //printf("%d ", g);
      //for( int j = 0; j < data.leader_dimension; j++ )
      //   printf("%f ", popL[10 * data.leader_dimension + j]);
      //printf("\n");

      //TODO: repensar esse loop como um outro kernel, pois assim economizaria na transferência de dados do popL, VF e VL entre CPU e GPU

//#pragma omp parallel for
      for( int i = 0; i < data.population_leader_size; i++ )
      {
         fit_VL = blde_evaluate( i, 1, VL, VF );
         if( g == 0 ) {fit_popL[i] = blde_evaluate( i, 1, popL, popLValoresF );}

         // The functions 1001, 1002, ..., 1008 are all minimization functions.
         // They do not have any restriction. 
	      if( fit_VL <= fit_popL[i] )
         {
            for( int j = 0; j < data.leader_dimension; j++ )
            {
               popL[i * data.leader_dimension + j] = VL[i * data.leader_dimension + j];
            }
            for( int j = 0; j < data.follower_dimension; j++ )
            {
               popLValoresF[i * data.follower_dimension + j] = VF[i * data.follower_dimension + j];
            }
            fit_popL[i] = fit_VL;
         }

         //TODO
         // testa criterio de parada
         // start
         //if ( getNEval(1) == MAX_NFE_L){  
         //    i = data.population_leader_size;
         //} 
         // testa criterio de parada
         // end
      }

      // testa criterio de parada
      // start
		int idx = best_individual( fit_popL );
      if( fabs(fit_popL[idx]) < alpha )
      {                
          g = data.num_generation_leader;
      }                
      // testa criterio de parada
      // end
	}

   // best individual
   // start
   int idx = best_individual( fit_popL );
   // best individual
   // end

   // print best individual
   // start
   cout << "[Leader] ";
   for( int j = 0; j < data.leader_dimension; j++ ){
      cout << popL[idx * data.leader_dimension + j] << " ";
   }
   cout << "Fitness: " << fit_popL[idx] << endl;
   cout << "[Follower] ";
   for( int j = 0; j < data.follower_dimension; j++ ){
      cout << popLValoresF[idx * data.leader_dimension + j] << " ";
   }
   cout << "Fitness: " << blde_evaluate( idx, 2, popL, popLValoresF ) << endl;
   // print best individual
   // end

   delete[] popL;
   delete[] popLValoresF;
   delete[] VF;
   delete[] VL;
}


