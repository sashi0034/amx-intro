
```
mkdir build
mkdir data
mkdir results
```

# テスト生成

```
icpc -O3 t12_gen.cpp -o build/t12_gen && build/t12_gen 500000
```

```
icc -g -O2 t12_tile1.c -o build/t12_tile1 && tssrun -p gr10034a --rsc m=1G build/t12_tile1
```


計測 1

```sh
icc -g -O2 t12_tile1.c -o build/t12_tile1 && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=1G vtune -collect hotspots -r=./results/result_amx build/t12_tile1
```
