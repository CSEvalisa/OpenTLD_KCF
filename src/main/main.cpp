#include <opencv2/opencv.hpp>
#include <TLD.h>
#include <iostream>
using namespace std;
using namespace cv;

cv::Mat org,dst,img,tmp;
cv::Rect init_Rect;
int init_Finish;
static void on_mouse(int event,int x,int y,int flags,void *ustc);
void configureTracker(tld::TLD *tracker,Mat grey);

int main()
{
    VideoCapture capture(0);
    int seed;
    srand(seed);
    init_Finish = 0;
    namedWindow("img");
    while(1)
    {
        capture>>img;
        img.copyTo(org);
        org.copyTo(tmp);
        setMouseCallback("img",on_mouse,0);
        if(init_Finish == 1)
            break;
        imshow("img",img);
        waitKey(1);
    }

    cv::Rect init_bbox = init_Rect;
    Mat grey(org.rows,org.cols, CV_8UC1);
    cvtColor(org,grey,CV_BGR2GRAY);

    tld::TLD *trackerTLD = new tld::TLD();
    configureTracker(trackerTLD,grey);
    trackerTLD->selectObject(grey,org,&init_bbox);
    cv::Mat image;
    while (1)
    {
        capture>>image;
        image.copyTo(org);
        double tic = cvGetTickCount();
        trackerTLD->processImage(image);
        double toc = (cvGetTickCount() - tic) / cvGetTickFrequency();
        toc = toc / 1000000;
        float fps = 1 / toc;
        ostringstream getString;
        getString <<"fps: "<<fps;
        cv::putText(org,getString.str(),Point(20,30),FONT_HERSHEY_SIMPLEX,1,Scalar(0,0,255,255));
        if(trackerTLD->currBB != NULL)
            cv::rectangle(org,Rect(trackerTLD->currBB->x,trackerTLD->currBB->y,trackerTLD->currBB->width,trackerTLD->currBB->height),Scalar(0,255,0,255),4);
        cv::imshow("img", org);
        char key = cv::waitKey(20);
        if (key == 'q')
        {
            break;
        }
    }
    delete trackerTLD;
    return 0;
}


void configureTracker(tld::TLD *tracker , Mat grey)
{

    tracker->alternating = false;
    tracker->trackerEnabled = true;
    tracker->learningEnabled = true;
    tracker->detectorCascade->varianceFilter->enabled = true;
    tracker->detectorCascade->ensembleClassifier->enabled = true;
    tracker->detectorCascade->nnClassifier->enabled = true;

    // classifier
    tracker->detectorCascade->useShift = true;
    tracker->detectorCascade->shift = 0.1;
    tracker->detectorCascade->minScale = -10;
    tracker->detectorCascade->maxScale = 10;
    tracker->detectorCascade->minSize = 25;
    tracker->detectorCascade->numTrees = 10;
    tracker->detectorCascade->numFeatures = 13;
    tracker->detectorCascade->nnClassifier->thetaTP = 0.65;
    tracker->detectorCascade->nnClassifier->thetaFP = 0.5;

    tracker->detectorCascade->imgWidth = grey.cols;
    tracker->detectorCascade->imgHeight = grey.rows;
    tracker->detectorCascade->imgWidthStep = grey.step;

}


static void on_mouse(int event,int x,int y,int flags,void *ustc)
{
    static Point pre_pt(-1,-1);
    static Point cur_pt(-1,-1);
    char temp[16];

    if (event == CV_EVENT_LBUTTONDOWN)
    {
        org.copyTo(img);
        pre_pt = Point(x,y);
        //imshow("img",img);
    }
    else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))
    {
        img.copyTo(tmp);
        cur_pt = Point(x,y);
        rectangle(tmp,pre_pt,cur_pt,Scalar(0,255,0,0),2,8,0);
        imshow("img",tmp);
    }
    else if (event == CV_EVENT_LBUTTONUP)
    {
        org.copyTo(img);
        sprintf(temp,"(%d,%d)",x,y);
        cur_pt = Point(x,y);
        rectangle(img,pre_pt,cur_pt,Scalar(0,255,0,0),2,8,0);
        imshow("img",img);
        img.copyTo(tmp);
        int width = abs(pre_pt.x - cur_pt.x);
        int height = abs(pre_pt.y - cur_pt.y);
        if (width == 0 || height == 0)
        {
            printf("width == 0 || height == 0");
            return;
        }
        init_Rect = Rect(min(cur_pt.x,pre_pt.x),min(cur_pt.y,pre_pt.y),width,height);
        init_Finish = 1;
    }
    waitKey(10);
}
