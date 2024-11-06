

# ローカル実行
```sh
icc -g -O2 t8_mhd_origin.c -o build/t8_mhd_origin && build/t8_mhd_origin
```

# クラスタ実行
```sh
icc -g -O2 t8_mhd_origin.c -o build/t8_mhd_origin && tssrun -p gr10034a --rsc m=50G build/t8_mhd_origin
```

```sh
icc -g -O2 t8_mhd_amx.c -o build/t8_mhd_amx && tssrun -p gr10034a --rsc m=50G build/t8_mhd_amx
```

```sh
icc -g -O2 t8_amx_bf16.c -o build/t8_amx_bf16 && tssrun -p gr10034a --rsc m=1G build/t8_amx_bf16
```

```sh
icc -g -O2 test_bf16.c -o build/test_bf16 && tssrun -p gr10034a build/test_bf16
```