//
// Created by lisan on 2019/4/7.
//

#ifndef OPENCLREPO_KERNEL_H
#define OPENCLREPO_KERNEL_H


const char *kernel_code = R"(

const sampler_t samp = CLK_NORMALIZED_COORDS_FALSE
                        | CLK_ADDRESS_NONE
                        | CLK_FILTER_NEAREST;

//一次处理一个点
__kernel void remapy(read_only image2d_t src, write_only image2d_t dst, read_only image2d_t mapx, read_only image2d_t mapy)
{
    int2 coord = (int2)(get_local_id(0), get_local_id(1));

    float4 fromx = read_imagef(mapx, samp, coord);
    float4 fromy = read_imagef(mapy, samp, coord);
    fromx.y = fromy.x;

    int2 coord2 = convert_int2(fromx.xy);
    float2 ffrac = (float2)(fromx.x - (float)(coord2.x), fromx.y - (float)(coord2.y));

    uint4 y0 = read_imageui(src, samp, coord2);
    coord2.x += 1;
    uint4 y1 = read_imageui(src, samp, coord2);
    coord2.y += 1;
    uint4 y3 = read_imageui(src, samp, coord2);
    coord2.x -= 1;
    uint4 y2 = read_imageui(src, samp, coord2);

    float2 y01 = convert_float2((uint2)(y0.x, y1.x));
    float2 y23 = convert_float2((uint2)(y2.x, y3.x));

    float2 y_v = mix(y01, y23, ffrac.y);
    float result = mix(y_v.x, y_v.y, ffrac.x);

    write_imageui(dst, coord, (uint4)((uint)(result), 0, 0, 0));
}


__kernel void remapuv(read_only image2d_t src, write_only image2d_t dst, read_only image2d_t mapx, read_only image2d_t mapy)
{
    int2 coord = (int2)(get_local_id(0), get_local_id(1));

    float4 fromx = read_imagef(mapx, samp, coord * 2);  //因为这个是y通道的map
    float4 fromy = read_imagef(mapy, samp, coord * 2);
    fromx.y = fromy.x;

    fromx /= 2.0;   //把针对y的坐标缩小到一半

    int2 coord2 = convert_int2(fromx.xy);
    float2 ffrac = (float2)(fromx.x - (float)(coord2.x), fromx.y - (float)(coord2.y));

    uint4 y0 = read_imageui(src, samp, coord2);
    coord2.x += 1;
    uint4 y1 = read_imageui(src, samp, coord2);
    coord2.y += 1;
    uint4 y3 = read_imageui(src, samp, coord2);
    coord2.x -= 1;
    uint4 y2 = read_imageui(src, samp, coord2);

    float4 y01 = convert_float4((uint4)(y0.xy, y1.xy));
    float4 y23 = convert_float4((uint4)(y2.xy, y3.xy));

    float4 y_v = mix(y01, y23, ffrac.y);
    float2 result = mix(y_v.xy, y_v.zw, ffrac.x);

    write_imageui(dst, coord, (uint4)(convert_uint2(result), 0, 0));
}
)";

#endif //OPENCLREPO_KERNEL_H
