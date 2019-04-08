//
// Created by lisan on 2019/4/7.
//

#include <comTypes.h>
#include <CL/cl.h>
#include <utils/cl_wrapper.h>
#include <cl_utils.h>
#include "remaper.h"
#include "kernel.h"


typedef struct RemapEngine{
    int imgwidth;
    int imgheight;
    int imgformat;
    float *remapx;
    float *remapy;

    cl_mem memRemapX;
    cl_mem memRemapY;
    cl_mem memOut;
    cl_mem memSrc;
    cl_wrapper *pwrapper;
    cl_program program;
    cl_kernel kernel_y;
    cl_kernel kernel_uv;

}*pRemapEngine;



FUNC_STATUS initRemaper(void **outEngine, pREMAPER_INIT_PARAM param){
    pRemapEngine engine = new RemapEngine;

    engine->imgformat = param->imgformat;
    engine->imgwidth = param->imgwidth;
    engine->imgheight = param->imgheight;

    engine->remapx = new float[engine->imgwidth * engine->imgheight];
    engine->remapy = new float[engine->imgwidth * engine->imgheight];
    memcpy(engine->remapx, param->remapx, sizeof(float) * engine->imgwidth * engine->imgheight);
    memcpy(engine->remapy, param->remapy, sizeof(float) * engine->imgwidth * engine->imgheight);

    cl_int err;
    engine->pwrapper = new cl_wrapper;
    cl_context context = engine->pwrapper->get_context();

    cl_image_format remap_image_format;
    remap_image_format.image_channel_order = CL_R;
    remap_image_format.image_channel_data_type = CL_FLOAT;

    cl_image_desc remap_image_desc;
    memset(&remap_image_desc, 0, sizeof(cl_image_desc));
    remap_image_desc.image_width = engine->imgwidth;
    remap_image_desc.image_height = engine->imgheight;
    remap_image_desc.image_type = CL_MEM_OBJECT_IMAGE2D;

    engine->memRemapX = clCreateImage(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, &remap_image_format, &remap_image_desc, param->remapx, &err);
    checkCLErr(err, "engine->memRemapX");
    engine->memRemapY = clCreateImage(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, &remap_image_format, &remap_image_desc, param->remapy, &err);
    checkCLErr(err, "engine->memRemapY");

    engine->program = engine->pwrapper->make_program(&kernel_code, strlen(kernel_code));
    engine->kernel_y = clCreateKernel(engine->program, "remapy", &err);
    checkCLErr(err, "kernel y");
    engine->kernel_uv = clCreateKernel(engine->program, "remapuv", &err);
    checkCLErr(err, "kernel uv");

    *outEngine = engine;
    return FUNC_SUCCESS;
}


