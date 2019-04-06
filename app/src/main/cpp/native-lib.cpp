#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>
#include <opencv2/opencv.hpp>
#include <utils/cl_wrapper.h>
#include "common/com_logger.h"
#include "ion/ion.h"

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

    // TODO
    cl_wrapper clWrapper;
    LOGD("%p %p", &clWrapper, clWrapper.get_context());
    cv::Mat img(2,2,CV_8UC1);
    img.data[0] = 9;
    int ionf = ion_open();
    LOGE("%d ionf %d", img.data[0], ionf);
    ion_close(ionf);
}