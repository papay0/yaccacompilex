0: .file test/syntexerr.c
1: .area start
2: .local 0 c
3: AFC 1 5
4: COP @0 1
5: .local 1 main
6: .area bootstrap
7: CALL @777
8: EXIT
