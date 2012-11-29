/*
 * get_point_clusters.h
 * Create histogram to bin the data
 */

#include <vector>

 /* 
  * Notes: 
  * 	- we need the centroids param from get_point_centroids.h 
  * 	- we need K = size(centroids,1);
  *		- this should eventually return XClustered
  */

void get_point_clusters(vector<vector<vector<double> > > data)
{
	//XClustered = cell(size(data,2),1);
	//K = size(centroids,1);

	for (int c = 0; c < 60; c++)
    {
        for (int d = 0; d < 10; d++)
        {
        	for (int e = 0; e < K; e++)
        	{	
        		//temp(j) = sqrt( (centroids(j,1) - data(n,i,1))^2+(centroids(j,2) - data(n,i,2))^2+(centroids(j,3) - data(n,i,3))^2);
        	}
        }
        //[idx,I] = min(temp);
        //XClustered{i}(n,1) = I(1);
    }
}