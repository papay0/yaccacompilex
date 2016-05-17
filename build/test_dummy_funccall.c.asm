0: .file test/dummy/funccall.c
1: .area start
2: JMP 29
3: .function f 3
4: .array arr 1 5
5: PTR 1 2
6: COP 7 -5
7: COP 1 7
8: RET
9: RET
10: .function main 0
11: .local 1 a
12: AFC 2 1
13: .pusharg
14: AFC 3 2
15: .pusharg
16: AFC 4 3
17: .pusharg
18: .stacksize 5
19: .popargs 3
20: AFC 5 22
21: CALL @0
22: COP 2 8
23: COP 1 2
24: COP 3 1
25: COP 1 3
26: RET
27: RET
28: .area bootstrap
29: AFC @0 4
30: AFC @1 11
31: CALL @1
32: EXIT
