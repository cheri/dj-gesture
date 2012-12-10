/*
 * get_point_clusters.h
 * Create histogram to bin the data
 */

#include <vector>
#include <cv.h>
#define square(x) x*x
 /* 
  * Notes: 
  * 	- we need the centroids param from get_point_centroids.h 
  * 	- we need K = size(centroids,1);
  *		- this should eventually return XClustered
  */
double min(double temp[8]) {
    double min_val = 100000;
    int idx = -1;
    for(int i= 0; i < 8; i++) {
       if(min_val > temp[i]) {
            min_val = temp[i];
            idx = i; 
        }
    }
    return idx;
}

void get_point_clusters(double data[60][10][3], double centroids[8][3], int D, double XClustered[10][60])
{
	int K = 8;
	for (int c = 0; c < 60; c++)
    {
        for (int d = 0; d < 10; d++)
        {
            double temp[8];
        	for (int j = 0; j < K; j++)
            {	
                temp[j] = sqrt( pow(centroids[j][0] - data[c][d][0], 2) + pow(centroids[j][1] - data[c][d][1], 2)+ pow(centroids[j][2] - data[c][d][2], 2));
            }
            XClustered[d][c] = min(temp);
        }
    }
}
