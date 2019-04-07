//
// Created by lisan on 2019/4/7.
//

#ifndef OPENCLREPO_FILEUTILS_H
#define OPENCLREPO_FILEUTILS_H


#include <cstdio>
#include "com_logger.h"

inline void writeFile(const void *data, const int size, const char *filepath, const char *mode = "wb"){
    FILE *fs = fopen(filepath, mode);
    LOGD("FS %p", fs);
    fwrite(data, size, 1, fs);
    fclose(fs);
}

#endif //OPENCLREPO_FILEUTILS_H
