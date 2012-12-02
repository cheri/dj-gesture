#include <iostream>
#include <cv.h>
using namespace std;
using namespace cv;

int min(Mat *data, Mat *min) {
    int i = 0;
    double min_val0, min_val1, min_val2;

    min_val0 = min_val1 = min_val2 = 10000;

    for(i = 0; i < data->rows; i++) {
        if(min_val0 < data->at<double>( i, 0))
            min_val0 = data->at<double>(i, 0);
        if(min_val1 < data->at<double>( i, 1))
            min_val1 = data->at<double>(i, 1);
        if(min_val2 < data->at<double>( i, 2))
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
    max_val0 = max_val1 = max_val1 = -10000;

    for(i = 0; i < data->rows; i++) {
        if(max_val0 > data->at<double>( i, 0))
            max_val0 = data->at<double>(i, 0);
        if(max_val1 > data->at<double>( i, 1))
            max_val1 = data->at<double>(i, 1);
        if(max_val2 > data->at<double>( i, 2))
            max_val2 = data->at<double>(i, 2);
    }

    max->at<double>(0,0) = max_val0;
    max->at<double>(0,1) = max_val1;
    max->at<double>(0,2) = max_val2;

    return 0;
} 
 

int kmeans(double data[60][3], double k) {
    int idx[60];
    Mat cvdata;
    Mat data_min, data_max, data_diff;
    Mat centroid(8, 3, CV_64FC1);
    Mat oldPositions(8, 3, CV_64FC1);
    Mat pointsInCluster(8, 1, CV_64FC1);
    double data_dim = 3, nbData = 60;
    double rnum = 0, pos_diff = 1.0;
    memset(idx, 0, sizeof(idx));

    cvdata = Mat(60, 3, CV_64FC1, data); 
    data_min = Mat(1, 3, CV_64FC1);
    data_max = Mat(1, 3, CV_64FC1);
    data_diff = Mat(1, 3, CV_64FC1);

    min(&cvdata, &data_min);
    max(&cvdata, &data_max);
    RNG rng(0);
    rng.fill(centroid, RNG::UNIFORM, 0, 1);

    int i;
    for(i = 0; i < centroid.rows; i++) {
        centroid.row(i) = centroid.row(i).mul(data_diff);
        centroid.row(i) = centroid.row(i) + data_diff;
    }

    while(pos_diff > 0.0) {
        int d,c;
        double min1_diff, diff1_2c;
        Mat min_diff(1, 3, CV_64FC1);
        Mat mat_diff(3, 3, CV_64FC1);
        for(d = 0; d < cvdata.rows; d++) {
            int curAssignment = 1;
            min_diff = cvdata.row(d) - centroid.row(1);
            mat_diff = min_diff * min_diff.t();
            curAssignment = 1;
            
            for(c=1; c < 8; c++) {
                Mat diff2c(1, 3, CV_64FC1);
                Mat mat_diff2c(3, 3, CV_64FC1);
                diff2c = cvdata.row(d) - centroid.row(c);
                mat_diff2c = diff2c * diff2c.t();
                min1_diff = sum(sum(mat_diff))[0];
                diff1_2c  = sum(sum(mat_diff2c))[0];
                if(min1_diff >= diff1_2c) {
                    curAssignment = c;
                    min1_diff = diff1_2c;
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
        for(c = 0; c < cvdata.rows; c++) {
            if(pointsInCluster.at<double>(c,1) != 0) {
                centroid.row(c) = centroid.row(c) / pointsInCluster.at<double>(c,1);
            } else {
                Mat randx(1, 3, CV_64FC1);
                rng.fill(randx, RNG::UNIFORM, 0, 1);
                centroid.row(c) = randx.mul(data_diff) + data_min;
            }
        }
        Mat posdiffMat = centroid - oldPositions;
        pos_diff = sum(sum(posdiffMat.mul(posdiffMat)))[0];
    }
    return 0;
}


int main(void) {
    return 0;
}
