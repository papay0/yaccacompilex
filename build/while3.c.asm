0: .file test/while3.c
1: .area start
2: JMP 51
3: .function main 0
4: .local 1 a
5: AFC 2 100
6: COP 1 2
7: .local 2 d
8: AFC 3 0
9: COP 2 3
10: COP 3 1
11: AFC 4 100
12: EQ 3 3 4
13: JMF 3 42
14: .local 3 b
15: AFC 5 100
16: COP 3 5
17: COP 5 1
18: COP 6 3
19: EQ 5 5 6
20: JMF 5 39
21: .local 4 c
22: AFC 7 300
23: COP 4 7
24: COP 7 1
25: COP 8 4
26: NEQ 7 7 8
27: JMF 7 36
28: COP 8 1
29: AFC 9 100
30: ADD 8 8 9
31: COP 1 8
32: .local 5 d
33: AFC 10 500
34: COP 5 10
35: JMP 24
36: .local 5 e
37: AFC 6 600
38: COP 5 6
39: .local 4 f
40: AFC 5 700
41: COP 4 5
42: .local 3 g
43: AFC 4 800
44: COP 3 4
45: COP 4 1
46: AFC 5 300
47: EQ 4 4 5
48: ASRT 4
49: RET
50: .area bootstrap
51: AFC @0 4
52: CALL @0
53: EXIT
