//
// Created by lisan on 2019/4/7.
//

#ifndef OPENCLREPO_QCOM_LOGGER_H
#define OPENCLREPO_QCOM_LOGGER_H

#include <android/log.h>


#define LOGTAG_QCOM "LOGTAG_QCOM"

#define LOGE(...)     __android_log_print(ANDROID_LOG_ERROR, LOGTAG_QCOM, __VA_ARGS__)
#define LOGD(...)     __android_log_print(ANDROID_LOG_DEBUG, LOGTAG_QCOM, __VA_ARGS__)



#endif //OPENCLREPO_QCOM_LOGGER_H
