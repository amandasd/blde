#include "funcoes.h"
#include <iostream>
#include "stdio.h"
#include <math.h>
using namespace std;

//Armazena o numero de avaliacoes para cada nivel
int nEvalL = 0;
int nEvalF = 0;

//Dimensao dos problemas escalaveis SMD1 a SMD8
int DL, DF;
int p, q, r, s;

void iniciaSMD(int funcao, int dimL, int dimF){
    
        DL = dimL; DF = dimF;
        int DIM = DL+DF;
            
        // problemas 1001-1005,1007,1008
        if (funcao == 1006) {                
            r = floor(DL/2.);
            p = DL - r;
            q = floor((DF - r)/2. - EPS);
            s = ceil((DF - r)/2. + EPS);
            
        } else {                            
            r = floor(DL/2.);
            p = DL - r;
            q = DF - r;      
            s = 0;
            
        }        
//        cout << " r: " << r << " p: " << p << " q: " << q << " s: " << s << "  " << endl;
               
}

int getNEval(int nivel){
	if (nivel == 1)
		return nEvalL;
	else
		return nEvalF;
}

int getDimensao(int funcao, int nivel){       
            
	switch(funcao){
		case 1001: //SMD1 -Deb
			if (nivel == 1){
				return DL;
			} else {
				return DF;
			}
		break;
		case 1002: //SMD2
			if (nivel == 1){
				return DL;
			} else {
				return DF;
			}
		break;
		case 1003: //SMD3
			if (nivel == 1){
				return DL;
			} else {
				return DF;
			}
		break;
		case 1004: //SMD4
			if (nivel == 1){
				return DL;
			} else {
				return DF;
			}
		break;
                case 1005: //SMD5
			if (nivel == 1){
				return DL;
			} else {
				return DF;
			}
		break;
                case 1006: //SMD6
			if (nivel == 1){
				return DL;
			} else {
				return DF;
			}
		break;
                case 1007: //SMD7
			if (nivel == 1){
				return DL;
			} else {
				return DF;
			}
		break;
                case 1008: //SMD8
			if (nivel == 1){
				return DL;
			} else {
				return DF;
			}
		break;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
		case 1: // Shimizu,81(Ex.1)
			if (nivel == 1){
				return 1;
			} else {
				return 1;
			}
		break;
		case 2: //Shimize,81(Ex.2)
			if (nivel == 1){
				return 2;
			} else {
				return 2;
			}
		break;
		case 3: //Candler,82
			if (nivel == 1){
				return 2;
			} else {
				return 3;
			}
		break;
		case 4: //Bard,82(Ex.2)
			if (nivel == 1){
				return 2;
			} else {
				return 2;
			}
		break;
		case 5: //Aiyoshi,84(Ex.2)
			if (nivel == 1){
				return 2;
			} else if (nivel == 2){
				return 2;
			}
		break;
		case 6: //Bard,88(Ex.1)
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
		case 7: //Bard,88(Ex.2)
			if (nivel == 1){
				return 2;
			} else if (nivel == 2){
				return 2;
			}
		break;
		case 8: //Anandalingam,90
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
		case 9: //Savard,94
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 2;
			}
		break;
		case 10: //Falk,95
			if (nivel == 1){
				return 2;
			} else if (nivel == 2){
				return 2;
			}
		break;
		case 11: //Shimizu,95
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
		case 12: //Bard,98(pg.197)
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
		case 13: //Bard,98(pg.263)
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
                case 14: //Bard,98(pg.306)
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
                case 15: //Oduguwa,02(Ex.1)
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 2;
			}
		break;
                case 16: //Rajesh,03 (Ex.2)
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
                case 17: //Rajesh,03 (Ex.3)
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
                case 18: //Rajesh,03 (Ex.4)
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
		///// Novos exemplos para a apresentação Calvete2004
                case 19:
			if (nivel == 1){
				return 2;
			} else if (nivel == 2){
				return 6;
			}
		break;
	}
}

//Retorna se min (1) ou max (2)
int getTipo(int funcao, int nivel){
	switch(funcao){
		case 1001:
			if (nivel == 1){
				return 1;
			} else {
				return 1;
			}
		break;
		case 1002:
			if (nivel == 1){
				return 1;
			} else {
				return 1;
			}
		break;
		case 1003:
			if (nivel == 1){
				return 1;
			} else {
				return 1;
			}
		break;
		case 1004:
			if (nivel == 1){
				return 1;
			} else {
				return 1;
			}
		break;
                case 1005:
			if (nivel == 1){
				return 1;
			} else {
				return 1;
			}
		break;
                case 1006:
			if (nivel == 1){
				return 1;
			} else {
				return 1;
			}
		break;
                case 1007:
			if (nivel == 1){
				return 1;
			} else {
				return 1;
			}
		break;
                case 1008:
			if (nivel == 1){
				return 1;
			} else {
				return 1;
			}
		break;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
		case 1: //Shimizu,81
			if (nivel == 1){
				return 1;
			} else {
				return 1;
			}
		break;
		case 2: //Shimizu,81
			if (nivel == 1){
				return 1;
			} else {
				return 1;
			}
		break;
		case 3: //Candler,82
			if (nivel == 1){
				return 2;
			} else {
				return 2;
			}
		break;
		case 4: //Bard,82
			if (nivel == 1){
				return 2;
			} else {
				return 2;
			}
		break;
		case 5: //Aiyoshi,84
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
		case 6: //Bard,88
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
		case 7: //Bard,88
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
		case 8: //Anandalingam,90
			if (nivel == 1){
				return 2;
			} else if (nivel == 2){
				return 2;
			}
		break;
		case 9: //Savard,94
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
		case 10: //Falk,95
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
		case 11: //Shimizu,95
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
		case 12: //Bard,98
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
		case 13: //Bard,98
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
                case 14: //Bard,98
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
                case 15: //Oduguwa,02
			if (nivel == 1){
				return 2;
			} else if (nivel == 2){
				return 2;
			}
		break;
                case 16: //Rajesh,03
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
                case 17: ////Rajesh,03
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
                case 18: //Rajesh,03
			if (nivel == 1){
				return 2;
			} else if (nivel == 2){
				return 2;
			}
		break;
		///// Novos exemplos para a apresentação Calvete2004
		case 19:
			if (nivel == 1){
				return 1;
			} else if (nivel == 2){
				return 1;
			}
		break;
	}
}


