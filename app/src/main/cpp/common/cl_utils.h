//
// Created by lisan on 2019/4/7.
//

#ifndef OPENCLREPO_CL_UTILS_H
#define OPENCLREPO_CL_UTILS_H

#include <CL/cl_platform.h>
#include <string>
#include <CL/cl.h>
#include "com_logger.h"


const char * getCLErrString(const cl_int err);





inline void checkCLErr(const cl_int err, const std::string &msg = ""){
    if(CL_SUCCESS != err){
        LOGE("ERROR: %s     %s", getCLErrString(err), msg.c_str());
    }else{
        LOGD("success:%s", msg.c_str());
    }
}





#endif //OPENCLREPO_CL_UTILS_H
