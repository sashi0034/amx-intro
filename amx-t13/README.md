# テスト生成

AMX

```sh
icc -g -O2 t13_generate.c -o build/t13_generate && tssrun -p gr10034a --rsc m=50G build/t13_generate 100
```

# クラスタ実行

Naive

```sh
icc -g -O2 t13_naive.c -o build/t13_naive && tssrun -p gr10034a --rsc m=50G build/t13_naive
```

AMX

```sh
icc -g -O2 t13_amx.c -o build/t13_amx && tssrun -p gr10034a --rsc m=50G build/t13_amx
```

