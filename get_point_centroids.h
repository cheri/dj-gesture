/*
 * get_point_centroids.h
 * Create histogram to bin the data
 */
#include <vector>
using namespace std;
 /* 
  * Notes: 
  *		- original parameter "int D" is assumed to be 3 
  * 	- in get_xyz_data.h (etc.), I'm pretty sure it's supposed to be [60][10] not [10][60],
  *		   easy solution for now is swapped c and d in data[d][c][e] below...				
  * 	- this should return something eventually...need to bridge with Nava's kmeans file
 */
void get_point_centroids(vector<vector<vector<double> > > data, int K);
