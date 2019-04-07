//
// Created by lisan on 2019/4/7.
//

#include <vector>
#include <opencv2/opencv.hpp>
#include "fisheye.h"
#include "com_logger.h"
//param of assets/fisheye_src.jpg

const float CAMERA_MK[4]{1.4257343180276255e+03, 1.4249228776570621e+03, 1.6438600094951873e+03, 1.2205608588655102e+03};
const int SENSOR_W = 3264.0;
const int SENSOR_H = 2448.0;
const float CAMERA_MD[4]{2.1116465821801322e-01, 3.0469587980068197e-01,
                         -2.8249263547856812e-02, -1.3334716903612459e-01};
const float INV_MK[9]{  7.4965769095467036e-04, 2.0870891820345960e-18,
                        -1.2311400780811705e+00, -1.3010426069826053e-18,
                        7.2065474590198735e-04, -8.8348555116836414e-01,
                        1.1926223897340549e-18, -1.6263032587282567e-18,
                        1.0000000000000002e+00   };

void getSize(int *w, int *h){
    *w = SENSOR_W;
    *h = SENSOR_H;
}


void getFloat2Remap(float *mapx, float *mapy){
    using namespace cv;
    Mat mk(3, 3, CV_32FC1, (void*)CAMERA_MK);
    Mat md(4, 1, CV_32FC1, (void*)CAMERA_MD);
    const Size imgsize(SENSOR_W, SENSOR_H);

    Mat map1, map2;
    cv::fisheye::initUndistortRectifyMap(mk, md, cv::noArray(), mk.clone(), imgsize, CV_32FC1, map1, map2);
    if(map1.isContinuous()){
        memcpy(mapx, map1.data, SENSOR_W * SENSOR_H * sizeof(float));
    }else{
        LOGD("map1 not continous!");
    }
    if(map2.isContinuous()){
        memcpy(mapy, map2.data, SENSOR_W * SENSOR_H * sizeof(float));
    }else{
        LOGD("map2 not continous!");
    }
}