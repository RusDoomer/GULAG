#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyze.h"
#include "global.h"
#include "util.h"
#include "io_util.h"
#include "meta.h"

void single_analyze(layout *lt)
{
    int row0, col0, row1, col1, row2, col2, row3, col3;
    for (int i = 0; i < MONO_END; i++)
    {
        lt->mono_score[i] = 0;
        int length = stats_mono[i].length;
        for (int j = 0; j < length; j++)
        {
            unflat_mono(stats_mono[i].ngrams[j], &row0, &col0);
            if (lt->matrix[row0][col0] != -1)
            {
                size_t index = index_mono(lt->matrix[row0][col0]);
                lt->mono_score[i] += linear_mono[index];
            }
        }
    }

    for (int i = 0; i < BI_END; i++)
    {
        lt->bi_score[i] = 0;
        int length = stats_bi[i].length;
        for (int j = 0; j < length; j++)
        {
            unflat_bi(stats_bi[i].ngrams[j], &row0, &col0, &row1, &col1);
            if (lt->matrix[row0][col0] != -1 && lt->matrix[row1][col1] != -1)
            {
                size_t index = index_bi(lt->matrix[row0][col0], lt->matrix[row1][col1]);
                lt->bi_score[i] += linear_bi[index];
            }
        }
    }

    for (int i = 0; i < TRI_END; i++)
    {
        lt->tri_score[i] = 0;
        int length = stats_tri[i].length;
        for (int j = 0; j < length; j++)
        {
            unflat_tri(stats_tri[i].ngrams[j], &row0, &col0, &row1, &col1, &row2, &col2);
            if (lt->matrix[row0][col0] != -1 && lt->matrix[row1][col1] != -1 && lt->matrix[row2][col2] != -1)
            {
                size_t index = index_tri(lt->matrix[row0][col0], lt->matrix[row1][col1], lt->matrix[row2][col2]);
                lt->tri_score[i] += linear_tri[index];
            }
        }
    }

    for (int i = 0; i < QUAD_END; i++)
    {
        lt->quad_score[i] = 0;
        int length = stats_quad[i].length;
        for (int j = 0; j < length; j++)
        {
            unflat_quad(stats_quad[i].ngrams[j], &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
            if (lt->matrix[row0][col0] != -1 && lt->matrix[row1][col1] != -1 && lt->matrix[row2][col2] != -1 && lt->matrix[row3][col3] != -1)
            {
                size_t index = index_quad(lt->matrix[row0][col0], lt->matrix[row1][col1], lt->matrix[row2][col2], lt->matrix[row3][col3]);
                lt->quad_score[i] += linear_quad[index];
            }
        }
    }

    for (int i = 0; i < SKIP_END; i++)
    {
        int length = stats_skip[i].length;
        for (int k = 1; k <= 9; k++)
        {
            lt->skip_score[k][i] = 0;
            for (int j = 0; j < length; j++)
            {
                unflat_bi(stats_skip[i].ngrams[j], &row0, &col0, &row1, &col1);
                if (lt->matrix[row0][col0] != -1 && lt->matrix[row1][col1] != -1)
                {
                    size_t index = index_skip(k, lt->matrix[row0][col0], lt->matrix[row1][col1]);
                    lt->skip_score[k][i] += linear_skip[index];
                }
            }
        }
    }

    meta_analysis(lt);
}

void cl_single_analyze(layout *lt)
{
    /*
    // 1. Get Platform and Device Information
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    if (ret != CL_SUCCESS) {error("OpenCL: clGetPlatformIDs failed");}

    // Try to get a GPU device, if not available, fall back to CPU
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
    if (ret != CL_SUCCESS)
    {
        wprintf(L"OpenCL: GPU not found, attempting CPU\n");
        ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_CPU, 1, &device_id, &ret_num_devices);
        if (ret != CL_SUCCESS) {error("OpenCL: clGetDeviceIDs failed");}
    }

    // 2. Create OpenCL Context
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    if (ret != CL_SUCCESS) {error("OpenCL: clCreateContext failed");}

    // 3. Create Command Queue
    cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret);
    if (ret != CL_SUCCESS) {error("OpenCL: clCreateCommandQueueWithProperties failed");}
    */
    error("opencl analysis not implemented");
}
