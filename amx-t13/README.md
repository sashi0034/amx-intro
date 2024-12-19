
# テスト生成
AMX
```sh
icc -g -O2 t13_gen.c -o build/t13_gen && tssrun -p gr10034a --rsc m=50G build/t13_gen 100
```

# クラスタ実行
AMX
```sh
icc -g -O2 t13_naive.c -o build/t13_naive && tssrun -p gr10034a --rsc m=50G build/t13_naive
```

