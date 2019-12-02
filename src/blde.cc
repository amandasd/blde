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
#include "definitions.h"
#include "util/CmdLineParser.h"
#include "util/Util.h"
//#ifdef _OPENMP
//#include <omp.h>
//#endif

//TODO: OpenMP
//TODO: cmake
//TODO: testar se eh PROFILE, etc...

using namespace std;

namespace { static struct t_data { int num_generation_leader; int population_leader_size; int leader_dimension; int follower_dimension; int r; int p; int q; int s; bool verbose; } data; };

void blde_init( int argc, char** argv ) 
{
   CmdLine::Parser Opts( argc, argv );

   Opts.Bool.Add( "-v", "--verbose" );

   Opts.Int.Add( "-gl", "--generation-leader", 10, 0, std::numeric_limits<int>::max() );
   Opts.Int.Add( "-pls", "--population-leader-size", 64, 1, std::numeric_limits<int>::max() );

   Opts.Int.Add( "-dl", "--dimension-leader", 8, 0, std::numeric_limits<int>::max() );
   Opts.Int.Add( "-df", "--dimension-follower", 8, 0, std::numeric_limits<int>::max() );

   Opts.Int.Add( "-t", "--threads", -1, 0);

   Opts.String.Add( "-function", "", "", "1001", "1002", "1003", "1004", "1005", "1006", "1007", "1008", "1009", NULL );

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
      data.s = ceil(  ( data.follower_dimension - data.r )/2. + EPS );
   }
   else
   {
      data.q = data.follower_dimension - data.r;
      data.s = 0.;
   }

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

   if( acc_follower_init( argc, argv, data.r, data.p, data.q, data.s ) )
   {
      fprintf(stderr,"Error in initialization phase.\n");
   }
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
   real_t* fit_popL = new real_t[data.population_leader_size]; 
   real_t* fit_popLValoresF = new real_t[data.population_leader_size]; 

   acc_seed();

   // popL and popLValoresF initialization
   // start
   acc_follower( 1 );
   // popL and popLValoresF initialization
   // end

   //TODO: criterio de parada -> nEval
	for( int g = 0; g < data.num_generation_leader; g++ )
   {  
      // for each uL there is a uF
      // POPL_SIZE uLs run simultaneously, so at the end you have POPL_SIZE uFs 
      acc_follower( 0 );

      // compare each new pair (uL, uF) with its respective old ones (popL, popLValoresF)
      acc_leader( fit_popL, fit_popLValoresF, g, popL, popLValoresF );

      // testa criterio de parada
      // start
		int idx = best_individual( fit_popL );
      if( fabs(fit_popL[idx]) < alpha )
      {                
          g = data.num_generation_leader;
      }                
      // testa criterio de parada
      // end

      if (data.verbose)
      {
         printf( "\n[%d] %.12f :: %.12f", g, fit_popL[idx], fit_popLValoresF[idx] ); 
         cout << "\n[Leader] ";
         for( int j = 0; j < data.leader_dimension; j++ ){
            cout << popL[idx + j * data.population_leader_size] << " ";
         }
         cout << "\n[Follower] ";
         for( int j = 0; j < data.follower_dimension; j++ ){
            cout << popLValoresF[idx + j * data.population_leader_size] << " ";
         }
         cout << endl;

      }
	}
   printf( "\n" ); 

   // best individual
   // start
   //int idx = best_individual( fit_popL );
   // best individual
   // end

   // print best individual
   // start
   //cout << "[Leader] ";
   //for( int j = 0; j < data.leader_dimension; j++ ){
   //   cout << popL[idx + j * data.population_leader_size] << " ";
   //}
   //cout << "Fitness: " << fit_popL[idx] << endl;
   //cout << "[Follower] ";
   //for( int j = 0; j < data.follower_dimension; j++ ){
   //   cout << popLValoresF[idx + j * data.population_leader_size] << " ";
   //}
   //cout << "Fitness: " << fit_popLValoresF[idx] << endl;
   // print best individual
   // end

   delete[] popL;
   delete[] popLValoresF;
   delete[] fit_popL;
   delete[] fit_popLValoresF;
}


