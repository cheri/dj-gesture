/*
 * rsum.h
 * row sum
 */

 #include <vector>

vector<double> rsum(vector<vector<double> > X)
{
    // [N M]=size(X);
    int N = X.size();
    int M = X[0].size();

    // Z=zeros(N,1);
    vector<double> Z;
    Z.resize(N);


    for (int i=0; i < N; i++)
    {
        double sum = 0;
        for (int j=0; j < M; j++)
        {
            sum += X[i][j];	
        }
        Z[i] = sum;
    }

    return Z;
}
