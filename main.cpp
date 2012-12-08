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
        bins[i][1] = (double)(i+1);
    }

    // Train the model:
    int cyc = 50;
    dhmm_numeric(ATrainBinned, pP, bins, M, cyc, .00001);

    /****TESTING****/    
#if 0

    double sumLik = 0;
    double minLik = 1000000;
    for j=1:length(ATrainBinned)
        lik = pr_hmm(ATrainBinned{j},P,E',Pi);
    if (lik < minLik)
        minLik = lik;
    end
        sumLik = sumLik + lik;
    end
        gestureRecThreshold = 2.0*sumLik/length(ATrainBinned);

    fprintf('\n\n********************************************************************\n');
    fprintf('Testing %i sequences for a log likelihood greater than %f\n',length(ATestBinned),gestureRecThreshold);
    fprintf('********************************************************************\n\n');

    recs = 0;
    tLL = zeros(length(ATestBinned),1);
    for j=1:length(ATestBinned)
        tLL(j,1) = pr_hmm(ATestBinned{j},P,E',Pi);
    if (tLL(j,1) > gestureRecThreshold)
        recs = recs + 1;
    fprintf('Log likelihood: %f > %f (threshold) -- FOUND %s GESTURE!\n',tLL(j,1),gestureRecThreshold,test_gesture);
    else
        fprintf('Log likelihood: %f < %f (threshold) -- NO %s GESTURE.\n',tLL(j,1),gestureRecThreshold,test_gesture);
    end
        end
        fprintf('Recognition success rate: %f percent\n',100*recs/length(ATestBinned));

#endif
    return 0;
} 
