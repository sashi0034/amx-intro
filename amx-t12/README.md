
```
mkdir build
mkdir data
mkdir results
```

# テスト生成

```
icpc -O3 t12_gen.cpp -o build/t12_gen && tssrun -p gr10034a --rsc m=50G build/t12_gen 25000000
```

実行 1
```
icc -g -O2 t12_tile1.c -o build/t12_tile1 && tssrun -p gr10034a --rsc m=50G build/t12_tile1
```

計測 1
```sh
icc -g -O2 t12_tile1.c -o build/t12_tile1 && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./results/t12_tile1 build/t12_tile1
```

実行 3
```
icc -g -O2 t12_tile3.c -o build/t12_tile3 && tssrun -p gr10034a --rsc m=50G build/t12_tile3
```

計測 3
```
icc -g -O2 t12_tile3.c -o build/t12_tile3 && \
module load intel-vtune && \
tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./results/t12_tile3 build/t12_tile3
```

