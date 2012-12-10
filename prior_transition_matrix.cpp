/*
 * prior_transition_matrix.h
 * Create a prior for the transition matrix
 */

#include <vector>
#include "prior_transition_matrix.h" 
 /*
  * @param LR is the allowable number of left-to-right transitions
  * @return P...a diag matrix with 12 columns and 12 rows
  */

void prior_transition_matrix(int K, int LR, double P[12][12])
{
	/* eye(K) returns K*K identity matrix*/
	//P = ((1/LR))*eye(K);
    double x = (double)1/LR;
    
    for(int i = 0; i < 12; i++) {
        for(int j=0; j < 12;j++) {
            if(i==j)
                P[i][j] = x;
            else
                P[i][j] = 0;
        }
    }

	for (int i = 0; i < (K-(LR-1)); i++)
    {
        for (int j = 0; j < (LR-1); j++)
        {
        	P[i][i+j] = (1/LR);
        }
    }

    //test; might just be LR-2
    for (int i = (K-(LR-3)); i < K; i++)
    {
        for (int j = 0; j < (K-i+1); j++)
        {
        	P[i][i+j-1] = (1/(K-i+1));
        }
    }

    //return P;
}
