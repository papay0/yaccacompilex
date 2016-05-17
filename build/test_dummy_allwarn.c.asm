0: .file test/dummy/allwarn.c
1: .area start
2: JMP 87
3: .function redef 1
4: .function redef 1
5: RET
6: .function doubledef 1
7: .function doubledef 1
8: .function f 2
9: AFC 1 1
10: COP 1 1
11: RET
12: RET
13: .function main 0
14: AFC 1 56
15: .pusharg
16: AFC 2 565656
17: .pusharg
18: .local 1 a
19: .local 2 a
20: COP 5 2
21: AFC 6 0
22: EQ 5 5 6
23: JMF 5 25
24: .local 3 a
25: .local 3 b
26: COP 4 @2
27: COP 3 4
28: AFC 4 5
29: .pusharg
30: AFC 5 5
31: .pusharg
32: .stacksize 6
33: .popargs 2
34: AFC 6 36
35: CALL @2
36: COP 4 9
37: AFC 4 0
38: .pusharg
39: .stacksize 5
40: .popargs 1
41: AFC 5 43
42: CALL @2
43: COP 4 8
44: AFC 4 0
45: .pusharg
46: AFC 5 1
47: .pusharg
48: AFC 6 2
49: .pusharg
50: .stacksize 7
51: .popargs 3
52: AFC 7 54
53: CALL @2
54: COP 4 10
55: PTR 4 3
56: .pusharg
57: PTR 5 @2
58: .pusharg
59: .stacksize 6
60: .popargs 2
61: AFC 6 63
62: CALL 3
63: COP 4 9
64: .stacksize 4
65: .popargs 0
66: AFC 4 68
67: CALL 2
68: COP 5 7
69: COP 4 @2
70: AFC 5 5
71: ADD 4 4 5
72: COP 3 4
73: AFC 5 5
74: COP 4 5
75: COP 8 5
76: AFC 9 5
77: ADD 8 8 9
78: COP 6 8
79: .local 7 ptr
80: .local 8 integer
81: COP 12 7
82: COP 13 8
83: ADD 12 12 13
84: COP 7 12
85: RET
86: .area bootstrap
87: AFC @0 5
88: AFC @2 9
89: AFC @3 14
90: CALL @3
91: EXIT
