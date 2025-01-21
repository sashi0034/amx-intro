
# クラスタ実行
AMX
```sh
icc -g -O3 t30_task_amx.c t30_mhd.c t30_sim.c -o build/t30_task_amx && tssrun -p gr10034a --rsc m=50G build/t30_task_amx
```

Naive
```sh
icc -g -O3 t30_task_naive.c t30_mhd.c t30_sim.c -o build/t30_task_naive && tssrun -p gr10034a --rsc m=50G build/t30_task_naive
```

```
icc -g -O3 -xCOMMON-AVX512 t30_task_amx.c t30_mhd.c t30_sim.c -o build/t30_task_amx && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./result/t30_task_amx build/t30_task_amx
```

```
icc -g -O3 -xCOMMON-AVX512 t30_task_naive.c t30_mhd.c t30_sim.c -o build/t30_task_naive && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./result/t30_task_naive build/t30_task_naive
```

失敗
```
nvc -acc -gpu=managed -g -O2 t30_task_amx.c t30_mhd.c t30_sim.c -o build/t30_task_amx --diag_suppress declared_but_not_referenced && tssrun -p gr10034a --rsc m=50G build/t30_task_amx
```

GPU
```sh
# module switch PrgEnvIntel PrgEnvNvidia
module load PrgEnvNvidia
nvc -acc -gpu=managed -g -O2 t30_task_gpu.c t30_mhd.c t30_sim.c -o build/t30_task_gpu --diag_suppress declared_but_not_referenced && tssrun -p gr10034a --rsc m=50G build/t30_task_gpu
```

```
module load PrgEnvNvidia
nvc -acc -gpu=managed -g -O2 t30_task_gpu.c t30_mhd.c t30_sim.c -o build/t30_task_gpu --diag_suppress declared_but_not_referenced && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./result/t30_task_gpu build/t30_task_gpu
```
