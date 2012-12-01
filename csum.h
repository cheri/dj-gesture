/*
 * csum.h
 * column sum
 */

#include <vector>

/*
 * @param X: e.g., 8 by 12 matrix
 * @return Z: e.g., 1 by 12 matrix
 */
vector<double> rsum(vector<vector<double> > X)
{
 	//N=length(X(:,1));
 	int N = X[0].size();
 	vector<double> Z;
 	Z.resize(N);

	if (N > 1)
	{
		for (int i = 0; i < N; i++)
		{
			int sum = 0;
			for (int j = 0; j < X[1].size(); j++)
			{
				sum = sum + X[i][j];
			}
			Z[i] = sum;
		}
	}
	else
	{
		Z = X;
	}

	return Z;
}