import sys
import subprocess
tests = ["table.c", "facto.c", "func.c", "while.c"]


def col(n):
    return "\033[0;" + str(n) + "m"

def endcol():
    return "\033[0m"

for test in tests:
    ret = subprocess.call("bin/parser < test/" + test + " > bin/parser_out" + "&& python3 interpreter.py bin/yaccacompilex 0 > bin/interp_out", shell=True)
    if ret == 0:
        print(col(32) + test + ": OK" + endcol())
    else:
        print(col(31) + test + ": FAILED" + endcol())
