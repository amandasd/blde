
/*
 DE original
 */

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <iostream>
#include "funcoes.h"
#include "../src/util/Util.h"

using namespace std;

#define PENALTY 1

// variaveis e parametros do DE
int SIZEL;
int SIZEF;
int GENL;
int GENF;
int SEED;
int FUNCAO;
int DIML;
int DIMF;
double CR, F;
int VAR;
double alpha_leader = 10e-3;
double alpha_follower = 10e-4;

int MAX_NFE_L = 1000;

// populações do algoritmo
double **popL;
double **popLNova;
//Populacao para armazenar os valores da Follower correspondentes ao Leader em popL
double **popLValoresF;

//funcoes
void inicializaFollower(double** &pop, double *leader, int n, int d);
void inicializa(double** &pop, int n, int d, int nivel = 2);


// =============================================================================

void calculaVariancia(double **pop, double *var, int dim, int size){
        // variancias e medias de cada variavel
	double *med = new double[dim];

        for (int d = 0; d < dim; d++){
            double soma = 0;
            for (int n = 0 ; n < size ; n++){
                soma += pop[n][d];
            }
            med[d] = soma / size;
        }

        for (int d = 0; d < dim; d++){
            double soma_Pvar = 0;
            for (int n = 0 ; n < size ; n++){
                soma_Pvar += (pop[n][d] - med[d]) * (pop[n][d] - med[d]);
            }
            var[d] = soma_Pvar / size;
        }

        delete[] med;
}

void calculaAptidao(double *ind, int d, int nivel, double *leader, double *follower){
	if (follower == NULL || leader == NULL){
		ind[d] = RAND_MAX;
		ind[d + 1] = RAND_MAX;
		return;
	}
	calculaFuncao(ind, d, nivel, leader, follower, FUNCAO);
}


void imprimePopulacao(double **pop, int n, int d){
	for(int i = 0; i < n; i++){
		cout << i << ") ";
		for(int j = 0; j < d; j++){
			cout << pop[i][j] << " ";
		}
		cout << " Fit: " << pop[i][d] << " Const: " << pop[i][d+1];

		cout <<  " Foll.: ";
		for(int j = 0; j < DIMF; j++){
			cout << popLValoresF[i][j] << " ";
		}
		cout << " Fit: " << popLValoresF[i][DIMF] << " Const: " << popLValoresF[i][DIMF+1] << endl;
	}
}

void selecionaIndividuos(int &ind1, int &ind2, int &ind3, int i, int n){
	do{
		ind1 = rand()%n;
	} while(ind1 == i);

	do{
		ind2 = rand()%n;
	} while(ind2 == i || ind2 == ind1);

	do{
		ind3 = rand()%n;
	} while(ind3 == i || ind3 == ind1 || ind3 == ind2);
}

int compara(double *ind1, double *ind2, int d, int nivel){
    
	//Se a funcao eh de maximizacao compara com >= Caso contrario usa <=
		if (ind1[d] <= ind2[d]){
			return 1;
		} else {
			return 0;
		}
}

int selecionaMelhor(double *ind, double **pop, int n, int d, int nivel){
    
	int m = 0;
	for(int j = 0; j < d + 2; j++){
		ind[j] = pop[0][j];
	}
	for (int i = 1; i < n; i++){
		if(compara(pop[i], ind, d, nivel) > 0){
			for(int j = 0; j < d + 2; j++){
				ind[j] = pop[i][j];
				m = i;
			}
		}
	}
	return m;
}

int iguais(double *ind1, double *ind2, int d){
    
	for(int i = 0; i < d; i++){
		if (ind1[i] != ind2[i]){
			return 0;
		}
	}
	return 1;
   // 0 = diferentes ; 1 = iguais
}

