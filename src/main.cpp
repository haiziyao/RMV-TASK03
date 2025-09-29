#include <ceres/ceres.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <string>
#include <vector>
#include "findhsv.h"
using namespace cv;
using namespace std;
int main() {
    //saveimg();
    //findhsv();
    vector<Point> goal = testmain();
    
}


