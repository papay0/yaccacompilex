0: .file test/facto.c
1: .area start
2: JMP 47
3: .function facto 2
4: COP 1 -4
5: AFC 2 0
6: EQ 1 1 2
7: JMF 1 11
8: AFC 2 1
9: COP 1 2
10: RET
11: COP 1 -4
12: COP 2 -4
13: AFC 3 1
14: SUB 2 2 3
15: .pusharg
16: COP 3 -4
17: AFC 4 1
18: SUB 3 3 4
19: .pusharg
20: .stacksize 4
21: .popargs 2
22: AFC 4 24
23: CALL @0
24: COP 2 7
25: MUL 1 1 2
26: COP 1 1
27: RET
28: RET
29: .function main 0
30: .local 1 a
31: AFC 2 5
32: .pusharg
33: AFC 3 5
34: .pusharg
35: .stacksize 4
36: .popargs 2
37: AFC 4 39
38: CALL @0
39: COP 2 7
40: COP 1 2
41: COP 2 1
42: AFC 3 120
43: EQ 2 2 3
44: ASRT 2
45: RET
46: .area bootstrap
47: AFC @0 4
48: AFC @1 30
49: CALL @1
50: EXIT
