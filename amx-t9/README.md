
# クラスタ実行
```sh
icc -g -O2 t9_mhd_amx.c t9_mhd.c t9_amx_8x16.c -o build/t9_mhd_amx && tssrun -p gr10034a --rsc m=50G build/t9_mhd_amx
```

