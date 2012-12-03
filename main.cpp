/* 
 * Main function...just calls functions to test for now
 */

#include <vector>
#include <iostream>
#include "stdlib.h"
#include "get_xyz_data.h"
#include "get_point_centroids.h"
#include "get_point_clusters.h"

#include <unistd.h> 
#define GetCurrentDir getcwd

using namespace std;
int main()
{

	/* Initialize XYZ "data" vector*/
    double data[60][10][3];
    
    
    /* Get current directory path */
    char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
    	return 0;
    }
    string filepath = cCurrentPath;
    string testpath = cCurrentPath;
	filepath += "/data/train/";
	testpath += "/data/test/";

    /* Grab XYZ data */
	get_xyz_data(filepath, "circle", data);
//	testing = get_xyz_data(filepath, "circle");

	/* Print data */
#if 0
	for (int e = 0; e < 3; e++)
    {
    	// x, y, or z
    	cout << ("***Group ");
    	cout << e;
    	cout << (":***\n");

		for (int c = 0; c < 10; c++)
	    {
	    	cout << ("Column ");
	        cout << c;
	        cout << (":\n");
	        for (int d = 0; d < 60; d++)
	        {
	        	
	        	cout << data[c][d][e];
	        	cout << (" ");
	        }
	        cout << ("\n\n");
	    }
    }
#endif
    /* Call get_point_centroids */
    // N = # of states
    int N = 8, D = 3;
    double XClustered[10][60];
    double centroids[8][3];
    get_point_centroids(data,N, D, centroids);
    get_point_clusters(data, centroids, D, XClustered);
} 
