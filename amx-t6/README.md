
# AMX を利用しながら他の CPU コアで同時計算が可能かどうか

# OpenMP 例

```
icc -qopenmp -o build/mp_tutorial mp_tutorial.c && \
./build/mp_tutorial
```


