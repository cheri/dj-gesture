/* 
 * Main function...just calls functions to test for now
 */

#include <vector>
#include <iostream>
#include "stdlib.h"
#include "get_xyz_data.h"
#include "get_point_centroids.h"
#include "get_point_clusters.h"
#include "prior_transition_matrix.h"
#include "dhmm_numeric.h"

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
    int M = 12, LR = 2;
    double XClustered[10][60];
    double centroids[8][3];
    get_point_centroids(data,N, D, centroids);
    get_point_clusters(data, centroids, D, XClustered);
    double P[12][12];
    prior_transition_matrix(M, LR, P);

    vector<vector<double> > ATrainBinned;

    ATrainBinned.resize(10);
    for (int i=0; i<10; i++)
    {
        ATrainBinned[i].resize(60);
    }

    vector<vector<double> > pP;
    pP.resize(12);
    for (int i=0; i<12; i++)
    {
        pP[i].resize(12);
    }    

    vector<vector<double> > bins;
    bins.resize(8);
    for (int i=0; i<8; i++)
    {
        bins[i][1] = (double)(i+1);
    }

    dhmm_numeric(ATrainBinned, pP, bins, M,50,.00001);

    return 0;
} 
