//
// Created by lisan on 2019/4/7.
//

#ifndef OPENCLREPO_FILEUTILS_H
#define OPENCLREPO_FILEUTILS_H


#include <cstdio>
#include "com_logger.h"
#include "comTypes.h"


pImageDATA createEmptyYVU(const int format, const int width, const int height, int stride = 0);

void freeImageData(pImageDATA data);

pImageDATA changYV12ToNV12(pImageDATA yv12);

pImageDATA changNV12ToYV12(pImageDATA nv12);

inline void writeFile(const void *data, const int size, const char *filepath, const char *mode = "wb"){
    FILE *fs = fopen(filepath, mode);
    LOGD("FS %p", fs);
    fwrite(data, size, 1, fs);
    fclose(fs);
}

#endif //OPENCLREPO_FILEUTILS_H
