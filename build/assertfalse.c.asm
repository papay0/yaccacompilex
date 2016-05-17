0: .file test/assertfalse.c
1: .area start
2: JMP 8
3: .function main 0
4: AFC 1 0
5: ASRT 1
6: RET
7: .area bootstrap
8: AFC @0 4
9: CALL @0
10: EXIT
