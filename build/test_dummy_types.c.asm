0: .file test/dummy/types.c
1: .area start
2: JMP 14
3: .function main 0
4: .local 1 a
5: .local 2 b
6: .local 3 c
7: .local 4 t
8: COP 5 1
9: COP 4 5
10: COP 6 2
11: COP 4 6
12: RET
13: .area bootstrap
14: AFC @0 4
15: CALL @0
16: EXIT
