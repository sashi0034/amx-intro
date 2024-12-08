
# クラスタ実行
AMX
```sh
icc -g -O2 t11_task_amx.c t11_mhd.c t11_sim.c -o build/t11_task_amx && tssrun -p gr10034a --rsc m=50G build/t11_task_amx
```

```
icc -g -O2 t11_task_amx.c t11_mhd.c t11_sim.c -o build/t11_task_amx && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./result/t11_task_amx build/t11_task_amx
```

失敗
```
nvc -acc -gpu=managed -g -O2 t11_task_amx.c t11_mhd.c t11_sim.c -o build/t11_task_amx --diag_suppress declared_but_not_referenced && tssrun -p gr10034a --rsc m=50G build/t11_task_amx
```

Naive
```sh
icc -g -O2 t11_task_naive.c t11_mhd.c t11_sim.c -o build/t11_task_naive && tssrun -p gr10034a --rsc m=50G build/t11_task_naive
```

GPU
```sh
# module switch PrgEnvIntel PrgEnvNvidia
module load PrgEnvNvidia
nvc -acc -gpu=managed -g -O2 t11_task_gpu.c t11_mhd.c t11_sim.c -o build/t11_task_gpu --diag_suppress declared_but_not_referenced && tssrun -p gr10034a --rsc m=50G build/t11_task_gpu
```

```
module load PrgEnvNvidia
nvc -acc -gpu=managed -g -O2 t11_task_gpu.c t11_mhd.c t11_sim.c -o build/t11_task_gpu --diag_suppress declared_but_not_referenced && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./result/t11_task_gpu build/t11_task_gpu
```
