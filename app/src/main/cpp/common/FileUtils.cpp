//
// Created by lisan on 2019/4/7.
//

#include <cstdio>
#include <malloc.h>
#include <cstring>
#include "FileUtils.h"
#include "comTypes.h"


pImageDATA createEmptyYVU(const int format, const int width, const int height, int stride){
    if(stride == 0) stride = width;
    pImageDATA result = (pImageDATA)malloc(sizeof(IMAGE_DATA));
    result->width = width;
    result->height = height;

    if(FORMAT_YUV_NV12 == format){
        result->format = FORMAT_YUV_NV12;
        result->pstride[0] = stride;
        result->pstride[1] = stride;
        result->pstride[2] = stride;

        result->pplane[0] = (uchar *)malloc(height * stride * 3 / 2);
        result->pplane[1] = result->pplane[0] + stride * height;
    }else if(FORMAT_YUV_YV12 == format){
        result->format = FORMAT_YUV_YV12;
        result->pstride[0] = stride;
        result->pstride[1] = stride / 2;
        result->pstride[2] = stride / 2;

        result->pplane[0] = (uchar *)malloc(height * stride * 3 / 2);
        result->pplane[1] = result->pplane[0] + stride * height;
        result->pplane[2] = result->pplane[1] + stride * height / 4;
    }else{
        free(result);
        LOGE("NOT SUPPORT FORMAT %d", format);
        return NULL;
    }

    return result;
}


void freeImageData(pImageDATA data){
    free(data->pplane[0]);
    free(data);
}

pImageDATA changYV12ToNV12(pImageDATA yv12)
{
    pImageDATA result = createEmptyYVU(FORMAT_YUV_NV12, yv12->width, yv12->height, yv12->pstride[0]);
    memcpy(result->pplane[0], yv12->pplane[0], yv12->pstride[0] * yv12->height);

    for (int i = 0; i < yv12->height / 2; ++i) {
        uchar *ptr1 = result->pplane[1] + i * result->pstride[1];
        uchar *ptr2 = yv12->pplane[1] + i * yv12->pstride[1];
        uchar *ptr3 = yv12->pplane[2] + i * yv12->pstride[2];
        for (int j = 0; j < yv12->width / 2; ++j) {
            *ptr1++ = *ptr3++;
            *ptr1++ = *ptr2++;
        }
    }
    return result;
}


pImageDATA changNV12ToYV12(pImageDATA nv12)
{
    pImageDATA result = createEmptyYVU(FORMAT_YUV_YV12, nv12->width, nv12->height, nv12->pstride[0]);
    memcpy(result->pplane[0], nv12->pplane[0], nv12->pstride[0] * nv12->height);

    for (int i = 0; i < nv12->height / 2; ++i) {
        uchar *ptr1 = result->pplane[1] + i * result->pstride[1];
        uchar *ptr2 = result->pplane[2] + i * result->pstride[2];
        uchar *ptr3 = nv12->pplane[1] + i * nv12->pstride[1];
        for (int i = 0; i < nv12->width / 2; ++i) {
            *ptr2++ = *ptr3++;
            *ptr1++ = *ptr3++;
        }
    }
    return result;
}

