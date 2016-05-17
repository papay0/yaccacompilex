0: .file test/malloc.c
1: .area start
2: JMP 43
3: .function main 0
4: .local 1 ptr
5: AFC 2 10
6: MAL 2 2
7: COP 1 2
8: .local 2 ptr2
9: AFC 3 20
10: MAL 3 3
11: COP 2 3
12: .local 3 ptr3
13: AFC 4 30
14: MAL 4 4
15: COP 3 4
16: AFC 4 9
17: COPB 1 4
18: COP 5 1
19: FRE 5
20: COP 6 1
21: PRI 6
22: COP 6 2
23: PRI 6
24: COP 6 1
25: AFC 7 512
26: EQ 6 6 7
27: ASRT 6
28: COP 6 2
29: AFC 7 522
30: EQ 6 6 7
31: ASRT 6
32: COP 6 3
33: AFC 7 542
34: EQ 6 6 7
35: ASRT 6
36: COP 6 1
37: AFC 7 0
38: ADD 6 6 7
39: COPA 6 6
40: PRI 6
41: RET
42: .area bootstrap
43: AFC @0 4
44: CALL @0
45: EXIT
