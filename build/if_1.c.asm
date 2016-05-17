0: .file test/if_1.c
1: .area start
2: JMP 29
3: .function main 0
4: .local 1 a
5: AFC 2 2
6: COP 1 2
7: COP 2 1
8: AFC 3 2
9: EQ 2 2 3
10: JMF 2 24
11: .local 2 b
12: AFC 4 3
13: COP 2 4
14: COP 4 2
15: COP 5 1
16: EQ 4 4 5
17: JMF 4 21
18: .local 3 c
19: AFC 6 4
20: COP 3 6
21: .local 3 d
22: AFC 4 5
23: COP 3 4
24: .local 2 e
25: AFC 3 12
26: COP 2 3
27: RET
28: .area bootstrap
29: AFC @0 4
30: CALL @0
31: EXIT
