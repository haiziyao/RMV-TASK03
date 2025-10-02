#include <ceres/ceres.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include "findhsv.h"
#include "fit.h"
#include <chrono>
using namespace cv;
using namespace std;
int main() {
    auto start_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();

    
    cal(testmain());

    auto end_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();

    uint64_t cost_ms = end_ms - start_ms;
    std::cout << "程序耗时：" << cost_ms << " ms" << std::endl; 
}


