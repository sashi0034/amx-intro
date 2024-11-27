
# クラスタ実行
AMX
```sh
icc -g -O2 t9_mhd_amx.c t9_mhd.c t9_amx_8x16.c -o build/t9_mhd_amx && tssrun -p gr10034a --rsc m=50G build/t9_mhd_amx
```

GPU
```sh
module switch PrgEnvIntel PrgEnvNvidia
nvc -acc -gpu=managed -O2 t9_mhd_gpu.c t9_mhd.c -o build/t9_mhd_gpu --diag_suppress declared_but_not_referenced && tssrun -p gr10034a --rsc m=50G build/t9_mhd_gpu
```
