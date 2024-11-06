

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define NX 198
#define NY 198
#define NZ 248
#define NXP 100
#define NB 8
#define NBB 11
#define NBBB 3
#define NX1 (NX + 1)
#define NX2 (NX + 2)
#define NY1 (NY + 1)
#define NY2 (NY + 2)
#define N4 (NX2*NY2*NZ2 * NB)
#define N4BBB (NX2 * NY2 * NZ2 * NBBB)
#define N4BB (NX2 * NY2 * NZ2 * NBB)
#define NZ1 (NZ + 1)
#define NZ2 (NZ + 2)
#define NX11 (NX1 - 2 * NXP)
#define LAST 64
#define NMLF0 8
#define NUPB0 32
#define NOUT0 64
#define THX 0.0005
#define TAM 0.5
#define XL 120.6
#define YL 80.6
#define ZL 80.6
#define RA_I 4.0
#define RA_I2 (RA_I * RA_I)
#define ARU 4.0
#define HX (XL / NX1)
#define HY (YL / NY1)
#define HZ (ZL / NZ1)
#define T (0.5 * HX * THX)
#define DX2 (0.25 * T / HX)
#define DY2 (0.25 * T / HY)
#define DZ2 (0.25 * T / HZ)
#define DX4 (T / (HX * HX))
#define DY4 (T / (HY * HY))
#define DZ4 (T / (HY * HZ))
#define PI 3.1415926
#define GAM 5.0 / 3.0
#define GM1 (2.0 / (GAM - 1))
#define VO0 1.38e-4
#define GRA 2.2e-7
#define PO0 ((GAM - 1.0) * GRA / GAM)
#define B0 -1.0
#define PR010 3.62e-12
#define RO010 1.84e-5
#define NDIVIDE 1.00
#define RO01 (NDIVIDE * RO010)
#define PR01 (NDIVIDE * PR010)
#define VSW 3.27e-3
#define IMF 0.25e-6
#define R1CJ 10.0
#define DR 2.0
#define ALP 10.0
#define BET 1.20

#define VMU0 0.0005
#define PMU0 1.0
#define DFU0 1.0
#define EATT 0.0020
#define RRAT 0.2
#define RRA1 0.2
#define ARC 4.0

#define VMU (VMU0 * RO01)
#define PMU (VMU0 * PMU0)
#define DFU (VMU0 * DFU0)
#define RO02 (RRAT * RO01)
#define PR02 (RRAT * PR01)

void equil_out(float f[restrict NB][NZ2][NY2][NX2], float pp[restrict NBBB][NZ2][NY2][NX2]) {
    float ralf, x, y, z, ra1, ra2, ra3, ra1m, ra2m, ra3m, y1, xm, xm1;
    float th1, rr2, psi0, psi2, v0, vo, bx1, bx2, by1, by2, bz1, bz2, alp1, vr1;
    int i, j, k;

    alp1 = 1.0 / sqrt(R1CJ);
    xm = -(pow(2.0 * R1CJ * B0 * B0 / (RO01 * VSW * VSW), 1.0 / 6.0));

    for (k = 0; k < NZ2; k++) {
        for (j = 0; j < NY2; j++) {
            for (i = 0; i < NX2; i++) {
                x = 0.5 * HX * (2 * i - NX2 + 1 + 2 * NXP);
                y = 0.5 * HY * (2 * j - NY2 + 1);
                z = 0.5 * HZ * (2 * k - NZ2 + 1);
                ra2 = x * x + y * y + z * z;
                ra1 = sqrt(ra2);
                ra3 = ra1 * ra2;
                f[5][k][j][i] = -3.0 * B0 * x * z / (ra2 * ra3);
                f[6][k][j][i] = -3.0 * B0 * y * z / (ra2 * ra3);
                f[7][k][j][i] = B0 * (x * x + y * y - 2.0 * z * z) / (ra2 * ra3);
            }
        }
    }

    for (k = 0; k < NZ1; k++) {
        for (j = 0; j < NY1; j++) {
            for (i = 0; i < NX1; i++) {
                pp[2][k][j][i] =
                        0.25 * ((f[6][k + 1][j + 1][i + 1] + f[6][k + 1][j][i + 1] +
                                 f[6][k][j + 1][i + 1] + f[6][k][j][i + 1] -
                                 f[6][k + 1][j + 1][i] - f[6][k + 1][j][i] -
                                 f[6][k][j + 1][i] - f[6][k][j][i]) /
                                HX -
                                (f[5][k + 1][j + 1][i + 1] - f[5][k + 1][j][i + 1] +
                                 f[5][k][j + 1][i + 1] - f[5][k][j][i + 1] +
                                 f[5][k + 1][j + 1][i] - f[5][k + 1][j][i] +
                                 f[5][k][j + 1][i] - f[5][k][j][i]) /
                                HY);
                pp[1][k][j][i] =
                        0.25 * ((f[5][k + 1][j + 1][i + 1] + f[5][k + 1][j][i + 1] -
                                 f[5][k][j + 1][i + 1] - f[5][k][j][i + 1] +
                                 f[5][k + 1][j + 1][i] + f[5][k + 1][j][i] -
                                 f[5][k][j + 1][i] - f[5][k][j][i]) /
                                HZ -
                                (f[7][k + 1][j + 1][i + 1] + f[7][k + 1][j][i + 1] +
                                 f[7][k][j + 1][i + 1] + f[7][k][j][i + 1] -
                                 f[7][k + 1][j + 1][i] - f[7][k + 1][j][i] -
                                 f[7][k][j + 1][i] - f[7][k][j][i]) /
                                HX);
                pp[0][k][j][i] =
                        0.25 * ((f[7][k + 1][j + 1][i + 1] - f[7][k + 1][j][i + 1] +
                                 f[7][k][j + 1][i + 1] - f[7][k][j][i + 1] +
                                 f[7][k + 1][j + 1][i] - f[7][k + 1][j][i] +
                                 f[7][k][j + 1][i] - f[7][k][j][i]) /
                                HY -
                                (f[6][k + 1][j + 1][i + 1] + f[6][k + 1][j][i + 1] -
                                 f[6][k][j + 1][i + 1] - f[6][k][j][i + 1] +
                                 f[6][k + 1][j + 1][i] + f[6][k + 1][j][i] -
                                 f[6][k][j + 1][i] - f[6][k][j][i]) /
                                HZ);
            }
        }
    }
    for (k = 0; k < NZ2; k++) {
        for (j = 0; j < NY2; j++) {
            for (i = 0; i < NX2; i++) {
                x = 0.5 * HX * (2 * i - NX2 + 1 + 2 * NXP);
                y = 0.5 * HY * (2 * j - NY2 + 1);
                z = 0.5 * HZ * (2 * k - NZ2 + 1);
                xm1 = x - 2.0 * xm;
                ra2 = x * x + y * y + z * z;
                ra1 = sqrt(ra2);
                ra3 = ra1 * ra2;
                ra2m = xm1 * xm1 + y * y + z * z;
                ra1m = sqrt(ra2m);
                ra3m = ra1m * ra2m;
                f[0][k][j][i] = 1 / ra3;
                if (f[0][k][j][i] < RO02)
                    f[0][k][j][i] = RO02;
                if (f[0][k][j][i] > 1.0)
                    f[0][k][j][i] = 1.0;
                f[4][k][j][i] = PO0 / ra2;
                if (f[4][k][j][i] < PR02)
                    f[4][k][j][i] = PR02;
                if (f[4][k][j][i] > PO0)
                    f[4][k][j][i] = PO0;
                bx1 = -3.0 * B0 * x * z / (ra2 * ra3);
                by1 = -3.0 * B0 * y * z / (ra2 * ra3);
                bz1 = B0 * (x * x + y * y - 2.0 * z * z) / (ra2 * ra3);
                bx2 = -3.0 * B0 * xm1 * z / (ra2m * ra3m);
                by2 = -3.0 * B0 * y * z / (ra2m * ra3m);
                bz2 = B0 * (xm1 * xm1 + y * y - 2.0 * z * z) / (ra2m * ra3m);
                f[5][k][j][i] = bx1 + bx2;
                f[6][k][j][i] = by1 + by2;
                f[7][k][j][i] = bz1 + bz2;
                if (x < xm) {
                    f[5][k][j][i] = 0.0;
                    f[6][k][j][i] = 0.0;
                    f[7][k][j][i] = 0.0;
                }
                if (x < xm) {
                    f[5][k][j][i] = 0.0;
                    f[6][k][j][i] = 0.0;
                    f[7][k][j][i] = 0.0;
                }
                y1 = alp1 + (1.0 - alp1) * (x - xm) / (BET - 1.0) / xm;
                if (x > xm)
                    y1 = 0.0;
                y1 = fmax(y1, 0.0);
                y1 = fmin(y1, 1.0);
                vr1 = VSW * y1;
                f[1][k][j][i] = vr1;
                f[0][k][j][i] = f[0][k][j][i] + (RO01 - RO02) * y1;
                f[4][k][j][i] = f[4][k][j][i] + (PR01 - PR02) * y1;
            }
        }
    }
}

