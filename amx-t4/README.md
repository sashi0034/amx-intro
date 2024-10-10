
# 10月: AMX と AVX-512 比較

`make build/t4_amx && tssrun -p gr10034a build/t4_amx < build/test.txt`

https://web.kudpc.kyoto-u.ac.jp/manual/ja/compilers/intel_vtune

`icc -g -O2 t4_amx.c -o build/t4_amx`

`tssrun -p gr10034a vtune -collect hotspots  -r=./result.vtune build/t4_amx < build/test.txt`

# テスト生成

`make build/generate_test && tssrun -p gr10034a build/generate_test > build/test.txt`