void deFollower(double *uL, double *uF){
	
	double **popF;
	double **popFNova;
	inicializaFollower(popF, uL, SIZEF, DIMF);               
	inicializa(popFNova, SIZEF, DIMF);


	for (int gF = 0; gF < GENF; gF++){

		for(int i = 0; i < SIZEF; i++){
			
			int ind1, ind2, ind3;
			selecionaIndividuos(ind1, ind2, ind3, i, SIZEF);
			
			double *u = new double[DIMF + 2];
			int jRand = rand()%DIMF;
			for(int j = 0; j < DIMF; j++){
				if (j == jRand || rand()/(float)RAND_MAX < CR){
					if (VAR == 1){
						//--- DE/rand/1/bin
						u[j] = popF[ind1][j] + F*(popF[ind2][j] - popF[ind3][j]);
					} else if (VAR == 3){
						//--- DE/target-to-rand/1/bin
						u[j] = popF[i][j] + F*(popF[ind1][j] - popF[i][j]) + F*(popF[ind2][j] - popF[ind3][j]);
               }                                                                                                               

					if (u[j] < getLower(2, FUNCAO, j)) //LOWER
						u[j] = getLower(2, FUNCAO, j); 
					else if (u[j] > getUpper(2, FUNCAO, j)) //UPPER
						u[j] = getUpper(2, FUNCAO, j);
				} else {
					u[j] = popF[i][j];
				}
			}

			calculaAptidao(u, DIMF, 2, uL, u);

			
			if(u[DIMF] <= popF[i][DIMF]){
				for(int j = 0; j < DIMF + 2; j++){
					popFNova[i][j] = u[j];
				}
			} else {
				for(int j = 0; j < DIMF + 2; j++){
					popFNova[i][j] = popF[i][j];
				}
			}
			delete[] u;
		}
		//copia a populacao nova
		for (int i = 0; i < SIZEF; i++){
			for(int j = 0; j < DIMF + 2; j++){
				popF[i][j] = popFNova[i][j];
			}
		}

	}

	selecionaMelhor(uF, popF, SIZEF, DIMF, 2);

	for(int i = 0; i < SIZEF; i++){
		delete[] popF[i];
		delete[] popFNova[i];
	}
	delete[] popF;
	delete[] popFNova;

}

void imprimeCabecalho(){
	cout << "g leader ";
	for(int i = 0; i < DIML; i++){
		cout << "x" << i << " ";
	}
	cout << "fitLeader fitLeaderValue constLeader constLeaderValue follower ";
	for(int i = 0; i < DIMF; i++){
		cout << "y" << i << " ";
	}
	cout << "fitFollower fitFollowerValue constFollower constFollowerValue" << " nEvalL nEvalF" << endl;
}

void inicializaFollower(double** &pop, double *leader, int n, int d){
	pop = new double*[n];
	for (int i = 0; i < n; i++){
		pop[i] = new double[d + 2];
		for(int j = 0; j < d; j++){
			pop[i][j] = getLower(2, FUNCAO, j) + (rand()/(double)RAND_MAX)*(getUpper(2, FUNCAO, j) - getLower(2, FUNCAO, j)); //UPPER - LOWER2
		}
		calculaAptidao(pop[i], d, 2, leader, pop[i]);
	}
}


void inicializa(double** &pop, int n, int d, int nivel){

   pop = new double*[n];
   for (int i = 0; i < n; i++){
      pop[i] = new double[d + 2];
      for(int j = 0; j < d; j++){
         pop[i][j] = getLower(nivel, FUNCAO, j) + (rand()/(double)RAND_MAX)*(getUpper(nivel, FUNCAO, j) - getLower(nivel, FUNCAO, j)); //UPPER - LOWER
      }

      if (nivel == 1){ // leader                                        
         deFollower(pop[i], popLValoresF[i]);
         calculaAptidao(pop[i], DIML, 1, pop[i], popLValoresF[i]);
      } else {                        
         calculaAptidao(pop[i], d, 0, NULL, NULL); //coloca a pior aptidao possivel                                   
      }
   }
}

int best_individual(int idx, double** popL, double** popLValoresF)
{
   for( int i = 1; i < SIZEL; i++ )
   {
      //if( (popL[i][DIML] <= popL[idx][DIML]) )
      if( (popL[i][DIML] <= popL[idx][DIML]) && (popLValoresF[i][DIMF] <= popLValoresF[idx][DIMF]) )
      {     
         idx = i;
      }
   }
   return idx;
}

