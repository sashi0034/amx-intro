
# 10月: AMX と AVX-512 比較

`make build/t4_amx && tssrun -p gr10034a build/t4_amx < build/test.txt`

https://web.kudpc.kyoto-u.ac.jp/manual/ja/compilers/intel_vtune

単体コンパイル 

`icc -g -O3 t4_amx.c -o build/t4_amx`

VTune プロファイル

`module load intel-vtune`

`tssrun -p gr10034a vtune -collect hotspots  -r=./result_amx build/t4_amx < build/test.txt`

アセンブリ確認

- AMX

```
icc -O3 -S t4_amx.c -o build/t4_amx.s
```

- AVX-512

```
icc -g -xCORE-AVX512 -qopt-zmm-usage=high -O3 -S t4_avx512.c -o build/t4_avx512.s
```

コンパイルしてから VTune プロファイル

- AMX

```sh
icc -g -O3 t4_amx.c -o build/t4_amx && \
module load intel-vtune && \
tssrun -p gr10034a vtune -collect hotspots  -r=./result_amx build/t4_amx < build/test.txt
```

AVX-512
```sh
icc -g -xCORE-AVX512 -qopt-zmm-usage=high -O3 t4_avx512.c -o build/t4_avx512 && \
module load intel-vtune && \
tssrun -p gr10034a vtune -collect hotspots  -r=./result_t4_avx512 build/t4_avx512 < build/test.txt
```

# テスト生成

```
make build/generate_test && tssrun -p gr10034a build/generate_test > build/test.txt
```

結果確認

```
head -n 10 build/test.txt
```

