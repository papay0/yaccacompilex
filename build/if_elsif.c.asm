0: .file test/if_elsif.c
1: .area start
2: JMP 53
3: .function main 0
4: .local 1 i
5: AFC 2 0
6: COP 1 2
7: .array ok 2 2
8: PTR 2 3
9: COP 5 1
10: AFC 6 2
11: INF 5 5 6
12: JMF 5 51
13: COP 6 1
14: AFC 7 0
15: EQ 6 6 7
16: JMF 6 27
17: COP 7 2
18: AFC 8 0
19: AFC 9 1
20: ADD 7 7 8
21: COPB 7 9
22: COP 8 1
23: AFC 10 0
24: EQ 8 8 10
25: ASRT 8
26: JMP 40
27: COP 5 1
28: AFC 6 1
29: EQ 5 5 6
30: JMF 5 41
31: COP 6 2
32: AFC 7 1
33: AFC 8 1
34: ADD 6 6 7
35: COPB 6 8
36: COP 7 1
37: AFC 9 1
38: EQ 7 7 9
39: ASRT 7
40: JMP 41
41: COP 5 2
42: COP 6 1
43: ADD 5 5 6
44: COPA 5 5
45: ASRT 5
46: COP 5 1
47: AFC 6 1
48: ADD 5 5 6
49: COP 1 5
50: JMP 9
51: RET
52: .area bootstrap
53: AFC @0 4
54: CALL @0
55: EXIT