void bdry_z_m(float f[restrict NB][NZ2][NY2][NX2], float u[restrict NB][NZ2][NY2][NX2]) {
    float xx3, xx4, x, xx, y, z;
    int i, j, k, m;

    xx4 = 0.5 * HX * (2 * NXP - NX1 - 2);
    xx3 = HX * NX1 + xx4;

    for (m = 0; m < NB; m++) {
        for (j = 1; j < NY1; j++) {
            f[m][0][j][1] = f[m][1][j][0];
            f[m][NZ1][j][1] = f[m][NZ1 - 1][j][0];
            for (i = 3; i < NX1; i++) {
                x = HX * i + xx4;
                x = 1.0 - x / xx3;
                x = fmin(x, 1.0);
                x = 1.0 + x / 3.0;
                xx = 1.0 - x;
                f[m][0][j][i] = x * f[m][1][j][i - 1] + xx * f[m][2][j][i - 2];
                f[m][NZ1][j][i] =
                        x * f[m][NZ1 - 1][j][i - 1] + xx * f[m][NZ1 - 2][j][i - 2];
            }
        }
    }

    for (k = 0; k < NZ2; k++) {
        for (m = 0; m < NB; m++) {
            f[m][k][0][1] = f[m][k][1][0];
            f[m][k][NY1][1] = f[m][k][NY][0];
            for (i = 3; i < NX1; i++) {
                x = HX * i + xx4;
                x = 1.0 - x / xx3;
                x = fmin(x, 1.0);
                x = 1.0 + x / 3.0;
                xx = 1.0 - x;
                f[m][k][0][i] = x * f[m][k][1][i - 1] + xx * f[m][k][2][i - 2];
                f[m][k][NY1][i] =
                        x * f[m][k][NY][i - 1] + xx * f[m][k][NY - 1][i - 2];
            }

            for (j = 0; j < NY2; j++) {
                f[m][k][j][NX1] = f[m][k][j][NX];
            }
        }

        for (j = 0; j < NY2; j++) {
            for (i = 0; i < NX2; i++) {
                f[4][k][j][i] = fmax(f[4][k][j][i], PR02);
                f[0][k][j][i] = fmax(f[0][k][j][i], RO02);
                u[4][k][j][i] = fmax(u[4][k][j][i], PR02);
                u[0][k][j][i] = fmax(u[0][k][j][i], RO02);
            }
        }
    }
}

void bdry_up(float f[restrict NB][NZ2][NY2][NX2], float u[restrict NB][NZ2][NY2][NX2]) {
    int i, j, k;
    float x2 = 2.0;
    float x1 = 2.0 * x2;
    float x;

    for (k = 0; k < NZ2; k++) {
        for (j = 0; j < NY2; j++) {
            for (i = 0; i < NX2; i++) {
                x = (2.0 * HX * i / HX - (float) (2 + NX11)) / (float) NX11;
                x = (x1 + 1.0) * x + x1;
                x = -x;
                x = fmax(x, 0.0);
                x = fmin(x, 1.0);
                x = x * x;

                f[7][k][j][i] = (1.0 - x) * f[7][k][j][i] + IMF * x;
                f[6][k][j][i] = (1.0 - x) * f[6][k][j][i];
                f[5][k][j][i] = (1.0 - x) * f[5][k][j][i];
                f[4][k][j][i] = (1.0 - x) * f[4][k][j][i] + PR01 * x;
                f[3][k][j][i] = (1.0 - x) * f[3][k][j][i];
                f[2][k][j][i] = (1.0 - x) * f[2][k][j][i];
                f[1][k][j][i] = (1.0 - x) * f[1][k][j][i] + VSW * x;
                f[0][k][j][i] = (1.0 - x) * f[0][k][j][i] + RO01 * x;
            }
        }
    }

    for (k = 0; k < NZ1; k++) {
        for (j = 0; j < NY1; j++) {
            for (i = 0; i < NX1; i++) {
                x = (2.0 * HX * i / HX - (float) (2 + NX11)) / (float) NX11;
                x = (x1 + 1.0) * x + x1;
                x = -x;
                x = fmax(x, 0.0);
                x = fmin(x, 1.0);
                x = x * x;

                u[7][k][j][i] = (1.0 - x) * u[7][k][j][i] + IMF * x;
                u[6][k][j][i] = (1.0 - x) * u[6][k][j][i];
                u[5][k][j][i] = (1.0 - x) * u[5][k][j][i];
                u[4][k][j][i] = (1.0 - x) * u[4][k][j][i] + PR01 * x;
                u[3][k][j][i] = (1.0 - x) * u[3][k][j][i];
                u[2][k][j][i] = (1.0 - x) * u[2][k][j][i];
                u[1][k][j][i] = (1.0 - x) * u[1][k][j][i] + VSW * x;
                u[0][k][j][i] = (1.0 - x) * u[0][k][j][i] + RO01 * x;
            }
        }
    }
}

float vmaxf(float f[restrict NB][NZ2][NY2][NX2], float vmax) {
    int i, j, k;
    float vmax1;

    for (k = 0; k < NZ2; k++) {
        for (j = 0; j < NY2; j++) {
            for (i = 0; i < NX2; i++) {
                vmax1 = f[1][k][j][i] * f[1][k][j][i] +
                        f[2][k][j][i] * f[2][k][j][i] +
                        f[3][k][j][i] * f[3][k][j][i];
                vmax = fmax(vmax, vmax1);
            }
        }
    }

    return vmax;
}

void simuinfo() {
    printf("(NX2, NY2, NZ2) = (%d, %d, %d)\n", NX2, NY2, NZ2);
    printf(" HX,    THX, RA_I,      T\n");
    printf("%.1f %.5f %5.1f %.5f\n", HX, THX, RA_I, T);
    printf("\n");
}

void gflops(double zt) {
    double sx8r = 1.0 / 602.0 / 402.0 / 402.0 / 64.0 * 98.334 * 79.706;
    double flops = sx8r * NX2 * NY2 * NZ2 * LAST / zt;

    printf("Peak Performance\n");
    printf("Rmax = %.3f GFlops\n", flops);
    printf("\n");
}

