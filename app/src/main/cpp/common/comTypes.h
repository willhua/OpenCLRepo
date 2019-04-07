//
// Created by lisan on 2019/4/7.
//

#ifndef OPENCLREPO_COMTYPES_H
#define OPENCLREPO_COMTYPES_H




/***************************************************************/
/**********************status define****************************/
/***************************************************************/
#define FUNC_STATUS int
const FUNC_STATUS FUNC_SUCCESS          = 0;
const FUNC_STATUS FUNC_FAIL_GENERAL     = FUNC_SUCCESS - 1;
const FUNC_STATUS FUNC_FAIL_BAD_PARAM   = FUNC_SUCCESS - 2;
const FUNC_STATUS FUNC_FAIL_MEM_ALLOC   = FUNC_SUCCESS - 3;



/***************************************************************/
/**********************image define*****************************/
/***************************************************************/
const int FORMAT_RGBA       = 0;
const int FORMAT_RGB        = FORMAT_RGBA + 1;
const int FORMAT_BGR        = FORMAT_RGBA + 2;
const int FORMAT_ARGB       = FORMAT_RGBA + 3;
const int FORMAT_GRAY       = FORMAT_RGBA + 4;
const int FORMAT_YUV_NV12   = FORMAT_RGBA + 5;
const int FORMAT_YUV_NV21   = FORMAT_RGBA + 6;
const int FORMAT_YUV_YV12   = FORMAT_RGBA + 7;

typedef struct _tag_IMAGE_DATA{
    int                 width;
    int                 height;
    int                 format;
    unsigned char       *pplane[4];
    int                 pstride[4];
}IMAGE_DATA, *pImageDATA;




#endif //OPENCLREPO_COMTYPES_H
