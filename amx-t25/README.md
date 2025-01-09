# 実行

```sh
icc -g -O2 t25_amx.c -o build/t25_amx && tssrun -p gr10034a build/t25_amx
```

```sh
icc -g -O2 t25_naive.c -o build/t25_naive && tssrun -p gr10034a build/t25_naive
```

# アセンブリ出力

```sh
icc -g -O2 t25_amx.c -S -o build/t25_amx.s
```

# コンパイルしてから VTune プロファイル

- AMX
```sh
icc -g -O3 t25_amx.c -o build/t25_amx && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune -collect hotspots -r=./results/result_amx build/t25_amx
```

- Naive
```sh
icc -g -O3 t25_naive.c -o build/t25_naive && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./results/result_naive build/t25_naive
```