int mlf(float f[restrict NB][NZ2][NY2][NX2],
        float u[restrict NB][NZ2][NY2][NX2], float pp[restrict NBBB][NZ2][NY2][NX2],
        int ii, int nmlf, float vmax) {
    float (*restrict ff)[NZ2][NY2][NX2] = (float (*)[NZ2][NY2][NX2]) malloc(NB * NZ2 * NY2 * NX2 * sizeof(float));
    float (*restrict p)[NZ2][NY2][NX2] = (float (*)[NZ2][NY2][NX2]) malloc(NBB * NZ2 * NY2 * NX2 * sizeof(float));
    float xn, z, y, x, ra2, ra3, x2, x5, div2_b, y5, y1, div_v;
    float t1, dx1, dy1, dz1, eat;
    int i, j, k, m;
    double bw, bt0, bt1;

    // First step
    if (ii == 1 || nmlf == NMLF0) {
        for (m = 0; m < NB; m++) {
            for (k = 0; k < NZ1; k++) {
                for (j = 0; j < NY1; j++) {
                    for (i = 0; i < NX1; i++) {
                        u[m][k][j][i] =
                                0.125 *
                                (f[m][k][j][i] + f[m][k][j][i + 1] + f[m][k][j + 1][i] +
                                 f[m][k][j + 1][i + 1] + f[m][k + 1][j][i] +
                                 f[m][k + 1][j][i + 1] + f[m][k + 1][j + 1][i] +
                                 f[m][k + 1][j + 1][i + 1]);
                    }
                }
            }
            t1 = TAM * T;
            dx1 = 0.25 * t1 / HX;
            dy1 = 0.25 * t1 / HY;
            dz1 = 0.25 * t1 / HZ;
        }
    }

    //main calculation
    for (k = 0; k < NZ1; k++) {
        for (j = 0; j < NY1; j++) {
            for (i = 0; i < NX1; i++) {
                p[10][k][j][i] =
                        0.25 * ((f[6][k + 1][j + 1][i + 1] + f[6][k + 1][j][i + 1] +
                                 f[6][k][j + 1][i + 1] + f[6][k][j][i + 1] -
                                 f[6][k + 1][j + 1][i] - f[6][k + 1][j][i] -
                                 f[6][k][j + 1][i] - f[6][k][j][i]) /
                                HX -
                                (f[5][k + 1][j + 1][i + 1] - f[5][k + 1][j][i + 1] +
                                 f[5][k][j + 1][i + 1] - f[5][k][j][i + 1] +
                                 f[5][k + 1][j + 1][i] - f[5][k + 1][j][i] +
                                 f[5][k][j + 1][i] - f[5][k][j][i]) /
                                HY);

                p[9][k][j][i] =
                        0.25 * ((f[5][k + 1][j + 1][i + 1] + f[5][k + 1][j][i + 1] -
                                 f[5][k][j + 1][i + 1] - f[5][k][j][i + 1] +
                                 f[5][k + 1][j + 1][i] + f[5][k + 1][j][i] -
                                 f[5][k][j + 1][i] - f[5][k][j][i]) /
                                HZ -
                                (f[7][k + 1][j + 1][i + 1] + f[7][k + 1][j][i + 1] +
                                 f[7][k][j + 1][i + 1] + f[7][k][j][i + 1] -
                                 f[7][k + 1][j + 1][i] - f[7][k + 1][j][i] -
                                 f[7][k][j + 1][i] - f[7][k][j][i]) /
                                HX);

                p[8][k][j][i] =
                        0.25 * ((f[7][k + 1][j + 1][i + 1] - f[7][k + 1][j][i + 1] +
                                 f[7][k][j + 1][i + 1] - f[7][k][j][i + 1] +
                                 f[7][k + 1][j + 1][i] - f[7][k + 1][j][i] +
                                 f[7][k][j + 1][i] - f[7][k][j][i]) /
                                HY - //

                                (f[6][k + 1][j + 1][i + 1] + f[6][k + 1][j][i + 1] -
                                 f[6][k][j + 1][i + 1] - f[6][k][j][i + 1] +
                                 f[6][k + 1][j + 1][i] + f[6][k + 1][j][i] -
                                 f[6][k][j + 1][i] - f[6][k][j][i]) /
                                HZ);

            }
        }
        for (j = 0; j < NY1; j++) {
            for (i = 0; i < NX1; i++) {
                p[10][k][j][i] = p[10][k][j][i] - pp[2][k][j][i];
                p[9][k][j][i] = p[9][k][j][i] - pp[1][k][j][i];
                p[8][k][j][i] = p[8][k][j][i] - pp[0][k][j][i];
            }
        }
        //
        for (m = 0; m < NB; m++) {
            for (j = 0; j < NY1; j++) {
                for (i = 0; i < NX1; i++) {
                    p[m][k][j][i] =
                            0.125 * (f[m][k][j][i] + f[m][k][j][i + 1] +
                                     f[m][k][j + 1][i] + f[m][k][j + 1][i + 1] +
                                     f[m][k + 1][j][i] + f[m][k + 1][j][i + 1] +
                                     f[m][k + 1][j + 1][i] + f[m][k + 1][j + 1][i + 1]);
                }
            }
        }
        z = 0.5 * HZ * (2.0 * k - NZ2 + 2);
        for (j = 0; j < NY1; j++) {
            y = 0.5 * HY * (2.0 * j - NY2 + 2);
            for (m = 0; m < NB; m++) {
                for (i = 0; i < NX1; i++) {
                    ff[m][k][j][i] = u[m][k][j][i];
                }
            }

            for (i = 0; i < NX1; i++) {
                x = 0.5 * HX * (2.0 * i - NX2 + 2 * NXP + 2);
                ra2 = x * x + y * y + z * z;
                ra2 = fmax(ra2, RA_I2);
                ra3 = ra2 * sqrtf(ra2);
                u[7][k][j][i] +=
                        dx1 * (f[3][k + 1][j + 1][i + 1] * f[5][k + 1][j + 1][i + 1] -
                               f[1][k + 1][j + 1][i + 1] * f[7][k + 1][j + 1][i + 1] +
                               f[3][k + 1][j][i + 1] * f[5][k + 1][j][i + 1] -
                               f[1][k + 1][j][i + 1] * f[7][k + 1][j][i + 1] +
                               f[3][k][j + 1][i + 1] * f[5][k][j + 1][i + 1] -
                               f[1][k][j + 1][i + 1] * f[7][k][j + 1][i + 1] +
                               f[3][k][j][i + 1] * f[5][k][j][i + 1] -
                               f[1][k][j][i + 1] * f[7][k][j][i + 1]) -
                        dy1 * (f[2][k + 1][j + 1][i + 1] * f[7][k + 1][j + 1][i + 1] -
                               f[3][k + 1][j + 1][i + 1] * f[6][k + 1][j + 1][i + 1] -
                               f[2][k + 1][j][i + 1] * f[7][k + 1][j][i + 1] +
                               f[3][k + 1][j][i + 1] * f[6][k + 1][j][i + 1] +
                               f[2][k + 1][j + 1][i] * f[7][k][j + 1][i + 1] -
                               f[3][k][j + 1][i + 1] * f[6][k][j + 1][i + 1] -
                               f[2][k][j][i + 1] * f[7][k][j][i + 1] +
                               f[3][k][j][i + 1] * f[6][k][j][i + 1]);

                u[6][k][j][i] +=
                        dz1 * (f[2][k + 1][j + 1][i + 1] * f[7][k + 1][j + 1][i + 1] -
                               f[3][k + 1][j + 1][i + 1] * f[6][k + 1][j + 1][i + 1] +
                               f[2][k + 1][j][i + 1] * f[7][k + 1][j][i + 1] -
                               f[3][k + 1][j][i + 1] * f[6][k + 1][j][i + 1] -
                               f[2][k + 1][j + 1][i] * f[7][k][j + 1][i + 1] +
                               f[3][k][j + 1][i + 1] * f[6][k][j + 1][i + 1] -
                               f[2][k][j][i + 1] * f[7][k][j][i + 1] +
                               f[3][k][j][i + 1] * f[6][k][j][i + 1]) -
                        dx1 * (f[1][k + 1][j + 1][i + 1] * f[6][k + 1][j + 1][i + 1] -
                               f[2][k + 1][j + 1][i + 1] * f[5][k + 1][j + 1][i + 1] +
                               f[1][k + 1][j][i + 1] * f[6][k + 1][j][i + 1] -
                               f[2][k + 1][j][i + 1] * f[5][k + 1][j][i + 1] +
                               f[1][k][j + 1][i + 1] * f[6][k][j + 1][i + 1] -
                               f[2][k + 1][j + 1][i] * f[5][k][j + 1][i + 1] +
                               f[1][k][j][i + 1] * f[6][k][j][i + 1] -
                               f[2][k][j][i + 1] * f[5][k][j][i + 1]);

                u[5][k][j][i] +=
                        dy1 * (f[1][k + 1][j + 1][i + 1] * f[6][k + 1][j + 1][i + 1] -
                               f[2][k + 1][j + 1][i + 1] * f[5][k + 1][j + 1][i + 1] -
                               f[1][k + 1][j][i + 1] * f[6][k + 1][j][i + 1] +
                               f[2][k + 1][j][i + 1] * f[5][k + 1][j][i + 1] +
                               f[1][k][j + 1][i + 1] * f[6][k][j + 1][i + 1] -
                               f[2][k + 1][j + 1][i] * f[5][k][j + 1][i + 1] -
                               f[1][k][j][i + 1] * f[6][k][j][i + 1] +
                               f[2][k][j][i + 1] * f[5][k][j][i + 1] +
                               f[1][k + 1][j + 1][i] * f[6][k + 1][j + 1][i] -
                               f[2][k + 1][j + 1][i] * f[5][k + 1][j + 1][i] -
                               f[1][k + 1][j][i] * f[6][k + 1][j][i] +
                               f[2][k + 1][j][i] * f[5][k + 1][j][i] +
                               f[1][k][j + 1][i] * f[6][k][j + 1][i] -
                               f[2][k][j + 1][i] * f[5][k][j + 1][i] -
                               f[1][k][j][i] * f[6][k][j][i] +
                               f[2][k][j][i] * f[5][k][j][i]) -
                        dz1 * (f[3][k + 1][j + 1][i + 1] * f[5][k + 1][j + 1][i + 1] -
                               f[1][k + 1][j + 1][i + 1] * f[7][k + 1][j + 1][i + 1] +
                               f[3][k + 1][j][i + 1] * f[5][k + 1][j][i + 1] -
                               f[1][k + 1][j][i + 1] * f[7][k + 1][j][i + 1] -
                               f[3][k][j + 1][i + 1] * f[5][k][j + 1][i + 1] +
                               f[1][k][j + 1][i + 1] * f[7][k][j + 1][i + 1] -
                               f[3][k][j][i + 1] * f[5][k][j][i + 1] +
                               f[1][k][j][i + 1] * f[7][k][j][i + 1] +
                               f[3][k + 1][j + 1][i] * f[5][k + 1][j + 1][i] -
                               f[1][k + 1][j + 1][i] * f[7][k + 1][j + 1][i] +
                               f[3][k + 1][j][i] * f[5][k + 1][j][i] -
                               f[1][k + 1][j][i] * f[7][k + 1][j][i] -
                               f[3][k][j + 1][i] * f[5][k][j + 1][i] +
                               f[1][k][j + 1][i] * f[7][k][j + 1][i] -
                               f[3][k][j][i] * f[5][k][j][i] +
                               f[1][k][j][i] * f[7][k][j][i]);

                div_v = dx1 * (f[1][k + 1][j + 1][i + 1] + f[1][k + 1][j][i + 1] +
                               f[1][k][j + 1][i + 1] + f[1][k][j][i + 1] -
                               f[1][k + 1][j + 1][i] - f[1][k + 1][j][i] -
                               f[1][k][j + 1][i] - f[1][k][j][i]) +
                        dy1 * (f[2][k + 1][j + 1][i + 1] - f[2][k + 1][j][i + 1] +
                               f[2][k + 1][j + 1][i] - f[2][k][j][i + 1] +
                               f[2][k + 1][j + 1][i] - f[2][k + 1][j][i] +
                               f[2][k][j + 1][i] - f[2][k][j][i]) +
                        dz1 * (f[3][k + 1][j + 1][i + 1] + f[3][k + 1][j][i + 1] -
                               f[3][k][j + 1][i + 1] - f[3][k][j][i + 1] +
                               f[3][k + 1][j + 1][i] + f[3][k + 1][j][i] -
                               f[3][k][j + 1][i] - f[3][k][j][i]);

                u[4][k][j][i] +=
                        -p[4][k][j][i] * GAM * div_v -
                        dx1 * p[1][k][j][i] *
                        (f[4][k + 1][j + 1][i + 1] + f[4][k + 1][j][i + 1] +
                         f[4][k][j + 1][i + 1] + f[4][k][j][i + 1] -
                         f[4][k + 1][j + 1][i] - f[4][k + 1][j][i] -
                         f[4][k][j + 1][i] - f[4][k][j][i]) -
                        dy1 * p[2][k][j][i] *
                        (f[4][k + 1][j + 1][i + 1] - f[4][k + 1][j][i + 1] +
                         f[4][k][j + 1][i + 1] - f[4][k][j][i + 1] +
                         f[4][k + 1][j + 1][i] - f[4][k + 1][j][i] +
                         f[4][k][j + 1][i] - f[4][k][j][i]) -
                        dz1 * p[3][k][j][i] *
                        (f[4][k + 1][j + 1][i + 1] + f[4][k + 1][j][i + 1] -
                         f[4][k][j + 1][i + 1] - f[4][k][j][i + 1] +
                         f[4][k + 1][j + 1][i] + f[4][k + 1][j][i] -
                         f[4][k][j + 1][i] - f[4][k][j][i]);

                u[3][k][j][i] +=
                        t1 * ((p[8][k][j][i] * p[6][k][j][i] -
                               p[9][k][j][i] * p[5][k][j][i]) /
                              p[0][k][j][i] -
                              GRA * z / ra3) -
                        dx1 * p[1][k][j][i] *
                        (f[3][k + 1][j + 1][i + 1] + f[3][k + 1][j][i + 1] +
                         f[3][k][j + 1][i + 1] + f[3][k][j][i + 1] -
                         f[3][k + 1][j + 1][i] - f[3][k + 1][j][i] -
                         f[3][k][j + 1][i] - f[3][k][j][i]) -
                        dy1 * p[2][k][j][i] *
                        (f[3][k + 1][j + 1][i + 1] - f[3][k + 1][j][i + 1] +
                         f[3][k][j + 1][i + 1] - f[3][k][j][i + 1] +
                         f[3][k + 1][j + 1][i] - f[3][k + 1][j][i] +
                         f[3][k][j + 1][i] - f[3][k][j][i]) -
                        dz1 * p[3][k][j][i] *
                        (f[3][k + 1][j + 1][i + 1] + f[3][k + 1][j][i + 1] -
                         f[3][k][j + 1][i + 1] - f[3][k][j][i + 1] +
                         f[3][k + 1][j + 1][i] + f[3][k + 1][j][i] -
                         f[3][k][j + 1][i] - f[3][k][j][i]) -
                        dz1 *
                        (f[4][k + 1][j + 1][i + 1] + f[4][k + 1][j][i + 1] -
                         f[4][k][j + 1][i + 1] - f[4][k][j][i + 1] +
                         f[4][k + 1][j + 1][i] + f[4][k + 1][j][i] -
                         f[4][k][j + 1][i] - f[4][k][j][i]) /
                        p[0][k][j][i];

                u[2][k][j][i] +=
                        t1 * ((p[10][k][j][i] * p[5][k][j][i] -
                               p[8][k][j][i] * p[7][k][j][i]) /
                              p[0][k][j][i] -
                              GRA * x / ra3) -
                        dx1 * p[1][k][j][i] *
                        (f[2][k + 1][j + 1][i + 1] + f[2][k + 1][j][i + 1] +
                         f[2][k + 1][j + 1][i] + f[2][k][j][i + 1] -
                         f[2][k + 1][j + 1][i] - f[2][k + 1][j][i] -
                         f[2][k][j + 1][i] - f[2][k][j][i]) -
                        dy1 * p[2][k][j][i] *
                        (f[2][k + 1][j + 1][i + 1] - f[2][k + 1][j][i + 1] +
                         f[2][k + 1][j + 1][i] - f[2][k][j][i + 1] +
                         f[2][k + 1][j + 1][i] - f[2][k + 1][j][i] +
                         f[2][k][j + 1][i] - f[2][k][j][i]) -
                        dz1 * p[3][k][j][i] *
                        (f[2][k + 1][j + 1][i + 1] + f[2][k + 1][j][i + 1] -
                         f[2][k + 1][j + 1][i] - f[2][k][j][i + 1] +
                         f[2][k + 1][j + 1][i] + f[2][k + 1][j][i] -
                         f[2][k][j + 1][i] - f[2][k][j][i]) -
                        dx1 *
                        (f[4][k + 1][j + 1][i + 1] + f[4][k + 1][j][i + 1] +
                         f[4][k][j + 1][i + 1] + f[4][k][j][i + 1] -
                         f[4][k + 1][j + 1][i] - f[4][k + 1][j][i] -
                         f[4][k][j + 1][i] - f[4][k][j][i]) /
                        p[0][k][j][i];

                u[1][k][j][i] +=
                        t1 * ((p[9][k][j][i] * p[7][k][j][i] -
                               p[10][k][j][i] * p[6][k][j][i]) /
                              p[0][k][j][i] -
                              GRA * x / ra3) -
                        dx1 * p[1][k][j][i] *
                        (f[1][k + 1][j + 1][i + 1] + f[1][k + 1][j][i + 1] +
                         f[1][k][j + 1][i + 1] + f[1][k][j][i + 1] -
                         f[1][k + 1][j + 1][i] - f[1][k + 1][j][i] -
                         f[1][k][j + 1][i] - f[1][k][j][i]) -
                        dy1 * p[2][k][j][i] *
                        (f[1][k + 1][j + 1][i + 1] - f[1][k + 1][j][i + 1] +
                         f[1][k][j + 1][i + 1] - f[1][k][j][i + 1] +
                         f[1][k + 1][j + 1][i] - f[1][k + 1][j][i] +
                         f[2][k][j + 1][i] - f[1][k][j][i]) -
                        dz1 * p[3][k][j][i] *
                        (f[1][k + 1][j + 1][i + 1] + f[1][k + 1][j][i + 1] -
                         f[1][k][j + 1][i + 1] - f[1][k][j][i + 1] +
                         f[1][k + 1][j + 1][i] + f[1][k + 1][j][i] -
                         f[1][k][j + 1][i] - f[1][k][j][i]) -
                        dx1 *
                        (f[4][k + 1][j + 1][i + 1] + f[4][k + 1][j][i + 1] +
                         f[4][k][j + 1][i + 1] + f[4][k][j][i + 1] -
                         f[4][k + 1][j + 1][i] - f[4][k + 1][j][i] -
                         f[4][k][j + 1][i] - f[4][k][j][i]) /
                        p[0][k][j][i];

                u[0][k][j][i] +=
                        -dx1 * (f[1][k + 1][j + 1][i + 1] * f[0][k + 1][j + 1][i + 1] +
                                f[1][k + 1][j][i + 1] * f[0][k + 1][j][i + 1] +
                                f[1][k][j + 1][i + 1] * f[0][k][j + 1][i + 1] +
                                f[1][k][j][i + 1] * f[0][k][j][i + 1] -
                                f[1][k + 1][j + 1][i] * f[0][k + 1][j + 1][i] -
                                f[1][k + 1][j][i] * f[0][k + 1][j][i] -
                                f[1][k][j + 1][i] * f[0][k][j + 1][i] -
                                f[1][k][j][i] * f[0][k][j][i]) -
                        dy1 * (f[2][k + 1][j + 1][i + 1] * f[0][k + 1][j + 1][i + 1] -
                               f[2][k + 1][j][i + 1] * f[0][k + 1][j][i + 1] +
                               f[2][k + 1][j + 1][i] * f[0][k][j + 1][i + 1] -
                               f[2][k][j][i + 1] * f[0][k][j][i + 1] +
                               f[2][k + 1][j + 1][i] * f[0][k + 1][j + 1][i] -
                               f[2][k + 1][j][i] * f[0][k + 1][j][i] +
                               f[2][k][j + 1][i] * f[0][k][j + 1][i] -
                               f[2][k][j][i] * f[0][k][j][i]) -
                        dz1 * (f[3][k + 1][j + 1][i + 1] * f[0][k + 1][j + 1][i + 1] +
                               f[3][k + 1][j][i + 1] * f[0][k + 1][j][i + 1] -
                               f[3][k][j + 1][i + 1] * f[0][k][j + 1][i + 1] -
                               f[3][k][j][i + 1] * f[0][k][j][i + 1] +
                               f[3][k + 1][j + 1][i] * f[0][k + 1][j + 1][i] +
                               f[3][k + 1][j][i] * f[0][k + 1][j][i] -
                               f[3][k][j + 1][i] * f[0][k][j + 1][i] -
                               f[3][k][j][i] * f[0][k][j][i]);
            }

            for (i = 0; i < NX1; i++) {
                x = 0.5 * HX * (2 * i - NX2 + 1 + 2 * NXP);
                ra2 = x * x + y * y + z * z;
                x2 = fmax(ra2 - RA_I2, 0.0);
                x2 = ARU * x2 * x2;
                x2 = x2 / (x2 + 1.0);
                ra2 = fmax(ra2, RA_I2);
                ra3 = ra2 * sqrt(ra2);

                u[7][k][j][i] = u[7][k][j][i] * x2 + ff[7][k][j][i] * (1.0 - x2);
                u[6][k][j][i] = u[6][k][j][i] * x2 + ff[6][k][j][i] * (1.0 - x2);
                u[5][k][j][i] = u[5][k][j][i] * x2 + ff[5][k][j][i] * (1.0 - x2);
                u[4][k][j][i] = u[4][k][j][i] * x2 + ff[4][k][j][i] * (1.0 - x2);
                u[3][k][j][i] = u[3][k][j][i] * x2 + ff[3][k][j][i] * (1.0 - x2);
                u[2][k][j][i] = u[2][k][j][i] * x2 + ff[2][k][j][i] * (1.0 - x2);
                u[1][k][j][i] = u[1][k][j][i] * x2 + ff[1][k][j][i] * (1.0 - x2);
                u[0][k][j][i] = u[0][k][j][i] * x2 + ff[0][k][j][i] * (1.0 - x2);
            }
        }
    }

    bt0 = clock();
    for (m = 0; m < NB; m++) {
        for (k = 0; k < NZ2; k++) {
            for (j = 0; j < NY2; j++) {
                for (i = 0; i < NX2; i++) {
                    ff[m][k][j][i] = f[m][k][j][i];
                }
            }
        }
    }
    bt1 = clock();
    bw = N4 / ((double) (bt1 - bt0) / CLOCKS_PER_SEC) * 4 / 1024 / 1024 / 1024;
    printf("Bandwidth: %10.3f GB/s\n", bw);

    for (k = 1; k < NZ1; k++) {
        for (j = 1; j < NY1; j++) {
            for (i = 1; i < NX1; i++) {
                p[10][k][j][i] =
                        0.25 * ((u[6][k][j][i] + u[6][k][j - 1][i] + u[6][k - 1][j][i] +
                                 u[6][k - 1][j - 1][i] - u[6][k][j][i - 1] -
                                 u[6][k][j - 1][i - 1] - u[6][k - 1][j][i - 1] -
                                 u[6][k - 1][j - 1][i - 1]) /
                                HX -
                                (u[5][k][j][i] - u[5][k][j - 1][i] + u[5][k - 1][j][i] -
                                 u[5][k - 1][j - 1][i] + u[5][k][j][i - 1] -
                                 u[5][k][j - 1][i - 1] + u[5][k - 1][j][i - 1] -
                                 u[5][k - 1][j - 1][i - 1]) /
                                HY);

                p[9][k][j][i] =
                        0.25 * ((u[5][k][j][i] + u[5][k][j - 1][i] - u[5][k - 1][j][i] -
                                 u[5][k - 1][j - 1][i] + u[5][k][j][i - 1] +
                                 u[5][k][j - 1][i - 1] - u[5][k - 1][j][i - 1] -
                                 u[5][k - 1][j - 1][i - 1]) /
                                HZ -
                                (u[7][k][j][i] + u[7][k][j - 1][i] + u[7][k][j][i - 1] +
                                 u[7][k - 1][j - 1][i] - u[7][k][j][i - 1] -
                                 u[7][k][j - 1][i - 1] - u[7][k - 1][j][i - 1] -
                                 u[7][k - 1][j - 1][i - 1]) /
                                HX);

                p[8][k][j][i] =
                        0.25 * ((u[7][k][j][i] - u[7][k][j - 1][i] + u[7][k][j][i - 1] -
                                 u[7][k - 1][j - 1][i] + u[7][k][j][i - 1] -
                                 u[7][k][j - 1][i - 1] + u[7][k - 1][j][i - 1] -
                                 u[7][k - 1][j - 1][i - 1]) /
                                HY -
                                (u[6][k][j][i] + u[6][k][j - 1][i] - u[6][k - 1][j][i] -
                                 u[6][k - 1][j - 1][i] + u[6][k][j][i - 1] +
                                 u[6][k][j - 1][i - 1] - u[6][k - 1][j][i - 1] -
                                 u[6][k - 1][j - 1][i - 1]) /
                                HZ);
            }
        }

        for (j = 1; j < NY1; j++) {
            for (i = 1; i < NX1; i++) {
                p[10][k][j][i] -=
                        0.125 * (pp[2][k][j][i] + pp[2][k][j][i - 1] +
                                 pp[2][k][j - 1][i] + pp[2][k][j - 1][i - 1] +
                                 pp[2][k][j][i - 1] + pp[2][k - 1][j][i - 1] +
                                 pp[2][k - 1][j - 1][i] + pp[2][k - 1][j - 1][i - 1]);

                p[9][k][j][i] -=
                        0.125 * (pp[1][k][j][i] + pp[1][k][j][i - 1] +
                                 pp[1][k][j - 1][i] + pp[1][k][j - 1][i - 1] +
                                 pp[1][k - 1][j][i] + pp[1][k - 1][j][i - 1] +
                                 pp[1][k - 1][j - 1][i] + pp[1][k - 1][j - 1][i - 1]);

                p[8][k][j][i] -=
                        0.125 * (pp[0][k][j][i] + pp[0][k][j][i - 1] +
                                 pp[0][k][j - 1][i] + pp[0][k][j - 1][i - 1] +
                                 pp[0][k - 1][j][i] + pp[0][k - 1][j][i - 1] +
                                 pp[0][k - 1][j - 1][i] + pp[0][k - 1][j - 1][i - 1]);
            }
        }

        for (m = 0; m < NB; m++) {
            for (j = 1; j < NY1; j++) {
                for (i = 1; i < NX1; i++) {
                    p[m][k][j][i] =
                            0.125 * (u[m][k][j][i] + u[m][k][j][i - 1] +
                                     u[m][k][j - 1][i] + u[m][k][j - 1][i - 1] +
                                     u[m][k - 1][j][i] + u[m][k - 1][j][i - 1] +
                                     u[m][k - 1][j - 1][i] + u[m][k - 1][j - 1][i - 1]);
                }
            }
        }

        z = 0.5 * HZ * (2 * k - NZ2 + 1);
        for (j = 1; j < NY1; j++) {
            y = 0.5 * HY * (2 * j - NY2 + 1);
            for (i = 1; i < NX1; i++) {
                x = 0.5 * HX * (2 * i - NX2 + 1 + 2 * NXP);
                ra2 = x * x + y * y + z * z;
                ra2 = fmax(ra2, RA_I2);
                ra3 = ra2 * sqrt(ra2);

                x2 = fmax(ra2 / RA_I2 - 1.0, 0.0);
                x2 = ARU * x2 * x2;
                x2 = x2 / (x2 + 1.0);
                eat = EATT * x2;

                div2_b = DX4 * (ff[7][k][j][i + 1] - 2.0 * ff[7][k][j][i] +
                                ff[7][k][j][i - 1]) +
                         DY4 * (ff[7][k][j + 1][i] - 2.0 * ff[7][k][j][i] +
                                ff[7][k][j - 1][i]) +
                         DZ4 * (ff[7][k + 1][j][i] - 2.0 * ff[7][k][j][i] +
                                ff[7][k][j][i - 1]);

                f[7][k][j][i] =
                        ff[7][k][j][i] + eat * div2_b +
                        DX2 * (u[3][k][j][i] * u[5][k][j][i] -
                               u[1][k][j][i] * u[7][k][j][i] +
                               u[3][k][j - 1][i] * u[5][k][j - 1][i] -
                               u[1][k][j - 1][i] * u[7][k][j - 1][i] +
                               u[3][k - 1][j][i] * u[5][k - 1][j][i] -
                               u[1][k - 1][j][i] * u[7][k][j][i - 1] +
                               u[3][k - 1][j - 1][i] * u[5][k - 1][j - 1][i] -
                               u[1][k - 1][j - 1][i] * u[7][k - 1][j - 1][i] -
                               u[3][k][j][i - 1] * u[5][k][j][i - 1] +
                               u[1][k][j][i - 1] * u[7][k][j][i - 1] -
                               u[3][k][j - 1][i - 1] * u[5][k][j - 1][i - 1] +
                               u[1][k][j - 1][i - 1] * u[7][k][j - 1][i - 1] -
                               u[3][k - 1][j][i - 1] * u[5][k - 1][j][i - 1] +
                               u[1][k - 1][j][i - 1] * u[7][k - 1][j][i - 1] -
                               u[3][k - 1][j - 1][i - 1] * u[5][k - 1][j - 1][i - 1] +
                               u[1][k - 1][j - 1][i - 1] * u[7][k - 1][j - 1][i - 1]) -
                        DY2 * (u[2][k][j][i] * u[7][k][j][i] -
                               u[3][k][j][i] * u[6][k][j][i] +
                               u[2][k][j - 1][i] * u[7][k][j - 1][i] -
                               u[3][k][j - 1][i] * u[6][k][j - 1][i] +
                               u[2][k][j][i - 1] * u[7][k][j][i - 1] -
                               u[3][k - 1][j][i] * u[6][k - 1][j][i] +
                               u[2][k - 1][j - 1][i] * u[7][k - 1][j - 1][i] -
                               u[3][k - 1][j - 1][i] * u[6][k - 1][j - 1][i] -
                               u[2][k][j][i - 1] * u[7][k][j][i - 1] +
                               u[3][k][j][i - 1] * u[6][k][j][i - 1] -
                               u[2][k][j - 1][i - 1] * u[7][k][j - 1][i - 1] +
                               u[3][k][j - 1][i - 1] * u[6][k][j - 1][i - 1] -
                               u[2][k - 1][j][i - 1] * u[7][k - 1][j][i - 1] +
                               u[3][k - 1][j][i - 1] * u[6][k - 1][j][i - 1] -
                               u[2][k - 1][j - 1][i - 1] * u[7][k - 1][j - 1][i - 1] +
                               u[3][k - 1][j - 1][i - 1] * u[6][k - 1][j - 1][i - 1]);

                div2_b = DX4 * (ff[6][k][j][i + 1] - 2.0 * ff[6][k][j][i] +
                                ff[6][k][j][i - 1]) +
                         DY4 * (ff[6][k][j + 1][i] - 2.0 * ff[6][k][j][i] +
                                ff[6][k][j - 1][i]) +
                         DZ4 * (ff[6][k + 1][j][i] - 2.0 * ff[6][k][j][i] +
                                ff[6][k - 1][j][i]);

                f[6][k][j][i] =
                        ff[6][k][j][i] + eat * div2_b +
                        DZ2 * (u[2][k][j][i] * u[7][k][j][i] -
                               u[3][k][j][i] * u[6][k][j][i] +
                               u[2][k][j - 1][i] * u[7][k][j - 1][i] -
                               u[3][k][j - 1][i] * u[6][k][j - 1][i] -
                               u[2][k][j][i - 1] * u[7][k][j][i - 1] +
                               u[3][k - 1][j][i] * u[6][k - 1][j][i] -
                               u[2][k - 1][j - 1][i] * u[7][k - 1][j - 1][i] +
                               u[3][k - 1][j - 1][i] * u[6][k - 1][j - 1][i] +
                               u[2][k][j][i - 1] * u[7][k][j][i - 1] -
                               u[3][k][j][i - 1] * u[6][k][j][i - 1] +
                               u[2][k][j - 1][i - 1] * u[7][k][j - 1][i - 1] -
                               u[3][k][j - 1][i - 1] * u[6][k][j - 1][i - 1] -
                               u[2][k - 1][j][i - 1] * u[7][k - 1][j][i - 1] +
                               u[3][k - 1][j][i - 1] * u[6][k - 1][j][i - 1] -
                               u[2][k - 1][j - 1][i - 1] * u[7][k - 1][j - 1][i - 1] +
                               u[3][k - 1][j - 1][i - 1] * u[6][k - 1][j - 1][i - 1]) -
                        DX2 * (u[1][k][j][i] * u[6][k][j][i] -
                               u[2][k][j][i] * u[5][k][j][i] +
                               u[1][k][j - 1][i] * u[6][k][j - 1][i] -
                               u[2][k][j - 1][i] * u[5][k][j - 1][i] +
                               u[1][k - 1][j][i] * u[6][k - 1][j][i] -
                               u[2][k][j][i - 1] * u[5][k - 1][j][i] +
                               u[1][k - 1][j - 1][i] * u[6][k - 1][j - 1][i] -
                               u[2][k - 1][j - 1][i] * u[5][k - 1][j - 1][i] -
                               u[1][k][j][i - 1] * u[6][k][j][i - 1] +
                               u[2][k][j][i - 1] * u[5][k][j][i - 1] -
                               u[1][k][j - 1][i - 1] * u[6][k][j - 1][i - 1] +
                               u[2][k][j - 1][i - 1] * u[5][k][j - 1][i - 1] -
                               u[1][k - 1][j][i - 1] * u[6][k - 1][j][i - 1] +
                               u[2][k - 1][j][i - 1] * u[5][k - 1][j][i - 1] -
                               u[1][k - 1][j - 1][i - 1] * u[6][k - 1][j - 1][i - 1] +
                               u[2][k - 1][j - 1][i - 1] * u[5][k - 1][j - 1][i - 1]);

                div2_b = DX4 * (ff[5][k][j][i + 1] - 2.0 * ff[5][k][j][i] +
                                ff[5][k][j][i - 1]) +
                         DY4 * (ff[5][k][j + 1][i] - 2.0 * ff[5][k][j][i] +
                                ff[5][k][j - 1][i]) +
                         DZ4 * (ff[5][k + 1][j][i] - 2.0 * ff[5][k][j][i] +
                                ff[5][k - 1][j][i]);

                f[5][k][j][i] =
                        ff[5][k][j][i] + eat * div2_b +
                        DY2 * (u[1][k][j][i] * u[6][k][j][i] -
                               u[2][k][j][i] * u[5][k][j][i] -
                               u[1][k][j - 1][i] * u[6][k][j - 1][i] +
                               u[2][k][j - 1][i] * u[5][k][j - 1][i] +
                               u[1][k - 1][j][i] * u[6][k - 1][j][i] -
                               u[2][k][j][i - 1] * u[5][k - 1][j][i] -
                               u[1][k - 1][j - 1][i] * u[6][k - 1][j - 1][i] +
                               u[2][k - 1][j - 1][i] * u[5][k - 1][j - 1][i] +
                               u[1][k][j][i - 1] * u[6][k][j][i - 1] -
                               u[2][k][j][i - 1] * u[5][k][j][i - 1] -
                               u[1][k][j - 1][i - 1] * u[6][k][j - 1][i - 1] +
                               u[2][k][j - 1][i - 1] * u[5][k][j - 1][i - 1] +
                               u[1][k - 1][j][i - 1] * u[6][k - 1][j][i - 1] -
                               u[2][k - 1][j][i - 1] * u[5][k - 1][j][i - 1] -
                               u[1][k - 1][j - 1][i - 1] * u[6][k - 1][j - 1][i - 1] +
                               u[2][k - 1][j - 1][i - 1] * u[5][k - 1][j - 1][i - 1]) -
                        DZ2 * (u[3][k][j][i] * u[5][k][j][i] -
                               u[1][k][j][i] * u[7][k][j][i] +
                               u[3][k][j - 1][i] * u[5][k][j - 1][i] -
                               u[1][k][j - 1][i] * u[7][k][j - 1][i] -
                               u[3][k - 1][j][i] * u[5][k - 1][j][i] +
                               u[1][k - 1][j][i] * u[7][k][j][i - 1] +
                               u[3][k - 1][j - 1][i] * u[5][k - 1][j - 1][i] -
                               u[1][k - 1][j - 1][i] * u[7][k - 1][j - 1][i] -
                               u[3][k][j][i - 1] * u[5][k][j][i - 1] +
                               u[1][k][j][i - 1] * u[7][k][j][i - 1] -
                               u[3][k][j - 1][i - 1] * u[5][k][j - 1][i - 1] +
                               u[1][k][j - 1][i - 1] * u[7][k][j - 1][i - 1] +
                               u[3][k - 1][j][i - 1] * u[5][k - 1][j][i - 1] -
                               u[1][k - 1][j][i - 1] * u[7][k - 1][j][i - 1] -
                               u[3][k - 1][j - 1][i - 1] * u[5][k - 1][j - 1][i - 1] +
                               u[1][k - 1][j - 1][i - 1] * u[7][k - 1][j - 1][i - 1]);

                div_v = DX2 * (u[1][k][j][i] + u[1][k][j - 1][i] +
                               u[1][k - 1][j][i] + u[1][k - 1][j - 1][i] -
                               u[1][k][j][i - 1] - u[1][k][j - 1][i - 1] -
                               u[1][k - 1][j][i - 1] - u[1][k - 1][j - 1][i - 1]) +
                        DY2 * (u[2][k][j][i] - u[2][k][j - 1][i] +
                               u[2][k][j][i - 1] - u[2][k - 1][j - 1][i] +
                               u[2][k][j][i - 1] - u[2][k][j - 1][i - 1] +
                               u[2][k - 1][j][i - 1] - u[2][k - 1][j - 1][i - 1]) +
                        DZ2 * (u[3][k][j][i] + u[3][k][j - 1][i] -
                               u[3][k - 1][j][i] - u[3][k - 1][j - 1][i] +
                               u[3][k][j][i - 1] + u[3][k][j - 1][i - 1] -
                               u[3][k - 1][j][i - 1] - u[3][k - 1][j - 1][i - 1]);

                f[4][k][j][i] =
                        ff[4][k][j][i] - p[4][k][j][i] * GAM * div_v -
                        DX2 * p[1][k][j][i] *
                        (u[4][k][j][i] + u[4][k][j - 1][i] + u[4][k - 1][j][i] +
                         u[4][k - 1][j - 1][i] - u[4][k][j][i - 1] -
                         u[4][k][j - 1][i - 1] - u[4][k - 1][j][i - 1] -
                         u[4][k - 1][j - 1][i - 1]) -
                        DY2 * p[2][k][j][i] *
                        (u[4][k][j][i] - u[4][k][j - 1][i] + u[4][k - 1][j][i] -
                         u[4][k - 1][j - 1][i] + u[4][k][j][i - 1] -
                         u[4][k][j - 1][i - 1] + u[4][k - 1][j][i - 1] -
                         u[4][k - 1][j - 1][i - 1]) -
                        DZ2 * p[3][k][j][i] *
                        (u[4][k][j][i] + u[4][k][j - 1][i] - u[4][k - 1][j][i] -
                         u[4][k - 1][j - 1][i] + u[4][k][j][i - 1] +
                         u[4][k][j - 1][i - 1] - u[4][k - 1][j][i - 1] -
                         u[4][k - 1][j - 1][i - 1]) +
                        PMU * (DX4 * (ff[4][k][j][i + 1] - 2.0 * ff[4][k][j][i] +
                                      ff[4][k][j][i - 1]) +
                               DY4 * (ff[4][k][j + 1][i] - 2.0 * ff[4][k][j][i] +
                                      ff[4][k][j - 1][i]) +
                               DZ4 * (ff[4][k + 1][j][i] - 2.0 * ff[4][k][j][i] +
                                      ff[4][k - 1][j][i]));

                f[3][k][j][i] =
                        ff[3][k][j][i] +
                        T * ((p[8][k][j][i] * p[6][k][j][i] -
                              p[9][k][j][i] * p[5][k][j][i]) /
                             p[0][k][j][i] -
                             GRA * z / ra3) -
                        DX2 * p[1][k][j][i] *
                        (u[3][k][j][i] + u[3][k][j - 1][i] + u[3][k - 1][j][i] +
                         u[3][k - 1][j - 1][i] - u[3][k][j][i - 1] -
                         u[3][k][j - 1][i - 1] - u[3][k - 1][j][i - 1] -
                         u[3][k - 1][j - 1][i - 1]) -
                        DY2 * p[2][k][j][i] *
                        (u[3][k][j][i] - u[3][k][j - 1][i] + u[3][k - 1][j][i] -
                         u[3][k - 1][j - 1][i] + u[3][k][j][i - 1] -
                         u[3][k][j - 1][i - 1] + u[3][k - 1][j][i - 1] -
                         u[3][k - 1][j - 1][i - 1]) -
                        DZ2 * p[3][k][j][i] *
                        (u[3][k][j][i] + u[3][k][j - 1][i] - u[3][k - 1][j][i] -
                         u[3][k - 1][j - 1][i] + u[3][k][j][i - 1] +
                         u[3][k][j - 1][i - 1] - u[3][k - 1][j][i - 1] -
                         u[3][k - 1][j - 1][i - 1]) -
                        DZ2 *
                        (u[4][k][j][i] + u[4][k][j - 1][i] - u[4][k - 1][j][i] -
                         u[4][k - 1][j - 1][i] + u[4][k][j][i - 1] +
                         u[4][k][j - 1][i - 1] - u[4][k - 1][j][i - 1] -
                         u[4][k - 1][j - 1][i - 1]) /
                        p[0][k][j][i] +
                        VMU *
                        (DX4 * (ff[3][k][j][i + 1] - 2.0 * ff[3][k][j][i] +
                                ff[3][k][j][i - 1]) +
                         DY4 * (ff[3][k][j + 1][i] - 2.0 * ff[3][k][j][i] +
                                ff[3][k][j - 1][i]) +
                         DZ4 * (ff[3][k + 1][j][i] - 2.0 * ff[3][k][j][i] +
                                ff[3][k - 1][j][i])) /
                        ff[0][k][j][i];

                f[2][k][j][i] =
                        ff[2][k][j][i] +
                        T * ((p[10][k][j][i] * p[5][k][j][i] -
                              p[8][k][j][i] * p[7][k][j][i]) /
                             p[1][k][j][i] -
                             GRA * y / ra3) -
                        DX2 * p[1][k][j][i] *
                        (u[2][k][j][i] + u[2][k][j - 1][i] + u[2][k][j][i - 1] +
                         u[2][k - 1][j - 1][i] - u[2][k][j][i - 1] -
                         u[2][k][j - 1][i - 1] - u[2][k - 1][j][i - 1] -
                         u[2][k - 1][j - 1][i - 1]) -
                        DY2 * p[2][k][j][i] *
                        (u[2][k][j][i] - u[2][k][j - 1][i] + u[2][k][j][i - 1] -
                         u[2][k - 1][j - 1][i] + u[2][k][j][i - 1] -
                         u[2][k][j - 1][i - 1] + u[2][k - 1][j][i - 1] -
                         u[2][k - 1][j - 1][i - 1]) -
                        DZ2 * p[3][k][j][i] *
                        (u[2][k][j][i] + u[2][k][j - 1][i] - u[2][k][j][i - 1] -
                         u[2][k - 1][j - 1][i] + u[2][k][j][i - 1] +
                         u[2][k][j - 1][i - 1] - u[2][k - 1][j][i - 1] -
                         u[2][k - 1][j - 1][i - 1]) -
                        DY2 *
                        (u[4][k][j][i] - u[4][k][j - 1][i] + u[4][k - 1][j][i] -
                         u[4][k - 1][j - 1][i] + u[4][k][j][i - 1] -
                         u[4][k][j - 1][i - 1] + u[4][k - 1][j][i - 1] -
                         u[4][k - 1][j - 1][i - 1]) /
                        p[0][k][j][i] +
                        VMU *
                        (DX4 * (ff[2][k][j][i + 1] - 2.0 * ff[2][k][j][i] +
                                ff[2][k][j][i - 1]) +
                         DY4 * (ff[2][k][j + 1][i] - 2.0 * ff[2][k][j][i] +
                                ff[2][k][j - 1][i]) +
                         DZ4 * (ff[2][k + 1][j][i] - 2.0 * ff[2][k][j][i] +
                                ff[2][k][j][i - 1])) /
                        ff[0][k][j][i];

                f[1][k][j][i] =
                        ff[1][k][j][i] +
                        T * ((p[9][k][j][i] * p[7][k][j][i] -
                              p[10][k][j][i] * p[6][k][j][i]) /
                             p[0][k][j][i] -
                             GRA * x / ra3) -
                        DX2 * p[1][k][j][i] *
                        (u[1][k][j][i] + u[1][k][j - 1][i] + u[1][k - 1][j][i] +
                         u[1][k - 1][j - 1][i] - u[1][k][j][i - 1] -
                         u[1][k][j - 1][i - 1] - u[1][k - 1][j][i - 1] -
                         u[1][k - 1][j - 1][i - 1]) -
                        DY2 * p[2][k][j][i] *
                        (u[1][k][j][i] - u[1][k][j - 1][i] + u[1][k - 1][j][i] -
                         u[1][k - 1][j - 1][i] + u[1][k][j][i - 1] -
                         u[1][k][j - 1][i - 1] + u[1][k - 1][j][i - 1] -
                         u[1][k - 1][j - 1][i - 1]) -
                        DZ2 * p[3][k][j][i] *
                        (u[1][k][j][i] + u[1][k][j - 1][i] - u[1][k - 1][j][i] -
                         u[1][k - 1][j - 1][i] + u[1][k][j][i - 1] +
                         u[1][k][j - 1][i - 1] - u[1][k - 1][j][i - 1] -
                         u[1][k - 1][j - 1][i - 1]) -
                        DX2 *
                        (u[4][k][j][i] + u[4][k][j - 1][i] + u[4][k - 1][j][i] +
                         u[4][k - 1][j - 1][i] - u[4][k][j][i - 1] -
                         u[4][k][j - 1][i - 1] - u[4][k - 1][j][i - 1] -
                         u[4][k - 1][j - 1][i - 1]) /
                        p[0][k][j][i] +
                        VMU *
                        (DX4 * (ff[1][k][j][i + 1] - 2.0 * ff[1][k][j][i] +
                                ff[1][k][j][i - 1]) +
                         DY4 * (ff[1][k][j + 1][i] - 2.0 * ff[1][k][j][i] +
                                ff[1][k][j - 1][i]) +
                         DZ4 * (ff[1][k + 1][j][i] - 2.0 * ff[1][k][j][i] +
                                ff[1][k - 1][j][i])) /
                        ff[0][k][j][i];

                f[0][k][j][i] =
                        ff[0][k][j][i] -
                        DX2 * (u[1][k][j][i] * u[0][k][j][i] +
                               u[1][k][j - 1][i] * u[0][k][j - 1][i] +
                               u[1][k - 1][j][i] * u[0][k - 1][j][i] +
                               u[1][k - 1][j - 1][i] * u[0][k - 1][j - 1][i] -
                               u[1][k][j][i - 1] * u[0][k][j][i - 1] -
                               u[1][k][j - 1][i - 1] * u[0][k][j - 1][i - 1] -
                               u[1][k - 1][j][i - 1] * u[0][k - 1][j][i - 1] -
                               u[1][k - 1][j - 1][i - 1] * u[0][k - 1][j - 1][i - 1]) -
                        DY2 * (u[2][k][j][i] * u[0][k][j][i] -
                               u[2][k][j - 1][i] * u[0][k][j - 1][i] +
                               u[2][k][j][i - 1] * u[0][k - 1][j][i] -
                               u[2][k - 1][j - 1][i] * u[0][k - 1][j - 1][i] +
                               u[2][k][j][i - 1] * u[0][k][j][i - 1] -
                               u[2][k][j - 1][i - 1] * u[0][k][j - 1][i - 1] +
                               u[2][k - 1][j][i - 1] * u[0][k - 1][j][i - 1] -
                               u[2][k - 1][j - 1][i - 1] * u[0][k - 1][j - 1][i - 1]) -
                        DZ2 * (u[3][k][j][i] * u[0][k][j][i] +
                               u[3][k][j - 1][i] * u[0][k][j - 1][i] -
                               u[3][k - 1][j][i] * u[0][k - 1][j][i] -
                               u[3][k - 1][j - 1][i] * u[0][k - 1][j - 1][i] +
                               u[3][k][j][i - 1] * u[0][k][j][i - 1] +
                               u[3][k][j - 1][i - 1] * u[0][k][j - 1][i - 1] -
                               u[3][k - 1][j][i - 1] * u[0][k - 1][j][i - 1] -
                               u[3][k - 1][j - 1][i - 1] * u[0][k - 1][j - 1][i - 1]) +
                        DFU * (DX4 * (ff[0][k][j][i + 1] - 2.0 * ff[0][k][j][i] +
                                      ff[0][k][j][i - 1]) +
                               DY4 * (ff[0][k][j + 1][i] - 2.0 * ff[0][k][j][i] +
                                      ff[0][k][j - 1][i]) +
                               DZ4 * (ff[0][k + 1][j][i] - 2.0 * ff[0][k][j][i] +
                                      ff[0][k - 1][j][i]));
            }

            for (i = 1; i < NX1; i++) {
                x = 0.5 * HX * (2 * i - NX2 + 1 + 2 * NXP);
                ra2 = x * x + y * y + z * z;
                x2 = fmax(ra2 / RA_I2 - 1.0, 0.0);
                x2 = ARU * x2 * x2;
                x2 = x2 / (x2 + 1.0);
                ra2 = fmax(ra2, RA_I2);
                ra3 = ra2 * sqrt(ra2);

                f[7][k][j][i] = f[7][k][j][i] * x2 + ff[7][k][j][i] * (1.0 - x2);
                f[6][k][j][i] = f[6][k][j][i] * x2 + ff[6][k][j][i] * (1.0 - x2);
                f[5][k][j][i] = f[5][k][j][i] * x2 + ff[5][k][j][i] * (1.0 - x2);
                f[4][k][j][i] = f[4][k][j][i] * x2 + ff[4][k][j][i] * (1.0 - x2);
                f[3][k][j][i] = f[3][k][j][i] * x2 + ff[3][k][j][i] * (1.0 - x2);
                f[2][k][j][i] = f[2][k][j][i] * x2 + ff[2][k][j][i] * (1.0 - x2);
                f[1][k][j][i] = f[1][k][j][i] * x2 + ff[1][k][j][i] * (1.0 - x2);
                f[0][k][j][i] = f[0][k][j][i] * x2 + ff[0][k][j][i] * (1.0 - x2);
            }
        }
    }
    vmaxf(f, vmax);

    if (ii == 1 || nmlf == NMLF0) {
        t1 = T;
        dx1 = 0.25 * t1 / HX;
        dy1 = 0.25 * t1 / HY;
        dz1 = 0.25 * t1 / HZ;
    }

    return 0;
}

