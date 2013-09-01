#include "viewport.h"

#include <cstdio>

Viewport::Viewport()
{
    mSourcePoint[0] = Point2f(50,50);
    mSourcePoint[1] = Point2f(50,550);
    mSourcePoint[2] = Point2f(750,550);
    mSourcePoint[3] = Point2f(750,50);

    mImgInit = imread("patterns_calib/init.tiff", 1); //CV_LOAD_IMAGE_COLOR);
    mImgBlack = imread("patterns_calib/black.tiff", 1);//CV_LOAD_IMAGE_COLOR);
    mImgEnd = imread("patterns_calib/end.tiff", 1); //CV_LOAD_IMAGE_COLOR);
    for(int i = 0; i < 4; i++)
    {
        char path[256];
        sprintf(path, "patterns_calib/corner%d.tiff", i);
        mImgCorner[i] = imread(path, 1);
    }

    mCurrentState = CALIB_INIT;
    mCurrentCorner = 0;
    imshow("display", mImgInit);
}    

void Viewport::initCalibration()
{
    mCurrentCorner = 0;
    toState(CALIB_BLACK); 
}

void Viewport::toState(CalibState newState)
{
    switch(newState)
    {
        case CALIB_BLACK:
            imshow("display", mImgBlack);
            break;
        case CALIB_WHITE:
            imshow("display", mImgCorner[mCurrentCorner]);
            break;
        case CALIB_END:
            imshow("display", mImgBlack);
            break;
        default:
            break;
    }

    mCurrentState = newState;
}

void Viewport::processCalibration(Mat orig)
{
    Mat diff, abs, thres, colored, warpped, frame;
    Moments m;
    Vec3b green, black;
    green[0] = 0;
    green[1] = 127;
    green[2] = 0;
    black[0] = black[1] = black[2] = 0;
    int x, y;
    cvtColor(orig, frame, COLOR_RGB2GRAY);
    
    switch(mCurrentState)
    {
        case CALIB_BLACK:
            frame.copyTo(mLastBlack);
            toState(CALIB_WHITE);
            break;
        case CALIB_WHITE:
            diff = frame - mLastBlack;
            
            // TODO : store the centroid
            convertScaleAbs(diff, abs);
            threshold(abs, thres, 127, 255, 0);

            m = moments(thres);
            x = m.m10 / m.m00;
            y = m.m01 / m.m00;
            mVideoPoint[mCurrentCorner] = Point2f(x,y);
            cvtColor(thres, colored, COLOR_GRAY2RGB);
            circle(colored, Point( x, y ), 5,  CV_RGB(255,0,0), 2, 8, 0 );
            //normalize(diff, diff_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );

            imshow("diff", colored);
            
            if(mCurrentCorner < 3)
            {
                mCurrentCorner++;
                toState(CALIB_BLACK);
            }
            else
            {
                mTransform = getPerspectiveTransform(mVideoPoint, mSourcePoint); 
                mFirstGreenOutput = true;
                imshow("display", mImgBlack);
                toState(CALIB_END);
            }
            break;
        case CALIB_END:
            warpPerspective(orig, warpped, mTransform, Size(800,600));
            if(mFirstGreenOutput)
                warpped.copyTo(greenOutput); // TODO resize instead

            for(int i = 0; i < warpped.rows; i++)
                for(int j = 0; j < warpped.cols; j++)
                    if((warpped.at<Vec3b>(i,j))[0] > 200)
                        greenOutput.at<Vec3b>(i,j) = green;
                    else if(mFirstGreenOutput)
                        greenOutput.at<Vec3b>(i,j) = black;
            mFirstGreenOutput = false;
            imshow("display", greenOutput);
            break;
        default:
            break;
    }
}


