0: .file test/testbench.c
1: .area start
2: .local 0 a
3: AFC 1 10
4: COP @0 1
5: .local 1 b
6: AFC 2 20
7: COP @1 2
8: JMP 107
9: .function mult 1
10: COP 1 -3
11: COPA 1 1
12: AFC 2 2
13: MUL 1 1 2
14: COPB -3 1
15: COP 2 -3
16: COPA 2 2
17: COP 1 2
18: RET
19: RET
20: .function g2 2
21: COP 1 -3
22: COP 2 -4
23: SUB 1 1 2
24: COP 1 1
25: RET
26: RET
27: .function g3 3
28: COP 1 -5
29: COP 2 -4
30: ADD 1 1 2
31: COP 2 -3
32: ADD 1 1 2
33: COP 1 1
34: RET
35: RET
36: .function f 1
37: COP 1 -3
38: AFC 2 0
39: EQ 1 1 2
40: JMF 1 44
41: AFC 2 1
42: COP 1 2
43: RET
44: COP 1 -3
45: COP 2 -3
46: AFC 3 1
47: SUB 2 2 3
48: .pusharg
49: .stacksize 3
50: .popargs 1
51: AFC 3 53
52: CALL @5
53: COP 2 6
54: MUL 1 1 2
55: COP 1 1
56: RET
57: RET
58: .function p3 3
59: COP 1 -3
60: COP 2 -4
61: ADD 1 1 2
62: COP 2 -5
63: ADD 1 1 2
64: COP 1 1
65: RET
66: RET
67: .function main 0
68: .local 1 a
69: AFC 2 3
70: AFC 3 10
71: .pusharg
72: AFC 4 1
73: .pusharg
74: AFC 5 5
75: .pusharg
76: .stacksize 6
77: .popargs 1
78: AFC 6 80
79: CALL @5
80: COP 5 9
81: .pusharg
82: AFC 6 -1
83: .pusharg
84: .stacksize 7
85: .popargs 3
86: AFC 7 88
87: CALL @6
88: COP 4 10
89: .pusharg
90: AFC 5 20
91: .pusharg
92: .stacksize 6
93: .popargs 3
94: AFC 6 96
95: CALL @6
96: COP 3 9
97: MUL 2 2 3
98: COP 1 2
99: COP 2 1
100: PRI 2
101: COP 2 1
102: AFC 3 450
103: EQ 2 2 3
104: ASRT 2
105: RET
106: .area bootstrap
107: AFC @2 10
108: AFC @3 21
109: AFC @4 28
110: AFC @5 37
111: AFC @6 59
112: AFC @7 68
113: CALL @7
114: EXIT
