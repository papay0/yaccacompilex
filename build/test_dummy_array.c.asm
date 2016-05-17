0: .file test/dummy/array.c
1: .area start
2: JMP 28
3: .function main 0
4: .array arr 1 5
5: PTR 1 2
6: COP 7 1
7: AFC 8 3
8: AFC 9 10
9: ADD 7 7 8
10: COPB 7 9
11: .local 7 b
12: COP 9 1
13: AFC 11 3
14: ADD 9 9 11
15: COPA 9 9
16: COP 7 9
17: COP 9 1
18: AFC 11 3
19: ADD 9 9 11
20: COPA 9 9
21: PRI 9
22: COP 9 1
23: PRI 9
24: PTR 9 1
25: PRI 9
26: RET
27: .area bootstrap
28: AFC @0 4
29: CALL @0
30: EXIT