int main(int argc, char *argv[]){

   util::Timer t_init;

	for(int i = 0; i < argc; i++){
		if (strcmp(argv[i], "-genL") == 0){
			GENL  = atoi(argv[++i]);
		} else if (strcmp(argv[i], "-popL") == 0){
			SIZEL = atoi(argv[++i]);
		} else if (strcmp(argv[i], "-popF") == 0){
			SIZEF = atoi(argv[++i]);
		} else if (strcmp(argv[i], "-genF") == 0){
			GENF  = atoi(argv[++i]);
		} else if (strcmp(argv[i], "-seed") == 0){
			SEED  = atoi(argv[++i]);
		} else if (strcmp(argv[i], "-func") == 0){
			FUNCAO = atoi(argv[++i]);
		} else if (strcmp(argv[i], "-F") == 0){
			F = atof(argv[++i]);
      } else if (strcmp(argv[i], "-CR") == 0){
			CR = atof(argv[++i]);                        
		} else if (strcmp(argv[i], "-v") == 0){
			VAR = atof(argv[++i]);
      } else if (strcmp(argv[i], "-dimL") == 0){
         DIML = atof(argv[++i]);
      } else if (strcmp(argv[i], "-dimF") == 0){
         DIMF = atof(argv[++i]);
      }
	}


	srand(SEED);    
   // Apenas para funcoes SMDs
   if ( FUNCAO > 1000 ){           
       iniciaSMD(FUNCAO, DIML, DIMF);
   }
	DIML = getDimensao(FUNCAO, 1);
	DIMF = getDimensao(FUNCAO, 2);

   double time_init = t_init.elapsed();

   util::Timer t_initialization;

   popLValoresF = new double*[SIZEL];
   for (int i = 0; i < SIZEL; i++){
      popLValoresF[i] = new double[DIMF + 2];
   }

	inicializa(popL, SIZEL, DIML, 1);
	inicializa(popLNova, SIZEL, DIML, 0);       


   double time_initialization = t_initialization.elapsed();

   double time_follower = 0.0;
   double time_leader   = 0.0;

   bool stop_fit = false; bool stop_eps = false;
   int m = 0; int g;
	for(g = 0; g < GENL && !stop_eps; g++){                           
	//for(g = 0; g < GENL && !stop_fit; g++){                           
                
	   for(int i = 0; i < SIZEL; i++){

         util::Timer t_follower;

	   	int ind1, ind2, ind3;
	   	selecionaIndividuos(ind1, ind2, ind3, i, SIZEL);
	   	
	   	double *u = new double[DIML + 2];
	   	int jRand = rand()%DIML;
	   	for(int j = 0; j < DIML; j++){
	   		if (j == jRand || rand()/(float)RAND_MAX < CR){	
               if(VAR == 1){
	   				//DE/rand/1/bin
	   				u[j] = popL[ind1][j] + F*(popL[ind2][j] - popL[ind3][j]);
	   			} else if (VAR == 3){
	   				//DE/target-to-rand/1/bin
	   				u[j] = popL[i][j] + F*(popL[ind1][j] - popL[i][j]) + F*(popL[ind2][j] - popL[ind3][j]);
               }
	   		} else {
	   			u[j] = popL[i][j];
	   		}
	   		if (u[j] < getLower(1, FUNCAO, j))//LOWER
	   			u[j] = getLower(1, FUNCAO, j);
	   		else if (u[j] > getUpper(1, FUNCAO, j)) //UPPER
	   			u[j] = getUpper(1, FUNCAO, j);
	   	}

	   	double *uF = new double[DIMF + 2];  
                                   
	   	deFollower(u, uF);

         time_follower += t_follower.elapsed();
                                
         util::Timer t_leader;

         calculaAptidao(u, DIML, 1, u, uF);
                                   				
	   	//if(compara(u, popL[i], DIML, 1) > 0){
	   	if(compara(u, popL[i], DIML, 1) > 0 && compara(uF, popLValoresF[i], DIMF, 2) > 0){
	   		for(int j = 0; j < DIML + 2; j++){
	   			popLNova[i][j] = u[j];
	   		}
	   		for(int j = 0; j < DIMF + 2; j++){
	   			popLValoresF[i][j] = uF[j];
	   		}
	   	} else {
	   		for(int j = 0; j < DIML + 2; j++){
	   			popLNova[i][j] = popL[i][j];
	   		}
	   	}
	   	delete[] uF;
	   	delete[] u;

         time_leader += t_leader.elapsed();
	   }

      util::Timer t_leader;

	   //copia a populacao nova
	   for (int i = 0; i < SIZEL; i++){
	   	for(int j = 0; j < DIML + 2; j++){
	   		popL[i][j] = popLNova[i][j];
	   	}
	   }

      time_leader += t_leader.elapsed();
	
		m = best_individual(m, popL, popLValoresF);
      double eps = 0.; 
      for( int j = 0; j < DIML; j++ ){
         eps += popL[m][j] * popL[m][j];
      }   
      if (sqrt(eps) < 0.01) { stop_eps = true; }
      //if ( fabs(popL[m][DIML]) < alpha_leader && fabs(popLValoresF[m][DIMF]) < alpha_follower ) stop_fit = true;
	}

   printf("time_init: %lf, time_initialization: %lf, time_follower: %lf, time_leader: %lf, gen: %d, stop_eps: %d\n", time_init, time_initialization, time_follower/g, time_leader/g, g, stop_eps);

   //printf( "[%d] %.12f :: %.12f :: %d\n", g, popL[m][DIML], popLValoresF[m][DIMF], stop_fit ); 
}
