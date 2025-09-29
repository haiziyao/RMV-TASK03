#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <ceres/ceres.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/videoio.hpp>
#include <ostream>
#include <string>
#include <vector>

using namespace cv;
using namespace std;


struct Obs {
  double t;
  double x;
  double y;
};


int findhsv(){
    int hmin = 62,smin=0,vmin=255;
    int hmax =105,smax=59,vmax=255;

    String filepath = "resources/video.mp4";
    VideoCapture cap;
    Mat img ;
    cap.open(filepath);
    if(!cap.isOpened()) {
        cout<<"视频打开失败"<<endl;
        return -1;
    }
    namedWindow("Trackbars", 400);
    createTrackbar("Hue Min", "Trackbars", &hmin, 179);
    createTrackbar("Hue Max", "Trackbars", &hmax, 179);
    createTrackbar("Sat Min", "Trackbars", &smin, 255);
    createTrackbar("Sat Max", "Trackbars", &smax, 255);
    createTrackbar("Val Min", "Trackbars", &vmin, 255);
    createTrackbar("Val Max", "Trackbars", &vmax, 255);
    while(1){
        cap.read(img);
        if(img.empty()){
            cout<< "img is null "<<endl;
            break;
        }
        Mat imgGauss,imgHSV,imgMask;
        GaussianBlur(img, imgGauss, Size(13,13), 7);
        cvtColor(imgGauss, imgHSV, COLOR_BGR2HSV);

        while(1){
            Scalar lower(hmin,smin,vmin);
            Scalar upper(hmax,smax,vmax);
            inRange(imgHSV, lower, upper, imgMask);
            imshow("img",img);
            imshow("imgHSV",imgHSV);
            imshow("imgMask",imgMask);
            char key = waitKey(1);
            if(key==27){
                
                return 0;
            }
        }
    }
     
   
     
    

    return 0 ;
}


int saveimg(){
     cout << "Ceres installed!" << endl;
    string filepath = "resources/video.mp4";
    VideoCapture cap ;
    cap.open(filepath);
    if(!cap.isOpened()){
        cout<<"请检查视频源路径，没有找到vedio"<<endl;
    }
    cv::Mat frame;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    Mat hsv,mask;
    Mat result;
    while (true) {
        cap.read(frame);
        if(frame.empty()){
            cout<<"视频源已经读完了"<<endl;
            break;
        }
        imshow("vedio",frame);
        char key = waitKey(0);
        if(key == 27){
            imwrite("resources/test.png", frame);
            break;
        }
    }  
    while(1){
        char key = waitKey(1);
        if(key == 27){
            break;
        }
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}

vector<Obs> testmain() {
    cout << "Ceres installed!" << endl;
    string filepath = "resources/video.mp4";
    VideoCapture cap ;
    cap.open(filepath);
    if(!cap.isOpened()){
        cout<<"请检查视频源路径，没有找到vedio"<<endl;
    }
    cv::Mat frame,imgGauss;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    vector<Obs> goal;
    Mat hsv,mask;
    Mat result;
    int count = 1;
    int H ;
    while (true) {
        cap.read(frame);
        if(frame.empty()){
            cout<<"视频源已经读完了"<<endl;
            break;
        }
        H = frame.rows;
        GaussianBlur(frame, imgGauss, Size(13,13), 7);
        cvtColor(imgGauss, hsv, COLOR_BGR2HSV);
        inRange(hsv, Scalar(0,0,0), Scalar(179,170,255), mask);
        bitwise_not(mask, mask);
        result = mask;
         
        cv::findContours(result,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
        cout << "找到 " << contours.size() << " 个轮廓" << endl;
        if(contours.size()==0) break;
        for (size_t i = 0; i < contours.size(); i++) {
            drawContours(frame, contours, (int)i, Scalar(0, 0, 255), 1);
        }

        for (size_t i = 0; i < contours.size(); i++) {            
            Moments mom = moments(contours[i]);
            if (mom.m00 != 0){
                double cx = static_cast<double>(mom.m10) / mom.m00;
                double cy = static_cast<double>(mom.m01) / mom.m00;
                cy = H -cy;
                Obs obs={static_cast<double>(count++)/60,cx,cy};
                goal.push_back(obs);
                cout<<"("<<cx<<","<<cy<<")"<<endl;
            } 
            
        }
        imshow("vedio",result);
        imshow("controus",frame);
        char key = waitKey(0);
        if(key == 27){
            break;
        }
    }  
    while(1){
        char key = waitKey(1);
        if(key == 27){
            break;
        }
    }
    cap.release();
    cv::destroyAllWindows();
    return goal;
}