int main() {
    // Time variables
    clock_t zt0, zt1, zt2, zt3;
    double zt, ztt;

    // Simulation arrays
    float (*restrict f)[NZ2][NY2][NX2] = (float (*)[NZ2][NY2][NX2]) malloc(NB * NZ2 * NY2 * NX2 * sizeof(float));
    float (*restrict u)[NZ2][NY2][NX2] = (float (*)[NZ2][NY2][NX2]) malloc(NB * NZ2 * NY2 * NX2 * sizeof(float));
    float (*restrict pp)[NZ2][NY2][NX2] = (float (*)[NZ2][NY2][NX2]) malloc(NBBB * NZ2 * NY2 * NX2 * sizeof(float));

    int nmlf = 0, nupb = 0, nout = 0, ii;
    int ntap = 10;

    float vmax = 0.0;

    // Simulation information
    simuinfo();

    // Record start time
    zt0 = clock();

    // Initial conditions
    equil_out(f, pp);

    // Record initial time
    zt1 = clock();

    // Main loop
    for (ii = 1; ii <= LAST; ii++) {
        nmlf++;
        nupb++;
        nout++;

        // Outer boundary for f
        bdry_z_m(f, u);

        // Main calculation
        mlf(f, u, pp, ii, nmlf, vmax);

        if (vmax > 1.0)
            nout = NOUT0;

        if (nmlf == NMLF0)
            nmlf = 0;

        // Upward boundary condition for f
        if (nupb == NUPB0) {
            nupb = 0;
            bdry_up(f, u);
        }

        // Finalization part (write data, etc.)
        if (nout == NOUT0) {
            nout = 0;

            // Record end time
            zt3 = clock();

            // Output information
            ntap++;

            printf("    ii, LAST, ntap,   vmax,    THX\n");
            printf("%6d%6d%6d%8.4f%8.4f\n", ii, LAST, ntap, vmax, THX);
            printf(" \n");

            zt = (double) (zt3 - zt1) / CLOCKS_PER_SEC;
            ztt = (double) (zt1 - zt0) / CLOCKS_PER_SEC;

            printf("    ii,      init,        zt\n");
            printf("%6d%11.4f%11.4f\n", ii, ztt, zt);
            printf(" \n");

            gflops(zt);
        }
    }
    return 0;
}
