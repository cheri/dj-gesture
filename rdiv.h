/*
 * rdiv.h
 * row division: Z = X / Y row-wise
 */

#include <vector>

vector<vector<double> > rdiv(vector<vector<double> > X, vector<double> Y)
{
    // [N M]=size(X);
    int N = X.size();
    int M = X[0].size();

    // Z=zeros(N,M);
    vector<vector<double> > Z;
    Z.resize(N);

    for (int a = 0; a < N; a++)
        Z[a].resize(M);	

    for (int ni = 0; ni < N; ni++)
        for (int mi = 0; mi < M; mi++)	
            Z[ni][mi] = X[ni][mi] / Y[ni];

    return Z;
}
