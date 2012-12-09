/*
 * cdiv.h
 * column division
 */

#include <vector>

vector<vector<double> > cdiv_2(vector<vector<double> > X, vector<vector<double> > Y) 
{
	/* Initialize Z */
    vector<vector<double> > Z;
    Z.resize(X.size());
    for (int h = 0; h < X[0].size(); h++)
    {
        Z[h].resize(X[1].size());
    }

    /* Find Z */
    for(int i=0; i < X[0].size(); i++)
    {
    	for(int j=0; j < X[1].size(); j++)
    	{
    		Z[i][j] = (double) X[i][j] / (double) Y[0][j];	
    	}
    }

    return Z;
}
