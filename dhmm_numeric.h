/*
 * dhmm_numeric.h
 * Adapted from Zoubin Ghahramani http://mlg.eng.cam.ac.uk/zoubin/software.html
 */

#include <cfloat>
#include <cmath>
#include <ctime>
#include <vector>
#include "csum.h"
#include "cdiv.h"
#include "rdiv.h"
#include "rsum.h"

/* 
 * Iterates until a proportional change < tol in the log likelihood 
 * or cyc steps of Baum-Welch
 *
 * @param X: 	N x p array of Xs, p sequences
 * @param pP: 	12x12 "alphabet" 
 * @param bins: 8x1 possible bins of Xs
 * @param K: 	number of states (default 2)
 * @param cyc: 	maximum number of cycles of Baum-Welch (default 100)
 * @param tol: 	termination tolerance (prop change in likelihood) (default 0.0001)
 * @return E: 	8x12 observation emission probabilities
 * @return P:  	state transition probabilities
 * @return Pi:  initial state prior probabilities
 * @return LL: 	1x50 log likelihood curve
 * 
 * Notes:
 * 		- nargin is always assumed to be 6
 * 		- dhmm_numeric should eventually return E, P, Pi, LL as above
 */

/*
 * Checks if given double x is finite.
 * Useful for division by zero checking.
 */
bool IsFiniteNumber(double x) 
{
    return (x <= DBL_MAX && x >= -DBL_MAX); 
}  

vector<double> dot_mult_1_1(vector<double> mat1, vector<double> mat2) {
    vector<double> prod;

    for(int i=0; i < mat1.size(); i++) {
            prod[i] = mat1[i] * mat2[i];        
        }
    }

    return prod;
}

vector<vector<double> > dot_mult_2_2(vector<vector<double> > mat1, vector<vector<double> > mat2) {
    vector<vector<double> > prod;
    for(int i=0; i < mat1.size(); i++) {
        for(int j=0; j < mat1[0].size(); j++) {
            prod[i][j] = mat1[i][j] * mat2[i][j];
        }
    }
    return prod;
}

vector<vector<double> > mult_2_2(vector<vector<double> > mat1, vector<vector<double> > mat2) {
    int m, c, n;
    int i, j, k;

    m = mat1.size();
    c = mat1[0].size();
    n = mat2[0].size();
    vector<vector<double> > mat3;
    mat3.resize(m);
    for(i=0; i < m; i++) {
        mat3[i].resize(n);
    }
    for(i=0; i<m; i++) {
        for(j=0; j<n; j++) {
            mat3[i][j] = 0;
            for(k=0; k<c; k++)
                mat3[i][j] += mat1[i][k] * mat2[k][j];
        }
    }
    return mat3;
}

vector<double> mult_1_2(vector<double> mat1, vector<vector<double> >mat2) {
    int c, n;
    int i, j, k;
    c = mat1.size();
    n = mat2[0].size();
    vector<double> mat3;
    mat3.resize(c);
    for(j=0; j<n; j++) {
        mat3[j] = 0;
        for(k=0; k<c; k++)
            mat3[j] += mat1[k] * mat2[k][j];
    }
    return mat3;
}

vector<double> mult_2_1(vector<vector<double> >mat1, vector<double> mat2) {
    int m, c, n;
    int i, j, k;
    m = mat1.size();
    c = mat1[0].size();
    vector<double> mat3;
    mat3.resize(c);
    for(i=0; i<m; i++) {
            mat3[i] = 0;
            for(k=0; k<c; k++)
                mat3[i] += mat1[i][k] * mat2[k];
    }
    return mat3;
}


/* 
 * Returns an m-by-n matrix of random entries. 
 * @param m, n
 */
vector<vector<double> > randomMatrix(int m, int n)
{
	// initialize matrix
	vector<vector<double> > randMatrix;
	randMatrix.resize(m);
	for (int rm=0; rm<m; rm++)
	{
		randMatrix[rm].resize(n);
	}

	// populate matrix with random values in (0,1)
	for (int i=0; i<m;i++)
	{
		for (int j=0; j<n; j++)
		{
  			srand ( time(NULL) );  //random seed
			randMatrix[i][j] = ((double) rand() / (RAND_MAX));
		}
	}
	return randMatrix;
}

