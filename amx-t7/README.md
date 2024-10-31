
# 10月: AMX と AVX-512 比較

https://web.kudpc.kyoto-u.ac.jp/manual/ja/compilers/intel_vtune

アセンブリ確認

- AMX

```
icc -O3 -S t7_amx.c -o build/t7_amx.s
```

- AVX-512

```
icc -g -xCORE-AVX512 -qopt-zmm-usage=high -O3 -S t7_noamx.c -o build/t7_avx512.s
```

コンパイルしてから実行

```
icc -g -O3 t7_amx.c -o build/t7_amx && tssrun -p gr10034a build/t7_amx
```

```
icc -g -xCORE-AVX512 -qopt-zmm-usage=high -O3 t7_noamx.c -o build/t7_avx512 && build/t7_avx512
```

コンパイルしてから VTune プロファイル

- AMX

```sh
icc -g -O3 t7_amx.c -o build/t7_amx && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune -collect hotspots -r=./results/result_amx build/t7_amx
```

AVX-512
```sh
icc -g -xCORE-AVX512 -qopt-zmm-usage=high -O3 t7_noamx.c -o build/t7_avx512 && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune -collect hotspots -r=./results/result_avx512 build/t7_avx512
```

No-AMX
```sh
icc -g -O3 t7_noamx.c -o build/t7_noamx && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune --rsc m=1G -collect hotspots -r=./results/result_noamx build/t7_noamx
```

# テスト生成

```
icpc -O3 t7_gen.cpp -o build/t7_gen && build/t7_gen 500000
```

結果確認

```
head -n 10 build/test.txt
```

