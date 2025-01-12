# 実行

```sh
icc -g -O3 t27_amx.c -o build/t27_amx && tssrun -p gr10034a build/t27_amx

icc -g -O3 t27_amx_o2.c -o build/t27_amx_o2 && tssrun -p gr10034a build/t27_amx_o2
```

```sh
icc -g -O3 t27_naive.c -o build/t27_naive && tssrun -p gr10034a build/t27_naive
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
