/* 
 * Main function...just calls functions to test for now
 */

#include <vector>
#include "get_xyz_data.h"

#include <unistd.h> 
#define GetCurrentDir getcwd

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

    /* Get current directory path */
    char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
    	return 0;
    }
    string filepath = cCurrentPath;
	filepath += "/data/train/";

    /* Grab XYZ data */
	data = get_xyz_data(filepath, "circle");

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