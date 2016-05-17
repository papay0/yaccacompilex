0: .file test/while_benoit.c
1: .area start
2: JMP 44
3: .function main 0
4: .local 1 a
5: AFC 2 0
6: COP 1 2
7: .local 2 b
8: AFC 3 0
9: COP 2 3
10: COP 3 1
11: AFC 4 10
12: INF 3 3 4
13: JMF 3 30
14: AFC 4 0
15: COP 2 4
16: COP 5 2
17: AFC 6 10
18: INF 5 5 6
19: JMF 5 25
20: COP 6 2
21: AFC 7 1
22: ADD 6 6 7
23: COP 2 6
24: JMP 16
25: COP 3 1
26: AFC 4 1
27: ADD 3 3 4
28: COP 1 3
29: JMP 10
30: COP 3 1
31: PRI 3
32: COP 3 2
33: PRI 3
34: COP 3 1
35: AFC 4 10
36: EQ 3 3 4
37: ASRT 3
38: COP 3 2
39: AFC 4 10
40: EQ 3 3 4
41: ASRT 3
42: RET
43: .area bootstrap
44: AFC @0 4
45: CALL @0
46: EXIT
