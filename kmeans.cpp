#include <iostream>
#include <cv.h>
#include <time.h>
#include "kmeans.h"
#include <stdlib.h>
using namespace std;
using namespace cv;
int min(Mat *data, Mat *min) {
    int i = 0;
    double min_val0, min_val1, min_val2;

    min_val0 = min_val1 = min_val2 = 10000;

    for(i = 0; i < data->rows; i++) {
        if(min_val0 > data->at<double>( i, 0))
            min_val0 = data->at<double>(i, 0);
        if(min_val1 > data->at<double>( i, 1))
            min_val1 = data->at<double>(i, 1);
        if(min_val2 > data->at<double>( i, 2))
            min_val2 = data->at<double>(i, 2);
    }

    min->at<double>(0,0) = min_val0;
    min->at<double>(0,1) = min_val1;
    min->at<double>(0,2) = min_val2;

    return 0;
}

int max(Mat *data, Mat *max) {
    int i = 0;
    double max_val0, max_val1, max_val2; 
    max_val0 = max_val1 = max_val2 = -10000;
    
    for(i = 0; i < data->rows; i++) {
        if(max_val0 < data->at<double>( i, 0))
            max_val0 = data->at<double>(i, 0);
        if(max_val1 < data->at<double>( i, 1))
            max_val1 = data->at<double>(i, 1);
        if(max_val2 < data->at<double>( i, 2))
            max_val2 = data->at<double>(i, 2);
    }

    max->at<double>(0,0) = max_val0;
    max->at<double>(0,1) = max_val1;
    max->at<double>(0,2) = max_val2;

    return 0;
}
 
int fill_rand_matrix(Mat *centroid) {
    int i, j;
    for(i = 0; i < centroid->rows; i++) {
        for(j = 0; j < centroid->cols; j++) {
            centroid->at<double>(i,j) = (double)rand()/RAND_MAX;
        }
    }
}


int kmeans(double data[60][3], double k, int idx[60], double arr_centroid[8][3], double pCluster[8][1]) {
    Mat cvdata;
    Mat data_min, data_max, data_diff;
    Mat centroid(8, 3, CV_64FC1);
    Mat oldPositions(8, 3, CV_64FC1);
    Mat pointsInCluster(8, 1, CV_64FC1);
    double data_dim = 3, nbData = 60;
    double rnum = 0, pos_diff = 1.0;
    memset(idx, 0, sizeof(idx));

    srand(time(NULL));
    cvdata = Mat(60, 3, CV_64FC1, data); 
    data_min = Mat(1, 3, CV_64FC1);
    data_max = Mat(1, 3, CV_64FC1);
    data_diff = Mat(1, 3, CV_64FC1);

    min(&cvdata, &data_min);
    max(&cvdata, &data_max);
    data_diff = data_max - data_min;

    fill_rand_matrix(&centroid);
    int i;
    for(i = 0; i < 8; i++) {
        printf("%f %f %f\n",  centroid.at<double>(i,0),  centroid.at<double>(i,1), centroid.at<double>(i,2));
    }
    for(i = 0; i < centroid.rows; i++) {
        centroid.row(i) = centroid.row(i).mul(data_diff);
        centroid.row(i) = centroid.row(i) + data_min;
    }
    for(i = 0; i < 8; i++) {
        printf("%f %f %f\n",  centroid.at<double>(i,0),  centroid.at<double>(i,1), centroid.at<double>(i,2));
    }
    while(pos_diff > 0.0) {
        int d,c;
        for(d = 0; d < cvdata.rows; d++) {
            Mat min_diff = cvdata.row(d) - centroid.row(1);
            Mat mat_diff = min_diff * min_diff.t();
            double min_diff_t = mat_diff.at<double>(0,0);
            int curAssignment = 1;
            
            for(c=1; c < k; c++) {
                Mat diff2c = cvdata.row(d) - centroid.row(c);
                Mat mat_diff2c = diff2c * diff2c.t();
                if(min_diff_t >= mat_diff2c.at<double>(0,0)) {
                    curAssignment = c;
                    min_diff_t = mat_diff2c.at<double>(0,0);
                }
            }
            idx[d] = curAssignment;    
        }
        oldPositions = centroid;
        
        centroid.setTo(0);
        pointsInCluster.setTo(0); 

        for(d = 0; d < cvdata.rows; d++) {
            centroid.row(idx[d]) += cvdata.row(d);
            pointsInCluster.at<double>(idx[d],1) = pointsInCluster.at<double>(idx[d],1) + 1;
        }
        for(c = 0; c < 8; c++) {
            if(pointsInCluster.at<double>(c,1) != 0) {
                centroid.row(c) = centroid.row(c) / pointsInCluster.at<double>(c,1);
            } else {
                Mat randx(1, 3, CV_64FC1);
                fill_rand_matrix(&randx);
                centroid.row(c) = randx.mul(data_diff) + data_min;
            }
        }
        Mat posdiffMat = centroid - oldPositions;
        pos_diff = sum(sum(posdiffMat.mul(posdiffMat)))[0];
    }
    int j;
    for(i = 0; i < 8; i++) {
        for(j = 0; j < 3; j++) {
            arr_centroid[i][j] = centroid.at<double>(i,j);         
        }
        pCluster[i][0] = pointsInCluster.at<double>(i,0);
    }

    return 0;
}


