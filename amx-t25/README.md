```sh
icc -g -O2 t25_amx.c -o build/t25_amx && tssrun -p gr10034a build/t25_amx
```

```sh
icc -g -O2 t25_naive.c -o build/t25_naive && tssrun -p gr10034a build/t25_naive
```
