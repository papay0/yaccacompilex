0: .file test/dummy/malloc.c
1: .area start
2: JMP 43
3: .function get_ptr 0
4: .local 1 ptr
5: AFC 2 50
6: MAL 2 2
7: COP 1 2
8: COP 2 1
9: AFC 3 20
10: AFC 4 50
11: ADD 2 2 3
12: COPB 2 4
13: COP 3 1
14: COP 1 3
15: RET
16: RET
17: .function main 0
18: .local 1 ptr
19: .stacksize 2
20: .popargs 0
21: AFC 2 23
22: CALL @0
23: COP 3 5
24: COP 1 3
25: COP 2 1
26: AFC 3 20
27: ADD 2 2 3
28: COPA 2 2
29: AFC 3 50
30: EQ 2 2 3
31: ASRT 2
32: COP 2 1
33: AFC 3 20
34: ADD 2 2 3
35: COPA 2 2
36: PRI 2
37: COP 2 1
38: FRE 2
39: COP 3 1
40: FRE 3
41: RET
42: .area bootstrap
43: AFC @0 4
44: AFC @1 18
45: CALL @1
46: EXIT
