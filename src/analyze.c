/*
 * analyze.c - Analysis functions for the GULAG project.
 *
 * Author: Rus Doomer
 *
 * Description: Implements layout analysis, including single layout analysis
 *              and OpenCL-accelerated analysis.
 */

#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyze.h"
#include "global.h"
#include "util.h"
#include "io_util.h"
#include "meta.h"

/*
 * Performs analysis on a single layout, calculating statistics for monograms,
 * bigrams, trigrams, quadgrams, and skipgrams. It then delegates to meta_analysis
 * for the calculation of meta-statistics.
 *
 * Parameters:
 *   lt: A pointer to the layout to analyze.
 *
 * Returns: void.
 */
void single_analyze(layout *lt)
{
    int row0, col0, row1, col1, row2, col2, row3, col3;

    /* Calculate monogram statistics. */
    for (int i = 0; i < MONO_END; i++)
    {
        lt->mono_score[i] = 0;
        int length = stats_mono[i].length;
        for (int j = 0; j < length; j++)
        {
            /* util.c - unflattens a 1D index into a 2D matrix coordinate */
            unflat_mono(stats_mono[i].ngrams[j], &row0, &col0);
            if (lt->matrix[row0][col0] != -1)
            {
                /* util.c - calculates the index for a monogram in a linearized array */
                size_t index = index_mono(lt->matrix[row0][col0]);
                lt->mono_score[i] += linear_mono[index];
            }
        }
    }

    /* Calculate bigram statistics. */
    for (int i = 0; i < BI_END; i++)
    {
        lt->bi_score[i] = 0;
        int length = stats_bi[i].length;
        for (int j = 0; j < length; j++)
        {
            /* util.c - unflattens a 1D index into a 4D matrix coordinate */
            unflat_bi(stats_bi[i].ngrams[j], &row0, &col0, &row1, &col1);
            if (lt->matrix[row0][col0] != -1 && lt->matrix[row1][col1] != -1)
            {
                /* util.c - calculates the index for a bigram in a linearized array */
                size_t index = index_bi(lt->matrix[row0][col0], lt->matrix[row1][col1]);
                lt->bi_score[i] += linear_bi[index];
            }
        }
    }

    /* Calculate trigram statistics. */
    for (int i = 0; i < TRI_END; i++)
    {
        lt->tri_score[i] = 0;
        int length = stats_tri[i].length;
        for (int j = 0; j < length; j++)
        {
            /* util.c - unflattens a 1D index into a 6D matrix coordinate */
            unflat_tri(stats_tri[i].ngrams[j], &row0, &col0, &row1, &col1, &row2, &col2);
            if (lt->matrix[row0][col0] != -1 && lt->matrix[row1][col1] != -1 && lt->matrix[row2][col2] != -1)
            {
                /* util.c - calculates the index for a trigram in a linearized array */
                size_t index = index_tri(lt->matrix[row0][col0], lt->matrix[row1][col1], lt->matrix[row2][col2]);
                lt->tri_score[i] += linear_tri[index];
            }
        }
    }

    /* Calculate quadgram statistics. */
    for (int i = 0; i < QUAD_END; i++)
    {
        lt->quad_score[i] = 0;
        int length = stats_quad[i].length;
        for (int j = 0; j < length; j++)
        {
            /* util.c - unflattens a 1D index into a 8D matrix coordinate */
            unflat_quad(stats_quad[i].ngrams[j], &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
            if (lt->matrix[row0][col0] != -1 && lt->matrix[row1][col1] != -1 && lt->matrix[row2][col2] != -1 && lt->matrix[row3][col3] != -1)
            {
                /* util.c - calculates the index for a quadgram in a linearized array */
                size_t index = index_quad(lt->matrix[row0][col0], lt->matrix[row1][col1], lt->matrix[row2][col2], lt->matrix[row3][col3]);
                lt->quad_score[i] += linear_quad[index];
            }
        }
    }

    /* Calculate skipgram statistics. */
    for (int i = 0; i < SKIP_END; i++)
    {
        int length = stats_skip[i].length;
        for (int k = 1; k <= 9; k++)
        {
            lt->skip_score[k][i] = 0;
            for (int j = 0; j < length; j++)
            {
                /* util.c - unflattens a 1D index into a 4D matrix coordinate */
                unflat_bi(stats_skip[i].ngrams[j], &row0, &col0, &row1, &col1);
                if (lt->matrix[row0][col0] != -1 && lt->matrix[row1][col1] != -1)
                {
                    /* util.c - calculates the index for a skipgram in a linearized array */
                    size_t index = index_skip(k, lt->matrix[row0][col0], lt->matrix[row1][col1]);
                    lt->skip_score[k][i] += linear_skip[index];
                }
            }
        }
    }

    /* Perform meta-analysis, which may depend on previously calculated statistics. */
    meta_analysis(lt);
}

/*
 * Placeholder for OpenCL-accelerated layout analysis.
 * Currently, this function is not implemented and will throw an error if called.
 *
 * Parameters:
 *   lt: A pointer to the layout to analyze.
 *
 * Returns: void.
 */
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
