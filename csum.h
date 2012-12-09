/*
 * csum.h
 * column sum
 */

#include <vector>

/*
 * @param X: e.g., 8 by 12 matrix
 * @return Z: e.g., 1 by 12 matrix
 */
vector<double> csum(vector<vector<double> > X)
{
    int N = X.size();
    int M = X[0].size();

    vector<double> Z;
    Z.resize(M);

    for (int j = 0; j < M; j++)
    {
        double sum = 0;
        for (int i = 0; i < N; i++)
        {
            sum = sum + X[i][j];
        }
        Z[j] = sum;
    }

    return Z;
}
