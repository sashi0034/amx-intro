#ifndef AMX_INTRO_T9_MHD_H
#define AMX_INTRO_T9_MHD_H

#define NX 198
#define NY 198
#define NZ 248

#define NB 8
#define NBB 11
#define NBBB 3

#define NX1 (NX + 1)
#define NX2 (NX + 2)
#define NY1 (NY + 1)
#define NY2 (NY + 2)
#define NZ1 (NZ + 1)
#define NZ2 (NZ + 2)

#define LAST 64

#define THX 0.0005

#define NMLF0 8
#define NUPB0 32

void simuinfo();

void equil_out(float f[restrict NB][NZ2][NY2][NX2], float pp[restrict NBBB][NZ2][NY2][NX2]);

void bdry_z_m(float f[restrict NB][NZ2][NY2][NX2], float u[restrict NB][NZ2][NY2][NX2]);

int mlf(float f[restrict NB][NZ2][NY2][NX2],
        float u[restrict NB][NZ2][NY2][NX2],
        float pp[restrict NBBB][NZ2][NY2][NX2],
        int ii, int nmlf, float vmax);

void bdry_up(float f[restrict NB][NZ2][NY2][NX2], float u[restrict NB][NZ2][NY2][NX2]);

void gflops(double zt);

#endif //AMX_INTRO_T9_MHD_H
