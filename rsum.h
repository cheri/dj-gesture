/*
 * rsum.h
 * row sum
 */

 #include <vector>

vector<double> rsum(vector<vector<double> > X)
 {
 	// [N M]=size(X);
 	int N = X[0].size();
 	int M = X[1].size();

 	// Z=zeros(N,1);
 	vector<double> Z;
 	Z.resize(N);

 	if (M==1)
 	{
 		Z = X;
 	}
 	else if (M<2*N)
 	{
  		for (int mi=0; mi<M, mi++)
  		{
  			for (int x0i=0; x0i<X[0].size(); x0i++)
  			{
  				//add X[x0i][mi] to every element of Z
  				//Z = Z + X[x0i][mi];
  				for (int zi=0; zi<N; zi++)
  				{
  					Z[zi] = Z[zi] + X[x0i][mi];	
  				}
  				
  			}
  		}
 	}
 	else
 	{
 	
    	for (int ni=0; ni<N, ni++)
  		{
  			int sum = 0;
  			for (int x1i=0; x1i<X[1].size(); x1i++)
  			{
  				sum += X[n1][x1i];	
  			}
  			Z[ni] = sum;
  		}
 	}

 	return Z;
 }
