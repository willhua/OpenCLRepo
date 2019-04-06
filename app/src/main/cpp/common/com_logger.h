//
// Created by lisan on 2019/4/7.
//

#ifndef OPENCLREPO_COM_LOGGER_H
#define OPENCLREPO_COM_LOGGER_H

#include <android/log.h>

#define LOGTAG_COM "LOGTAG_COMMON"

#define LOGE(...)     __android_log_print(ANDROID_LOG_ERROR, LOGTAG_COM, __VA_ARGS__)
#define LOGD(...)     __android_log_print(ANDROID_LOG_DEBUG, LOGTAG_COM, __VA_ARGS__)



#endif //OPENCLREPO_COM_LOGGER_H