void dhmm_numeric(vector<vector<double> > X, vector<vector<double> > pP, vector<vector<double> > bins, int K, int cyc, double tol)
{
	int D = X[1].size();
	int num_bins = bins[0].size();

	float epsi = pow(1.0,-10);

	// number of sequences (e.g., 10)
	int N = X[0].size();

	// length of each sequence
	vector<double> T;
    T.resize(N);

    // initialize with ones
    for (int i=0; i<N; i++)
    {
    	T[i] = 1;	
    }

    // populate T
    for (int j=0; j<N; j++)
    {
    	//Rightahere!
        //looks fine too me...
    	// T(n)=size(X{n},1);...but I think this should equal 60 every time the way we set it up
		T[j] = 60;
    }

    double TMAX = *max_element(T.begin(), T.end());

	cout << ("\n********************************************************************\n");
	cout << ("Training!");
	cout << ("********************************************************************\n");

	vector<vector<double> > E = randomMatrix(num_bins,K);

	for (int nb=0; nb<num_bins; nb++)
	{
		for (int nk=0; nk<K; nk++)
		{
			E[nb][nk] = (E[nb][nk]*0.1 + 1) / num_bins;
		}
	}

	E = cdiv(E,csum(E));

	vector<vector<double> > B;	
	B.resize(TMAX);
	for (int bi=0;bi<TMAX;bi++)
	{
		B[bi].resize(K);
	}

	// initialize Pi with random values
	vector<vector<double> > Pi = randomMatrix(1, K);

	// find sum of Pi
	int sumPi = 0;
	for (int pii=0; pii<K; pii++)
	{
		sumPi = sumPi + Pi[1][pii];
	}	

	// normalize Pi
	for (int pip=0; pip<K; pip++)
	{
		Pi[1][pip] = Pi[1][pip] / sumPi;
	}

	// transition matrix
	vector<vector<double> > P = pP;
	P=rdiv(P,rsum(P));

	/* This is useful if the transition matrix is 
	 * initialized with many zeros, i.e. for a 
	 * left-to-right HMM.  However, my guess is 
	 * that this is an optimization procedure 
	 * and not really necessary.  I hope.
	 */
	//P=sparse(P);  
	
	vector<double> LL;
	LL.resize(50);
	int Lidx = 0; // index for LL
	double lik = 0, likbase=0, oldlik=0;

	for (int cycle=0; cycle<cyc; cycle++)
	{
		vector<double> Scale;
		Scale.resize(TMAX);	
		
		vector<vector<double> > Gammaksum;
		Gammaksum.resize(num_bins);

		for (int act=0; act<K; act++)
		{
			Gammaksum[act].resize(K);
		}

		//1xK vectors
		vector<vector<double> > Gammainit, Gammasum;
		Gammainit[0].resize(K);
		Gammasum[0].resize(K);

		//originally a sparse, but we don't need no stinkin' optimization
		vector<vector<double> > sxi;
		sxi.resize(K);

		for (int ii=0;ii<K;ii++)
		{
			sxi[ii].resize(K);
		}

		for (int n=0; n<N; n++)
		{
			vector<vector<double> > alpha, beta, gamma, gammaksum;

			alpha.resize(T[n]);
			beta.resize(T[n]);
			gamma.resize(T[n]);
			gammaksum.resize(num_bins);

			for (int eger=0; eger<T[n]; eger++)
			{
				alpha[eger].resize(K);
				beta[eger].resize(K);
				gamma[eger].resize(K);
				gammaksum[eger].resize(K);				
			}
  			
  			/* Inital values of B = Prob(output|s_i), given data X*/
			vector<double> Xcurrent=X[n];

			for (int g=0; g<T[n]; g++)
			{
				/* 
				 * find the letter in the alphabet 
				 * i.e., this finds linear indices for when Xcurrent[g] is found in bins 
				 * m = findstr(alphabet, Xcurrent(i));
				 */

				int mIndex = 0;
				bool nuthin = true;
				for (int ra=0; ra<bins[0].size(); ra++)
				{
					if (bins[ra][0] == Xcurrent[g]) {
                        mIndex = ra;
                        nuthin = false;
                        break;
                    }
				}

				/* 
				 * we don't perform this check when we find the  
				 * letter in the alphabet later on, so it may 
				 * not be a huge deal
				 */
				if (nuthin)
				{
					cout << ("Error: Symbol not found\n");
					return;
				}

				for (int e=0; e<K; e++)
				{
					//Rightahere!
					// m is an array...quickfix for now
                    //made some changes here
					B[g][e]	= E[mIndex][e];
				}
			}

			vector<double> scale;
			scale.resize(T[n]);	

			// find transpose of Pi (1xK becomes Kx1)
			vector<vector<double> > PiT;
			PiT.resize(K);
			for (int pik=0;pik<K; pik++)
			{
				PiT[pik][0] = Pi[0][pik];	
			}

			
			for (int ake=0; ake<K; ake++)	
			{
				/* 
				 *  alpha(1,:)=Pi(:)'.*B(1,:);
				 * B = 1 x K and PiT is Kx1: automatic broadcasting operation - will generate KxK??
				 */
				alpha[0][ake] = PiT[ake][0] * B[0][ake];
				
				//scale(1)=sum(alpha(1,:));
				int alphasum=0;
				for (int achu=0; achu<K; achu++)
				{
					alphasum=alphasum+alpha[0][achu];		
				}
				scale[0]=alphasum;
				alpha[0][ake]= alpha[0][ake] / scale[0];
			}
			
			/*alpha(ellect,:)=(alpha(ellect-1,:)*P).*B(ellect,:);*/
			int asum=0;	
			for (int ellect=1; ellect<T[n]; ellect++)
			{
                alpha[ellect] = mult_1_2(alpha[ellect], P);
				for (int egrate=0; egrate<K; egrate++)
				{
					//Rightahere!
					// P is 12x12 (sparse) - two for loops (below) to account for this...
					// alpha is T(n) by K
					// B is 1 x K
					// ...perhaps they mean B[0][egrate] instead of B[ellect][egrate]?
					for (int ready; ready <P[0].size(); ready++)
					{
						for (int set; set<P[1].size(); set++)
						{
							alpha[ellect][egrate]=alpha[ellect-1][egrate]*B[ellect][egrate];
						}
					}
					asum = asum+alpha[ellect][egrate];
				}
				//scale(i)=sum(alpha(i,:));
				scale[ellect] = asum;

				//alpha(i,:)=alpha(i,:)/scale(i);
				for (int an=0; an<K; an++)
				{
					alpha[ellect][an] = alpha[ellect][an] / scale[ellect];		
				}
				
			}

			for (int s=0;s<K; s++)	
			{
				beta[T[n]][s]= 1/(scale[T[n]]);
			}

			/* find transpose of P(12x12) */
			vector<vector<double> > PT;
			PT.resize(P[1].size());

			for(int pc=0; pc<P[1].size();pc++)
			{
				PT[pc].resize(P[0].size());
			}

			for (int px=0;px<P[0].size(); px++)
			{
				for(int py=0; py<P[1].size();py++)
				{

					PT[py][px] = P[px][py];
				}
			}

			for (int v=T[n]-2; v>0; v-=1)
			{
				for(int w=0; w<K; w++)
				{
					// Rightahere!
					// again, PT is technically 12x12...potential issues here
					beta[v][w]=beta[v+1][w]*B[v+1][w]/scale[w]; //*(P')
				}	
                beta[v] = mult_1_2(beta[v], PT);
			}

		   	for (int seg=0; seg<T[n]; seg++)
		   	{
		   		for(int fault=0; fault<K; fault++)
		   		{
		   			gamma[seg][fault] = alpha[seg][fault]*beta[seg][fault]+epsi;	
		   		}
		   	}

		    gamma=rdiv(gamma,rsum(gamma));
		    
		    vector<vector<double> > gammasum;
		    gammasum.resize(K);
		    int gsIdx = 0;

		    for (int row=0; row<T[n]; row++) 
		    {
		    	int colSum = 0;
		    	for(int col=0; col < K; col++)
		    	{
		    		colSum = colSum + gamma[row][col];
		    	}	
		    	gammasum[gsIdx][0] = colSum;
		    }

		    for (int egral=0; egral<T[n]; egral++)
		    {
		    	/* find the letter in the alphabet
			 	 * m = findstr(alphabet, Xcurrent(i));
			 	 */
			 	 
				int mIndex2 = 0;
			
				for (int ra=0; ra<bins[0].size(); ra++)
				{
					if (bins[ra][0] == Xcurrent[egral])
					{
						mIndex2++;
                        break;
					}
				}

				// gammaksum = T[n] x K	
			 	 for(int gk=0; gk<K; gk++)
			 	 {
			 	 	//Rightahere!
			 	 	//m is an array again...quickfix for now...
			 	 	gammaksum[mIndex2][gk] = gammaksum[mIndex2][gk] + gamma[egral][gk];
			 	 }
		    }

		    // Grab transpose of alpha (T[n] x K)
			vector<vector<double> > alphaT;

			alphaT.resize(K);

			for (int go=0; go<K; go++)
			{
				alphaT[go].resize(T[n]);
			}

			for (int nava=0; nava<T[n]; nava++)
			{
				for (int sar=0; sar<K; sar++)
				{
					alphaT[sar][nava] = alpha[nava][sar];
				}
			}

			//Rightahere!
		    for (int el=0; el<T[n]-1; el++)	//but this can't be the first var considering the matrix dims...
            {
                vector<vector<double> > t;
                t.resize(K);

                for (int go=0; go<K; go++)
                    t[go].resize(K);
                
                t = dot_mult_2_2(P, (mult_1_2(alphaT[el], (dot_mult_1_1(beta[el+1], B[el+1])))));
                
                // find sum of t <--can sum all values; only diag. will matter
                int tsum=0;
                for (int le=0; le<t[0].size(); le++)	
                {
                    for (int uk=0; uk<t[1].size(); uk++)
                    {
                        tsum = tsum + t[le][uk];
                    }
                }

                /* sxi=sxi+t/sum(t(:));	*/
                for (int ka=0; ka<K; ka++)
                {
                    for (int ko=0; ko<K; ko++)
                    {
                        // Rightahere!
                        // t is 12 by 12...
                        sxi[ka][ko] = sxi[ka][ko] + t[ka][ko]/tsum;
                    }
                }
            }

		    // Gammasum/Gammainit = 1xK, gammasum = Kx1
			for (int u=0; u<K; u++)				      
			{
				Gammainit[0][u]= Gammainit[0][u] + gamma[0][u];	
				Gammasum[0][u] = Gammasum[0][u] + gammasum[u][0];
			}

			for(int a=0; a<num_bins; a++)
			{
				for (int z=0; z<K; z++)
				{
					Gammaksum[a][z] = Gammaksum[a][z] + gammaksum[a][z];
				}
			}

			for (int y=0; y<T[n]-1; y++)	
			{
				/* Scale and scale are single vectors...
				 * ...so it is unclear why they would use Scale(i,:).
				 */
				Scale[y] = Scale[y] + log(scale[y]);
			}
			Scale[T[n]] = Scale[T[n]] + log(scale[T[n]]);
			
		} //end loop over N

		/* M STEP */
		// outputs
		E = cdiv(Gammaksum, Gammasum);

		// transition matrix (orig. sparse)
		P = rdiv(sxi, rsum(sxi));	

		//P = P*eye(size(P,1));  // Rightahere!  ...why?  isn't this just P?

		// priors 
		/* find sum */
		int sum = 0;
		for (int r = 0; r < K; r++)
		{				
			sum = sum + Gammainit[0][r];
		}

		for (int q = 0; q < K; q++)
		{
			Pi[0][q] = Gammainit[0][q] / sum;	
		}

		oldlik=lik;

		/* lik = sum(Scale); */
		for (int ch=0; ch<TMAX; ch++)
		{
			lik = lik + Scale[ch];
		}
		
		LL[Lidx] = lik;
		Lidx = Lidx + 1;

		float smallNum = pow(1.0,-6);

		if (cycle<=2)
		{
			likbase=lik;
		}
		else if (lik < (oldlik - smallNum))
		{
		}
		else if ( (lik-likbase) < (1+tol)*(oldlik-likbase) || !IsFiniteNumber(lik))
		{
			return;
		}

	} //end loop over cyc
}
