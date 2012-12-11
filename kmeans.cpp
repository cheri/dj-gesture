#include <iostream>
#include <cv.h>
#include <time.h>
#include "kmeans.h"
#include <stdlib.h>
using namespace std;
using namespace cv;
int min(Mat *data, Mat *min) {
    double min_val0, min_val1, min_val2;

    min_val0 = min_val1 = min_val2 = 10000;

    for(int i = 0; i < data->rows; i++) {
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
    double max_val0, max_val1, max_val2; 
    max_val0 = max_val1 = max_val2 = -10000;
    
    for(int i = 0; i < data->rows; i++) {
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
    for(int i = 0; i < centroid->rows; i++) {
        for(int j = 0; j < centroid->cols; j++) {
            centroid->at<double>(i,j) = (double)rand()/RAND_MAX;
           // centroid->at<double>(i,j) = 1;
        }
    }
    return 0;
}


int kmeans_1(double data[60][3], double k, int idx[60], double arr_centroid[8][3], double pCluster[8][1]) {
    Mat cvdata(60, 3, CV_32F);
    for(int i=0; i < 60; i++)
        for(int j=0; j < 3; j++)
            cvdata.at<float>(i,j) = data[i][j];
    Mat centers;
    Mat labels;
    int m = 8;
    cv::kmeans(cvdata, 8, labels, TermCriteria(CV_TERMCRIT_ITER, 100,0), 100, KMEANS_RANDOM_CENTERS, centers); 
    printf("labels  rows: %d cols: %d\n", labels.rows, labels.cols);
    printf("centers rows: %d cols: %d\n", centers.rows, centers.cols);
    for(int i =0; i < 8; i++)
        for(int j=0; j < 3; j++)
            arr_centroid[i][j] = centers.at<float>(i,j);
    return 0;
#if 0
    Mat cvdata;
    Mat data_min, data_max, data_diff;
    Mat centroid(8, 3, CV_64FC1);
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
    printf("%s %d\n", __FILE__, __LINE__);

    fill_rand_matrix(&centroid);
    for(int i = 0; i < centroid.rows; i++) {
        centroid.row(i) = centroid.row(i).mul(data_diff);
        centroid.row(i) = centroid.row(i) + data_min;
    }
    int xin = 0;
    while(pos_diff > 0.0) {
        xin++;
        for(int d = 0; d < cvdata.rows; d++) {
            Mat min_diff = cvdata.row(d) - centroid.row(1);
            Mat mat_diff = min_diff * min_diff.t();
            double min_diff_t = mat_diff.at<double>(0,0);
            int curAssignment = 1;

            for(int c=1; c < k; c++) {
                Mat diff2c = cvdata.row(d) - centroid.row(c);
                Mat mat_diff2c = diff2c * diff2c.t();
                if(min_diff_t >= mat_diff2c.at<double>(0,0)) {
                    curAssignment = c;
                    min_diff_t = mat_diff2c.at<double>(0,0);
                }
            }
            idx[d] = curAssignment;    
        }
        Mat oldPositions = centroid.clone();

        centroid.setTo(0);
        pointsInCluster.setTo(0); 

        for(int d = 0; d < cvdata.rows; d++) {
            centroid.row(idx[d]) += cvdata.row(d);
            pointsInCluster.at<double>(idx[d],0) += 1;
        }
        for(int c = 0; c < k; c++) {
            if(pointsInCluster.at<double>(c,0) != 0) {
                centroid.row(c) = centroid.row(c) / pointsInCluster.at<double>(c,0);
            } else {
                Mat randx(1, 3, CV_64FC1);
                fill_rand_matrix(&randx);
                centroid.row(c) = randx.mul(data_diff) + data_min;
            }
        }
        Mat posdiffMat = centroid - oldPositions;
        pos_diff = sum(sum(posdiffMat.mul(posdiffMat)))[0];
        printf("diff: %f %f\n", s, pos_diff);
        if(xin > 14)
            exit(0);
    }
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 3; j++) {
            arr_centroid[i][j] = centroid.at<double>(i,j);         
            printf( " %f ", arr_centroid[i][j]);
        }
        pCluster[i][0] = pointsInCluster.at<double>(i,0);
        printf("\n pcluster %f pos: %d\n ", pCluster[i][0], i);
    }

    printf("%s %d\n", __FILE__, __LINE__);
    exit(0);
    return 0;
#endif
}

#if 0
    for(int i=0; i < oldPositions.rows; i++) {
        for(int j=0; j < oldPositions.cols; j++)
            printf(" %f ", oldPositions.at<double>(i,j));
        printf("\n");
    }
#endif
