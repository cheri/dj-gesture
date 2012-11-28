/* 
 * Main function...just calls functions to test for now
 * Path etc. will have to change once we have more files and get this set up properly
 */

#include <vector>
#include "get_xyz_data.h"
int main()
{

	/* Initialize XYZ "data" vector*/
    vector<vector<vector<double> > > data;
    
    data.resize(10);
    for (int a = 0; a < 10; a++)
    {
        data[a].resize(60);
        for (int b = 0; b < 60; b++)
        {
            data[a][b].resize(3);
        }
    }

    /* Grab XYZ data */
	data = get_xyz_data("/Users/sarahharmon/Desktop/data/train/", "circle");

	/* Print data */
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
} 