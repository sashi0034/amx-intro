# AMX 簡略化した疑似コード

# _tile_loadd

`void _tile_loadd (__tile dst, const void * base, size_t stride)`

タイルに行列をロードする

- 基本 stride の値は dst.colsb に等しいと思われる
- tileconfig.startRow と stride を調整することで部分的に行列の一部分のロードが可能と思われる

```
current_row := tileconfig.startRow // _tile_loadconfig で設定された値
nbytes := dst.colsb
while (start < dst.rows):
  memptr := base + current_row * stride
  dst.row[current_row] = (memory*)memptr for nbytes
  current_row += 1
```


# _tile_dpbssd 

`void _tile_dpbssd (__tile dst, __tile a, __tile b)`

行列積の計算
- dpb = dot product of bytes
- ssd = signed signed (data? dest?)

```
for m in [0, dst.rows - 1] // dst の m 行目を計算
  for n in [0, dst.cols - 1] // dst[m][n] を計算 (m 行 n 列目)
    for k in [0, a.colsb / 4 - 1]
      dst[m][n] += a.row[m].byte[k * 4 + 0] * b.row[k].byte[n * 4 + 0]
      // ...
      dst[m][n] += a.row[m].byte[k * 4 + 3] * b.row[k].byte[n * 4 + 3]
```

