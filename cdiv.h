/*
 * cdiv.h
 * column division
 */

#include <vector>

vector<vector<double> > cdiv(vector<vector<double> > X, vector<double> Y) 
{
	/* Initialize Z */
    vector<vector<double> > Z;
    Z.resize(X.size());
    for (int h = 0; h < X.size(); h++)
    {
        Z[h].resize(X[0].size());
    }

    /* Find Z */
    for(int j=0; j < X[0].size(); j++)
    {
    	for(int i=0; i < X.size(); i++)
    	{
    		Z[i][j] = (double) X[i][j] / (double) Y[i];	
    	}
    }

    return Z;
}