void calculaFuncao(double *ind, int d, int nivel, double *leader, double *follower, int funcao){

	//Incrementa o numero de chamadas a funcao
	if (nivel == 1)
		nEvalL++;
	else 
		nEvalF++;                                            
    
        double *x = new double[DL];
        double *y = new double[DF];
	
        if (funcao == 1001){
                           
		double fit = 0, rest = 0.0;
                double F1 = 0.0, F2 = 0.0, F3 = 0.0;
                double sum, sum2;
                
		if (nivel == 1){                                       
                    for (int i = 0; i < DL; i++){
                        x[i] = ind[i];                        
                    }                    
                    for (int i = 0; i < DF; i++){                       
                        y[i] = follower[i];
                    }                                                    
                                        
                    for (int i = 0 ; i < p ; i++){
                        F1 += (x[i]*x[i]);
                    }
                    for (int i = 0 ; i < q ; i++){
                        F2 += (y[i]*y[i]);
                    }                                      
                    sum = 0.0; sum2 = 0.0;
                    for (int i = 0 ; i < r ; i++){
                        sum += (x[p+i]*x[p+i]);
                        sum2 += ((x[p+i]-tan(y[q+i]))*(x[p+i]-tan(y[q+i])));
                    }
                    F3 = sum+sum2;
                    fit = F1+F2+F3;
                   
                } else if (nivel == 2){
                    for (int i = 0; i < DL; i++){
                        x[i] = leader[i];                        
                    }                    
                    for (int i = 0; i < DF; i++){                       
                        y[i] = ind[i];
                    }                                                    

                    for (int i = 0 ; i < p ; i++){
                        F1 += (x[i]*x[i]);
                    }                                                          
                    for (int i = 0 ; i < q ; i++){
                        F2 += (y[i]*y[i]);
                    }                    
                    for (int i = 0 ; i < r ; i++){                        
                        F3 += ((x[p+i] - tan(y[q+i]))*(x[p+i] - tan(y[q+i])));
                    }                    
                    fit = F1+F2+F3;
                    
                }
                ind[d] = fit;
		ind[d + 1] = rest;

        } 
        else if (funcao == 1002){
                
		double fit = 0, rest = 0.0;
                double F1 = 0.0, F2 = 0.0, F3 = 0.0;
                double sum, sum2, prod; 

		if (nivel == 1){
                    for (int i = 0; i < DL; i++){
                        x[i] = ind[i];                        
                    }                    
                    for (int i = 0; i < DF; i++){                       
                        y[i] = follower[i];
                    }  
                    for (int i = 0 ; i < p ; i++){
                        F1 += (x[i]*x[i]);
                    }
                    sum = 0.0;
                    for (int i = 0 ; i < q ; i++){
                        sum += (y[i]*y[i]);
                    }
                    F2 = -(sum);
                    sum = 0.0; sum2 = 0.0;
                    for (int i = 0 ; i < r ; i++){
                        sum += ( x[p+i]*x[p+i] );
                        sum2 += ((x[p+i]-log(y[q+i]))*(x[p+i]-log(y[q+i])));
                    }
                    F3 = sum - sum2;
                    fit = F1+F2+F3;                   

                } else if (nivel == 2){
                    
                    for (int i = 0; i < DL; i++){
                        x[i] = leader[i];                        
                    }                    
                    for (int i = 0; i < DF; i++){                       
                        y[i] = ind[i];
                    }                                                    

                    for (int i = 0 ; i < p ; i++){
                        F1 += (x[i]*x[i]);
                    }                                                          
                    for (int i = 0 ; i < q ; i++){
                        F2 += (y[i]*y[i]);
                    }                    
                    for (int i = 0 ; i < r ; i++){                        
                        F3 += ((x[p+i] - log(y[q+i]))*(x[p+i] - log(y[q+i])));
                    }                    
                    fit = F1+F2+F3;

                }
                ind[d] = fit;
		ind[d + 1] = rest;


        } 
        else if (funcao == 1003){
                
		double fit = 0, rest = 0.0;
                double F1 = 0.0, F2 = 0.0, F3 = 0.0;
                double sum, sum2; 

		if (nivel == 1){
                    for (int i = 0; i < DL; i++){
                        x[i] = ind[i];                        
                    }                    
                    for (int i = 0; i < DF; i++){                       
                        y[i] = follower[i];
                    }  

                    for (int i = 0 ; i < p ; i++){
                        F1 += (x[i]*x[i]);
                    }
                    for (int i = 0 ; i < q ; i++){
                        F2 += (y[i]*y[i]);
                    }                                      
                    sum = 0.0; sum2 = 0.0;
                    for (int i = 0 ; i < r ; i++){
                        sum += (x[p+i]*x[p+i]);
                        sum2 += ( ((x[p+i]*x[p+i])-tan(y[q+i]))*((x[p+i]*x[p+i])-tan(y[q+i])) );
                    }
                    F3 = sum+sum2;
                    fit = F1+F2+F3;
                                       
                } else if (nivel == 2){
                    
                    for (int i = 0; i < DL; i++){
                        x[i] = leader[i];                        
                    }                    
                    for (int i = 0; i < DF; i++){                       
                        y[i] = ind[i];
                    }  

                    for (int i = 0 ; i < p ; i++){
                        F1 += (x[i]*x[i]);
                    }         
                    F2 = q;
                    for (int i = 0 ; i < q ; i++){
                        F2 += ( (y[i]*y[i]) - cos( 2*M_PI*y[i] ));
                    }                    
                    for (int i = 0 ; i < r ; i++){                        
                        F3 += (((x[p+i]*x[p+i])-tan(y[q+i]))*((x[p+i]*x[p+i])-tan(y[q+i])));
                    }                    
                    fit = F1 + F2 + F3;                                        

                }
                ind[d] = fit;
		ind[d + 1] = rest;


        } 
        else if (funcao == 1004){
                
		double fit = 0, rest = 0.0;
                double F1 = 0.0, F2 = 0.0, F3 = 0.0;
                double sum, sum2, prod; 

		if (nivel == 1){
                    for (int i = 0; i < DL; i++){
                        x[i] = ind[i];                        
                    }                    
                    for (int i = 0; i < DF; i++){                       
                        y[i] = follower[i];
                    }  
                                       
                    for (int i = 0 ; i < p ; i++){
                        F1 += (x[i]*x[i]);
                    }
                    sum = 0.0;
                    for (int i = 0 ; i < q ; i++){
                        sum += (y[i]*y[i]);
                    }         
                    F2 = -(sum);
                    sum = 0.0; sum2 = 0.0;
                    for (int i = 0 ; i < r ; i++){
                        sum += (x[p+i]*x[p+i]);
                        sum2 += ( (fabs(x[p+i])-log(1+y[q+i]))*(fabs(x[p+i])-log(1+y[q+i])) );
                    }
                    F3 = sum - sum2;
                    fit = F1+F2+F3;                    

                } else if (nivel == 2){
                    for (int i = 0; i < DL; i++){
                        x[i] = leader[i];                        
                    }                    
                    for (int i = 0; i < DF; i++){                       
                        y[i] = ind[i];
                    }  

                    for (int i = 0 ; i < p ; i++){
                        F1 += (x[i]*x[i]);
                    }         
                    F2 = q;
                    for (int i = 0 ; i < q ; i++){
                        F2 += ( (y[i]*y[i]) - cos( 2*M_PI*y[i] ));
                    }                    
                    for (int i = 0 ; i < r ; i++){                        
                        F3 += ((fabs(x[p+i])-log(1+y[q+i]))*(fabs(x[p+i])-log(1+y[q+i])));
                    }                    
                    fit = F1 + F2 + F3;                       
                }
                ind[d] = fit;
		ind[d + 1] = rest;

        } 
        else if (funcao == 1005){
                
		double fit = 0, rest = 0.0;
                double F1 = 0.0, F2 = 0.0, F3 = 0.0;
                double sum, sum2, prod; 

		if (nivel == 1){
                    for (int i = 0; i < DL; i++){
                        x[i] = ind[i];                        
                    }                    
                    for (int i = 0; i < DF; i++){                       
                        y[i] = follower[i];
                    }  

                    for (int i = 0 ; i < p ; i++){
                        F1 += x[i]*x[i];
                    }
                    sum = 0.0;
                    for (int i = 0 ; i < q-1 ; i++){
                        sum += (y[i+1]-(y[i]*y[i]))*(y[i+1]-(y[i]*y[i])) + ((y[i]-1.)*(y[i]-1.));                        
                    }         
                    F2 = -(sum);
                    sum = 0.0; sum2 = 0.0;
                    for (int i = 0 ; i < r ; i++){
                        sum += x[p+i]*x[p+i];
                        sum2 += (fabs(x[p+i])-(y[q+i]*y[q+i]))*(fabs(x[p+i])-(y[q+i]*y[q+i]));
                    }
                    F3 = sum - sum2;
                    fit = F1+F2+F3;   
                    
                } else if (nivel == 2){
                    for (int i = 0; i < DL; i++){
                        x[i] = leader[i];                        
                    }                    
                    for (int i = 0; i < DF; i++){                       
                        y[i] = ind[i];
                    }  
                    
                    for (int i = 0 ; i < p ; i++){
                        F1 += x[i]*x[i];
                    }         
                    for (int i = 0 ; i < q-1 ; i++){
                        F2 += (y[i+1]-(y[i]*y[i]))*(y[i+1]-(y[i]*y[i])) + ((y[i]-1.)*(y[i]-1.));                        
                    }                             
                    for (int i = 0 ; i < r ; i++){                        
                        F3 += (fabs(x[p+i])-(y[q+i]*y[q+i]))*(fabs(x[p+i])-(y[q+i]*y[q+i]));
                    }                    
                    fit = F1 + F2 + F3;                                         

                }
                ind[d] = fit;
		ind[d + 1] = rest;


        } 
        else if (funcao == 1006){                            
                
                double fit = 0, rest = 0.0;
                double F1 = 0.0, F2 = 0.0, F3 = 0.0;
                double sum, sum2, prod; 

		if (nivel == 1){
                    for (int i = 0; i < DL; i++){
                        x[i] = ind[i];                        
                    }                    
                    for (int i = 0; i < DF; i++){                       
                        y[i] = follower[i];
                    }  

                    for (int i = 0 ; i < p ; i++){
                        F1 += (x[i]*x[i]);
                    }
                    sum = 0.0; sum2 = 0.0;
                    for (int i = 0 ; i < q ; i++){
                        sum += ( y[i]*y[i] );
                    }
                    for (int i = q ; i < q+s ; i++){
                        sum2 += ( y[i]*y[i] );
                    }
                    F2 = - sum + sum2;
                    sum = 0.0; sum2 = 0.0;
                    for (int i = 0 ; i < r ; i++){
                        sum += ( x[p+i]*x[p+i] );
                        sum2 += (( x[p+i]-y[q+s+i] )*( x[p+i]-y[q+s+i] ));
                    }
                    F3 = sum - sum2;
                    fit = F1+F2+F3;                       

                } else if (nivel == 2){
                    for (int i = 0; i < DL; i++){
                        x[i] = leader[i];                        
                    }                    
                    for (int i = 0; i < DF; i++){                       
                        y[i] = ind[i];
                    }
                    
                    for (int i = 0 ; i < p ; i++){
                        F1 += ( x[i]*x[i] );
                    }         
                    sum = 0.0; 
                    for (int i = 0 ; i < q ; i++){
                        sum += ( (y[i]*y[i]) );
                    }                     
                    sum2 = 0.0;
                    for (int i = q ; i < q+s-1 ; i = i+2){
                        sum2 += ( (y[i+1] - y[i])*(y[i+1] - y[i]) );
                    }      
                    F2 =  sum + sum2;
                    for (int i = 0 ; i < r ; i++){                        
                        F3 += (( x[p+i]-y[q+s+i] )*( x[p+i]-y[q+s+i] ));
                    }                    
                    fit = F1 + F2 + F3;     
                    
                }
                ind[d] = fit;
		ind[d + 1] = rest;
                
        } 
        else if (funcao == 1007){
                                                                              
		double fit = 0, rest = 0.0;
                double F1 = 0.0, F2 = 0.0, F3 = 0.0;
                double sum, sum2, prod;                                          

		if (nivel == 1){                    
                    for (int i = 0; i < DL; i++){
                        x[i] = ind[i];                        
                    }                    
                    for (int i = 0; i < DF; i++){                       
                        y[i] = follower[i];
                    }                    
                                       
                    sum = 0.0;
                    prod = 1.0;
                    for (int i = 0; i < p; i++){
                        sum += ( x[i]*x[i] );
                        prod *= ( cos( x[i]/sqrt(i+1) ) );                                
                    }                    
                    F1 = 1. + ( (1/400.) * sum ) - prod;                                        
                    for (int i = 0; i < q; i++){
                        F2 += ( y[i]*y[i] );
                    }
                    F2 = -(F2);
                    sum = 0.0; sum2 = 0.0;
                    int j = p;
                    int w = q;
                    for (int i = 0; i < r; i++){
                        sum += ( x[j]*x[j] );
                        sum2 += ( (x[j] - log(y[w]))*(x[j] - log(y[w])) );
                        j++; w++;
                    }
                    F3 = sum - sum2;  
                    fit = F1+F2+F3;                                        

                } else if (nivel == 2){
                                        
                    for (int i = 0; i < DL; i++){
                        x[i] = leader[i];                        
                    }  
                    for (int i = 0; i < DF; i++){
                        y[i] = ind[i];
                    }  
                                        
                    for (int i = 0; i < p; i++){
                        F1 += ( x[i]*x[i]*x[i] );                        
                    }                                                
                    for (int i = 0; i < q; i++){
                        F2 += ( y[i]*y[i] );                        
                    }                                           
                    for (int i = 0; i < r; i++){
                        F3 += ( (x[p+i]-log(y[q+i]))*(x[p+i]-log(y[q+i])) );
                    }          
                    fit = F1+F2+F3;
                    
                                        
                }
                                              
                ind[d] = fit;
		ind[d + 1] = rest;
        }
        else if (funcao == 1008){
            
          	double fit = 0, rest = 0.0;
                double F1 = 0.0, F2 = 0.0, F3 = 0.0;
                double sum, sum2; 

		if (nivel == 1){
                    for (int i = 0; i < DL; i++){
                        x[i] = ind[i];                        
                    }                    
                    for (int i = 0; i < DF; i++){                       
                        y[i] = follower[i];
                    } 
                    
                    sum = 0.0; sum2 = 0.0;
                    for (int i = 0; i < p ; i++){
                        sum += x[i]*x[i];
                        sum2 += cos(2*M_PI*x[i]);  
                    }
                    F1 = 20. + M_E - (20. * exp(-0.2 * sqrt((1./p)*sum))) - exp((1./p)*sum2);                    
                    sum = 0.0;
                    for (int i = 0; i < q-1 ; i++){
                        sum += (y[i+1]-(y[i]*y[i])) + ((y[i]-1)*(y[i]-1));
                        //sum += ( ( (y[i+1]-(y[i]*y[i]))*(y[i+1]-(y[i]*y[i])) ) + ((y[i]-1)*(y[i]-1)));
                    }
                    F2 = -(sum);
                    sum = 0.0; sum2 = 0.0;
                    for (int i = 0; i < r; i++){
                        sum += x[p+i]*x[p+i];
                        sum2 += (x[p+i]-(y[q+i]*y[q+i]*y[q+i]))*(x[p+i]-(y[q+i]*y[q+i]*y[q+i]));
                    }
                    F3 = sum - sum2;
                    fit = F1+F2+F3;
 
                } else if (nivel == 2){
                    for (int i = 0; i < DL; i++){
                        x[i] = leader[i];                        
                    }  
                    for (int i = 0; i < DF; i++){
                        y[i] = ind[i];
                    } 
                    
                    for (int i = 0 ; i < p ; i++){
                        F1 += fabs(x[i]);
                    }                       
                    for (int i = 0 ; i < q-1 ; i++){
                        F2 += (y[i+1]-(y[i]*y[i])) + ((y[i]-1)*(y[i]-1));
                        //F2 += ( ((y[i+1]-(y[i]*y[i]))*(y[i+1]-(y[i]*y[i]))) + ((y[i]-1)*(y[i]-1)) );
                    }                    
                    for (int i = 0; i < r; i++){                        
                        F3 += (x[p+i]-(y[q+i]*y[q+i]*y[q+i]))*(x[p+i]-(y[q+i]*y[q+i]*y[q+i]));
                    }
                    fit = F1+F2+F3;
                    
                }
                ind[d] = fit;
		ind[d + 1] = rest;
                
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
        } else if (funcao == 1){
		//Shimizu,81(Ex.1) - (10,10) => f1=100, f2=0
		double x, y;
		double fit = 0, rest = 0;
		double v;
		if (nivel == 1){
			x = ind[0];
			y = follower[0];

			fit = x*x + (y - 10)*(y - 10);
			
			//LEADER
			rest = 0.0;
			v = -x + y;
			if (v > EPS){
				rest += v;
			}
		} else if (nivel == 2){
			y = ind[0];
			x = leader[0];

			fit = (x + 2*y - 30)*(x + 2*y - 30);

			//FOLLOWER
			v = x + y -20;
			if (v > EPS){
				rest += v;
			}
		}
		ind[d] = fit;
		ind[d + 1] = rest;

        } else if (funcao == 2){
		//Shimizu,81(Ex.2) - (20,5,10,5) => f1=225, f2=100
		double x1, x2, y1, y2;
		double fit = 0, rest = 0;
		double v;
		if (nivel == 1){
			x1 = ind[0];
                        x2 = ind[1];
			y1 = follower[0];
                        y2 = follower[1];

			fit = (x1-30)*(x1-30)+(x2-20)*(x2-20)-20*y1+20*y2;
			rest = 0;
			//LEADER
			rest = 0.0;
			v = -(x1 + 2*x2 - 30);
			if (v > EPS){
				rest += v;
			}
			v = x1+x2-25;
			if (v > EPS){
				rest += v;
			}
			v = x2-15;
			if (v > EPS){
				rest += v;
			}
		} else if (nivel == 2){
			y1 = ind[0];
                        y2 = ind[1];
			x1 = leader[0];
                        x2 = leader[1];

			fit = (x1-y1)*(x1-y1) + (x2-y2)*(x2-y2);
                        rest = 0.0;
			v = y1 - 10;
			if (v > EPS){
				rest += v;
			}
			v = y2 - 10;
			if (v > EPS){
				rest += v;
			}


		}
		ind[d] = fit;
		ind[d + 1] = rest;

	} else if (funcao == 3){
		//Candler,82 - (0,0.9,0,0.4,0.4) f1=29.2, f2=-3.2
		double x1, x2, y1, y2, y3;
		double fit = 0, rest = 0;
		if (nivel == 1){
			x1 = ind[0];
			x2 = ind[1];
			y1 = follower[0];
			y2 = follower[1];
			y3 = follower[2];

			fit = (8*x1 + 4*x2 - 4*y1 + 40*y2 + 4*y3);
			rest = 0;
		} else if (nivel == 2){
			y1 = ind[0];
			y2 = ind[1];
			y3 = ind[2];
			x1 = leader[0];
			x2 = leader[1];

			fit = (-x1 - 2*x2 - y1 - y2 - 2*y3);

			rest = 0.0;
			double v = -y1 + y2 + y3 - 1;
			if (v > EPS){
				rest += v;
			}
			v = 2*x1 - y1 + 2*y2 - 0.5*y3 - 1;
			if (v > EPS){
				rest += v;
			}
			v = 2*x2 + 2*y1 - y2 - 0.5*y3 - 1;
			if (v > EPS){
				rest += v;
			}
		}
		ind[d] = fit;
		ind[d + 1] = rest;

        } else if (funcao == 4){
		//Bard,82(Ex.2) - (1,0,0.5,1) f1=1.75, f2=0
		double x1,x2, y1, y2;
		double fit = 0, rest = 0;
		if (nivel == 1){
			x1 = ind[0];
			x2 = ind[1];
			y1 = follower[0];
			y2 = follower[1];

			fit = (2*x1 - x2 - 0.5*y1);
			rest = 0;
		} else if (nivel == 2){
			y1 = ind[0];
			y2 = ind[1];
			x1 = leader[0];
			x2 = leader[1];

			fit = (-x1 -x2 + 4*y1 - y2);

			rest = 0.0;
			double v = -(2*x1 - y1 + y2 - 2.5);
			if (v > EPS){
				rest += v;
			}
			v = -(-x1 + 3*x2 - y2 + 2);
			if (v > EPS){
				rest += v;
			}
			v = -(-x1 - x2 + 2);
			if (v > EPS){
				rest += v;
			}
		}
		ind[d] = fit;
		ind[d + 1] = rest;

        } else if (funcao == 5){
		//Aiyoshi,84(Ex,2) - (25,30,5,10) f1=5, f2=0
		double x1, x2, y1, y2;
		double fit = 0, rest = 0;
                double v;
		if (nivel == 1){
			x1 = ind[0];
                        x2 = ind[1];
			y1 = follower[0];
                        y2 = follower[1];

			fit = 2*x1 +2*x2 -3*y1 -3*y2 -60;
			rest = 0.0;

                        v = x1+x2+y1-2*y2-40;
			if (v > EPS){
				rest += v;
			}

		} else if (nivel == 2){
			y1 = ind[0];
                        y2 = ind[1];
			x1 = leader[0];
                        x2 = leader[1];

			fit = (y1-x1+20)*(y1-x1+20)+(y2-x2+20)*(y2-x2+20);

			rest = 0.0;
			v = 2*y1-x1+10;
			if (v > EPS){
				rest += v;
			}
			v = 2*y2-x2+10;
			if (v > EPS){
				rest += v;
			}
		}
		// x1,x2 em [0,50]
                v = x1-50;
		if (v > EPS){
			rest += v;
		}
                v = x2-50;
		if (v > EPS){
			rest += v;
		}
                //y1,y2 em [-10,20]
                v = y1-20;
		if (v > EPS){
			rest += v;
		}
                v = y2-20;
		if (v > EPS){
			rest += v;
		}
                v = -(y1 + 10);
		if (v > EPS){
			rest += v;
		}
                v = -(y2 + 10);
		if (v > EPS){
			rest += v;
		}
		ind[d] = fit;
		ind[d + 1] = rest;

        } else if (funcao == 6){
		//BBard,88(Ex.1) - (1.778,2.333) f1=42.5, f2=-4.445
		double x, y;
		double fit = 0, rest = 0;
                double v;
		if (nivel == 1){
			x = ind[0];
			y = follower[0];

			fit = (x-5)*(x-5) + (2*y+1)*(2*y+1);
			rest = 0;

		} else if (nivel == 2){
			y = ind[0];
			x = leader[0];

			fit = (y-1)*(y-1) -1.5*x*y;

			rest = 0.0;
			v = -(3*x-y-3);
			if (v > EPS){
				rest += v;
			}
			v = -(-x+0.5*y+4);
			if (v > EPS){
				rest += v;
			}
			v = -(-x-y+7);
			if (v > EPS){
				rest += v;
			}
		}
		ind[d] = fit;
		ind[d + 1] = rest;

        } else if (funcao == 7){
		//BBard,88(Ex.3) - (0,2,4,1) f1=-21, f2=11
		double x1, x2, y1, y2;
		double fit = 0, rest = 0;
                double v;
		if (nivel == 1){
			x1 = ind[0];
                        x2 = ind[1];
			y1 = follower[0];
                        y2 = follower[1];

			fit = - x1*x1 -3*x2 -4*y1 +y2*y2;

			rest = 0.0;
                        v = x1*x1 + 2*x2 -4;
			if (v > EPS){
				rest += v;
			}

		} else if (nivel == 2){
			y1 = ind[0];
                        y2 = ind[1];
			x1 = leader[0];
                        x2 = leader[1];

			fit = 2*x1*x1 + y1*y1 - 5*y2;

			rest = 0.0;
			v = -(x1*x1 -2*x1 +x2*x2 -2*y1 +y2 +3);
			if (v > EPS){
				rest += v;
			}
			v = -(x2 +3*y1 -4*y2 -4);
			if (v > EPS){
				rest += v;
			}
		}
		ind[d] = fit;
		ind[d + 1] = rest;
                
	} else if (funcao == 8){
		//Anandalingam,90 - (16,11) f1=49, f2=-17
		double x, y;
		double fit = 0, rest = 0;
		if (nivel == 1){
			x = ind[0];
			y = follower[0];

			fit = (x + 3*y);
			rest = 0;
		} else if (nivel == 2){
			y = ind[0];
			x = leader[0];

			fit = (x - 3*y);

			rest = 0.0;
			double v = -x -2*y + 10;
			if (v > EPS){
				rest += v;
			}
			v = x - 2*y - 6;
			if (v > EPS){
				rest += v;
			}
			v = 2*x - y - 21;
			if (v > EPS){
				rest += v;
			}
			v = x + 2*y - 38;
			if (v > EPS){
				rest += v;
			}
			v = -x + 2*y - 18;
			if (v > EPS){
				rest += v;
			}
		}
		ind[d] = fit;
		ind[d + 1] = rest;

        } else if (funcao == 9){
		//Savard,94 - (1.889,0.889) f1=-1.21, f2=7.61
		double x, y1, y2;
		double fit = 0, rest = 0;
		if (nivel == 1){
			x = ind[0];
			y1 = follower[0];
			y2 = follower[1];

			fit = (x - 1)*(x - 1) + 2*y1*y1 - 2*x;
			rest = 0;
		} else if (nivel == 2){
			y1 = ind[0];
			y2 = ind[1];
			x = leader[0];

			fit = (2*y1 - 4)*(2*y1 - 4) + (2*y2 - 1)*(2*y2 - 1) + x*y1;

			rest = 0.0;
			double v = 4*x + 5*y1 + 4*y2 - 12;
			if (v > EPS){
				rest += v;
			}
			v = -4*x - 5*y1 + 4*y2 + 4;
			if (v > EPS){
				rest += v;
			}
			v = 4*x - 4*y1 + 5*y2 - 4;
			if (v > EPS){
				rest += v;
			}
			v = -4*x + 4*y1 + 5*y2 - 4;
			if (v > EPS){
				rest += v;
			}
		}
		ind[d] = fit;
		ind[d + 1] = rest;

        } else if (funcao == 10){
		//Falk,95 - (0.5,.0.5,0.5,0.5) f1=-1, f2=0
		double x1, x2, y1, y2;
		double fit = 0, rest = 0;
                double v;
		if (nivel == 1){
			x1 = ind[0];
                        x2 = ind[1];
			y1 = follower[0];
			y2 = follower[1];

			fit = x1*x1 -2*x1 +x2*x2 -2*x2 + y1*y1 + y2*y2;

			rest = 0.0;

		} else if (nivel == 2){
			y1 = ind[0];
			y2 = ind[1];
			x1 = leader[0];
                        x2 = leader[1];

			fit = (y1-x1)*(y1-x1)+(y2-x2)*(y2-x2);

			rest = 0.0;
			v = 0.5-y1;
			if (v > EPS){
				rest += v;
			}
			v = 0.5-y2;
			if (v > EPS){
				rest += v;
			}
			v = y1-1.5;
			if (v > EPS){
				rest += v;
			}
			v = y2-1.5;
			if (v > EPS){
				rest += v;
			}
		}
		ind[d] = fit;
		ind[d + 1] = rest;

        } else if (funcao == 11){
		//Shimizu,95 - (11.25,5) f1=2250, f2=197.75
		double x, y;
		double fit = 0, rest = 0;
                double v;
		if (nivel == 1){
			x = ind[0];;
			y = follower[0];

			fit = 16*x*x + 9*y*y;

			rest = 0;
                        v = -4*x +y;
			if (v > EPS){
				rest += v;
			}

		} else if (nivel == 2){
			y = ind[0];
			x = leader[0];

			fit = (x+y-20)*(x+y-20)*(x+y-20)*(x+y-20);

			rest = 0.0;
			v = 4*x +y -50;
			if (v > EPS){
				rest += v;
			}
		}
		ind[d] = fit;
		ind[d + 1] = rest;

        } else if (funcao == 12){
		//Bard,98(pg.197) - (4,4) f1=-12, f2=4
		double x, y;
		double fit = 0, rest = 0;
                double v;

		if (nivel == 1){
			x = ind[0];;
			y = follower[0];

			fit = (x - 4*y);
			rest = 0.0;

		} else if (nivel == 2){
			y = ind[0];
			x = leader[0];

			fit = y;

			rest = 0.0;
			v = -x -y + 3;
			if (v > EPS){
				rest += v;
			}
			v = -2*x + y;
			if (v > EPS){
				rest += v;
			}
			v = 2*x + y -12;
			if (v > EPS){
				rest += v;
			}
			v = -(-3*x + 2*y + 4);
			if (v > EPS){
				rest += v;
			}
		}
		ind[d] = fit;
		ind[d + 1] = rest;

        } else if (funcao == 13){
		//Bard,98(pg.263) - (0.889,2.222) f1=3.111, f2=-6.662
		double x, y;
		double fit = 0, rest = 0;
                double v;

		if (nivel == 1){
			x = ind[0];;
			y = follower[0];

			fit = x+y;

			rest = 0.0;

		} else if (nivel == 2){
			y = ind[0];
			x = leader[0];

			fit = -5*x -y ;

			rest = 0.0;
			v = -x -y/2 +2;
			if (v > EPS){
				rest += v;
			}
			v = -x/4 +y -2;
			if (v > EPS){
				rest += v;
			}
			v = x +y/2 -8;
			if (v > EPS){
				rest += v;
			}
			v = x -2*y -4;
			if (v > EPS){
				rest += v;
			}
		}
		ind[d] = fit;
		ind[d + 1] = rest;

        } else if (funcao == 14){
		//Bard,98(pg.306) - (10.02,0.82) f1=81.33, f2=-0.4838
		double x, y;
		double fit = 0, rest = 0;
		if (nivel == 1){
			x = ind[0];;
			y = follower[0];

			fit = ((x - 1)*(x - 1) + (y -1)*(y - 1));

		} else if (nivel == 2){
			y = ind[0];
			x = leader[0];

			fit = 0.5*y*y + 500*y - 50*x*y;
		}
		rest = 0.0;
		ind[d] = fit;
		ind[d + 1] = rest;

	} else if (funcao == 15){
		//Oduguwa,02(Ex.1) - (?,?) f1=1000, f2=1
		double x, y1, y2;
		double fit = 0, rest = 0;
		if (nivel == 1){
			x = ind[0];
			y1 = follower[0];
			y2 = follower[1];

			fit = (100*x + 1000*y1);
			rest = 0;
		} else if (nivel == 2){
			y1 = ind[0];
			y2 = ind[1];
			x = leader[0];

			fit = (y1 + y2);

			rest = 0.0;
			double v = x + y1 - y2 -1;
			if (v > EPS){
				rest += v;
			}
			v = y1 + y2 - 1;
			if (v > EPS){
				rest += v;
			}
		}
		double v;
		v = x - 1;
		if (v > EPS){
			rest += v;
		}
		v = y1 - 1;
		if (v > EPS){
			rest += v;
		}
		v = y2 - 1;
		if (v > EPS){
			rest += v;
		}
		ind[d] = fit;
		ind[d + 1] = rest;

        } else if (funcao == 16){
		//Rajesh,03(Ex.2) - (1,3) f1=5, f2=4
                double x, y;
		double fit = 0, rest = 0;
                double v;

		if (nivel == 1){
			x = ind[0];;
			y = follower[0];

			fit = (x-3)*(x-3) + (y-2)*(y-2);

			rest = 0.0;
                        // x em [0,8]
                        v = x - 8;
			if (v > EPS){
				rest += v;
			}

		} else if (nivel == 2){
			y = ind[0];
			x = leader[0];

			fit = (y-5)*(y-5);

			rest = 0.0;
			v = -(2*x-y +1);
			if (v > EPS){
				rest += v;
			}
			v = -(-x+2*y-2);
			if (v > EPS){
				rest += v;
			}
			v = -(-x-2*y+14);
			if (v > EPS){
				rest += v;
			}
		}
		ind[d] = fit;
		ind[d + 1] = rest;

        } else if (funcao == 17){
		//Rajesh,03(Ex.3) - (3,5) f1=9, f2=0
                double x, y;
		double fit = 0, rest = 0;
                double v;

		if (nivel == 1){
			x = ind[0];;
			y = follower[0];

			fit = (x-3)*(x-3) + (y-2)*(y-2);

			rest = 0.0;
                        v = -(2*x-y +1);
			if (v > EPS){
				rest += v;
			}
			v = -(-x+2*y-2);
			if (v > EPS){
				rest += v;
			}
			v = -(-x-2*y+14);
			if (v > EPS){
				rest += v;
			}
                        // x em [0,8]
                        v = x - 8;
			if (v > EPS){
				rest += v;
			}

		} else if (nivel == 2){
			y = ind[0];
			x = leader[0];

			fit = (y-5)*(y-5);

			rest = 0.0;

		}
		ind[d] = fit;
		ind[d + 1] = rest;

        } else if (funcao == 18){
		//Rajesh,03(Ex.4) - (17.4545,10.90909) f1=85.0909, f2=?
                double x, y;
		double fit = 0, rest = 0;
                double v;

		if (nivel == 1){
			x = ind[0];;
			y = follower[0];

			fit = -2*x+11*y;

			rest = 0.0;

		} else if (nivel == 2){
			y = ind[0];
			x = leader[0];

			fit = -x-3*y;

			rest = 0.0;
                        v = x -2*y -4;
			if (v > EPS){
				rest += v;
			}
                        v = 2*x -y -24;
			if (v > EPS){
				rest += v;
			}
                        v = 3*x +4*y -96;
			if (v > EPS){
				rest += v;
			}
                        v = x +7*y -126;
			if (v > EPS){
				rest += v;
			}
                        v = -4*x +5*y -65;
			if (v > EPS){
				rest += v;
			}
                        v = -(x +4*y -8);
			if (v > EPS){
				rest += v;
			}
		}
		ind[d] = fit;
		ind[d + 1] = rest;

        } 
	///// Novos exemplos para a apresentação Calvete2004
	else if (funcao == 19){
		double x1, x2, y3, y4, y5, y6, y7, y8;
		double fit = 0, rest = 0;
                double v;

		if (nivel == 1){
			x1 = ind[0];
			x2 = ind[1];
			y3 = follower[0];
			y4 = follower[1];
			y5 = follower[2];
			y6 = follower[3];
			y7 = follower[4];
			y8 = follower[5];

			fit = (1 + x1 - x2 + 2*y4)/(8 - x1 - 2*y3 + y4 + 5*y5);

			rest = 0.0;

		} else if (nivel == 2){
			y3 = ind[0];
			y4 = ind[1];
			y5 = ind[2];
			y6 = ind[3];
			y7 = ind[4];
			y8 = ind[5];
			x1 = leader[0];
			x2 = leader[1];

			fit = (1 + x1 + x2 + 2*y3 - y4 + y5)/(6 + 2*x1 + y3 + y4 - 3*y5);

			rest = 0.0;
                        v = -y3 + y4 + y5 + y6 - 1;
			if (fabs(v) > 0.000000001){
				rest += fabs(v);
			}
			
			//rest += fabs(v - EPS);

                        v = 2*x1 - y3 + 2*y4 - 0.5*y5 + y7 - 1;
			if (fabs(v) > 0.000000001){
				rest += fabs(v);;
			}
			//rest += fabs(v - EPS);

                        v = 2*x2 + 2*y3 - y4 - 0.5*y5 + y8 - 1;
			if (fabs(v) > 0.000000001){
				rest += fabs(v);
			}
			//rest += fabs(v - EPS);
		}
		ind[d] = fit;
		ind[d + 1] = rest;
	}

        delete[] x;
        delete[] y;

}

