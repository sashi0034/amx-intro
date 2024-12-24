
# 10月: AMX と AVX-512 比較

https://web.kudpc.kyoto-u.ac.jp/manual/ja/compilers/intel_vtune

アセンブリ確認

- AMX

```
icc -O3 -S t20_amx.c -o build/t20_amx.s
```

- AVX-512

```
icc -g -xCORE-AVX512 -qopt-zmm-usage=high -O3 -S t20_naive.c -o build/t20_avx512.s
```

コンパイルしてから実行

```
icc -g -O3 t20_amx.c -o build/t20_amx && tssrun -p gr10034a --rsc m=50G build/t20_amx
```

```
icc -g -xCORE-AVX512 -qopt-zmm-usage=high -O3 t20_naive.c -o build/t20_avx512 && build/t20_avx512
```

コンパイルしてから VTune プロファイル

- AMX

```sh
icc -g -O3 t20_amx.c -o build/t20_amx && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune -collect hotspots -r=./results/result_amx build/t20_amx
```

AVX-512
```sh
icc -g -xCORE-AVX512 -qopt-zmm-usage=high -O3 t20_naive.c -o build/t20_avx512 && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune -collect hotspots -r=./results/result_avx512 build/t20_avx512
```

No-AMX
```sh
icc -g -O3 t20_naive.c -o build/t20_naive && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune --rsc m=1G -collect hotspots -r=./results/result_noamx build/t20_naive
```

# テスト生成

```
icpc -O3 t20_generate.cpp -o build/t20_generate && build/t20_generate 500000
```

結果確認

```
head -n 10 build/test.txt
```

