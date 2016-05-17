0: .file test/benoit_funcptr.c
1: .area start
2: JMP 34
3: .function f 1
4: .local 1 a
5: AFC 2 1
6: COP 1 2
7: RET
8: AFC 3 0
9: COP 1 3
10: RET
11: RET
12: .function main 0
13: .local 1 f2
14: COP 2 @0
15: AFC 3 5
16: ADD 2 2 3
17: COP 1 2
18: .stacksize 3
19: .popargs 0
20: AFC 3 22
21: CALL 1
22: COP 4 6
23: PRI 4
24: .stacksize 3
25: .popargs 0
26: AFC 3 28
27: CALL 1
28: COP 4 6
29: AFC 3 0
30: EQ 3 4 3
31: ASRT 3
32: RET
33: .area bootstrap
34: AFC @0 4
35: AFC @1 13
36: CALL @1
37: EXIT
