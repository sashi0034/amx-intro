
# 10月: AMX と AVX-512 比較

https://web.kudpc.kyoto-u.ac.jp/manual/ja/compilers/intel_vtune

アセンブリ確認

- AMX

```
icc -O3 -S t5_amx.c -o build/t5_amx.s
```

- AVX-512

```
icc -g -xCORE-AVX512 -qopt-zmm-usage=high -O3 -S t5_avx512.c -o build/t5_avx512.s
```

コンパイルしてから実行

```
icc -g -O3 t5_amx.c -o build/t5_amx && tssrun -p gr10034a build/t5_amx
```

```
icc -g -xCORE-AVX512 -qopt-zmm-usage=high -O3 t5_avx512.c -o build/t5_avx512 && build/t5_avx512
```

コンパイルしてから VTune プロファイル

- AMX

```sh
icc -g -O3 t5_amx.c -o build/t5_amx && \
module load intel-vtune && \
tssrun -p gr10034a vtune -collect hotspots  -r=./result_amx build/t5_amx < build/test.txt
```

AVX-512
```sh
icc -g -xCORE-AVX512 -qopt-zmm-usage=high -O3 t5_avx512.c -o build/t5_avx512 && \
module load intel-vtune && \
tssrun -p gr10034a vtune -collect hotspots  -r=./result_t5_avx512 build/t5_avx512 < build/test.txt
```

# テスト生成

```
icpc -O3 t5_gen.cpp -o build/t5_gen && build/t5_gen
```

結果確認

```
head -n 10 build/test.txt
```

