0: .file test/errors.c
1: .area start
2: .local 0 global
3: JMP 82
4: .function f1 1
5: RET
6: .function f2 1
7: RET
8: .function f3 1
9: RET
10: .function main 0
11: .local 1 chr
12: .local 2 i
13: COP 3 1
14: .pusharg
15: .stacksize 4
16: .popargs 1
17: AFC 4 19
18: CALL @1
19: COP 3 7
20: COP 3 2
21: COPA 3 3
22: .pusharg
23: .stacksize 4
24: .popargs 1
25: AFC 4 27
26: CALL @1
27: COP 3 7
28: COP 3 1
29: .pusharg
30: COP 4 2
31: .pusharg
32: .stacksize 5
33: .popargs 2
34: AFC 5 36
35: CALL @1
36: COP 3 8
37: .stacksize 3
38: .popargs 0
39: AFC 3 41
40: CALL @1
41: COP 4 6
42: COP 4 3
43: .pusharg
44: .stacksize 5
45: .popargs 1
46: AFC 5 48
47: CALL @1
48: COP 4 8
49: AFC 4 6
50: COP 1 4
51: AFC 5 8
52: COP 2 5
53: AFC 6 9
54: COP 2 6
55: .local 4 ptr
56: .local 5 number
57: COP 9 4
58: COP 10 5
59: ADD 9 9 10
60: COP 4 9
61: COP 10 4
62: COP 11 5
63: ADD 10 10 11
64: COP 5 10
65: COP 11 4
66: COP 12 4
67: ADD 11 11 12
68: COP 4 11
69: .local 6 ptr3
70: .local 7 ptr2
71: COP 14 6
72: COP 7 14
73: COP 15 6
74: .pusharg
75: .stacksize 16
76: .popargs 1
77: AFC 16 79
78: CALL @3
79: COP 15 19
80: RET
81: .area bootstrap
82: AFC @1 5
83: AFC @2 7
84: AFC @3 9
85: AFC @4 11
86: CALL @4
87: EXIT
