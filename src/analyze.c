#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyze.h"
#include "global.h"
#include "util.h"

void single_analyze(layout *lt)
{
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    if (ret != CL_SUCCESS) {error("OpenCL: clGetPlatformIDs failed");}

    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
    if (ret != CL_SUCCESS)
    {
        wprintf(L"OpenCL: GPU not found, attempting CPU\n");
        ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_CPU, 1, &device_id, &ret_num_devices);
        if (ret != CL_SUCCESS) {error("OpenCL: clGetDeviceIDs failed");}
        else {wprintf(L"OpenCL: CPU found\n");}
    }
    else {wprintf(L"OpenCL: GPU found\n");}

    return;
}

