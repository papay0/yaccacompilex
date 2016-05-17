0: .file test/if_2.c
1: .area start
2: JMP 23
3: .function main 0
4: .local 1 a
5: AFC 2 1
6: COP 1 2
7: COP 2 1
8: AFC 3 1
9: EQ 2 2 3
10: JMF 2 15
11: .local 2 b
12: AFC 4 2
13: COP 2 4
14: JMP 18
15: .local 2 c
16: AFC 3 3
17: COP 2 3
18: .local 2 d
19: AFC 3 4
20: COP 2 3
21: RET
22: .area bootstrap
23: AFC @0 4
24: CALL @0
25: EXIT
