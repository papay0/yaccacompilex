0: .file test/func_sale.c
1: .area start
2: JMP 104
3: .function p 2
4: COP 1 -4
5: PRI 1
6: COP 1 -3
7: PRI 1
8: COP 1 -4
9: COP 1 1
10: RET
11: RET
12: .function p2 2
13: COP 1 -4
14: COP 2 -3
15: ADD 1 1 2
16: PRI 1
17: COP 1 -4
18: COP 2 -3
19: ADD 1 1 2
20: COP 1 1
21: RET
22: RET
23: .function pf 3
24: COP 1 -4
25: .pusharg
26: COP 2 -3
27: .pusharg
28: .stacksize 3
29: .popargs 2
30: AFC 3 32
31: CALL -5
32: COP 1 6
33: COP 1 1
34: RET
35: RET
36: .function main 0
37: .local 1 banane
38: AFC 2 9
39: COP 1 2
40: .local 2 pf_func
41: AFC 3 15
42: COP 2 3
43: .local 3 pf_func2
44: AFC 4 15
45: COP 3 4
46: COP 4 1
47: .pusharg
48: AFC 5 20
49: .pusharg
50: AFC 6 20
51: .pusharg
52: .stacksize 7
53: .popargs 3
54: AFC 7 56
55: CALL 2
56: COP 4 10
57: AFC 5 40
58: EQ 4 4 5
59: ASRT 4
60: COP 4 1
61: .pusharg
62: AFC 5 20
63: .pusharg
64: AFC 6 20
65: .pusharg
66: .stacksize 7
67: .popargs 3
68: AFC 7 70
69: CALL 3
70: COP 4 10
71: AFC 5 40
72: EQ 4 4 5
73: ASRT 4
74: COP 4 @0
75: .pusharg
76: AFC 5 5
77: .pusharg
78: AFC 6 10
79: .pusharg
80: .stacksize 7
81: .popargs 3
82: AFC 7 84
83: CALL @2
84: COP 4 10
85: AFC 5 5
86: EQ 4 4 5
87: ASRT 4
88: COP 4 @1
89: .pusharg
90: AFC 5 5
91: .pusharg
92: AFC 6 10
93: .pusharg
94: .stacksize 7
95: .popargs 3
96: AFC 7 98
97: CALL @2
98: COP 4 10
99: AFC 5 15
100: EQ 4 4 5
101: ASRT 4
102: RET
103: .area bootstrap
104: AFC @0 4
105: AFC @1 13
106: AFC @2 24
107: AFC @3 37
108: CALL @3
109: EXIT
