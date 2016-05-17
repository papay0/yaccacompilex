0: .file test/while2.c
1: .area start
2: JMP 41
3: .function main 0
4: .local 1 i
5: AFC 2 0
6: COP 1 2
7: COP 2 1
8: AFC 3 100
9: INF 2 2 3
10: JMF 2 32
11: .local 2 a
12: AFC 4 100
13: COP 2 4
14: .local 3 b
15: AFC 5 200
16: COP 3 5
17: COP 5 2
18: COP 6 3
19: EQ 5 5 6
20: JMF 5 24
21: .local 4 c
22: AFC 7 300
23: COP 4 7
24: .local 4 d
25: AFC 5 400
26: COP 4 5
27: COP 5 1
28: AFC 6 1
29: ADD 5 5 6
30: COP 1 5
31: JMP 7
32: .local 2 e
33: AFC 3 500
34: COP 2 3
35: COP 3 1
36: AFC 4 100
37: EQ 3 3 4
38: ASRT 3
39: RET
40: .area bootstrap
41: AFC @0 4
42: CALL @0
43: EXIT
