
# クラスタ実行
AMX
```sh
icc -g -O2 t9_mhd_amx.c t9_mhd.c t9_amx_8x16.c -o build/t9_mhd_amx && tssrun -p gr10034a --rsc m=50G build/t9_mhd_amx
```

```
icc -g -O2 t9_mhd_amx.c t9_mhd.c t9_amx_8x16.c -o build/t9_mhd_amx && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./result/t9_mhd_amx build/t9_mhd_amx
```

GPU
```sh
# module switch PrgEnvIntel PrgEnvNvidia
module load PrgEnvNvidia
nvc -acc -gpu=managed -O2 t9_mhd_gpu.c t9_mhd.c -o build/t9_mhd_gpu --diag_suppress declared_but_not_referenced && tssrun -p gr10034a --rsc m=50G build/t9_mhd_gpu
```

```
nvc -acc -gpu=managed -O2 t9_mhd_gpu.c t9_mhd.c -o build/t9_mhd_gpu --diag_suppress declared_but_not_referenced && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./result/t9_mhd_gpu build/t9_mhd_gpu
```
