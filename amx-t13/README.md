# テスト生成

AMX

```sh
icc -g -O2 t13_generate.c -o build/t13_generate && tssrun -p gr10034a --rsc m=50G build/t13_generate 5000000
```

# クラスタ実行

Naive

```sh
icc -g -O2 t13_naive.c -o build/t13_naive && tssrun -p gr10034a --rsc m=50G build/t13_naive
```

```
icc -g -O2 t13_naive.c -o build/t13_naive && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./results/t13_naive build/t13_naive
```

AMX

```sh
icc -g -O2 t13_amx.c -o build/t13_amx && tssrun -p gr10034a --rsc m=50G build/t13_amx
```

```
icc -g -O2 t13_amx.c -o build/t13_amx && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./results/t13_amx build/t13_amx
```
