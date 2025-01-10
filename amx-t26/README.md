# 実行

```sh
icc -g -O2 t26_amx.c -o build/t26_amx && tssrun -p gr10034a build/t26_amx
```

```sh
icc -g -O2 t26_naive.c -o build/t26_naive && tssrun -p gr10034a build/t26_naive
```

# アセンブリ出力

```sh
icc -g -O2 t26_amx.c -S -o build/t26_amx.s
```

# コンパイルしてから VTune プロファイル

- AMX
```sh
icc -g -O3 t26_amx.c -o build/t26_amx && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune -collect hotspots -r=./results/result_amx build/t26_amx
```

```
-xCOMMON-AVX512
-xCORE-AVX2
-xSSE4.2
```

- Naive
```sh
icc -g -O3 t26_naive.c -o build/t26_naive && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./results/result_naive build/t26_naive
```
