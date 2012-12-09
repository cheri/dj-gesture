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
    //N=length(X(:,1));
    int N = X.size();

    vector<double> Z;
    Z.resize(N);

    for (int i = 0; i < N; i++)
    {
        double sum = 0;
        for (int j = 0; j < X[0].size(); j++)
        {
            sum = sum + X[i][j];
        }
        Z[i] = sum;
    }

    return Z;
}
