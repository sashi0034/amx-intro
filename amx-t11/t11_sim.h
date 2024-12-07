#ifndef AMX_INTRO_T11_SIM_H
#define AMX_INTRO_T11_SIM_H

#include <stdio.h>
#include <bits/types/clock_t.h>

#include "t11_mhd.h"

#define STATIC_ASSERT(expr, msg) typedef char static_assert_##msg[(expr) ? 1 : -1]

#define DEFINE_MATRIX(name, r, c) \
    typedef struct name { \
        union { \
            float fp32s[(r) * (c)]; \
            struct { \
                float cols[(c)]; \
            } rows[(r)];          \
            float mat[r][c]; \
        }; \
    } name; \
    \
    static void print_##name(const name* mat) { \
        for (int i = 0; i < (r); ++i) { \
            for (int j = 0; j < (c); ++j) { \
                printf("%f ", (mat->rows[i].cols[j])); \
            } \
            printf("\n"); \
        } \
    } \
    \
    static void fprint_##name(FILE* file, const name* mat) { \
        for (int i = 0; i < (r); ++i) { \
            for (int j = 0; j < (c); ++j) { \
                fprintf(file, "%f ", (mat->rows[i].cols[j])); \
            } \
            fprintf(file, "\n"); \
        } \
    } \
    \
    static void fprint_##name##_to_file(const char* filename, const name* mat) { \
        FILE* file = fopen(filename, "w"); \
        if (!file) { \
            perror("Failed to open file"); \
            return; \
        } \
        fprint_##name(file, mat); \
        fclose(file); \
    }

// -----------------------------------------------

#define SIM_MAT_ROWS 192
#define SIM_MAT_COLS 192

STATIC_ASSERT(SIM_MAT_COLS == NX2, sim_mat_cols_must_be_nx2);
STATIC_ASSERT(SIM_MAT_ROWS == NY2, sim_mat_rows_must_be_ny2);

DEFINE_MATRIX(SimMat, SIM_MAT_ROWS, SIM_MAT_COLS)

#define FILTER_SIZE 3
#define FILTER_PADDING ((FILTER_SIZE - 1) / 2)

DEFINE_MATRIX(Filter3x3, FILTER_SIZE, FILTER_SIZE)

void make_filter(Filter3x3 *filter);

// -----------------------------------------------

typedef struct SimState {
    // Simulation arrays
    float (*restrict f)[NZ2][NY2][NX2];
    float (*restrict u)[NZ2][NY2][NX2];
    float (*restrict pp)[NZ2][NY2][NX2];

    clock_t zt0, zt1, zt2, zt3;
    double zt, ztt;

    int nmlf, nupb, nout;
    int ntap;

    float vmax;
} SimState;

void init_sim_state(SimState *state);

#define NOUT0 64

void tick_sim_state(SimState *state, int ii);

#define SAMPLE_LAYER_N 6
#define SAMPLE_LAYER_Z (NZ2 / 2)

#endif //AMX_INTRO_T11_SIM_H
