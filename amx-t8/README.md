

# ローカル実行
```sh
icc -g -O2 t8_mhd_origin.c -o build/t8_mhd_origin && build/t8_mhd_origin
```

# クラスタ実行
```sh
icc -g -O2 t8_mhd_origin.c -o build/t8_mhd_origin && tssrun -p gr10034a --rsc m=1G build/t8_mhd_origin
```

