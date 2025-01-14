# 実行

```sh
icc -g -O3 t27_amx.c -o build/t27_amx && tssrun -p gr10034a build/t27_amx

icc -g -O3 t27_amx_o2.c -o build/t27_amx_o2 && tssrun -p gr10034a build/t27_amx_o2

icc -g -O3 t27_amx_o3.c -o build/t27_amx_o3 && tssrun -p gr10034a build/t27_amx_o3

icc -g -O3 t27_amx_o4.c -o build/t27_amx_o4 && tssrun -p gr10034a build/t27_amx_o4
```

```sh
icc -g -O3 t27_naive.c -xCOMMON-AVX512 -o build/t27_naive && tssrun -p gr10034a build/t27_naive
```

# アセンブリ出力

```sh
icc -g -O3 t27_amx.c -S -o build/t27_amx.s
```

# コンパイルしてから VTune プロファイル

- AMX
```sh
icc -g -O3 t27_amx.c -o build/t27_amx && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune -collect hotspots -r=./results/result_amx build/t27_amx
```

o2
```sh
icc -g -O3 t27_amx_o2.c -o build/t27_amx_o2 && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune -collect hotspots -r=./results/result_amx_o2 build/t27_amx_o2
```

o3
```sh
icc -g -O3 t27_amx_o3.c -o build/t27_amx_o3 && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune -collect hotspots -r=./results/result_amx_o3 build/t27_amx_o3
```

o4
```sh
icc -g -O3 t27_amx_o4.c -o build/t27_amx_o4 && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune -collect hotspots -r=./results/result_amx_o4 build/t27_amx_o4
```

```
-xCOMMON-AVX512
-xCORE-AVX2
-xSSE4.2
```

- Naive
```sh
icc -g -O3 t27_naive.c -xCOMMON-AVX512 -o build/t27_naive && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune -collect hotspots -r=./results/result_naive build/t27_naive
```

SysG
```
nvc -acc -gpu=cc80 -Minfo=accel -O2 -o build/t27g_naive t27_naive.c && tssrun build/t27g_naive

nvc -acc -gpu=cc80 -Minfo=accel -O2 -o build/t27g_gpu t27_gpu.c && tssrun build/t27g_gpu
```

