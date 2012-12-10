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
#include "pr_hmm.h"
#include <unistd.h> 
#define GetCurrentDir getcwd

using namespace std;
int main(void)
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
    get_xyz_data(filepath, train_gesture, training);
    get_xyz_data(testpath, test_gesture, testing);

    /****INITIALIZE****/
    double gestureRecThreshold = 0; // set below
    int N = 8, D = 3, M = 12, LR = 2;
    double TrainXClustered[10][60], TestXClustered[10][60], centroids[8][3];
    get_point_centroids(training, N, D, centroids);
    get_point_clusters(training, centroids, D, TrainXClustered);
//    get_point_clusters(testing, centroids, D, TestXClustered);

    /****TRAINING****/
    // Set priors:
    double P[12][12];
    //prior_transition_matrix(M, LR, P);

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

    for(int xx=0; xx < pP.size(); xx++) {
        for(int yy=0; yy < pP[0].size(); yy++) {
            if(xx == yy) {
                pP[xx][yy] = 0.5;
            }
            else {
                pP[xx][yy] = 0;
            }
        }
    }

    for(int xx= 0; xx < pP.size(); xx++) {
        for(int yy=0; yy < pP[0].size(); yy++) {
            if(xx == yy) {
                pP[xx][yy+1] = 0.5;
            }
        }
    }
    pP[11][11] = 1;
                
    for(int xx=0; xx < ATrainBinned.size(); xx++) { 
        for(int yy=0; yy < ATrainBinned[0].size(); yy++) {
            ATrainBinned[xx][yy] = TrainXClustered[xx][yy];
           // printf(" %f ", ATrainBinned[xx][yy]);
        }
        //printf("\n");
    }
   // return 0;

    vector<vector<double> > bins;
    bins.resize(8);
    for (int i=0; i<8; i++)
        bins[i].resize(1);
    for (double i=0; i<8; i++)
        bins[i][0] = i;
    
    // Train the model:
    int cyc = 50;
     dhmm_numeric(ATrainBinned, pP, bins, M, cyc, .00001); 
#if 0
    /****TESTING****/    
    double sumLik = 0, minLik = 1000000, lik=0;
    for (int j=0; j < ATrainBinned[0].size(); j++)
    {
        /* Find E Transpose (E = 8x12)*/
        vector<vector<double> > ET;
        vector<vector<double> > E;
        ET.resize(12);

        for (int go=0; go<12; go++)
        {
            ET[go].resize(8);
        }
        E.resize(8);
        for (int go=0; go < 8; go++)
        {
            E[go].resize(12);
        }

        // dhmm_numeric returns E
        for (int nava=0; nava<8; nava++)
        {
            for (int sar=0; sar<12; sar++)
            {
                ET[sar][nava] = E[nava][sar];
            }
        }
        
        // we pass ATrainBinned[j][:] here...
        // Pi is from dhmm_numeric
        lik = pr_hmm(j, ATrainBinned, P, ET, Pi);

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
        tLL[j][0] = pr_hmm(j, ATestBinned, P, ET, Pi);
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
#endif
    return 0;
}