//NV12
FUNC_STATUS remap(void *pengine, pImageDATA imgdata){
    LOGD("remap start %p", pengine);
    pRemapEngine engine = (pRemapEngine)pengine;
    cl_int err;
    cl_context context = engine->pwrapper->get_context();

    cl_image_format image_format;
    image_format.image_channel_order = CL_R;
    image_format.image_channel_data_type = CL_UNSIGNED_INT8;

    cl_image_desc image_desc;
    memset(&image_desc, 0, sizeof(cl_image_desc));
    image_desc.image_width = imgdata->pstride[0];
    image_desc.image_height = imgdata->height;
    image_desc.image_type = CL_MEM_OBJECT_IMAGE2D;

    cl_mem mem_srcy = clCreateImage(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, &image_format, &image_desc, imgdata->pplane[0], &err);
    checkCLErr(err, "mem_srcy");
    cl_mem mem_outy = clCreateImage(context, CL_MEM_WRITE_ONLY, &image_format, &image_desc, NULL, &err);
    checkCLErr(err, "mem_outy");

    image_format.image_channel_order = CL_RG;
    image_desc.image_width = imgdata->pstride[1] / 2;
    image_desc.image_height = imgdata->height / 2;
    cl_mem mem_srcuv = clCreateImage(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, &image_format, &image_desc, imgdata->pplane[1], &err);
    checkCLErr(err, "mem_SRCUV");
    cl_mem mem_outUV = clCreateImage(context, CL_MEM_WRITE_ONLY, &image_format, &image_desc, NULL, &err);
    checkCLErr(err, "mem_OUTUV");


    err = clSetKernelArg(engine->kernel_y, 0, sizeof(cl_mem), &mem_srcy);
    checkCLErr(err, "kernel arg y 0");
    err = clSetKernelArg(engine->kernel_y, 1, sizeof(cl_mem), &mem_outy);
    checkCLErr(err, "kernel arg y 1");
    err = clSetKernelArg(engine->kernel_y, 2, sizeof(cl_mem), &engine->memRemapX);
    checkCLErr(err, "kernel arg y 2");
    err = clSetKernelArg(engine->kernel_y, 3, sizeof(cl_mem), &engine->memRemapY);
    checkCLErr(err, "kernel arg y 3");

    const size_t offset[]{0, 0};
    const size_t globalsize_y[]{(size_t)imgdata->width, (size_t)imgdata->height};
    const size_t local_size[]{20, 20};
    err = clEnqueueNDRangeKernel(engine->pwrapper->get_command_queue(), engine->kernel_y, 2, offset, globalsize_y, local_size, 0, NULL, NULL);

    const size_t globalsize_uv[]{(size_t)imgdata->width / 2, (size_t)imgdata->height / 2};
    err = clEnqueueNDRangeKernel(engine->pwrapper->get_command_queue(), engine->kernel_uv, 2, offset, globalsize_uv, local_size, 0, NULL, NULL);

    clFinish(engine->pwrapper->get_command_queue());


    /********get result*********/
    size_t rowpitch_y;
    const size_t origin[]{0, 0, 0};
    const size_t range_y[]{(size_t)imgdata->pstride[0], (size_t)imgdata->height, 1};
    char *mappedPtr = (char*)clEnqueueMapImage(engine->pwrapper->get_command_queue(),
            mem_outy, CL_BLOCKING, CL_MAP_READ, origin, range_y, &rowpitch_y, NULL, 0, NULL, NULL, &err);
    checkCLErr(err, "map y");
    for (int i = 0; i < imgdata->height; ++i) {
        memcpy(imgdata->pplane[0] + imgdata->pstride[0] * i, mappedPtr + rowpitch_y * i, imgdata->width);
    }
    clEnqueueUnmapMemObject(engine->pwrapper->get_command_queue(), mem_outy, mappedPtr, 0, NULL, NULL);

    size_t rowpitch_uv;
    const size_t range_uv[]{(size_t)imgdata->pstride[1] / 2, (size_t)imgdata->height / 2, 1};
    mappedPtr = (char*)clEnqueueMapImage(engine->pwrapper->get_command_queue(),
                                                mem_outUV, CL_BLOCKING, CL_MAP_READ, origin, range_uv, &rowpitch_uv, NULL, 0, NULL, NULL, &err);
    checkCLErr(err, "map uv");
    for (int i = 0; i < imgdata->height / 2; ++i) {
        memcpy(imgdata->pplane[1] + imgdata->pstride[1] * i, mappedPtr + rowpitch_uv * i, imgdata->width);
    }
    clEnqueueUnmapMemObject(engine->pwrapper->get_command_queue(), mem_outUV, mappedPtr, 0, NULL, NULL);

    //todo:release


    return FUNC_SUCCESS;
}





void remaptest(pImageDATA img, float *remapx, float *remapy){
    void *engine;
    REMAPER_INIT_PARAM init_param;
    init_param.imgheight = img->height;
    init_param.imgwidth = img->width;
    init_param.imgformat = FORMAT_YUV_NV12;
    init_param.remapx = remapx;
    init_param.remapy = remapy;


    initRemaper(&engine, &init_param);

    remap(engine, img);


    LOGD("remaptest end");
}