double getUpper(int nivel, int funcao, int indice){
	if (funcao == 1001){
		if (nivel == 1){
			return 10;
		} else {                    
                    if ( indice < q ){
                        return 10;
                    } else {
                        return M_PI/2 - 0.000000000001;
                    }			
		}
	} else if (funcao == 1002 || funcao == 1007){
		if (nivel == 1){
                        if ( indice < p ){
                            return 10;
                        } else {
                            return 1;
                        }  
		} else {
                        if ( indice < q ){
                            return 10;
                        } else {
                            return M_E;
                        }
		}
	} else if (funcao == 1003){
		if (nivel == 1){
			return 10;			
		} else {
                        if (indice < q){
                            return 10;
                        } else {
                            return M_PI/2 - 0.000000000001;
                        }
		}
	} else if (funcao == 1004){
		if (nivel == 1){                    
                        if (indice < p){
                            return 10;
                        } else {
                            return 1;
                        } 
		} else {
                        if ( indice < q){
                            return 10;
                        } else {
                            return M_E;
                        }
		}
	} else if (funcao == 1005 || funcao == 1006 || funcao == 1008){
		if (nivel == 1){
			return 10;			
		} else {
			return 10;			
		}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

	} else if (funcao == 1){ // x < 15; y < 20
                switch(indice){
                    case 0: return 15;
                    case 1: return 20;
                }

        } else if (funcao == 2){ // x1 > 0; x2 < 15; y1,y2 < 20
                switch(indice){
                    case 0: return 100;
                    case 1: return 15;
                    case 2: return 20;
                    case 3: return 20;
                }

	} else if (funcao == 5){ // x1,x2 < 50; y1,y2 < 20
                switch(indice){
                    case 0: return 50;
                    case 1: return 50;
                    case 2: return 20;
                    case 3: return 20;
                    case 4: return 20;
                }

        } else if (funcao == 10){ // x1,x2 > 0; y1,y2 < 1.5
                switch(indice){
                    case 0: return 100;
                    case 1: return 100;
                    case 2: return 1.5;
                    case 3: return 1.5;
                }

        } else if (funcao == 15){ // x < 1; y1,y2 < 1
                switch(indice){
                    case 0: return 1;
                    case 1: return 1;
                    case 2: return 1;
                }

        } else if (funcao == 16 || funcao == 17){ // x < 8; y > 0
                switch(indice){
                    case 0: return 8;
                    case 1: return 100;
                }

	} else{
		return 100;
	}
}

