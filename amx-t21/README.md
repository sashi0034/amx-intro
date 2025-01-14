
https://web.kudpc.kyoto-u.ac.jp/manual/ja/compilers/intel_vtune


コンパイラオプション

- https://www.intel.com/content/www/us/en/docs/cpp-compiler/developer-guide-reference/2021-8/alphabetical-option-list.html

SIMD 関連

- https://en.wikipedia.org/wiki/AVX-512

アセンブリ確認

- AMX

```
icc -O3 -S t21_amx.c -o build/t21_amx.s
```

- AVX-512

```
icc -g -xCOMMON-AVX512 -O3 -S t21_naive.c -o build/t21_avx512.s
```

- ymm のみ

```
icc -g -xCORE-AVX2 -O3 -S t21_naive.c -o build/t21_avx512.s
```

- xmm のみ

```
icc -g -xSSE4.2 -O3 -S t21_naive.c -o build/t21_avx512.s
```

コンパイルしてから実行

```
icc -g -O3 t21_amx.c -o build/t21_amx && tssrun -p gr10034a --rsc m=50G build/t21_amx
```

```
icc -g -xCORE-AVX512 -qopt-zmm-usage=high -O3 t21_naive.c -o build/t21_avx512 && build/t21_avx512
```

```
icc -g -xCORE-AVX512 -qopt-zmm-usage=high -O3 t21_avx.c -o build/t21_avx512 && build/t21_avx512
```

コンパイルしてから VTune プロファイル

- AMX

```sh
icc -g -O3 t21_amx.c -o build/t21_amx && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune -collect hotspots -r=./results/result_amx build/t21_amx
```

AVX-512
```sh
icc -g -xCOMMON-AVX512 -O3 t21_avx.c -o build/t21_avx512 && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./results/result_avx512 build/t21_avx512
```

Naive
```sh
icc -g -O3 t21_naive.c -o build/t21_naive && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune --rsc m=1G -collect hotspots -r=./results/result_noamx build/t21_naive
```

# テスト生成

```
icpc -O3 t21_generate.cpp -o build/t21_generate && build/t21_generate 500000
```

結果確認

```
head -n 10 build/test.txt
```

