#ifndef INCLUDED_VIEWPORTH
#define INCLUDED_VIEWPORTH

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#define SCREEN_CHANGE_DELAY 1000 // in milliseconds

using namespace cv;
using namespace std;

enum CalibState
{
    CALIB_INIT,
    CALIB_BLACK,
    CALIB_WHITE,
    CALIB_END,
    CALIB_RUNNING
};        
typedef enum CalibState CalibState;

class Viewport
{
    public:
        Viewport();

        void initCalibration();
    
        void processCalibration(Mat frame);

    private:
        void toState(CalibState newState);
        bool requestScreenUpdate();
        void getTime(struct timeval* tv);

        CalibState mCurrentState;
        int mCurrentCorner;
        Mat mLastBlack;
        Point2f mVideoPoint[4];
        Point2f mSourcePoint[4];

        Mat mImgInit, mImgBlack, mImgCorner[4], mImgEnd;

        Mat mTransform;
        bool mFirstGreenOutput;
        Mat greenOutput;

        struct timeval* mLastScreenChange;
        struct timezone* mTimezone;
};
        


#endif

