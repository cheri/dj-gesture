/*
 * get_point_centroids.h
 * Create histogram to bin the data
 */

 #include <vector>

 /* 
  * Notes: 
  *		- original parameter "int D" is assumed to be 3 
  * 	- in get_xyz_data.h (etc.), I'm pretty sure it's supposed to be [60][10] not [10][60],
  *		   easy solution for now is swapped c and d in data[d][c][e] below...				
  * 	- this should return something eventually...need to bridge with Nava's kmeans file
 */
 void get_point_centroids(vector<vector<vector<double> > > data, int K)
 {
 	/* Initialize mean vector (automatically filled with zeros)*/
 	vector<vector<double> > mean;
    mean.resize(60);
    for (int a = 0; a < 60; a++)
    {
        mean[a].resize(3);
    }

    /* Set mean vector based on input params*/
   for (int c = 0; c < 60; c++)
    {
        for (int d = 0; d < 10; d++)
        {
        	for (int e = 0; e < 3; e++)
        	{	
        		mean[c][e] = mean[c][e] + data[d][c][e];
        	}
        }
        for (int f = 0; f < 3; f++)
        {
        	mean[c][f] = mean[c][f] / 10;
        }
    }

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

 }


