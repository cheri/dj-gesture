/*
 * get_xyz_data.h
 * Get 10 sequences of XYZ data from CSVs (setup is not really generic)
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "get_xyz_data.h"
/*headers eventually...?*/

using namespace std;

/* Reads CSV file given filename 
 * @param filename: the name of the file to read 
 * @return x: the 2D array of size [10][60]
 */
vector<vector<double> > getData(string filename)
{
    /* Initialize array to return: double x[10][60]; */
    vector<vector<double> > x;
    x.resize(10);
    for (int h = 0; h < 10; h++)
    {
        x[h].resize(60);
    }

    /* Init vars to traverse array x */
    int i = 0, j = 0;

    /* Stores data from file into array*/
    ifstream csvfile(filename.c_str());

    string line;
    while(getline(csvfile,line))
    {
        stringstream lineStream(line);
        string cell;
        while(getline(lineStream,cell,','))
        {
            x[i][j] = atof(cell.c_str());
            i++;
        }
        i=0;
        j++;
    } 

    /* Prints out data from x's csv */
    /*for(int s=0; s<10; s++)
    {
        cout << ("Column ");
        cout << s;
        cout << (":\n");
        for (int h=0; h<60; h++)
        {
            cout << x[s][h];
            cout << (" ");
        }
        cout << ("\n\n");
    }*/

    /* Return array */
    return x;
}

/* Reads CSV file given filename 
 * @param path: filepath for csv file
 * @param name: name of csv file
 * @return data: merged XYZ data
 */
vector<vector<vector<double> > > get_xyz_data(string path, string name)
{
    /* Specify file name & path */
    string x_file = path + "/" + name + "_x.csv";
    string y_file = path + "/" + name + "_y.csv";
    string z_file = path + "/" + name + "_z.csv";

    /* Read and store CSV data */
    vector<vector<double> > x = getData(x_file);
    vector<vector<double> > y = getData(y_file);
    vector<vector<double> > z = getData(z_file);

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

    /* Store XYZ data in "data" vector */
   for (int c = 0; c < 10; c++)
    {
        for (int d = 0; d < 60; d++)
        {
            data[c][d][0] = x[c][d];  
            data[c][d][1] = y[c][d];  
            data[c][d][2] = z[c][d];  
        }
    }

    return data;
}