double getLower(int nivel, int funcao, int indice){
	
	if (funcao == 1001){        
		if (nivel == 1){
			return -5;
		} else {
                        if ( indice < q){
                            return -5;
                        } else {
                             return -(M_PI/2) + 0.000000000001;
                        }
		}
	} else if (funcao == 1002 || funcao == 1007){
		if (nivel == 1){
			return -5;			
		} else {
                        if (indice < q) {
                            return -5;
                        } else {
                            return 0.000000000001;
                        }
		}
	} else if (funcao == 1003){
		if (nivel == 1){
			return -5;			
		} else {
                        if (indice < q){
                            return -5;
                        } else {
                            return -(M_PI/2) + 0.000000000001;
                        }
		}
	} else if (funcao == 1004){
		if (nivel == 1){
                        if (indice < p){
                            return -5;
                        } else {
                            return -1;
                        }
		} else {
                        if (indice < q){
                            return -5;
                        } else {
                            return 0.000000000001;
                        }
		}
	} else if (funcao == 1005 || funcao == 1006 || funcao == 1008){
		if (nivel == 1){
			return -5;			
		} else {
			return -5;			
		}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

        } else if (funcao == 1){ // x > 0; y > 0
                switch(indice){
                    case 0: return 0;
                    case 1: return 0;
                }

        } else if (funcao == 2){ // x1,x2 > 0; y1,y2 > 0
                switch(indice){
                    case 0: return 0;
                    case 1: return 0;
                    case 2: return 0;
                    case 3: return 0;
                }

	} else if (funcao == 5){ // x1,x2 < 50; y1,y2 < 20
                switch(indice){
                    case 0: return 0;
                    case 1: return 0;
                    case 2: return -10;
                    case 3: return -10;
                    case 4: return -10;
                }

        } else if (funcao == 10){ // x1,x2 > 0; y1,y2 > 0.5
                switch(indice){
                    case 0: return 0;
                    case 1: return 0;
                    case 2: return 0.5;
                    case 3: return 0.5;
                }

        } else if (funcao == 15){ // x > 0; y1,y2 > 0
                switch(indice){
                    case 0: return 0;
                    case 1: return 0;
                    case 2: return 0;
                }

        } else if (funcao == 16 || funcao == 17){ // x < 8; y > 0
                switch(indice){
                    case 0: return 0;
                    case 1: return 0;
                }
        } else {
               return 0;
        }
}
