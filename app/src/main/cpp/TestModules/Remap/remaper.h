//
// Created by lisan on 2019/4/7.
//

#ifndef OPENCLREPO_REMAPER_H
#define OPENCLREPO_REMAPER_H


#include <comTypes.h>

typedef struct _tag_REMAPER_INIT_PARAM{
    int imgwidth;
    int imgheight;
    int imgformat;
    float *remapx;
    float *remapy;
}REMAPER_INIT_PARAM, *pREMAPER_INIT_PARAM;



void remaptest(pImageDATA img, float *remapx, float *remapy);

#endif //OPENCLREPO_REMAPER_H
