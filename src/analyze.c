/*
 * analyze.c - Analysis functions for the GULAG.
 *
 * Implements layout analysis, including single layout cpu analysis.
 */

#include "analyze.h"
#include "global.h"
#include "structs.h"
#include "util.h"
#include "meta.h"

/*
 * Performs analysis on a single layout, calculating statistics for monograms,
 * bigrams, trigrams, quadgrams, and skipgrams. Then uses those values for meta
 * statistics.
 *
 * Parameters:
 *   lt: A pointer to the layout to analyze.
 */
void single_analyze(layout *lt)
{
    int row0, col0, row1, col1, row2, col2, row3, col3;

    /* Calculate monogram statistics. */
    for (int i = 0; i < MONO_LENGTH; i++)
    {
        if(!stats_mono[i].skip)
        {
            lt->mono_score[i] = 0;
            int length = stats_mono[i].length;
            for (int j = 0; j < length; j++)
            {
                /* unflattens a 1D index into a 2D matrix coordinate */
                unflat_mono(stats_mono[i].ngrams[j], &row0, &col0); /* util.c */
                if (lt->matrix[row0][col0] != -1)
                {
                    /* calculates the index for a monogram in a linearized array */
                    size_t index = index_mono(lt->matrix[row0][col0]); /* util.c */
                    lt->mono_score[i] += linear_mono[index];
                }
            }
        }
    }

    /* Calculate bigram statistics. */
    for (int i = 0; i < BI_LENGTH; i++)
    {
        if(!stats_bi[i].skip)
        {
            lt->bi_score[i] = 0;
            int length = stats_bi[i].length;
            for (int j = 0; j < length; j++)
            {
                /* unflattens a 1D index into a 4D matrix coordinate */
                unflat_bi(stats_bi[i].ngrams[j], &row0, &col0, &row1, &col1); /* util.c */
                if (lt->matrix[row0][col0] != -1 && lt->matrix[row1][col1] != -1)
                {
                    /* calculates the index for a bigram in a linearized array */
                    size_t index = index_bi(lt->matrix[row0][col0], lt->matrix[row1][col1]); /* util.c */
                    lt->bi_score[i] += linear_bi[index];
                }
            }
        }
    }

    /* Calculate trigram statistics. */
    for (int i = 0; i < TRI_LENGTH; i++)
    {
        if(!stats_tri[i].skip)
        {
            lt->tri_score[i] = 0;
            int length = stats_tri[i].length;
            for (int j = 0; j < length; j++)
            {
                /* unflattens a 1D index into a 6D matrix coordinate */
                unflat_tri(stats_tri[i].ngrams[j], &row0, &col0, &row1, &col1, &row2, &col2); /* util.c */
                if (lt->matrix[row0][col0] != -1 && lt->matrix[row1][col1] != -1 && lt->matrix[row2][col2] != -1)
                {
                    /* calculates the index for a trigram in a linearized array */
                    size_t index = index_tri(lt->matrix[row0][col0], lt->matrix[row1][col1], lt->matrix[row2][col2]); /* util.c */
                    lt->tri_score[i] += linear_tri[index];
                }
            }
        }
    }

    /* Calculate quadgram statistics. */
    for (int i = 0; i < QUAD_LENGTH; i++)
    {
        if(!stats_quad[i].skip)
        {
            lt->quad_score[i] = 0;
            int length = stats_quad[i].length;
            for (int j = 0; j < length; j++)
            {
                /* unflattens a 1D index into a 8D matrix coordinate */
                unflat_quad(stats_quad[i].ngrams[j], &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3); /* util.c */
                if (lt->matrix[row0][col0] != -1 && lt->matrix[row1][col1] != -1 && lt->matrix[row2][col2] != -1 && lt->matrix[row3][col3] != -1)
                {
                    /* calculates the index for a quadgram in a linearized array */
                    size_t index = index_quad(lt->matrix[row0][col0], lt->matrix[row1][col1], lt->matrix[row2][col2], lt->matrix[row3][col3]); /* util.c */
                    lt->quad_score[i] += linear_quad[index];
                }
            }
        }
    }

    /* Calculate skipgram statistics. */
    for (int i = 0; i < SKIP_LENGTH; i++)
    {
        if(!stats_skip[i].skip)
        {
            int length = stats_skip[i].length;
            for (int k = 1; k <= 9; k++)
            {
                lt->skip_score[k][i] = 0;
                for (int j = 0; j < length; j++)
                {
                    /* unflattens a 1D index into a 4D matrix coordinate */
                    unflat_bi(stats_skip[i].ngrams[j], &row0, &col0, &row1, &col1); /* util.c */
                    if (lt->matrix[row0][col0] != -1 && lt->matrix[row1][col1] != -1)
                    {
                        /* calculates the index for a skipgram in a linearized array */
                        size_t index = index_skip(k, lt->matrix[row0][col0], lt->matrix[row1][col1]); /* util.c */
                        lt->skip_score[k][i] += linear_skip[index];
                    }
                }
            }
        }
    }

    /* Perform meta-analysis, which may depend on previously calculated statistics. */
    for (int i = 0; i < META_LENGTH; i++)
    {
        if (!stats_meta[i].skip)
        {
            lt->meta_score[0] = 0;
            int j = 0;
            while (stats_meta[i].stat_types[j] != 'x')
            {
                switch(stats_meta[i].stat_types[j]) {
                default:
                case 'm':
                    lt->meta_score[i] += lt->mono_score[stats_meta[i].stat_indices[j]] * stats_meta[i].stat_weights[j];
                    break;
                case 'b':
                    lt->meta_score[i] += lt->bi_score[stats_meta[i].stat_indices[j]] * stats_meta[i].stat_weights[j];
                    break;
                case 't':
                    lt->meta_score[i] += lt->tri_score[stats_meta[i].stat_indices[j]] * stats_meta[i].stat_weights[j];
                    break;
                case 'q':
                    lt->meta_score[i] += lt->quad_score[stats_meta[i].stat_indices[j]] * stats_meta[i].stat_weights[j];
                    break;
                case '1':
                    lt->meta_score[i] += lt->skip_score[1][stats_meta[i].stat_indices[j]] * stats_meta[i].stat_weights[j];
                    break;
                case '2':
                    lt->meta_score[i] += lt->skip_score[2][stats_meta[i].stat_indices[j]] * stats_meta[i].stat_weights[j];
                    break;
                case '3':
                    lt->meta_score[i] += lt->skip_score[3][stats_meta[i].stat_indices[j]] * stats_meta[i].stat_weights[j];
                    break;
                case '4':
                    lt->meta_score[i] += lt->skip_score[4][stats_meta[i].stat_indices[j]] * stats_meta[i].stat_weights[j];
                    break;
                case '5':
                    lt->meta_score[i] += lt->skip_score[5][stats_meta[i].stat_indices[j]] * stats_meta[i].stat_weights[j];
                    break;
                case '6':
                    lt->meta_score[i] += lt->skip_score[6][stats_meta[i].stat_indices[j]] * stats_meta[i].stat_weights[j];
                    break;
                case '7':
                    lt->meta_score[i] += lt->skip_score[7][stats_meta[i].stat_indices[j]] * stats_meta[i].stat_weights[j];
                    break;
                case '8':
                    lt->meta_score[i] += lt->skip_score[8][stats_meta[i].stat_indices[j]] * stats_meta[i].stat_weights[j];
                    break;
                case '9':
                    lt->meta_score[i] += lt->skip_score[9][stats_meta[i].stat_indices[j]] * stats_meta[i].stat_weights[j];
                    break;
                }
                j++;
            }
            if (stats_meta[i].absv && lt->meta_score[i] < 0) {lt->meta_score[i] *= -1;}
        }
    }
}
