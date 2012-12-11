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
int get_training_attr(const char * training_type, vector<vector<double> >& E, 
                vector<vector<double> >& ET, vector<vector<double> >& P, 
                vector<vector<double> >& Pi, double *gthresh,
                double centroids[8][3])
{

    /* Initialize XYZ "data" vectors */
    double training[60][10][3]; 

    /* Get current directory path */
    char cCurrentPath[FILENAME_MAX];
    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
        return 0;
    }
    string filepath = cCurrentPath;
    filepath += "/data/train/";


    /* Grab XYZ data */
    get_xyz_data(filepath, training_type, training);

    /****INITIALIZE****/
    double gestureRecThreshold = 0; // set below
    int N = 8, D = 3, M = 12, LR = 2;
    double TrainXClustered[10][60];
    get_point_centroids(training, N, D, centroids);
    get_point_clusters(training, centroids, D, TrainXClustered);

    /****TRAINING****/
    // Set priors:

    vector<vector<double> > ATrainBinned;
    ATrainBinned.resize(10);
    for (int i=0; i<10; i++)
        ATrainBinned[i].resize(60);

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
        }
    }

    vector<vector<double> > bins;
    bins.resize(8);
    for (int i=0; i<8; i++)
        bins[i].resize(1);
    for (double i=0; i<8; i++)
        bins[i][0] = i;
    /* Find E Transpose (E = 8x12)*/


    // Train the model:
    int cyc = 50;
    dhmm_numeric(ATrainBinned, pP, bins, M, cyc, .001, E, P, Pi); 
    /****TESTING****/    
    double sumLik = 0, minLik = 1000000, lik=0;
    for (int j=0; j < ATrainBinned.size(); j++)
    {
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
        lik = pr_hmm(ATrainBinned[j], P, ET, Pi);

        if (lik < minLik)
        {
            minLik = lik;
        }
        sumLik = sumLik + lik;
    }
    gestureRecThreshold = 2.0*sumLik/ATrainBinned.size();
    *gthresh = gestureRecThreshold;

    return 0;
}

int get_testing_attr(const char * training_type, vector<vector<double> >& ATestBinned, double centroids[8][3])
{

    /* Initialize XYZ "data" vectors */
    double testing[60][10][3];

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
    get_xyz_data(testpath, training_type, testing);

    /****INITIALIZE****/
    int D = 3;
    double TestXClustered[10][60];
    get_point_clusters(testing, centroids, D, TestXClustered);

    ATestBinned.resize(10);
    for (int i=0; i<10; i++)
        ATestBinned[i].resize(60);

    for(int xx=0; xx < ATestBinned.size(); xx++)
        for(int yy=0; yy < ATestBinned[0].size(); yy++)
            ATestBinned[xx][yy] = TestXClustered[xx][yy];
    return 0;
}


int main(void) {
    double gthresh_c, gthresh_x, gthresh_l, gthresh_z;
    double centroids_c[8][3], centroids_x[8][3], centroids_l[8][3], centroids_z[8][3];

    vector<vector<double> > ATestBinned_c, ATestBinned_x, ATestBinned_l, ATestBinned_z;

    vector<vector<double> > E_c, ET_c, P_c, Pi_c;
    vector<vector<double> > E_x, ET_x, P_x, Pi_x;
    vector<vector<double> > E_l, ET_l, P_l, Pi_l;
    vector<vector<double> > E_z, ET_z, P_z, Pi_z;

    ET_c.resize(12);
    P_c.resize(12);
    Pi_c.resize(12);

    ET_x.resize(12);
    P_x.resize(12);
    Pi_x.resize(12);

    ET_l.resize(12);
    P_l.resize(12);
    Pi_l.resize(12);

    ET_z.resize(12);
    P_z.resize(12);
    Pi_z.resize(12);

    for (int go=0; go<12; go++)
    {
        ET_c[go].resize(8);
        P_c[go].resize(12);
        Pi_c[go].resize(12);

        ET_x[go].resize(8);
        P_x[go].resize(12);
        Pi_x[go].resize(12);

        ET_l[go].resize(8);
        P_l[go].resize(12);
        Pi_l[go].resize(12);

        ET_z[go].resize(8);
        P_z[go].resize(12);
        Pi_z[go].resize(12);

    }

    E_c.resize(8);

    E_x.resize(8);

    E_l.resize(8);

    E_z.resize(8);

    for (int go=0; go < 8; go++)
    {
        E_c[go].resize(12);
        E_x[go].resize(12);
        E_l[go].resize(12);
        E_z[go].resize(12);
    }
    get_training_attr("o", E_c, ET_c, P_c, Pi_c, &gthresh_c, centroids_c); 
    get_training_attr("x", E_x, ET_x, P_x, Pi_x, &gthresh_x, centroids_x); 
    get_training_attr("l", E_l, ET_l, P_l, Pi_l, &gthresh_l, centroids_l); 
    get_training_attr("z", E_z, ET_z, P_z, Pi_z, &gthresh_z, centroids_z); 

    get_testing_attr("o", ATestBinned_c, centroids_c); 
    get_testing_attr("x", ATestBinned_x, centroids_x); 
    get_testing_attr("l", ATestBinned_l, centroids_l); 
    get_testing_attr("z", ATestBinned_z, centroids_z); 

    int j =2;
    if(pr_hmm(ATestBinned_x[j], P_x, ET_x, Pi_x) > gthresh_x)
        system("canberra-gtk-play --file=orangex.wav");
    else if(pr_hmm(ATestBinned_x[j], P_l, ET_l, Pi_l) > gthresh_l)
        system("canberra-gtk-play --file=megalead.wav");
    else if(pr_hmm(ATestBinned_x[j], P_c, ET_c, Pi_c) > gthresh_c)
        system("canberra-gtk-play --file=orchhit.wav");
    else if(pr_hmm(ATestBinned_x[j], P_z, ET_z, Pi_z) > gthresh_z)
        system("canberra-gtk-play --file=zekick.wav");
    
    return 0;
}
