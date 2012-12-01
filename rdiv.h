/*
 * rdiv.h
 * row division: Z = X / Y row-wise
 */

#include <vector>

vector<vector<double> > rdiv(vector<vector<double> > X, vector<vector<double> > Y)
{
	// [N M]=size(X);
 	int N = X[0].size();
 	int M = X[1].size();

 	// [K L]=size(Y);
 	int K = Y[0].size();
 	int L = Y[1].size();

 	if(N != K || L != 1)
 	{
 		cout << ("Error in RDIV");
 		return;
 	}

 	// Z=zeros(N,M);
 	vector<vector<double> > Z;
 	Z.resize(N);
 	for (int a = 0; a < N; a++)
    {
    	Z[a].resize(M);	
    }

    if (M < N)
    {
    	for (int mi = 0; mi < M; mi++)
    	{
    		for (int ni = 0; ni < N; ni++)	
    		{
    			//Z(:,m)=X(:,m)./Y;
    			Z[ni][mi] = X[ni][mi] / Y[ni];
    		}
    		
    	}
    }
    else
    {
    	for (int ni = 0; ni < N; ni++)	
    	{
    		for (int mi = 0; mi < M; mi++)
    		{
    			Z[ni][mi] = X[ni][mi] / Y[ni];
    		}
    	}
    }

    return Z;
} 