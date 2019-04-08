#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>
#include <opencv2/opencv.hpp>
#include <utils/cl_wrapper.h>
#include <FileUtils.h>
#include "common/com_logger.h"
#include "ion/ion.h"
#include "TestModules/FisheyeCamera/fisheye.h"
#include "FileUtils.h"
#include "Remap/remaper.h"


AAssetManager *assetManager;
extern "C"
JNIEXPORT void JNICALL
Java_com_willhua_openclrepo_MainActivity_setNativeAM(JNIEnv *env, jobject instance,
                                                     jobject am) {
    assetManager = AAssetManager_fromJava(env, am);
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_willhua_openclrepo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_willhua_openclrepo_MainActivity_clTest(JNIEnv *env, jobject instance) {

    using namespace cv;
    using namespace std;

    // TODO
    cl_wrapper clWrapper;
    LOGD("%p %p", &clWrapper, clWrapper.get_context());
    cv::Mat img(2,2,CV_8UC1);
    img.data[0] = 9;
    int ionf = ion_open();
    LOGE("%d ionf %d", img.data[0], ionf);
    ion_close(ionf);

    int w, h;
    getSize(&w, &h);
    float *remapdata = (float*)malloc(sizeof(float) * w* h * 2);
    getFloat2Remap(remapdata, remapdata + w * h);

    Mat map1(h, w, CV_32FC1, remapdata);
    Mat map2(h, w, CV_32FC1, remapdata + w * h);

    writeFile(remapdata, sizeof(float)*w*h, "/sdcard/remapx.bin");
    writeFile(remapdata + w * h, sizeof(float)*w*h, "/sdcard/remapy.bin");

    AAsset *file = AAssetManager_open(assetManager, "fisheye_src.jpg", AASSET_MODE_BUFFER);
    size_t length = AAsset_getLength(file);
    const void *filedata = AAsset_getBuffer(file);
    std::vector<char> imgdata;
    imgdata.resize(length);
    memcpy(imgdata.data(), filedata, length);

    cv::Mat srcimg = cv::imdecode(imgdata, 1);
    imwrite("/sdcard/src.jpg", srcimg);

    Mat undistimg;
    cv::remap(srcimg, undistimg, map1, map2, INTER_LINEAR);
    imwrite("/sdcard/undist.jpg", undistimg);


    /***************/
    Mat yuv;
    cvtColor(srcimg, yuv, COLOR_RGB2YUV_YV12);
    IMAGE_DATA myimg;
    myimg.width = w;
    myimg.height = h;
    myimg.format = FORMAT_YUV_YV12;
    myimg.pplane[0] = yuv.data;
    myimg.pplane[1] = myimg.pplane[0] + w * h;
    myimg.pplane[2] = myimg.pplane[1] + w * h / 4;
    myimg.pstride[0] = w;
    myimg.pstride[1] = w / 2;
    myimg.pstride[2] = w / 2;

    pImageDATA nv12 = changYV12ToNV12(&myimg);

    float *tmpt = remapdata + w * h;
    remaptest(nv12, remapdata, tmpt);

    Mat matnv12(nv12->height * 3 / 2, nv12->width, CV_8UC1, nv12->pplane[0]);
    Mat matbgr;
    cvtColor(matnv12, matbgr, COLOR_YUV2BGR_NV12);
    imwrite("/sdcard/undist2.jpg", undistimg);


    free(nv12);
    free(remapdata);

}