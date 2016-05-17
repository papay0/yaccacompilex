0: .file test/func.c
1: .area start
2: JMP 59
3: .function g2 2
4: COP 1 -4
5: COP 2 -3
6: ADD 1 1 2
7: COP 1 1
8: RET
9: RET
10: .function g3 3
11: COP 1 -5
12: COP 2 -4
13: ADD 1 1 2
14: COP 2 -3
15: ADD 1 1 2
16: COP 1 1
17: RET
18: RET
19: .function main 0
20: .local 1 a
21: AFC 2 1
22: .pusharg
23: AFC 3 2
24: .pusharg
25: .stacksize 4
26: .popargs 2
27: AFC 4 29
28: CALL @0
29: COP 2 7
30: AFC 3 40
31: .pusharg
32: AFC 4 20
33: .pusharg
34: AFC 5 30
35: .pusharg
36: .stacksize 6
37: .popargs 2
38: AFC 6 40
39: CALL @0
40: COP 4 9
41: .pusharg
42: AFC 5 30
43: AFC 6 30
44: ADD 5 5 6
45: .pusharg
46: .stacksize 6
47: .popargs 3
48: AFC 6 50
49: CALL @1
50: COP 3 9
51: MUL 2 2 3
52: COP 1 2
53: COP 2 1
54: AFC 3 450
55: EQ 2 2 3
56: ASRT 2
57: RET
58: .area bootstrap
59: AFC @0 4
60: AFC @1 11
61: AFC @2 20
62: CALL @2
63: EXIT
