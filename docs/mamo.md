# 対話型実行方法

```
tssrun <file>

# 最大 1 分間実行
tssrun -t 0:1:0 <file>
```

# ディスアセンブリ

```
objdump -M intel -d build/t1  > t1.asm
```
