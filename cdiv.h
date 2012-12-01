/*
 * cdiv.h
 * column division
 */

#include <vector>

vector<vector<double> > cdiv(vector<vector<double> > X, vector<vector<double> > Y) 
{
	if (X[1].size() != Y[1].size() || Y[1].size() != 1)
	{
		cout << ("Error in CDIV");
		return;
	}

	/* Initialize Z */
    vector<vector<double> > Z;
    Z.resize(X[0].size());
    for (int h = 0; h < X[0].size(); h++)
    {
        Z[h].resize(X[1].size());
    }

    /* Find Z */
    for(int i=0; i < X[0].size(); i++)
    {
    	for(int j=0; j < X[1].size(); j++)
    	{
    		Z[i][j] = X[i][j] / Y[i];	
    	}
    }

    return Z;
}