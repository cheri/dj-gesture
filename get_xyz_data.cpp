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
    x.resize(60);
    for (int h = 0; h < 60; h++)
    {
        x[h].resize(10);
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
            j++;
        }
        j=0;
        i++;
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
void get_xyz_data(string path, string name, double data[60][10][3])
{
    /* Specify file name & path */
    string x_file = path + "/" + name + "_x.csv";
    string y_file = path + "/" + name + "_y.csv";
    string z_file = path + "/" + name + "_z.csv";

    /* Read and store CSV data */
    vector<vector<double> > x = getData(x_file);
    vector<vector<double> > y = getData(y_file);
    vector<vector<double> > z = getData(z_file);

    
    /* Store XYZ data in "data" vector */
    for (int d = 0; d < 60; d++)
    {
        for (int c = 0; c < 10; c++)
        {
            data[d][c][0] = x[d][c];  
            data[d][c][1] = y[d][c];  
            data[d][c][2] = z[d][c];  
        }
    }
}
