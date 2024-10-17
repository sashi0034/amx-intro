
# AMX を利用しながら他の CPU コアで同時計算が可能かどうか

# OpenMP 例

```
icc -qopenmp -o build/mp_tutorial mp_tutorial.c && \
./build/mp_tutorial
```

# No-AMX

ローカル
```
icc -g -O2 -qopenmp -o build/t6_noamx t6_noamx.c && \
./build/t6_noamx
```

スパコン
```
icc -g -O2 -qopenmp -o build/t6_noamx t6_noamx.c && \
module load intel-vtune && \
tssrun -p gr10034a vtune -collect hotspots  -r=./result_t6_noamx build/t6_noamx
```


# AMX

VTune 計測
```
icc -g -O2 -qopenmp -o build/t6_amx t6_amx.c && \
module load intel-vtune && \
tssrun -p gr10034a vtune -collect hotspots  -r=./result_t6_amx build/t6_amx
```

VTune 無し
```
icc -g -O2 -qopenmp -o build/t6_amx t6_amx.c && \
tssrun -p gr10034a build/t6_amx
```

