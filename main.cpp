/* 
 * Main function
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

    /* Initialize XYZ "data" vectors */
    double training[60][10][3], testing[60][10][3]; 

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

    string train_gesture = "circle";
    string test_gesture = "circle";

    /* Grab XYZ data */
    get_xyz_data(filepath, train_gesture, training);    // train
    get_xyz_data(testpath, test_gesture, testing);      // test

    /****INITIALIZE****/
    double gestureRecThreshold = 0; // set below
    int N = 8, D = 3, M = 12, LR = 2;
    double TrainXClustered[10][60], TestXClustered[10][60], centroids[8][3];
    get_point_centroids(training, N, D, centroids);
    get_point_clusters(training, centroids, D, TrainXClustered);
    get_point_clusters(testing, centroids, D, TestXClustered);

    /****TRAINING****/
    // Set priors:
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
        bins[i].resize(1);
    }
    for (double i=0; i<8; i++)
    {
        bins[i][0] = (i+1.0);
    }
    
    // Train the model:
    int cyc = 50;
    // dhmm_numeric(ATrainBinned, pP, bins, M, cyc, .00001); 

    /****TESTING****/    
    double sumLik = 0, minLik = 1000000, lik=0;
    for (int j=0; j < ATrainBinned[0].size(); j++)
    {
        //lik = pr_hmm(ATrainBinned{j},P,E',Pi);
        if (lik < minLik)
        {
            minLik = lik;
        }
        sumLik = sumLik + lik;
    }
    gestureRecThreshold = 2.0*sumLik/ATrainBinned[0].size();

    cout << ("Testing!\n");
    int recs=0;
    double tLL[10][1]; 

    for (int j=0; j<10; j++)
    {
        //tLL[j][0] = pr_hmm(ATestBinned{j},P,E',Pi);
        if (tLL[j][0] > gestureRecThreshold)
        {
            recs = recs+1;
            cout << ("Found gesture!\n");
        }
        else
        {
            cout << ("No gesture (╯°□°）╯︵ ┻━┻ \n");   
        }
    }

    cout << ("Recognition success rate: ");
    cout << (100*recs/10);
    cout << ("\n");

    return 0;
} 
