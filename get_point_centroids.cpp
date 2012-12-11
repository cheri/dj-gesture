/*
 * get_point_centroids.h
 * Create histogram to bin the data
 */

 #include <vector>
#include "get_point_centroids.h"
#include "kmeans.h"
#include "stdio.h"
#include "string.h"
 /* 
  * Notes: 
  *		- original parameter "int D" is assumed to be 3 
  * 	- in get_xyz_data.h (etc.), I'm pretty sure it's supposed to be [60][10] not [10][60],
  *		   easy solution for now is swapped c and d in data[d][c][e] below...				
  * 	- this should return something eventually...need to bridge with Nava's kmeans file
 */
void get_point_centroids(double data[60][10][3], int K, int D, double arr_centroid[8][3])
 {
    int idx[60];
    double pCluster[8][1];
 	/* Initialize mean vector (automatically filled with zeros)*/
 	double mean[60][3];
    memset(mean, 0, sizeof(mean));
    /* Set mean vector based on input params*/
   for (int c = 0; c < 60; c++)
    {
        for (int d = 0; d < 10; d++)
        {
        	for (int e = 0; e < 3; e++)
        	{
        		mean[c][e] = mean[c][e] + data[c][d][e];
        	}
        }
        for (int f = 0; f < 3; f++)
        {
        	mean[c][f] = mean[c][f] / 10;
        }
    }
#if 0
int i;
    for(i = 0; i < 60; i++) {
        //printf("%d %f %f %f\n", i,  mean[i][0],  mean[i][1], mean[i][2]);
    }
#endif

    /* Verify means are correct */
    /*cout << ("***Means***");
	
	for (int h = 0; h < 3; h++)
	{
		cout << ("\n");
		cout << ("Column ");
        cout << (h);
        cout << (": \n");
 		for (int g = 0; g < 60; g++)
    	{
        
        	cout << mean[g][h];
        	cout << (" ");
        }
    }*/

    //Combine with Nava's code
    /*% Using k-means to make data discrete
	[centroids,points,idx] = kmeans(mean,K);
	K = size(centroids,1);*/

    kmeans_1(mean, K, idx, arr_centroid, pCluster);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 3; j++) {
            //printf(" %f ", arr_centroid[i][j]);
        }
        //printf(" points in cluster: %f at %d\n", pCluster[i][0], i);
    }
    int c;
    //for(c = 0; c < 60; c++) {
    //    printf("%d:%d\n", c, idx[c]);
    //}
 }


