#include <iostream>
#include <sstream>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "viewport.h"

using namespace cv;
using namespace std;

void help()
{
    cout << "This program detects the squares of a chessboard from a camera and displays "
        << "their corners onscreen." << endl;
    cout << "It uses OpenCV." << endl;
}

const float clocks_per_millisec = CLOCKS_PER_SEC / 1000.0;

long millisecond_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

float matrixIntegral(Mat m, float factor)
{
    float sum = 0;
    for(int i = 0; i < m.rows; i++)
        for(int j = 0; j < m.cols; j++)
            sum += factor * m.at<float>(i,j);
    return sum;
}

int main(int argc, char **argv)
{
    if(argc > 1)
    {
        help();
        return 1;
    }

    int cameraID = 1;
    VideoCapture inputCapture(cameraID);

    namedWindow("control",1);
    namedWindow("display",1);
    namedWindow("diff",1);
    namedWindow("warpped",1);

    Viewport vp;

    int key = -1;
    bool inited = false;
    while(1)
    {
        Mat frame, gray;
                
        inputCapture >> frame;

        imshow("control", frame);

        vp.processCalibration(frame);
        
        key = waitKey(60);
        if(key > 0 && !inited)
        {
            vp.initCalibration();
        }
    }

    return 0;
}


